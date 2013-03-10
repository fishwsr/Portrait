///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisAlign.cpp -- global image alignment implementation
//
// DESCRIPTION
//  The CVisAligner class aligns images using a global group
//  transformation such as a translation, rotation, affine or planar
//  projective (perspective) transformation.  The transformation is
//  described using a 4x4 homogeneous coordinate transform matrix
//  (the third row and column are ignored -- see VisGeometry.h 
//  for a more detailed description)
//
//  CVisAligner::SingleStep proceeds in the following manner:
//      1. re-sample deformable into warped by current transformation
//      2. compute the image gradients (several options exist)
//      3. accumulate the outer product gradients and
//         gradient/error products, possibly on a coarser grid
//      4. accumulate the Hessian and gradient vectors for the parameters
//      5. solve for the change in transformation and update
//      6. compute the change in pixel positions
//  Optionally, after the initial re-sampling at a given level,
//  an exhaustive search on integer steps may be carried out.
//
// SEE ALSO
//  VisAlign.h        description of class methods and options
//  VisGeometry.h     geometric transformations
//  VisResample.h     global geometric warping
//
// BUGS
//   
//
// DESIGN
//  
//
// HISTORY
//  16-May-96  Richard Szeliski (szeliski) at Microsoft
//  Created.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////

#include "VisImageProcPch.h"
#include "VisImageProc.h"

//KE added 6/3;  Rick Szeliski: removed 8/20
// #define DEBUG_ALIGN

#if defined(_DEBUG) && defined(_AFX)
#include <afxwin.h>         // MFC core and standard components
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

const float max_energy_increase_tolerated = 1.01f;

#ifndef __max
#define __max(a, b)  (((a) > (b)) ? (a) : (b))
#define __min(a, b)  (((a) < (b)) ? (a) : (b))
#endif

//#define PANORAMIC_ROTATION
// #define FOCAL_LENGTH 270
#define FOCAL_LENGTH 468
  
//
//  Number of parameters solved for for each motion model
//  (fewer parameters may actually be free or independent)
//

static int model_size[] = {
//    0, 2, 6, 6, 6, 8, 0
//    0, 2, 6, 6, 6, 8, 3, 8, 0
        0, 
        2, // translation
        6, // + rotation
        6, // + rotation and scale
        6, // + aspect and scale
        6, // affine
        8, // projective
        3, // free rotation with known focal length
        8, // rotation normalized projective
        4, // unknown focal length plus free rotation
        0
};


template <class TImage>
CVisAligner<TImage>::CVisAligner()
{
    // Default parameters
    motion_model    = VIS_ALIGNMENT_TRANSLATION;    // current motion model
    n_levels        = 4;    // number of levels in coarse-to-fine
    band_pass       = 0;    // use band-pass filtered images
    initial_blur    = 2;    // number of initial 121 blur steps
    adjust_bias     = 1;    // recompute bias after each iteration
    robustify       = 0;    // use robust outlier removal
    robust_cutoff   = 3.0f; // multiple of sigma to use as cutoff
    decrease_E      = 1;    // no change if E has not decreased
    use_mask        = 0;    // use masked images or alpha channel
    bound_box_extra = -1;    // extra pixels around bbox edge
    subsample       = 8;    // speedup Hessian and gradient computation
    fixed_Hessian   = 1;    // don't recompute Hessian
    store_error     = 0;    // store error image (for debugging)
    border_width    = 2;    // ignore pixels around border
    max_uv_change   = 2.0f; // maximum allowed motion per step
    min_uv_change   = 0.04f;// motion threshold for decreasing level
    max_inner       = 10;   // maximum inner iterations per step
    max_steps_level = 10;   // maximum steps per level
    search_area2    = 2;    // half-size of correlation search area
    search_subdivide= 4;    // subdivide into n^2 problems
    search_margin   = 0.0;  // winner must beat (0,0) cost by this much
    search_coarsest = 1;    // only perform search at coarsest level
    horizontal_flow = 0;    // constrain flow to be horizontal
    vertical_flow   = 0;    // constrain flow to be vertical
    gridded_xy      = 0;    // snap (x,y) coordinates inside block to grid

    current_level   = 0;
    stopat_level    = 0; 

    bias            = 0.0f; // reset for each new set of images

    m_f0 = m_f1     = 0;    // needs to be set by user before free_rotation

    m_valid_fixed_levels= 0;
    m_valid_float_levels= 0;
    m_valid_Hessian     = 0;
    m_searched_level    = 0;
    m_search_block      = 0;
    m_fixed             = 0;
    m_float             = 0;
    m_warped    = new TImage[ALIGN_MAX_LEVELS];
    m_gradient  = new CVisFloatImage[ALIGN_MAX_LEVELS];
    m_search_uv = new CVisFloatImage[ALIGN_MAX_LEVELS];
}

template <class TImage>
CVisAligner<TImage>::~CVisAligner()
{
     delete m_fixed;
     delete m_float;
     delete [] m_warped;
     delete [] m_gradient;
     delete [] m_search_uv;
}


//
// Set or update (reset) the two images being aligned
//

template <class TImage>
static TImage Blur_121(TImage &img, int blur_steps)
{
    if (blur_steps > 0) {
        TImage blurred(img.Shape());    // new image
        for (int b = 0; b < (blur_steps & -2); b += 2)
            VisConvolveSeparable((b == 0) ? img : blurred, blurred,
                                   VisConvolutionKernel_14641,
                                   VisConvolutionKernel_14641, 1);
        if (b < blur_steps)
            VisConvolveSeparable((b == 0) ? img : blurred, blurred,
                                   VisConvolutionKernel_121,
                                   VisConvolutionKernel_121, 1);
        return blurred;
    } else
        return img;  // make a copy, sharing storage
}


template <class TImage>
void CVisAligner<TImage>::SetFixed(TImage &img)
{
    // Note: initial_blur should be set before this call
    m_fixed_original = Blur_121(img, initial_blur);
    m_valid_fixed_levels = 0;
    m_valid_Hessian = 0;
    current_level = n_levels-1;
    bias = 0.0f;
}

template <class TImage>
void CVisAligner<TImage>::SetDeformable(TImage &img)
{
    // Note: initial_blur should be set before this call
    m_float_original = Blur_121(img, initial_blur);
    m_valid_float_levels = 0;
    m_searched_level = 0;
    current_level = n_levels-1;
    bias = 0.0f;
}

//
// Initialize and read back motion parameters (two forms)
//

template <class TImage>
void CVisAligner<TImage>::SetAlignment(CVisTransform4x4  transform)
{
    m_motion = transform;
}

template <class TImage>
void CVisAligner<TImage>::GetAlignment(CVisTransform4x4 &transform)
{
    transform = m_motion;
}

template <class TImage>
void CVisAligner<TImage>::SetAlignment(CVisTransformChain  geometry)
{
    m_geometry = geometry;
    assert(geometry.size() == 6); 
    m_motion = m_geometry.CompleteTransform(); 
}

template <class TImage>
void CVisAligner<TImage>::GetAlignment(CVisTransformChain &geometry)
{
    geometry = m_geometry;
}

template <class TImage>
void CVisAligner<TImage>::SetTranslation(double uv[2])
{
    m_motion;
    m_motion[0][3] = uv[0];
    m_motion[1][3] = uv[1];
}

template <class TImage>
void CVisAligner<TImage>::GetTranslation(double uv[2])
{
    uv[0] = m_motion[0][3];
    uv[1] = m_motion[1][3];
}

//
// Compute bounding boxes in each image according to current motion
//  These are then used to trim each of the images, to make computations
//  such as filtering and error summing quicker.
// This routine should be called after SetFixed, SetDeformable,
//  and SetAlignment.
//

template <class TImage>
void CVisAligner<TImage>::RecomputeBoundingBoxes()
{
    trimmed_fixed_shape = m_fixed_original.Shape();
    trimmed_float_shape = m_float_original.Shape();
    if (bound_box_extra < 0)
        return;
    
    // Warp float to get bbox on fixed
    CVisTransform4x4 i_motion = m_motion.Inverted();
    CVisShape warped_float = VisResampleShape(m_float_original.Shape(), i_motion);
    warped_float.InflateRect(bound_box_extra, bound_box_extra);

    // this step may not be necessary because of the use of 
    // Decimate(fixed.Shape(), subsample) in InitLevel() below

    // make the image a multiple of subsample
    // perhaps should dilate trimmed_fixed_shape instead
    // need to dilate right-1 and bottom-1 as well? 

    int extraCol, extraRow; 
    extraCol = warped_float.Left() % subsample; 
    extraRow = warped_float.Top() % subsample; 
    warped_float.InflateRect(extraCol, extraRow, 0, 0); 

    trimmed_fixed_shape = (trimmed_fixed_shape & warped_float);
    if (trimmed_fixed_shape.Width() <= 0 || trimmed_fixed_shape.Height() <= 0)
        throw CVisError("Fixed's bounding box is null", eviserrorShape,
                        "CVisAligner::RecomputeBoundingBoxes", 
                        __FILE__, __LINE__);

    // Warp fixed to get bbox on float
    CVisShape warped_fixed = VisResampleShape(m_fixed_original.Shape(), m_motion);
    warped_fixed.InflateRect(bound_box_extra, bound_box_extra);


    extraCol = warped_fixed.Left() % subsample; 
    extraRow = warped_fixed.Top() % subsample; 
    warped_fixed.InflateRect(extraCol, extraRow, 0, 0); 

    trimmed_float_shape = (trimmed_float_shape & warped_fixed);
    if (trimmed_float_shape.Width() <= 0 || trimmed_float_shape.Height() <= 0)
        throw CVisError("Float's bounding box is null", eviserrorShape,
                        "CVisAligner::RecomputeBoundingBoxes", 
                        __FILE__, __LINE__);
}

//
//  Initialize the level images: crop, pre-filter, and reduce
//

template <class TImage>
void CVisAligner<TImage>::InitLevel(int level)
{
    // Check for necessary conditions:  replace with error messages...
    assert(level < ALIGN_MAX_LEVELS);
    assert(m_fixed_original.NPixels() > 0);
    assert(m_float_original.NPixels() > 0);

    // Initialize the pyramids
    if (m_fixed == 0) {
        EVisPyramid pyramid_type = 
            (band_pass) ? pyramid_band_pass : pyramid_low_pass;
        CVisFloatImage &kernel = VisConvolutionKernel_14641;
        EVisPad replication = use_mask ? evispadZero : evispadReplicate;
        int band_pass_bias = 
            (m_fixed_original.PixFmt() == evispixfmtGrayByte ||
             m_fixed_original.PixFmt() == evispixfmtRGBAByte) ? 128 : 0;
        m_fixed = new CVisPyramid<TImage>(ALIGN_MAX_LEVELS,
            pyramid_type, kernel, replication, 
			band_pass_bias);
        m_float = new CVisPyramid<TImage>(ALIGN_MAX_LEVELS,
            pyramid_type, kernel, replication, 
			band_pass_bias);
        m_valid_fixed_levels = 0;
        m_valid_float_levels = 0;
    }

    // Trim the images by the bounding boxes
    RecomputeBoundingBoxes();
    TImage trimmed_fixed = 
        m_fixed_original.SubImage(trimmed_fixed_shape);
    TImage trimmed_float = 
        m_float_original.SubImage(trimmed_float_shape);
    m_fixed->SetBase(trimmed_fixed);
    m_float->SetBase(trimmed_float);

    // Construct the pyramid, if necessary
    band_pass = (band_pass != 0);
    if (level + band_pass > m_valid_fixed_levels) {
        m_fixed->Construct(m_valid_fixed_levels, level);
        m_valid_fixed_levels = level+1;
    }
    if (level + band_pass > m_valid_float_levels) {
        m_float->Construct(m_valid_float_levels, level);
        m_valid_float_levels = level+1;
    }

    // Update the warped through error pyramid shapes...
    TImage& fixed = (*m_fixed)[level];
    CVisShape dilatedShape = CVisShape(fixed.Shape()); 
    dilatedShape.InflateRect(search_area2, search_area2);         
    if (m_warped[level].Shape() != dilatedShape) {
        m_warped[level].Allocate(dilatedShape);
        CVisShape grad_shape = fixed.Shape().Decimated(subsample, true);
        grad_shape.SetNBands(sizeof(SVisGradientBlock) / sizeof(float));
        if (grad_shape.Width() <= 0 || grad_shape.Height() <= 0)
            throw CVisError("Gradient block array is null", eviserrorShape,
                            "CVisAligner::InitLevel", __FILE__, __LINE__);
        m_gradient[level].Allocate(grad_shape);
		m_search_block = (search_subdivide <= 0) ? 
            2 * __max(fixed.Height(), fixed.Width()) :
            __max(__min(fixed.Height(), fixed.Width()) / 
                  search_subdivide, subsample);
        CVisShape ss = fixed.Shape().Decimated(m_search_block, true);
        ss.SetNBands(sizeof(SVisGradientBlock) / sizeof(float));
        if (ss.Width() <= 0 || ss.Height() <= 0)
            throw CVisError("Search block array is null", eviserrorShape,
                            "CVisAligner::InitLevel", __FILE__, __LINE__);
        m_search_uv[level].Allocate(ss, ss);
    }
}


//
//  Perform a 2-D search to find nearest integral displacement
//      Note that using a clipped sqr tbl is incompatible with
//      mean subtraction, so it's not used
//

typedef unsigned short CVisSqTblType;   // could also be BYTE
const int sq_tbl_max_val = 255;         // 15 if using BYTE

static CVisSqTblType *sq_tbl()
{
    static CVisSqTblType tbl[513];
    if (tbl[0] == 0) {
        for (int i = 0; i < 513; i++) {
            int j = i-256, k = __min(abs(j), sq_tbl_max_val);
            tbl[i] = k*k;
        }
    }
    return &tbl[256];
}

static inline void RmsDiffRow(BYTE *p1, BYTE *p2, int nc,
                              int &sum0, float &sum1, float &sum2)
{
    int s0 = 0, s1 = 0, s2 = 0;
    CVisSqTblType *tbl = sq_tbl();
    for (int c = 0; c < nc; c++) {
        int i1 = p1[c], i2 = p2[c], d = i1 - i2;
        s0 += 1, s1 += d, s2 += int(tbl[d]);
    }
    sum0 += s0, sum1 += s1, sum2 += s2;
}

static inline void RmsDiffRow(CVisRGBABytePixel *p1, CVisRGBABytePixel *p2, int nc,
                              int &sum0, float &sum1, float &sum2)
{
    int s0 = 0, s1 = 0, s2 = 0;
    CVisSqTblType *tbl = sq_tbl();
    for (int c = 0; c < nc; c++) {
        if (p1[c].A() != p2[c].A())
            continue;
        int r1 = p1[c].R(), r2 = p2[c].R(), rd = r1 - r2;
        int g1 = p1[c].G(), g2 = p2[c].G(), gd = g1 - g2;
        int b1 = p1[c].B(), b2 = p2[c].B(), bd = b1 - b2;
        s0 += 3, s1 += rd+gd+bd, s2 += int(tbl[rd])+int(tbl[gd])+int(tbl[bd]);
    }
    sum0 += s0, sum1 += s1, sum2 += s2;
}

template <class PixelType>
static inline void RmsDiffRow(PixelType *p1, PixelType *p2, int nc,
                              int &sum0, float &sum1, float &sum2)
{
    int s0 = 0;
    float s1 = 0.0, s2 = 0.0;
    for (int c = 0; c < nc; c++) {
        float d = p1[c] - p2[c];
        s0 += 1, s1 += d, s2 += d*d;
    }
    sum0 += s0, sum1 += s1, sum2 += s2;
}

template <class PixelType>
static void RmsDiff(CVisImage<PixelType>& img1,
                    CVisImage<PixelType>& img2, CVisRect s, int u, int v,
                    SVisGradientBlock& result)
{
    CVisRect s2 = img2.Shape();
	s2.left -= u;
	s2.top -= v;
	s2.right -= u;
	s2.bottom -= v;

    s &= s2;        // block area trimmed against shifted warped image
    int nc = s.Width() * img1.NBands();
    int sum0 = 0; float sum1 = 0.0, sum2 = 0.0;
    for (int r = s.top; r <= s.bottom-1 && nc > 0; r++) {
        PixelType *p1 = &img1.Pixel(s.left  , r  );
        PixelType *p2 = &img2.Pixel(s.left+u, r+v);
        RmsDiffRow(p1, p2, nc, sum0, sum1, sum2);
    }
    if (sum0 > 0) {
        sum2 -= sum1 * sum1 / sum0;
    }
    result.n  = sum0;
    result.e  = sum1;
    result.e2 = sum2;
}

template <class TImage>
static void SearchBlock(SVisGradientBlock& best, SVisGradientBlock& no_motion,
                        TImage& img1, TImage& img2, 
                        CVisRect &block, int nu, int nv, int u_old, int v_old,
                        float better_by,
                        int compute_Hessian_and_subpixel_uv)
{
    // Try all (2n + 1)^2 possible shifts
    // use (0,0) as default instead of (-n, -n)
    int mu = 2*nu+1, mv = 2*nv+1, u0 = -nu, v0 = -nv;
    CVisImage<SVisGradientBlock> resimg(CVisRect(-nu, -nv, mu-nu, mv-nv));
    for (int v = -nv; v <= nv; v++) {
        for (int u = -nu; u <= nu; u++) {
            SVisGradientBlock& result = resimg.Pixel(u + u_old,  v + v_old);
            SVisGradientBlock& winner = resimg.Pixel(u0, v0);
            memset(&result, 0, sizeof(SVisGradientBlock));
            RmsDiff(img1, img2, block, u, v, result);
            result.u = u, result.v = v;

            // Test if new winner
            if (result.e2 * winner.n < winner.e2 * result.n
                || winner.n == 0 && result.n > 0)
                u0 = u, v0 = v;

            // Accumulate (0,0) result for old error estimate
            if (u == 0 && v == 0) {
                no_motion.n  += result.n;
                no_motion.e2 += result.e2;
            }
        }
    }

    // Choose no motion if best error not significantly better
    SVisGradientBlock& winner    = resimg.Pixel(u0, v0);
    SVisGradientBlock& zero_motion = resimg.Pixel( 0,  0);
    if (winner.e2 * zero_motion.n >= 
        zero_motion.e2 * winner.n * (1.0 - better_by))
        u0 = v0 = 0;
    
    // Copy the winning resimg into best (but save x & y first)
    float x = best.x, y = best.y;
    best = resimg.Pixel(u0, v0);
    best.x = x, best.y = y;
    best.u = u0, best.v = v0;
    if (! compute_Hessian_and_subpixel_uv)
        return;

    // Convert into a regular gradient block
    //  see Rick's research notes, Nov 12, 1996
    //  note that this code is deprecated: we use gradient-based matching
    //  after this search to get a better Hessian and sub-pixel flow
    float e0  = best.e2;
    float du1 = __max(resimg.Pixel(u0+(u0 < nu), v0).e2 - e0, 0.0);
    float du2 = __max(resimg.Pixel(u0-(u0 >-nu), v0).e2 - e0, 0.0);
    best.gx2  = du1 + du2;
    float dv1 = __max(resimg.Pixel(u0, v0+(v0 < nv)).e2 - e0, 0.0);
    float dv2 = __max(resimg.Pixel(u0, v0-(v0 >-nv)).e2 - e0, 0.0);
    best.gy2  = dv1 + dv2;
    float e11 = resimg.Pixel(u0+(u0 < nu), v0+(v0 < nv)).e2;
    float e12 = resimg.Pixel(u0-(u0 >-nu), v0+(v0 < nv)).e2;
    float e21 = resimg.Pixel(u0+(u0 < nu), v0-(v0 >-nv)).e2;
    float e22 = resimg.Pixel(u0-(u0 >-nu), v0-(v0 >-nv)).e2;
    best.gx_gy= e11 + e22 - e12 - e21;
    if (best.gx_gy * best.gx_gy > best.gx2 * best.gy2)
        best.gx_gy = (best.gx_gy < 0.0 ? -1.0 : 1.0) *
                     sqrt(best.gx2 * best.gy2); // non-negative det

    // Compute sub-pixel motion estimate
    float uu = u0 + ((best.gx2 > 0.0) ? 0.5 * (du2 - du1) / best.gx2 : 0.0);
    float vv = v0 + ((best.gy2 > 0.0) ? 0.5 * (dv2 - dv1) / best.gy2 : 0.0);
    best.e_gx = uu * best.gx2 + vv * best.gx_gy;
    best.e_gy = vv * best.gy2 + uu * best.gx_gy;
    best.w    = 1.0f;   // for now, compute robust later...
    best.u = uu, best.v = vv;
}

//TVisPixel CVisImage<SVisGradientBlock>::class_pixel_type = VIS_PIXEL_FLOAT;

//
// Accumulate gradients and Hessian elements
//  We use alpha0 and alpha1 to hold the matte (weighting) coefficients
//  for each image.  For unmatted images, this results in two additional
//  multiplies per pixel, but this should be < 5% penalty.
//  If the alpha values are not valid (e.g., one is from framegrabber,
//  alpha = 0, and the other from file, alpha = 1), set
//  is_matted = -1 (will just skip last band).
//

template <class PIXELTYPE>
static void AccumulateBlock(SVisGradientBlock &g, 
                            PIXELTYPE *img0, PIXELTYPE *img1,
                            int row_offset0, int row_offset1,
                            int nrows, int ncols, int nbands,
                            float bias, int accumulate_Hessian,
                            int is_matted, int compute_uv = 0)
{
    // Initialize accumulators
    float sum_n = 0.0f, sum_e = 0.0f, sum_e2 = 0.0f;
    float sum_e_gx = 0.0f, sum_e_gy = 0.0f;
    float sum_gx2 = 0.0f, sum_gx_gy = 0.0f, sum_gy2 = 0.0f;
    const float frac255 = 1.0f/255.0f;
    float alpha0 = 1.0f, alpha1 = 1.0f, alpha01 = 1.0f;
    float wbias = bias, half = 0.5f;

    // Iterate over the pixels in the block
    for (int r = 0; r < nrows; r++) {
        for (int c = 0, b = 0; c < ncols*nbands; c++, b++) {


            // Special code for matte images (alpha in last band)
            //  this assumes each VisCore.has premultiplied alphas
            if (is_matted) {
                if (b == nbands-1) {
                    b = -1;
                    continue;
                } else if (b == 0 && is_matted > 0) {
                    alpha0 = img0[c+nbands-1] * frac255;
                    alpha1 = img1[c+nbands-1] * frac255;
                    if (alpha0 == 0.0f || alpha1 == 0.0f) {
                        c += nbands-1;
                        b = -1;
                        continue;
                    }
                    alpha01 = alpha0 * alpha1;
                    wbias   = alpha01 * bias;
                    half    = 0.5f * alpha01;
                }
            }

            // Compute error and gradients
            float pix0 = alpha1 * img0[c], pix1 = alpha0 * img1[c];
            float ei = pix0 - pix1 - wbias;
            float gxi  = half * (img0[c+nbands] - img0[c-nbands]);
            float gyi  = half * (img0[c+row_offset0] - img0[c-row_offset0]);

            // Accumulate
            sum_n    += alpha01;
            sum_e    += ei;
            sum_e2   += ei * ei;
            sum_e_gx += ei * gxi;
            sum_e_gy += ei * gyi;
            if (accumulate_Hessian) {
                sum_gx2   += gxi * gxi;
                sum_gx_gy += gxi * gyi;
                sum_gy2   += gyi * gyi;
            }

        }
        img0 += row_offset0;
        img1 += row_offset1;
    }

    // Save in the gradient structure
    g.n     = sum_n;
    g.e     = sum_e;
    g.e2    = sum_e2;
    g.e_gx  = sum_e_gx;
    g.e_gy  = sum_e_gy;
    g.gx2   = sum_gx2;
    g.gx_gy = sum_gx_gy;
    g.gy2   = sum_gy2;
    g.w     = (sum_n > 0.0) ? 1.0f : 0.0f;

    // Compute the sub-pixel motion estimate
    float det = sum_gx2 * sum_gy2 - sum_gx_gy * sum_gx_gy; 
    if (g.n > 1.0 && det > 0.0001) {
        float detInv  = 1.0 / det; 
        g.u = (sum_gy2 * sum_e_gx - sum_gx_gy * sum_e_gy) * detInv; 
        g.v = (sum_gx2 * sum_e_gy - sum_gx_gy * sum_e_gx) * detInv; 
    } else
        g.u = g.v = 0.0f;
}

static void AccumulateBlock(SVisGradientBlock &g, 
                            CVisRGBABytePixel *img0, CVisRGBABytePixel *img1,
                            int row_offset0, int row_offset1,
                            int nrows, int ncols, int nbands,
                            float bias, int accumulate_Hessian,
                            int is_matted, int compute_uv)
{
    assert(nbands == 1);    // don't support multiband color
    const int cbands = sizeof(CVisRGBABytePixel) / sizeof(BYTE);
    AccumulateBlock(g, (BYTE *) img0, (BYTE *) img1,
                    row_offset0*cbands, row_offset1*cbands,
                    nrows, ncols, cbands, bias, accumulate_Hessian,
                    is_matted ? 1 : -1, compute_uv);
}

//
//  Accumulate pixel gradients over the whole image
//

template <class TImage>
inline static int VisInterRowSpacing(TImage& img)
{
    return img.CbRow() / img.CbPixel();
}


template <class TImage>
void CVisAligner<TImage>::AccumulatePixelGradients(
        int level, int recompute_Hessian, int search, int compute_uv, int use_old_uv)
{
    TImage &fixed = (*m_fixed)[level];
    TImage &warped= m_warped[level];
    CVisFloatImage &grad  = (search) ? m_search_uv[level] : m_gradient[level];
    int s  = (search) ? m_search_block : subsample;
    SVisGradientBlock no_motion;
    no_motion.n = no_motion.e2 = 0.0f;
    int f_offset = VisInterRowSpacing(fixed);
    int w_offset = VisInterRowSpacing(warped);
    int nbands = fixed.NBands();

    // Compute image shape and trimmed shape (- border_width)
    int bw = (border_width < 1 && search != 2) ? 1 : border_width;
    CVisRect trimmed = fixed.Shape();
    trimmed.InflateRect(-bw, -bw);

    // Iterate over the blocks defined by s
    int width = fixed.Width(), height = fixed.Height();
    float xc = fixed.Left() + 0.5f * width;     // center of image
    float yc = fixed.Top() + 0.5f * height;    // center of image
    for (int br = 0; br < grad.Height(); br++) {
        for (int bc = 0; bc < grad.Width(); bc++) {

            // Get the current gradient block and initialize
            SVisGradientBlock& gb = *(SVisGradientBlock *)
                &grad.Pixel(bc + grad.Left(), br + grad.Top(), 0);
            int old_u = (use_old_uv) ? int(floor(gb.u + 0.5)) : 0;
            int old_v = (use_old_uv) ? int(floor(gb.v + 0.5)) : 0;
            memset(&gb, 0, sizeof(SVisGradientBlock));

            // Compute the valid region
            CVisRect uncropped;
			uncropped.left = fixed.Left() + bc * s;
			uncropped.top = fixed.Top() + br * s;
			uncropped.right = uncropped.left + s;
			uncropped.bottom = uncropped.top + s;
            CVisRect patch = (uncropped & trimmed);

            // Compute the patch center (origin at center of fixed image)
            CVisRect r = (gridded_xy) ? uncropped : patch;
            float x = 0.5f * (r.left + r.right-1) - xc;
            float y = 0.5f * (r.top + r.bottom-1) - yc;
            gb.x = x, gb.y = y;
 
            // Empty patch
            if (patch.Width() <= 0 || patch.Height() <= 0)
                continue;

            if (search) {
                
                // Search over the pixels inside the block
                SearchBlock(gb, no_motion, fixed, warped, patch, 
                            vertical_flow ? 0 : search_area2, 
                            horizontal_flow ? 0 : search_area2, 
                            old_u, old_v, search_margin, search == 2);
                if (search != 2) {
                    old_u += int(gb.u);
                    old_v += int(gb.v);
                }
            }

            // search == 2 is flag for OLD correlation-based Hessian and sub-pixel u-v computation
            if (search != 2) {

                // Adjust patch boundary against warped
				patch.left += old_u;
				patch.top += old_v;
				patch.right += old_u; //erick 
				patch.bottom += old_v;

				patch &= warped.Shape();

				patch.left -= old_u;
				patch.top -= old_v;
				patch.right -= old_u;
				patch.bottom -= old_v;

                // Accumulate derivatives inside the block
                AccumulateBlock(gb, &fixed.Pixel(patch.left, patch.top), 
                                &warped.Pixel(patch.left+old_u, patch.top+old_v),
                                f_offset, w_offset, patch.Height(), patch.Width(), nbands,
                                bias, recompute_Hessian, use_mask, compute_uv);
            }

            // Update the motion and accumulated gradient
            if (old_u != 0 || old_v != 0) {
                gb.u += old_u, gb.v += old_v;
                gb.e_gx = old_u * gb.gx2 + old_v * gb.gx_gy;
                gb.e_gy = old_v * gb.gy2 + old_u * gb.gx_gy;
            }
        }
    }


    // No motion error computation (old_error)
    if (search)
        old_error = float(sqrt(no_motion.e2 / 
                                (no_motion.n + (no_motion.n == 0.0))));
}

//
//  Modify the weights based on the current intensity and/or motion error
//

typedef SVisGradientBlock *pGradientBlock;

struct GBCompare {  // compare gradient block RMS errors
    int operator()(pGradientBlock p1, pGradientBlock p2) {
        return p1->e2 * p2->n <= p2->e2 * p1->n;
    }
};

template <class TImage>
void CVisAligner<TImage>::MakeWeightsRobust(int level, int search)
{
    // Allocate a list block pointers
    CVisFloatImage &grad  = (search) ? m_search_uv[level] : m_gradient[level];
    int n = grad.Height() * grad.Width();
    percent_outliers = 0.0;
    if (n <= 0)
        return;
    pGradientBlock *l = new pGradientBlock[n];

    // Iterate over the blocks to accumulate list of errors
    for (int br = 0, i = 0; br < grad.Height(); br++) {
        for (int bc = 0; bc < grad.Width(); bc++) {
            l[i] = (SVisGradientBlock *)
                &grad.Pixel(bc + grad.Left(), br + grad.Top(), 0);
            i += (l[i]->n > 0.0);    // don't count empty blocks
        }
    }
    n = i;  // actual number of non-empty blocks
    if (n <= 2)	{
		delete l;
        return;
	}

    // Compute the median and a "robust" standard deviation estimate
    pGradientBlock med = VisMedian(GBCompare(), l, n, n/2);
    float med_sigma = sqrt(med->e2 / med->n);
    float robust_sigma = 1.4 * med_sigma;       // see Huber's book

    // Downweight blocks with large sigma's:
    //   what formula to use?  
    //   ... let's try a hard cutoff for now...
    float cutoff_sigma = robust_cutoff * robust_sigma;
    float e2_threshold = cutoff_sigma * cutoff_sigma;
    int n_outliers = 0;
    for (br = 0; br < grad.Height(); br++) {
        for (int bc = 0; bc < grad.Width(); bc++) {
            SVisGradientBlock& gb = *(SVisGradientBlock *)
                &grad.Pixel(bc + grad.Left(), br + grad.Top(), 0);
            if (gb.e2 > e2_threshold * gb.n) {
                gb.w = 0.0;
                n_outliers += 1;
            }
        }
    }
    percent_outliers = n_outliers * 100.0 / n;

    // Clean up
    delete l;
}

//
// Accumulate the derivatives for the incremental deformation parameters
//  (see Rick's notes of May 29, 1996)
// The entries are formed using the relation:
//  dI/dp = [1 0 x 0 y 0 -x^2 -xy]^T [gx]
//          [0 1 0 x 0 y -xy -y^2]   [gy]
// and
//  b = \sum_i w_i e_i dI/dp
//  A = \sum_i w_i dI\dp dI\dp^T
//    = [I xI yI -[x][x y]]^T [G] [I xI yI -[x][x y]]
//                [y]                       [y]
//

// ifdef for now; need to add a menu later 
// this is the code for only updating rotation matrix 
// with known focal length; 
// The entries are 
//  dI/dp = [-xy/f      f+x^2/f -y]^T [gx] =    [a0 a1 a2]^T
//          [-f-y^2/y   xy/f     x]   [gy]      [a3 a4 a5]
// and 
//  b = \sum_i w_i e_i dI/dp 
//  A = \sum_i w_i dI/dP dI/dp^T
//    = [a0^2 gx2 + 2a0a3 gxy + a3^2 gy2        ---                                 ---]
//      [a0a1 gx2 + (a1a3+a0a4)gxy + a3a4 gy2   a1^2 gx2 + 2a1a4 gxy + a4^2 gy2     ---]
//      [a0a2 gx2 + (a2a3+a0a5)gxy + a3a5 gy2   <*>     a2^2 gx2 + 2a2a5 gxy + a5^2 gy2]
//                                               ^
//                                               |
//                               a1a2 gx2 + (a1a5+a2a4)gxy + a4a5 gy2



static void AccumulateParameterGradient(SVisGradientBlock *g,
                                        TVisAlignmentModel motion_model,
                                        double b[8], double f)
{
    // Accumulate the deformation parameter derivatives
    float w = g->w, w_e_gx = w * g->e_gx, w_e_gy = w * g->e_gy;
    float x = g->x, y = g->y;

    if (motion_model == VIS_ALIGNMENT_FREEROTATION || 
             motion_model == VIS_ALIGNMENT_FOCALLENGTH)
    {
        double a0 = -x*y/f; 
        double a1 = f + x*x/f; 
        double a2 = -y; 
        double a3 = -f - y*y/f; 
        double a4 = x*y/f; 
        double a5 = x; 
        
        b[0] += a0 * w_e_gx + a3 * w_e_gy; 
        b[1] += a1 * w_e_gx + a4 * w_e_gy; 
        b[2] += a2 * w_e_gx + a5 * w_e_gy; 
        if (motion_model == VIS_ALIGNMENT_FOCALLENGTH)
            b[3] += x * w_e_gx + y * w_e_gy;
    }
    else 
    {
        b[0] += w_e_gx;
        b[1] += w_e_gy;
        if (motion_model <= VIS_ALIGNMENT_TRANSLATION)
            return;
        b[2] += x * w_e_gx;
        b[3] += x * w_e_gy;
        b[4] += y * w_e_gx;
        b[5] += y * w_e_gy;
        if (motion_model <= VIS_ALIGNMENT_AFFINE)
            return;
        b[6] -= x * (x * w_e_gx + y * w_e_gy);
        b[7] -= y * (x * w_e_gx + y * w_e_gy);
    }
}

static void AccumulateMouseConstraint(double x0, double y0, double x1, double y1,
                                      TVisAlignmentModel motion_model,
                                      double L[2][9], double f)
{
    /*
    if (motion_model == VIS_ALIGNMENT_FOCALLENGTH)
    {
        double C[2][9] = {
            {x0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, x1-x0}, 
            {y0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, y1-y0}};
        memcpy(L, C, 18*sizeof(double)); 
    }
    else */
    if (motion_model == VIS_ALIGNMENT_FREEROTATION || 
             motion_model == VIS_ALIGNMENT_FOCALLENGTH) {
        // See Rick's research notes, 7-Jan-97
        double a0 = -x1*y0/f;
        double a1 = f + x0*x1/f;
        double a2 = -y0;
        double a3 = -f - y0*y1/f;
        double a4 = x0*y1/f;
        double a5 = x0;

        double C[2][9] = {
                {a0, a1, a2, 0.0, 0.0, 0.0, 0.0, 0.0, x1-x0},
                {a3, a4, a5, 0.0, 0.0, 0.0, 0.0, 0.0, y1-y0}};
        memcpy(L, C, 18*sizeof(double)); 

    } else {
        double C[2][9] = {
            {1.0, 0.0,  x0, 0.0,  y0, 0.0, -x0*x1, -y0*x1, x1-x0},
            {0.0, 1.0, 0.0,  x0, 0.0,  y0, -x0*y1, -y0*y1, y1-y0}};
        memcpy(L, C, 18*sizeof(double));
    }
}

static void AccumulateParameterHessian(SVisGradientBlock *g,
                                       TVisAlignmentModel motion_model,
                                       double A[8][8], double f)
{
    // Accumulate the deformation parameter Hessian
    double x = g->x;
    double y = g->y;
    double w = g->w;
    double w_gx2 = w * g->gx2;
    double w_gx_gy = w * g->gx_gy;
    double w_gy2 = w * g->gy2;

    if (motion_model == VIS_ALIGNMENT_FREEROTATION ||
             motion_model == VIS_ALIGNMENT_FOCALLENGTH)
    {
        double a0 = -x*y/f; 
        double a1 = f + x*x/f; 
        double a2 = -y; 
        double a3 = -f - y*y/f; 
        double a4 = x*y/f; 
        double a5 = x; 
        
        A[0][0] += a0*a0*w_gx2 + 2*a0*a3*w_gx_gy + a3*a3*w_gy2; 
        A[1][1] += a1*a1*w_gx2 + 2*a1*a4*w_gx_gy + a4*a4*w_gy2; 
        A[2][2] += a2*a2*w_gx2 + 2*a2*a5*w_gx_gy + a5*a5*w_gy2; 
        
        A[0][1] += a0*a1*w_gx2 + (a0*a4+a1*a3)*w_gx_gy + a3*a4*w_gy2; 
        A[0][2] += a0*a2*w_gx2 + (a0*a5+a2*a3)*w_gx_gy + a3*a5*w_gy2; 
        A[1][2] += a1*a2*w_gx2 + (a1*a5+a2*a4)*w_gx_gy + a4*a5*w_gy2; 
        
        A[1][0] = A[0][1]; 
        A[2][0] = A[0][2]; 
        A[2][1] = A[1][2];

        if (motion_model == VIS_ALIGNMENT_FOCALLENGTH){
            A[3][3] += x*x*w_gx2 + 2*x*y*w_gx_gy + y*y*w_gy2;

            A[0][3] += a0*x*w_gx2 + (a0*y + a3*x)*w_gx_gy + a3*y*w_gy2; 
            A[1][3] += a1*x*w_gx2 + (a1*y + a4*x)*w_gx_gy + a4*y*w_gy2; 
            A[2][3] += a2*x*w_gx2 + (a2*y + a5*x)*w_gx_gy + a5*y*w_gy2; 

            A[3][0] = A[0][3]; 
            A[3][1] = A[1][3]; 
            A[3][2] = A[2][3]; 
        }

    }
    else
    {
        A[0][0] += w_gx2;
        A[0][1] += w_gx_gy;
        A[1][1] += w_gy2;
        if (motion_model > VIS_ALIGNMENT_TRANSLATION) {
            A[0][2] += x * w_gx2;
            A[0][3] += x * w_gx_gy;
            A[1][3] += x * w_gy2;
            A[0][4] += y * w_gx2;
            A[0][5] += y * w_gx_gy;
            A[1][5] += y * w_gy2;
            A[2][2] += x * x * w_gx2;
            A[2][3] += x * x * w_gx_gy;
            A[3][3] += x * x * w_gy2;
            A[2][4] += x * y * w_gx2;
            A[2][5] += x * y * w_gx_gy;
            A[3][5] += x * y * w_gy2;
            A[4][4] += y * y * w_gx2;
            A[4][5] += y * y * w_gx_gy;
            A[5][5] += y * y * w_gy2;
        }
        if (motion_model > VIS_ALIGNMENT_AFFINE) {
            double t1 = x * w_gx2 + y * w_gx_gy;
            double t2 = x * w_gx_gy + y * w_gy2;
            A[0][6] -= x * t1;
            A[0][7] -= y * t1;
            A[1][6] -= x * t2;
            A[1][7] -= y * t2;
            A[2][6] -= x * (x * t1);
            A[2][7] -= x * (y * t1);
            A[3][6] -= x * (x * t2);
            A[3][7] -= x * (y * t2);
            A[4][6] -= y * (x * t1);
            A[4][7] -= y * (y * t1);
            A[5][6] -= y * (x * t2);
            A[5][7] -= y * (y * t2);
            A[6][6] += x * x * (x * t1 + y * t2);
            A[6][7] += x * y * (x * t1 + y * t2);
            A[7][7] += y * y * (x * t1 + y * t2);
            A[7][6]  = A[6][7];
        }
    }
}

static void FillInHessian(double A[8][8])
{
    // Fill in the missing entries in the Hessian
    int i, j;
    // Fill in the LL corner of each 2x2 entry for a[0]..a[5]
    for (i = 1; i < 6; i += 2)
        for (j = i+1; j < 6; j += 2)
            A[i][j] = A[i-1][j+1];
    // Fill in lower-left symmetric half of A
    for (i = 1; i < 8; i++)
        for (j = 0; j < i; j++)
            A[i][j] = A[j][i];
}

//
// Compute the RMS error:
//  This routine is now separate from AccumulateParameterGradients,
//  since we wish to check for the new error without distrubing the
//  parameter gradients.

static float ComputeError(CVisFloatImage& grad)
{
    double sum_n = 0.0, sum_e2 = 0.0;
    for (int br = 0; br < grad.Height() && grad.Width() > 0; br++) {
        SVisGradientBlock *g = (SVisGradientBlock *) grad.PtrToFirstPixelInRow(br+grad.Top());
        for (int bc = 0; bc < grad.Width(); bc++, g++) {
            float w = g->w;
            sum_n  += w * g->n;
            sum_e2 += w * g->e2;
        }
    }
    return (float) sqrt(sum_e2 / (sum_n + (sum_n == 0.0)));
}

//
// Compute the gradients and Hessian w.r.t. the motion parameters
//

template <class TImage>
void CVisAligner<TImage>::AccumulateParameterGradients(int level,
                                      int recompute_Hessian, int search)
{
    CVisFloatImage &grad  = (search) ? m_search_uv[level] : m_gradient[level];
    TImage &fixed = (*m_fixed)[level];
    double f = m_f0 / (1 << level);     // scaled focal length

    // Initialize the gradient and Hessian accumulators
    memset(b, 0, sizeof(b));
    if (recompute_Hessian) {
        memset(A, 0, sizeof(A));
        for (int i = 0; i < 8; i++)
            A[i][i] = 1.0;  // just to be on the safe side...
    }

    // Iterate over the blocks
    double sum_n = 0.0, sum_e = 0.0;
    for (int br = 0; br < grad.Height(); br++) {
        SVisGradientBlock *g = (SVisGradientBlock *) grad.PtrToFirstPixelInRow(br+grad.Top());
        for (int bc = 0; bc < grad.Width(); bc++, g++) {

            // Accumulate the deformation parameter derivatives
            AccumulateParameterGradient(g, motion_model, b, f);
            if (recompute_Hessian)
                AccumulateParameterHessian(g, motion_model, A, f);
            float w = g->w;
            sum_n += w * g->n;
            sum_e += w * g->e;
        }
    }

    // Update the bias
    if (adjust_bias)
        bias += (float) (sum_e / (sum_n + (sum_n == 0.0)));

    // Fill in the missing entries in the Hessian
    int fill_ll = (motion_model != VIS_ALIGNMENT_FREEROTATION &&
                   motion_model != VIS_ALIGNMENT_FOCALLENGTH);
    if (recompute_Hessian && fill_ll)
        FillInHessian(A);
}

//
//  Modify Ax=b by a linear constraint L where L[k] = 1
//      (see Rick's research notes, June 13, 1996)
//

static void ConstrainSystem(double A[8][8], double b[8], double L[9],
                            int k, int n)
{
    // Set A += (Akk L - Ak)L^T + L (Akk L - Ak)^T
    double Akk = A[k][k];
    double Ak[8];
    memcpy(Ak, A[k], n*sizeof(double));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            A[i][j] += (Akk * L[i] - Ak[i])*L[j] +
                       (Akk * L[j] - Ak[j])*L[i];

    // Set b += (Akk L - Ak)d   + L(Akk d - bk)
    double bk = b[k], d = L[8];
    for (i = 0; i < n; i++)
        b[i] += (Akk * L[i] - Ak[i])*d + L[i]*(Akk*d - bk);
}

// Ayache book p.210
//static CVisTransform4x4 RodriguesFormula(double rx, double ry, double rz)
CVisTransform4x4 RodriguesFormula(double rx, double ry, double rz)
{
    CVisTransform4x4 R; 
    CVisTransform4x4 Xn; 
    double rnorm = sqrt(rx*rx + ry*ry + rz*rz); 

    if (rnorm == 0)
        return R; 

    rx /= rnorm; 
    ry /= rnorm; 
    rz /= rnorm; 

    double sintheta;
    if (rnorm < 1.0)
        sintheta = rnorm; 
    else 
        sintheta = 0.0; 

    double theta = asin(sintheta); 
    double costheta = 1.0 - cos(theta); 

    int i; 
    for (i=0; i<4; i++)
        Xn[i][i] = 0;

#ifdef OLD_4x4
    Xn[0][1] = -rz; 
    Xn[0][3] = ry; 
    Xn[1][0] = rz; 
    Xn[1][3] = -rx; 
    Xn[3][0] = -ry; 
    Xn[3][1] = rx; 
#else
    Xn[0][1] = -rz; 
    Xn[0][2] = ry; 
    Xn[1][0] = rz; 
    Xn[1][2] = -rx; 
    Xn[2][0] = -ry; 
    Xn[2][1] = rx; 
#endif //OLD_4x4

    CVisTransform4x4 Xn2 = Xn * Xn; 

    int j; 
    for (i=0; i<4; i++)
        for (j=0; j<4; j++)
            R[i][j] += sintheta * Xn[i][j] + costheta * Xn2[i][j]; 

    return R; 
}

//  VisMatrixSolveFreeRotation
static CVisTransform4x4 VisMatrixSolveFreeRotation(
        double A[8][8], double b[8], float step_size) // 
{
    // Form the CVisDMatrix and CVisDVector, and call the linear solver
    //  We perform an explict copy in case VisMatrixSolveSPD is destructive
    int size = model_size[VIS_ALIGNMENT_FREEROTATION];

    // has some numerical problem where eigenvalues are negative
    /*
    CVisDMatrix Am(8, 8);
    memcpy(&Am[0][0], A[0], 8*8*sizeof(double));
    CVisDVector bv(8);
    memcpy(&bv[0], b, 8*sizeof(double));
    CVisDVector m(8);
    m = 0.0;
    assert(VisMatrixIsSymmetric(Am));
 
    VisMatrixSolveSPD(Am, bv, m, size);
    */

    CVisDMatrix Am(size, size); 
    for (int i=0; i<size; i++)
        for (int j=0; j<size; j++)
            Am[i][j] = A[i][j]; 
    CVisDVector bv(size); 
    for (i=0; i<size; i++)
        bv[i] = b[i]; 
    CVisDVector m(size); 
    m=0.0; 
    VisDMatrixSolve(Am, bv, m); 

    // Form the incremental motion array
    CVisTransform4x4 dM;
    double s = step_size;


    dM = RodriguesFormula(s*m[0], s*m[1], s*m[2]); 
    return dM;    
}


//  VisMatrixSolveFocalLength
//  only e1 is adjust for now 
static CVisTransform4x4 VisMatrixSolveFocalLength(
        double A[8][8], double b[8], float step_size)
{
    int size = model_size[VIS_ALIGNMENT_FOCALLENGTH];

    CVisDMatrix Am(size, size); 
    for (int i=0; i<size; i++)
        for (int j=0; j<size; j++)
            Am[i][j] = A[i][j]; 
    CVisDVector bv(size); 
    for (i=0; i<size; i++)
        bv[i] = b[i]; 
    CVisDVector m(size); 
    m=0.0; 
    VisDMatrixSolve(Am, bv, m); 

    // Form the incremental motion array
    CVisTransform4x4 dM;
    double s = step_size;

    dM = RodriguesFormula(s*m[0], s*m[1], s*m[2]); 

    dM[3][3] = s*m[3]; // this is the estimated focal length increment
    return dM;    
}


//
//  VisDMatrixSolve for the incremental motion parameters
//

static CVisTransform4x4 VisMatrixSolveForMotion(double A[8][8], double b[8],
                                    TVisAlignmentModel motion_model,
                                    float step_size)
{
    //  Remove two degrees of freedom from rotation, similarity and aspect motions
    if (motion_model == VIS_ALIGNMENT_ROTATION ||
        motion_model == VIS_ALIGNMENT_SIMILARITY) {
        double L[2][9] = {
            {0.0, 0.0, 1.0, 0.0, 0.0,-1.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0}};
        ConstrainSystem(A, b, L[0], 2, model_size[motion_model]);
        ConstrainSystem(A, b, L[1], 3, model_size[motion_model]);
    }
    if (motion_model == VIS_ALIGNMENT_ASPECT) {
        double L[2][9] = {
            {0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0},
            {0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}};
        ConstrainSystem(A, b, L[0], 3, model_size[motion_model]);
        ConstrainSystem(A, b, L[1], 4, model_size[motion_model]);
    }
 
    // Make sure diagonal has no zero entries (added by szeliski, 06-Aug-98)
    for (int i = 0; i < 8; i++)
        A[i][i] += (A[i][i] <= 0.0);

    // Form the CVisDMatrix and CVisDVector, and call the linear solver
    //  We perform an explict copy in case VisMatrixSolveSPD is destructive
    int size = model_size[(int) motion_model];
    CVisDMatrix Am(8, 8);
    memcpy(&Am[0][0], A[0], 8*8*sizeof(double));
    CVisDVector bv(8);
    memcpy(&bv[0], b, 8*sizeof(double));
    CVisDVector m(8);
    m = 0.0;
    assert(Am.IsSymmetric());

//    VisDMatrixSolveSPD(Am, bv, m, size); 
	VisDMatrixSolveSPD(Am, bv, m);	//KE: size=8;
    // Form the incremental motion array
    CVisTransform4x4 dM;
    double s = step_size;

    dM[0][0] += s*m[2], dM[0][1] += s*m[4], dM[0][3] += s*m[0];    
    dM[1][0] += s*m[3], dM[1][1] += s*m[5], dM[1][3] += s*m[1];    
    dM[3][0] += s*m[6], dM[3][1] += s*m[7];    

    //  Make sure that dM is a rigid motion
    if (motion_model == VIS_ALIGNMENT_ROTATION) {
        double d0 = 0.5*(dM[0][0] + dM[1][1]);  // avg 1+c
        double d1 = 0.5*(dM[0][1] - dM[1][0]);  // avg s
        double h = _hypot(d0, d1);
        if (h == 0.0)
            d0 = 1.0, d1 = 0.0;
        else
            d0 /= h, d1 /= h;
        dM[0][0] = d0, dM[1][1] =  d0;
        dM[0][1] = d1, dM[1][0] = -d1;
    }

    return dM;
}


//
// Control the iteration
//

static void AdjustMotion(CVisTransform4x4& m, int level, int post_process)
{
    // Adjust the motion matrix by the pyramid level
    if (level == 0)
        return;
    double s = 1.0, f = (post_process) ? 2.0 : 0.5;
    for (int i = 0; i < level; i++, s *= f);
    m[0][3] *= s, m[1][3] *= s;
    m[3][0] /= s, m[3][1] /= s;
}

static void AdjustMotionGeometry(CVisTransformChain& g, int level, int post_process)
{
    // Adjust the motion geometry chain TVRRinvVinvTinv by the pyramid level 
    if (level == 0)
        return; 
    double s = 1.0, f = (post_process) ? 2.0 : 0.5;
    for (int i = 0; i < level; i++, s *= f);

    assert(g.size() == 6); 

    CVisTransform4x4& T    = g[0]; 
    CVisTransform4x4& V    = g[1];

    CVisTransform4x4& Vinv = g[4]; 
    CVisTransform4x4& Tinv = g[5]; 

    //R and Rinv don't change with level 
    T[0][3] *= s; T[1][3] *= s; 
    Tinv[0][3] *= s; Tinv[1][3] *= s; // note here still *s because Tinv = -T 
    // Is adjusting V right? 
    V[0][0] *= s; V[1][1] *= s; 
    Vinv[0][0] /= s; Vinv[1][1] /= s; 
}


static void UpdateModelParameters(int interactive, CVisTransform4x4& motion, 
                                  CVisTransformChain& geometry, TVisAlignmentModel model, 
                                  double c[2], double zoom, 
                                  double A[8][8], double b[8], double step_size)
{
    if (model == VIS_ALIGNMENT_FREEROTATION ||
        model == VIS_ALIGNMENT_FOCALLENGTH) {

        assert(geometry.size() == 6); 

        // update VRV format only for FREEROTATION (+ FOCALLENGTH)

        // extract current R from known f
        CVisTransform4x4 &rotation = geometry[3]; 
        CVisTransform4x4 Rview = geometry[2]; 

        // extract current V1
        /*
        CVisTransform4x4 &V1 = geometry.Nth(4).ThisTransform(); 
        */
        if (model == VIS_ALIGNMENT_FREEROTATION) {
            // get the delta R
            CVisTransform4x4 dR = VisMatrixSolveFreeRotation(A, b, step_size); 
            // put the new R into the chain
            //rotation = dR * rotation; 
            rotation = Rview.Inverted() * dR * Rview * rotation; 
        }
#if 0
        else if (model == VIS_ALIGNMENT_FOCALLENGTH) {
            // which should be adjusted? f0 or f1???
            if (interactive)
            {
                // get the delta R
                CVisTransform4x4 dR = VisMatrixSolveFreeRotation(A, b, 1.0f); 
                // put the new R into the chain
                rotation = dR * rotation;  

                // put the new V into the geometry chain
                V1[0][0] = f1*zoom; 
                V1[1][1] = f1*zoom; 
            }
            else
            {
                // get dR, and delta V1, i.e. e1
                CVisTransform4x4 dR = VisMatrixSolveFocalLength(A, b, step_size); 
                double e1 = dR[3][3];
                // modify dR 
                dR[3][3] = 1.0; 
                // put the new R back to the chain
                rotation = dR * rotation; 
                // put the new V into the geometry chain
                V1[0][0] *= (1.0 + e1); 
                V1[1][1] *= (1.0 + e1); 
            }
        }
#endif
        motion = geometry.CompleteTransform(); 

    }
    else // all other motion models using projective 8-parameters or fewer
    { // old code 

        CVisTransform4x4 t1;
		t1.Translate(-c[0], -c[1]); 
        CVisTransform4x4 t2;
		t2.Translate(c[0], c[1]); 
        // transform the T in original coordinate to centered coordinate
        CVisTransform4x4 center_motion = t1*motion*t2; 

        // in centered coordinate
        CVisTransform4x4 delta_motion = 
            VisMatrixSolveForMotion(A, b, model, step_size);

        CVisTransform4x4 new_motion = delta_motion * center_motion; 

        // for models like affine or translational, we can't get focal length
        if (model == VIS_ALIGNMENT_PROJ_ROTNORM ){
            double mf0; 
            double mf1; 
            CVisTransform4x4 R;
            if (VisGuessRotationAndF(new_motion, R, mf0, mf1, 0, 0)) 
                //AfxMessageBox("Can not estimate focal length!"); 
            //else 
                VisProjFromRotAndF(R, new_motion, mf0, mf1, 0, 0); 
        }

        // back to the original 
        motion = t2 * new_motion * t1;
        // is this necessary? 
        geometry = CVisTransformChain(motion); 
    }

}


//
// VisDMatrixSolve the equations for the free motion parameters increment
// and update the motion matrix
//
//  Note that if we want to make the computation centered at the
//  fixed image center (-0.5f * width above...), then we need
//  pre-translate the motion before the new_motion computation as
//  well as post-translating it (see ComputeInteractiveDeformation)
//
//  Experimentally, it doesn't seem to make any difference if we
//  translate or not...
//  But it matters for freerotation case

template <class TImage>
void CVisAligner<TImage>::UpdateParameters(int level, float step_size)
{
    // Compute the old corners
    CVisShape sh = m_warped[level].Shape();
    CVisVector4 uvw[2][4];
    VisResampleCorners(sh, m_motion, uvw[0]);

    // Pre-translate to center of fixed image
    TImage &fixed = (*m_fixed)[level];
    double c[2] = {0.5 * fixed.Width(), 0.5 * fixed.Height()};

    UpdateModelParameters(0, m_motion, m_geometry, motion_model, 
                          c, 1.0, A, b, step_size);

    // Compute the new corners
    VisResampleCorners(sh, m_motion, uvw[1]);
    double uv_sum = 0.0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            double diff = uvw[1][i][j] - uvw[0][i][j];
            uv_sum += diff * diff;
        }
    }
    uv_correction = (float) sqrt(uv_sum / 8.0);
}


// compute flow error given fixed and float image and transformation
// at lowest (finest) level 
template <class TImage>
double CVisAligner<TImage>::CalcError()
{
    // Set up the required structures: fixed and float images
    try {
        InitLevel(0);
    } catch(CVisError& ) {   // no intersection between fixed and float
        RMS_error       = -1.0f;
        uv_correction   = 0.0f;
        return (RMS_error);
    }

    // float to warped using M
    VisResample((*m_float)[0], m_warped[0],
                m_motion, VIS_INTERP_LINEAR, 1);

    // Just reuse the code for now; can use a simplified function later
    AccumulatePixelGradients(0, 0, 0, 0, 0);

    RMS_error = ComputeError(m_gradient[0]);
    return (RMS_error); 
}


template <class TImage>
void CVisAligner<TImage>::SingleStep(int level)
{
    // collapse m_motion from m_geometry if changed from TVRVT to 8-parameters
 
    if (motion_model != VIS_ALIGNMENT_FREEROTATION &&
        motion_model != VIS_ALIGNMENT_FOCALLENGTH)
    {
        if (m_geometry.size() > 1)
        {
            m_motion = m_geometry.CompleteTransform(); 
        }
    }

    // Set up the required structures

    try {
        InitLevel(level);
    } catch(CVisError& ) {   // no intersection between fixed and float
        RMS_error       = -1.0f;
        uv_correction   = 0.0f;
        return;
    }

    // Adjust the transformation by the level
   
    if (motion_model == VIS_ALIGNMENT_FREEROTATION ||
        motion_model == VIS_ALIGNMENT_FOCALLENGTH){
        AdjustMotionGeometry(m_geometry, level, 0);

        //CVisTransform4x4 T = VisMatrixInverse(m_geometry.Nth(2).ThisTransform());
        //CVisTransform4x4 V = VisMatrixInverse(m_geometry.Next().ThisTransform());
        //m_motion = T * V * m_geometry.CompleteTransform(); 
        assert(m_geometry.size() == 6); 
        m_motion = m_geometry.CompleteTransform(); 
    }
    else 
        AdjustMotion(m_motion, level, 0);
    
    // Hessian and pre-warped images are valid
    int search_me = (search_area2 > 0) && !(m_searched_level & (1 << level))
        && (! search_coarsest || level == n_levels-1);
    int recompute_Hessian = !(fixed_Hessian &&
        (m_valid_Hessian & (1 << level))) || search_me;

    // Iterate until error has decreased or change is too small
    CVisTransform4x4 old_motion = m_motion;
    CVisTransformChain old_geometry = m_geometry; 
    for (int iter = 0; /* see break conditions below */; iter++) {

        int search_now = search_me && iter == 0;    // only first time

        // Resample the float image, compute error
        if ((recompute_Hessian || iter > 0 || search_me) &&
            uv_correction <= max_uv_change) {

            // Resample the float image
            VisResample((*m_float)[level], m_warped[level],
                          m_motion, VIS_INTERP_LINEAR, 1);
#ifdef DEBUG_ALIGN
            char filename[1024];
            sprintf(filename, "C:\\temp\\fix%d-%d.tga", level, iter);
            (*m_fixed)[level].WriteFile(filename);
            sprintf(filename, "C:\\temp\\flt%d-%d.tga", level, iter);
            (*m_float)[level].WriteFile(filename);
            sprintf(filename, "C:\\temp\\wrp%d-%d.tga", level, iter);
            m_warped[level].WriteFile(filename);
#endif
            
            // Accumulate the pixel gradients and Hessians
            AccumulatePixelGradients(level, recompute_Hessian, search_now, 0, 0);
            if (robustify)
                MakeWeightsRobust(level, search_now);

            // Compute the (new) error
            RMS_error = ComputeError(search_now ? m_search_uv[level] :
                                     m_gradient[level]);
        }

        // If error has decreased, terminate loop
        if (iter == 0) {
            if (! search_me)
                old_error = RMS_error;
        } else if (RMS_error < old_error && 
//                   (search_me || uv_correction <= max_uv_change))
                    uv_correction <= max_uv_change)
            break;

        // We have run inner loop too many times
        else if (iter > max_inner ||
                 uv_correction < min_uv_change) {
            if (decrease_E && (uv_correction > max_uv_change ||
                    RMS_error > old_error * max_energy_increase_tolerated)) {
                m_geometry = old_geometry;  // don't move
                m_motion = old_motion;      // don't move
                RMS_error = old_error;
                uv_correction = 0.0f;
            }
            break;
        }

        // Accumulate the parameter gradient and Hessian
        if (iter == 0) {
            AccumulateParameterGradients(level, recompute_Hessian, search_now);
            if (search_now) {
                m_searched_level |= (1 << level);
                m_valid_Hessian &= ~(1 << level);
            } else
                m_valid_Hessian |= (1 << level);
        } else // RMS did not decrease, restore starting
        {
            m_motion = old_motion;  
            m_geometry = old_geometry; 
        }

        // VisDMatrixSolve for the parameter (corner) update
        UpdateParameters(level, 1.0f / (1 << iter));

        if (motion_model == VIS_ALIGNMENT_FREEROTATION ||
            motion_model == VIS_ALIGNMENT_FOCALLENGTH){
            m_motion = m_geometry.CompleteTransform(); 
        }


#if OLD
        // If using search, terminate now (don't check for energy decrease)
        if (search_me)
            break;
#endif
    }

    // converting m_geometry and m_motion back to level 0
    // Adjust the transformation by the level
    
    if (motion_model == VIS_ALIGNMENT_FREEROTATION ||
        motion_model == VIS_ALIGNMENT_FOCALLENGTH){
        AdjustMotionGeometry(m_geometry, level, 1);

        m_motion = m_geometry.CompleteTransform(); 
    }
    else 
        AdjustMotion(m_motion, level, 1);
    
}

template <class TImage>
int CVisAligner<TImage>::SingleStep()
{
    // Take a step
    SingleStep(current_level);

    // Check if converged
    int converged = uv_correction < min_uv_change;
/*
    if (stopat_level >= n_levels)
    {
        stopat_level = n_levels - 1; 
    }
*/
    if (converged && current_level == stopat_level)
        return 1;   // all done
    current_level -= converged;
    return 0;       // not done yet;
}
    
template <class TImage>
void CVisAligner<TImage>::RunMultilevel(int max_iterations)
{
    current_level = n_levels-1;
    for (int i = 0; i < max_iterations; i++)
        if (SingleStep())
            break;
}

template <class TImage>
void CVisAligner<TImage>::GetHessian(double hessian[8][8]){
	for(int i = 0; i < 8; i++)
		for(int j = 0; j< 8; j++)
			hessian[i][j] = A[i][j];
}
template <class TImage>
void CVisAligner<TImage>::GetB(double error_b[8]){
	for(int i = 0; i < 8; i++)
			error_b[i] = b[i];
}

//
//  ComputeInteractiveDeformation:
//      Compute the new motion matrix resulting from moving p0 to p1,
//      while trying to minimize the corner motion.
//  This can be used for interactively positioning a tile.
//
//  See Rick's notes of June 12, 1996 for more details.
//
//  02-Dec-97: changed so that far away corners stay put
//

//CVisTransform4x4 ComputeInteractiveDeformation(CVisTransform4x4 motion,
CVisTransformChain ComputeInteractiveDeformation(CVisTransform4x4 motion,
    CVisShape sh, TVisAlignmentModel motion_model,
    CVisVector4 p0, CVisVector4 p1, double f, double zoom, 
    CVisTransformChain view)
{
    if (motion_model == VIS_ALIGNMENT_TRANSLATION)
        return motion.Translated(p1[0]-p0[0], p1[1]-p0[1]);

    double c[2] = {0.5 * (sh.Left() + sh.right-1), 
                   0.5 * (sh.top + sh.bottom-1)};
    CVisTransform4x4 t1;
	t1.Translate(-c[0], -c[1]); 
    CVisTransform4x4 t2;
	t2.Translate(c[0], c[1]); 
    // transform the T in original coordinate to centered coordinate
    // not necessary for geometry based
    CVisTransform4x4 tra_motion = t1*motion*t2;
    // shift the shape the same way we shift p0 and p1 below
    sh = CVisShape(int(-c[0]), int(-c[1]), int(-c[0])+sh.Width(), int(-c[1])+sh.Height());

    // VisProjectVector the four corners of the image (and subtract center)
    CVisVector4 uv[4];
    VisResampleCorners(sh, tra_motion, uv);

    // Compute the "normalized" coordinates of mouse within the tile
    double x0 = p0[0]-c[0], x1 = p1[0]-c[0];
    double y0 = p0[1]-c[1], y1 = p1[1]-c[1];
    CVisVector4 v0(x0, y0);
    CVisTransform4x4 inv_motion = tra_motion.Inverted();
    CVisVector4 u0 = inv_motion.ProjectVector(v0);

    // Compute corner weights using a logistic function
    static double slope = 2.0;
    double ux = slope * 2.0 * u0[0] / sh.Width();
    double uy = slope * 2.0 * u0[1] / sh.Height();
    double wx = 0.5 * (1.0 + tanh(ux));
    double wy = 0.5 * (1.0 + tanh(uy));

    // Accumulate the Hessian for the 4 corners
    double A[8][8], b[8];
    memset(A, 0, 64*sizeof(double));
    memset(b, 0, 8*sizeof(double));
    SVisGradientBlock g = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                           0.0, 0.0, 1.0f};
    for (int i = 0; i < 4; i++) {
        g.x = (float) uv[i][0], g.y = (float) uv[i][1];
        g.w = __max(((i < 2) ? wy : 1.0 - wy),
                    ((i == 0 || i == 3) ? wx : 1.0 - wx));
        AccumulateParameterHessian(&g, motion_model, A, f);
    }
    FillInHessian(A);
    assert(CVisDMatrix(8, 8, A[0]).IsSymmetric());

    // Form the two linear equations for the mouse motion constraint
    //  and add them to the Hessian (see Rick's notes, June 13, 1996)
    double L[2][9];
    AccumulateMouseConstraint(x0, y0, x1, y1, motion_model, L, f);
    
    int nV = model_size[motion_model]; 
    if (nV == 4) nV = 3; // do not solve for zoom interactively

    ConstrainSystem(A, b, L[0], 0, nV);
    assert(CVisDMatrix(8, 8, A[0]).IsSymmetric());
    ConstrainSystem(A, b, L[1], 1, nV);
    assert(CVisDMatrix(8, 8, A[0]).IsSymmetric());

    int level = 0;

    CVisTransformChain deform_geometry = CVisTransformChain(); 
    CVisTransform4x4 deform_motion = CVisTransform4x4(motion); 

    if (motion_model == VIS_ALIGNMENT_FREEROTATION || 
        motion_model == VIS_ALIGNMENT_FOCALLENGTH)
    {
        deform_geometry = ExtractTVRRVT(deform_motion, c[0], c[1], f, f*zoom, 
                    view); 
    }

    UpdateModelParameters(1, deform_motion, deform_geometry, motion_model, 
                          c, zoom, A, b, 1.0);

    // return deform_motion;
    if (motion_model != VIS_ALIGNMENT_FREEROTATION &&
        motion_model != VIS_ALIGNMENT_FOCALLENGTH)
    {
        deform_geometry = CVisTransformChain(deform_motion); 
    }

    return deform_geometry; 

}




//
//  Projective to panoramic model transformation:
//  given a homography (8-parameter perspective transformation)
//  recover the two focal lengths and rotation matrix.
//


//
// Explicit template instantiation
//

#pragma warning(disable : 4660)  // template class already instantiated

//KE template CVisAligner<CVisByteImage>;
template CVisAligner<CVisRGBAByteImage>;
// template CVisAligner<CVisIntImage>;
// template CVisAligner<CVisFloatImage>;

#pragma warning(default : 4660)

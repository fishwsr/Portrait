///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisComposite.cpp -- image composite implementation
//
// DESCRIPTION
//  The CVisComposite class is used to assemble the collection
//  of images in an ImageCollection into a single image for display.
//
// SEE ALSO
//  VisComposite.h      composite class and functionality description
//
// BUGS
//   
//
// DESIGN
//  
//
// HISTORY
//  06-Jun-96  Richard Szeliski (szeliski) at Microsoft
//  Created.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////

#include "VisImageProcPch.h"
#include "VisImageProc.h"

#define Clip255(in) __max(0, __min(255, int(in)))
#ifndef M_PI 
#define M_PI    3.1415926536
#endif // M_PI

struct ScaleBy
{
    const float scale;
    ScaleBy(float s) : scale(s) {};
    void operator()(float& out, float& in)
      { out = in * scale; }
};

struct FloatToByte {
        void operator()(CVisBytePixel& out, CVisFloatPixel& in)
        { 
            out = (unsigned char) in; 
        }
};


//
// Constructor and destructor
//

template<class TPixel>
CVisComposite<TPixel>::CVisComposite(EVisSequence evissequence)
  : CVisCollection<TPixel>(evissequence)
{
    m_operator = composite_feather;
    m_feather_distance = 255;
    m_overlay = VIS_COMPOSITE_VIEW_OVERLAY;
    m_zoom = 1.0;
    m_bounding_box = 2.0f;
    m_interpolation = VIS_INTERP_LINEAR;
    m_max_pixel_stretch = 4.0f;

//	m_feather.SetReadIfEmpty(false);

    m_crop_region = CVisRect(0, 0, 0, 0);
    m_n_accumulated = 0;
    m_i_not_accumulated = -1;
    m_valid_layers = 0;             // layers are valid
    m_always_valid = 0;             // make these layers always valid
    m_blink = 0;
    m_background_gray_level = 255;  // currently not settable
    m_neighbor_frames = 0;
    m_skip_frames = 0;
    m_save_layers = 0;              // save warped layers in m_layers
    m_generation = 0;


    // for pyramid blending
    // need to fix pyramid code for special case when pyramid level is 1
    m_blend_level = 7;              // 10 is used for 513x513 
    // also depends on the image size width/height > 2^level
}

template<class TPixel>
CVisComposite<TPixel>::~CVisComposite()
{
}


template<class TPixel>
void CVisComposite<TPixel>::ResetCurrentFrameGeometry()
{
    if (IsEmpty())
        return;
    CVisTransformChain &geom = Frame().TransformChain();
#ifdef MARCH10
    geom = m_view_geometry.CompleteTransform();
#else
    geom = m_view_geometry; 
#endif // MARCH10

    Invalidate();
}

template<class TPixel>
void CVisComposite<TPixel>::PredictLastFrameGeometry(int n_last)
{
    if (n_last < 1 || NFrames() < 2)
        return;
    CVisTransformChain &next = (*this)[NFrames()-1].TransformChain();
    CVisTransformChain &last = (*this)[NFrames()-2].TransformChain();
    if (n_last == 1 || NFrames() < 3)
        next = last;
    else {
        CVisTransformChain &prev = (*this)[NFrames()-3].TransformChain();
        // compute change in transform, and predict ...
        if (last.size() != 3)
        {
            CVisTransform4x4 tl = last.CompleteTransform();
            CVisTransform4x4 tp = prev.CompleteTransform();
            CVisTransform4x4 pl = tl * tp.Inverted();
            CVisTransform4x4 tn = pl * tl;
            next = tn;
        }
        else
        {
            CVisTransform4x4 Rtl = last[0]; 
            CVisTransform4x4 Rtp = prev[0]; 
            next = last; 
            CVisTransform4x4 &Rinv = next[0]; 
            Rinv = Rtl * Rtp.Inverted() * Rtl; 
        }
    }
}

template<class TPixel>
void CVisComposite<TPixel>::SetViewGeometry(const CVisTransformChain& geometry)
{
    m_view_geometry = geometry;
    Invalidate();
} 

//
// Overridden collection operations:
//  these are needed to keep the m_feather collection consistant
//

#ifdef TAKEOUT
template<class TFrame>
void CVisComposite<TFrame>::Insert(TFrame &frame, int index, int new_geometry)
{
    CVisCollection<TFrame>::Insert(frame, index, new_geometry);
    m_feather.Insert("");   // corresponding feather image (unallocated)
}


template<class TFrame>
void CVisComposite<TFrame>::Insert(const char *filename, int index)
{
    CVisCollection<TFrame>::Insert(filename, index);
    // because this will call Intert(frame, index, new_geometry)
    // m_feather.Insert("");   // corresponding feather image (unallocated)
}

template<class TFrame>
void CVisComposite<TFrame>::Raise(int index, int to_top)
{
    CVisCollection<TFrame>::Raise(index, to_top);
    m_feather.Raise(index, to_top);
}

template<class TFrame>
void CVisComposite<TFrame>::Lower(int index, int to_top)
{
    CVisCollection<TFrame>::Lower(index, to_top);
    m_feather.Lower(index, to_top);
}

template<class TFrame>
void CVisComposite<TFrame>::Delete(int index)
{
    CVisCollection<TFrame>::Delete(index);
    m_feather.Delete(index);
}

template<class TFrame>
void CVisComposite<TFrame>::DeleteAll()
{
    CVisCollection<TFrame>::DeleteAll();
    m_feather.DeleteAll();
}
#endif // TAKEOUT

//
// File I/O
//  Write the composite of images into a descriptive text file,
//  and optionally write out (back) the constituent files.
//  Note that only the metadata (view parameters, etc), differs from a collection
//

template<class TPixel>
void CVisComposite<TPixel>::ReadMetaData(FILE *stream)
{
    char line[1024];
    fgets(line, 1024, stream); // read rest of line
    if(strcmp(line, " View geometry:\n") != 0)
        return;
    m_view_geometry.Read(stream);
    int frame;
    if (fscanf(stream, "\n# Current frame:\n%d\n", &frame) != 1)
        return;
    if (0 <= frame && frame < NFrames())
        SetCurrentFrame(frame);
}

template<class TPixel>
void CVisComposite<TPixel>::WriteMetaData(FILE *stream)
{
    fprintf(stream, "# View geometry:\n");
    m_view_geometry.Write(stream, 1);
    fprintf(stream, "\n# Current frame:\n%d\n", CurrentFrame());
}

//
// Layer geometry (include base view geometry)
//

static void VisFixTransform4x4(CVisTransform4x4& c)
{
    // Special code to set tiny elements to 0 (make identity really identity)
    const double epsilon = 1.0e-9;
    for (int j = 0; j < 4; j++) {
        for (int k = 0; k < 4; k++) {
            int c_jk = int(floor(c[j][k] + 0.5));
            if (fabs(c[j][k] - c_jk) < epsilon)
                c[j][k] = c_jk;
        }
    }
}

template<class TPixel>
CVisTransform4x4 CVisComposite<TPixel>::LayerTransform(int i)
{
    CVisTransformChain &geom = (*this)[i].TransformChain();
    CVisTransform4x4 layer_t = geom.CompleteTransform();
    CVisTransform4x4 base_t  = m_view_geometry.CompleteTransform().Inverted();
    CVisTransform4x4 layer_c = base_t * layer_t;
    VisFixTransform4x4(layer_c);
    return layer_c;
}

template<class TPixel>
void CVisComposite<TPixel>::SetLayerTransform(int i, CVisTransform4x4 layer_c)
{
    // Input transform has m_view_geometry factored in
    CVisTransformChain &geom = (*this)[i].TransformChain();
    CVisTransform4x4 base_ti = m_view_geometry.CompleteTransform();
    CVisTransform4x4 layer_t = base_ti * layer_c;
    //geom = layer_t;     // set to the true (intrinsic) geometry
    geom = CVisTransformChain(layer_t); 
}


// if we use geometry instead of transform, 
// TransformChain.size() should be set to 3 for each frame in composite
// each frame is represented as R^-1 V^-1 T_Image^-1
// layer_g.size() should be 6 because it is T_Image V Rview  R^-1 V^-1 T_Image^-1. 
template<class TPixel>
CVisTransformChain CVisComposite<TPixel>::LayerGeometry(int i, double f)
{
    CVisShape s = (*this)[0].Shape(); // if all images have same size
    double cx = 0.5*(s.left + s.right-1);
    double cy = 0.5*(s.top + s.bottom-1); 

    assert(m_view_geometry.size() == 3); 

    CVisTransformChain geom   = (*this)[i].TransformChain(); 
    assert(geom.size() == 3); 

    CVisTransformChain layer_g = m_view_geometry.Inverted(); 
    layer_g.push_back(geom); 
    assert(layer_g.size() == 6); 
    return layer_g; 
}

template<class TPixel>
void CVisComposite<TPixel>::SetLayerGeometry(int i, CVisTransformChain& layer_g, double f)
{

    CVisShape s = (*this)[0].Shape(); 
    double cx = 0.5*(s.left + s.right-1);
    double cy = 0.5*(s.top + s.bottom-1); 
    
    assert(m_view_geometry.size() == 3); 

    CVisTransformChain &geom = (*this)[i].TransformChain();

    assert(layer_g.size() == 6); 
    // set to the true (intrinsic) geometry
    geom = layer_g[3]; 
    geom.push_back(layer_g[4]); 
    geom.push_back(layer_g[5]); 
}

template<class TPixel>
void CVisComposite<TPixel>::MakeViewGeometryChain(double f)
{
    CVisShape s = (*this)[0].Shape(); // if all images have same size
    double cx = 0.5*(s.left + s.right-1);
    double cy = 0.5*(s.top + s.bottom-1); 

    int d = m_view_geometry.size(); 
    if (d != 3)
    {
        assert (d == 1 || d == 5);

        CVisTransform4x4    M = m_view_geometry.CompleteTransform(); 
        m_view_geometry = VisExtractTVRVT(M, cx, cy, f, f); 
		m_view_geometry.pop_front();
		m_view_geometry.pop_front();
    }
}

template<class TPixel>
void CVisComposite<TPixel>::CollapseGeometryChain()
{
    int nFrames = (*this).NFrames(); 
    CVisShape s = (*this)[0].Shape(); // if all images have same size
    double cx = 0.5*(s.left + s.right-1);
    double cy = 0.5*(s.top + s.bottom-1); 

    if (m_view_geometry.size() != 1)
    {
    
        CVisTransform4x4    M = m_view_geometry.CompleteTransform(); 
        if (m_view_geometry.size() == 3)
        {
            CVisTransform4x4 transform = m_view_geometry[2].Inverted(); 
            CVisTransform4x4 V = m_view_geometry[1].Inverted(); 
            M = transform*V*M; 
        }
        m_view_geometry = CVisTransformChain(M); 
    }
}

template<class TPixel>
CVisShape CVisComposite<TPixel>::LayerShape(int i, int ignore_stretch)
{
    CVisShape sh = Warped(i).Shape();
    if (sh.Height() <= 0 || sh.Width() <= 0)
        return sh;  // empty frame (for layered stereo)
    if (LayerTransform(i) == CVisTransform4x4())
        return sh;
    CVisShape shape = VisResampleShape(sh, LayerTransform(i),
                                       ignore_stretch ? 100 : m_max_pixel_stretch);
    return shape;
}


template<class TPixel>
void CVisComposite<TPixel>::LayerCenter(int frame, int center[2], double zoom)
{
    int line[4][2][2];
    LayerOutline(frame, line, zoom);
    center[0] = center[1] = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 2; j++)
            center[j] += line[i/2][i&1][j];
    center[0] = (center[0] + 4) / 8;
    center[1] = (center[1] + 4) / 8;
}

template<class TPixel>
void CVisComposite<TPixel>::LayerOutline(int i, int line[4][2][2], double zoom)
{
    VisResampleOutline(Warped(i).Shape(), LayerTransform(i), line, zoom);
}

//
// Layer composition
//

static CVisShape RoundUpToNearest4Columns(CVisShape shape)
{
    // Images displayed using SetDIBitsToDevice need to be multiples of 4 columns
	shape.right += ((- shape.Width()) & 3);
    return shape;
}

template<class TPixel>
int CVisComposite<TPixel>::IsActive(int index)
{
    int is_composite = m_overlay == VIS_COMPOSITE_VIEW_COMPLETE;
    int valid_nghbr  = m_neighbor_frames == 0 ||
            abs(index - CurrentFrame()) <= abs(m_neighbor_frames) &&
            (m_neighbor_frames > 0 || index <= CurrentFrame()); // use only previous
    int valid_skip   = m_skip_frames < 2 || (index % m_skip_frames) == 0;
    return is_composite || valid_nghbr && valid_skip;
}

template<class TPixel>
CVisComposite<TPixel>::T_Image& CVisComposite<TPixel>::TopLayer()
{
    if (! (m_valid_layers & VIS_VALID_TOP))
        FormTopLayer(TopLayerShape());
    if (! (m_valid_layers & VIS_VALID_TOP_BKGD))
        FormTopWBkgd(m_top_layer.Shape());
    return m_top_w_bkgd;
}

template<class TPixel>
CVisComposite<TPixel>::T_Image& CVisComposite<TPixel>::TopLayerAlpha()
{
    if (! (m_valid_layers & VIS_VALID_TOP))
        FormTopLayer(TopLayerShape());
    return m_top_layer;
}

template<class TPixel>
CVisShape CVisComposite<TPixel>::TopLayerShape()
{
    return LayerShape(CurrentFrame());
}

template<class TPixel>
void CVisComposite<TPixel>::FormTopLayer(CVisShape shape)
{
    // Planar transformation
    CVisTransform4x4 t = LayerTransform(CurrentFrame()).Inverted();

    // Create the resampled image
    if (m_top_layer.Shape() != shape && shape.Height() > 0 && shape.Width() > 0)
        m_top_layer.Allocate(shape);

    // Resample it
    if (shape.Height() > 0 && shape.Width() > 0)
        VisResample(Warped(), m_top_layer, t, m_interpolation);
    m_valid_layers |= VIS_VALID_TOP;
}

template<class TPixel>
void CVisComposite<TPixel>::FormTopWBkgd(CVisShape shape)
{
    // Create the top layer image with background
    if (m_top_w_bkgd.Shape() != shape && shape.Height() > 0 && shape.Width() > 0)
        m_top_w_bkgd.Allocate(shape);

    // Form the visible top layer (with desired opaque background value)
    int g = m_background_gray_level;
    m_top_w_bkgd.FillPixels(CVisRGBABytePixel(g, g, g, 255));
        // this will break if CVisComposite<TPixel>::T_Image != CVisRGBABytePixel ... fix this later ...
    VisComposite(m_top_layer, m_top_w_bkgd, m_top_w_bkgd, composite_over);
    m_valid_layers |= VIS_VALID_TOP_BKGD;
}

template<class TPixel>
CVisComposite<TPixel>::T_Image& CVisComposite<TPixel>::BottomAlpha()
{
    BottomLayer();  // force recomputation if necessary
    return m_bottom_alpha;
}

template<class TPixel>
CVisComposite<TPixel>::T_Image& CVisComposite<TPixel>::BottomLayer()
{
    if (! (m_valid_layers & VIS_VALID_BOTTOM))
        FormBottomLayer(BottomLayerShape(),
                        DisplayMode() != VIS_COMPOSITE_VIEW_COMPLETE);
    return m_bottom_layer;
}

template<class TPixel>
CVisShape CVisComposite<TPixel>::BottomLayerShape()
{
#ifdef SMALLVIEW
    return Warped(0).Shape(); 
#endif
    // Don't recompute if there is already a valid bottom (anchored)
    if (m_valid_layers & m_always_valid & VIS_VALID_BOTTOM)
        return m_bottom_layer.Shape();
    CVisShape shape;
    shape = CVisRect(0, 0, 0, 0);
    for (int i = 0; i < NFrames(); i++) {
        if (! IsActive(i))
            continue;
        CVisShape sh = LayerShape(i);
        if (sh.Height() <= 0 || sh.Width() <= 0)
            continue;
        if (shape.Height())
            shape = (shape | sh);
        else
            shape = sh;
        shape.SetNBands(Warped(i).NBands());
    }
    // Trim shape to m_bounding_box times the size of top layer
    if (m_bounding_box > 0.0f) {
        CVisShape top_shape = TopLayerShape();
        int dwidth  = int(top_shape.Width() * (m_bounding_box - 1) / 2);
        int dheight = int(top_shape.Height() * (m_bounding_box - 1) / 2);
//        CVisShape trim_shape = top_shape.DilatedBy(dwidth, dheight);
        CVisShape trim_shape = top_shape;
		trim_shape.InflateRect(dwidth, dheight);

        shape = (shape & trim_shape);
    }
#ifdef ROUNDUP
    return RoundUpToNearest4Columns(shape);
#else
    return shape; 
#endif
}

static void VisInsertPixels(CVisRGBAByteImage &src, CVisRGBAByteImage &accumulator,
                            int b)
{
    // Insert a warped layer in to the accumulator table for later sorting...
    int nrows = src.Height(), ncols = src.Width(), nbands = accumulator.NBands();
    for (int r = 0; r < nrows; r++) {
        CVisRGBABytePixel *s = &src.Pixel(src.Left(), r + src.Top());
        CVisRGBABytePixel *a = &accumulator.Pixel(src.Left(), r + src.Top(), 0);
        for (int c = 0; c < ncols; c++, a += nbands)
			a[b] = s[c];
    }
}

struct PxCompare {  // compare color pixels in b'th band
    int b;
    PxCompare(int band) { b = band; }
    int operator()(CVisRGBABytePixel& p1, CVisRGBABytePixel& p2) {
        unsigned char *b1 = (unsigned char *) &p1;
        unsigned char *b2 = (unsigned char *) &p2;
        return b1[b] <= b2[b];
    }
};

static CVisRGBABytePixel VisPixelMedian(CVisRGBABytePixel *l, int n, int b)
{
    return VisMedian(PxCompare(b), l, n, n/2);
}

static CVisRGBAByteImage VisMedianCompute(CVisRGBAByteImage &accumulator)
{
    // Do the actual median computation
    CVisShape sh = accumulator.Shape();
    sh.SetNBands(1);
    CVisRGBAByteImage median(sh);
    int nrows = sh.Height(), ncols = sh.Width(), nbands = accumulator.NBands();
    if (nbands == 0) {
        median.ClearPixels();
        return median;
    }
	CVisRGBABytePixel *buf = new CVisRGBABytePixel[nbands];
    for (int r = 0; r < nrows; r++) {
        CVisRGBABytePixel  *a = &accumulator.Pixel(sh.Left(), r + sh.Top());
        CVisRGBABytePixel  *m = &median.Pixel(sh.Left(), r + sh.Top());
        for (int c = 0; c < ncols; c++, a += nbands) {
			// Gather up all the opaque pixels
			for (int b = 0, n = 0; b < nbands; b++) {
				if (a[b].A() == 255)
					buf[n++] = a[b];
			}
            if (n > 2) {
                // Compute each band separately
                for (int j = 0; j < 3; j++) {
                    CVisRGBABytePixel p = VisPixelMedian(buf, n, j);
                    unsigned char *pa = (unsigned char *) &p;
                    unsigned char *ma = (unsigned char *) &m[c];
                    ma[j] = pa[j];
                }
                m[c].SetA(255);   // opaque
            } else if (n > 0)
                m[c] = buf[0];  // take the first one
            else
                m[c] = CVisRGBABytePixel(0, 0, 0, 0);  // empty
        }
    }
	delete []buf;
    return median;
}

static void VisPyramidCenterBlend(CVisRGBAFloatImage &src, 
                                 CVisRGBAFloatImage &srcR)
{
    int nrows = src.Height(), ncols = src.Width();
    assert(src.NBands() == 1); 

    for (int r = 0; r < nrows; r++) {
        //CVisRGBAFloatPixel    *s = &src.Pixel(src.Left(), r + src.Top());
        //CVisRGBAFloatPixel    *s2 = &srcR.Pixel(srcR.Left(), r + srcR.Top());
        CVisRGBAFloatPixel  *s  = src.PtrToFirstPixelInRow(r + src.Top());
        CVisRGBAFloatPixel  *s2 = srcR.PtrToFirstPixelInRow(r + srcR.Top()); 
        int c2 = ncols/2; 

        s[c2].SetB((s[c2].B() + s2[c2].B())/2); 
        s[c2].SetG((s[c2].G() + s2[c2].G())/2);
        s[c2].SetR((s[c2].R() + s2[c2].R())/2);
        s[c2].SetA((s[c2].A() + s2[c2].A())/2);

        if (ncols < 3)
            break; 

        c2--; 
        s[c2].SetB((3*s[c2].B() + s2[c2].B())/4);  
        s[c2].SetG((3*s[c2].G() + s2[c2].G())/4);
        s[c2].SetR((3*s[c2].R() + s2[c2].R())/4);
        s[c2].SetA((3*s[c2].A() + s2[c2].A())/4);

        c2++, c2++;
        s[c2].SetB((s[c2].B() + 3*s2[c2].B())/4);  
        s[c2].SetG((s[c2].G() + 3*s2[c2].G())/4);
        s[c2].SetR((s[c2].R() + 3*s2[c2].R())/4);
        s[c2].SetA((s[c2].A() + 3*s2[c2].A())/4);

        c2++; 
        for (int c = c2; c < ncols; c++) 
        {
            s[c].SetB(s2[c].B());
            s[c].SetG(s2[c].G());
            s[c].SetR(s2[c].R());
            s[c].SetA(s2[c].A());
        }
    }
}

// code used for our appleOrange blending example
static void LeftOrRightBlendWeight(CVisFloatImage &weight, 
                                   int RightBlend, float value)
{
    int nrows = weight.Height(), ncols = weight.Width();
    for (int r = 0; r < nrows; r++) 
    {
        CVisFloatPixel      *w = &weight.Pixel(weight.Left(), r + weight.Top());
        if (RightBlend) // i = 1, second image
        {
            for (int c = ncols/2; c < ncols; c++) 
            {
                w[c] = value;
            }
            for (c = 0; c < ncols/2; c++) 
            {
                w[c] = 0.1 * value;
            }
        }
        else // left blend i = 0
        {
            for (int c = 0; c <= ncols/2; c++) 
            {
                w[c] = value;
            }
            for (c = ncols/2+1; c < ncols; c++) 
            {
                w[c] = 0.1 * value;
            }
        }
    }
}

static void PreMultiplyAlpha(CVisFloatImage &weight, 
                             CVisRGBAByteImage &layer)
{
    int nrows = layer.Height(), ncols = layer.Width();
    VisCheckConformal(&layer, &weight);
    for (int r = 0; r < nrows; r++) 
    {
        CVisRGBABytePixel *s =  &layer.Pixel(layer.Left(), r + layer.Top()); 
        CVisFloatPixel      *w = &weight.Pixel(layer.Left(), r + layer.Top());
        for (int c = 0; c < ncols; c++) 
        {
            w[c] *= (float) s[c].A() / 255.0;
        }
    }
}

static void AccumulateWeightMaps(CVisFloatImage &weight, 
                                 CVisFloatImage &sumweight)
{
    int nrows = weight.Height(), ncols = weight.Width();
    //VisCheckConformal(&weight, &sumweight);
    // weight should be subimage of sumweight!
    for (int r = 0; r < nrows; r++) 
    {
        CVisFloatPixel *w = &weight.Pixel(weight.Left(), r + weight.Top()); 
        CVisFloatPixel *sw= &sumweight.Pixel(weight.Left(), r + weight.Top());
        for (int c = 0; c < ncols; c++) 
        {
            sw[c] += w[c]; 
        }
    }
}

//
//  For this code, we assume that all pixel addresses are the same,
//  i.e., that src may be a subimage of accumulator
//
static void VisMultiplyWeightAccumulate(CVisRGBAFloatImage &src, 
                                        CVisRGBAFloatImage &dst, 
                                        CVisFloatImage &weight,
                                        CVisFloatImage &sumweight, 
                                        int nf) 
{
    float scale = 1.0 / (float) nf; 
    int nrows = src.Height(), ncols = src.Width();
    VisCheckConformal(&dst, &sumweight);
    VisCheckConformal(&src, &weight);
    for (int r = 0; r < nrows; r++) 
    {
        CVisRGBAFloatPixel *s = &src.Pixel(src.Left(), r + src.Top()); 
        CVisRGBAFloatPixel *a = &dst.Pixel(src.Left(), r + src.Top());
        CVisFloatPixel      *sw= &sumweight.Pixel(src.Left(), r + src.Top());

        float *w = &weight.Pixel(src.Left(), r + weight.Top()); 
        for (int c = 0; c < ncols; c++) 
        {
            float ww = (sw[c] == 0.0? scale : w[c]/sw[c]); 
            // float ww = (sw[c] == 0.0? 0.0 : w[c]/sw[c]); 
			a[c].SetB(a[c].B() + s[c].B() * ww);  
			a[c].SetG(a[c].G() + s[c].G() * ww);
			a[c].SetR(a[c].R() + s[c].R() * ww);
			a[c].SetA(a[c].A() + s[c].A() * ww);
        }
    }
}

static void VisWeightedAverage(CVisRGBAFloatImage &dst, CVisFloatImage &weight)
{
    int nrows = dst.Height(), ncols = dst.Width();
    VisCheckConformal(&dst, &weight);
    for (int r = 0; r < nrows; r++) {
        CVisFloatPixel      *w = weight.PtrToFirstPixelInRow(r + weight.Top()); 
        CVisRGBAFloatPixel *d = dst.PtrToFirstPixelInRow(r + dst.Top());
        for (int c = 0; c < ncols; c++) {
            float ww = (w[c] > 0.0? 1.0 / w[c] : 0.0);
			d[c].SetB(d[c].B() * ww);  
			d[c].SetG(d[c].G() * ww);
			d[c].SetR(d[c].R() * ww);
			d[c].SetA(d[c].A() * ww);
        }
    }
}

static void VisCopyNoAlpha(CVisRGBAFloatImage &accumulator, CVisRGBAByteImage &dst)
{
    int empty_value = 0; 
    int nrows = dst.Height(), ncols = dst.Width();
    VisCheckConformal(&accumulator, &dst);
    for (int r = 0; r < nrows; r++) {
        CVisRGBABytePixel      *d = dst.PtrToFirstPixelInRow(r + dst.Top());
        CVisRGBAFloatPixel     *a = accumulator.PtrToFirstPixelInRow(r + accumulator.Top());
        for (int c = 0; c < ncols; c++) {
#ifndef NOZEROALPHA
            if (d[c].A() != 0) // only copy the region with non-zero alpha
            {
#endif
                float red   = (float) a[c].R(); 
                float green = (float) a[c].G(); 
                float blue  = (float) a[c].B(); 
                float alpha = (float) a[c].A(); 

                if (alpha < 0) 
                    alpha = 0;
                if (red < 0) 
                    red = 0; 
                if (green < 0)
                    green = 0; 
                if (blue < 0)
                    blue = 0; 

                if (red > alpha)
                    red = alpha; 
                if (green > alpha)
                    green = alpha; 
                if (blue > alpha)
                    blue = alpha; 

                // Set to opaque white if empty
                if (alpha == 0)
				{
                    d[c].SetR(empty_value); 
                    d[c].SetG(empty_value);
                    d[c].SetB(empty_value); 
                    d[c].SetA(empty_value);
				}
                else if (alpha != 255.0)
                {
                    float w = 255.0f / alpha;
                    d[c].SetR((int) (red * w)); 
                    d[c].SetG((int) (green * w));
                    d[c].SetB((int) (blue * w)); 
                    d[c].SetA(255);   // opaque
                } 
                else 
                {
                    d[c].SetR((int) (red)); 
                    d[c].SetG((int) (green));
                    d[c].SetB((int) (blue)); 
                    d[c].SetA(255);   // opaque
                }
#ifndef NOZEROALPHA
            }
#endif
        }
    }
}

static float JacobianWeight(CVisTransform4x4 transform, float x, float y)
{
    double d = transform[3][0] * x + transform[3][1] * y + transform[3][3]; 
    double n1 = transform[0][0] * x + transform[0][1] * y + transform[0][3]; 
    double n2 = transform[1][0] * x + transform[1][1] * y + transform[1][3]; 
    double d3 = d * d * d; 
    double m0m4m1m3 = transform[0][0] * transform[1][1] - transform[0][1] * transform[1][0]; 
    double m3m7m4m6 = transform[1][0] * transform[3][1] - transform[1][1] * transform[3][0]; 
    double m1m6m0m7 = transform[0][1] * transform[3][0] - transform[0][0] * transform[3][1]; 

    float  w = (float) (m0m4m1m3 * d + m3m7m4m6 * n1 + m1m6m0m7 * n2) / d3;
    return w*w; 
}

static void VisJacobianWeight(CVisTransform4x4 transform, CVisFloatImage &src)
{
    int nrows = src.Height(), ncols = src.Width(); 

    for (int r = 0; r < nrows; r++) 
    {
        float *w = src.PtrToFirstPixelInRow(r + src.Top());
        for (int c = 0; c < ncols; c++)
            w[c] *= JacobianWeight(transform, (float) c, (float) r);
    }
}

// quick hack for calculating Jacobian for cylindrical map
static CVisFloatImage VisSineJacobDistance(CVisRGBAByteImage &src, int max_dist)
{
    CVisFloatImage fdst(src.Shape()); 
    int nrows = src.Height(), ncols = src.Width();
    float *lut = new float[ncols];
    for (int c = 0; c < ncols; c++)
    {
        float theta = c * M_PI / (ncols - 1.0); 
        float scale = sin(theta);
        lut[c] = max_dist * scale * scale * scale * scale; 
    }

    for (int r = 0; r < nrows; r++) {
        float *df = fdst.PtrToFirstPixelInRow(r + src.Top());
        for (int c = 0; c < ncols; c++)
            df[c] = lut[c];
    }
    delete lut;

    return fdst;
}

// should merge with VisCityBlockDistance
static CVisFloatImage VisSineBlockDistance(CVisRGBAByteImage &src, int max_dist)
{
    CVisByteImage dst(src.Shape());
    CVisFloatImage fdst(src.Shape()); 
    int nrows = src.Height(), ncols = src.Width();

    // Forward sweep
    for (int r = 0; r < nrows; r++) {
        CVisRGBABytePixel     *s = &src.Pixel(src.Left(), r + src.Top());
        unsigned char *d = &dst.Pixel(src.Left(), r + src.Top());
        unsigned char *p = &dst.Pixel(src.Left(), r-(r>0) + src.Top());

        // First row: just set to totally opaque alpha value
        if (r == 0) {
            for (int c = 0; c < ncols; c++)
                d[c] = (s[c].A() == 255);
        }
        
        // Other rows: find minimum of left and up neighbors
        else {
            d[0] = (s[0].A() != 0);
            for (int c = 1; c < ncols; c++) {
                if (s[c].A()) {
                    int d1 = d[c-1], d2 = p[c];
                    int d3 = __min(d1, d2)+1;
                    d[c]   = __min(d3, max_dist);
                } else
                    d[c] = 0;
            }
        }
    }

    // Backward sweep
    for (r = nrows-1; r >= 0; r--) {
        CVisRGBABytePixel     *s = &src.Pixel(src.Left(), r + src.Top());
        unsigned char *d = &dst.Pixel(src.Left(), r + src.Top());
        unsigned char *p = &dst.Pixel(src.Left(), r+(r<nrows-1) + src.Top());

        // Last row: just set to totally opaque alpha value
        if (r == nrows-1) {
            for (int c = 0; c < ncols; c++)
                d[c] = (s[c].A() == 255);
        }
        
        // Other rows: find minimum of right and down neighbors
        else {
            d[ncols-1] = (s[ncols-1].A() != 0);
            for (int c = ncols-2; c >= 0; c--) {
                if (s[c].A()) {
                    int d1 = d[c+1], d2 = p[c];
                    int d3 = __min(d1, d2)+1;
                    int d4 = __min(d3, d[c]);
                    d[c] = __min(d4, max_dist);
                } else
                    d[c] = 0;
            }
        }
    }

    // find the real maximum
    int maximum = 0; 
    for (r = 0; r < nrows; r++) {
        unsigned char *d = dst.PtrToFirstPixelInRow(r + src.Top());
        for (int c = 0; c < ncols; c++)
        {
            if (d[c] > maximum)
                maximum = d[c];
        }
    }
    // Postprocess:  smooth out map by passing through a raised cosine
    float *lut = new float[maximum+1];
    for (int i = 0; i <= maximum; i++)
    {
        float scale = 0.5 * (1.0 - cos(i * M_PI / maximum));
        //lut[i] = (float) i * max_dist / maximum;
        //lut[i] = max_dist * scale; 
        lut[i] = max_dist * scale * scale; 
        //lut[i] = max_dist * scale * scale * scale * scale;
    }
    for (r = 0; r < nrows; r++) {
        unsigned char *d = dst.PtrToFirstPixelInRow(r + src.Top());
        float *df = fdst.PtrToFirstPixelInRow(r + src.Top());
        for (int c = 0; c < ncols; c++)
            df[c] = lut[d[c]];
    }
    delete lut;

    return fdst;
}

static void SumTransforms(CVisTransform4x4 in, CVisTransform4x4 &out, float s)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        out[i][j] += s * in[i][j]; 
}

static void AverageTransforms(CVisTransform4x4& sum, int n)
{
    assert(n > 0); 
    float scale = 1.0 / (float) n; 
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        sum[i][j] *= scale; 
}

template<class TPixel>
void CVisComposite<TPixel>::FormBottomLayer(CVisShape shape, int omit_current)
{
    // Progress report
    ProgressReport("Forming bottom layer");

    // for cases where we want to synthesize new views (perhaps with smaller view
#ifdef NEWVIEW
    // setting new view matrix
    CVisTransform4x4 sumTrans = VisDiagonalMatrix(0, 0, 0, 0); 
    float s[2] = {0.5, 0.5};
    for (int ii = 0; ii < NFrames(); ii++) {
        CVisTransformChain &geom = (*this)[ii].TransformChain();
        CVisTransform4x4 layer_i = geom.CompleteTransform();
        SumTransforms(layer_i, sumTrans, s[ii]); 
    }
    AverageTransforms(sumTrans, NFrames()); 
    SetViewGeometry(CVisTransformChain(sumTrans)); 
#endif //NEWVIEW

    CVisPyramid<CVisFloatImage, CVisFloatImage>* weightPyramid; 
    CVisPyramid<CVisFloatImage, CVisFloatImage>* weightPyramidAccumulator; 
    T_Image bottom_alpha_single(shape); 

    // Allocate the bottom layer
    if (m_bottom_layer.Shape() != shape && shape.Height() > 0 && shape.Width() > 0) {
        m_bottom_layer.Allocate(shape);
        m_bottom_alpha.Allocate(shape);
    }
    // Initialize bottom layer or accumulator
    if (CompositingOperator() == composite_over ||
        CompositingOperator() == composite_minimum ||
        CompositingOperator() == composite_maximum ||
        CompositingOperator() == composite_sum)
        m_bottom_alpha.ClearPixels();
    if (CompositingOperator() == composite_accumulate ||
        CompositingOperator() == composite_feather ||
        CompositingOperator() == composite_pyramid_feather ||
        CompositingOperator() == composite_median) {
        CVisShape sh = m_bottom_layer.Shape();
        if (m_accumulator.Height() == 0)
            m_accumulator = CVisRGBAFloatImage(sh);
        CVisRGBAFloatImage old_accumulator = m_accumulator; // save pixels
        if (m_accumulator.Shape() != sh && sh.Height() > 0 && sh.Width() > 0)
            m_accumulator.Allocate(sh);

        // Special case for incremental accumulation
        if ((m_valid_layers & VIS_VALID_ACCUMULATOR)) { // old accumulator is (partially) valid
            CVisShape sh1 = old_accumulator.Shape();
            if (sh1 != sh) {
                m_accumulator.ClearPixels();
                sh1 = (sh & sh1);
                if (sh1.Height() > 0 && sh1.Width() > 0) // check non-empty
                    old_accumulator.SubImage(sh1).CopyPixelsTo(
						m_accumulator.SubImage(sh1));
            }
        } else
            m_accumulator.ClearPixels();

        // pyramid for blending
        if (CompositingOperator() == composite_pyramid_feather) 
        {
            m_pyramid_accumulator = new CVisPyramid<CVisRGBAByteImage>(m_blend_level, 
                                                pyramid_band_pass); //ColorByte

            m_pyramid_layer = new CVisPyramid<CVisRGBAByteImage>(m_blend_level, 
                                                pyramid_band_pass); //ColorByte

            weightPyramid = new CVisPyramid<CVisFloatImage, CVisFloatImage>(m_blend_level); 
            weightPyramidAccumulator = new CVisPyramid<CVisFloatImage, CVisFloatImage>(m_blend_level); 
        }
    }

    if (CompositingOperator() == composite_median || m_save_layers) {
        for (int i = 0, n_active = 0; i < NFrames(); i++)
            n_active += IsActive(i);    // count active frames
        // Allocate a multiband image to hold the layers (interleaved)
        CVisShape sh = m_bottom_layer.Shape();
        sh.SetNBands(n_active);         // replace with active frame count
        if (sh.Height() > 0 && sh.Width() > 0)
            m_layers.Allocate(sh);
        m_layers.ClearPixels();
    }

    // normalize the weight maps
    if (CompositingOperator() == composite_pyramid_feather)
    {
        for (int i = 0; i < NFrames(); i++) {
            // may need proper testing for early out: add later
            CVisTransform4x4 t = LayerTransform(i).Inverted();
            CVisShape sh = LayerShape(i, 1);    // don't clip by max_stretch...
            if (sh.Height() <= 0 || sh.Width() <= 0) // check non-empty
                continue;
            sh = (shape & sh);
            if (sh.Height() <= 0 || sh.Width() <= 0) // check non-empty
                continue;
            T_Image layer(sh);
            // Resample it
            VisResample(Warped(i), layer, t, m_interpolation);

            CVisFloatImage w(sh); 
#ifndef INPLACE
            CVisFloatImage w0 = VisSineBlockDistance(Warped(i), m_feather_distance);
//            CVisFloatImage w0 = VisSineJacobDistance(Warped(i), m_feather_distance);

            // different weights 
/*
            CVisByteImage w1(w0.Shape()); 
            VisMap2(FloatToByte(), w1, w0); 
            w1.WriteFile("weight.bmp"); 
*/
            // Jacobian weighting on top of Feathering map 
/*
            if (i==0) VisMap2(ScaleBy(1.0f), w0, w0); 
            else VisMap2(ScaleBy(1.0f), w0, w0);
            
            if (t != VisIdentityMatrix4x4()) 
                VisJacobianWeight(t, w0); 
  */             
            VisResample(w0, w, t, m_interpolation); 
#else
                w.ClearPixels(); 
                LeftOrRightBlendWeight(w, i, 100.0); 
#endif

            PreMultiplyAlpha(w, layer); 

            // construct the Gaussian pyramid for weight image
            weightPyramid->SetBase(w); 
            weightPyramid->Construct(); 

            if (i == 0) // the first image
            {
                CVisFloatImage wpa(shape); 
                wpa.ClearPixels(); 
                weightPyramidAccumulator->SetBase(wpa); 
                weightPyramidAccumulator->Construct(); 

                int l; 
                for (l=0; l<m_blend_level; l++)
                {
                    weightPyramidAccumulator->LowPass(l).ClearPixels(); 
                }
            }
            for (int l=0; l<m_blend_level; l++)
            {
                AccumulateWeightMaps(weightPyramid->LowPass(l), 
                                     weightPyramidAccumulator->LowPass(l)); 
            }
        }
    }

    // Sequence through the images back to front, optionally omitting current layer
    for (int i = 0, j = 0; i < NFrames(); i++) {
        int omit_this_one = omit_current && i == CurrentFrame();
        
        // Incremental accumulation for more efficiency
        if ((CompositingOperator() == composite_accumulate ||
             CompositingOperator() == composite_feather) &&
            (m_valid_layers & VIS_VALID_ACCUMULATOR) &&
            i < m_n_accumulated && i != m_i_not_accumulated) {
            if (! omit_this_one)
                continue;   // already in accumulator
            omit_this_one = -1;     // special value, do a negative accumulate
        }

        // Early out (don't use current frame)
        if (omit_this_one == 1)
            continue;

        // Early out if not within neighbors window
        if (! IsActive(i))
            continue;

        // Planar transformation
        CVisTransform4x4 t = LayerTransform(i).Inverted();
        VisFixTransform4x4(t);
        CVisShape sh = LayerShape(i, 1);    // don't clip by max_stretch...
        if (sh.Height() <= 0 || sh.Width() <= 0) // check non-empty
            continue;
        sh = (shape & sh);
        if (sh.Height() <= 0 || sh.Width() <= 0) // check non-empty
            continue;
        T_Image layer(sh);

        // Resample it
        VisResample(Warped(i), layer, t, m_interpolation);
        T_Image bott_sub = m_bottom_alpha.SubImage(sh);
        T_Image bott_sub_single = bottom_alpha_single.SubImage(sh); 

        // Progress report
        ProgressReport("Compositing frame %d", i);

        // Blend it in
        switch (CompositingOperator()) {

        case composite_over:
        case composite_minimum:
        case composite_maximum:
        case composite_sum:
            // Back-to-front (actually, first to last) composition
            VisComposite(layer, bott_sub, bott_sub, CompositingOperator());
            break;

        case composite_accumulate:
            // A-buffer (average) blending ...
            VisAccumulate(layer, m_accumulator, *(CVisFloatImage *) 0, omit_this_one);
            break;

        case composite_feather:
            // A-buffer (weighted average) blending
            {
                static int smooth_ramp = 1;     // use raised cosine shape
                // compute weighting function from warped layer
                // resample weighting function computed from image
                CVisFloatImage& w0 = (*this)[i].ImageFeather();
                if (Warped(i).Shape() != w0.Shape())
                    w0 = VisFloatCityBlockDistance(Warped(i), m_feather_distance,
                                                   smooth_ramp); 
                CVisFloatImage w(sh);
                VisResample(w0, w, t, m_interpolation);
                VisAccumulate(layer, m_accumulator, w, omit_this_one);
            }
            break;

        case composite_pyramid_feather:
            {
#ifndef APPLEORANGE   // feathered pyramid blending: new

                CVisFloatImage w(sh); 
#ifndef INPLACE
                CVisFloatImage w0 = VisSineBlockDistance(Warped(i), m_feather_distance);
//                CVisFloatImage w0 = VisSineJacobDistance(Warped(i), m_feather_distance);
/*                
            if (i==0) VisMap2(ScaleBy(1.0f), w0, w0); 
            else VisMap2(ScaleBy(1.0f), w0, w0);
            
                if (t != VisIdentityMatrix4x4()) 
                    VisJacobianWeight(t, w0); 
  */              
                VisResample(w0, w, t, m_interpolation); 
                //w.WriteFile("weight.bmp"); 
#else
                w.ClearPixels(); 
                LeftOrRightBlendWeight(w, i, 100.0); 
#endif
                PreMultiplyAlpha(w, layer); 

                // construct the Gaussian pyramid for weight image
                weightPyramid->SetBase(w); 
                weightPyramid->Construct(); 

                if (i == 0) // the first image
                {
                    m_bottom_alpha.ClearPixels(); 

                    CVisRGBAByteImage ipa(shape); 
                    //m_pyramid_accumulator->SetBase(m_bottom_alpha); 
                    ipa.ClearPixels(); 
                    m_pyramid_accumulator->SetBase(ipa); 
                    m_pyramid_accumulator->Construct(); 

                    int l; 
                    for (l=0; l<m_blend_level-1; l++)
                        m_pyramid_accumulator->BandPassFloat(l).ClearPixels(); 
                    for (l=0; l<m_blend_level; l++)
                    {
                        m_pyramid_accumulator->LowPassFloat(l).ClearPixels(); 
                    }
                }

                VisComposite(layer, bott_sub, bott_sub, composite_over); 

                m_pyramid_layer->SetBase(layer); 
                m_pyramid_layer->Construct(); 

                // at the top level, lowpass is bandpass. 
                int level = m_blend_level - 1; 
                VisMultiplyWeightAccumulate(m_pyramid_layer->LowPassFloat(level), 
                                            m_pyramid_accumulator->LowPassFloat(level), 
                                            weightPyramid->LowPass(level), 
                                            weightPyramidAccumulator->LowPass(level), 
                                            NFrames()); 
                for (level = m_blend_level-2; level >= 0; level--)
                {
                    VisMultiplyWeightAccumulate(m_pyramid_layer->BandPassFloat(level), 
                                                m_pyramid_accumulator->BandPassFloat(level), 
                                                weightPyramid->LowPass(level), 
                                                weightPyramidAccumulator->LowPass(level), 
                                                NFrames()); 
                }

#else  // Apple and Orange blending
                if (i == 0)
                    m_bottom_alpha.ClearPixels(); 
                VisComposite(layer, bott_sub_single, bott_sub_single, composite_over); 
                VisComposite(layer, bott_sub, bott_sub, composite_over); 
                if (i == 0) // the first image
                {
                    //m_pyramid_accumulator->SetBase(m_bottom_alpha); 
                    m_pyramid_accumulator->SetBase(bottom_alpha_single); 
                    m_pyramid_accumulator->Construct(); 
                }
                else
                {
                    m_pyramid_layer->SetBase(bottom_alpha_single); 
                    m_pyramid_layer->Construct(); 

                    for (int level = m_blend_level-2; level >= 0; level--)
                    {
                       VisPyramidCenterBlend(m_pyramid_accumulator->BandPassFloat(level), 
                                             m_pyramid_layer->BandPassFloat(level)); 
                    }
                    // at the top level, lowpass is bandpass. 
                    VisPyramidCenterBlend(m_pyramid_accumulator->LowPassFloat(m_blend_level-1), 
                                          m_pyramid_layer->LowPassFloat(m_blend_level-1)); 
                }
#endif
            }
            break; 

        }
        if (CompositingOperator() == composite_median || m_save_layers) {
            // Median filtering, accumulate statistics for now
            VisInsertPixels(layer, m_layers, j++);
        }
    }

    // Progress report
    ProgressReport("Finishing up bottom layer");

    // Final normalization and readout
    if (CompositingOperator() == composite_accumulate ||
        CompositingOperator() == composite_feather) {
        VisAccumulateNormalize(m_accumulator, m_bottom_alpha, 0);
        m_n_accumulated = NFrames();
        m_i_not_accumulated = (omit_current) ? CurrentFrame() : -1;
        m_valid_layers |= VIS_VALID_ACCUMULATOR;
    }
    else if (CompositingOperator() == composite_pyramid_feather)
    {
#ifdef APPLEORANGE
        int level = m_blend_level-1;
        // at the top level, lowpass is bandpass. 
        VisWeightedAverage(m_pyramid_accumulator->LowPassFloat(level), 
                           weightPyramidAccumulator->LowPass(level)); 
        for (level = m_blend_level-2; level >= 0; level--)
        {
            VisWeightedAverage(m_pyramid_accumulator->BandPassFloat(level), 
                               weightPyramidAccumulator->LowPass(level)); 
        }
#endif
        //m_bottom_alpha.WriteFile("output.bmp"); 
        m_pyramid_accumulator->ReConstruct(); 
        VisCopyNoAlpha(m_pyramid_accumulator->LowPassFloat(0), m_bottom_alpha); 

        m_n_accumulated = NFrames();
        m_i_not_accumulated = (omit_current) ? CurrentFrame() : -1;
        m_valid_layers |= VIS_VALID_ACCUMULATOR;

        delete m_pyramid_accumulator; 
        delete m_pyramid_layer; 
        delete weightPyramid; 
        delete weightPyramidAccumulator; 
    }
    if (CompositingOperator() == composite_median) {
        m_bottom_alpha = VisMedianCompute(m_layers);
    }

    // Form the visible bottom layer (with desired opaque background value)
    int g = m_background_gray_level;
    m_bottom_layer.FillPixels(CVisRGBABytePixel(g, g, g, 255));
        // this will break if T_Image != CVisRGBABytePixel ... fix this later ...
    VisComposite(m_bottom_alpha, m_bottom_layer,
                   m_bottom_layer, composite_over);

    // Invalidate the cropped bottom region
    m_bottom_cropped = T_Image();

    m_valid_layers |= VIS_VALID_BOTTOM;
}

template<class TPixel>
CVisComposite<TPixel>::T_Image& CVisComposite<TPixel>::Composite()
{
    if (! (m_valid_layers & VIS_VALID_TOP))
        FormTopLayer(TopLayerShape());
    if (! (m_valid_layers & VIS_VALID_BOTTOM))
        FormBottomLayer(BottomLayerShape(),
                        DisplayMode() != VIS_COMPOSITE_VIEW_COMPLETE);
    if (DisplayMode() == VIS_COMPOSITE_VIEW_COMPLETE)
        return CroppedBottomLayer();
    if (! (m_valid_layers & VIS_VALID_COMPOSITE))
        FormComposite(CompositeShape());
    return m_composite;
}

template<class TPixel>
CVisShape CVisComposite<TPixel>::CompositeShape()
{
    CVisShape bshape = BottomLayerShape();
    CVisShape tshape = TopLayerShape();
    CVisShape cshape = bshape.UnionedWith(tshape);
    return cshape;
}

template<class TPixel>
void CVisComposite<TPixel>::FormComposite(CVisShape shape)
{
    // Allocate the composite layer
    if (m_composite.Shape() != shape && shape.Height() > 0 && shape.Width() > 0)
        m_composite.Allocate(shape);

    // Composite the top and bottom layers
    EVisComposite op;
    switch (DisplayMode()) {

    case VIS_COMPOSITE_VIEW_SEQUENCE:     // don't do anything
    case VIS_COMPOSITE_VIEW_INVISIBLE:
        return;

    case VIS_COMPOSITE_VIEW_OVERLAY:      // show top layer over others composited
    case VIS_COMPOSITE_VIEW_BLINK:        // blink top layer over others composited
    case VIS_COMPOSITE_VIEW_TRANSPARENT:  // show top layer transparently
        op = (DisplayMode() == VIS_COMPOSITE_VIEW_TRANSPARENT) ?
             composite_average : composite_over;
       // The correct code should first trim the composite, bottom and top
        // layers by the supplied shape, and then overlay them ...
        if ((m_composite.Shape() == m_top_layer.Shape()) &&
            (m_composite.Shape() == m_bottom_layer.Shape()))
            VisComposite(m_top_layer, m_bottom_layer, m_composite, op);
        else {
            if ((m_composite.Shape() == m_bottom_layer.Shape()))
                m_bottom_layer.CopyPixelsTo(m_composite);
            else {
                int g = m_background_gray_level;
                m_composite.FillPixels(CVisRGBABytePixel(g, g, g, 255));
                CVisRect sh = (m_composite.Shape() & 
                                              m_bottom_layer.Shape());
                if (sh.Height() > 0 && sh.Width() > 0) { // check non-empty
                    T_Image bott_sub = m_bottom_layer.SubImage(sh);
                    T_Image comp_sub = m_composite.SubImage(sh);
                    bott_sub.CopyPixelsTo(comp_sub);
                }
            }
            CVisShape top_shape = m_top_layer.Shape();
            top_shape = (top_shape & m_composite.Shape());
            T_Image top_sub = m_top_layer.SubImage(top_shape);
            T_Image comp_sub = m_composite.SubImage(top_shape);
            VisComposite(top_sub, comp_sub, comp_sub, op);
        }
        break;
    
    default:
        assert(m_composite.Height() >= 0);   // should never get here
        break;
    }
    m_valid_layers |= VIS_VALID_COMPOSITE;
}

template<class TPixel>
CVisComposite<TPixel>::T_Image& CVisComposite<TPixel>::CroppedBottomLayer()
{
    if (m_crop_region.Height() == 0)
            return BottomLayer();
    if (m_bottom_cropped.Shape() != m_crop_region) {
        T_Image& bottom = BottomLayer();
        T_Image cropped = bottom.SubImage(m_crop_region);
        m_bottom_cropped = T_Image(m_crop_region);
        cropped.CopyPixelsTo(m_bottom_cropped);
    }
    return m_bottom_cropped;
}

//
// Display the image selected by SetDisplayMode()
//

template<class TPixel>
void CVisComposite<TPixel>::SetDisplayMode(TVisCompositeView v)
{
    // Check if composite has to be recomputed
    if (v == m_overlay)
        return;

    if (m_overlay == VIS_COMPOSITE_VIEW_COMPLETE || 
                v == VIS_COMPOSITE_VIEW_COMPLETE)
        Invalidate(VIS_VALID_TOP);
    if (m_overlay == VIS_COMPOSITE_VIEW_TRANSPARENT ||
                v == VIS_COMPOSITE_VIEW_TRANSPARENT)
        Invalidate(VIS_VALID_TOP | VIS_VALID_BOTTOM);
    m_overlay = v;
}

template<class TPixel>
CVisComposite<TPixel>::T_Image& CVisComposite<TPixel>::CurrentView()
{
    switch (DisplayMode()) {
    case VIS_COMPOSITE_VIEW_SEQUENCE:     // just show selected image
        return Frame();
        break;
    case VIS_COMPOSITE_VIEW_OVERLAY:      // show top layer over others composited
        return Composite();
        break;
    case VIS_COMPOSITE_VIEW_BLINK:        // blink top layer over others composited
        if (m_blink)
            return BottomLayer();
        else
            return Composite();
        break;
    case VIS_COMPOSITE_VIEW_TRANSPARENT:  // show top layer transparently
        return Composite();    // not implemented yet
        break;
    case VIS_COMPOSITE_VIEW_INVISIBLE:    // don't show top layer (only others)
        return BottomLayer();
        break;
    case VIS_COMPOSITE_VIEW_SEGMENT:      // show only the top layer
        return TopLayer();
        break;
    case VIS_COMPOSITE_VIEW_COMPLETE:     // show complete composition
        return CroppedBottomLayer();
        break;
    }
    return Frame();
}

template<class TPixel>
CVisComposite<TPixel>::T_Image& CVisComposite<TPixel>::Warped(int index)
{
    return Frame(index);
}

template<class TPixel>
int CVisComposite<TPixel>::FindNextSame(int index, CVisTransform4x4& t)
{
	// hack mattu: temporarily clearing this flag causes the images
	//             not to be read when the [] operator is invoked below.
	
	int seqopt = SequenceOptions();
	SetSequenceOptions( 
		 seqopt&~(evissequenceMaskDelayRead|evissequenceMaskLimitMemoryUsage)|
		          evissequenceDontDelayRead|evissequenceDontLimitMemoryUsage );

    // Find the next frame with same filename
    for (int i = index+1; i < NFrames(); i++)
        if (strcmp((*this)[i].Name(), (*this)[index].Name()) == 0)
            break;

    SetSequenceOptions( seqopt );

    if (i >= NFrames())
        return -1;

    // Compute the transform between the first and last frames
    CVisTransformChain &geom = (*this)[i].TransformChain();
    CVisTransform4x4 last_t = Frame(i    ).TransformChain().CompleteTransform();
    CVisTransform4x4 base_t = Frame(index).TransformChain().CompleteTransform().Inverted();
    t = base_t * last_t;
    return i;
}

template<class TPixel>
int CVisComposite<TPixel>::FindPreviousSame(int index)
{
    assert(index < NFrames()); 
    // find the previous frame with same filename
    for (int i = 0; i <index; i++)
        if (strcmp((*this)[i].Name(), (*this)[index].Name()) == 0)
            break;
    if (i == index)
        return -1; 

    return i; 
}

#if 0
//
// Video sequence de-interlacing (for moving scenes)
//  (note that same VisPixelAverage code lives in VisResample.cpp)
//

template <class PIXELTYPE>
inline static void AverageRows(PIXELTYPE *src0, PIXELTYPE *src1, 
                               PIXELTYPE *dst, int cols)
{
    for (int i = 0; i < cols; i++)
        dst[i] = VisPixelAverage(src0[i], src1[i]);
}


template <class T_Image>
void DeInterlace(CVisImageOf<T_Image> &src, CVisImageOf<T_Image> &dst, int odd)
{
    int rows = src.Height(), cols = src.Width()*src.NBands();
    for (int r = 0; r < rows; r++) {
        int r0 = ((r & 1) == odd) ? r : (r > 0) ? r-1 : r+1;
        int r1 = ((r & 1) == odd) ? r : (r+1 < rows) ? r+1 : r-1;
        T_Image *src_ptr0 = src.PtrToFirstPixelInRow(r0 + src.Top());
        T_Image *src_ptr1 = src.PtrToFirstPixelInRow(r1 + src.Top());
        T_Image *dst_ptr  = dst.PtrToFirstPixelInRow(r  + dst.Top());
        if (r0 == r1)
            memcpy(dst_ptr, src_ptr0, cols*sizeof(T_Image));
        else
            AverageRows(src_ptr0, src_ptr1, dst_ptr, cols);
    }
}
#endif


template <class TPixel>
void CVisComposite<TPixel>::BuildPropList(CVisPropList& refproplist)
{
	T_BaseSequence::BuildPropList(refproplist);
	refproplist.AddPropReference("m_view_geometry", m_view_geometry, false);
}

//
// Virtual CVisSequence methods
// 

template <class TPixel>
const type_info& CVisComposite<TPixel>::ImageTypeInfo(void) const
{
	return typeid(T_Frame);
}

template <class TPixel>
const type_info& CVisComposite<TPixel>::ObjectTypeInfo(int iLevel) const
{
	if ((iLevel < 0) || (iLevel >= eilevelComposite))
		return typeid(CVisComposite<TPixel>);

	return CVisCollection<T_Pixel>::ObjectTypeInfo(iLevel);
}


template <class TPixel>
CVisSequenceBase *CVisComposite<TPixel>::Clone(bool fCopyThis) const
{
	if (fCopyThis)
		return new CVisComposite<TPixel>(*this);

	return new CVisComposite<TPixel>;
}


template <class TPixel>
void CVisComposite<TPixel>::SDReadWriteProperties(CVisSDStream& s,
		bool fAddComma)
{
	// UNDONE
	CVisCollection<T_Pixel>::SDReadWriteProperties(s, fAddComma);
}



// Specialized PropList helper functions for common types
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisCompositeFrame<BYTE> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisCompositeFrame<BYTE> >(0, fAlwaysTypedef); }

CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisCompositeFrame<CVisRGBA<BYTE> > const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisCompositeFrame<CVisRGBA<BYTE> > >(0, fAlwaysTypedef); }


CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisComposite<BYTE> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisComposite<BYTE> >(0, fAlwaysTypedef); }

CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisComposite<CVisRGBA<BYTE> > const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisComposite<CVisRGBA<BYTE> > >(0, fAlwaysTypedef); }


//
// Explicit template instantiation
//

#pragma warning(disable : 4660)  // template class already instantiated

template CVisComposite<CVisRGBABytePixel>;
// UNDONE:  template CVisComposite<BYTE>;

#pragma warning(default : 4660)

//
// Special code to make gray composites compile...
//

void CVisComposite<BYTE>::FormBottomLayer(CVisShape shape, int omit_current)
{
    assert(shape.Height() < 0); // not yet implemented
}

void CVisComposite<BYTE>::FormComposite(CVisShape shape)
{
    assert(shape.Height() < 0); // not yet implemented
}

static CVisShape VerticalCropExtent(CVisByteImage& img)
{
    assert(&img == 0);      // not yet implemented...
    return CVisShape();
}

// template CVisComposite<CVisByteFrame,CVisByteImage>;

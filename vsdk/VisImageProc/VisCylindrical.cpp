///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisCylindrical.cpp -- cylindrical and spherical coordinates
//
// DESCRIPTION
//  These routines are used for mapping to/from cylindrical or
//  spherical coordinates.  Currently, the only user is the
//  VideoMosaic application and its derivatives (TextureMapper).
//
//  These routines can also be used to perform "local warping",
//  i.e., fine local motions to remove misregistration "ghosts".
//
// SEE ALSO
//  VisImage.h        definition of image class library
//  VisWarp.h         definition of warping function
//  VisBlockAdjust.h    computation of local warp field (block correction)
//
// BUGS
//  
//
// DESIGN
//  
//
// HISTORY
//  10-Dec-96  Richard Szeliski (szeliski) at Microsoft
//  Split off from VisWarp.h
//
//  26-Mar-97  Richard Szeliski (szeliski) at Microsoft
//  Added local warping fields.
//
// Copyright © Microsoft Corp. 1996-2000
//
///////////////////////////////////////////////////////////////////////////


#include "VisImageProcPch.h"
#include "VisImageProc.h"


#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

#if defined(_DEBUG) && defined(_AFXDLL)
#include <afxwin.h>         // MFC core and standard components
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

//
//  VisCylindricalWarp:
//      warp from planar image into cylindrical/spherical, or vice versa
//
//  The cylindrical transformation (see IEEE CG&A article, March 1996) is:
//      u = f atan(x / f), v = f y / sqrt(x^2 + f^2)
//      x = f  tan(u / f), y =   v /  cos(u / f)
//  The spherical transformation (see Rick's notes, July 22, 1996) is:
//      u = f atan(x / f), v = f atan(y / sqrt(x^2 + f^2))
//      x = f  tan(u / f), y = f  tan(v) /  cos(u / f)
//
//  The code assumes that the optical center of the camera lies
//  at the image center, that the aspect ratio is unity.
//
//  Two optional radial distortion parameters can be used to warp the images
//  using quadractic and quartic distortion
//      r^2 <- r^2 (1 + k1 r^2 + k2 r^4)
//  where
//      r^2 = (x^2 + y^2) / f^2
//

float FocalLengthFromHFOV(CVisImageBase& src, float horizontal_field_of_view)
{
    // Focal length, measured in pixels
    double w2 = 0.5 * (src.Width() - 1);
    return (float) (w2 / tan(0.5 * horizontal_field_of_view * M_PI / 180.0));
}

float FocalLengthFromVFOV(CVisImageBase& src, float vertical_field_of_view)
{
    // Focal length, measured in pixels
    double h2 = 0.5 * (src.Height() - 1);
    return (float) (h2 / tan(0.5 * vertical_field_of_view * M_PI / 180.0));
}

float HFOVFromFocalLength(CVisImageBase& src, float focal_length)
{
    // Focal length, measured in pixels
    double w2 = 0.5 * (src.Width() - 1);
    return (float) (2.0 * 180.0 / M_PI * atan2(w2, double(focal_length)));

}

float VFOVFromFocalLength(CVisImageBase& src, float focal_length)
{
    // Focal length, measured in pixels
    double h2 = 0.5 * (src.Height() - 1);
    return (float) (2.0 * 180.0 / M_PI * atan2(h2, double(focal_length)));
}

// This function is no longer used: original shape is used instead
//  (more consistent coordinates, no flip between local warp and no warp...)
//  see Rick's research notes of 01-Apr-97

CVisShape VisCylindricalShape(CVisImageBase& src, 
                                 float focal_length, float torsion, float tilt,
                                 TVisWarp warp, int inverse, int uv_spacing,
                                 float k1, float k2)
{
    // Compute minimum size of re-sampled image
    //  We used to be able to just look at the bulging along the (x,y)
    //  axes, but with rotation, we have to look at the whole border
    //  Because of the current interface to VisCylindricalUV
    //  (no externally supplied image centers), we have to pass in
    //  a whole image.  If this is a bottle neck, increase uv_spacing
    CVisFloatImage uvi = VisCylindricalUV(src, src, focal_length, torsion, tilt,
                                        warp, ! inverse, uv_spacing, k1, k2);

    // Find the min and max values
    double x_min = uvi.Pixel(0, 0, 0), x_max = x_min;
    double y_min = uvi.Pixel(0, 0, 1), y_max = y_min;
    int nrows = uvi.Height(), ncols = uvi.Width();
    for (int i = 0; i < nrows; i++) {
        float *uvp = uvi.PtrToFirstPixelInRow(i);
        for (int j = 0; j < ncols; j++, uvp += 2) {
            float x = uvp[0], y = uvp[1];
            if (x < x_min) x_min = x;
            if (x > x_max) x_max = x;
            if (y < y_min) y_min = y;
            if (y > y_max) y_max = y;
        }
    }

    //  The shape returned is NOT anchored at (0,0): it can have any shape
    CVisShape shape = src.Shape();
#ifdef OLD
    shape.Set(0, 0,
              (int) (ceil(y_max) - floor(y_min)) + 2,
              (int) (ceil(x_max) - floor(x_min)) + 2);
#else
	shape.left = (int) floor((floor(x_min) - 1) / uv_spacing) * uv_spacing;
	shape.top = (int) floor((floor(y_min) - 1) / uv_spacing) * uv_spacing;
	shape.right = (int)  ceil(x_max) + 2;
	shape.bottom = (int)  ceil(y_max) + 2;
#endif
    return shape;
}

CVisFloatImage VisCylindricalUV(CVisImageBase& src, CVisImageBase &dst,
                              CVisTransform4x4& T1, CVisTransform4x4& T2,
                              TVisWarp warp, int inverse, int uv_spacing,
                              float k1, float k2, BOOL bRadialUsesDistCoord)
{
	double* radial_dist_table = NULL;
	double  radial_dist_scale;

    // Allocate the warped coordinate image
    CVisShape shape = inverse? src.Shape() : dst.Shape();
	shape.left = (int) floor(shape.left/(double) uv_spacing);
	shape.top = (int) floor(shape.top/(double) uv_spacing);
	shape.right = (int) ceil ((shape.right - 1)/(double) uv_spacing) + 1;
	shape.bottom = (int) ceil ((shape.bottom - 1)/(double) uv_spacing) + 1;
    shape.SetNBands(2);
    CVisFloatImage uv(shape);

    // Zero image
    if (warp == VIS_WARP_ZERO) {
        uv.ClearPixels();
        return uv;
    }

    // Fill in the warped coordinates
    double uvs = (double) uv_spacing;
    for (int i = shape.top; i < shape.bottom; i++) {
        float *uvp = uv.PtrToFirstPixelInRow(i);
        for (int j = shape.left; j < shape.right; j++, uvp += 2) {

            // First transformation (shift and rotate)
            CVisVector4 p1(j * uvs, i * uvs, 0);
            CVisVector4 q1 = T1 * p1;
            double x = q1[0], y = q1[1], z = q1[2], zi = 1.0 / z;
            double u, v;

            // Radial distortion
            if ((k1 != 0.0f || k2 != 0.0f) && inverse) {
                x *= zi, y *= zi, z = zi = 1.0;
                double r2 = x*x + y*y;
                double s = 1.0 + k1 * r2 + k2 * r2*r2;
                x *= s;
                y *= s;
            }

            // Non-linear transformation
            if (warp == VIS_WARP_NONE) {
                u = x * zi;
                v = y * zi;
            } else if (inverse) {
                u = atan2(x, z);
                if (warp == VIS_WARP_SPHERICAL)
                    v = atan2(y, _hypot(x, z));
                else
                    v = y / _hypot(x, z);
            } else {
                u = tan(x * zi);
                if (warp == VIS_WARP_SPHERICAL)
                    v = tan(y * zi) /  cos(x * zi);
                else
                    v = y * zi / cos(x * zi);
            }

            // Radial distortion
            if ((k1 != 0.0f || k2 != 0.0f) && !inverse) {
                                
                if ( bRadialUsesDistCoord )
                {
					if ( radial_dist_table == NULL )
					{
						radial_dist_table = new double[512 * 32];
						radial_dist_scale = 512. * 32. / sqrt(u*u + v*v);
						
						double rdist = 0.0;
						
						// build a table of radius corrections
						for( int i = 0; i < 512 * 32; i++ )
						{
							// use newton-raphson to solve the 5th power eq.
							int c = 0;
							
							double rundist = i / radial_dist_scale;
							
							double rdist2, rdist3, rdist4, rdist5;
							double last_rdist = rdist + 1.;
							
							while ( (fabs(rdist-last_rdist) > .0001) && c < 20 )
							{
								rdist2 = rdist * rdist;
								rdist3 = rdist2 * rdist;
								rdist4 = rdist3 * rdist;
								rdist5 = rdist4 * rdist;
								
								double num = rdist +  k1 * rdist3 + k2 * rdist5 - rundist;
								double den = 1. + 3 * k1 * rdist2 + 5 * k2 * rdist4;
								
								last_rdist = rdist;
								rdist -= num / den;
								
								c++;
							}

							radial_dist_table[i] = 
								1. + k1 * rdist2 + k2 * rdist2 * rdist2;
						}
					}
					
					// use the look-up to do radial distortion correction
					double correction = 
						radial_dist_table[int(sqrt(u*u + v*v) * radial_dist_scale + 0.5)]; 
					u /= correction;
					v /= correction;
                }
                else
                {
                    double r2 = u*u + v*v;
                    double s = 1.0 + k1 * r2 + k2 * r2*r2;
                    u /= s;
                    v /= s;
                }
            }

            // Third transformation (rotate and shift)
            CVisVector4 p2(u, v, 1.0);
            CVisVector4 q2 = T2 * p2;
            x = q2[0], y = q2[1], z = q2[2], zi = 1.0 / z;
            uvp[0] = (float) (x * zi);
            uvp[1] = (float) (y * zi);
        }
    }

	if( radial_dist_table )
	{
		delete [] radial_dist_table;
	}

    return uv;
}

CVisFloatImage VisCylindricalUV(CVisImageBase& src, CVisImageBase &dst,
                              float focal_length, float torsion, float tilt,
                              TVisWarp warp, int inverse, int uv_spacing,
                              float k1, float k2, BOOL bRadialUsesDistCoord)
{
    // Compute image centers
#ifdef OLD
    float xc0 = inverse ? 0.0f : 0.5f * (src.Width() - 1);
    float yc0 = inverse ? 0.0f : 0.5f * (src.Height() - 1);
    float xc1 = inverse ? 0.5f * (src.Width() - 1) : 0.0f;
    float yc1 = inverse ? 0.5f * (src.Height() - 1) : 0.0f;
#else
    float xc0 = 0.5f * (src.Width() - 1);
    float yc0 = 0.5f * (src.Height() - 1);
    float xc1 = 0.5f * (src.Width() - 1);
    float yc1 = 0.5f * (src.Height() - 1);
#endif

    // Guard against 0 focal length (used in null transform)
    if (focal_length <= 0.0f)
        focal_length = 1.0f;     // no warping, just local warp

    // Compute the initial transform
    CVisTransform4x4 T1;
	T1.Translate(-xc1, -yc1, focal_length);
    CVisTransform4x4 T2;

    // Compute rotation matrix
	if( torsion != 0.0 || tilt != 0.0 )
	{
		CVisTransform4x4 R;
		if (inverse)
		{
			R.Rotate(torsion);
			R *= CVisTransform4x4().Rotate(tilt, 1.0, 0.0, 0.0);
		}
		else
		{
			R.Rotate(-tilt, 1.0, 0.0, 0.0);
			R *= CVisTransform4x4().Rotate(-torsion);
		}
		
		// Optionally prepend it
		if (inverse)
			T1 = R * T1;
		else
			T2 = R;
	}

    // Scale and shift the final coordinates
    //  since the shift occurs BEFORE the z division in VisCylindrical,
    //  we need the shift values to go into the z (not w) column
    //  (see 14-Dec-96 research notes)
    T2.Scale(focal_length, focal_length);
    CVisTransform4x4 T0;
    T0[0][2] = xc0, T0[1][2] = yc0;     // NOT the usual translate
    T2 = T0 * T2;

    if( bRadialUsesDistCoord )
    {
        float f2 = focal_length*focal_length; 
        k1 *= f2;
        k2 *= f2*f2;
    }

    // Finally, call the coordinate warping computation
    return VisCylindricalUV(src, dst, T1, T2, warp, inverse, uv_spacing, k1, k2,
                            bRadialUsesDistCoord);
}

template <class TPixel>
void VisCylindricalWarp(CVisImage<TPixel> &src, 
                          CVisImage<TPixel> &dst, 
                          TVisInterpolation interpolation,
                          float focal_length, TVisWarp warp,
                          int inverse, int uv_spacing, int zero_outside,
                          float k1, float k2)
{
    CVisFloatImage uv = VisCylindricalUV(src, dst,
        focal_length, warp, 0, uv_spacing, k1, k2);
    VisInverseWarp(src, dst, uv, interpolation, uv_spacing, zero_outside, 0);
}



//
// Cylindrically warping image composition class
//

template<class TPixel>
CVisWarpedComposite<TPixel>::CVisWarpedComposite(EVisSequence evissequence)
  : CVisComposite<TPixel>(evissequence)
{
    m_warp = VIS_WARP_NONE;
    m_horizontal_fov    = 0.0f;
    m_vertical_fov      = 0.0f;
    m_focal_length      = 0.0f;
    m_torsion           = 0.0f;
    m_tilt              = 0.0f;
    m_k1 = m_k2         = 0.0f;
    m_bRadialUsesDistCoord = FALSE;
    m_enlarge_by        = 0.0f;
    m_warp_grid_spacing = 1;
    m_local_warp        = 0;
    m_local_warp_blur   = 2.0;

    m_sphericalMap      = 1;      
    m_mapLength         = 512;         
}

int CVisLocalWarp::m_use_B_channel = 1;     // use blue channel when storing warps
int CVisLocalWarp::m_write_as_float = 0;    // write out the warp/flow directly
int CVisLocalWarp::m_warp_on_read = 1;      // force re-warp of images on readin

template<class TPixel>
void CVisWarpedComposite<TPixel>::InvalidateWarp(int dont_delete_local_warps)
{
    if (dont_delete_local_warps < 2)
        m_warp_uv.Deallocate();  // reset to empty
//    m_warped.DeleteAll();
//    if (! dont_delete_local_warps)
//        m_warp_local.DeleteAll();
	for (int i = 0; i < NFrames(); ++i)
	{
		At(i).ImageWarped().Deallocate();
		if (! dont_delete_local_warps)
			At(i).ImageLocalWarp().Deallocate();
	}
    Invalidate();
}

template<class TPixel>
void CVisWarpedComposite<TPixel>::ClearColorAdjust()
{
	for (int i = 0; i < NFrames(); ++i)
	{
		At(i).ClearColorAdjust();
		At(i).ImageWarped().Deallocate();
	}
	Invalidate();
}

template<class TPixel>
void CVisWarpedComposite<TPixel>::SetWarpMode(TVisWarp w)
{
    if (m_warp == w)
        return;
    m_warp = w;
    InvalidateWarp();
}

template<class TPixel>
void CVisWarpedComposite<TPixel>::SetLocalWarpMode(int is_local)
{
    m_local_warp = is_local;
    // don't invalidate:  may want to re-use later
}

template<class TPixel>
void CVisWarpedComposite<TPixel>::SetWarpGridSpacing(int s)
{
    if (m_warp_grid_spacing == s)
        return;
    m_warp_grid_spacing = s;
    InvalidateWarp();
}

template<class TPixel>
void CVisWarpedComposite<TPixel>::SetHorizontalFOV(float fov)
{
    m_horizontal_fov = m_vertical_fov = m_focal_length = fov;
    if (! IsEmpty() && fov > 0.0f) {
        m_focal_length = FocalLengthFromHFOV(Frame(0),
                                             m_horizontal_fov);
        m_vertical_fov = VFOVFromFocalLength(Frame(0),
                                             m_focal_length);
    }
    InvalidateWarp();
}

template<class TPixel>
void CVisWarpedComposite<TPixel>::SetVerticalFOV(float fov)
{
    m_horizontal_fov = m_vertical_fov = m_focal_length = fov;
    if (! IsEmpty() && fov > 0.0f) {
        m_focal_length = FocalLengthFromVFOV(Frame(0),
                                             m_vertical_fov);
        m_horizontal_fov = HFOVFromFocalLength(Frame(0),
                                               m_focal_length);
    }
    InvalidateWarp();
}

template<class TPixel>
void CVisWarpedComposite<TPixel>::SetFocalLength(float f)
{
    m_horizontal_fov = m_vertical_fov = m_focal_length = f;
    if (! IsEmpty() && f > 0.0f) {
        m_horizontal_fov = HFOVFromFocalLength(Frame(0),
                                               m_focal_length);
        m_vertical_fov   = VFOVFromFocalLength(Frame(0),
                                               m_focal_length);
    }
    InvalidateWarp();
}

template<class TPixel>
void CVisWarpedComposite<TPixel>::SetTorsion(float angle)
{
    m_torsion = angle;
    InvalidateWarp();
}

template<class TPixel>
void CVisWarpedComposite<TPixel>::SetTilt(float angle)
{
    m_tilt = angle;
    InvalidateWarp();
}

template<class TPixel>
void CVisWarpedComposite<TPixel>::SetK1(float k1)
{
    m_k1 = k1;
    InvalidateWarp();
}

template<class TPixel>
void CVisWarpedComposite<TPixel>::SetK2(float k2)
{
    m_k2 = k2;
    InvalidateWarp();
}

template<class TPixel>
void CVisWarpedComposite<TPixel>::SetRadialUsesDistCoord(BOOL val)
{
    m_bRadialUsesDistCoord = val;
    InvalidateWarp();
}

template<class TPixel>
void CVisWarpedComposite<TPixel>::SetEnlargeBy(float enlarge_by)
{
    m_enlarge_by = enlarge_by;
    InvalidateWarp();
}

template<class TPixel>
CVisWarpedComposite<TPixel>::T_Image& CVisWarpedComposite<TPixel>::Warped(int index)
{
    // Progress report
    if (((*this)[index].Height() == 0)
			&& ((EVisSequence() & evissequenceMaskDelayRead)
					== evissequenceDelayReadUntilNeeded)) {
        ProgressReport("Reading %s", (*this)[index].Name());
    }

	assert(index < NFrames());

    if (DisplayMode() == VIS_COMPOSITE_VIEW_SEQUENCE ||
			!m_local_warp && m_focal_length == 0.0 || 
			m_warp == VIS_WARP_NONE && ! m_local_warp &&
			m_torsion == 0.0 && m_tilt == 0.0 && m_k1 == 0.0 && m_k2 == 0.0)
	{
		m_frameTmp = Frame(index).Cropped(); //K .Cropped
        return  m_frameTmp; // just show selected image
	}
    else if (At(index).ImageWarped().IsValid())
	{
        m_frameTmp =  At(index).ImageWarped();
		if ((At(index).RectCrop().right - At(index).RectCrop().left) > 0)
			m_frameTmp.SetRect(At(index).RectCrop());  // UNDONE:  Is this cropping needed?
        return  m_frameTmp; // just show selected image
	}

    // Progress report
    ProgressReport("Warping frame %d", index);

    // Allocate new frames and new warped image
// UNDONE:  Should we change this to call Deallocate on each local warp and warped image?
//    while (m_warped.NFrames() <= index) {
//        m_warped.Insert("");
//        m_warp_local.Insert("");  // harry 7-14-97 
//    }
    GlobalWarp(index);       // force recomputation of m_warped_shape, if necessary
	T_Image& refimageWarped = At(index).ImageWarped();
    refimageWarped.Allocate(m_warped_shape);

	// Want to use cropped image below
	// UNDONE:  Is this cropping needed?
	m_frameTmp = refimageWarped;
	if ((m_frameTmp.IsValid()) && 
		((At(index).RectCrop().right - At(index).RectCrop().left) > 0))
		m_frameTmp.SetRect(At(index).RectCrop());

    // Compute the warped image
    CVisFloatImage& warp_field = m_local_warp ? LocalWarp(index) : GlobalWarp(index);
    VisInverseWarp(Frame(index).Cropped(), m_frameTmp, warp_field, 
                    m_interpolation, m_warp_grid_spacing, 1, 0);

	// adjust the color
	VisAdjustColor( m_frameTmp, At(index).ColorAdjust(VIS_COLOR_LEFT),
		            At(index).ColorAdjust(VIS_COLOR_RIGHT),
					0.0 );

    return m_frameTmp;
}

// Simon Baker 24-Aug-97 - I've hacked Warped(), GlobalWarp(), and LocalWarp()
// to *try* to get them to work with images of varying shapes. I've added the
// "index" argument to GlobalWarp which is used to specify which image is used
// in the computation of m_warped_shape. I've also added the second disjunction 
// to the test of whether to recompute m_warped_shape and m_warp_uw. 
template<class TPixel>
CVisFloatImage& CVisWarpedComposite<TPixel>::GlobalWarp(int index)
{
    // Compute the warping function (reuse this between calls if f same)
    if (m_warp_uv.Height() == 0
#if 0
        // 18-Oct-99 (Rick): no longer check for same shape, since
        //  m_enlarge_by can make these unequal.  This breaks composites
        //  where all the images are not the same size...
		|| Frame(index).Shape() != m_warped_shape
#endif
        )
    {
		// First compute m_warped_shape
        T_Image &img = Frame(index);
#if 0 // no longer used: see notes before VisCylindricalShape, above...
        m_warped_shape = 
            VisCylindricalShape(img, m_focal_length, m_torsion, m_tilt,
                                  m_warp, 0, m_warp_grid_spacing, m_k1, m_k2);
#else
        m_warped_shape = img.Shape();
#endif

        // Optionally enlarge (or shrink) the warped image size
        if (m_enlarge_by != 0.0f && m_enlarge_by > -0.6f) {
            int extra_width  = int(m_warped_shape.Width()  * m_enlarge_by);
            int extra_height = int(m_warped_shape.Height() * m_enlarge_by);
            m_warped_shape.InflateRect(extra_width, extra_height);
        }

		// Recompute m_warp_uv based on m_warped_shape
        T_Image warped(m_warped_shape);   // temporary
        m_warp_uv =
            VisCylindricalUV(img, warped, m_focal_length, m_torsion, m_tilt,
                             m_warp, 0, m_warp_grid_spacing, m_k1, m_k2,
                             m_bRadialUsesDistCoord );
    }
    return m_warp_uv;
}

template<class TPixel>
CVisFloatImage& CVisWarpedComposite<TPixel>::LocalWarp(int index)
{
    // Allocate and initialize local warping field(s)
// UNDONE:  Should we change this to call Deallocate on each warped image?
//    while (m_warp_local.NFrames() <= index) {
//        m_warp_local.Insert("");
//    }
    CVisFloatImage& warp_field = At(index).ImageLocalWarp();
    GlobalWarp(index);   // force allocation and computation, if necessary
    if (warp_field.Shape() == m_warp_uv.Shape())
        return warp_field;
    if (m_local_warp > 1 && index > 0) {
        // Special mode: all local warp fields are shared
        CVisFloatImage& warp_0 = LocalWarp(0);
        warp_field = warp_0;    // share the same storage
    } else {
        warp_field.Allocate(m_warp_uv.Shape());
        m_warp_uv.CopyPixelsTo(warp_field);
        const char *name = warp_field.Name();
        if (name == 0 || name[0] == 0) {
            char *new_name = WarpFilename(index, "W", 0, "msv");
            // UNDONE:  this might break writing out as color images?
            warp_field.SetName(new_name);
            delete new_name;
        }
    }
    return warp_field;
}

//
// Overridden collection operations:
//  these are needed to keep the m_warped and m_warp_local collections consistant
//
#ifdef TAKEOUT

template<class TFrame>
void CVisWarpedComposite<TFrame>::Insert(TFrame &frame, int index, int new_geometry)
{
    CVisComposite<TFrame>::Insert(frame, index, new_geometry);
    m_warped.Insert("");    // corresponding warped image (unallocated)
    m_warp_local.Insert("");    // corresponding warp_local image (unallocated)
}

template<class TFrame>
void CVisWarpedComposite<TFrame>::Insert(const char *filename, int index)
{
    CVisComposite<TFrame>::Insert(filename, index);
    m_warped.Insert("");    // corresponding warped image (unallocated)
    m_warp_local.Insert("");    // corresponding warp_local image (unallocated)
}

template<class TFrame>
void CVisWarpedComposite<TFrame>::Raise(int index, int to_top)
{
    CVisComposite<TFrame>::Raise(index, to_top);
    if (index < m_warped.NFrames())
        m_warped.Raise(index, to_top);
    if (index < m_warp_local.NFrames())
        m_warp_local.Raise(index, to_top);
}

template<class TFrame>
void CVisWarpedComposite<TFrame>::Lower(int index, int to_top)
{
    CVisComposite<TFrame>::Lower(index, to_top);
    if (index < m_warped.NFrames())
        m_warped.Lower(index, to_top);
    if (index < m_warp_local.NFrames())
        m_warp_local.Lower(index, to_top);
}

template<class TFrame>
void CVisWarpedComposite<TFrame>::Delete(int index)
{
    CVisComposite<TFrame>::Delete(index);
    if (index < m_warped.NFrames())
        m_warped.Delete(index);
    if (index < m_warp_local.NFrames())
        m_warp_local.Delete(index);
}

template<class TFrame>
void CVisWarpedComposite<TFrame>::DeleteAll()
{
    CVisComposite<TFrame>::DeleteAll();
    m_warped.DeleteAll();
    m_warp_local.DeleteAll();
}
#endif // TAKEOUT

//
// File I/O
//  Write the composite of images into a descriptive text file,
//  and optionally write out (back) the constituent files.
//  Note that only the metadata (view parameters, etc), differs from a collection
//

template<class TPixel>
void CVisWarpedComposite<TPixel>::ReadMetaData(FILE *stream)
{
    CVisComposite<T_Pixel>::ReadMetaData(stream);
    int warp_mode;  float f, hfov, vfov, torsion = 0.0f, tilt = 0.0f, k1 = 0.0f, k2 = 0.0f, enlarge_by = 0.0f;
    char line[1024];
    if (fgets(line, 1024, stream) == 0)
        return;
    if (strcmp(line, "# Final image shape (srow, scol, nrows, ncols):\n") == 0) {
        if (fgets(line, 1024, stream) == 0)     // skip image size line
            return;
        if (fgets(line, 1024, stream) == 0)     // get the next one
            return;
    }
    if (strcmp(line, "# Cylindrical warp (warp, focal_length, horizontal FOV, vertical FOV):\n") == 0) {
        // backwards compatibility
        if (fscanf(stream, "%d %f %f %f\n",
                   &warp_mode, &f, &hfov, &vfov) != 4)
            return;
    } else if (strcmp(line, "# Cylindrical warp (warp, focal_length, horizontal FOV, vertical FOV, torsion, tilt):\n") == 0) {
        if (fscanf(stream, "%d %f %f %f %f %f\n",
                   &warp_mode, &f, &hfov, &vfov, &torsion, &tilt) != 6)
            return;
    } else if (strcmp(line, "# Cylindrical warp (warp, focal_length, horizontal FOV, vertical FOV, torsion, tilt, k1, k2):\n") == 0) {
        if (fscanf(stream, "%d %f %f %f %f %f %f %f\n",
                   &warp_mode, &f, &hfov, &vfov, &torsion, &tilt, &k1, &k2) != 8)
            return;
    } else if (strcmp(line, "# Cylindrical warp (warp, focal_length, horizontal FOV, vertical FOV, torsion, tilt, k1, k2, enlarge_by):\n") == 0) {
        if (fscanf(stream, "%d %f %f %f %f %f %f %f %f\n",
                   &warp_mode, &f, &hfov, &vfov, &torsion, &tilt, &k1, &k2, &enlarge_by) != 9)
            return;
    } else
        return;
    SetWarpMode((TVisWarp) warp_mode);
    if (f > 0.0)
        SetFocalLength(f);  // HFOV and VFOV are recomputed from focal length
    SetTorsion(torsion);
    SetTilt(tilt);
    SetK1(k1);
    SetK2(k2);
    SetEnlargeBy(enlarge_by);
    ReadWarpCollection(stream);
}

template<class TPixel>
void CVisWarpedComposite<TPixel>::WriteMetaData(FILE *stream)
{
    CVisComposite<T_Pixel>::WriteMetaData(stream);
    if (DisplayMode() == VIS_COMPOSITE_VIEW_COMPLETE) {
        fprintf(stream, "# Final image shape (srow, scol, nrows, ncols):\n");
        T_Image& img = CroppedBottomLayer();
        CVisRect r= img.Shape();
        fprintf(stream, "%d %d %d %d\n",
            r.top, r.left, r.Height(), r.Width());
    }
    if (WarpMode() != VIS_WARP_NONE || FocalLength() > 0.0 || LocalWarpMode()) {
        if (EnlargeBy() != 0.0)
        fprintf(stream, "# Cylindrical warp (warp, focal_length, horizontal FOV, vertical FOV, torsion, tilt, k1, k2, enlarge_by):\n%d %.2f %.2f %.2f %.2f %.2f %.6f %.6f %.2f\n",
                (int) WarpMode(), FocalLength(), HorizontalFOV(), VerticalFOV(), 
                Torsion(), Tilt(), K1(), K2(), EnlargeBy());
        else
        fprintf(stream, "# Cylindrical warp (warp, focal_length, horizontal FOV, vertical FOV, torsion, tilt, k1, k2):\n%d %.2f %.2f %.2f %.2f %.2f %.6f %.6f\n",
                (int) WarpMode(), FocalLength(), HorizontalFOV(), VerticalFOV(), 
                Torsion(), Tilt(), K1(), K2());
    }
// UNDONE:  We need a condition similar to m_local_warp.NFrames() > 0.
//    if (LocalWarpMode() && m_warp_local.NFrames() > 0)
    if (LocalWarpMode() && (NFrames() > 0) && Front().ImageLocalWarp().IsValid())
        WriteWarpCollection(stream);
}

//
//  To write out the warp field, we first scale it to 12-bit fixpoint,
//  then store the bits in the RGB fields of a color sequence
//

struct MinMaxFn {
	double m_dblMin, m_dblMax;
	MinMaxFn() : m_dblMin(0.0f), m_dblMax(0.0f) {}; // UNDONE:  Use type limits instead of 0?
	void operator()(float u, float u0)
	{
		u -= u0;
		if (u < m_dblMin) m_dblMin = u;
		if (u > m_dblMax) m_dblMax = u;
	}
};

template<class TPixel>
char* CVisWarpedComposite<TPixel>::WarpFilename(int index, char *prefix,
                                                char *suffix, char *extension) const
{
    // Make up a new name by pre-pending prefix (usually "W")
    // TODO (potential bug): can result in memory leak if result is not freed
    const char *filename = Frame(index).Name();
    int n_extra = strlen(prefix) + (suffix ? strlen(suffix) : 0) +
        (extension ? strlen(extension) : 0);
    char *new_name = new char[strlen(filename)+n_extra+2];
    char *e1 = strrchr(filename, '/'), *e2 = strrchr(filename, '\\');
    char *e3 = strrchr(filename, ':');
    const char *slash = (e2) ? e2 : (e1) ? e1 : (e3) ? e3 : (filename - 1);
    strncpy(new_name, filename, (slash - filename)+1);
    new_name[(slash - filename)+1] = 0;
    strcat(new_name, prefix);
    strcat(new_name, slash+1);
    if (suffix || extension) {
        if (extension == 0) {
            char *dot = strrchr(filename, '.');
            extension = (dot) ? (dot + 1) : "";
        }
        char *new_dot = strrchr(new_name, '.');
        if (new_dot == 0)
            new_dot = &new_name[strlen(new_name)];
        strcpy(new_dot, suffix ? suffix : "");
        strcat(new_name, ".");
        strcat(new_name, extension);
    }
    return new_name;
}

template<class TPixel>
void CVisWarpedComposite<TPixel>::WarpToOffset(CVisRGBAByteCollection &color_coll)
{
    // Compute a new collection of color offset images
	// Simon Baker - 25-Aug-97 
	// Compute the maximum absolute flow value accross m_warp_local - m_warp_uv
	// (See comment before VisMaxAbsVal for why not using VisMaxAbsVal)
	MinMaxFn fn;
	float max_val = 0, next_max;
	for(int i = 0; i < NFrames(); i++) {
		LocalWarp(i);
		VisMap2(fn, At(i).ImageLocalWarp(), m_warp_uv);
		next_max = (fn.m_dblMax > -fn.m_dblMin) ? fn.m_dblMax : (fn.m_dblMin != 0.0) ? -fn.m_dblMin : 1.0;
		if (next_max > max_val) {
			max_val = next_max;
		}
	}

    // Compute the appropriate scaling for the warp fields based on max value
    const int bits = 12, iscale = (1 << (bits-1));
    m_warp_grid_scale = max_val / (iscale - 1);   // 12 bit precision
    float inv_scale = 1.0f / m_warp_grid_scale;
    
    // Convert the warp fields into fixpoint RGB images
    color_coll.DeleteAll();
    for (i = 0; i < NFrames(); i++) {

        // Progress report
        ProgressReport("Warp to flow, frame %d", i);

		// Simon Baker 25-Aug-97: Added to force recomputation of m_warp_uv
		// for image collections of varying image size and shape
		LocalWarp(i);

        // Allocate the color image and set its filename
        CVisFloatImage &uv = At(i).ImageLocalWarp();
        CVisShape s = CVisRect(uv.Shape());
        CVisRGBAByteImage cuv(s);
        color_coll.PushBack(CVisRGBAByteFrame(cuv));
        if (uv.Name()[0] == 0) {
            char *new_name = WarpFilename(i, "W");
            color_coll.Frame(i).SetName(new_name);
            delete new_name;
        } else
            color_coll.Frame(i).SetName(uv.Name());

        // Copy the flow values
        for (int r = 0; r < uv.Height(); r++) {
            float *pf = uv.PtrToFirstPixelInRow(r+uv.Top());
            float *bf = m_warp_uv.PtrToFirstPixelInRow(r+m_warp_uv.Top());
            CVisRGBABytePixel *pc = cuv.PtrToFirstPixelInRow(r+cuv.Top());
            for (int c = 0; c < uv.Width(); c++, pf += 2, bf += 2, pc++) {
                int iu = int((pf[0] - bf[0]) * inv_scale + 0.5) + iscale;
                int iv = int((pf[1] - bf[1]) * inv_scale + 0.5) + iscale;
                pc->SetR(iu >> 4);
                pc->SetG(iv >> 4);
                if (CVisLocalWarp::m_use_B_channel)
                    pc->SetB(((iu & 0xf) << 4) + (iv & 0xf));
                else
                    pc->SetB(0);
                pc->SetA(255);
            }
        }
    }
}

template<class TPixel>
void CVisWarpedComposite<TPixel>::WriteWarpCollection(FILE *stream)
{
    // Convert the warp fields into fixpoint RGB images or float collection
    CVisRGBAByteCollection color_coll;
    CVisFloatCollection    float_coll;
    if (CVisLocalWarp::m_write_as_float == 0)
        WarpToOffset(color_coll);
    else {
        // Make a temporary flow collection
        //  (this code added to temporarily support CVisMotionEstimator class).
        for (int i = 0; i < NFrames(); i++) {
            CVisFloatImage &uv = At(i).ImageLocalWarp();
            if (uv.Name()[0] == 0) {
                char *new_name = WarpFilename(i, "W");
                uv.SetName(new_name);
                delete new_name;
            }
            float_coll.PushBack(CVisFloatFrame(uv));
        }
        m_warp_grid_scale = -1;     // special value for CVisFloatCollection
    }

    // Write out collection onto stream and write scaled warp images
    fprintf(stream, "# Local warp (grid_spacing, fixpoint_scale)\n%d %f\n",
            m_warp_grid_spacing, m_warp_grid_scale);
    fprintf(stream, "# ImageCollection, frame = filename_only\n");
        // see VisCollection.cpp static variable collection_header
    if (CVisLocalWarp::m_write_as_float == 0) {
        color_coll.WriteCollection(stream);
        color_coll.WriteFiles();
    } else {
        float_coll.WriteCollection(stream);
        float_coll.WriteFiles();
    }
}

template<class TPixel>
void CVisWarpedComposite<TPixel>::OffsetToWarp(CVisRGBAByteCollection &color_coll)
{
    // Convert the fixpoint RGB images into warp fields
    const int bits = 12, iscale = (1 << (bits-1));
    for (int i = 0; i < NFrames(); i++) {

        // Progress report
        ProgressReport("Flow to warp, frame %d", i);

        // Copy the flow values
        CVisFloatImage &uv = LocalWarp(i);
        CVisRGBAByteImage &cuv = color_coll.Frame(i);

		// Simon Baker 25-Aug-97: In LayeredStereo the shape of the local warp field
		// which we have just read into color_coll will not be the same as the image
		// it corresponds to (ie. the shape of uv) because warped images always
		// have (top, left) = (0,0). So, reset the origin.
		if (CVisRect(uv.Shape()) != CVisRect(cuv.Shape())) {
			cuv.ResetOrigin(uv.Left(), uv.Top());
		}
        assert(CVisRect(uv.Shape()) == CVisRect(cuv.Shape()));

        for (int r = 0; r < uv.Height(); r++) {
            float *pf = uv.PtrToFirstPixelInRow(r+uv.Top());
            float *bf = m_warp_uv.PtrToFirstPixelInRow(r+m_warp_uv.Top());
            CVisRGBABytePixel *pc = cuv.PtrToFirstPixelInRow(r+cuv.Top());
            for (int c = 0; c < uv.Width(); c++, pf += 2, bf += 2, pc++) {
                int iu = ((int(pc->R()) & 0xff) << 4) + ((pc->B() >> 4) & 0xf);
                int iv = ((int(pc->G()) & 0xff) << 4) + ((pc->B() >> 0) & 0xf);
                pf[0] = m_warp_grid_scale * (iu - iscale) + bf[0];
                pf[1] = m_warp_grid_scale * (iv - iscale) + bf[1];
            }
        }
    }
}

template<class TPixel>
void CVisWarpedComposite<TPixel>::ReadWarpCollection(FILE *stream)
{
    // Read in collection from stream and read scaled warp images
    char line[1024];
    if (fgets(line, 1024, stream) == 0)
        return;
    if (strcmp(line, "# Local warp (grid_spacing, fixpoint_scale)\n") != 0)
        return;
    if (fscanf(stream, "%d %f\n", &m_warp_grid_spacing, &m_warp_grid_scale) != 2)
        return;
    if (fgets(line, 1024, stream) == 0)
        return;
    if (strcmp(line, "# ImageCollection, frame = filename_only\n") != 0)
        return;
    CVisLocalWarp::m_write_as_float = (m_warp_grid_scale < 0);

    // Read and convert either a color collection or a float collection
    CVisRGBAByteCollection color_coll;
    CVisFloatCollection    float_coll;
    if (CVisLocalWarp::m_write_as_float == 0) {
        color_coll.SetWorkingDir(m_working_dir);
        color_coll.ReadCollection(stream);
        color_coll.ReadFiles();
    } else {
        float_coll.SetWorkingDir(m_working_dir);
        float_coll.ReadCollection(stream);
        float_coll.ReadFiles();
    }
    SetLocalWarpMode(1);    // force creation of local warp fields
    
    // Convert the fixpoint RGB images into warp fields
    if (CVisLocalWarp::m_write_as_float == 0)
        OffsetToWarp(color_coll);
    else {
        // Just copy the warp fields directly
        for (int i = 0; i < NFrames(); i++) {
            CVisFloatImage &uv = At(i).ImageLocalWarp();
            uv = float_coll[i];
        }
    }

    // Force the re-warping of image by calling Warped()
    for (int i = 0; i < NFrames() && CVisLocalWarp::m_warp_on_read; i++)
        Warped(i);
}


//
//  Final straightening, re-sizing, and cropping
//    rotate/scale so that the sequence is level and of desired size,
//    and set cropping parameters
//

static CVisShape VerticalCropExtent(CVisRGBAByteImage& img)
{
    // Compute the horizontal projections
    int nrows = img.Height(), ncols = img.Width(), r, c;
    int *all_visible = new int[nrows];
    for (r = 0; r < nrows; r++) {
        CVisRGBABytePixel *p = img.PtrToFirstPixelInRow(r+img.Top());
        for (c = 0; c < ncols; c++)
            if (p[c].A() != 255)
                break;
        all_visible[r] = (c == ncols);
    }

    // Find the largest contiguous range
    int start_longest = 0, n_longest = 0;
    for (r = 0; r < nrows; r++)
        if (all_visible[r]) {
            for (int s = r; r < nrows && all_visible[r]; r++);
            if (r - s > n_longest)
                start_longest = s, n_longest = r - s;
    }
    delete all_visible;

    // Return the cropped shape
    CVisShape sh = img.Shape();
	sh.top += start_longest;
	sh.bottom = sh.top + n_longest;
    return sh;
}

template<class TPixel>
void CVisWarpedComposite<TPixel>::StraightenToWidth(int base_frame, 
                                              int desired_width, 
                                              int crop_vertically)
{
    // Find matching end frame and its transform
    CVisTransform4x4 translation;
    int last_frame = FindNextSame(base_frame, translation);
    if (last_frame <= base_frame) {
        char msg[1024];
        sprintf(msg, "Frame %d has not been composited twice", base_frame);
        throw CVisError(msg, eviserrorOpFailed, "StraightenToWidth");
    }

    // Negative translation:  switch roles of base and last
    if (translation[0][3] < 0) {
        int tmp = last_frame;
        last_frame = base_frame;
        base_frame = tmp;
    }

    // Check if a pure translation
    CVisTransform4x4 M0 = Frame(base_frame).TransformChain().CompleteTransform();
    CVisTransform4x4 M1 = Frame(last_frame).TransformChain().CompleteTransform();
    if (M1[0][0] != 1.0 || M1[0][1] != 0.0 ||
        M1[1][0] != 0.0 || M1[1][1] != 1.0) {

        // Compute incremental scale/rotation to restore unity,
        // assuming it is a scale/rotate (see Jan 22, 1997 notes)
        int n = abs(last_frame - base_frame);
        double scale = pow(M1[0][0] * M1[1][1] - M1[0][1] * M1[1][0], -0.5 / n);
        double theta = atan2(M1[0][1], M1[0][0]) / n;
        double c = scale * cos(theta), s = scale * sin(theta);
        CVisTransform4x4 R(c, c, 1.0, 1.0);
        R[0][1] = -s, R[1][0] = s;  // is this sign right?
        CVisTransform4x4 Ri(1.0, 1.0, 1.0, 1.0);

        // Postmultiply all transformations
        int inc = (last_frame > base_frame) ? 1 : -1;
        for (int i = 0; i <= n; i++) {
            int j = base_frame + inc * i;
            CVisTransform4x4& M = Frame(j).TransformChain()[0];
            M = M * Ri;
            Ri = R * Ri;
        }
        M0 = Frame(base_frame).TransformChain().CompleteTransform();
        M1 = Frame(last_frame).TransformChain().CompleteTransform();
    }

    // Compute the base transform needed to achieve the desired width
    double x0 = M0[0][3], y0 = M0[1][3];
    double x1 = M1[0][3], y1 = M1[1][3];
    double  c = (x1 - x0) / desired_width;
    double  s = (y1 - y0) / desired_width;
    CVisTransformChain view_geometry;
    CVisTransform4x4& M = view_geometry[0];
    M[0][0] = c, M[0][1] = -s, M[0][3] = x0;
    M[1][0] = s, M[1][1] =  c, M[1][3] = y0;
    SetViewGeometry(view_geometry);

    // Compute offset to center of base tile
    int center[2];
    LayerCenter(base_frame, center);
    // sanity check: a different way to compute this...
    CVisShape base_shape = Frame(base_frame).Shape();
    int base_center = (base_shape.left + base_shape.right - 1) / 2;

    // Crop to desired width
    float sav_bbox = BoundingBoxSize();
    SetBoundingBoxSize(0);
    SetDisplayMode(VIS_COMPOSITE_VIEW_COMPLETE);
    T_Image& comp = BottomLayer();
    T_Image  crop = comp.SubImage(comp.Top(), center[0], comp.Height(), desired_width);
    SetCroppedSize(crop.Shape());
    SetBoundingBoxSize(sav_bbox);

    // Optional vertical cropping
    if (crop_vertically) {
        T_Image cropped_alpha = BottomAlpha().SubImage(crop.Shape());
        CVisShape crop_shape = VerticalCropExtent(cropped_alpha);
        SetCroppedSize(crop_shape);
    }
}

//
// Convert a cylindrical panorama into a regular (pure rotation) panorama
//  This code adapted from StraightenToWidth
//

template<class TPixel>
void CVisWarpedComposite<TPixel>::CylindricalToPanoramic(int base_frame)
{
    // Find matching end frame and its transform
    CVisTransform4x4 translation;
    int last_frame = FindNextSame(base_frame, translation);
    if (last_frame <= base_frame) {
        char msg[1024];
        sprintf(msg, "Frame %d has not been composited twice", base_frame);
        throw CVisError(msg, eviserrorOpFailed, "CylindricalToPanoramic");
    }

    // Negative translation:  switch roles of base and last
    if (translation[0][3] < 0) {
        int tmp = last_frame;
        last_frame = base_frame;
        base_frame = tmp;
    }

    // Compute the tilt in the sequence and the focal length
    CVisTransform4x4 M0 = Frame(base_frame).TransformChain().CompleteTransform();
    CVisTransform4x4 M1 = Frame(last_frame).TransformChain().CompleteTransform();
    double x0 = M0[0][3], y0 = M0[1][3];
    double x1 = M1[0][3], y1 = M1[1][3];
    double dx = x1 - x0, dy = y1 - y0;
    double twist = 180.0 / M_PI * atan2(dy, dx) + m_torsion;
    double circ  = _hypot(dx, dy);
    double fov   = circ / (2.0 * M_PI);

    // Compute the rotation matrix for each frame
    CVisTransform4x4 M0i = M0.Inverted();
    for (int f = 0; f < NFrames(); f++) {
        CVisTransform4x4 M = Frame(f).TransformChain().CompleteTransform();
        CVisTransform4x4 Md = M * M0i;

        double pan  = Md[0][3] * 360.0 / circ;
        double tilt = (Md[1][3] - Md[0][3] * dy / dx) * 360 / circ + m_tilt;

        // Form the rotation and view matrices
//        CVisTransform4x4 R = VisRotateMatrix( twist, 0.0, 0.0, 1.0) *
//                             VisRotateMatrix( tilt,  1.0, 0.0, 0.0) *
//                             VisRotateMatrix(-pan,   0.0, 1.0, 0.0);
        CVisTransform4x4 R;
		R.Rotate(-pan,   0.0, 1.0, 0.0);
		R.Rotate(tilt,  1.0, 0.0, 0.0);
		R.Rotate(twist, 0.0, 0.0, 1.0);

        CVisTransform4x4 V(1.0/fov, 1.0/fov, 0.0, 0.0);
        V[2][3] = V[3][2] = 1.0;    // transpose z and w
        CVisShape s = Frame(f).Shape();
        double cx = 0.5 * (s.Width() - 1);
        double cy = 0.5 * (s.Height() - 1);
        CVisTransform4x4 T;
		T.Translate(-cx, -cy);

        // Form the three element geometry chain and replace current
        CVisTransformChain Mn = CVisTransformChain(T);
		Mn.push_front(V);
		Mn.push_front(R);
        Frame(f).TransformChain() = Mn;
    }

    // Reset the view mode to no warp
    SetFocalLength(0.0f);
    SetTorsion(0.0f);
    SetTilt(0.0f);
    SetWarpMode(VIS_WARP_NONE);              // no warp, flat images
    InvalidateWarp();
}

//
// Update the local warp field based on sparse flow computation
//  (see BlockAdjust.h).
//
// The idea is to compute the incremental flow correction by
//  splatting the patch-based flow using a bilinear kernel.
//

static void VisAccumulateFlow(CVisFloatImage& patch, SVisPatchFlow* pf, 
                              int size, int scale, bool ignore_covariances)
{
    float u = -pf->u;   // inverse warp, so negate sign of flow
    float v = -pf->v;
    float x = pf->x + pf->u;
    float y = pf->y + pf->v;
    for (int r = patch.Top(); r < patch.Bottom(); r++) {
        float *ff = patch.PtrToFirstPixelInRow(r);
        SVisPatchFlow *bf = (SVisPatchFlow *) ff;
        for (int c = patch.Left(); c < patch.Right(); c++, bf++) {

            // Compute a bi-linear weighting term
            float dx = x - c * scale;
            float dy = y - r * scale;
            float wx = __max(0, size - fabs(dx));
            float wy = __max(0, size - fabs(dy));
            float w  = wx * wy;

            // Increment the accumulator buffer
           if (ignore_covariances) {
            bf->C[0][0] += w;
            bf->C[1][1] += w;
            bf->u += w * u;
            bf->v += w * v;
           } else {
            bf->C[0][0] += w * pf->C[0][0];
            bf->C[0][1] += w * pf->C[0][1];
            bf->C[1][0] += w * pf->C[1][0];
            bf->C[1][1] += w * pf->C[1][1];
            bf->u += w * (pf->C[0][0] * u + pf->C[0][1] * v);
            bf->v += w * (pf->C[1][0] * u + pf->C[1][1] * v);
           }
            bf->n += w;
        }
    }
}

template<class TPixel>
void CVisWarpedComposite<TPixel>::UpdateLocalWarp(int index, CVisFloatImage& patch_flow_input,
                                               int patch_spacing, bool ignore_covariances)
{
    // If m_local_warp == 2, we interpret this as 
    // a GLOBAL warp adjustment from all NFrames() (common local warp field).

    // Create temporary images for accumulating flow
    CVisFloatImage &uv = LocalWarp(index);  // force allocation, if necessary
    CVisShape s = uv.Shape();
    s.SetNBands(sizeof(SVisPatchFlow) / sizeof(float));
    CVisFloatImage buf(s);
    buf.ClearPixels();
    int win_size = int(patch_spacing * m_local_warp_blur / m_warp_grid_spacing);

    // Minimum weighting (to feather flow towards 0)
    static float min_weight = 0.1f;
    float min_C_diag = min_weight * win_size * win_size;

    // Optionally iterate over the frames, in case of a global warp
   int n_frames = ((m_local_warp == 1)? 1 : NFrames());
   for (int frame = 0; frame < n_frames; frame++) {
    CVisFloatImage& patch_flow = (&patch_flow_input)[frame];

    // Iterate over the patch results, accumulating flow estimates
    for (int r = 0; r < patch_flow.Height(); r++) {
        float *ff = patch_flow.PtrToFirstPixelInRow(r + patch_flow.Top());
        SVisPatchFlow *pf = (SVisPatchFlow *) ff;
        for (int c = 0; c < patch_flow.Width(); c++, pf++) {

            // Check if valid result
            if (pf->n == 0.0f)
                continue;

            // Compute the affected region
            int xn = int(pf->x + pf->u + 0.5f);
            int yn = int(pf->y + pf->v + 0.5f);
            CVisRect reg(xn, yn, xn+1, yn+1);
            reg.InflateRect(win_size, win_size);
            reg &= s;
            if (reg.Height() <= 0 || reg.Width() <= 0)
                continue;
            CVisFloatImage patch = buf.SubImage(reg);

            // Accumulate the flow
            VisAccumulateFlow(patch, pf, win_size, m_warp_grid_spacing, ignore_covariances);
        }
    }

   }   // end of per-frame iteration

    // Solve for flow at each pixel/control vertex, and update
    double uv2 = 0.0, uvc = 0.0;   // amount of warping/deghosting
    for (int r = 0; r < uv.Height(); r++) {
        float *puv = uv.PtrToFirstPixelInRow(r + uv.Top());
        float *ff = buf.PtrToFirstPixelInRow(r + buf.Top());
        SVisPatchFlow *pf = (SVisPatchFlow *) ff;
        for (int c = 0; c < uv.Width(); c++, puv += 2, pf++) {

            // Compute the flow (warp) increment
            pf->C[0][0] += min_C_diag;
            pf->C[1][1] += min_C_diag;
            float det = pf->C[0][0] * pf->C[1][1] - pf->C[0][1] * pf->C[1][0];
            if (det > 0.0001) {
                float detInv  = 1.0 / det; 
                float du = (pf->C[1][1] * pf->u - 
                            pf->C[0][1] * pf->v) * detInv;
                float dv = (pf->C[0][0] * pf->v - 
                            pf->C[1][0] * pf->u) * detInv;
                puv[0] += du;
                puv[1] += dv;
                uv2 += du*du + dv*dv, uvc += 1.0;
            }
        }
    }

    // Echo amount of change
    uv2 = sqrt(uv2 / (uvc + (uvc == 0.0)));
    char msg[1024];
    sprintf(msg, "Deghosting RMS motion = %.2f", uv2);
    ProgressReport(msg);

    // Set the dirty flag, to force image writing on output
	uv.SetIsDirty(true);
}





//
//	Spherical map construction:
//		longitude (panning) theta; latitude (tilting) phi
//		ray direction p = (X, Y, Z) 
//				X = 
//				Y = 
//				Z = 
//		screen coordinate in a perspective image (geometry TVR)
//				(x, y, 1) = TVR p 
//
//
//  Speedup later ...
//

// special warping code from perspective image to complete spherical image
CVisFloatImage VisSphericalMapUV(CVisImageBase& src, CVisImageBase &dst,
								CVisTransform4x4& TVR)
//								CVisImageBase& dstWeight, TVisMap mapMode
{
    // Allocate the warped coordinate image
    CVisShape shape = dst.Shape();
    shape.SetNBands(2);
    CVisFloatImage uv(shape);

	int cols = shape.Width();
	int rows = shape.Height(); 
    int rows1 = rows - 1; 
    float MAXFLOW = 4.0 * cols; 
    float twoPI = float(2 * M_PI); 
    //float dphi = M_PI / rows1; 
    //float dtheta = 2 * M_PI/cols; 

	assert(cols == 2 * rows); // spherical map [-180,180)x[-90,90]

    float *SinTheta = new float[cols]; 
    float *CosTheta = new float[cols]; 
    float *SinPhi   = new float[rows]; 
    float *CosPhi   = new float[rows]; 

    for (int i = shape.Top(); i< shape.right; i++)
    {
		float phi = M_PI * (i - 0.5*rows1) / rows1; 
        SinPhi[i] = sin(phi); 
        CosPhi[i] = cos(phi); 
    }

    for (int j = shape.left; j < shape.right; j++)
    {
		float theta = twoPI * (j - 0.5*cols) / cols; 
        SinTheta[j] = sin(theta); 
        CosTheta[j] = cos(theta); 
    }


    for (i = shape.Top(); i < shape.right; i++) 
    {
        float *uvp = uv.PtrToFirstPixelInRow(i);
        for (j = shape.left; j < shape.right; j++, uvp += 2) 
        {
			CVisVector4 x(SinTheta[j]*CosPhi[i], SinPhi[i], CosTheta[j]*CosPhi[i]);
            CVisVector4 p = TVR * x;

            // when uvp is too large, it causes problem later in warp.cpp 
            // that (int) (uv->v * 256.0f)

            // assert(p[3] != 0); 
			if (p[3] <= 0)
            {
                uvp[0] = uvp[1] = MAXFLOW; 
            }
            else {
                uvp[0] = (float) (p[0]/p[3]);
                uvp[1] = (float) (p[1]/p[3]);

                if (uvp[0] > MAXFLOW)
                    uvp[0] = MAXFLOW; 
                else if (uvp[0] < -MAXFLOW)
                    uvp[0] = -MAXFLOW; 

                if (uvp[1] > MAXFLOW)
                    uvp[1] = MAXFLOW; 
                else if (uvp[1] < -MAXFLOW)
                    uvp[1] = -MAXFLOW; 

            }
        }
    }

    delete SinTheta; 
    delete CosTheta; 
    delete SinPhi; 
    delete CosPhi; 

    return uv;
}

template<class TPixel>
CVisFloatImage& CVisWarpedComposite<TPixel>::GlobalSphericalMap(int index)
{
    // Compute the warping function 
    //if (m_warp_uv.Height() == 0) {
        T_Image &img = Frame(index);  
		assert(Frame(index).TransformChain().size() == 3); // RVT format only

// Was:		CVisTransform4x4 TVR = Frame(index).TransformChain().Inverse().CompleteTransform(); 
		CVisTransform4x4 TVR = Frame(index).TransformChain().CompleteTransform().Inverted();
		
        T_Image warped(m_warped_shape);   
        m_warp_uv =
            VisSphericalMapUV(img, warped, TVR);
    //}
    return m_warp_uv;
}

// Test whether Warped will actually change the image or not
// A return value of true means Warped returns the original image
// Boolean expression is identical to that used in Warped
template<class TPixel>
bool CVisWarpedComposite<TPixel>::WarpingDisabled()
{
    return (DisplayMode() == VIS_COMPOSITE_VIEW_SEQUENCE ||
			!m_local_warp && m_focal_length == 0.0 || 
			m_warp == VIS_WARP_NONE && ! m_local_warp &&
			m_torsion == 0.0 && m_tilt == 0.0 && m_k1 == 0.0 && m_k2 == 0.0);
}

template<class TPixel>
CVisWarpedComposite<TPixel>::T_Image& CVisWarpedComposite<TPixel>::Mapped(int index)
{
    T_Image& img = Frame(index); 

    T_Image imgWarped = T_Image(img.Shape()); 
    img.CopyPixelsTo(imgWarped);
    if (m_local_warp)
    {
        VisInverseWarp(img, imgWarped, LocalWarp(index), 
                        m_interpolation, m_warp_grid_spacing, 1, 0);
    }
        
    // Allocate new frames and new mapped image
// UNDONE:  Should we change this to call Deallocate on each warped image?
//    while (m_warped.NFrames() <= index) {
//        m_warped.Insert("");
//    }
    T_Image& warped = At(index).ImageWarped();

    int sphericalMap = m_sphericalMap; 
    int mapLength = m_mapLength; 
    if (sphericalMap)
    {
        int mapHeight = mapLength / 2; 
		m_warped_shape.left = 0;
		m_warped_shape.top = 0;
		m_warped_shape.right = mapLength;
		m_warped_shape.bottom = mapHeight;
    }
    else
    {
        int mapHeight = mapLength / 4; 
		m_warped_shape.left = 0;
		m_warped_shape.top = 0;
		m_warped_shape.right = mapLength;
		m_warped_shape.bottom = mapHeight;
    }

    warped.Allocate(m_warped_shape);

    // Progress report
    ProgressReport("Mapping frame %d", index);

    // Compute the mapped image
    // method 1: using warp field
    /*
    CVisFloatImage& warp_field = GlobalSphericalMap(index);
    VisInverseWarp(img, warped, warp_field, 
                    m_interpolation, m_warp_grid_spacing, 1, 0);
                    */

    // method 2: direct resampling
	assert(Frame(index).TransformChain().size() == 3); // RVT format only

// Was:	CVisTransform4x4 TVRf = Frame(index).TransformChain().Inverse().CompleteTransform(); 
	CVisTransform4x4 TVRf = Frame(index).TransformChain().CompleteTransform().Inverted(); 

    CVisTransform4x4 R_view = ViewGeometry()[0];
    CVisTransform4x4 TVRv = TVRf * R_view;
    VisSphericalResample(imgWarped, warped, TVRv, m_interpolation, sphericalMap); 

    // compute the weight image 
    
    // computing w0 is redundant for every image
    //CVisByteImage w0 = VisCityBlockDistance(img, m_feather_distance, 1); 
    //CVisByteImage& weightImg = m_feather.Frame(index); 
    CVisFloatImage w0 = VisFloatCityBlockDistance(img, m_feather_distance, 1); 
    CVisFloatImage& weightImg = (*this)[index].ImageFeather(); 
    weightImg.Allocate(m_warped_shape); 
    //CVisByteImage weightImg = CVisByteImage(m_warped_shape); 
    VisSphericalResample(w0, weightImg, TVRv, m_interpolation, sphericalMap); 

    // working ok now: m_feather is public
    //SetFeatherImage(index, weightImg); 
    return warped;
}

// special warping code from perspective image to complete spherical image



template <class TPixel>
void CVisWarpedComposite<TPixel>::BuildPropList(CVisPropList& refproplist)
{
	// LATER:  If (DisplayMode() == VIS_COMPOSITE_VIEW_COMPLETE), we might
	// want to write the final image shape in a comment (as in the old
	// WriteMetaData method).

	// UNDONE:  Code to write the warp fields for each frame.  (And post-read
	// code to force re-warping if CVisLocalWarp::m_warp_on_read is true.)

	// Note; We don't recompute vfov and hfov in a post-read step.

	T_BaseSequence::BuildPropList(refproplist);

	refproplist.AddPropReference("m_warp", (unsigned int&) m_warp, false);
	refproplist.AddPropReference("m_focal_length", m_focal_length, false);
	refproplist.AddPropReference("m_horizontal_fov", m_horizontal_fov, false);
	refproplist.AddPropReference("m_vertical_fov", m_vertical_fov, false);
	refproplist.AddPropReference("m_torsion", m_torsion, false);
	refproplist.AddPropReference("m_tilt", m_tilt, false);
	refproplist.AddPropReference("m_k1", m_k1, false);
	refproplist.AddPropReference("m_k2", m_k2, false);
	refproplist.AddPropReference("m_enlarge_by", m_enlarge_by, false);

	// UNDONE:  Should these be written?
	refproplist.AddPropReference("m_warp_grid_spacing", m_warp_grid_spacing, false);
	refproplist.AddPropReference("m_warp_grid_scale", m_warp_grid_scale, false);

	// UNDONE:  Should these be written?
	refproplist.AddPropReference("m_local_warp_blur", m_local_warp_blur, false);
	refproplist.AddPropReference("m_local_warp", m_local_warp, false);
	refproplist.AddPropReference("m_warped_shape", m_warped_shape, false);
	refproplist.AddPropReference("m_sphericalMap", m_sphericalMap, false);
	refproplist.AddPropReference("m_mapLength", m_mapLength, false);

	// Don't write m_warp_uv and m_frameTmp?
}


//
// Virtual CVisSequence methods
// 

template <class TPixel>
void CVisWarpedComposite<TPixel>::PreReadValue(CVisSDIStream& refsdistream)
{
	T_BaseSequence::PreReadValue(refsdistream);
}

template <class TPixel>
void CVisWarpedComposite<TPixel>::PostReadValue(CVisSDIStream& refsdistream)
{
	T_BaseSequence::PostReadValue(refsdistream);

    CVisLocalWarp::m_write_as_float = (m_warp_grid_scale < 0);

    if (LocalWarpMode() && (NFrames() > 0) && Front().ImageLocalWarp().IsValid())
	{
		if (CVisLocalWarp::m_write_as_float == 0)
		{
			CVisPropList::const_iterator iteratorT
					= PropList().IteratorFromPropName(
							"VisRGBACollectionWarpFieldT");
			if (iteratorT != PropList().end())
			{
				CVisRGBAByteCollection color_coll;
				PropList().GetObj(iteratorT, color_coll);

				SetLocalWarpMode(1);    // force creation of local warp fields

				OffsetToWarp(color_coll);

				const_cast<CVisWarpedComposite<TPixel> *>(this)
						->PropList().DeleteProp(iteratorT);
			}
		}
		else
		{
			CVisPropList::const_iterator iteratorT
					= PropList().IteratorFromPropName(
							"VisFloatCollectionWarpFieldT");
			if (iteratorT != PropList().end())
			{
				CVisFloatCollection float_coll;
				PropList().GetObj(iteratorT, float_coll);

				// Just copy the warp fields directly
				for (int i = 0; i < NFrames(); i++)
				{
					CVisFloatImage &uv = At(i).ImageLocalWarp();
					uv = float_coll[i];
				}

				const_cast<CVisWarpedComposite<TPixel> *>(this)
						->PropList().DeleteProp(iteratorT);
			}
		}
	}

    if (LocalWarpMode() && (NFrames() > 0) && (CVisLocalWarp::m_warp_on_read))
	{
		// Force the re-warping of image by calling Warped()
		for (int i = 0; i < NFrames(); i++)
			Warped(i);
	}

	// LATER:  If (DisplayMode() == VIS_COMPOSITE_VIEW_COMPLETE), we might
	// want to write the final image shape in a comment (as in the old
	// WriteMetaData method).

	// Note; We don't recompute vfov and hfov in a post-read step.

	// LATER:  We might want to append the current working directory to
	// relative names in frames.  This should be done in the CVisSequenceBase
	// class.
}

template <class TPixel>
void CVisWarpedComposite<TPixel>::PreWriteValue(CVisSDOStream& refsdostream) const
{
	T_BaseSequence::PreWriteValue(refsdostream);

	CVisWarpedComposite<TPixel> *pcompositeNonConstThis
			= const_cast<CVisWarpedComposite<TPixel> *>(this);

#if 0   // we will use the SDS writing of m_local_warp to achieve this...
    if (LocalWarpMode() && (NFrames() > 0) && Front().ImageLocalWarp().IsValid())
	{
		// Convert the warp fields into fixpoint RGB images or float collection.
		// LATER:  Is there a better way to do this?
		if (CVisLocalWarp::m_write_as_float == 0)
		{
			CVisRGBAByteCollection color_coll;
			pcompositeNonConstThis->WarpToOffset(color_coll);
			pcompositeNonConstThis->PropList().AddProp(
					"VisRGBACollectionWarpFieldT", color_coll, false);
		}
		else
		{
			CVisFloatCollection    float_coll;

			// Make a temporary flow collection
			//  (this code added to temporarily support CVisMotionEstimator class).
			for (int i = 0; i < NFrames(); i++)
			{
				CVisFloatImage &uv = pcompositeNonConstThis->At(i).ImageLocalWarp();
				if (uv.Name()[0] == 0)
				{
					char *new_name = WarpFilename(i, "W");
					uv.SetName(new_name);
					delete new_name;
				}
				float_coll.PushBack(CVisFloatFrame(uv));
			}

			// special value for CVisFloatCollection
			pcompositeNonConstThis->m_warp_grid_scale = -1;

			pcompositeNonConstThis->PropList().AddProp(
					"VisFloatCollectionWarpFieldT", float_coll, false);
		}
    }
#elif 0
	if ((NFrames() > 0) && Front().ImageLocalWarp().IsValid())
	{
		// This will turn off printing of the "m_imageLocalWarp" property.
		// (Is that what we wanted?  Should we turn it back on in the post-write step?)
		CVisPropTypeInfoBase *pproptypeinfoFrames = VisGetPropTypeInfo(Front());
		assert(pproptypeinfoFrames != 0);
//		bool valid_warps = (LocalWarpMode() && (NFrames() > 0) && 
//							Front().ImageLocalWarp().IsValid());
		pproptypeinfoFrames->PPropList()->SetPrinting("m_imageLocalWarp", LocalWarpMode());
	}
#endif
	// LATER:  If (DisplayMode() == VIS_COMPOSITE_VIEW_COMPLETE), we might
	// want to write the final image shape in a comment (as in the old
	// WriteMetaData method).

	// LATER:  We might want to remove the current working directory to use
	// relative names with frames that are in the current working directory.
	// This should be done in the CVisSequenceBase class.
}

template <class TPixel>
void CVisWarpedComposite<TPixel>::PostWriteValue(CVisSDOStream& refsdostream) const
{
	T_BaseSequence::PostWriteValue(refsdostream);
#if 0   // this causes an infinite loop when erase() gets called (Rick, 15-Feb-99)
    if (LocalWarpMode() && (NFrames() > 0) && Front().ImageLocalWarp().IsValid())
	{
		if (CVisLocalWarp::m_write_as_float == 0)
		{
			const_cast<CVisWarpedComposite<TPixel> *>(this)->PropList().DeleteProp(
					"VisRGBACollectionWarpFieldT");
		}
		else
		{
			const_cast<CVisWarpedComposite<TPixel> *>(this)->PropList().DeleteProp(
					"VisFloatCollectionWarpFieldT");
		}
	}
#endif
}



template <class TPixel>
const type_info& CVisWarpedComposite<TPixel>::ImageTypeInfo(void) const
{
	return typeid(T_Frame);
}

template <class TPixel>
const type_info& CVisWarpedComposite<TPixel>::ObjectTypeInfo(int iLevel) const
{
	if ((iLevel < 0) || (iLevel >= eilevelWarpedComposite))
		return typeid(CVisWarpedComposite<TPixel>);

	return CVisComposite<T_Pixel>::ObjectTypeInfo(iLevel);
}


template <class TPixel>
CVisSequenceBase *CVisWarpedComposite<TPixel>::Clone(bool fCopyThis) const
{
	if (fCopyThis)
		return new CVisWarpedComposite<TPixel>(*this);

	return new CVisWarpedComposite<TPixel>;
}


template <class TPixel>
void CVisWarpedComposite<TPixel>::SDReadWriteProperties(CVisSDStream& s,
		bool fAddComma)
{
	CVisComposite<T_Pixel>::SDReadWriteProperties(s, fAddComma);
}

template <class TPixel>
void CVisWarpedFrame<TPixel>::BuildPropList(CVisPropList& refproplist)
{
	T_BaseImage::BuildPropList(refproplist);
	refproplist.AddPropReference("m_imageLocalWarp", m_imageLocalWarp, false);
	refproplist.AddPropReference("m_ColorAdjust", m_ColorAdjust[0],
	                      		  CVisDim(VIS_COLOR_LAST), false);
}

// Specialized PropList helper functions for common types
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisWarpedFrame<CVisRGBA<BYTE> > const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisWarpedFrame<CVisRGBA<BYTE> > >(0, fAlwaysTypedef); }

CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisWarpedComposite<CVisRGBA<BYTE> > const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisWarpedComposite<CVisRGBA<BYTE> > >(0, fAlwaysTypedef); }

#ifdef LATER
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisWarpedFrame<BYTE> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisWarpedFrame<BYTE> >(0, fAlwaysTypedef); }

CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisWarpedComposite<BYTE> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisWarpedComposite<BYTE> >(0, fAlwaysTypedef); }
#endif // LATER



//
// Special code to make gray composites compile...
//

#if 0
CVisByteImage& CVisWarpedComposite<BYTE>::Warped(int index)
{
    assert(index < 0 && index > 1); // not yet implemented
    return (*this)[index];    // should never get here
}
#endif

//
// Explicit template instantiation
//

#pragma warning(disable : 4660)  // template class already instantiated

template CVisWarpedFrame<CVisRGBABytePixel>;

//template CVisWarpedComposite<CVisRGBAByteFrame>;
template CVisWarpedComposite<CVisRGBABytePixel>;
//template CVisWarpedComposite<CVisRGBAByteWarpedFrame>;
// template CVisWarpedComposite<CVisByteFrame,CVisByteImage>;
// UNDONE:  template CVisWarpedComposite<BYTE>;

#pragma warning(default : 4660)


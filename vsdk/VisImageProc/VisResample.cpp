///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisResample.cpp -- image resampling (zoom, rotate) routines
//  
// DESCRIPTION
//  This routine isused for resampling images, for example to zoom
//  in on an image in a window, to rotate or shear an image.
//
//  For a longer description, see VisResample.h.
//
// SEE ALSO
//  VisResample.h     description of resampling routines
//
//  Fast Color Pixel Blends, Technical Note No. 1 (Vision Technology Group)
//      a description of how to more quickly blend color pixels
//
// BUGS
//  VisResampleShape currently returns a null bounding box if
//  any of the four corners have a negative projected w value.
//  This should be replaced with a proper clipping to the forward
//  half viewing space, and possibly a backface cull operation.
//
// DESIGN
//  For color (CVisRGBABytePixel) pixels, we could either cast the pixels
//  to a multiband BYTE image, or use the faster "packed word"
//  implementation
//
//  The possible cases for a global (3x3 projective) warp are:
//      pure integral shift
//      pure sub-pixel shift
//      pure integral zoom and shift
//      fractional zoom and shift
//      shear preserving scanlines
//      affine transformation
//      full projective (perspective) transformation
//  Some of these cases produce identical results if nearest neighbor
//  interpolation is used.
//
//  To limit the number of special cases, we break the problem down
//  into the following possibilites, based on how a scanline is computed:
//      1. scanline is a pure copy of the source
//      2. scanline is a sub-pixel shift of the source
//      3. scanline is an integral interpolation of the source
//      4. scanline is a non-integral interpolation of the source
//      5. scanline is a slanted (affine) resampling of the source
//      6. scanline is a perspective resampling of the source
//  Some of the 2-D warps also require the blending of successive lines.
//
//  For linear/bilinear interpolation, computations are carried out
//  using incremental fixed-point (scaled doubleing point) operations,
//  using 16-bits of precision for the incremental update, and 16-bit
//  linear interpolation coefficients (8-bit precision for CVisRGBABytePixel).
//  This means that the blended color pixel values may have 1/256 
//  (~ 0.4 %) relative error, that int values should not exceed +/- 2^15,
//  and that the pixel (screen) coordinates must be in +/- 2^15.
//
//
// HISTORY
//  01-Apr-96  Richard Szeliski (szeliski) at Microsoft
//  Created.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////


#include "VisImageProcPch.h"
#include "VisImageProc.h"

float VisResampleMaxUVError = (float) (1.0/128.0);

#ifndef __max
#define __max(a, b)  (((a) > (b)) ? (a) : (b))
#define __min(a, b)  (((a) < (b)) ? (a) : (b))
#endif
#define sgn(a) (((a) > 0.0) ? 1.0 : ((a) < 0.0) ? -1.0 : 0.0)

struct uv_pair { short u,v; };  // max parallax is 255 pixels
#define sscale (1 << 8)         // precision of uv_pair

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif //M_PI

//
//  Compute the values of x \in [x_min, x_max]
//      s.t. M . x is inside valid area of src
//  See notes of Apr 8, 1996.
//

static int invalid_range(double a, double b, int &x_min, int &x_max)
{
    // Ensure that ax + b >= 0
    if (a > 0) {
        if (a * x_max + b < 0)
            return 1;
        if (a * x_min + b < 0)
            x_min = (int) ceil(- b / a);
    } else if (a < 0) {
        if (a * x_min + b < 0)
            return 1;
        if (a * x_max + b < 0)
            x_max = (int) floor(- b / a);
   } else
        return (b < 0);
    return 0;
}

static int valid_pixels(CVisRect &rectSrc, CVisTransform4x4 &m, int y,
                        int &x_min, int &x_max)
{
    double a = m[0][0], b = m[0][1]*y + m[0][3]; // x' = ax+b / (ex + f)
    double c = m[1][0], d = m[1][1]*y + m[1][3]; // y' = cx+d / (ex + f)
    double e = m[3][0], f = m[3][1]*y + m[3][3];
    
    // Special test for negative denominator
    if (f + e * x_min < 0.0)    // denominator shouldn't change sign
        a = -a, b = -b, c = -c, d = -d, e = -e, f = -f;

    // Test Top() <= y', i.e., cx + d >= Top()(ex + f)
    if (invalid_range(c - e*rectSrc.top, d - f*rectSrc.top, x_min, x_max))
        return 0;

    // Test Bottom() - 1 >= y', i.e., cx + d <= (Bottom() - 1)(ex + f)
    if (invalid_range(e*(rectSrc.bottom - 1) - c, f*(rectSrc.bottom - 1) - d, x_min, x_max))
        return 0;
    
    // Test Left() <= x', i.e., ax + b >= Left()(ex + f)
    if (invalid_range(a - e*rectSrc.left, b - f*rectSrc.left, x_min, x_max))
        return 0;

    // Test Right() - 1 >= x', i.e., ax + b <= (Right() - 1)(ex + f)
    if (invalid_range(e*(rectSrc.right - 1) - a, f*(rectSrc.right - 1) - b, x_min, x_max))
        return 0;

    return (x_max >= x_min) ? x_max - x_min + 1 : 0;
}

//
//  Horizontal row, nearest neighbor interpolation
//

template <class TPixel>
static void RowResampleNearest(TPixel *src, TPixel *dst,
                               double a, double b, int cols, int bands)
{
    int ia = (int) floor(a), ib = (int) floor(b);
    src += ib*bands, b -= ib;

    // Pure copy
    if (a == 1.0)
        memcpy(dst, src, cols * bands * sizeof(TPixel));

    // Pixel replication (use Bresenham / DDA)
    else if (fabs(a) < 1.0) {
		const int s = (1 << 16);
        int sa = int(a * s), sb = int(b * s);
        for (int i = 0, j = 0; i < cols * bands; i += bands, sb += sa) {
            for (int k = 0; k < bands; k++)
                dst[i+k] = src[j+k];
            if (sb >= s)
                sb -= s, j += bands;
            else if (sb < 0)
                sb += s, j -= bands;
        }
    }

    // Integral subsampling
    else if (a == ia) {
        if (bands == 1) {
            for (int i = 0, j = 0; i < cols; i += 1, j += ia)
                dst[i] = src[j];
        } else {
            for (int i = 0, j = 0; i < cols * bands; i += bands, j += ia * bands)
                for (int k = 0; k < bands; k++)
                    dst[i+k] = src[j+k];
        }
    }

    // Non-integral subsampling
    else {
        for (int i = 0; i < cols * bands; i += bands, b += a) {
            int j = (int) (b * bands);
            for (int k = 0; k < bands; k++)
                dst[i+k] = src[j+k];
        }
    }
}


//
//  Horizontal row, linear interpolation and inter-row blending
//

template <class TPixel>
static void RowResampleLinear(TPixel *src, TPixel *dst,
                               double a, double b, int cols, int bands)
{
    // Bump up the start pointer
    int ib = (int) floor(b);

    // Scale the pixel coordinates and increments
    int sa = (int) (a * g_scale + 0.5);
    int sb = (int) (b * g_scale + 0.5);

    // Pure copy
    if (a == 1.0 && b == 0)
        memcpy(dst, src, cols * bands * sizeof(TPixel));

    // Doubling
    else if (a == 0.5 && b == 0.0) {
        if (bands == 1) {
            for (int i = 0; i < cols-1; i += 2, dst += 2, src++)
                dst[0] = src[0], dst[1] = VisPixelAverage(src[0], src[1]);
            if (i < cols)
                dst[0] = src[0];
        } else {
            for (int i = 0; i < cols; i += 2) {
                for (int k = 0; k < bands; k++, dst++)
                    dst[0] = src[k];
                if (i < cols-1)
                    for (int k = 0; k < bands; k++, dst++, src++)
                        dst[0] = VisPixelAverage(src[0], src[bands]);
            }
        }
    }

    // General case
    else {
        for (int i = 0; i < cols; i++, dst += bands, sb += sa) {
            int c = sb / g_scale;
            int u = sb & (g_scale-1);
            if (bands == 1)
                dst[0] = PixelSampleLinear(src[c], src[c+1], u);
            else {
                TPixel *srcp = &src[(bands > 1) ? c*bands : c];
                for (int b = 0; b < bands; b++)
                    dst[b] = PixelSampleLinear(srcp[b], srcp[b+bands], u);
            }
        }
    }
}

template <class TPixel>
static void BlendRows(TPixel *src0, TPixel *src1, TPixel *dst,
                      double a, int cols)
{
    if (a == 0.5) {
        for (int i = 0; i < cols; i++)
            dst[i] = VisPixelAverage(src0[i], src1[i]);
    } else {
        int scaled_a = (int) floor(a * g_scale + 0.5);
        if (scaled_a >= g_scale)    // scaled_a can be >= 1...
            memcpy(dst, src1, cols * sizeof(TPixel));
        else
        for (int i = 0; i < cols; i++)
            dst[i] = PixelSampleLinear(src0[i], src1[i], scaled_a);
    }
}


//
//  Resample image according to a global zoom
//      x' = ax + b = m[0][0]x + m[0][3]
//      y' = cy + d = m[1][1]y + m[1][3]
//

template <class TImage>
void VisZoom(TImage &src, TImage &dst,
               double a, double b, double c, double d,
               TVisInterpolation interpolation,
               int zero_outside)
{
    // Local variables
    int bands = dst.NBands(), psize = dst.CbPixel();
    int cols = dst.Width(), bytes_per_row = cols * bands * psize;
    TImage linebuf(dst.Width(), 2, dst.NBands());    // scratch
    int linebuf_id = src.Top()-2;

    // Compute the valid dst subimage
    int x_min = dst.Left(), x_max = dst.Right() - 1;
    int y_min = dst.Top(), y_max = dst.Bottom() - 1;
    int z = (interpolation == VIS_INTERP_NEAREST);
    int empty = invalid_range( a, b - src.Left()  , x_min, x_max) ||
                invalid_range(-a, src.Right() - 1 + z - b, x_min, x_max) ||
                invalid_range( c, d - src.Top()  , y_min, y_max) ||
                invalid_range(-c, src.Bottom() - 1 + z - d, y_min, y_max);
    int vcols = x_max - x_min + 1;

    // Iterate over the scanlines
    for (int y = dst.Top(); y < dst.Bottom(); y++) {
        
        // Test if empty line
        if (empty || y < y_min || y > y_max) {
            if (zero_outside)
                memset(dst.PtrToFirstPixelInRow(y), 0, bytes_per_row);
            continue;
        }

        // Compute current and previous line addresses
        double fy = c*y + d;
        int    iy = (int) floor(fy);        // current src line
        double fo = c*(y-1) + d;
        int    yo = (int) floor(fo);        // previous src line
        double bb = a * x_min + b;          // offset taking x_min into account
        int    ix = (int) floor(bb);
        bb -= ix;

        // Nearest neighbor
        if (interpolation == VIS_INTERP_NEAREST) {
 
            // Check if same line as before (nearest neighbor only)
            if (y > y_min && iy == yo)
                memcpy(dst.PtrToFirstPixelInRow(y), dst.PtrToFirstPixelInRow(y-1), bytes_per_row);

            // Generate a new row
            else {
                RowResampleNearest(&src.Pixel(ix, iy, 0),
                                   &dst.Pixel(x_min, y, 0),
                                   a, bb, vcols, bands);
            }
        }

        // Pure zoom (horizontal scanline) and linear blend
        else {

            // Integral row sample, in linebuf
            if (fy == linebuf_id+1)
                memcpy(&dst.Pixel(x_min, y, 0), linebuf.PtrToFirstPixelInRow(1),
                       vcols * bands * psize);
            else if (iy == fy)      // resample into dst
                RowResampleLinear(&src.Pixel(ix, iy, 0), 
                                  &dst.Pixel(x_min, y, 0),
                                  a, bb, vcols, bands);
            else {
                // Resample prev & next row
                if (linebuf_id != iy) {
                    if (linebuf_id+1 == iy)
                        memcpy(linebuf.PtrToFirstPixelInRow(0), linebuf.PtrToFirstPixelInRow(1),
                               vcols * bands * psize);
                    else if (iy == fo && y > dst.Top())
                        memcpy(linebuf.PtrToFirstPixelInRow(0), &dst.Pixel(x_min, y, 0),
                               vcols * bands * psize);
                    else
                        RowResampleLinear(&src.Pixel(ix, iy, 0),
                                          linebuf.PtrToFirstPixelInRow(0),
                                          a, bb, vcols, bands);
                    int not_last = (iy+1 < src.Bottom());    // guard against last row problem
                    RowResampleLinear(&src.Pixel(ix, iy+not_last, 0),
                                      linebuf.PtrToFirstPixelInRow(1),
                                      a, bb, vcols, bands);
                    linebuf_id = iy;
                }

                // Blend the two rows
                BlendRows(linebuf.PtrToFirstPixelInRow(0), linebuf.PtrToFirstPixelInRow(1),
                          &dst.Pixel(x_min, y, 0),
                          fy - iy, vcols * bands);
            }
        }

        // Pad out ends
        if (zero_outside) {
            if (x_min > dst.Left())
                memset(dst.PtrToFirstPixelInRow(y), 0, 
                       (x_min-dst.Left()) * bands * psize);
            if (x_max < dst.Right() - 1)
                memset(&dst.Pixel(x_max+1, y, 0), 0, 
                       (dst.Right() - 1 - x_max) * bands * psize);
        }
    }
}


//
//  Utility routine:  does not check to see if g_scale == 1.0
//

template <class TImage>
TImage VisZoomed(TImage &src, double zoom,
                      TVisInterpolation interpolation)
{
    // Zoom up (or down) the image
    //  note that zoom down is currently done by subsampling
    CVisShape s1 = src.Shape(), s2 = s1;
	s2.SetLeft(floor(s1.Left() * zoom));
	s2.SetTop(floor(s1.Top() * zoom));
	s2.SetRight(s2.left + ceil(s1.Width() * zoom));
	s2.SetBottom(s2.top + ceil(s1.Height() * zoom));
    TImage zoomed(s2);
    VisZoom(src, zoomed, 1.0 / zoom, 0.0, 1.0 / zoom, 0.0,
            interpolation, 0);
    return zoomed;
}

// no need to convert to fixed point arithmetic for nearest sampling
template <class TPixel>
inline static void PixelSampleNearest(CVisImage<TPixel> &src, TPixel *dst, 
                                      int row, int col, int bands)
{
    TPixel *sPixel = &src.Pixel(col, row, 0); 
    for (int b = 0; b < bands; b++)
    {
        dst[b] = sPixel[b]; 
    }
}

// This function is in VisWarp.cpp.
//  UNDONE: the declaration should go in an internal header file
template <class TPixel>
void VisInverseResampleCubic(
		CVisImage<TPixel> &src, TPixel *dst, TPixel *grad,
		SVisUVCoord *uv, int cols, int zero_outside);

//
//  Diagonal row resampling
//

template <class TPixel>
inline static void PixelSampleBilinear(CVisImage<TPixel> &src, TPixel *dst,
                                       int col, int row, int bands,
                                       int composite_under = 0,
                                       CVisByteImage *depth = 0, uv_pair *uv_map = 0,
                                       TVisInterpolation interpolation = VIS_INTERP_LINEAR,
                                       int check_bounds = 0)
{
    int nearest_pixel = (interpolation == VIS_INTERP_NEAREST);
    int r = row / g_scale, c = col / g_scale;
    int u = col & (g_scale-1);
    int v = row & (g_scale-1);
	int out_of_bounds = 0;
    if (depth) {    // add out of plane depth parallax (see notes 29-Dec-97)
        int d;
        if (uv_map == 0) {  // read out correction from "depth" map
            int *id = (int *) &depth->Pixel(c, r, 0);
            if (*id == -1)   // invalid correction/flow
                d = 0;
            else
                d = 1, uv_map = ((uv_pair *) id)-1;  // compute correction "map"
        } else
            d = depth->Pixel(c, r, 0);
        if  (d == 0)
            out_of_bounds = 1;  // no valid d, invisible pixel
        else {
            col += uv_map[d].u * (g_scale / sscale);
            row += uv_map[d].v * (g_scale / sscale);
            r = row / g_scale, c = col / g_scale;
            u = col & (g_scale-1);
            v = row & (g_scale-1);
        }
    }

	// Check that we are in bounds
    if ((check_bounds || depth) && !out_of_bounds)
		out_of_bounds = ! src.ContainsPoint(c, r) 
			    || u > 0 && ! src.ContainsPoint(c+1, r)
			    || v > 0 && ! src.ContainsPoint(c, r+1);

	int zero = 0;
    TPixel *src0 = out_of_bounds ? (TPixel *) &zero : &src.Pixel(c, r+0, 0);
    int& rslt = *(int *) dst, frgd = 0;
    if (composite_under) {
        if ((rslt & 0xFF000000) == 0xFF000000)
            return;
        frgd = rslt;     // save old value
    }
    if (out_of_bounds || nearest_pixel || u == 0 && v == 0) {
        for (int b = 0; b < bands; b++)
            dst[b] = src0[b];
    } else if (v == 0 && interpolation != VIS_INTERP_CUBIC) {
        for (int b = 0; b < bands; b++)
            dst[b] = PixelSampleLinear(src0[b], src0[b+bands], u);
    } else {
        TPixel *src1 = &src.Pixel(c, r+1, 0);
        if (composite_under) {  // should only be called with colorbyte images...
            int *i0 = (int *) src0, *i1 = (int *) src1;
            if ((i0[0] | i0[1] | i1[0] | i1[1]) == 0)
                return;     // src is empty
        }
        if (interpolation == VIS_INTERP_CUBIC) {
    		SVisUVCoord uv;
            uv.u = c + (1.0 / g_scale) * u;
            uv.v = r + (1.0 / g_scale) * v;
            VisInverseResampleCubic(src, dst, (TPixel *) 0, &uv, 1, 1);
        } else
        for (int b = 0; b < bands; b++) {
            TPixel p0 = PixelSampleLinear(src0[b], src0[b+bands], u);
            TPixel p1 = PixelSampleLinear(src1[b], src1[b+bands], u);
            dst[b]       = PixelSampleLinear(p0,      p1,          v);
        }
    }
    if (composite_under && frgd != 0) {
        rslt = RGBA_over(frgd, rslt);
    }
}

template <class TPixel>
static void LineResample(CVisImage<TPixel> &src, TPixel *dst,
                         CVisTransform4x4 m, int iy, 
                         int x_min, int x_max, int bands,
                         TVisInterpolation interpolation,
                         int composite_under = 0,
                         CVisByteImage *depth = 0, uv_pair *uv_map = 0)
{
    int cols = x_max - x_min + 1;
    double x0 = m[0][0] * x_min + m[0][1] * iy + m[0][3];
    double x1 = x0 + m[0][0] * (x_max - x_min);
    double y0 = m[1][0] * x_min + m[1][1] * iy + m[1][3];
    double y1 = y0 + m[1][0] * (x_max - x_min);
    double z0 = m[3][0] * x_min + m[3][1] * iy + m[3][3];
    double z1 = z0 + m[3][0] * (x_max - x_min);
    double u0 = (z0 == 1.0) ? x0 : x0 / z0;
    double u1 = (z1 == 1.0) ? x1 : x1 / z1;
    double v0 = (z0 == 1.0) ? y0 : y0 / z0;
    double v1 = (z1 == 1.0) ? y1 : y1 / z1;

    // Subdivide if necessary
    // ... (depends on value of interpolate): see notes of May 3, 1996
    if (z0 != z1 && x_max - x_min > 1) {
        double factor = 0.25 * fabs(z0 - z1) / __min(fabs(z0), fabs(z1));
        double max_u_err = factor * fabs(u0 - u1);
        double max_v_err = factor * fabs(v0 - v1);
        if (__max(max_u_err, max_v_err) > VisResampleMaxUVError) {
            int x_mid = (x_min + x_max + ((x_min+x_max)&1)) / 2;
            LineResample(src, dst, m, iy, x_min, x_mid-1, bands, interpolation, 
                composite_under, depth, uv_map);
            dst += (x_mid - x_min) * bands;
            LineResample(src, dst, m, iy, x_mid, x_max,   bands, interpolation,
                composite_under, depth, uv_map);
            return;
        }
    }

    // Scale the pixel coordinates and increments
    int col = (int) (u0 * g_scale + 0.5);
    int row = (int) (v0 * g_scale + 0.5), ci, ri;
    if (interpolation == VIS_INTERP_NEAREST)
        row += g_scale/2, col += g_scale/2; // round up coordinates
    if (z0 == 1.0 && z1 == 1.0) {
        ci = (int) (m[0][0] * g_scale);
        ri = (int) (m[1][0] * g_scale);
    } else {
        double icols = (cols > 1) ? 1.0 / (cols - 1) : 1.0;
        ci  = (int) ((u1 - u0) * icols * g_scale);
        ri  = (int) ((v1 - v0) * icols * g_scale);
    }

    // Sample the pixels
    if (interpolation == VIS_INTERP_NEAREST && depth == 0) {
        // Turn into an inline function, like PixelSampleBilinear?
        if (composite_under && bands == 1) {    // use only with CVisRGBAByteImage
	        for (int i = 0; i < cols; i++, dst += bands, row += ri, col += ci) {
                CVisRGBABytePixel *p = (CVisRGBABytePixel *) dst;
                if (p->A() != 255) {
                    int r = row / g_scale, c = col / g_scale;
                    TPixel &pxl = src.Pixel(c, r);
                    if (p->A() == 0)
                        dst[0] = pxl;
                    else {
                        int *f = (int *) dst, *b = (int *) &pxl;
                        *f = RGBA_over(*f, *b);
                    }
                }
            }
        } else if (bands == 1) {
	        for (int i = 0; i < cols; i++, dst += bands, row += ri, col += ci) {
                int r = row / g_scale, c = col / g_scale;
                dst[0] = src.Pixel(c, r);
            }
          }
		else {
	        for (int i = 0; i < cols; i++, dst += bands, row += ri, col += ci) {
                int r = row / g_scale, c = col / g_scale;
                TPixel *srcp = &src.Pixel(c, r, 0);
                for (int b = 0; b < bands; b++)
			        dst[b] = srcp[b];
            }
	    }
    } else {
        for (int i = 0; i < cols; i++, dst += bands, row += ri, col += ci)
            PixelSampleBilinear(src, dst, col, row, bands, composite_under,
                                depth, uv_map, interpolation);
    }
}

//
// This code is primarily for "true" parallax correction, although
//  it can also be used to compute an "exact" homography/
//

template <class TPixel>
static void LineResampleZ(CVisImage<TPixel> &src, TPixel *dst,
                         CVisTransform4x4 m, int iy, 
                         int x_min, int x_max, int bands,
                         TVisInterpolation interpolation,
                         int composite_under = 0,
                         CVisByteImage *depth = 0,
                         double u_p = 0.0, double v_p = 0.0, double w_p = 0.0)
{
    int cols = x_max - x_min + 1;
    double x0 = m[0][0] * x_min + m[0][1] * iy + m[0][3];
    double xd = m[0][0];
    double y0 = m[1][0] * x_min + m[1][1] * iy + m[1][3];
    double yd = m[1][0];
    double z0 = m[3][0] * x_min + m[3][1] * iy + m[3][3];
    double zd = m[3][0];

    // Sample the pixels
    if (interpolation == VIS_INTERP_NEAREST && depth == 0) {
        // Turn into an inline function, like PixelSampleBilinear?
        if (composite_under && bands == 1) {    // use only with CVisRGBAByteImage
	        for (int i = 0; i < cols; i++, dst += bands, x0 += xd, y0 += yd, z0 += zd) {
                CVisRGBABytePixel *p = (CVisRGBABytePixel *) dst;
                if (p->A() != 255) {
                    double zi = (z0 == 1.0) ? 1.0 : 1.0 / z0;
                    int c = int(x0 * zi + 0.5);
                    int r = int(y0 * zi + 0.5);
                    TPixel &pxl = src.Pixel(c, r);
                    if (p->A() == 0)
                        dst[0] = pxl;
                    else {
                        int *f = (int *) dst, *b = (int *) &pxl;
                        *f = RGBA_over(*f, *b);
                    }
                }
            }
        } else if (bands == 1) {
	        for (int i = 0; i < cols; i++, dst += bands, x0 += xd, y0 += yd, z0 += zd) {
                double zi = (z0 == 1.0) ? 1.0 : 1.0 / z0;
                int c = int(x0 * zi + 0.5);
                int r = int(y0 * zi + 0.5);
                dst[0] = src.Pixel(c, r);
            }
          }
		else {
	        for (int i = 0; i < cols; i++, dst += bands, x0 += xd, y0 += yd, z0 += zd) {
                double zi = (z0 == 1.0) ? 1.0 : 1.0 / z0;
                int c = int(x0 * zi + 0.5);
                int r = int(y0 * zi + 0.5);
                TPixel *srcp = &src.Pixel(c, r, 0);
                for (int b = 0; b < bands; b++)
			        dst[b] = srcp[b];
            }
	    }
    } else {
	    for (int i = 0; i < cols; i++, dst += bands, x0 += xd, y0 += yd, z0 += zd) {
            double zi = (z0 == 1.0) ? 1.0 : 1.0 / z0;
            int col = int(x0 * zi * g_scale + 0.5);
            int row = int(y0 * zi * g_scale + 0.5);
            if (depth) {
                int c = int(x0 * zi + 0.5);
                int r = int(y0 * zi + 0.5);
                int d = depth->Pixel(c, r);
                if (d == 0) {   // invalide depth value => out-of-bounds pixel
                    if (! composite_under)
                        *(int *) dst = 0;   // valid only for ColorByte images...
                    continue;
                }
                double x = x0 + u_p * (d-128);
                double y = y0 + v_p * (d-128);
                double z = z0 + w_p * (d-128);
                double zi2 = g_scale / z;
                col = int(x * zi2 + 0.5);
                row = int(y * zi2 + 0.5);
            }
            PixelSampleBilinear(src, dst, col, row, bands, composite_under,
                                0, 0, interpolation, 1);
        }
    }
}

//
//  Given an image and its associated TVR transform
//  Resample it to its spherical or cylindrical coordinates
//

template <class TImage>
void VisSphericalResample(TImage &src, TImage &dst, 
                          CVisTransform4x4 TVR, 
                          TVisInterpolation interpolation, 
                          int spherical)
{
    // Allocate the warped coordinate image
    CVisShape shape = dst.Shape();
	int cols = shape.Width(); 
	int rows = shape.Height(); 
    int rows1 = rows - 1; 
    float twoPI = float(2 * M_PI);
    float PI_2 = float(M_PI / 2.0); 
    int bands = dst.NBands(); 

    dst.ClearPixels(); // set to zeros

    if (spherical)
    {
	    assert(cols == 2 * rows); // spherical map [-180,180)x[-90,90]
    }
    else
    {
        assert(cols == 4 * rows); // cylindrical map [-180,180)x[-45,45]
    }

    float *SinTheta = new float[cols]; 
    float *CosTheta = new float[cols]; 
    float *SinPhi   = new float[rows]; 
    float *CosPhi   = new float[rows]; 

    //for (int i = shape.Top(); i<= shape.Bottom(); i++)
    for (int i = 0; i < shape.Height(); i++)
    {
		float phi = M_PI * (i - 0.5*rows1) / rows1; 
        SinPhi[i] = sin(phi); 
        CosPhi[i] = cos(phi); 
    }

    //for (int j = shape.Left(); j< shape.Right(); j++)
    for (int j = 0; j< shape.Width(); j++)
    {
		float theta = twoPI * (j - 0.5*cols) / cols; 
        SinTheta[j] = sin(theta); 
        CosTheta[j] = cos(theta); 
    }

    for (i = 0; i < shape.Height(); i++) 
    {
        float y; 
        float n = (float) rows1; 
        if (!spherical)
        {
            // or should this be tan(y)? 
            float yAngle = PI_2 * (i - 0.5 * n) / n; 
            y = tan(yAngle); 
        }
        for (j = 0; j < shape.Width(); j++) 
        {
		    CVisVector4 xVec(SinTheta[j]*CosPhi[i], SinPhi[i], CosTheta[j]*CosPhi[i]);
            if (!spherical) 
            {
                xVec = CVisVector4(SinTheta[j], y, CosTheta[j]); 
            }
            CVisVector4 p = TVR * xVec;

			if (p[3] > 0)
            {
                float fcol = (float) (p[0]/p[3]); // x axis
                float frow = (float) (p[1]/p[3]); // y axis

                if (fcol >= src.Left() && fcol < src.Right() 
                    && frow >= src.Top() && frow < src.Bottom())
                {
                    // Sample the pixels
                    if (interpolation == VIS_INTERP_NEAREST) 
                    {
                        int r = (int) frow; 
                        int c = (int) fcol; 
                        PixelSampleNearest(src, &dst.Pixel(j, i, 0), c, r, bands); 
                    }
                    else {
                        // fixed point computation
                        int rS = (int) (frow * g_scale); 
                        int cS = (int) (fcol * g_scale); 
                        PixelSampleBilinear(src, &dst.Pixel(j, i, 0),
                            cS, rS, bands, 0, 0, 0, interpolation, 1);
                    }
                }
            }
        }
    }

    delete SinTheta; 
    delete CosTheta; 
    delete SinPhi; 
    delete CosPhi; 

}

//
//  Resample image according to a global warp
//

template <class TImage>
void VisResample(TImage &src, TImage &dst,
                 CVisTransform4x4 geometry,
                 TVisInterpolation interpolation,
                 int zero_outside, int composite_under,
                 int true_perspective,
                 CVisByteImage *depth, int parallax_flow,
                 double u_p, double v_p, double w_p)
{
    // Local variables
    CVisTransform4x4 m = geometry.Normalize();
    int bands = dst.NBands(), psize = dst.CbPixel();
    int cols = dst.Width(), bytes_per_row = cols * bands * psize;

    // Pure zoom
    if (m[0][1] == 0 && m[1][0] == 0 &&
        m[3][0] == 0 && m[3][1] == 0 && m[3][3] == 1 &&
        ! composite_under && depth == 0 &&
        (interpolation < VIS_INTERP_CUBIC || m == CVisTransform4x4())) {
        VisZoom(src, dst, m[0][0], m[0][3], m[1][1], m[1][3],
                  interpolation, zero_outside );
        return;
    }

    // Precompute the uv_map (not needed for parallax_flow)
    uv_pair uv_map[256], *uvp = uv_map;
    if (depth && ! parallax_flow) {
        for (int d = 0; d < 256; d++) {
            uv_map[d].u = int((d-128) * sscale * u_p + 0.5);
            uv_map[d].v = int((d-128) * sscale * v_p + 0.5);
        }
    } else
        uvp = 0;

    // Iterate over the scanlines
    for (int y = dst.Top(); y < dst.Bottom(); y++) {

        // Compute bounds and test if empty line
        int x_min = dst.Left(), x_max = dst.Right() - 1;
        CVisRect src_shape = (depth) ? depth->Shape() : src.Shape();
        int vcols = valid_pixels(src_shape, m, y, x_min, x_max);
        if (vcols == 0) {
            if (zero_outside)
                memset(dst.PtrToFirstPixelInRow(y), 0, bytes_per_row);
            continue;
        }

        // Resample the line
        if (true_perspective && ! parallax_flow)
            LineResampleZ(src, &dst.Pixel(x_min, y, 0), m, y,
                         x_min, x_max, bands, interpolation,
                         composite_under, depth, u_p, v_p, w_p);
        else
            LineResample(src, &dst.Pixel(x_min, y, 0), m, y,
                         x_min, x_max, bands, interpolation,
                         composite_under, depth, uvp);

        // Pad out ends
        if (zero_outside) {
            if (x_min > dst.Left())
                memset(dst.PtrToFirstPixelInRow(y), 0, 
                       (x_min-dst.Left()) * bands * psize);
            if (x_max < dst.Right() - 1)
                memset(&dst.Pixel(x_max + 1, y, 0), 0, 
                       (dst.Right() - 1 - x_max) * bands * psize);
        }
    }
}

//
//  Forward warping of depth map for parallax
//    to make this raster order independent, need a separate scratch
//    image or at least an extra output buffer line(s)
//

template <class TPixel>
void VisFillGaps(CVisImage<TPixel>& img)
{
    int nrows = img.Height(), ncols = img.Width();
    int b = img.NBands(), nbytes = ncols * b * sizeof(TPixel);
    int invalid = (b > 1) ? -1 : 0;     // special to (u,v) parallax
    CVisImage<TPixel> scratch(ncols+2, 2, b);      // scratch buffer
    scratch.FillPixels(0);
    memcpy(&scratch.Pixel(1,1,0), img.PtrToFirstPixelInRow(img.Top()), nbytes);
    for (int y = img.Top(); y < img.Bottom(); y++) {
        TPixel *p = img.PtrToFirstPixelInRow(y);
        TPixel *q0 = &scratch.Pixel(1,(y&1)^1,0);
        TPixel *q1 = &scratch.Pixel(1,(y&1)^0,0);
        TPixel *q2 = img.PtrToFirstPixelInRow(y + (y < img.Bottom() - 1));
        memcpy(q1, p, nbytes);       // save before overwriting
        for (int x = 0; x < ncols*b; x++) {
            if (p[x] == invalid) {
                int sum = 0, cnt = 0;
                //
                //  TODO:  find first non-zero neighbor instead???
                //
                sum += q0[x  ], cnt += (q0[x  ] > 0);
                sum += q1[x-b], cnt += (q1[x-b] > 0);
                sum += q1[x+b], cnt += (q1[x+b] > 0);
                sum += q2[x  ], cnt += (q2[x  ] > 0);
                if (cnt > 0)
                    p[x] = (sum  + cnt/2) / cnt;   // could replace with LUT...
            }
        }
    }
}

CVisByteImage VisForwardWarpDepthMap(CVisByteImage& depth, CVisByteImage& warped,
                                     double u_p, double v_p, double w_p,
                                     int d_min, int d_max, 
                                     int true_perspective, int parallax_flow,
                                     int gap_size, int splat_size,
                                     int enlarge_by)
{
    // Compute the (nearest pixel) displacement table
    //  not strictly needed for true parallax, but used to compute warped shape...
    uv_pair uv_map[256];
    for (int d = d_min; d <= d_max; d++) {
        uv_map[d].u = int((d-128) * sscale * u_p + 0.5);
        uv_map[d].v = int((d-128) * sscale * v_p + 0.5);
    }
    
    // Compute inverse z table
    double zi_table[256];
    for (d = d_min; true_perspective && d <= d_max; d++)
        zi_table[d] = 1.0 / (1.0 + (d-128) * w_p);

    // Compute a conservative warped map size estimate
    int dx0 = uv_map[(u_p < 0) ? d_max : d_min].u / sscale;
    int dx1 = uv_map[(u_p > 0) ? d_max : d_min].u / sscale;
    int dy0 = uv_map[(v_p < 0) ? d_max : d_min].v / sscale;
    int dy1 = uv_map[(v_p > 0) ? d_max : d_min].v / sscale;
    CVisRect d_shape = depth.Shape();
    d_shape.InflateRect(- dx0, - dy0, dx1, dy1);
    d_shape.InflateRect(1, 1);  // so that InflateRect(1, 1) below still works...

    // Optionally enlarge the warped image
    CVisRect w_shape = warped.Shape();
    int desired_bands = parallax_flow ? sizeof(uv_pair) : 1;
    if ((w_shape.left > d_shape.left)
			|| (w_shape.top > d_shape.top) || (w_shape.right < d_shape.right)
			|| (w_shape.bottom < d_shape.bottom) || warped.NBands() != desired_bands) {
        d_shape.InflateRect(enlarge_by, enlarge_by);
        warped.Allocate(CVisShape(d_shape, desired_bands));
    }

    // Compute the direction of traversal and final (computed) bbox
    int x_dir = (u_p < 0) ? +1 : -1;
    int y_dir = (v_p < 0) ? +1 : -1;
    int x_s   = (u_p < 0) ? depth.Left() : depth.Right() - 1;
    int y_s   = (v_p < 0) ? depth.Top() : depth.Bottom() - 1;
    int x_e   = (u_p < 0) ? depth.Right() : depth.Left() - 1;
    int y_e   = (v_p < 0) ? depth.Bottom() : depth.Top() - 1;
    int xw_min = warped.Right() - 1, xw_max = warped.Left();
    int yw_min = warped.Bottom() - 1, yw_max = warped.Top();

    // Perform the forward mapping
    if (parallax_flow)
        warped.FillPixels(-1);
    else
        warped.ClearPixels();
    for (int y = y_s; y != y_e; y += y_dir) {
        BYTE *p = &depth.Pixel(x_s, y) - x_s;
        for (int x = x_s; x != x_e; x += x_dir) {
            int d  = p[x];
            if (d == 0)
                continue;   // invisible sprite pixel
            int u = uv_map[d].u;        // parallax flow
            int v = uv_map[d].v;
            if (true_perspective) {
#if 0   // for debugging only...
                static int x0 = 150, y0 = 150;
                if (x == x0 && y == y0)
                    u = x0;
#endif
                //  Compute new position using perspective division
                double x2 = (x + (d-128) * u_p) * zi_table[d];
                double y2 = (y + (d-128) * v_p) * zi_table[d];
                u = int((x2 - x) * sscale + 0.5);   // better parallax flow
                v = int((y2 - y) * sscale + 0.5);
            }
            int xw = x + u / sscale;
            int yw = y + v / sscale;
            // This check slows down code but is needed...
            if (! warped.ContainsPoint(xw, yw, 0))
                continue;
            if (parallax_flow) {
                uv_pair& uvp = *(uv_pair *) &warped.Pixel(xw, yw, 0);
                uvp.u = -u;
                uvp.v = -v;
            } else
                warped.Pixel(xw, yw) = d;
            if (splat_size > 1) {   // could be optimized...
                for (int yy = yw - splat_size/2, j = 0; j < splat_size; j++, yy++) {
                    for (int xx = xw - splat_size/2, k = 0; k < splat_size; k++, xx++) {
                        if (warped.ContainsPoint(xx, yy, 0))
                            warped.Pixel(xx, yy) = d;
                    }
                }
            }
            if (xw_min > xw) xw_min = xw;
            if (xw_max < xw) xw_max = xw;
            if (yw_min > yw) yw_min = yw;
            if (yw_max < yw) yw_max = yw;
        }
    }

    // Final bounding box
    CVisRect bbox(xw_min, yw_min, xw_max+1, yw_max+1);
    // bbox.Dilate(1);     // so that VisFillGaps can stay away from edge
    CVisByteImage trimmed = warped.SubImage(bbox);

    // Fill in the gaps
    for (int i = 0; i < gap_size; i++) {
        if (parallax_flow) {
            CVisShortImage simg;
			simg.Alias(trimmed);
            VisFillGaps(simg);
        } else
            VisFillGaps(trimmed);
    }

    return trimmed;
}
                            
                            
//
//  Shape of projected image
//

// Use the code in VisTransform4x4

void VisResampleCorners(CVisShape sh, const CVisTransform4x4 &m,
                        CVisVector4 uvw[4])
{
    // Project the four corners of the image and normalize,
    //  but leave sign in last coordinate
    double xc[2] = {sh.Left(), sh.Right() - 1};
    double yc[2] = {sh.Top(), sh.Bottom() - 1};
    for (int i = 0; i < 4; i++) {
        CVisVector4 v(xc[(i == 1) || (i == 2)], yc[i >> 1]);
        uvw[i] = m.ProjectVector(v);
    }
}

void VisResampleOutline(CVisShape sh, const CVisTransform4x4 &m,
                        int line[4][2][2], double zoom)
{
    // Project the four corners of the image
    //  and then clip each line to line in w > 0 half-space
    double xc[2] = {sh.Left(), sh.Right() - 1};
    double yc[2] = {sh.Top(), sh.Bottom() - 1};
    for (int i = 0; i < 4; i++) {
        int i0 = i, i1 = (i + 1)&3;
        CVisVector4 v0(xc[(i0 == 1) || (i0 == 2)], yc[i0 >> 1]);
        CVisVector4 v1(xc[(i1 == 1) || (i1 == 2)], yc[i1 >> 1]), e[2];
        m.ProjectLine(v0, v1, e[0], e[1], double(1 << 24)/zoom);

        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                line[i][j][k] = int(floor(zoom * e[j][k] + 0.5));
            }
        }
    }
}

//
//  Compute shape of resampled image, but trim it to avoid
//  excessive pixel stretch
//

static int VisEvaluateStretch(int point[2],
                              const CVisTransform4x4 &m, float max_stretch)
{
    CVisVector4 uvw[4];
    uvw[0] = m.ProjectVector(CVisVector4(point[0]+0.0, point[1]+0.5));
    uvw[1] = m.ProjectVector(CVisVector4(point[0]+0.0, point[1]-0.5));
    uvw[2] = m.ProjectVector(CVisVector4(point[0]+0.5, point[1]+0.5));
    uvw[3] = m.ProjectVector(CVisVector4(point[0]-0.5, point[1]+0.5));
    if (uvw[0][3] <= 0.0 || uvw[1][3] <= 0.0 ||
        uvw[2][3] <= 0.0 || uvw[3][3] <= 0.0)
        return 0;
    double dx0 = uvw[0][0] - uvw[1][0], dy0 = uvw[0][1] - uvw[1][1];
    double dx1 = uvw[2][0] - uvw[3][0], dy1 = uvw[2][1] - uvw[3][1];
    return (dx0*dx0 + dy0*dy0 <= max_stretch*max_stretch &&
            dx1*dx1 + dy1*dy1 <= max_stretch*max_stretch);
}

static int VisTrimByStretch(int point[2][2],
                            const CVisTransform4x4 &m, float max_stretch)
{
    int valid0 = VisEvaluateStretch(point[0], m, max_stretch);
    int valid1 = VisEvaluateStretch(point[1], m, max_stretch);
    if (valid0 == valid1)
        return valid0;
    int v[2] = {point[valid1][0], point[valid1][1]};    // valid
    int i[2] = {point[valid0][0], point[valid0][1]};    // invalid
    int mid[2];
    point[0][0] = v[0], point[0][1] = v[1];   // valid endpoint
    while (abs(v[0]-i[0]) > 1 || abs(v[1]-i[1]) > 1) {
        mid[0] = (v[0]+i[0])/2, mid[1] = (v[1]+i[1])/2;
        int valid = VisEvaluateStretch(mid, m, max_stretch);
        if (valid)
            v[0] = mid[0], v[1] = mid[1];
        else
            i[0] = mid[0], i[1] = mid[1];
    }
    point[1][0] = v[0], point[1][1] = v[1];   // trimmed valid endpoint
    return 1;
}

CVisShape VisResampleShape(CVisShape sh, const CVisTransform4x4 &m,
                              float max_stretch)
{
    CVisShape shape(0, 0, 0, 0);
    int xc[2] = {sh.Left(), sh.Right() - 1};
    int yc[2] = {sh.Top(), sh.Bottom() - 1};

    // Iterate over the four sides of the shape
    for (int i = 0; i < 4; i++) {

        // Form line endpoints
        int p[2][2];
        for (int j = 0; j < 2; j++) {
            int k = (i+j)&3;
            p[j][0] = xc[(k == 1) || (k == 2)];
            p[j][1] = yc[k >> 1];
        }

        // Trim to valid line segment and union with final shape
        if (VisTrimByStretch(p, m, max_stretch)) {
            CVisVector4 uvw[2];
            uvw[0] = m.ProjectVector(CVisVector4(p[0][0], p[0][1]));
            uvw[1] = m.ProjectVector(CVisVector4(p[1][0], p[1][1]));
            CVisShape sh0;
			sh0.left = int(uvw[0][0]+0.5);
			sh0.top = int(uvw[0][1]+0.5);
			sh0.right = sh0.left + 1;
			sh0.bottom = sh0.top + 1;
            CVisShape sh1;
			sh1.left = int(uvw[1][0]+0.5);
			sh1.top = int(uvw[1][1]+0.5);
			sh1.right = sh1.left + 1;
			sh1.bottom = sh1.top + 1;
            CVisShape sh2 = (sh0 | sh1);
            if (shape.Height() <= 0 || shape.Width() <= 0)
                shape = sh2;
            else
                shape |= sh2;
        }
    }
    return shape;
}

template <class TImage>
TImage VisResample(TImage &src, CVisShape shape,
                   CVisTransform4x4 geometry,
                   TVisInterpolation interpolation)
{
    TImage result(shape);
    VisResample(src, result, geometry, interpolation);
    // Question: does new initialize to 0?  If so, don't need to zero_outside
    return result;
}

//
//  Test loop, tries to exercise most paths of VisResample
//

struct SVisTestTransform {
    int n;                      // number of iterations
    double tx, ty;              // translation of image center
    double angle, ax, ay, az;   // rotation around image center
    double sx, sy;              // g_scale around image center
};

static TVisInterpolation test_interpolant = VIS_INTERP_LINEAR;

void VisResampleTestCase(CVisRGBAByteImage &cimg, CVisRGBAByteImage &rimg,
                           SVisTestTransform &t, int i)
{
    double tx0 = 0.5*(cimg.Left() + cimg.Right() - 1);
    double ty0 = 0.5*(cimg.Top() + cimg.Bottom() - 1);
    double tx1 = 0.5*(rimg.Left() + rimg.Right() - 1);
    double ty1 = 0.5*(rimg.Top() + rimg.Bottom() - 1);
    CVisTransformChain g;
    g.Translate(-tx0, -ty0);
    g.Rotate(t.angle * i, t.ax, t.ay, t.az);
    g.Scale(1.0 + t.sx * i, 1.0 + t.sy * i);
    CVisTransform4x4 f;
    f[3][2] = 0.001;
    g.front() *= f;
    g.Translate(tx1 + t.tx * i, ty1 + t.ty * i);
    VisResample(cimg, rimg, g.CompleteTransform(), test_interpolant, 1, 0, 0,
        (CVisByteImage *) 0, 0, 0.0, 0.0, 0.0);
}

static SVisTestTransform xfrms[] = {
    // n    tx      ty      angle   ax,y,z  sx      sy
#if 1
    { 8,    1,      0,      0,      0,0,0,  0,      0},
    { 8,    0,      1,      0,      0,0,0,  0,      0},
    { 8,    0.625,  0,      0,      0,0,0,  0,      0},
    { 8,    0,      0.625,  0,      0,0,0,  0,      0},
    {10,    0,      0,      0,      0,0,1,  0.125,  0.125},
    { 6,    0,      0,      0,      0,0,1, -0.125, -0.125},
    {15,    0,      0,     30,      0,0,1,  0,      0},
    {15,    0,      0,    -30,      0,0,1,  0,      0},
#endif
    {12,    0,      0,     10,      0,1,0,  0,      0},
    {12,    0,      0,     10,      0,2,1,  0,      0},
    { 0,    0,      0,      0,      0,0,0,  0,      0}
};

void VisResampleTestLoop(CVisRGBAByteImage &cimg, CVisRGBAByteImage &rimg,
                           void (*callback)(CVisRGBAByteImage &img))
{
    for (SVisTestTransform *t = xfrms; t->n; t++) { // interate over xfrms
        for (int i = 0; i < t->n; i++) {
            VisResampleTestCase(cimg, rimg, *t, i);
            callback(rimg);
// UNDONE:            CVisRefCountArray::PrintStatistics(0);
        }
        for (i = t->n; i > 0; i--) {
            VisResampleTestCase(cimg, rimg, *t, i);
            callback(rimg);
// UNDONE:            CVisRefCountArray::PrintStatistics(0);
        }
    }
}


//
// Template instantiation
//


template <class TImage>
void VisResampleInstantiate(TImage &img)
{
	TImage tmp1;
	VisZoom(img,tmp1,1.0,0.0,1.0,0.0,VIS_INTERP_LINEAR,0);
	VisResample(img, tmp1, CVisTransform4x4(), VIS_INTERP_LINEAR, 0, 0, 0,
                (CVisByteImage *) 0, 0, 0.0, 0.0, 0.0);
    VisSphericalResample(img, tmp1, CVisTransform4x4(), VIS_INTERP_LINEAR, 1);
    VisZoomed(img, 1.0, VIS_INTERP_NEAREST);
}
	
void VisResampleInstantiate()
{
    VisResampleInstantiate(CVisByteImage());
    VisResampleInstantiate(CVisRGBAByteImage());
	VisResampleInstantiate(CVisIntImage());
    VisResampleInstantiate(CVisFloatImage());
    VisResampleInstantiate(CVisRGBAFloatImage());
    // add more here, as needed...

}

// For some reason, Release compiler VC 6.0 doesn't instantiate correctly above...

template 
void VisResample<CVisRGBAFloatImage> (CVisRGBAFloatImage &src, CVisRGBAFloatImage &dst,
                 CVisTransform4x4 geometry,
                 TVisInterpolation interpolation,
                 int zero_outside, int composite_under,
                 int true_perspective,
                 CVisByteImage *depth, int parallax_flow,
                 double u_p, double v_p, double w_p);

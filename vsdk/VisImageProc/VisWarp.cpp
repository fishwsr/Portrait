///////////////////////////////////////////////////////////////////////////
//
// NAME
//  ImageWarp.cpp -- image warping routines
//  
// DESCRIPTION
//  These routines are used for general image warping,
//  e.g., for morphing or depth-corrected resampling.
//
//  For a longer description, see ImageWarp.h.
//
// SEE ALSO
//  ImageWarp.h         description of resampling routines
//  VisResample.h     description of resampling routines
//
// BUGS
//  
//
// DESIGN
//  
//
// HISTORY
//  10-May-96  Richard Szeliski (szeliski) at Microsoft
//  Split off from VisResample.h
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////

#include "VisImageProcPch.h"
#include "VisImageProc.h"


//
//  If necessary, interpolate up the (u,v) coordinates
//

SVisUVCoord *uv_interpolate(CVisFloatImage &uv, int r, int subsample,
                         SVisUVCoord *uv_row)
{
    if (subsample == 1)
        return (SVisUVCoord *) uv.PtrToFirstPixelInRow(r + uv.Top());
    else {
        float si = 1.0f / subsample;
        float rs = r * si;
        int   r0 = (int) floor(rs);
        int   r1 = r0 + 1;
        float a  = rs - r0;
        int cols = uv.Width(), j, k;
        SVisUVCoord *row0 = (SVisUVCoord *) uv.PtrToFirstPixelInRow(r0 + uv.Top());
        SVisUVCoord *row1 = (SVisUVCoord *) uv.PtrToFirstPixelInRow(r1 + uv.Top());
        for (j = 0, k = 0; j < cols; j++, k += subsample) {
            uv_row[k].u = row0[j].u + a * (row1[j].u - row0[j].u);
            uv_row[k].v = row0[j].v + a * (row1[j].v - row0[j].v);
        }
        for (j = 0; j < (cols-1)*subsample; j += subsample) {
            float b = si;
            for (k = 1; k < subsample; k++, b += si) {
                uv_row[j+k].u = uv_row[j].u +
                    b * (uv_row[j+subsample].u - uv_row[j].u);
                uv_row[j+k].v = uv_row[j].v +
                    b * (uv_row[j+subsample].v - uv_row[j].v);
            }
        }
        return uv_row;
    }
}

//
//  Nearest neighbor interpolation
//

template <class TPixel>
void VisInverseResampleNearest(void **ppvIliffe,
		CVisImage<TPixel> &src, TPixel *dst,
		SVisUVCoord *uv, int cols, int zero_outside)
{
    int bands = src.NBands();
    int psize = src.CbPixel();

    TPixel **texel = (TPixel **) ppvIliffe;
#if 0	// bug found by Rick 22-Aug-97
    int o = src.AR().Left() - src.MR().Left();
#elif 0 // can't access protected member
	int o = src.m_pixel_offsets[src.Left()] - src.Left();
#else
	int o = &src.Pixel(src.StartPoint()) - 
			&texel[src.Top()][src.Left()];
#endif

    for (int c = 0; c < cols; c++, dst += bands, uv++) {
        int iu = (int) floor(uv->u);
        int iv = (int) floor(uv->v);
        if (src.ContainsPoint(iu, iv)) {
            if (bands == 1)
                *dst = texel[iv][iu+o];
            else
                memcpy(dst, &texel[iv][(iu+o)*bands], bands*psize);
        } else if (zero_outside)
            memset(dst, 0, bands*psize);
    } 
}

//
//  Bilinear sub-pixel interpolation
//

template <class TPixel>
void VisInverseResampleLinear(void **ppvIliffe,
		CVisImage<TPixel> &src, TPixel *dst, TPixel *grad,
		SVisUVCoord *uv, int cols, int zero_outside)
{
    int bands = src.NBands();
    int psize = src.CbPixel();

    TPixel **texel = (TPixel **) ppvIliffe;
#if 0	// bug found by Rick 22-Aug-97
    int o = src.AR().Left() - src.MR().Left();
#elif 0 // can't access protected member
	int o = src.m_pixel_offsets[src.Left()] - src.Left();
#else
	int o = &src.Pixel(src.StartPoint(), 0) - 
			&texel[src.Top()][src.Left()];
#endif

    // UNDONE: gradient computation
    assert(grad == 0);

    for (int c = 0; c < cols; c++, dst += bands, uv++) {

        int iu = (int) floor(uv->u);    // integral part
        int iv = (int) floor(uv->v);
        double fu = uv->u - iu;         // fractional part
        double fv = uv->v - iv;

        // Bounds check
        if (! src.ContainsPoint(iu, iv) ||
            (fu != 0) && ! src.ContainsPoint(iu+1, iv) ||
            (fv != 0) && ! src.ContainsPoint(iu, iv+1)) {
            if (zero_outside)
                memset(dst, 0, bands*psize);
			continue;
        }

        // Corner values
        iu += o;
        TPixel *t00 = &texel[iv][iu*bands];  // pixel patch corners
        TPixel *t01 = &texel[iv][iu*bands+bands];
        TPixel *t10 = &texel[iv+1][iu*bands];
        TPixel *t11 = &texel[iv+1][iu*bands+bands];
		// note: to be on the safe side, the image Iliffe vector should have
		//	one extra dummy row entry, say pointing to the start of the image!
        
        // Special case on fractional parts (is this worth it?)
        if (fu == 0 && fv == 0) {
            for (int b = 0; b < bands; b++)
                dst[b] = t00[b];
            // alternate: memcpy(dst, t00, bands*psize);
        } else if (fu == 0) {
            for (int b = 0; b < bands; b++)
                dst[b] = (TPixel) (t00[b] + fv * (t10[b] - t00[b]));
        } else if (fv == 0) {
            for (int b = 0; b < bands; b++)
                dst[b] = (TPixel) (t00[b] + fu * (t01[b] - t00[b]));
        } else {
            for (int b = 0; b < bands; b++) {
                double t0 = t00[b] + fu * (t01[b] - t00[b]);
                double t1 = t10[b] + fu * (t11[b] - t10[b]);
                dst[b] = (TPixel) (t0 + fv * (t1  - t0));
            }
        }
    }
}

//
//  Bilinear sub-pixel interpolation for gray pixels
//		We use integer multiplies for speed (architecture dependant)
//		Just ifdef this part out if it doesn't improve performance
//

void VisInverseResampleLinear(void **ppvIliffe,
		CVisImage<BYTE> &src, BYTE *dst, BYTE *grad,
		SVisUVCoord *uv, int cols, int zero_outside)
{
    int bands = src.NBands();

    BYTE **texel = (BYTE **) ppvIliffe;
#if 0	// bug found by Rick 22-Aug-97
    int o = src.AR().Left() - src.MR().Left();
#elif 0 // can't access protected member
	int o = src.m_pixel_offsets[src.Left()] - src.Left();
#else
	int o = &src.Pixel(src.StartPoint(), 0) - 
			&texel[src.Top()][src.Left()];
#endif

    // UNDONE: gradient computation
    assert(grad == 0);

    for (int c = 0; c < cols; c++, dst += bands, uv++) {

        int uu = (int) (uv->u * 256.0f);
        int vv = (int) (uv->v * 256.0f);
        int fu = uu & 255;			// fractional part
		int fv = vv & 255;
        int iu = (uu - fu) / 256;   // integral part
        int iv = (vv - fv) / 256;

        // Bounds check
        if (! src.ContainsPoint(iu, iv) ||
            (fu != 0) && ! src.ContainsPoint(iu+1, iv) ||
            (fv != 0) && ! src.ContainsPoint(iu, iv+1)) {
            if (zero_outside)
                memset(dst, 0, bands);
			continue;
        }

        // Corner values
        iu += o;
        BYTE *t00 = &texel[iv][iu*bands];  // pixel patch corners
        BYTE *t01 = &texel[iv][iu*bands+bands];
        BYTE *t10 = &texel[iv+1][iu*bands];
        BYTE *t11 = &texel[iv+1][iu*bands+bands];
		// note: to be on the safe side, the image Iliffe vector should have
		//	one extra dummy row entry, say pointing to the start of the image!

        // Special case on fractional parts (is this worth it?)
        if (fu == 0 && fv == 0) {
            for (int b = 0; b < bands; b++)
                dst[b] = t00[b];
            // alternate: memcpy(dst, t00, bands);
        } else if (fu == 0) {
            for (int b = 0; b < bands; b++)
                dst[b] = (BYTE) (t00[b] + fv * (t10[b] - t00[b]) / 256);
        } else if (fv == 0) {
            for (int b = 0; b < bands; b++)
                dst[b] = (BYTE) (t00[b] + fu * (t01[b] - t00[b]) / 256);
        } else {
            for (int b = 0; b < bands; b++) {
                int t0 = t00[b] + fu * (t01[b] - t00[b]);
                int t1 = t10[b] + fu * (t11[b] - t10[b]);
                dst[b] = (BYTE) (t0 + fv * (t1  - t0) / (256 * 256));
            }
        }
    }
}

//
//  Bilinear sub-pixel interpolation for COLOR pixels (CVisRGBABytePixel)
//		We use 32-bit ints to perform the linear interpolation two at a time
//

inline static int blend32_part1(int x, int y, int alpha)
{
    int x1 = x & 0x00ff00ff;
	int y1 = y & 0x00ff00ff;
    int z1 = alpha * (y1 - x1) + 0x00800080;   // add rounding fraction
    int z2 = x1 + (z1 >> 8);
	return z2 & 0x00ff00ff;
}

inline static int blend32_part2(int x1, int y1, int x2, int y2, int alpha)
{
    int z1 = blend32_part1(x1, y1, alpha);
    int z2 = blend32_part1(x2, y2, alpha);
	return (z1 & 0x00ff00ff) | ((z2 & 0x00ff00ff) << 8);
}

inline static int blend32(int x, int y, int alpha)
{
    return blend32_part2(x, y, x >> 8, y >> 8, alpha);
}

inline static int half_diff(int v0, int v1)
{
    // Return half of byte-wise difference, offset by 128
    //  (see Rick's notes, 05-Nov-98)
    // 1R 1G 1B 1A - 0R 0G 0B 0A
    v0 = (v0 >> 1) & 0x7f7f7f7f;
    v1 = (v1 >> 1) & 0x7f7f7f7f;
    int diff = (v1 | 0x80808080) - v0;
    return diff;
}

void VisInverseResampleLinear(void **ppvIliffe,
		CVisImage<CVisRGBABytePixel> &src, CVisRGBABytePixel *dstc,
        CVisRGBABytePixel *gradc,
		SVisUVCoord *uv, int cols, int zero_outside)
{
    assert(src.NBands() == 1);  // ONLY works for single band color
    assert(src.CbPixel() == sizeof(int));

    int **texel = (int **) ppvIliffe;    // we'll use whole words
#if 0	// bug found by Rick 22-Aug-97
    int o = src.AR().Left() - src.MR().Left();
#elif 0 // can't access protected member
	int o = src.m_pixel_offsets[src.Left()] - src.Left();
#else
	int o = ((int *) &src.Pixel(src.StartPoint(), 0)) - 
					 &texel[src.Top()][src.Left()];
#endif

    int *dst = (int *) dstc;
    int *igrad = (int *) gradc;
    for (int c = 0; c < cols; c++, dst++, uv++, igrad += 2) {

        int uu = (int) (uv->u * 256.0f);
        int vv = (int) (uv->v * 256.0f);
        int fu = uu & 255;			// fractional part
		int fv = vv & 255;
        int iu = (uu - fu) / 256;   // integral part
        int iv = (vv - fv) / 256;

        // Default value
        if (zero_outside)
            *dst = 0;
        if (gradc)
            igrad[0] = igrad[1] = 0;

        // Bounds check
        if (! src.ContainsPoint(iu, iv) ||
            (fu != 0) && ! src.ContainsPoint(iu+1, iv) ||
            (fv != 0) && ! src.ContainsPoint(iu, iv+1)) {
			continue;
        }

        // Corner values
        iu += o;
        int t00 = texel[iv][iu];  // pixel patch corners

        // Special case on fractional parts (is this worth it?)
        if (fu == 0 && fv == 0)
            *dst = t00;
        else if (fu == 0.0f) {
            int t10 = texel[iv+1][iu];
            *dst = blend32(t00, t10, fv);
        } else if (fv == 0.0f) {
            int t01 = texel[iv][iu+1];
            *dst = blend32(t00, t01, fu);
        } else {
            int t01 = texel[iv][iu+1];
            int t10 = texel[iv+1][iu];
            int t11 = texel[iv+1][iu+1];
			int t0l = blend32_part1(t00, t01, fu);
			int t0h = blend32_part1(t00 >> 8, t01 >> 8, fu);
			int t1l = blend32_part1(t10, t11, fu);
			int t1h = blend32_part1(t10 >> 8, t11 >> 8, fu);
			int val = blend32_part2(t0l, t1l, t0h, t1h, fv);
            *dst = val;
        }

        // Compute gradient
        if (gradc && src.ContainsPoint(iu+1, iv+1)) {
            int t01 = texel[iv][iu+1];
            int t10 = texel[iv+1][iu];
            int t11 = texel[iv+1][iu+1];
            int g0x = half_diff(t00, t01);
            int g1x = half_diff(t10, t11);
            igrad[0]= blend32(g0x, g1x, fv);
            int gy0 = half_diff(t00, t10);
            int gy1 = half_diff(t01, t11);
            igrad[1]= blend32(gy0, gy1, fu);
        }

        // Gradient value
    }
}


//
// Bicubic sub-pixel interpolation
//  See Rick's research notes, 08-Feb-99.
//

template <class TPixel>
inline void VisLinearInterpolate(TPixel& s0, TPixel& s1, int it, TPixel& dst)
{
    double t = it * (1.0 / 256.0);
    dst = TPixel(double(s0) + t * (double(s1) - double(s0)));
}

inline void VisLinearInterpolate(unsigned char& s0, unsigned char& s1, int it, unsigned char& dst)
{
    dst = s0 + ((it * (int(s1) - int(s0)) + 128) / 256);
}

inline void VisLinearInterpolate(CVisRGBABytePixel& s0, CVisRGBABytePixel& s1, int it, CVisRGBABytePixel& dst)
{
    int &i0 = *(int *) &s0, &i1 = *(int *) &s1, &idst = *(int *) &dst;
    idst = blend32(i0, i1, it);
}

inline void VisLinearInterpolate(CVisRGBAFloatPixel& s0, CVisRGBAFloatPixel& s1, int it, CVisRGBAFloatPixel& dst)
{
    double t = it * (1.0 / 256.0);
    for (int b = 0; b < 4; b++)
        dst[b] = s0[b] + t * (s1[b] - s0[b]);
}

template <class TPixel>
inline void VisCubicCorrection(TPixel& s0, TPixel& s1, TPixel& s2,
                               int it, TPixel& correction)
{
    if (it == 0) {
        correction = 0;
        return;
    }
    double d1 = double(s1) - 0.5 * (double(s0) + double(s2));
    double t = it * (1.0 / 256.0);
    correction = TPixel(d1 * t * (1.0 - t) * (1.0 -t));
}

void VisCubicCorrection(unsigned char& s0, unsigned char& s1, unsigned char& s2,
                        int it, unsigned char& correction)
{
    if (it == 0) {
        correction = 0;
        return;
    }
    static unsigned char LUT[257];
    if (LUT[128] == 0) {
        for (int i = 0; i <= 256; i++)
            LUT[i] = (i * (256 - i) * (256 - i) + 64*128) / (128 * 128);
            // Phi_1 value scaled by 256 * 4: max is 16/27 * 256
    }
    int d1 = 2 * int(s1) - (int(s0) + int(s2));
    correction = (d1 * LUT[it] + 256*2) / (256 * 4) + 128;  // 2x actual correction
}

void VisCubicCorrection(CVisRGBABytePixel& s0, CVisRGBABytePixel& s1, CVisRGBABytePixel& s2,
                        int it, CVisRGBABytePixel& correction)
{
    if (it == 0 || s0.A() != 255 || s2.A() != 255) {
        correction.SetRGBA(128, 128, 128, 128);
        return;
    }
    for (int b = 0; b < 4; b++)
        VisCubicCorrection(s0[b], s1[b], s2[b], it, correction[b]);
}

void VisCubicCorrection(CVisRGBAFloatPixel& s0, CVisRGBAFloatPixel& s1, CVisRGBAFloatPixel& s2,
                        int it, CVisRGBAFloatPixel& correction)
{
    if (it == 0 || s0.A() != 255 || s2.A() != 255) {
        correction.SetRGBA(128, 128, 128, 128);
        return;
    }
    for (int b = 0; b < 4; b++)
        VisCubicCorrection(s0[b], s1[b], s2[b], it, correction[b]);
}

template <class TPixel>
inline void VisAddCorrections(TPixel& src, TPixel& c0, TPixel& c1, TPixel& dst)
{
    dst = src + c0 + c1;
}

void VisAddCorrections(unsigned char& src, unsigned char& c0, unsigned char& c1, unsigned char& dst)
{
    int idst = src + (c0 + c1 - 256 + 1) / 2;
    if (idst > 255)
        idst = 255;
    if (idst < 0)
        idst = 0;
    dst = idst;
}

void VisAddCorrections(CVisRGBABytePixel& src, CVisRGBABytePixel& c0, CVisRGBABytePixel& c1, CVisRGBABytePixel& dst)
{
    for (int b = 0; b < 4; b++)
        VisAddCorrections(src[b], c0[b], c1[b], dst[b]);
}

template <class TPixel>
inline void VisCubicInterpolate(TPixel *src, int b, int it,
                                int mask, TPixel& dst)
{
    dst = src[0];
    if (it == 0)
        return;
    VisLinearInterpolate(src[0], src[b], it, dst);
    TPixel corr0, corr1;
    VisCubicCorrection(src[-b], src[0], src[b],  (mask & 1) ?     it : 0, corr0);
    VisCubicCorrection(src[0], src[b], src[b+b], (mask & 2) ? 256-it : 0, corr1);
    VisAddCorrections(dst, corr0, corr1, dst);
}

template <class TPixel>
void VisInverseResampleCubic(
		CVisImage<TPixel> &src, TPixel *dst, TPixel *grad,
		SVisUVCoord *uv, int cols, int zero_outside)
{
    int bands = src.NBands();
    int psize = src.CbPixel();
    static TPixel empty;

    // UNDONE: gradient computation
    assert(grad == 0);

    for (int c = 0; c < cols; c++, dst += bands, uv++) {

        int uu = (int) (uv->u * 256.0f);
        int vv = (int) (uv->v * 256.0f);
        int fu = uu & 255;			// fractional part
		int fv = vv & 255;
        int iu = (uu - fu) / 256;   // integral part
        int iv = (vv - fv) / 256;

        // Bounds check
        if (! src.ContainsPoint(iu, iv) ||
            (fu != 0) && ! src.ContainsPoint(iu+1, iv) ||
            (fv != 0) && ! src.ContainsPoint(iu, iv+1)) {
            if (zero_outside)
                memset(dst, 0, bands*psize);
			continue;
        }

        // Horizontal and vertical in-bounds masks
        int hmask = (src.ContainsPoint(iu-1, iv) ? 1 : 0) + 
                    (src.ContainsPoint(iu+2, iv) ? 2 : 0);
        int vmask = (src.ContainsPoint(iu, iv-1) ? 1 : 0) + 
                    (src.ContainsPoint(iu, iv+2) ? 2 : 0);

        // Special case on fractional parts (is this worth it?)
        if (fu == 0 && fv == 0) {
            TPixel *t00 = &src.Pixel(iu, iv, 0);
            for (int b = 0; b < bands; b++)
                dst[b] = t00[b];
            continue;
            // alternate: memcpy(dst, t00, bands*psize);
        }

        // Iterate over the bands
        for (int b = 0; b < bands; b++) {

            // Compute the four horizontally resampled values
            TPixel rv[4];
            if ((vmask & 1) && fv)
                VisCubicInterpolate(&src.Pixel(iu, iv-1, b), bands, fu, hmask, rv[0]);
            if (1)
                VisCubicInterpolate(&src.Pixel(iu, iv+0, b), bands, fu, hmask, rv[1]);
            if (fv)
                VisCubicInterpolate(&src.Pixel(iu, iv+1, b), bands, fu, hmask, rv[2]);
            if ((vmask & 2) && fv)
                VisCubicInterpolate(&src.Pixel(iu, iv+2, b), bands, fu, hmask, rv[3]);

            // Combine the values
            VisCubicInterpolate(&rv[1], 1, fv, vmask, dst[b]);
        }
    }
}

//
//  Some explicit instantiations, for code in VisResample.cpp
//
template
void VisInverseResampleCubic(
		CVisImage<unsigned char> &src, unsigned char *dst, unsigned char *grad,
		SVisUVCoord *uv, int cols, int zero_outside);

template
void VisInverseResampleCubic(
		CVisImage<int> &src, int *dst, int *grad,
		SVisUVCoord *uv, int cols, int zero_outside);

template
void VisInverseResampleCubic(
		CVisImage<CVisRGBAFloatPixel> &src, CVisRGBAFloatPixel *dst, CVisRGBAFloatPixel *grad,
		SVisUVCoord *uv, int cols, int zero_outside);

//
//  VisInverseWarp:
//      look up (u,v) coordinates in src
//


template <class TPixel>
void VisInverseWarpInternal(CVisImage<TPixel> &src, 
                    CVisImage<TPixel> &dst, 
                    CVisFloatImage &uv,
                    TVisInterpolation interpolation,
                    int uv_spacing, int zero_outside,
                    CVisImage<TPixel> *grad_img_ptr)
{
    int nrows = dst.Height(), ncols = dst.Width();
    SVisUVCoord *uv_row = (uv_spacing > 1) ? 
        new SVisUVCoord[uv.Width()*uv_spacing] : 0;

    // Check that uv is compatible with dst
    assert(uv.NBands() == 2);
    assert((uv.Width()-1)*uv_spacing+1 >= dst.Width());
    assert(src.NBands() == dst.NBands());

	// Get the Iliffe vector for the source image.
	// UNDONE:  Are only pointers to (active) image rows needed, or are
	// pointers to all memory rows needed.
	CVisMemBlockOf<void *> memblockIliffe(src.Height());
	void **ppvIliffe = src.PpvFillIliffeVector(memblockIliffe.PObj());

    for (int r = 0; r < nrows; r++) {
        TPixel *dst_ptr = dst.PtrToFirstPixelInRow(r + dst.Top());
        SVisUVCoord *uv_ptr = uv_interpolate(uv, r, uv_spacing, uv_row);
        TPixel *grad_ptr = grad_img_ptr ? grad_img_ptr->PtrToFirstPixelInRow(r + dst.Top()) : 0;
        if (interpolation == VIS_INTERP_NEAREST)
            VisInverseResampleNearest(ppvIliffe, src, dst_ptr, uv_ptr, ncols, zero_outside);
        else if (interpolation == VIS_INTERP_LINEAR)
            VisInverseResampleLinear(ppvIliffe, src, dst_ptr, grad_ptr, uv_ptr, ncols, zero_outside);
        else if (interpolation == VIS_INTERP_CUBIC)
            VisInverseResampleCubic(src, dst_ptr, grad_ptr, uv_ptr, ncols, zero_outside);
        else
            assert(0);
    }
    if (uv_row)
        delete [] uv_row;
}

void VisInverseWarp(CVisRGBAByteImage &src, CVisRGBAByteImage &dst, 
                      CVisFloatImage &uv,
                      TVisInterpolation interpolation,
                      int uv_spacing, int zero_outside,
                      CVisRGBAByteImage *grad_img_ptr)
{
    VisInverseWarpInternal(src, dst, uv, interpolation, uv_spacing, 
        zero_outside, grad_img_ptr);
}

void VisInverseWarp(CVisByteImage &src, CVisByteImage &dst, 
                      CVisFloatImage &uv,
                      TVisInterpolation interpolation,
                      int uv_spacing, int zero_outside,
                      CVisByteImage *grad_img_ptr)
{
    VisInverseWarpInternal(src, dst, uv, interpolation, uv_spacing, 
        zero_outside, grad_img_ptr);
}

void VisInverseWarp(CVisFloatImage &src, CVisFloatImage &dst, 
                    CVisFloatImage &uv,
                    TVisInterpolation interpolation,
                    int uv_spacing, int zero_outside,
                    CVisFloatImage *grad_img_ptr)
{
    VisInverseWarpInternal(src, dst, uv, interpolation, uv_spacing, 
        zero_outside, grad_img_ptr);
}

#if 0   // No instantiation needed
void VisInverseWarpTest1()
{
    CVisByteImage gimg;
    CVisRGBAByteImage cimg;
    CVisFloatImage fimg;
#ifdef _DEBUG
    VisInverseWarp(gimg, gimg, fimg, VIS_INTERP_NEAREST, 1, 1, (CVisByteImage *) 0);
#endif // NDEBUG
    VisInverseWarp(cimg, cimg, fimg, VIS_INTERP_NEAREST, 1, 1, (CVisRGBAByteImage *) 0);
}
#endif
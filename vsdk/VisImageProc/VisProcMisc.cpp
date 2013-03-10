///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisProc.cpp -- image processing routines
//
// DESCRIPTION
//  These are examples of simple "processing" functions.
//  They are temporarily being used to debug/construct the vision library.
//
// SEE ALSO
//  
//
// BUGS
//  
//
// DESIGN
//
//
// HISTORY
//  01-Mar-96  Richard Szeliski (szeliski) at Microsoft
//  Created.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////


#include "VisImageProcPch.h"
#include "VisImageProc.h"


//
// Subsample(): keep only every nth pixel
//

template<class TYPE>
static void SubsampleRow(TYPE *src, TYPE *dst, int ncols, int rate)
{
    for (int c = 0; c < ncols; c++, src +=rate, dst++)
            *dst = *src;
}


// Subsampling can be implemented without knowing the pixel type
void VisSubSample(CVisImageBase *src, CVisImageBase *dst, int rate)
{
    int nrows = dst->Height(), ncols = dst->Width();
    int nbands = dst->NBands(), psize = nbands * dst->CbPixel();
    for (int rs = 0, rd = 0; rd < nrows; rs += rate, rd++) {
        BYTE *p  = (BYTE *) src->PbPixel(src->Left(), rs+src->Top());
        BYTE *q  = (BYTE *) dst->PbPixel(dst->Left(), rd+dst->Top());

        switch (psize) {
        case sizeof(BYTE):
            SubsampleRow((BYTE *) p, (BYTE *) q, ncols, rate);
            break;
        case sizeof(short):
            SubsampleRow((short *) p, (short *) q, ncols, rate);
            break;
        case sizeof(int):
            SubsampleRow((int *) p, (int *) q, ncols, rate);
            break;
#ifdef SIXTY_FOUR_BIT_LONG
        case sizeof(long):
            SubsampleRow((long *) p, (long *) q, ncols, rate);
            break;
#else
        case sizeof(double):
            SubsampleRow((double *) p, (double *) q, ncols, rate);
            break;
#endif
        default:
            for (int c = 0; c < ncols; c++, p += rate*psize, q += psize)
                memcpy(q, p, psize);
        }
    }
}

//
// Supersample(): replicate each pixel n times
//

template<class TYPE>
static void SupersampleRow(TYPE *src, TYPE *dst, int ncols, int rate)
{
    for (int c = 0, d = rate-1; c < ncols; c++, src += (d == 0), d--) {
        *dst++ = *src;
        if (d < 0)
            d += rate;
    }
}


// Supersampling can be implemented without knowing the pixel type
void VisSuperSample(CVisImageBase *src, CVisImageBase *dst, int rate)
{
    int nrows = dst->Height(), ncols = dst->Width();
    int nbands = dst->NBands(), psize = nbands * dst->CbPixel();
    for (int rs = 0, rd = 0; rd < nrows; rs += ((rd%rate)==rate-1), rd++) {
        BYTE *p  = (BYTE *) src->PbPixel(src->Left(), rs+src->Top());
        BYTE *q  = (BYTE *) dst->PbPixel(dst->Left(), rd+dst->Top());

        switch (psize) {
        case sizeof(BYTE):
            SupersampleRow((BYTE *) p, (BYTE *) q, ncols, rate);
            break;
        case sizeof(short):
            SupersampleRow((short *) p, (short *) q, ncols, rate);
            break;
        case sizeof(int):
            SupersampleRow((int *) p, (int *) q, ncols, rate);
            break;
#ifdef SIXTY_FOUR_BIT_LONG
        case sizeof(long):
            SupersampleRow((long *) p, (long *) q, ncols, rate);
            break;
#else
        case sizeof(double):
            SupersampleRow((double *) p, (double *) q, ncols, rate);
            break;
#endif
        default:
            for (int c = 0, d = rate-1; c < ncols; 
                 c++, p += ((d == 0) ? psize : 0), q += psize, d--) {
                memcpy(q, p, psize);
                if (d < 0)
                    d += rate;
            }
        }
    }
}

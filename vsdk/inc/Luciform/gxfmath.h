#pragma once

#ifndef __GXFMATH_H
#define __GXFMATH_H

/*//////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 1998 - 2000 Microsoft Corporation.  All Rights Reserved.
//
// File: gxfmath.h
// Description:
//    This include file contains versions of the math.h functions that are used
//  by the mathcore.  These functions implement single-precision floating point
//  (SPFP) functions that are used for SPFP mathcore template instantiations.
//  There are also some other useful utilies for single-precision computations.
//
//
//////////////////////////////////////////////////////////////////////////////*/

#include <math.h>


//-----------------------------------------------------------------------------
// The following functions should be defined for each instantiated template 
// type in the mathcore.
//-----------------------------------------------------------------------------

inline bool
GXSubset(float x, float y)
{
    return x == y;
}

inline float
GXInfinite(float)
{
    return 1.0e19f;     // a little under sqrt(FLT_MAX)
}

inline float
GXMax(float x, float y)
{
    return (x > y) ? x : y;
}

inline float
GXMin(float x, float y)
{
    return (x < y) ? x : y;
}

inline float
GXIntersection(float x, float y)
{
    return x;
}

inline float
GXBound(float x, float y)
{
    return y;
}

inline float
GXSqr( float x )
{
    return x*x;
}

inline float
GXHypot( float x, float y )
{
    return (float) _hypot(x, y);
}

inline void
GXSinCos( float fRadians, float *pfSin, float *pfCos )
{
#ifdef _X86_
    _asm
    {
        mov eax,pfSin
        mov edx,pfCos
        fld fRadians
        // fsincos
        _emit 0xd9
        _emit 0xfb
        fstp dword ptr [edx]
        fstp dword ptr [eax]
    }
#else
    *pfSin = sinf( fRadians );
    *pfCos = cosf( fRadians );
#endif
}

#if defined (_MSC_VER) && (_MSC_VER >= 1100)

inline float
GXACos(float f)
{
    return acosf(f);
}

inline float
GXASin(float f)
{
    return asinf(f);
}

inline float
GXATan(float f)
{
    return atanf(f);
}

inline float
GXATan2(float f1, float f2)
{
    return atan2f(f1, f2);
}

inline float
GXCos(float f)
{
    return cosf(f);
}

inline float
GXCosh(float f)
{
    return coshf(f);
}

inline float
GXExp(float f)
{
    return expf(f);
}

inline float
GXFAbs(float f)
{
    return fabsf(f);
}

inline float
GXFMod(float f1, float f2)
{
    return fmodf(f1, f2);
}

inline float
GXLog(float f)
{
    return logf(f);
}

inline float
GXLog10(float f)
{
    return log10f(f);
}


inline float
GXPow(float f1, float f2)
{
    return powf(f1, f2);
}

inline float
GXSin(float f)
{
    return sinf(f);
}

inline float
GXSinh(float f)
{
    return sinhf(f);
}

inline float
GXTan(float f)
{
    return tanf(f);
}

inline float
GXTanh(float f)
{
    return tanhf(f);
}

inline float
GXSqrt(float f)
{
    return sqrtf(f);
}

inline float
GXCeil(float f)
{
    return ceilf(f);
}

inline float
GXFloor(float f)
{
    return floorf(f);
}

inline float
GXModf(float f, float *pf)
{
    return modff(f, pf);
}

//
//------------------------------------------------------------------------------
// Function:    GXInvSqrt
// Purpose:     Compute inverse square root - full precision
//------------------------------------------------------------------------------
//
inline float
GXInvSqrt(float f)
{
    return 1.f / sqrtf(f);
}

#endif // defined (_MSC_VER) && (_MSC_VER >= 1100)


//
//-----------------------------------------------------------------------------
//   The IEEE single precision floating point format is:
// 
//   SEEEEEEEEMMMMMMM MMMMMMMMMMMMMMMM
// 
//   S = Sign bit for whole number
//   E = Exponent bit (exponent in excess 127 form)
//   M = Mantissa bit
//-----------------------------------------------------------------------------
//
#ifndef _nEXPBIAS
#define _nEXPBIAS       127
#define _maskEXP        0x7f800000
#define _nMANTBITS      23
#define _nEXPLSB        (1 << _nMANTBITS)
#endif // #ifndef _nEXPBIAS

// Note: VC4.2 had some strangeness with breaking the GXFloatToFixedXXX
//       inline functions in Release mode
//
//-----------------------------------------------------------------------------
// Function:    GXFloatToFixed
// Author:      mikemarr
// Purpose:     Compute the 16.16 fixed point from the float
// Notes:       Relies on IEEE bit representation of floating point numbers
//-----------------------------------------------------------------------------
//
inline UINT
GXFloatToFixed(float fSrc)
{
    UINT nRaw = *((UINT *) &(fSrc));
    // deal with invalid behavior with right shift more than 31
    if (nRaw < ((_nEXPBIAS + 15 - 31) << _nMANTBITS))
        return 0;
    else
        return ((nRaw | _nEXPLSB) << 8) >> ((_nEXPBIAS + 15) - (nRaw >> _nMANTBITS));
}

//
//-----------------------------------------------------------------------------
// Function:    GXFloatToFixedNoScale
// Author:      mikemarr
// Purpose:     Compute the 16.16 fixed point from the float, divides by 2^16
// Notes:       Relies on IEEE bit representation of floating point numbers
//-----------------------------------------------------------------------------
//
inline UINT
GXFloatToFixedNoScale(float fSrc)
{
    UINT nRaw = *((UINT *) &(fSrc));
    // deal with invalid behavior with right shift more than 31
    if (nRaw < (_nEXPBIAS << _nMANTBITS))
        return 0;
    else
        return ((nRaw | _nEXPLSB) << 8) >> ((_nEXPBIAS + 31) - (nRaw >> _nMANTBITS));
}

//
//-----------------------------------------------------------------------------
// Macro:       GXFLOAT_TO_FIXED
// Author:      mikemarr
// Purpose:     Compute the 16.16 fixed point from the float
// Notes:       Relies on IEEE bit representation of floating point numbers
//-----------------------------------------------------------------------------
//
#define GXFLOAT_TO_FIXED(__nDst, __fSrc) \
do { \
    float fTmp = __fSrc; \
    UINT nRaw = *((UINT *) &(fTmp)); \
    if (nRaw < ((_nEXPBIAS + 15 - 31) << _nMANTBITS)) \
        __nDst = 0; \
    else \
    __nDst = (((nRaw | _nEXPLSB) << 8) >> ((_nEXPBIAS + 15) - (nRaw >> _nMANTBITS))); \
} while (0)

//
//-----------------------------------------------------------------------------
// Macro:       GXFLOAT_TO_FIXED_NO_SCALE
// Author:      mikemarr
// Purpose:     Compute the 16.16 fixed point from the float, divides by 2^16
// Notes:       Relies on IEEE bit representation of floating point numbers
//-----------------------------------------------------------------------------
//
#define GXFLOAT_TO_FIXED_NO_SCALE(nDst, fSrc) \
do { \
    float fTmp = fSrc; \
    UINT nRaw = *((UINT *) &(fTmp)); \
    if (nRaw < (_nEXPBIAS << _nMANTBITS)) \
        nDst = 0; \
    else \
        nDst = (((nRaw | _nEXPLSB) << 8) >> ((_nEXPBIAS + 31) - (nRaw >> _nMANTBITS))); \
} while (0)

#define _nRAWUNSATURATED ((_nEXPBIAS - 8) << _nMANTBITS)
#define _nRAWSATURATED (_nEXPBIAS << _nMANTBITS)

//-----------------------------------------------------------------------------
// Function:    GXFloatToByteTrunc
// Author:      mikemarr
//     Quantizes IEEE floating point range [0.f, 1.f) to 8 bits.  Clamp outside 
//  of this range.  ie. uTrunc = Trunc(f * 256.f)
//  Notice that
//      Round(f * 256.f - 0.5f) != Trunc(f * 256.f)
//  because of round-to-even.
//  ex. Round(3.f/256.f - 0.5f) = 2
//      Trunc(3.f/256.f) = 3
//-----------------------------------------------------------------------------
inline UINT
GXFloatToByteTrunc(float f)
{
    // load floating point number into an integer register
    UINT nRaw = *((UINT *) &(f));

    // - check for larger than zero, but small enough to trunc to zero
    // Note: this is necessary because right shift is implemented incorrectly 
    //       on the X86 - instead of computing zero for right shift >= 32, it 
    //       just shifts by 5 LSB's
    // - negative numbers masquerade as large numbers because of the sign bit
    if ((nRaw < _nRAWUNSATURATED) | (nRaw >> 31))
        return 0;

    // compute value when f < 1.f
    if (nRaw < _nRAWSATURATED)
        return ((nRaw | _nEXPLSB) << 8) >> ((_nEXPBIAS + 23) - (nRaw >> _nMANTBITS));

    // saturated greater than or equal to 1.f, return 255
    return 0xff;
}

#undef _nRAWUNSATURATED
#undef _nRAWSATURATED


#define fTINY_EPS       1.E-35F
#define fZERO_EPS       1.E-7F
#define nTOLERANCE_BITS 4
#define nEXP_OFFSET     (_nMANTBITS - nTOLERANCE_BITS) << _nMANTBITS
//
//------------------------------------------------------------------------------
// Function:    GXFloatEquals
// Author:      mikemarr
// Purpose:     Perform a "fuzzy" compare of two floating point numbers
// Notes:       Relies on IEEE bit representation of floating point numbers
//------------------------------------------------------------------------------
//
extern "C" inline bool __cdecl
GXFloatEquals(float x1, float x2)
{
    float fEps;
    if ((x1 == 0.0f) || (x2 == 0.0f)) {
        fEps = fZERO_EPS;
    } else {
        float fMaxX;
        
        if (x1 > x2) 
            fMaxX = x1;
        else 
            fMaxX = x2;
        
        // grab the exponent of the larger number
        unsigned int uExp = (*((unsigned int *) &fMaxX) & _maskEXP);

        if (uExp < nEXP_OFFSET) {
            // near zero
            fEps = fTINY_EPS;
        } else {
            uExp -= nEXP_OFFSET;
            fEps = *((float *) &uExp);
        }
    }
    return (((x1 + fEps) >= x2) && ((x1 - fEps) <= x2));
}

#undef fZERO_EPS
#undef nTOLERANCE_BITS
#undef nEXP_OFFSET

#ifndef GXISWAP
    #define GXISWAP(a,b) do { (a)^=(b); (b)^=(a); (a)^=(b); } while (0)
#endif

//
//------------------------------------------------------------------------------
// Function:    GXFloatEquals
// Author:      mikemarr
// Purpose:     Perform a "fuzzy" compare of two floating point numbers, by
//              specifying the number of LSB's the floats can differ
// Notes:       Relies on IEEE bit representation of floating point numbers
//------------------------------------------------------------------------------
//
inline bool __cdecl
GXFloatEquals(float f1, float f2, UINT nToleranceBits)
{
    UINT uExpMin, uExpMax, uExpOffset;
    float fEps;

    // compute the larger exponent
    uExpMin = (*((unsigned int *) &f1) & _maskEXP);
    uExpMax = (*((unsigned int *) &f2) & _maskEXP);
    if (uExpMin > uExpMax)
        GXISWAP(uExpMin, uExpMax);
    if (uExpMin == 0)
        uExpMax = _nEXPBIAS << _nMANTBITS;

    uExpOffset = (_nMANTBITS - nToleranceBits) << _nMANTBITS;

    if (uExpMax < uExpOffset)
        fEps = fTINY_EPS;
    else {
        uExpMax -= uExpOffset;
        fEps = *((float *) &uExpMax);
    }
    return (((f1 + fEps) >= f2) && ((f1 - fEps) <= f2));
}

#undef fTINY_EPS


//
//------------------------------------------------------------------------------
// Function:    GXForceSinglePrecision
// Purpose:     Force the processor to perform floating point operations at
//              floating point single precision
//------------------------------------------------------------------------------
//
inline unsigned short
GXForceSinglePrecision()
{
#ifdef _X86_
    unsigned short wSave, wTemp;
    _asm 
    {
        fstcw wSave
        mov ax,wSave
        and ax,not 0x300 ; round to single
        and ax,not 31 ; enable exceptions (except inexact result)
        or ax, 0x10
        mov wTemp,ax
        fldcw wTemp
    }
    return wSave;
#else
    return 0;
#endif
}


#endif // #ifndef __GXFMATH_H

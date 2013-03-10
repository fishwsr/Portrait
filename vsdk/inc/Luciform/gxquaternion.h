#pragma once

#ifndef __GXQUATERNION_H
#define __GXQUATERNION_H

/*==========================================================================;
//
//  Copyright (C) 2000 Microsoft Corporation.  All Rights Reserved.
//
//  File:       GXQuaternion.h
//  Content:    GXQuaternion rotation vector type
//
//
//**************************************************************************/

#include "gxaffine3.h"

class GXQuaternion {
public:
                        GXQuaternion() {}
                        GXQuaternion(const GXVector3& , float);

    // concatenate operators
    friend GXQuaternion operator *(const GXQuaternion& , const GXQuaternion& );
    GXQuaternion&       operator *=(const GXQuaternion& );

    // equality operators
    friend bool         operator ==(const GXQuaternion& , const GXQuaternion& );
    friend bool         operator !=(const GXQuaternion& , const GXQuaternion& );

    // common operations
    friend GXQuaternion GXNormalize(const GXQuaternion& );
    friend GXQuaternion GXConjugate(const GXQuaternion& );
    friend GXQuaternion GXInverse(const GXQuaternion& );
    friend GXVector3    GXRotate(const GXQuaternion& q, const GXVector3& v);
    friend GXQuaternion GXIdentityQuaternion();

    // Note: the following algebraic operators and functions are generally 
    //       only used in defining quaternion interpolation schemes other 
    //       than slerp.

    // assignment operators
    GXQuaternion&       operator +=(const GXQuaternion& );
    GXQuaternion&       operator -=(const GXQuaternion& );
    GXQuaternion&       operator *=(float);
    GXQuaternion&       operator /=(float);

    // unary operators
    GXQuaternion        operator +() const;
    GXQuaternion        operator -() const;

    //NOTE: solid-body rotations are combined with *, not +
    friend GXQuaternion operator +(const GXQuaternion& , const GXQuaternion& );
    friend GXQuaternion operator -(const GXQuaternion& , const GXQuaternion& );
    friend GXQuaternion operator *(const GXQuaternion& , float);
    friend GXQuaternion operator *(float, const GXQuaternion& );
    friend GXQuaternion operator /(const GXQuaternion& , float);

    // misc functions
    friend float        GXDot(const GXQuaternion &q1, const GXQuaternion &q2);
    friend float        GXLengthSq(const GXQuaternion& );
    friend float        GXLength(const GXQuaternion& );

public:
    GXVector3   v;
    float       w;
};

#ifdef _GXDEFINE_DLL_ENTRYPOINTS
extern "C"
{
    //
    // NOTE: - angles are counterclockwise (positive axis out face of clock)
    //       - Euler angles follow the aeronautical "zyx" convention -
    //         Roll first, about axis the object is facing, then
    //         pitch upward, then yaw to face into the new heading
    //
    HRESULT WINAPI GXQuaternionFromEulerAngles( GXQuaternion *pqOut, float xRoll, float yPitch, float zYaw );
    HRESULT WINAPI GXQuaternionFromAxisAngle( GXQuaternion *pqOut, GXVector3 *pvAxis, float fAngle );
    HRESULT WINAPI GXQuaternionSlerp( GXQuaternion *pqOut, GXQuaternion *pqStart, GXQuaternion *pqStop, float t );
    HRESULT WINAPI GXMatrix3RotationQuaternion( GXMatrix3 *pmatOut, GXMatrix3 *pmatInvOut, GXQuaternion *pq );
}
#endif  // #ifdef _GXDEFINE_DLL_ENTRYPOINTS


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// GXQuaternion Inline Functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// -------------------------------------
// constructors
// -------------------------------------

inline
GXQuaternion::GXQuaternion(const GXVector3& _v, float _w)
: v(_v), w(_w)
{
}

// -------------------------------------
// concatenate operators
// -------------------------------------

inline GXQuaternion
operator *(const GXQuaternion &q1, const GXQuaternion &q2)
{
    //
    //	Cross(q1.v, q2.v) + q1.w*q2.v + q2.w*q1.v, q1.w*q2.w - Dot(q1.v, q2.v)
    //
    float A = (q1.w    + q1.v.dx) * (q2.w    + q2.v.dx);
    float B = (q1.v.dz - q1.v.dy) * (q2.v.dy - q2.v.dz);
    float C = (q1.v.dx - q1.w   ) * (q2.v.dy + q2.v.dz);
    float D = (q1.v.dy + q1.v.dz) * (q2.v.dx - q2.w   );
    float E = (q1.v.dx + q1.v.dz) * (q2.v.dx + q2.v.dy);
    float F = (q1.v.dx - q1.v.dz) * (q2.v.dx - q2.v.dy);
    float G = (q1.w    + q1.v.dy) * (q2.w    - q2.v.dz);
    float H = (q1.w    - q1.v.dy) * (q2.w    + q2.v.dz);
    return GXQuaternion(GXVector3( A - 0.5f*(E + F + G + H),
                     -C + 0.5f*(E - F + G - H),
                     -D + 0.5f*(E - F - G + H)),
                      B + 0.5f*(-E - F + G + H));
}

inline GXQuaternion&
GXQuaternion::operator *= ( const GXQuaternion& q )
{
    *this = *this * q;
    return *this;
}


// -------------------------------------
// equality operators
// -------------------------------------

inline bool
operator ==(const GXQuaternion& q1, const GXQuaternion& q2)
{
    return q1.v == q2.v && q1.w == q2.w;
}

inline bool
operator !=(const GXQuaternion& q1, const GXQuaternion& q2)
{
    return q1.v != q2.v || q1.w != q2.w;
}


// -------------------------------------
// assignment operators
// -------------------------------------

inline GXQuaternion&
GXQuaternion::operator +=(const GXQuaternion& q)
{
    v += q.v;
    w += q.w;
    return *this;
}

inline GXQuaternion&
GXQuaternion::operator -=(const GXQuaternion& q)
{
    v -= q.v;
    w -= q.w;
    return *this;
}

inline GXQuaternion&
GXQuaternion::operator *=(float f)
{
    v *= f;
    w *= f;
    return *this;
}

inline GXQuaternion&
GXQuaternion::operator /=(float f)
{
    v /= f;
    w /= f;
    return *this;
}

// -------------------------------------
// unary operators
// -------------------------------------

inline GXQuaternion
GXQuaternion::operator -() const
{
    return GXQuaternion(-v, -w);  // note: this is not the conjugate
}

inline GXQuaternion
GXQuaternion::operator +() const
{
    return *this;
}

// -------------------------------------
// binary operators
// -------------------------------------

inline GXQuaternion
operator +(const GXQuaternion& q1, const GXQuaternion& q2)
{
    return GXQuaternion(q1.v + q2.v, q1.w + q2.w);
}

inline GXQuaternion
operator -(const GXQuaternion& q1, const GXQuaternion& q2)
{
    return GXQuaternion(q1.v - q2.v, q1.w - q2.w);
}


inline GXQuaternion
operator *(const GXQuaternion& q, float f)
{
    return GXQuaternion(q.v * f, q.w * f);
}

inline GXQuaternion
operator *(float f, const GXQuaternion& q)
{
    return GXQuaternion(f * q.v, f* q.w);
}

inline GXQuaternion
operator /(const GXQuaternion& q, float f)
{
    return GXQuaternion(q.v / f, q.w / f);
}

// -------------------------------------
// misc functions
// -------------------------------------
#include "gxfmath.h"

inline float
GXDot(const GXQuaternion &q1, const GXQuaternion &q2)
{
    return GXDot(q1.v, q2.v) + q1.w * q2.w;
}

inline float
GXLengthSq(const GXQuaternion& q)
{
    return GXLengthSq(q.v) + q.w * q.w;
}

inline float
GXLength(const GXQuaternion& q)
{
    return GXSqrt(GXLengthSq(q));
}

inline GXQuaternion
GXNormalize(const GXQuaternion& q)
{
    return q * GXInvSqrt(GXLengthSq(q));
}

inline GXQuaternion
GXConjugate(const GXQuaternion& q)
{
    return GXQuaternion(-q.v, q.w);
}

inline GXQuaternion
GXInverse(const GXQuaternion& q)
{
    return GXConjugate(q)/GXLengthSq(q);    // LengthSq, not Length
}

inline GXQuaternion
GXIdentityQuaternion()
{
    return GXQuaternion(GXVector3(0.0f, 0.0f, 0.0f), 1.0f);
}

inline GXVector3
GXRotate(const GXQuaternion& q, const GXVector3& v)
{
    return (q * GXQuaternion(v, 0.0f) * GXInverse(q)).v;
}

#endif // #ifndef __GXQUATERNION_H


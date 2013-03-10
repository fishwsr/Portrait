#pragma once

#ifndef __GXAFFINE2_H
#define __GXAFFINE2_H

/*==========================================================================;
//
//  Copyright (C) 2000 Microsoft Corporation.  All Rights Reserved.
//
//  File:       GXAffine2.h
//  Content:    2D affine - space classes
//                  GXVector2TC   - 2D column vectors
//                  GXCoVector2TC - 2D row vectors (normals)
//                  GXPoint2TC    - 2D points
//
//
//**************************************************************************/

template <class Coord> class GXCoVector2TC;
template <class Coord> class GXPoint2TC;
template <class Coord> class GXMatrix2TC;

//-----------------------------------------------------------------------------
// Class: GXVector2TC
// Description: Column Vector
//-----------------------------------------------------------------------------
template<class Coord>
class GXVector2TC
{
public:
                                GXVector2TC() {}
                                GXVector2TC( const Coord& d );
                                GXVector2TC( const Coord& du, const Coord& dv );

    UINT                        Dim() const { return 2; }

    // access grants
    Coord&                      operator[] ( UINT i ) { return (&du)[i]; }
    const Coord&                operator[] ( UINT i ) const { return (&du)[i]; }

    // assignment operators
    GXVector2TC<Coord>&         operator += ( const GXVector2TC<Coord>& );
    GXVector2TC<Coord>&         operator -= ( const GXVector2TC<Coord>& );
    GXVector2TC<Coord>&         operator *= ( const Coord& );
    GXVector2TC<Coord>&         operator /= ( const Coord& );

    // unary operators
    GXVector2TC<Coord>          operator + () const;
    GXVector2TC<Coord>          operator - () const;

    // binary operators
    friend GXVector2TC<Coord>   operator + ( const GXVector2TC<Coord>&, const GXVector2TC<Coord>& );
    friend GXVector2TC<Coord>   operator - ( const GXVector2TC<Coord>&, const GXVector2TC<Coord>& );
    friend GXVector2TC<Coord>   operator * ( const Coord&, const GXVector2TC<Coord>& );
    friend GXVector2TC<Coord>   operator * ( const GXVector2TC<Coord>&, const Coord& );
    friend GXVector2TC<Coord>   operator / ( const GXVector2TC<Coord>&, const Coord& );
    friend bool                 operator == ( const GXVector2TC<Coord>&, const GXVector2TC<Coord>& );
    friend bool                 operator != ( const GXVector2TC<Coord>&, const GXVector2TC<Coord>& );

    // friend functions
    friend Coord                GXLengthSq( const GXVector2TC<Coord>& );
    friend Coord                GXLength( const GXVector2TC<Coord>& );
    friend GXVector2TC<Coord>   GXUnit( const GXVector2TC<Coord>& );
    friend Coord                GXMaximumNorm( const GXVector2TC<Coord>&);
    friend UINT                 GXMaxComponent( const GXVector2TC<Coord> &v);
    friend Coord                GXDot( const GXVector2TC<Coord>&, const GXVector2TC<Coord>& );
    friend Coord                GXDot( const GXVector2TC<Coord>&, const GXCoVector2TC<Coord>& );
    friend Coord                GXDot( const GXCoVector2TC<Coord>&, const GXVector2TC<Coord>& );
    friend Coord                GXCCW( const GXVector2TC<Coord>&, const GXVector2TC<Coord>& );

    friend GXMatrix2TC<Coord>   GXOuterProduct( const GXCoVector2TC<Coord>&, const GXCoVector2TC<Coord>& );
    friend GXMatrix2TC<Coord>   GXOuterProduct( const GXCoVector2TC<Coord>&, const GXVector2TC<Coord>& );
    friend GXMatrix2TC<Coord>   GXOuterProduct( const GXVector2TC<Coord>&, const GXCoVector2TC<Coord>& );
    friend GXMatrix2TC<Coord>   GXAlternatingProduct( const GXCoVector2TC<Coord>&, const GXCoVector2TC<Coord>& );
    friend GXMatrix2TC<Coord>   GXAlternatingProduct( const GXCoVector2TC<Coord>&, const GXVector2TC<Coord>& );
    friend GXMatrix2TC<Coord>   GXAlternatingProduct( const GXVector2TC<Coord>&, const GXCoVector2TC<Coord>& );

    // conversion operators - implicit conversions are inappropriate
    friend GXCoVector2TC<Coord> GXVecToCov( const GXVector2TC<Coord>& );
    friend GXVector2TC<Coord>   GXCovToVec( const GXCoVector2TC<Coord>& );
    friend GXVector2TC<Coord>   GXPntToVec( const GXPoint2TC<Coord>& );

    friend class GXPoint2TC<Coord>;

public:
    Coord du, dv;
};


//-----------------------------------------------------------------------------
// Class: GXCoVector2TC
// Description: Row Vector
//-----------------------------------------------------------------------------

template<class Coord>
class GXCoVector2TC
{
public:
                                GXCoVector2TC( ) {}
                                GXCoVector2TC( const Coord& n );
                                GXCoVector2TC( const Coord &nu, const Coord& nv );

    UINT                        Dim() const { return 2; }

    // access grants
    Coord&                      operator[] ( UINT i ) { return (&nu)[i]; }
    const Coord&                operator[] ( UINT i ) const { return (&nu)[i]; }

    // assignment operators
    GXCoVector2TC<Coord>&       operator += ( const GXCoVector2TC<Coord>& );
    GXCoVector2TC<Coord>&       operator -= ( const GXCoVector2TC<Coord>& );
    GXCoVector2TC<Coord>&       operator *= ( const Coord& );
    GXCoVector2TC<Coord>&       operator /= ( const Coord& );

    // unary operators
    GXCoVector2TC<Coord>        operator + () const;
    GXCoVector2TC<Coord>        operator - () const;

    // binary operators
    friend GXCoVector2TC<Coord> operator + ( const GXCoVector2TC<Coord>&, const GXCoVector2TC<Coord>& );
    friend GXCoVector2TC<Coord> operator - ( const GXCoVector2TC<Coord>&, const GXCoVector2TC<Coord>& );
    friend GXCoVector2TC<Coord> operator * ( const Coord&, const GXCoVector2TC<Coord>& );
    friend GXCoVector2TC<Coord> operator * ( const GXCoVector2TC<Coord>&, const Coord& );
    friend GXCoVector2TC<Coord> operator / ( const GXCoVector2TC<Coord>&, const Coord& );
    friend bool                 operator == ( const GXCoVector2TC<Coord>&, const GXCoVector2TC<Coord>& );
    friend bool                 operator != ( const GXCoVector2TC<Coord>&, const GXCoVector2TC<Coord>& );

    // friend functions
    friend Coord                GXLengthSq( const GXCoVector2TC<Coord>& );
    friend Coord                GXLength( const GXCoVector2TC<Coord>& );
    friend GXCoVector2TC<Coord> GXUnit( const GXCoVector2TC<Coord>& );
    friend Coord                GXMaximumNorm( const GXCoVector2TC<Coord>& , UINT *piComponent);   // L-infinity norm
    friend UINT                 GXMaxComponent( const GXCoVector2TC<Coord> &cv);
    friend Coord                GXDot( const GXCoVector2TC<Coord>&, const GXCoVector2TC<Coord>& );
    friend Coord                GXDot( const GXVector2TC<Coord>&, const GXCoVector2TC<Coord>& );
    friend Coord                GXDot( const GXCoVector2TC<Coord>&, const GXVector2TC<Coord>& );
    friend Coord                GXCCW( const GXCoVector2TC<Coord>&, const GXCoVector2TC<Coord>& );

    friend GXMatrix2TC<Coord>   GXOuterProduct( const GXCoVector2TC<Coord>&, const GXCoVector2TC<Coord>& );
    friend GXMatrix2TC<Coord>   GXOuterProduct( const GXCoVector2TC<Coord>&, const GXVector2TC<Coord>& );
    friend GXMatrix2TC<Coord>   GXOuterProduct( const GXVector2TC<Coord>&, const GXCoVector2TC<Coord>& );
    friend GXMatrix2TC<Coord>   GXAlternatingProduct( const GXCoVector2TC<Coord>&, const GXCoVector2TC<Coord>& );
    friend GXMatrix2TC<Coord>   GXAlternatingProduct( const GXCoVector2TC<Coord>&, const GXVector2TC<Coord>& );
    friend GXMatrix2TC<Coord>   GXAlternatingProduct( const GXVector2TC<Coord>&, const GXCoVector2TC<Coord>& );

    // conversion operators - implicit conversions are inappropriate
    friend GXCoVector2TC<Coord> GXVecToCov( const GXCoVector2TC<Coord>& );
    friend GXVector2TC<Coord>   GXCovToVec( const GXCoVector2TC<Coord>& );
    friend GXCoVector2TC<Coord> GXPntToCov( const GXPoint2TC<Coord>& );

    friend class GXPoint2TC<Coord>;

public:
    Coord nu, nv;
};


//-----------------------------------------------------------------------------
// Class: GXPoint2TC
//-----------------------------------------------------------------------------

template<class Coord>
class GXPoint2TC
{
public:
                                GXPoint2TC() { }
                                GXPoint2TC(Coord d);
                                GXPoint2TC(Coord u, Coord v);

    UINT                        Dim() const { return 2; }

    // access grants
    Coord&                      operator[] ( UINT i ) { return (&u)[i]; }
    const Coord&                operator[] ( UINT i ) const { return (&u)[i]; }

    // assignment operators
    GXPoint2TC<Coord>&          operator += ( const GXVector2TC<Coord>& );
    GXPoint2TC<Coord>&          operator -= ( const GXVector2TC<Coord>& );

    // binary operators
    friend GXPoint2TC<Coord>    operator + ( const GXPoint2TC<Coord>&, const GXVector2TC<Coord>& );
    friend GXPoint2TC<Coord>    operator + ( const GXVector2TC<Coord>&, const GXPoint2TC<Coord>& );
    friend GXPoint2TC<Coord>    operator - ( const GXPoint2TC<Coord>&, const GXVector2TC<Coord>& );
    friend GXVector2TC<Coord>   operator - ( const GXPoint2TC<Coord>&, const GXPoint2TC<Coord>& );
    friend bool                 operator == ( const GXPoint2TC<Coord>&, const GXPoint2TC<Coord>& );
    friend bool                 operator != ( const GXPoint2TC<Coord>&, const GXPoint2TC<Coord>& );

    // point dominance
    bool                        operator < ( const GXPoint2TC<Coord>& ) const;
    bool                        operator <= ( const GXPoint2TC<Coord>& ) const;
    bool                        operator > ( const GXPoint2TC<Coord>& ) const;
    bool                        operator >= ( const GXPoint2TC<Coord>& ) const;

    // friend functions
    friend GXPoint2TC<Coord>    GXLerp( const GXPoint2TC<Coord>&, const GXPoint2TC<Coord>&, const Coord& );
    friend GXPoint2TC<Coord>    GXBarycentric( const GXPoint2TC<Coord>& p, 
                                    const GXPoint2TC<Coord>& q, 
                                    const GXPoint2TC<Coord>& r,
                                    float f, float g);

    // conversion operators - implicit conversions are inappropriate
    friend GXVector2TC<Coord>   GXPntToVec( const GXPoint2TC<Coord>& );
    friend GXCoVector2TC<Coord> GXPntToCov( const GXPoint2TC<Coord>& );

public:
    Coord u, v;
};


//-----------------------------------------------------------------------------
// Class: GXMatrix2TC
//-----------------------------------------------------------------------------

template<class Coord>
class GXMatrix2TC
{
public:
                                GXMatrix2TC() {}
                                GXMatrix2TC( const Coord& s );
                                GXMatrix2TC( const GXVector2TC<Coord>& vCol1, const GXVector2TC<Coord>& vCol2 );
                                GXMatrix2TC( const GXCoVector2TC<Coord>& covRow1, const GXCoVector2TC<Coord>& covRow2 );

    // Return dimension of the type
    UINT                        Dim() const { return 2; }

    // access grants
    Coord&                      operator () ( UINT i, UINT j ) { return mat[i][j]; }
    const Coord&                operator () ( UINT i, UINT j ) const { return mat[i][j]; }
    Coord *                     operator [] ( int i ) { return mat[i]; }
    const Coord *               operator[] ( int i ) const { return mat[i]; }
    GXVector2TC<Coord>          GetColumn( int i ) const { return GXVector2TC<Coord>( mat[0][i], mat[1][i] ); }
    GXCoVector2TC<Coord>        GetRow( int i ) const { return GXCoVector2TC<Coord>( mat[i][0], mat[i][1] ); }
    void                        SetColumn( int i, const GXVector2TC<Coord>& v ) { mat[0][i] = v[0];  mat[1][i] = v[1]; }
    void                        SetRow( int i, const GXCoVector2TC<Coord>& cv ) { mat[i][0] = cv[0];  mat[i][1] = cv[1]; }

    // unary ops
    GXMatrix2TC<Coord>          operator + ( );
    GXMatrix2TC<Coord>          operator - ( );

    // binary operators
    friend GXMatrix2TC<Coord>   operator * ( const GXMatrix2TC<Coord>& m1, const GXMatrix2TC<Coord>& m2 );
    friend GXMatrix2TC<Coord>   operator + ( const GXMatrix2TC<Coord>&, const GXMatrix2TC<Coord>& );
    friend GXMatrix2TC<Coord>   operator - ( const GXMatrix2TC<Coord>&, const GXMatrix2TC<Coord>& );
    friend GXMatrix2TC<Coord>   operator * ( const GXMatrix2TC<Coord>&, float );
    friend GXMatrix2TC<Coord>   operator * ( float, const GXMatrix2TC<Coord>& );
    friend GXMatrix2TC<Coord>   operator / ( const GXMatrix2TC<Coord>&, float );
    friend bool                 operator == ( const GXMatrix2TC<Coord>&, const GXMatrix2TC<Coord>& );
    // see below for "operator *" with vectors, covectors, and points

    // assignment operators
    GXMatrix2TC<Coord>&         operator *= ( const GXMatrix2TC<Coord>& );
    GXMatrix2TC<Coord>&         operator += ( const GXMatrix2TC<Coord>& );
    GXMatrix2TC<Coord>&         operator -= ( const GXMatrix2TC<Coord>& );
    GXMatrix2TC<Coord>&         operator *= ( const Coord& );
    GXMatrix2TC<Coord>&         operator /= ( const Coord& );
    void                        SetToIdentity();
    bool                        IsIdentity() const;

    // misc functions
    friend GXMatrix2TC<Coord>   GXTranspose( const GXMatrix2TC<Coord>& mat );

public:
    Coord mat[2][2];
};

// matrix multiply with vectors, covectors, points
template<class mCoord, class vCoord> GXVector2TC<vCoord>     operator * ( const GXMatrix2TC<mCoord>& m, const GXVector2TC<vCoord>& v);
template<class mCoord, class cvCoord> GXCoVector2TC<cvCoord> operator * ( const GXCoVector2TC<cvCoord>& cv, const GXMatrix2TC<mCoord>& m );
template<class mCoord, class pCoord> GXPoint2TC<pCoord>      operator * ( const GXMatrix2TC<mCoord>& m, const GXPoint2TC<pCoord>& p );


//-----------------------------------------------------------------------------
// float-valued points, vectors, covectors
//-----------------------------------------------------------------------------
#include "gxfmath.h"
typedef GXPoint2TC<float> GXPoint2;
typedef GXVector2TC<float> GXVector2;
typedef GXCoVector2TC<float> GXCoVector2;
typedef GXMatrix2TC<float> GXMatrix2;


#ifdef _GXDEFINE_DLL_ENTRYPOINTS
extern "C"
{
    HRESULT WINAPI GXMatrix2Inverse( GXMatrix2 *pmatOut, GXMatrix2 *pmat );
    HRESULT WINAPI GXMatrix2Rotation( GXMatrix2 *pmatOut, GXMatrix2 *pmatInvOut, float fAngle );
    HRESULT WINAPI GXMatrix2Scale( GXMatrix2 *pmatOut, GXMatrix2 *pmatInvOut, float fScaleU, float fScaleV );
}
#endif  // #ifdef _GXDEFINE_DLL_ENTRYPOINTS


//-----------------------------------------------------------------------------
// Class: GXRay2
//-----------------------------------------------------------------------------
class GXRay2
{
public:
                                GXRay2() { }
                                GXRay2( const GXPoint2& _pntOrg, const GXVector2& _vDir )
                                    { pntOrg = _pntOrg; vDir = _vDir; }
//    GXPoint2                    Evaluate(float t) const { return this->pntOrg + t * this->vDir; }
public:
    GXPoint2    pntOrg;
    GXVector2   vDir;
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/// GXVector2TC Inline Functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// -------------------------------------
// constructors
// -------------------------------------

template<class Coord>
inline
GXVector2TC<Coord>::GXVector2TC( const Coord& _d )
{
    du = dv = _d;
}

template<class Coord>
inline
GXVector2TC<Coord>::GXVector2TC( const Coord& _du, const Coord& _dv )
{
    du = _du; dv = _dv;
}

// -------------------------------------
// assignment operators
// -------------------------------------

template<class Coord>
inline GXVector2TC<Coord>&
GXVector2TC<Coord>::operator += ( const GXVector2TC<Coord>& v )
{
    du += v.du;
    dv += v.dv;
    return *this;
}

template<class Coord>
inline GXVector2TC<Coord>&
GXVector2TC<Coord>::operator -= ( const GXVector2TC<Coord>& v )
{
    du -= v.du;
    dv -= v.dv;
    return *this;
}

template<class Coord>
inline GXVector2TC<Coord>&
GXVector2TC<Coord>::operator *= ( const Coord& s )
{
    du *= s;
    dv *= s;
    return *this;
}

template<class Coord>
inline GXVector2TC<Coord>&
GXVector2TC<Coord>::operator /= ( const Coord& s )
{
    du /= s;
    dv /= s;
    return *this;
}

// -------------------------------------
// unary operators
// -------------------------------------

template<class Coord>
inline GXVector2TC<Coord>
GXVector2TC<Coord>::operator + () const
{
    return *this;
}

template<class Coord>
inline GXVector2TC<Coord>
GXVector2TC<Coord>::operator - () const
{
    return GXVector2TC<Coord>( -du, -dv );
}

// -------------------------------------
// binary operators
// -------------------------------------

template<class Coord>
inline GXVector2TC<Coord>
operator + ( const GXVector2TC<Coord>& v1, const GXVector2TC<Coord>& v2 )
{
    return GXVector2TC<Coord>( v1.du + v2.du, v1.dv + v2.dv );
}

template<class Coord>
inline GXVector2TC<Coord>
operator - ( const GXVector2TC<Coord>& v1, const GXVector2TC<Coord>& v2 )
{
    return GXVector2TC<Coord>( v1.du - v2.du, v1.dv - v2.dv );
}

template<class Coord>
inline GXVector2TC<Coord>
operator * ( const Coord& s, const GXVector2TC<Coord>& v )
{
    return GXVector2TC<Coord>( v.du * s, v.dv * s );
}

template<class Coord>
inline GXVector2TC<Coord>
operator * ( const GXVector2TC<Coord>& v, const Coord& s )
{
    return GXVector2TC<Coord>( v.du * s, v.dv * s );
}

template<class Coord>
inline GXVector2TC<Coord>
operator / ( const GXVector2TC<Coord>& v, const Coord& s )
{
    return GXVector2TC<Coord>( v.du / s, v.dv / s );
}

template<class Coord>
inline bool
operator==(const GXVector2TC<Coord>& v1, const GXVector2TC<Coord>& v2)
{
    return v1.du == v2.du && v1.dv == v2.dv;
}

template<class Coord>
inline bool
operator != ( const GXVector2TC<Coord>& v1, const GXVector2TC<Coord>& v2 )
{
    return ! (v1 == v2);
}

// -------------------------------------
// friend functions
// -------------------------------------

template<class Coord>
inline Coord
GXLengthSq( const GXVector2TC<Coord>& v)
{
    return v.du*v.du + v.dv * v.dv;
}

template<class Coord>
inline Coord
GXLength( const GXVector2TC<Coord>& v)
{
    return (Coord) GXSqrt(GXLengthSq(v));
}

template<class Coord>
inline GXVector2TC<Coord>
GXUnit( const GXVector2TC<Coord>& v)
{
    return v * GXInvSqrt(GXLengthSq(v));
}

template<class Coord>
inline UINT
GXMaxComponent(const GXVector2TC<Coord> &v)
{
    UINT i, iMax = 0;
    for (i = 1; i < v.Dim(); i++)
        if (GXFAbs(v[i]) > GXFAbs(v[iMax]))
            iMax = i;
    return iMax;
}

template<class Coord>
inline Coord
GXMaximumNorm(const GXVector2TC<Coord> &v)
{
    return GXFAbs(v[GXMaxComponent(v)]);
}

template<class Coord>
inline Coord
GXDot( const GXVector2TC<Coord>& v1, const GXVector2TC<Coord>& v2 )
{
    return v1.du * v2.du + v1.dv * v2.dv;
}

template<class Coord>
inline Coord
GXDot( const GXVector2TC<Coord>& v1, const GXCoVector2TC<Coord>& v2 )
{
    return v1.du * v2.nu + v1.dv * v2.nv;
}

template<class Coord>
inline Coord
GXDot( const GXCoVector2TC<Coord>& v1, const GXVector2TC<Coord>& v2 )
{
    return v1.nu * v2.du + v1.nv * v2.dv;
}

template<class Coord>
inline Coord
GXCCW( const GXVector2TC<Coord>& v1, const GXVector2TC<Coord>& v2 )
{
    return v1.du * v2.dv - v1.dv * v2.du;
}

template<class Coord>
inline GXMatrix2TC<Coord>
GXOuterProduct( const GXVector2TC<Coord>& v1, const GXVector2TC<Coord>& v2 )
{
    GXMatrix2TC<Coord> ret;
    for (UINT i = 0; i < 2; i++)
        for (UINT j = 0; j < 2; j++)
            ret(i,j)=v1[i]*v2[j];
    return ret;
}

template<class Coord>
inline GXMatrix2TC<Coord>
GXOuterProduct( const GXVector2TC<Coord>& v1, const GXCoVector2TC<Coord>& v2 )
{
    GXMatrix2TC<Coord> ret;
    for (UINT i = 0; i < 2; i++)
        for (UINT j = 0; j < 2; j++)
            ret(i,j)=v1[i]*v2[j];
    return ret;
}

template<class Coord>
inline GXMatrix2TC<Coord>
GXOuterProduct( const GXCoVector2TC<Coord>& v1, const GXVector2TC<Coord>& v2 )
{
    GXMatrix2TC<Coord> ret;
    for (UINT i = 0; i < 2; i++)
        for (UINT j = 0; j < 2; j++)
            ret(i,j)=v1[i]*v2[j];
    return ret;
}

template<class Coord>
inline GXMatrix2TC<Coord>
GXOuterProduct( const GXCoVector2TC<Coord>& v1, const GXCoVector2TC<Coord>& v2 )
{
    GXMatrix2TC<Coord> ret;
    for (UINT i = 0; i < 2; i++)
        for (UINT j = 0; j < 2; j++)
            ret(i,j)=v1[i]*v2[j];
    return ret;
}


template<class Coord>
inline GXMatrix2TC<Coord>
GXAlternatingProduct( const GXVector2TC<Coord>& v1, const GXVector2TC<Coord>& v2 )
{
    GXMatrix2TC<Coord> ret;

    ret(0,0) = ret(1,1) = 0.0f;
    ret(1,0) = v1.dv*v2.du-v1.du*v2.dv;
    ret(0,1) = -ret(1,0);

    return ret;
}

template<class Coord>
inline GXMatrix2TC<Coord>
GXAlternatingProduct( const GXVector2TC<Coord>& v1, const GXCoVector2TC<Coord>& v2 )
{
    GXMatrix2TC<Coord> ret;

    ret(0,0) = ret(1,1) = 0.0f;
    ret(1,0) = v1.dv*v2.nu-v1.du*v2.nv;
    ret(0,1) = -ret(1,0);

    return ret;
}

template<class Coord>
inline GXMatrix2TC<Coord>
GXAlternatingProduct( const GXCoVector2TC<Coord>& v1, const GXVector2TC<Coord>& v2 )
{
    GXMatrix2TC<Coord> ret;

    ret(0,0) = ret(1,1) = 0.0f;
    ret(1,0) = v1.nv*v2.du-v1.nu*v2.dv;
    ret(0,1) = -ret(1,0);

    return ret;
}

template<class Coord>
inline GXMatrix2TC<Coord>
GXAlternatingProduct( const GXCoVector2TC<Coord>& v1, const GXCoVector2TC<Coord>& v2 )
{
    GXMatrix2TC<Coord> ret;

    ret(0,0) = ret(1,1) = 0.0f;
    ret(1,0) = v1.nv*v2.nu-v1.nu*v2.nv;
    ret(0,1) = -ret(1,0);

    return ret;
}

template<class Coord>
inline GXCoVector2TC<Coord>
GXVecToCov( const GXVector2TC<Coord>& v )
{
    return GXCoVector2TC<Coord>( v.du, v.dv );
}

template<class Coord>
inline GXVector2TC<Coord>
GXCovToVec( const GXCoVector2TC<Coord>& cv )
{
    return GXVector2TC<Coord>( cv.nu, cv.nv );
}

template<class Coord>
inline GXVector2TC<Coord>
GXPntToVec( const GXPoint2TC<Coord>& pnt )
{
    return GXVector2TC<Coord>( pnt.u, pnt.v );
}

template<class Coord>
inline GXCoVector2TC<Coord>
GXPntToCov( const GXPoint2TC<Coord>& pnt )
{
    return GXCoVector2TC<Coord>( pnt.u, pnt.v );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/// GXCoVector2TC Inline Functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// -------------------------------------
// constructors
// -------------------------------------

template<class Coord>
inline
GXCoVector2TC<Coord>::GXCoVector2TC( const Coord& _n )
{
    nu = nv = _n;
}

template<class Coord>
inline
GXCoVector2TC<Coord>::GXCoVector2TC( const Coord& _nu, const Coord& _nv )
{
    nu = _nu; nv = _nv;
}

// -------------------------------------
// assignment operators
// -------------------------------------

template<class Coord>
inline GXCoVector2TC<Coord>&
GXCoVector2TC<Coord>::operator += ( const GXCoVector2TC<Coord>& v )
{
    nu += v.nu;
    nv += v.nv;
    return *this;
}

template<class Coord>
inline GXCoVector2TC<Coord>&
GXCoVector2TC<Coord>::operator -= ( const GXCoVector2TC<Coord>& v )
{
    nu -= v.nu;
    nv -= v.nv;
    return *this;
}

template<class Coord>
inline GXCoVector2TC<Coord>&
GXCoVector2TC<Coord>::operator *= ( const Coord& s )
{
    nu *= s;
    nv *= s;
    return *this;
}

template<class Coord>
inline GXCoVector2TC<Coord>&
GXCoVector2TC<Coord>::operator /= ( const Coord& s )
{
    nu /= s;
    nv /= s;
    return *this;
}

// -------------------------------------
// unary operators
// -------------------------------------

template<class Coord>
inline GXCoVector2TC<Coord>
GXCoVector2TC<Coord>::operator + () const
{
    return *this;
}

template<class Coord>
inline GXCoVector2TC<Coord>
GXCoVector2TC<Coord>::operator - () const
{
    return GXCoVector2TC<Coord>( -nu, -nv );
}

// -------------------------------------
// binary operators
// -------------------------------------

template<class Coord>
inline GXCoVector2TC<Coord>
operator + ( const GXCoVector2TC<Coord>& v1, const GXCoVector2TC<Coord>& v2 )
{
    return GXCoVector2TC<Coord>( v1.nu + v2.nu, v1.nv + v2.nv );
}

template<class Coord>
inline GXCoVector2TC<Coord>
operator - ( const GXCoVector2TC<Coord>& v1, const GXCoVector2TC<Coord>& v2 )
{
    return GXCoVector2TC<Coord>( v1.nu - v2.nu, v1.nv - v2.nv );
}

template<class Coord>
inline GXCoVector2TC<Coord>
operator * ( const Coord& s, const GXCoVector2TC<Coord>& v )
{
    return GXCoVector2TC<Coord>( v.nu * s, v.nv * s );
}

template<class Coord>
inline GXCoVector2TC<Coord>
operator * ( const GXCoVector2TC<Coord>& v, const Coord& s )
{
    return GXCoVector2TC<Coord>( v.nu * s, v.nv * s );
}

template<class Coord>
inline GXCoVector2TC<Coord>
operator / ( const GXCoVector2TC<Coord>& v, const Coord& s )
{
    return GXCoVector2TC<Coord>(v.nu / s, v.nv / s);
}

template<class Coord>
inline bool
operator == ( const GXCoVector2TC<Coord>& v1, const GXCoVector2TC<Coord>& v2 )
{
    return v1.nu == v2.nu&& v1.nv == v2.nv;
}

template<class Coord>
inline bool
operator != ( const GXCoVector2TC<Coord>& v1, const GXCoVector2TC<Coord>& v2 )
{
    return ! (v1 == v2);
}

// -------------------------------------
// friend functions
// -------------------------------------

template<class Coord>
inline Coord
GXLengthSq( const GXCoVector2TC<Coord>& v)
{
    return v.nu*v.nu + v.nv * v.nv;
}

template<class Coord>
inline Coord
GXLength( const GXCoVector2TC<Coord>& v)
{
    return (Coord) GXSqrt(GXLengthSq(v));
}

template<class Coord>
inline GXCoVector2TC<Coord>
GXUnit( const GXCoVector2TC<Coord>& v)
{
    return v * GXInvSqrt(GXLengthSq(v));
}

template<class Coord>
inline UINT
GXMaxComponent(const GXCoVector2TC<Coord> &v)
{
    UINT i, iMax = 0;
    for (i = 1; i < v.Dim(); i++)
        if (GXFAbs(v[i]) > GXFAbs(v[iMax]))
            iMax = i;
    return iMax;
}

template<class Coord>
inline Coord
GXMaximumNorm(const GXCoVector2TC<Coord> &v)
{
    return GXFAbs(v[GXMaxComponent(v)]);
}

template<class Coord>
inline Coord
GXDot( const GXCoVector2TC<Coord>& v1, const GXCoVector2TC<Coord>& v2 )
{
    return v1.nu * v2.nu + v1.nv * v2.nv;
}

template<class Coord>
inline Coord
GXCCW( const GXCoVector2TC<Coord>& v1, const GXCoVector2TC<Coord>& v2 )
{
    return v1.nu * v2.nv - v1.nv * v2.nu;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/// GXPoint2TC Inline Functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// -------------------------------------
// constructors
// -------------------------------------

template<class Coord>
inline
GXPoint2TC<Coord>::GXPoint2TC( Coord _x )
{
    u = v = _x;
}

template<class Coord>
inline
GXPoint2TC<Coord>::GXPoint2TC ( Coord _u, Coord _v )
{
    u = _u; v = _v;
}

// -------------------------------------
// constructors
// -------------------------------------

template<class Coord>
inline GXPoint2TC<Coord>&
GXPoint2TC<Coord>::operator += (const GXVector2TC<Coord>& vDelta )
{
    u += vDelta.du;
    v += vDelta.dv;
    return *this;
}

template<class Coord>
inline GXPoint2TC<Coord>&
GXPoint2TC<Coord>::operator -= (const GXVector2TC<Coord>& vDelta )
{
    u -= vDelta.du;
    v -= vDelta.dv;
    return *this;
}

// -------------------------------------
// binary operators
// -------------------------------------

template<class Coord>
inline GXPoint2TC<Coord>
operator + ( const GXPoint2TC<Coord>& p, const GXVector2TC<Coord>& vDelta )
{
    return GXPoint2TC<Coord>( p.u + vDelta.du, p.v + vDelta.dv );
}

template<class Coord>
inline GXPoint2TC<Coord>
operator + ( const GXVector2TC<Coord>& vDelta, const GXPoint2TC<Coord>& p )
{
    return GXPoint2TC<Coord>( p.u + vDelta.du, p.v + vDelta.dv );
}

template<class Coord>
inline GXPoint2TC<Coord>
operator - ( const GXPoint2TC<Coord>& p, const GXVector2TC<Coord>& vDelta )
{
    return GXPoint2TC<Coord>( p.u - vDelta.du, p.v - vDelta.dv );
}

template<class Coord>
inline GXVector2TC<Coord>
operator - ( const GXPoint2TC<Coord>& p1, const GXPoint2TC<Coord>& p2)
{
    return GXVector2TC<Coord>( p1.u - p2.u, p1.v - p2.v );
}

template<class Coord>
inline bool
operator == ( const GXPoint2TC<Coord>& p1, const GXPoint2TC<Coord>& p2 )
{
    return p1.u == p2.u&& p1.v == p2.v;
}

template<class Coord>
inline bool
operator != ( const GXPoint2TC<Coord>& v1, const GXPoint2TC<Coord>& v2 )
{
    return ! (v1 == v2);
}

// -------------------------------------
// point dominance
// -------------------------------------

template<class Coord>
inline bool
GXPoint2TC<Coord>::operator < ( const GXPoint2TC<Coord>& p ) const
{
    return u < p.u && v < p.v;
}

template<class Coord>
inline bool
GXPoint2TC<Coord>::operator<= ( const GXPoint2TC<Coord>& p ) const
{
    return u <= p.u && v <= p.v;
}

template<class Coord>
inline bool
GXPoint2TC<Coord>::operator > ( const GXPoint2TC<Coord>& p ) const
{
    return u > p.u && v > p.v;
}

template<class Coord>
inline bool
GXPoint2TC<Coord>::operator >= ( const GXPoint2TC<Coord>& p ) const
{
    return u >= p.u && v >= p.v;
}

// -------------------------------------
// friend functions
// -------------------------------------

template<class Coord>
inline GXPoint2TC<Coord>
GXLerp( const GXPoint2TC<Coord>&p1, const GXPoint2TC<Coord>&p2, const Coord& s )
{
    GXVector2TC<Coord> p1p2 = p2 - p1;
    return GXPoint2TC<Coord>( p1 + s*p1p2 );
}

template<class Coord>
inline GXPoint2TC<Coord>
GXBarycentric( const GXPoint2TC<Coord>& p, 
             const GXPoint2TC<Coord>& q, 
             const GXPoint2TC<Coord>& r,
             float f, float g)
{
    return p+f*(q-p)+g*(r-p);
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// GXMatrix2TC Inline Functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// -------------------------------------
// constructors
// -------------------------------------

template<class Coord>
inline
GXMatrix2TC<Coord>::GXMatrix2TC( const Coord& s )
{
    mat[0][0] = s; mat[0][1] = s;
    mat[1][0] = s; mat[1][1] = s;
}

template<class Coord>
inline
GXMatrix2TC<Coord>::GXMatrix2TC( const GXVector2TC<Coord>& vCol1, const GXVector2TC<Coord>& vCol2 )
{
    mat[0][0] = vCol1.du; mat[0][1] = vCol2.du;
    mat[1][0] = vCol1.dv; mat[1][1] = vCol2.dv;
}

template<class Coord>
inline
GXMatrix2TC<Coord>::GXMatrix2TC( const GXCoVector2TC<Coord>& covRow1, const GXCoVector2TC<Coord>& covRow2 )
{
    mat[0][0] = covRow1.nu; mat[0][1] = covRow1.nv;
    mat[1][0] = covRow2.nu; mat[1][1] = covRow2.nv;
}

// -------------------------------------
// unary operators
// -------------------------------------

template<class Coord>
inline GXMatrix2TC<Coord>
GXMatrix2TC<Coord>::operator + ( )
{
    return *this;
}

template<class Coord>
inline GXMatrix2TC<Coord>
GXMatrix2TC<Coord>::operator - ( )
{
    GXMatrix2TC<Coord> ret;
    
    for (UINT i = 0; i < 2; i += 1)
        for (UINT j = 0; j < 2; j += 1)
            ret(i, j) = -mat[i][j];
    return ret;
}


// -------------------------------------
// binary operators
// -------------------------------------

template<class Coord>
inline GXMatrix2TC<Coord>
operator + ( const GXMatrix2TC<Coord>& m1, const GXMatrix2TC<Coord>& m2 )
{
    GXMatrix2TC<Coord> ret;

    for (UINT i = 0; i < 2; i += 1)
        for (UINT j = 0; j < 2; j += 1)
            ret(i, j) = m1(i, j) + m2(i, j);
    return ret;
}

template<class Coord>
inline GXMatrix2TC<Coord>
operator - ( const GXMatrix2TC<Coord>& m1, const GXMatrix2TC<Coord>& m2 )
{
    GXMatrix2TC<Coord> ret;

    for (UINT i = 0; i < 2; i += 1)
        for (UINT j = 0; j < 2; j += 1)
            ret(i, j) = m1(i, j) - m2(i, j);
    return ret;
}

template<class Coord>
inline GXMatrix2TC<Coord>
operator * ( const GXMatrix2TC<Coord>& m, float s )
{
    GXMatrix2TC<Coord> ret;

    for (UINT i = 0; i < 2; i += 1)
        for (UINT j = 0; j < 2; j += 1)
            ret(i, j) = m(i, j) * s;
    return ret;
}

template<class Coord>
inline GXMatrix2TC<Coord>
operator * ( float s, const GXMatrix2TC<Coord>& m )
{
    GXMatrix2TC<Coord> ret;

    for (UINT i = 0; i < 2; i += 1)
        for (UINT j = 0; j < 2; j += 1)
            ret(i, j) = m(i, j) * s;
    return ret;
}

template<class Coord>
inline GXMatrix2TC<Coord> 
operator * ( const GXMatrix2TC<Coord>& m1, const GXMatrix2TC<Coord>& m2 )
{
    GXMatrix2TC<Coord> ret;
    for (int i=0; i<2; i++) {
        for (int j=0; j<2; j++) {
            ret(i, j) = Coord(0.0);
            for (int k=0; k<2; k++) {
                ret(i, j) += m1(i, k) * m2(k, j);
            }
        }
    }
    return ret;
}

template<class Coord>
inline GXMatrix2TC<Coord>&
GXMatrix2TC<Coord>::operator *= ( const GXMatrix2TC<Coord>& m )
{
    *this = *this * m;
    return *this;
}


template<class Coord>
inline GXMatrix2TC<Coord>
operator / ( const GXMatrix2TC<Coord>& m, float s )
{
    GXMatrix2TC<Coord> ret;
    Coord s1 = Coord(1.0f) / s;

    for (UINT i = 0; i < 2; i += 1)
        for (UINT j = 0; j < 2; j += 1)
            ret(i, j) = m(i, j) * s1;
    return ret;
}

template<class Coord>
inline bool
operator == ( const GXMatrix2TC<Coord>& m1, const GXMatrix2TC<Coord>& m2)
{
    for (UINT i = 0; i < 2; i += 1)
        for (UINT j = 0; j < 2; j += 1)
            if ( m1(i,j) != m2(i,j) )
                return false;
    return true;
}

template<class mCoord, class vCoord>
inline GXVector2TC<vCoord>
operator * ( const GXMatrix2TC<mCoord> &m, const GXVector2TC<vCoord> &vec )
{
    return GXVector2TC<vCoord>( m.mat[0][0]*vec.du + m.mat[0][1]*vec.dv,
                                m.mat[1][0]*vec.du + m.mat[1][1]*vec.dv );
}

template<class mCoord, class cvCoord>
inline GXCoVector2TC<cvCoord>
operator * ( const GXCoVector2TC<cvCoord> &cov, const GXMatrix2TC<mCoord>& m )
{
    return GXCoVector2TC<cvCoord>(cov.nu*m.mat[0][0] + cov.nv*m.mat[1][0],
                                  cov.nu*m.mat[0][1] + cov.nv*m.mat[1][1]);
}

template<class mCoord, class pCoord>
inline GXPoint2TC<pCoord>
operator * ( const GXMatrix2TC<mCoord> &m, const GXPoint2TC<pCoord> &p )
{
    return GXPoint2TC<pCoord>( m.mat[0][0]*p.u + m.mat[0][1]*p.v,
                               m.mat[1][0]*p.u + m.mat[1][1]*p.v );
}

// -------------------------------------
// assignment operators
// -------------------------------------

template<class Coord>
inline GXMatrix2TC<Coord>&
GXMatrix2TC<Coord>::operator += ( const GXMatrix2TC<Coord>& m )
{
    for (UINT i = 0; i < 2; i += 1)
        for (UINT j = 0; j < 2; j += 1)
            mat[i][j] += m(i, j);
    return *this;
}

template<class Coord>
inline GXMatrix2TC<Coord>&
GXMatrix2TC<Coord>::operator -= ( const GXMatrix2TC<Coord>& m )
{
    for (UINT i = 0; i < 2; i += 1)
        for (UINT j = 0; j < 2; j += 1)
            mat[i][j] -= m(i, j);
    return *this;
}

template<class Coord>
inline GXMatrix2TC<Coord>&
GXMatrix2TC<Coord>::operator *= ( const Coord& s )
{
    for (UINT i = 0; i < 2; i += 1)
        for (UINT j = 0; j < 2; j += 1)
            mat[i][j] *= s;
    return *this;
}

template<class Coord>
inline GXMatrix2TC<Coord>&
GXMatrix2TC<Coord>::operator /= ( const Coord& s )
{
    Coord s1 = Coord(1.0f) / s;
    for (UINT i = 0; i < 2; i += 1)
        for (UINT j = 0; j < 2; j += 1)
            mat[i][j] *= s1;
    return *this;
}

// -------------------------------------
// misc functions
// -------------------------------------

template<class Coord>
inline void
GXMatrix2TC<Coord>::SetToIdentity()
{
    for (int i = 0; i < 2; i += 1)
        for (int j = 0; j < 2; j += 1)
            mat[i][j] = ((i==j) ? Coord(1.0) : Coord(0.0));
}

template<class Coord>
inline bool
GXMatrix2TC<Coord>::IsIdentity() const
{
    for (int i = 0; i < 2; i += 1)
        for (int j = 0; j < 2; j += 1)
            if (mat[i][j] != ((i==j) ? Coord(1.0) : Coord(0.0)))
                return false;
    return true;
}

template<class Coord>
inline GXMatrix2TC<Coord>
GXTranspose( const GXMatrix2TC<Coord>& mat )
{
    GXMatrix2TC<Coord> ret;
    for (int i = 0; i < 2; i += 1)
        for (int j = 0; j < 2; j += 1)
            ret(i,j) = mat(j,i);
    return ret;
}

#endif  // #ifndef __GXAFFINE2_H

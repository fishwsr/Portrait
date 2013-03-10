#pragma once

#ifndef __GXAFFINE4_H
#define __GXAFFINE4_H

/*==========================================================================;
//
//  Copyright (C) 2000 Microsoft Corporation.  All Rights Reserved.
//
//  File:       GXAffine4.h
//  Content:    4D affine - space classes
//                  GXVector4TC   - 4D column vectors
//                  GXCoVector4TC - 4D row vectors (normals)
//                  GXPoint4TC    - 4D points
//
//
//**************************************************************************/

template <class Coord> class GXCoVector4TC;
template <class Coord> class GXPoint4TC;
template <class Coord> class GXMatrix4TC;

//-----------------------------------------------------------------------------
// Class: GXVector4TC
// Description: Column Vector
//-----------------------------------------------------------------------------
template<class Coord>
class GXVector4TC
{
public:
                                GXVector4TC() {}
                                GXVector4TC( const Coord& d );
                                GXVector4TC( const Coord& dx, const Coord& dy, const Coord& dz, const Coord& dw );

    UINT                        Dim() const { return 4; }

    // access grants
    Coord&                      operator[] ( UINT i ) { return (&dx)[i]; }
    const Coord&                operator[] ( UINT i ) const { return (&dx)[i]; }

    // assignment operators
    GXVector4TC<Coord>&         operator += ( const GXVector4TC<Coord>& );
    GXVector4TC<Coord>&         operator -= ( const GXVector4TC<Coord>& );
    GXVector4TC<Coord>&         operator *= ( const Coord& );
    GXVector4TC<Coord>&         operator /= ( const Coord& );

    // unary operators
    GXVector4TC<Coord>          operator + () const;
    GXVector4TC<Coord>          operator - () const;

    // binary operators
    friend GXVector4TC<Coord>   operator + ( const GXVector4TC<Coord>&, const GXVector4TC<Coord>& );
    friend GXVector4TC<Coord>   operator - ( const GXVector4TC<Coord>&, const GXVector4TC<Coord>& );
    friend GXVector4TC<Coord>   operator * ( const Coord&, const GXVector4TC<Coord>& );
    friend GXVector4TC<Coord>   operator * ( const GXVector4TC<Coord>&, const Coord& );
    friend GXVector4TC<Coord>   operator / ( const GXVector4TC<Coord>&, const Coord& );
    friend bool                 operator == ( const GXVector4TC<Coord>&, const GXVector4TC<Coord>& );
    friend bool                 operator != ( const GXVector4TC<Coord>&, const GXVector4TC<Coord>& );

    // friend functions
    friend Coord                GXLengthSq( const GXVector4TC<Coord>& );
    friend Coord                GXLength( const GXVector4TC<Coord>& );
    friend GXVector4TC<Coord>   GXUnit( const GXVector4TC<Coord>& );
    friend UINT                 GXMaxComponent( const GXVector4TC<Coord> &);
    friend Coord                GXMaximumNorm( const GXVector4TC<Coord>& );
    friend Coord                GXDot( const GXVector4TC<Coord>&, const GXVector4TC<Coord>& );
    friend Coord                GXDot( const GXVector4TC<Coord>&, const GXCoVector4TC<Coord>& );
    friend Coord                GXDot( const GXCoVector4TC<Coord>&, const GXVector4TC<Coord>& );
    friend GXMatrix4TC<Coord>   GXOuterProduct( const GXVector4TC<Coord>&, const GXVector4TC<Coord>& );
    friend GXMatrix4TC<Coord>   GXOuterProduct( const GXCoVector4TC<Coord>&, const GXVector4TC<Coord>& );
    friend GXMatrix4TC<Coord>   GXOuterProduct( const GXVector4TC<Coord>&, const GXCoVector4TC<Coord>& );
    friend GXMatrix4TC<Coord>   GXAlternatingProduct( const GXVector4TC<Coord>&, const GXVector4TC<Coord>& );
    friend GXMatrix4TC<Coord>   GXAlternatingProduct( const GXCoVector4TC<Coord>&, const GXVector4TC<Coord>& );
    friend GXMatrix4TC<Coord>   GXAlternatingProduct( const GXVector4TC<Coord>&, const GXCoVector4TC<Coord>& );

    // conversion operators - implicit conversions are inappropriate
    friend GXCoVector4TC<Coord> GXVecToCov( const GXVector4TC<Coord>& );
    friend GXVector4TC<Coord>   GXCovToVec( const GXCoVector4TC<Coord>& );
    friend GXVector4TC<Coord>   GXPntToVec( const GXPoint4TC<Coord>& );

public:
    Coord dx, dy, dz, dw;
};


//-----------------------------------------------------------------------------
// Class: GXCoVector4TC
// Description: Row Vector
//-----------------------------------------------------------------------------

template<class Coord>
class GXCoVector4TC
{
public:
                                GXCoVector4TC( ) {}
                                GXCoVector4TC( const Coord& n );
                                GXCoVector4TC( const Coord& nx, const Coord& ny, const Coord& nz, const Coord& nw );

    UINT                        Dim() const { return 4; }

    // access grants
    Coord&                      operator[] ( UINT i ) { return (&nx)[i]; }
    const Coord&                operator[] ( UINT i ) const { return (&nx)[i]; }

    // assignment operators
    GXCoVector4TC<Coord>&       operator += ( const GXCoVector4TC<Coord>& );
    GXCoVector4TC<Coord>&       operator -= ( const GXCoVector4TC<Coord>& );
    GXCoVector4TC<Coord>&       operator *= ( const Coord& );
    GXCoVector4TC<Coord>&       operator /= ( const Coord& );

    // unary operators
    GXCoVector4TC<Coord>        operator + () const;
    GXCoVector4TC<Coord>        operator - () const;

    // binary operators
    friend GXCoVector4TC<Coord> operator + ( const GXCoVector4TC<Coord>&, const GXCoVector4TC<Coord>& );
    friend GXCoVector4TC<Coord> operator - ( const GXCoVector4TC<Coord>&, const GXCoVector4TC<Coord>& );
    friend GXCoVector4TC<Coord> operator * ( const Coord&, const GXCoVector4TC<Coord>& );
    friend GXCoVector4TC<Coord> operator * ( const GXCoVector4TC<Coord>&, const Coord& );
    friend GXCoVector4TC<Coord> operator / ( const GXCoVector4TC<Coord>&, const Coord& );
    friend bool                 operator == ( const GXCoVector4TC<Coord>&, const GXCoVector4TC<Coord>& );
    friend bool                 operator != ( const GXCoVector4TC<Coord>&, const GXCoVector4TC<Coord>& );

    // friend functions
    friend Coord                GXLengthSq( const GXCoVector4TC<Coord>& );
    friend Coord                GXLength( const GXCoVector4TC<Coord>& );
    friend GXCoVector4TC<Coord> GXUnit( const GXCoVector4TC<Coord>& );
    friend UINT                 GXMaxComponent( const GXCoVector4TC<Coord> &);
    friend Coord                GXMaximumNorm( const GXCoVector4TC<Coord>&);   // L-infinity norm
    friend Coord                GXDot( const GXCoVector4TC<Coord>&, const GXCoVector4TC<Coord>& );
    friend Coord                GXDot( const GXCoVector4TC<Coord>&, const GXVector4TC<Coord>& );
    friend Coord                GXDot( const GXVector4TC<Coord>&, const GXCoVector4TC<Coord>& );
    friend GXMatrix4TC<Coord>   GXOuterProduct( const GXCoVector4TC<Coord>&, const GXCoVector4TC<Coord>& );
    friend GXMatrix4TC<Coord>   GXOuterProduct( const GXCoVector4TC<Coord>&, const GXVector4TC<Coord>& );
    friend GXMatrix4TC<Coord>   GXOuterProduct( const GXVector4TC<Coord>&, const GXCoVector4TC<Coord>& );
    friend GXMatrix4TC<Coord>   GXAlternatingProduct( const GXCoVector4TC<Coord>&, const GXCoVector4TC<Coord>& );
    friend GXMatrix4TC<Coord>   GXAlternatingProduct( const GXCoVector4TC<Coord>&, const GXVector4TC<Coord>& );
    friend GXMatrix4TC<Coord>   GXAlternatingProduct( const GXVector4TC<Coord>&, const GXCoVector4TC<Coord>& );

    // conversion operators - implicit conversions are inappropriate
    friend GXCoVector4TC<Coord> GXVecToCov( const GXCoVector4TC<Coord>& );
    friend GXVector4TC<Coord>   GXCovToVec( const GXCoVector4TC<Coord>& );
    friend GXCoVector4TC<Coord> GXPntToCov( const GXPoint4TC<Coord>& );

public:
    Coord nx, ny, nz, nw;
};


//-----------------------------------------------------------------------------
// Class: GXPoint4TC
//-----------------------------------------------------------------------------

template<class Coord>
class GXPoint4TC
{
public:
                                GXPoint4TC() { }
                                GXPoint4TC( const Coord& x);
                                GXPoint4TC( const Coord& x, const Coord& y, const Coord& z, const Coord& w );

    UINT                        Dim() const { return 4; }

    // access grants
    Coord&                      operator[] ( UINT i ) { return (&x)[i]; }
    const Coord&                operator[] ( UINT i ) const { return (&x)[i]; }

    // assignment operators
    GXPoint4TC<Coord>&          operator += ( const GXVector4TC<Coord>& );
    GXPoint4TC<Coord>&          operator -= ( const GXVector4TC<Coord>& );

    // binary operators
    friend GXPoint4TC<Coord>    operator + ( const GXPoint4TC<Coord>&, const GXVector4TC<Coord>& );
    friend GXPoint4TC<Coord>    operator + ( const GXVector4TC<Coord>&, const GXPoint4TC<Coord>& );
    friend GXPoint4TC<Coord>    operator - ( const GXPoint4TC<Coord>&, const GXVector4TC<Coord>& );
    friend GXVector4TC<Coord>   operator - ( const GXPoint4TC<Coord>&, const GXPoint4TC<Coord>& );
    friend bool                 operator == ( const GXPoint4TC<Coord>&, const GXPoint4TC<Coord>& );
    friend bool                 operator != ( const GXPoint4TC<Coord>&, const GXPoint4TC<Coord>& );

    // point dominance
    bool                        operator < ( const GXPoint4TC<Coord>& ) const;
    bool                        operator <= ( const GXPoint4TC<Coord>& ) const;
    bool                        operator > ( const GXPoint4TC<Coord>& ) const;
    bool                        operator >= ( const GXPoint4TC<Coord>& ) const;

    // friend functions
    friend GXPoint4TC<Coord>    GXLerp( const GXPoint4TC<Coord>&, const GXPoint4TC<Coord>&, Coord );
    friend GXPoint4TC<Coord>    GXBarycentric( const GXPoint4TC<Coord>& p, 
                                    const GXPoint4TC<Coord>& q, 
                                    const GXPoint4TC<Coord>& r,
                                    Coord f, Coord g);

    // conversion operators - implicit conversions are inappropriate
    friend GXVector4TC<Coord>   GXPntToVec( const GXPoint4TC<Coord>& );
    friend GXCoVector4TC<Coord> GXPntToCov( const GXPoint4TC<Coord>& );

public:
    Coord x, y, z, w;
};


//-----------------------------------------------------------------------------
// Class: GXMatrix4TC
//-----------------------------------------------------------------------------

template<class Coord>
class GXMatrix4TC
{
public:
                                GXMatrix4TC() {}
                                GXMatrix4TC( const Coord& s );
                                GXMatrix4TC( const GXVector4TC<Coord>& v1, const GXVector4TC<Coord>& v2,
                                    const GXVector4TC<Coord>& v3, const GXVector4TC<Coord>& v4);
                                GXMatrix4TC( const GXCoVector4TC<Coord>& cov1, const GXCoVector4TC<Coord>& cov2, 
                                    const GXCoVector4TC<Coord>& cov3, const GXCoVector4TC<Coord>& cov4 );

    UINT                        Dim() const { return 4; }

    // access grants
    Coord *                     operator [] ( UINT iRow ) { return mat[iRow]; }
    const Coord *               operator [] ( UINT iRow ) const { return mat[iRow]; }
    Coord&                      operator () ( UINT iRow, UINT iCol ) { return mat[iRow][iCol]; }
    const Coord&                operator () ( UINT iRow, UINT iCol ) const { return mat[iRow][iCol]; }
    GXVector4TC<Coord>          GetColumn( int i ) const { return GXVector4TC<Coord>( mat[0][i], mat[1][i], mat[2][i], mat[3][i] ); }
    GXCoVector4TC<Coord>        GetRow( int i ) const { return GXCoVector4TC<Coord>( mat[i][0], mat[i][1], mat[i][2], mat[i][3] ); }
    void                        SetColumn( int i, const GXVector3TC<Coord>& v ) { mat[0][i] = v[0];  mat[1][i] = v[1]; mat[2][i] = v[2]; mat[3][i] = v[3]; }
    void                        SetRow( int i, const GXCoVector3TC<Coord>& cv ) { mat[i][0] = cv[0];  mat[i][1] = cv[1]; mat[i][2] = cv[2]; mat[i][3] = cv[3]; }

    // unary ops
    GXMatrix4TC<Coord>          operator + ( );
    GXMatrix4TC<Coord>          operator - ( );

    // binary operators
    friend GXMatrix4TC<Coord>   operator * ( const GXMatrix4TC<Coord>& m1, const GXMatrix4TC<Coord>& m2 );
    friend GXMatrix4TC<Coord>   operator + ( const GXMatrix4TC<Coord>&, const GXMatrix4TC<Coord>& );
    friend GXMatrix4TC<Coord>   operator - ( const GXMatrix4TC<Coord>&, const GXMatrix4TC<Coord>& );
    friend GXMatrix4TC<Coord>   operator * ( const GXMatrix4TC<Coord>&, float );
    friend GXMatrix4TC<Coord>   operator * ( float, const GXMatrix4TC<Coord>& );
    friend GXMatrix4TC<Coord>   operator / ( const GXMatrix4TC<Coord>&, float );
    friend bool                 operator == ( const GXMatrix4TC<Coord>&, const GXMatrix4TC<Coord>& );
    // see below for "operator *" with vectors, covectors, and points

    // assignment operators
    GXMatrix4TC<Coord>&         operator *= ( const GXMatrix4TC<Coord>& );
    GXMatrix4TC<Coord>&         operator += ( const GXMatrix4TC<Coord>& );
    GXMatrix4TC<Coord>&         operator -= ( const GXMatrix4TC<Coord>& );
    GXMatrix4TC<Coord>&         operator *= ( const Coord& );
    GXMatrix4TC<Coord>&         operator /= ( const Coord& );
    void                        SetToIdentity();
    bool                        IsIdentity() const;

    // misc functions
    friend GXMatrix4TC<Coord>   GXTranspose( const GXMatrix4TC<Coord>& mat );

public:
    Coord mat[4][4];
};

// matrix multiply with vectors, covectors, points
template<class mCoord, class vCoord> GXVector4TC<vCoord>     operator * ( const GXMatrix4TC<mCoord>& m, const GXVector4TC<vCoord>& v);
template<class mCoord, class cvCoord> GXCoVector4TC<cvCoord> operator * ( const GXCoVector4TC<cvCoord>& cv, const GXMatrix4TC<mCoord>& m );
template<class mCoord, class pCoord> GXPoint4TC<pCoord>      operator * ( const GXMatrix4TC<mCoord>& m, const GXPoint4TC<pCoord>& p );


//-----------------------------------------------------------------------------
// float-valued points, vectors, covectors
//-----------------------------------------------------------------------------
#include "gxfmath.h"
typedef GXVector4TC<float> GXVector4;
typedef GXCoVector4TC<float> GXCoVector4;
typedef GXPoint4TC<float> GXPoint4;
typedef GXMatrix4TC<float> GXMatrix4;


#ifdef _GXDEFINE_DLL_ENTRYPOINTS
extern "C"
{
    HRESULT WINAPI GXMatrix4Inverse( GXMatrix4 *pmatOut, GXMatrix4 *pmat );
}
#endif  // #ifdef _GXDEFINE_DLL_ENTRYPOINTS


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/// GXVector4TC Inline Functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// -------------------------------------
// constructors
// -------------------------------------

template<class Coord>
inline
GXVector4TC<Coord>::GXVector4TC( const Coord& _d )
{
    dx = dy = dz = dw = _d;
}

template<class Coord>
inline
GXVector4TC<Coord>::GXVector4TC( const Coord& _dx, const Coord& _dy, const Coord& _dz, const Coord& _dw )
{
    dx = _dx; dy = _dy; dz = _dz; dw = _dw;
}

// -------------------------------------
// assignment operators
// -------------------------------------

template<class Coord>
inline GXVector4TC<Coord>&
GXVector4TC<Coord>::operator += ( const GXVector4TC<Coord>& v)
{
    dx += v.dx;
    dy += v.dy;
    dz += v.dz;
    dw += v.dw;
    return *this;
}

template<class Coord>
inline GXVector4TC<Coord>&
GXVector4TC<Coord>::operator -= ( const GXVector4TC<Coord>& v )
{
    dx -= v.dx;
    dy -= v.dy;
    dz -= v.dz;
    dw -= v.dw;
    return *this;
}

template<class Coord>
inline GXVector4TC<Coord>&
GXVector4TC<Coord>::operator *= ( const Coord& s )
{
    dx *= s;
    dy *= s;
    dz *= s;
    dw *= s;
    return *this;
}

template<class Coord>
inline GXVector4TC<Coord>&
GXVector4TC<Coord>::operator /= ( const Coord& s )
{
    Coord s1 = Coord(1.0f) / s;
    dx *= s1;
    dy *= s1;
    dz *= s1;
    dw *= s1;
    return *this;
}

// -------------------------------------
// unary operators
// -------------------------------------

template<class Coord>
inline GXVector4TC<Coord>
GXVector4TC<Coord>::operator + () const
{
    return *this;
}

template<class Coord>
inline GXVector4TC<Coord>
GXVector4TC<Coord>::operator - () const
{
    return GXVector4TC<Coord>( -dx, -dy, -dz, -dw );
}

// -------------------------------------
// binary operators
// -------------------------------------

template<class Coord>
inline GXVector4TC<Coord>
operator + ( const GXVector4TC<Coord>& v1, const GXVector4TC<Coord>& v2 )
{
    return GXVector4TC<Coord>( v1.dx + v2.dx, v1.dy + v2.dy, v1.dz + v2.dz, v1.dw + v2.dw );
}

template<class Coord>
inline GXVector4TC<Coord>
operator - ( const GXVector4TC<Coord>& v1, const GXVector4TC<Coord>& v2 )
{
    return GXVector4TC<Coord>( v1.dx - v2.dx, v1.dy - v2.dy, v1.dz - v2.dz, v1.dw - v2.dw );
}

template<class Coord>
inline GXVector4TC<Coord>
operator * ( const Coord& s, const GXVector4TC<Coord>& v )
{
    return GXVector4TC<Coord>( v.dx * s, v.dy * s, v.dz * s, v.dw * s );
}

template<class Coord>
inline GXVector4TC<Coord>
operator * ( const GXVector4TC<Coord>& v, const Coord& s )
{
    return GXVector4TC<Coord>( v.dx * s, v.dy * s, v.dz * s, v.dw * s );
}

template<class Coord>
inline GXVector4TC<Coord>
operator / ( const GXVector4TC<Coord>& v, const Coord& s )
{
    Coord s1 = Coord(1.0f) / s;
    return GXVector4TC<Coord>( v.dx * s1, v.dy * s1, v.dz * s1, v.dw * s1 );
}

template<class Coord>
inline bool
operator == ( const GXVector4TC<Coord>& v1, const GXVector4TC<Coord>& v2 )
{
    return v1.dx == v2.dx && v1.dy == v2.dy && v1.dz == v2.dz && v1.dw == v2.dw;
}

template<class Coord>
inline bool
operator != ( const GXVector4TC<Coord>& v1, const GXVector4TC<Coord>& v2 )
{
    return ! (v1 == v2);
}

// -------------------------------------
// friend functions
// -------------------------------------

template<class Coord>
inline Coord
GXLengthSq( const GXVector4TC<Coord>& v )
{
    return v.dx*v.dx + v.dy*v.dy + v.dz*v.dz + v.dw*v.dw;
}

template<class Coord>
inline Coord
GXLength( const GXVector4TC<Coord>& v )
{
    return (Coord) GXSqrt(GXLengthSq(v));
}

template<class Coord>
inline GXVector4TC<Coord>
GXUnit( const GXVector4TC<Coord>& v )
{
    return v * GXInvSqrt(GXLengthSq(v));
}

template<class Coord>
inline UINT
GXMaxComponent(const GXVector4TC<Coord> &v)
{
    UINT i, iMax = 0;
    for (i = 1; i < v.Dim(); i++)
        if (GXFAbs(v[i]) > GXFAbs(v[iMax]))
            iMax = i;
    return iMax;
}

template<class Coord>
inline Coord
GXMaximumNorm(const GXVector4TC<Coord> &v)
{
    return GXFAbs(v[GXMaxComponent(v)]);
}

template<class Coord>
inline Coord
GXDot( const GXVector4TC<Coord>& v1, const GXVector4TC<Coord>& v2 )
{
    return v1.dx * v2.dx + v1.dy * v2.dy + v1.dz * v2.dz + v1.dw * v2.dw;
}

template<class Coord>
inline Coord
GXDot( const GXVector4TC<Coord>& v1, const GXCoVector4TC<Coord>& v2 )
{
    return v1.dx * v2.nx + v1.dy * v2.ny + v1.dz * v2.nz + v1.dw * v2.nw;
}

template<class Coord>
inline Coord
GXDot( const GXCoVector4TC<Coord>& v1, const GXVector4TC<Coord>& v2 )
{
    return v1.nx * v2.dx + v1.ny * v2.dy + v1.nz * v2.dz + v1.nw * v2.dw;
}

template<class Coord>
inline GXMatrix4TC<Coord>
GXOuterProduct( const GXVector4TC<Coord>& v1, const GXVector4TC<Coord>& v2 )
{
    GXMatrix4TC<Coord> ret;
    for (UINT i = 0; i < 4; i++)
        for (UINT j = 0; j < 4; j++)
            ret(i,j)=v1[i]*v2[j];
    return ret;
}

template<class Coord>
inline GXMatrix4TC<Coord>
GXOuterProduct( const GXCoVector4TC<Coord>& v1, const GXVector4TC<Coord>& v2 )
{
    GXMatrix4TC<Coord> ret;
    for (UINT i = 0; i < 4; i++)
        for (UINT j = 0; j < 4; j++)
            ret(i,j)=v1[i]*v2[j];
    return ret;
}

template<class Coord>
inline GXMatrix4TC<Coord>
GXOuterProduct( const GXVector4TC<Coord>& v1, const GXCoVector4TC<Coord>& v2 )
{
    GXMatrix4TC<Coord> ret;
    for (UINT i = 0; i < 4; i++)
        for (UINT j = 0; j < 4; j++)
            ret(i,j)=v1[i]*v2[j];
    return ret;
}

template<class Coord>
inline GXMatrix4TC<Coord>
GXOuterProduct( const GXCoVector4TC<Coord>& v1, const GXCoVector4TC<Coord>& v2 )
{
    GXMatrix4TC<Coord> ret;
    for (UINT i = 0; i < 4; i++)
        for (UINT j = 0; j < 4; j++)
            ret(i,j)=v1[i]*v2[j];
    return ret;
}

template<class Coord>
inline GXMatrix4TC<Coord>
GXAlternatingProduct( const GXVector4TC<Coord>& v1, const GXVector4TC<Coord>& v2 )
{
    GXMatrix4TC<Coord> ret;

    ret(0,0) = ret(1,1) = ret(2,2) = ret(3,3) = 0.0f;
    ret(1,0) = v1.dy*v2.dx-v1.dx*v2.dy;
    ret(2,0) = v1.dz*v2.dx-v1.dx*v2.dz; ret(2,1) = v1.dz*v2.dy-v1.dy*v2.dz;
    ret(3,0) = v1.dw*v2.dx-v1.dx*v2.dw; ret(3,1) = v1.dw*v2.dy-v1.dy*v2.dw; ret(3,2) = v1.dw*v2.dz-v1.dz*v2.dw;
    ret(0,1) = -ret(1,0);
    ret(0,2) = -ret(2,0);   ret(1,2) = -ret(2,1);
    ret(0,3) = -ret(3,0);   ret(1,3) = -ret(3,1);   ret(2,3) = -ret(3,2);

    return ret;
}

template<class Coord>
inline GXMatrix4TC<Coord>
GXAlternatingProduct( const GXVector4TC<Coord>& v1, const GXCoVector4TC<Coord>& v2 )
{
    GXMatrix4TC<Coord> ret;

    ret(0,0) = ret(1,1) = ret(2,2) = ret(3,3) = 0.0f;
    ret(1,0) = v1.dy*v2.nx-v1.dx*v2.ny;
    ret(2,0) = v1.dz*v2.nx-v1.dx*v2.nz; ret(2,1) = v1.dz*v2.ny-v1.dy*v2.nz;
    ret(3,0) = v1.dw*v2.nx-v1.dx*v2.nw; ret(3,1) = v1.dw*v2.ny-v1.dy*v2.nw; ret(3,2) = v1.dw*v2.nz-v1.dz*v2.nw;
    ret(0,1) = -ret(1,0);
    ret(0,2) = -ret(2,0);   ret(1,2) = -ret(2,1);
    ret(0,3) = -ret(3,0);   ret(1,3) = -ret(3,1);   ret(2,3) = -ret(3,2);

    return ret;
}

template<class Coord>
inline GXMatrix4TC<Coord>
GXAlternatingProduct( const GXCoVector4TC<Coord>& v1, const GXVector4TC<Coord>& v2 )
{
    GXMatrix4TC<Coord> ret;

    ret(0,0) = ret(1,1) = ret(2,2) = ret(3,3) = 0.0f;
    ret(1,0) = v1.ny*v2.dx-v1.nx*v2.dy;
    ret(2,0) = v1.nz*v2.dx-v1.nx*v2.dz; ret(2,1) = v1.nz*v2.dy-v1.ny*v2.dz;
    ret(3,0) = v1.nw*v2.dx-v1.nx*v2.dw; ret(3,1) = v1.nw*v2.dy-v1.ny*v2.dw; ret(3,2) = v1.nw*v2.dz-v1.nz*v2.dw;
    ret(0,1) = -ret(1,0);
    ret(0,2) = -ret(2,0);   ret(1,2) = -ret(2,1);
    ret(0,3) = -ret(3,0);   ret(1,3) = -ret(3,1);   ret(2,3) = -ret(3,2);

    return ret;
}

template<class Coord>
inline GXMatrix4TC<Coord>
GXAlternatingProduct( const GXCoVector4TC<Coord>& v1, const GXCoVector4TC<Coord>& v2 )
{
    GXMatrix4TC<Coord> ret;

    ret(0,0) = ret(1,1) = ret(2,2) = ret(3,3) = 0.0f;
    ret(1,0) = v1.ny*v2.nx-v1.nx*v2.ny;
    ret(2,0) = v1.nz*v2.nx-v1.nx*v2.nz; ret(2,1) = v1.nz*v2.ny-v1.ny*v2.nz;
    ret(3,0) = v1.nw*v2.nx-v1.nx*v2.nw; ret(3,1) = v1.nw*v2.ny-v1.ny*v2.nw; ret(3,2) = v1.nw*v2.nz-v1.nz*v2.nw;
    ret(0,1) = -ret(1,0);
    ret(0,2) = -ret(2,0);   ret(1,2) = -ret(2,1);
    ret(0,3) = -ret(3,0);   ret(1,3) = -ret(3,1);   ret(2,3) = -ret(3,2);

    return ret;
}

template<class Coord>
inline GXCoVector4TC<Coord>
GXVecToCov( const GXVector4TC<Coord>& v )
{
    return GXCoVector4TC<Coord>( v.dx, v.dy, v.dz, v.dw );
}

template<class Coord>
inline GXVector4TC<Coord>
GXCovToVec( const GXCoVector4TC<Coord>& cv )
{
    return GXVector4TC<Coord>( cv.nx, cv.ny, cv.nz, cv.nw );
}

template<class Coord>
inline GXVector4TC<Coord>
GXPntToVec( const GXPoint4TC<Coord>& pnt )
{
    return GXVector4TC<Coord>( pnt.x, pnt.y, pnt.z, pnt.w );
}

template<class Coord>
inline GXCoVector4TC<Coord>
GXPntToCov( const GXPoint4TC<Coord>& pnt )
{
    return GXCoVector4TC<Coord>( pnt.x, pnt.y, pnt.z, pnt.w );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/// GXCoVector4TC Inline Functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// -------------------------------------
// constructors
// -------------------------------------

template<class Coord>
inline
GXCoVector4TC<Coord>::GXCoVector4TC( const Coord& _nd )
{
    dx = dy = dz = dw = _n;
}

template<class Coord>
inline
GXCoVector4TC<Coord>::GXCoVector4TC( const Coord& _nx, const Coord& _ny, const Coord& _nz, const Coord& _nw )
{
    nx = _nx; ny = _ny; nz = _nz; nw = _nw;
}

// -------------------------------------
// assignment operators
// -------------------------------------

template<class Coord>
inline GXCoVector4TC<Coord>&
GXCoVector4TC<Coord>::operator += ( const GXCoVector4TC<Coord>& v)
{
    nx += v.nx;
    ny += v.ny;
    nz += v.nz;
    nw += v.nw;
    return *this;
}

template<class Coord>
inline GXCoVector4TC<Coord>&
GXCoVector4TC<Coord>::operator -= ( const GXCoVector4TC<Coord>& v )
{
    nx -= v.nx;
    ny -= v.ny;
    nz -= v.nz;
    nw -= v.nw;
    return *this;
}

template<class Coord>
inline GXCoVector4TC<Coord>&
GXCoVector4TC<Coord>::operator *= ( const Coord& s )
{
    nx *= s;
    ny *= s;
    nz *= s;
    nw *= s;
    return *this;
}

template<class Coord>
inline GXCoVector4TC<Coord>&
GXCoVector4TC<Coord>::operator /= ( const Coord& s )
{
    Coord s1 = Coord(1.0f) / s;
    nx *= s1;
    ny *= s1;
    nz *= s1;
    nw *= s1;
    return *this;
}

// -------------------------------------
// unary operators
// -------------------------------------

template<class Coord>
inline GXCoVector4TC<Coord>
GXCoVector4TC<Coord>::operator + () const
{
    return *this;
}

template<class Coord>
inline GXCoVector4TC<Coord>
GXCoVector4TC<Coord>::operator - () const
{
    return GXCoVector4TC<Coord>( -nx, -ny, -nz, -nw );
}

// -------------------------------------
// binary operators
// -------------------------------------

template<class Coord>
inline GXCoVector4TC<Coord>
operator + ( const GXCoVector4TC<Coord>& v1, const GXCoVector4TC<Coord>& v2 )
{
    return GXCoVector4TC<Coord>( v1.nx + v2.nx, v1.ny + v2.ny, v1.nz + v2.nz, v1.nw + v2.nw );
}

template<class Coord>
inline GXCoVector4TC<Coord>
operator - ( const GXCoVector4TC<Coord>& v1, const GXCoVector4TC<Coord>& v2 )
{
    return GXCoVector4TC<Coord>( v1.nx - v2.nx, v1.ny - v2.ny, v1.nz - v2.nz, v1.nw - v2.nw );
}

template<class Coord>
inline GXCoVector4TC<Coord>
operator * ( const Coord& s, const GXCoVector4TC<Coord>& v )
{
    return GXCoVector4TC<Coord>( v.nx * s, v.ny * s, v.nz * s, v.nw * s );
}

template<class Coord>
inline GXCoVector4TC<Coord>
operator * ( const GXCoVector4TC<Coord>& v, const Coord& s )
{
    return GXCoVector4TC<Coord>( v.nx * s, v.ny * s, v.nz * s, v.nw * s );
}

template<class Coord>
inline GXCoVector4TC<Coord>
operator / ( const GXCoVector4TC<Coord>& v, const Coord& s )
{
    Coord s1 = Coord(1.0f) / s;
    return GXCoVector4TC<Coord>( v.nx * s1, v.ny * s1, v.nz * s1, v.nw * s1 );
}

template<class Coord>
inline bool
operator == ( const GXCoVector4TC<Coord>& v1, const GXCoVector4TC<Coord>& v2 )
{
    return v1.nx == v2.nx && v1.ny == v2.ny && v1.nz == v2.nz && v1.nw == v2.nw;
}

template<class Coord>
inline bool
operator != ( const GXCoVector4TC<Coord>& v1, const GXCoVector4TC<Coord>& v2 )
{
    return ! (v1 == v2);
}

// -------------------------------------
// friend functions
// -------------------------------------

template<class Coord>
inline Coord
GXLengthSq( const GXCoVector4TC<Coord>& v )
{
    return v.nx*v.nx + v.ny*v.ny + v.nz*v.nz + v.nw*v.nw;
}

template<class Coord>
inline Coord
GXLength( const GXCoVector4TC<Coord>& v )
{
    return (Coord) GXSqrt(GXLengthSq(v));
}

template<class Coord>
inline GXCoVector4TC<Coord>
GXUnit( const GXCoVector4TC<Coord>& v )
{
    return v * GXInvSqrt(GXLengthSq(v));
}

template<class Coord>
inline UINT
GXMaxComponent(const GXCoVector4TC<Coord> &v)
{
    UINT i, iMax = 0;
    for (i = 1; i < v.Dim(); i++)
        if (GXFAbs(v[i]) > GXFAbs(v[iMax]))
            iMax = i;
    return iMax;
}

template<class Coord>
inline Coord
GXMaximumNorm(const GXCoVector4TC<Coord> &v)
{
    return GXFAbs(v[GXMaxComponent(v)]);
}


template<class Coord>
inline Coord
GXDot( const GXCoVector4TC<Coord>& v1, const GXCoVector4TC<Coord>& v2 )
{
    return v1.nx * v2.nx + v1.ny * v2.ny + v1.nz * v2.nz + v1.nw * v2.nw;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/// GXPoint4TC Inline Functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// -------------------------------------
// constructors
// -------------------------------------

template<class Coord>
inline
GXPoint4TC<Coord>::GXPoint4TC ( const Coord& _x, const Coord& _y, const Coord& _z, const Coord& _w )
{
    x = _x; y = _y; z = _z; w = _w;
}

template<class Coord>
inline
GXPoint4TC<Coord>::GXPoint4TC ( const Coord& _x )
{
    x = y = z = w = _x;
}

// -------------------------------------
// constructors
// -------------------------------------

template<class Coord>
inline GXPoint4TC<Coord>&
GXPoint4TC<Coord>::operator += ( const GXVector4TC<Coord>& vDelta )
{
    x += vDelta.dx;
    y += vDelta.dy;
    z += vDelta.dz;
    w += vDelta.dw;
    return *this;
}

template<class Coord>
inline GXPoint4TC<Coord>&
GXPoint4TC<Coord>::operator -= (const GXVector4TC<Coord>& vDelta )
{
    x -= vDelta.dx;
    y -= vDelta.dy;
    z -= vDelta.dz;
    w -= vDelta.dw;
    return *this;
}

// -------------------------------------
// binary operators
// -------------------------------------

template<class Coord>
inline GXPoint4TC<Coord>
operator + ( const GXPoint4TC<Coord>& p, const GXVector4TC<Coord>& vDelta )
{
    return GXPoint4TC<Coord>( p.x + vDelta.dx, p.y + vDelta.dy, p.z + vDelta.dz, p.w + vDelta.dw );
}

template<class Coord>
inline GXPoint4TC<Coord>
operator + ( const GXVector4TC<Coord>& vDelta, const GXPoint4TC<Coord>& p )
{
    return GXPoint4TC<Coord>( p.x + vDelta.dx, p.y + vDelta.dy, p.z + vDelta.dz, p.w + vDelta.dw );
}

template<class Coord>
inline GXPoint4TC<Coord>
operator - ( const GXPoint4TC<Coord>& p, const GXVector4TC<Coord>& vDelta )
{
    return GXPoint4TC<Coord>( p.x - vDelta.dx, p.y - vDelta.dy, p.z - vDelta.dz, p.w - vDelta.dw );
}

template<class Coord>
inline GXVector4TC<Coord>
operator - ( const GXPoint4TC<Coord>& p1, const GXPoint4TC<Coord>& p2 )
{
    return GXVector4TC<Coord>( p1.x - p2.x, p1.y - p2.y, p1.z - p2.z, p1.w - p2.w );
}

template<class Coord>
inline bool
operator == ( const GXPoint4TC<Coord>& p1, const GXPoint4TC<Coord>& p2 )
{
    return p1.x == p2.x && p1.y == p2.y && p1.z == p2.z && p1.w == p2.w;
}

template<class Coord>
inline bool
operator != ( const GXPoint4TC<Coord>& p1, const GXPoint4TC<Coord>& p2 )
{
    return ! (p1 == p2);
}

// -------------------------------------
// point dominance
// -------------------------------------

template<class Coord>
inline bool
GXPoint4TC<Coord>::operator < ( const GXPoint4TC<Coord>& p ) const
{
    return x < p.x && y < p.y && z < p.z && w < p.w;
}

template<class Coord>
inline bool
GXPoint4TC<Coord>::operator<= ( const GXPoint4TC<Coord>& p ) const
{
    return x <= p.x && y <= p.y && z <= p.z && w <= p.w;
}

template<class Coord>
inline bool
GXPoint4TC<Coord>::operator > ( const GXPoint4TC<Coord>& p ) const
{
    return x > p.x && y > p.y && z > p.z && w > p.w;
}

template<class Coord>
inline bool
GXPoint4TC<Coord>::operator >= ( const GXPoint4TC<Coord>& p ) const
{
    return x >= p.x && y >= p.y && z >= p.z && w >= p.w;
}

// -------------------------------------
// friend functions
// -------------------------------------

template<class Coord>
inline GXPoint4TC<Coord>
GXLerp( const GXPoint4TC<Coord>&p1, const GXPoint4TC<Coord>&p2, Coord s )
{
    GXVector4TC<Coord> p1p2 = p2 - p1;
    return GXPoint4TC<Coord>( p1 + s*p1p2 );
}

template<class Coord>
inline GXPoint4TC<Coord>
GXBarycentric( const GXPoint4TC<Coord>& p, 
              const GXPoint4TC<Coord>& q, 
              const GXPoint4TC<Coord>& r,
              Coord f, Coord g)
{
    return p+f*(q-p)+g*(r-p);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/// GXMatrix4TC Inline Functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// -------------------------------------
// constructors
// -------------------------------------

template<class Coord>
inline
GXMatrix4TC<Coord>::GXMatrix4TC( const Coord& s )
{
    for ( int i = 0; i < 4; i++ )
        for ( int j = 0; j < 4; j++ )
            mat[i][j] = s;
}

template<class Coord>
inline
GXMatrix4TC<Coord>::GXMatrix4TC(
    const GXVector4TC<Coord>& vCol1, 
    const GXVector4TC<Coord>& vCol2,
    const GXVector4TC<Coord>& vCol3, 
    const GXVector4TC<Coord>& vCol4)
{
    mat[0][0] = vCol1.dx; mat[0][1] = vCol2.dx; mat[0][2] = vCol3.dx; mat[0][3] = vCol4.dx;
    mat[1][0] = vCol1.dy; mat[1][1] = vCol2.dy; mat[1][2] = vCol3.dy; mat[1][3] = vCol4.dy;
    mat[2][0] = vCol1.dz; mat[2][1] = vCol2.dz; mat[2][2] = vCol3.dz; mat[2][3] = vCol4.dz;
    mat[3][0] = vCol1.dw; mat[3][1] = vCol2.dw; mat[3][2] = vCol3.dw; mat[3][3] = vCol4.dw;
}

template<class Coord>
inline
GXMatrix4TC<Coord>::GXMatrix4TC( 
    const GXCoVector4TC<Coord>& covRow1, 
    const GXCoVector4TC<Coord>& covRow2,
    const GXCoVector4TC<Coord>& covRow3,
    const GXCoVector4TC<Coord>& covRow4 )
{
    mat[0][0] = covRow1.nx; mat[0][1] = covRow1.ny; mat[0][2] = covRow1.nz; mat[0][3] = covRow1.nw;
    mat[1][0] = covRow2.nx; mat[1][1] = covRow2.ny; mat[1][2] = covRow2.nz; mat[1][3] = covRow2.nw;
    mat[2][0] = covRow3.nx; mat[2][1] = covRow3.ny; mat[2][2] = covRow3.nz; mat[2][3] = covRow3.nw;
    mat[3][0] = covRow4.nx; mat[3][1] = covRow4.ny; mat[3][2] = covRow4.nz; mat[3][3] = covRow4.nw;
}


// -------------------------------------
// unary operators
// -------------------------------------

template<class Coord>
inline GXMatrix4TC<Coord>
GXMatrix4TC<Coord>::operator + ( )
{
    return *this;
}

template<class Coord>
inline GXMatrix4TC<Coord>
GXMatrix4TC<Coord>::operator - ( )
{
    GXMatrix4TC<Coord> ret;
    
    for (UINT i = 0; i < 4; i += 1)
        for (UINT j = 0; j < 4; j += 1)
            ret(i, j) = -mat[i][j];
    return ret;
}

// -------------------------------------
// binary operators
// -------------------------------------

template<class Coord>
inline GXMatrix4TC<Coord>
operator + ( const GXMatrix4TC<Coord>& m1, const GXMatrix4TC<Coord>& m2 )
{
    GXMatrix4TC<Coord> ret;

    for (UINT i = 0; i < 4; i += 1)
        for (UINT j = 0; j < 4; j += 1)
            ret(i, j) = m1(i, j) + m2(i, j);
    return ret;
}

template<class Coord>
inline GXMatrix4TC<Coord>
operator - ( const GXMatrix4TC<Coord>& m1, const GXMatrix4TC<Coord>& m2 )
{
    GXMatrix4TC<Coord> ret;

    for (UINT i = 0; i < 4; i += 1)
        for (UINT j = 0; j < 4; j += 1)
            ret(i, j) = m1(i, j) - m2(i, j);
    return ret;
}

template<class Coord>
inline GXMatrix4TC<Coord>
operator * ( const GXMatrix4TC<Coord>& m, Coord s )
{
    GXMatrix4TC<Coord> ret;

    for (UINT i = 0; i < 4; i += 1)
        for (UINT j = 0; j < 4; j += 1)
            ret(i, j) = m(i, j) * s;
    return ret;
}

template<class Coord>
inline GXMatrix4TC<Coord>
operator * ( Coord s, const GXMatrix4TC<Coord>& m )
{
    GXMatrix4TC<Coord> ret;

    for (UINT i = 0; i < 4; i += 1)
        for (UINT j = 0; j < 4; j += 1)
            ret(i, j) = m(i, j) * s;
    return ret;
}

template<class Coord>
inline GXMatrix4TC<Coord> 
operator * ( const GXMatrix4TC<Coord>& m1, const GXMatrix4TC<Coord>& m2 )
{
    GXMatrix4TC<Coord> ret;
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            ret(i, j) = Coord(0.0);
            for (int k=0; k<4; k++) {
                ret(i, j) += m1(i, k) * m2(k, j);
            }
        }
    }
    return ret;
}

template<class mCoord, class vCoord>
inline GXVector4TC<vCoord>
operator * ( const GXMatrix4TC<mCoord>& m, const GXVector4TC<vCoord>& v)
{
    return GXVector4TC<vCoord>(v.dx*m(0,0) + v.dy*m(0,1) + v.dz*m(0,2)+ v.dw*m(0,3),
                               v.dx*m(1,0) + v.dy*m(1,1) + v.dz*m(1,2)+ v.dw*m(1,3),
                               v.dx*m(2,0) + v.dy*m(2,1) + v.dz*m(2,2)+ v.dw*m(2,3),
                               v.dx*m(3,0) + v.dy*m(3,1) + v.dz*m(3,2)+ v.dw*m(3,3)
                           );
}

template<class mCoord, class cvCoord>
inline GXCoVector4TC<cvCoord>
operator * ( const GXCoVector4TC<cvCoord>& v, const GXMatrix4TC<mCoord>& m )
{
    return GXCoVector4TC<cvCoord>(v.nx*m(0,0) + v.ny*m(1,0) + v.nz*m(2,0) + v.nw*m(3,0),
                                  v.nx*m(0,1) + v.ny*m(1,1) + v.nz*m(2,1) + v.nw*m(3,1),
                                  v.nx*m(0,2) + v.ny*m(1,2) + v.nz*m(2,2) + v.nw*m(3,2),
                                  v.nx*m(0,3) + v.ny*m(1,3) + v.nz*m(2,3) + v.nw*m(3,3)
                           );
}

template<class Coord>
inline GXMatrix4TC<Coord>
operator / ( const GXMatrix4TC<Coord>& m, Coord s )
{
    GXMatrix4TC<Coord> ret;
    Coord s1 = Coord(1.0f) / s;

    for (UINT i = 0; i < 4; i += 1)
        for (UINT j = 0; j < 4; j += 1)
            ret(i, j) = m(i, j) * s1;
    return ret;
}

template<class Coord>
inline int
operator == ( const GXMatrix4TC<Coord>& m1, const GXMatrix4TC<Coord>& m2)
{
    for (UINT i = 0; i < 4; i += 1)
        for (UINT j = 0; j < 4; j += 1)
            if ( m1(i,j) != m2(i,j) )
                return false;
    return true;
}


// -------------------------------------
// assignment operators
// -------------------------------------

template<class Coord>
inline GXMatrix4TC<Coord>&
GXMatrix4TC<Coord>::operator += ( const GXMatrix4TC<Coord>& m )
{
    for (UINT i = 0; i < 4; i += 1)
        for (UINT j = 0; j < 4; j += 1)
            mat[i][j] += m(i, j);
    return *this;
}

template<class Coord>
inline GXMatrix4TC<Coord>&
GXMatrix4TC<Coord>::operator -= ( const GXMatrix4TC<Coord>& m )
{
    for (UINT i = 0; i < 4; i += 1)
        for (UINT j = 0; j < 4; j += 1)
            mat[i][j] -= m(i, j);
    return *this;
}

template<class Coord>
inline GXMatrix4TC<Coord>&
GXMatrix4TC<Coord>::operator *= ( const Coord& s )
{
    for (UINT i = 0; i < 4; i += 1)
        for (UINT j = 0; j < 4; j += 1)
            mat[i][j] *= s;
    return *this;
}

template<class Coord>
inline GXMatrix4TC<Coord>&
GXMatrix4TC<Coord>::operator /= ( const Coord& s )
{
    Coord s1 = Coord(1.0f) / s;
    for (UINT i = 0; i < 4; i += 1)
        for (UINT j = 0; j < 4; j += 1)
            mat[i][j] *= s1;
    return *this;
}

template<class Coord>
inline GXMatrix4TC<Coord>&
GXMatrix4TC<Coord>::operator *= ( const GXMatrix4TC<Coord>& m )
{
    *this = *this * m;
    return *this;
}

// -------------------------------------
// misc functions
// -------------------------------------

template<class Coord>
void
GXMatrix4TC<Coord>::SetToIdentity()
{
    for (int i = 0; i < 4; i += 1)
        for (int j = 0; j < 4; j += 1)
            mat[i][j] = ((i==j) ? Coord(1.0) : Coord(0.0));
}

template<class Coord>
bool
GXMatrix4TC<Coord>::IsIdentity() const
{
    for (int i = 0; i < 4; i += 1)
        for (int j = 0; j < 4; j += 1)
            if (mat[i][j] != ((i==j) ? Coord(1.0) : Coord(0.0)))
                return false;
    return true;
}

template<class Coord>
inline GXMatrix4TC<Coord>
GXTranspose( const GXMatrix4TC<Coord>& mat )
{
    GXMatrix4TC<Coord> ret;
    for (int i = 0; i < 4; i += 1)
        for (int j = 0; j < 4; j += 1)
            ret(i,j) = mat(j,i);
    return ret;
}

#endif  // #ifndef __GXAFFINE4_H

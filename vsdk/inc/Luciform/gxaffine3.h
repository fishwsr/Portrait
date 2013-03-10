#pragma once

#ifndef __GXAFFINE3_H
#define __GXAFFINE3_H

/*==========================================================================;
//
//  Copyright (C) 2000 Microsoft Corporation.  All Rights Reserved.
//
//  File:       gxaffine3.h
//  Content:    3D affine-space classes
//                  GXVector3TC   - 3D column vectors
//                  GXCoVector3TC - 3D row vectors (normals)
//                  GXPoint3TC    - 3D points
//
//
//**************************************************************************/

// identifier was truncated to '255' characters in the browser information
#pragma warning(disable: 4786) 

template <class Coord> class GXCoVector3TC;
template <class Coord> class GXPoint3TC;
template <class Coord> class GXMatrix3TC;


//-----------------------------------------------------------------------------
// Class: GXVector3TC
// Description: Column Vector
//-----------------------------------------------------------------------------
template<class Coord>
class GXVector3TC
{
public:
                                GXVector3TC() {}
                                GXVector3TC( const Coord& d );
                                GXVector3TC( const Coord& dx, const Coord& dy, const Coord& dz );

    UINT                        Dim() const { return 3; }

    // access grants
    Coord&                      operator[] ( UINT i ) { return (&dx)[i]; }
    const Coord&                operator[] ( UINT i ) const { return (&dx)[i]; }

    // assignment operators
    GXVector3TC<Coord>&         operator += ( const GXVector3TC<Coord>& );
    GXVector3TC<Coord>&         operator -= ( const GXVector3TC<Coord>& );
    GXVector3TC<Coord>&         operator *= ( const Coord& );
    GXVector3TC<Coord>&         operator /= ( const Coord& );

    // unary operators
    GXVector3TC<Coord>          operator + () const;
    GXVector3TC<Coord>          operator - () const;

    // binary operators
    friend GXVector3TC<Coord>   operator + ( const GXVector3TC<Coord>&, const GXVector3TC<Coord>& );
    friend GXVector3TC<Coord>   operator - ( const GXVector3TC<Coord>&, const GXVector3TC<Coord>& );
    friend GXVector3TC<Coord>   operator * ( const Coord&, const GXVector3TC<Coord>& );
    friend GXVector3TC<Coord>   operator * ( const GXVector3TC<Coord>&, const Coord& );
    friend GXVector3TC<Coord>   operator / ( const GXVector3TC<Coord>&, const Coord& );
    friend bool                 operator == ( const GXVector3TC<Coord>&, const GXVector3TC<Coord>& );
    friend bool                 operator != ( const GXVector3TC<Coord>&, const GXVector3TC<Coord>& );

    // misc functions
    friend Coord                GXLengthSq( const GXVector3TC<Coord>& );
    friend Coord                GXLength( const GXVector3TC<Coord>& );
    friend GXVector3TC<Coord>   GXUnit( const GXVector3TC<Coord>& );
    friend UINT                 GXMaxComponent( const GXVector3TC<Coord> &v);
    friend Coord                GXMaximumNorm( const GXVector3TC<Coord>&);
    friend Coord                GXDot( const GXVector3TC<Coord>&, const GXVector3TC<Coord>& );
    friend Coord                GXDot( const GXVector3TC<Coord>&, const GXCoVector3TC<Coord>& );
    friend Coord                GXDot( const GXCoVector3TC<Coord>&, const GXVector3TC<Coord>& );
    friend GXVector3TC<Coord>   GXCross( const GXVector3TC<Coord>&, const GXVector3TC<Coord>& );
    friend GXMatrix3TC<Coord>   GXOuterProduct( const GXVector3TC<Coord>&, const GXVector3TC<Coord>& );
    friend GXMatrix3TC<Coord>   GXOuterProduct( const GXCoVector3TC<Coord>&, const GXVector3TC<Coord>& );
    friend GXMatrix3TC<Coord>   GXOuterProduct( const GXVector3TC<Coord>&, const GXCoVector3TC<Coord>& );
    friend GXMatrix3TC<Coord>   GXAlternatingProduct( const GXVector3TC<Coord>&, const GXVector3TC<Coord>& );
    friend GXMatrix3TC<Coord>   GXAlternatingProduct( const GXCoVector3TC<Coord>&, const GXVector3TC<Coord>& );
    friend GXMatrix3TC<Coord>   GXAlternatingProduct( const GXVector3TC<Coord>&, const GXCoVector3TC<Coord>& );

    // conversion operators - implicit conversions are inappropriate
    friend GXCoVector3TC<Coord> GXVecToCov( const GXVector3TC<Coord>& );
    friend GXVector3TC<Coord>   GXCovToVec( const GXCoVector3TC<Coord>& );
    friend GXVector3TC<Coord>   GXPntToVec( const GXPoint3TC<Coord>& );

public:
    Coord dx, dy, dz;
};


//-----------------------------------------------------------------------------
// Class: GXCoVector3TC
// Description: Row Vector
//-----------------------------------------------------------------------------
template<class Coord>
class GXCoVector3TC
{
public:
                                GXCoVector3TC( ) {}
                                GXCoVector3TC( const Coord& n );
                                GXCoVector3TC( const Coord& nx, const Coord& ny, const Coord& nz );

    UINT                        Dim() const { return 3; }

    // access grants
    Coord&                      operator[] ( UINT i ) { return (&nx)[i]; }
    const Coord&                operator[] ( UINT i ) const { return (&nx)[i]; }

    // assignment operators
    GXCoVector3TC<Coord>&       operator += ( const GXCoVector3TC<Coord>& );
    GXCoVector3TC<Coord>&       operator -= ( const GXCoVector3TC<Coord>& );
    GXCoVector3TC<Coord>&       operator *= ( const Coord& );
    GXCoVector3TC<Coord>&       operator /= ( const Coord& );

    // unary operators
    GXCoVector3TC<Coord>        operator + () const;
    GXCoVector3TC<Coord>        operator - () const;

    // binary operators
    friend GXCoVector3TC<Coord> operator + ( const GXCoVector3TC<Coord>&, const GXCoVector3TC<Coord>& );
    friend GXCoVector3TC<Coord> operator - ( const GXCoVector3TC<Coord>&, const GXCoVector3TC<Coord>& );
    friend GXCoVector3TC<Coord> operator * ( const Coord&, const GXCoVector3TC<Coord>& );
    friend GXCoVector3TC<Coord> operator * ( const GXCoVector3TC<Coord>&, const Coord& );
    friend GXCoVector3TC<Coord> operator / ( const GXCoVector3TC<Coord>&, const Coord& );
    friend bool                 operator == ( const GXCoVector3TC<Coord>&, const GXCoVector3TC<Coord>& );
    friend bool                 operator != ( const GXCoVector3TC<Coord>&, const GXCoVector3TC<Coord>& );

    // misc functions
    friend Coord                GXLengthSq( const GXCoVector3TC<Coord>& );
    friend Coord                GXLength( const GXCoVector3TC<Coord>& );
    friend GXCoVector3TC<Coord> GXUnit( const GXCoVector3TC<Coord>& );
    friend UINT                 GXMaxComponent( const GXCoVector3TC<Coord> &);
    friend Coord                GXMaximumNorm( const GXCoVector3TC<Coord>&); // L-infinity norm
    friend Coord                GXDot( const GXCoVector3TC<Coord>&, const GXCoVector3TC<Coord>& );
    friend Coord                GXDot( const GXVector3TC<Coord>&, const GXCoVector3TC<Coord>& );
    friend Coord                GXDot( const GXCoVector3TC<Coord>&, const GXVector3TC<Coord>& );
    friend GXCoVector3TC<Coord> GXCross( const GXCoVector3TC<Coord>&, const GXCoVector3TC<Coord>& );
    friend GXMatrix3TC<Coord>   GXOuterProduct( const GXCoVector3TC<Coord>&, const GXCoVector3TC<Coord>& );
    friend GXMatrix3TC<Coord>   GXOuterProduct( const GXCoVector3TC<Coord>&, const GXVector3TC<Coord>& );
    friend GXMatrix3TC<Coord>   GXOuterProduct( const GXVector3TC<Coord>&, const GXCoVector3TC<Coord>& );
    friend GXMatrix3TC<Coord>   GXAlternatingProduct( const GXCoVector3TC<Coord>&, const GXCoVector3TC<Coord>& );
    friend GXMatrix3TC<Coord>   GXAlternatingProduct( const GXCoVector3TC<Coord>&, const GXVector3TC<Coord>& );
    friend GXMatrix3TC<Coord>   GXAlternatingProduct( const GXVector3TC<Coord>&, const GXCoVector3TC<Coord>& );

    // conversion operators - implicit conversions are inappropriate
    friend GXCoVector3TC<Coord> GXVecToCov( const GXVector3TC<Coord>& );
    friend GXVector3TC<Coord>   GXCovToVec( const GXCoVector3TC<Coord>& );
    friend GXCoVector3TC<Coord> GXPntToCov( const GXPoint3TC<Coord>& );

public:
    Coord nx, ny, nz;
};


//-----------------------------------------------------------------------------
// Class: GXPoint3TC
//-----------------------------------------------------------------------------
template<class Coord>
class GXPoint3TC
{
public:
                                GXPoint3TC() { }
                                GXPoint3TC( const Coord& x);
                                GXPoint3TC( const Coord& x, const Coord& y, const Coord& z );

    UINT                        Dim() const { return 3; }

    // access grants
    Coord&                      operator[] ( UINT i ) { return (&x)[i]; }
    const Coord&                operator[] ( UINT i ) const { return (&x)[i]; }

    // assignment operators
    GXPoint3TC<Coord>&          operator += ( const GXVector3TC<Coord>& );
    GXPoint3TC<Coord>&          operator -= ( const GXVector3TC<Coord>& );

    // binary operators
    friend GXPoint3TC<Coord>    operator + ( const GXPoint3TC<Coord>&, const GXVector3TC<Coord>& );
    friend GXPoint3TC<Coord>    operator + ( const GXVector3TC<Coord>&, const GXPoint3TC<Coord>& );
    friend GXPoint3TC<Coord>    operator - ( const GXPoint3TC<Coord>&, const GXVector3TC<Coord>& );
    friend GXVector3TC<Coord>   operator - ( const GXPoint3TC<Coord>&, const GXPoint3TC<Coord>& );
    friend bool                 operator == ( const GXPoint3TC<Coord>&, const GXPoint3TC<Coord>& );
    friend bool                 operator != ( const GXPoint3TC<Coord>&, const GXPoint3TC<Coord>& );

    // point dominance
    bool                        operator < ( const GXPoint3TC<Coord>& ) const;
    bool                        operator <= ( const GXPoint3TC<Coord>& ) const;
    bool                        operator > ( const GXPoint3TC<Coord>& ) const;
    bool                        operator >= ( const GXPoint3TC<Coord>& ) const;

    // misc functions
    friend GXPoint3TC<Coord>    GXLerp( const GXPoint3TC<Coord>&, const GXPoint3TC<Coord>&, Coord );
    friend GXPoint3TC<Coord>    GXBarycentric( const GXPoint3TC<Coord>& p, 
                                    const GXPoint3TC<Coord>& q, 
                                    const GXPoint3TC<Coord>& r,
                                    Coord f, Coord g);

    // conversion operators - implicit conversions are inappropriate
    friend GXVector3TC<Coord>   GXPntToVec( const GXPoint3TC<Coord>& );
    friend GXCoVector3TC<Coord> GXPntToCov( const GXPoint3TC<Coord>& );
public:
    Coord x, y, z;
};


//-----------------------------------------------------------------------------
// Class: GXPoint3TC
// Description: 3D linear transform class
//-----------------------------------------------------------------------------
template<class Coord>
class GXMatrix3TC
{
public:
                                GXMatrix3TC() {}
                                GXMatrix3TC( const Coord& s );
                                GXMatrix3TC( const GXVector3TC<Coord>& vCol1, const GXVector3TC<Coord>& vCol2, const GXVector3TC<Coord>& vCol3 );
                                GXMatrix3TC( const GXCoVector3TC<Coord>& cvRow1, const GXCoVector3TC<Coord>& cvRow2, const GXCoVector3TC<Coord>& cvRow3 );

    UINT                        Dim() const { return 3; }

    // access grants
    Coord *                     operator [] ( UINT iRow ) { return mat[iRow]; }
    const Coord *               operator [] ( UINT iRow ) const { return mat[iRow]; }
    Coord&                      operator () ( UINT iRow, UINT iCol ) { return mat[iRow][iCol]; }
    const Coord&                operator () ( UINT iRow, UINT iCol ) const { return mat[iRow][iCol]; }
    GXVector3TC<Coord>          GetColumn( int i ) const { return GXVector3TC<Coord>( mat[0][i], mat[1][i], mat[2][i] ); }
    GXCoVector3TC<Coord>        GetRow( int i ) const { return GXCoVector3TC<Coord>( mat[i][0], mat[i][1], mat[i][2] ); }
    void                        SetColumn( int i, const GXVector3TC<Coord>& v ) { mat[0][i] = v[0];  mat[1][i] = v[1]; mat[2][i] = v[2]; }
    void                        SetRow( int i, const GXCoVector3TC<Coord>& cv ) { mat[i][0] = cv[0];  mat[i][1] = cv[1]; mat[i][2] = cv[2]; }

    // unary operators
    GXMatrix3TC<Coord>          operator + ( );
    GXMatrix3TC<Coord>          operator - ( );

    // binary operators
    friend GXMatrix3TC<Coord>   operator * ( const GXMatrix3TC<Coord>& m1, const GXMatrix3TC<Coord>& m2 );
    friend GXMatrix3TC<Coord>   operator + ( const GXMatrix3TC<Coord>&, const GXMatrix3TC<Coord>& );
    friend GXMatrix3TC<Coord>   operator - ( const GXMatrix3TC<Coord>&, const GXMatrix3TC<Coord>& );
    friend GXMatrix3TC<Coord>   operator * ( const GXMatrix3TC<Coord>&, Coord );
    friend GXMatrix3TC<Coord>   operator * ( Coord, const GXMatrix3TC<Coord>& );
    friend GXMatrix3TC<Coord>   operator / ( const GXMatrix3TC<Coord>&, Coord );
    friend bool                 operator == ( const GXMatrix3TC<Coord>&, const GXMatrix3TC<Coord>& );
    // see below for "operator *" with vectors, covectors, and points

    // assignment operators
    GXMatrix3TC<Coord>&         operator *= ( const GXMatrix3TC<Coord>& );
    GXMatrix3TC<Coord>&         operator += ( const GXMatrix3TC<Coord>& );
    GXMatrix3TC<Coord>&         operator -= ( const GXMatrix3TC<Coord>& );
    GXMatrix3TC<Coord>&         operator *= ( const Coord& );
    GXMatrix3TC<Coord>&         operator /= ( const Coord& );
    void                        SetToIdentity();
    bool                        IsIdentity() const;

    // misc functions
    friend GXMatrix3TC<Coord>   GXTranspose( const GXMatrix3TC<Coord>& mat );

public:
    Coord mat[3][3];
};

// matrix multiply with vectors, covectors, points
template<class mCoord, class vCoord> GXVector3TC<vCoord>     operator * ( const GXMatrix3TC<mCoord>& m, const GXVector3TC<vCoord>& v);
template<class mCoord, class cvCoord> GXCoVector3TC<cvCoord> operator * ( const GXCoVector3TC<cvCoord>& cv, const GXMatrix3TC<mCoord>& m );
template<class mCoord, class pCoord> GXPoint3TC<pCoord>      operator * ( const GXMatrix3TC<mCoord>& m, const GXPoint3TC<pCoord>& p );


//-----------------------------------------------------------------------------
// float-valued points, vectors, covectors
//-----------------------------------------------------------------------------
#include "gxfmath.h"
typedef GXPoint3TC<float> GXPoint3;
typedef GXVector3TC<float> GXVector3;
typedef GXCoVector3TC<float> GXCoVector3;
typedef GXCoVector3TC<float> GXNormal3;
typedef GXMatrix3TC<float> GXMatrix3;


#ifdef _GXDEFINE_DLL_ENTRYPOINTS
extern "C"
{
    //
    // NOTE: - angles are counterclockwise (positive axis out face of clock)
    //       - Euler angles follow the aeronautical "zyx" convention -
    //         Roll first, about axis the object is facing, then
    //         pitch upward, then yaw to face into the new heading
    //
    HRESULT WINAPI GXMatrix3RotationEuler( GXMatrix3 *pmatOut, GXMatrix3 *pmatInvOut, float xRoll, float yPitch, float zYaw );
    HRESULT WINAPI GXMatrix3RotationAxis( GXMatrix3 *pmatOut, GXMatrix3 *pmatInvOut, GXVector3 *pvAxis, float fAngle );
    HRESULT WINAPI GXMatrix3Scale( GXMatrix3 *pmatOut, GXMatrix3 *pmInvOut, float xScale, float yScale, float zScale );
    HRESULT WINAPI GXMatrix3Inverse( GXMatrix3 *pmatOut, GXMatrix3 *pmatIn );
}
#endif  // #ifdef _GXDEFINE_DLL_ENTRYPOINTS

#ifndef FLT_MAX
#include <float.h>
#endif

//-----------------------------------------------------------------------------
// Class: GXRay3
//-----------------------------------------------------------------------------
class GXRay3
{
public:
                                GXRay3() { }
                                GXRay3( const GXPoint3& _pntOrg, const GXVector3& _vDir, 
                                    float _fminT = 0.f, float _fmaxT = FLT_MAX);
    GXPoint3                    Evaluate(float t) const;
public:
    GXPoint3    pntOrg;
    GXVector3   vDir;
    float       fminT, fmaxT;
};


//-----------------------------------------------------------------------------
// Class: GXAffineMap3
// Description:  base struct for affine map implementation - no vtbl, just a 
//      collection of bytes with operators defined on them
//-----------------------------------------------------------------------------
class GXAffineMap3
{
public:
                                GXAffineMap3() { }
                                GXAffineMap3( const GXMatrix3& _mLinear, const GXMatrix3& _mLinearInv,
                                    const GXVector3& _vTranslation);

    // mapping functions
    GXPoint3                    MapPoint(const GXPoint3 &pnt) const;
    GXPoint3                    InvMapPoint(const GXPoint3 &pnt) const;
    GXVector3                   MapVector(const GXVector3 &v) const;
    GXVector3                   InvMapVector(const GXVector3 &v) const;
    GXCoVector3                 MapCoVector(const GXCoVector3 &cv) const;
    GXCoVector3                 InvMapCoVector(const GXCoVector3 &cv) const;
    GXRay3                      MapRay(const GXRay3 &ray) const;
    GXRay3                      InvMapRay(const GXRay3 &ray) const;

    // modification functions
    void                        ApplyTranslation(const GXVector3& vTranslate);
    void                        ApplyRotationMatrix(const GXMatrix3& mat);
    void                        ApplyMatrix(const GXMatrix3& mLinear, const GXMatrix3& mInv);
    void                        ApplyAffine(const GXAffineMap3& mymap);
    void                        LookAt(const GXPoint3 &pntEye, const GXPoint3 &pntInterest, 
                                    const GXVector3 &vUp);
    
    // misc functions
    void                        SetToIdentity();
    bool                        IsIdentity() const;

public:
    GXMatrix3   mLinear;
    GXMatrix3   mLinearInv;
    GXVector3   vTranslation;
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// GXVector3TC Inline Functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// -------------------------------------
// constructors
// -------------------------------------

template<class Coord>
inline
GXVector3TC<Coord>::GXVector3TC( const Coord& _d )
{
    dx = dy = dz = _d;
}

template<class Coord>
inline
GXVector3TC<Coord>::GXVector3TC( const Coord& _dx, const Coord& _dy, const Coord& _dz )
{
    dx = _dx; dy = _dy; dz = _dz;
}

// -------------------------------------
// assignment operators
// -------------------------------------

template<class Coord>
inline GXVector3TC<Coord>&
GXVector3TC<Coord>::operator += ( const GXVector3TC<Coord>& v)
{
    dx += v.dx;
    dy += v.dy;
    dz += v.dz;
    return *this;
}

template<class Coord>
inline GXVector3TC<Coord>&
GXVector3TC<Coord>::operator -= ( const GXVector3TC<Coord>& v )
{
    dx -= v.dx;
    dy -= v.dy;
    dz -= v.dz;
    return *this;
}

template<class Coord>
inline GXVector3TC<Coord>&
GXVector3TC<Coord>::operator *= ( const Coord& s )
{
    dx *= s;
    dy *= s;
    dz *= s;
    return *this;
}

template<class Coord>
inline GXVector3TC<Coord>&
GXVector3TC<Coord>::operator /= ( const Coord& s )
{
    Coord s1 = Coord(1.0f) / s;
    dx *= s1;
    dy *= s1;
    dz *= s1;
    return *this;
}

// -------------------------------------
// unary operators
// -------------------------------------

template<class Coord>
inline GXVector3TC<Coord>
GXVector3TC<Coord>::operator + () const
{
    return *this;
}

template<class Coord>
inline GXVector3TC<Coord>
GXVector3TC<Coord>::operator - () const
{
    return GXVector3TC<Coord>( -dx, -dy, -dz );
}

// -------------------------------------
// binary operators
// -------------------------------------

template<class Coord>
inline GXVector3TC<Coord>
operator + ( const GXVector3TC<Coord>& v1, const GXVector3TC<Coord>& v2 )
{
    return GXVector3TC<Coord>( v1.dx + v2.dx, v1.dy + v2.dy, v1.dz + v2.dz );
}

template<class Coord>
inline GXVector3TC<Coord>
operator - ( const GXVector3TC<Coord>& v1, const GXVector3TC<Coord>& v2 )
{
    return GXVector3TC<Coord>( v1.dx - v2.dx, v1.dy - v2.dy, v1.dz - v2.dz );
}

template<class Coord>
inline GXVector3TC<Coord>
operator * ( const Coord& s, const GXVector3TC<Coord>& v )
{
    return GXVector3TC<Coord>( v.dx * s, v.dy * s, v.dz * s );
}

template<class Coord>
inline GXVector3TC<Coord>
operator * ( const GXVector3TC<Coord>& v, const Coord& s )
{
    return GXVector3TC<Coord>( v.dx * s, v.dy * s, v.dz * s );
}

template<class Coord>
inline GXVector3TC<Coord>
operator / ( const GXVector3TC<Coord>& v, const Coord& s )
{
    Coord s1 = Coord(1.0f) / s;
    return GXVector3TC<Coord>( v.dx * s1, v.dy * s1, v.dz * s1 );
}

template<class Coord>
inline bool
operator == ( const GXVector3TC<Coord>& v1, const GXVector3TC<Coord>& v2 )
{
    return ((v1.dx == v2.dx) && (v1.dy == v2.dy) && (v1.dz == v2.dz));
}

template<class Coord>
inline bool
operator != ( const GXVector3TC<Coord>& v1, const GXVector3TC<Coord>& v2 )
{
    return ! (v1 == v2);
}

// -------------------------------------
// friend functions
// -------------------------------------

template<class Coord>
inline Coord
GXLengthSq( const GXVector3TC<Coord>& v )
{
    return v.dx*v.dx + v.dy*v.dy + v.dz*v.dz;
}

template<class Coord>
inline Coord
GXLength( const GXVector3TC<Coord>& v )
{
    return (Coord) GXSqrt(GXLengthSq(v));
}

template<class Coord>
inline GXVector3TC<Coord>
GXUnit( const GXVector3TC<Coord>& v )
{
    return v * GXInvSqrt(GXLengthSq(v));
}

template<class Coord>
inline UINT
GXMaxComponent(const GXVector3TC<Coord> &v)
{
    UINT i, iMax = 0;

    for (i = 1; i < v.Dim(); i++)
        if (GXFAbs(v[i]) > GXFAbs(v[iMax]))
            iMax = i;
    return iMax;
}

template<class Coord>
inline Coord
GXMaximumNorm(const GXVector3TC<Coord> &v)
{
    return GXFAbs(v[GXMaxComponent(v)]);
}

template<class Coord>
inline Coord
GXDot( const GXVector3TC<Coord>& v1, const GXVector3TC<Coord>& v2 )
{
    return v1.dx * v2.dx + v1.dy * v2.dy + v1.dz * v2.dz;
}

template<class Coord>
inline Coord
GXDot( const GXVector3TC<Coord>& v1, const GXCoVector3TC<Coord>& v2 )
{
    return v1.dx * v2.nx + v1.dy * v2.ny + v1.dz * v2.nz;
}

template<class Coord>
inline Coord
GXDot( const GXCoVector3TC<Coord>& v1, const GXVector3TC<Coord>& v2 )
{
    return v1.nx * v2.dx + v1.ny * v2.dy + v1.nz * v2.dz;
}

template<class Coord>
inline GXVector3TC<Coord>
GXCross( const GXVector3TC<Coord>& v1, const GXVector3TC<Coord>& v2 )
{
    return GXVector3TC<Coord>(v1.dy*v2.dz - v1.dz*v2.dy,
                            v1.dz*v2.dx - v1.dx*v2.dz,
                            v1.dx*v2.dy - v1.dy*v2.dx);
}

template<class Coord>
inline GXMatrix3TC<Coord>
GXOuterProduct( const GXVector3TC<Coord>& v1, const GXVector3TC<Coord>& v2 )
{
    GXMatrix3TC<Coord> ret;
    for (UINT i = 0; i < 3; i++)
        for (UINT j = 0; j < 3; j++)
            ret(i,j)=v1[i]*v2[j];
    return ret;
}

template<class Coord>
inline GXMatrix3TC<Coord>
GXOuterProduct( const GXCoVector3TC<Coord>& v1, const GXVector3TC<Coord>& v2 )
{
    GXMatrix3TC<Coord> ret;
    for (UINT i = 0; i < 3; i++)
        for (UINT j = 0; j < 3; j++)
            ret(i,j)=v1[i]*v2[j];
    return ret;
}

template<class Coord>
inline GXMatrix3TC<Coord>
GXOuterProduct( const GXVector3TC<Coord>& v1, const GXCoVector3TC<Coord>& v2 )
{
    GXMatrix3TC<Coord> ret;
    for (UINT i = 0; i < 3; i++)
        for (UINT j = 0; j < 3; j++)
            ret(i,j)=v1[i]*v2[j];
    return ret;
}

template<class Coord>
inline GXMatrix3TC<Coord>
GXOuterProduct( const GXCoVector3TC<Coord>& v1, const GXCoVector3TC<Coord>& v2 )
{
    GXMatrix3TC<Coord> ret;
    for (UINT i = 0; i < 3; i++)
        for (UINT j = 0; j < 3; j++)
            ret(i,j)=v1[i]*v2[j];
    return ret;
}

template<class Coord>
inline GXMatrix3TC<Coord>
GXAlternatingProduct( const GXVector3TC<Coord>& v1, const GXVector3TC<Coord>& v2 )
{
    GXMatrix3TC<Coord> ret;

    ret(0,0) = ret(1,1) = ret(2,2) = 0.0f;
    ret(1,0) = v1.dy*v2.dx-v1.dx*v2.dy;
    ret(2,0) = v1.dz*v2.dx-v1.dx*v2.dz; ret(2,1) = v1.dz*v2.dy-v1.dy*v2.dz;
    ret(0,1) = -ret(1,0);
    ret(0,2) = -ret(2,0);   ret(1,2) = -ret(2,1);

    return ret;
}

template<class Coord>
inline GXMatrix3TC<Coord>
GXAlternatingProduct( const GXVector3TC<Coord>& v1, const GXCoVector3TC<Coord>& v2 )
{
    GXMatrix3TC<Coord> ret;

    ret(0,0) = ret(1,1) = ret(2,2) = 0.0f;
    ret(1,0) = v1.dy*v2.nx-v1.dx*v2.ny;
    ret(2,0) = v1.dz*v2.nx-v1.dx*v2.nz; ret(2,1) = v1.dz*v2.ny-v1.dy*v2.nz;
    ret(0,1) = -ret(1,0);
    ret(0,2) = -ret(2,0);   ret(1,2) = -ret(2,1);

    return ret;
}

template<class Coord>
inline GXMatrix3TC<Coord>
GXAlternatingProduct( const GXCoVector3TC<Coord>& v1, const GXVector3TC<Coord>& v2 )
{
    GXMatrix3TC<Coord> ret;

    ret(0,0) = ret(1,1) = ret(2,2) = 0.0f;
    ret(1,0) = v1.ny*v2.dx-v1.nx*v2.dy;
    ret(2,0) = v1.nz*v2.dx-v1.nx*v2.dz; ret(2,1) = v1.nz*v2.dy-v1.ny*v2.dz;
    ret(0,1) = -ret(1,0);
    ret(0,2) = -ret(2,0);   ret(1,2) = -ret(2,1);

    return ret;
}

template<class Coord>
inline GXMatrix3TC<Coord>
GXAlternatingProduct( const GXCoVector3TC<Coord>& v1, const GXCoVector3TC<Coord>& v2 )
{
    GXMatrix3TC<Coord> ret;

    ret(0,0) = ret(1,1) = ret(2,2) = 0.0f;
    ret(1,0) = v1.ny*v2.nx-v1.nx*v2.ny;
    ret(2,0) = v1.nz*v2.nx-v1.nx*v2.nz; ret(2,1) = v1.nz*v2.ny-v1.ny*v2.nz;
    ret(0,1) = -ret(1,0);
    ret(0,2) = -ret(2,0);   ret(1,2) = -ret(2,1);

    return ret;
}


template<class Coord>
inline GXCoVector3TC<Coord>
GXVecToCov( const GXVector3TC<Coord>& v )
{
    return GXCoVector3TC<Coord>( v.dx, v.dy, v.dz );
}

template<class Coord>
inline GXVector3TC<Coord>
GXCovToVec( const GXCoVector3TC<Coord>& cv )
{
    return GXVector3TC<Coord>( cv.nx, cv.ny, cv.nz );
}

template<class Coord>
inline GXVector3TC<Coord>
GXPntToVec( const GXPoint3TC<Coord>& pnt )
{
    return GXVector3TC<Coord>( pnt.x, pnt.y, pnt.z );
}

template<class Coord>
inline GXCoVector3TC<Coord>
GXPntToCov( const GXPoint3TC<Coord>& pnt )
{
    return GXCoVector3TC<Coord>( pnt.x, pnt.y, pnt.z );
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/// GXCoVector3TC Inline Functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// -------------------------------------
// constructors
// -------------------------------------

template<class Coord>
inline
GXCoVector3TC<Coord>::GXCoVector3TC( const Coord& _n )
{
    nx = ny = nz = _n;
}

template<class Coord>
inline
GXCoVector3TC<Coord>::GXCoVector3TC( const Coord& _nx, const Coord& _ny, const Coord& _nz )
{
    nx = _nx; ny = _ny; nz = _nz;
}

// -------------------------------------
// assignment operators
// -------------------------------------

template<class Coord>
inline GXCoVector3TC<Coord>&
GXCoVector3TC<Coord>::operator += ( const GXCoVector3TC<Coord>& v)
{
    nx += v.nx;
    ny += v.ny;
    nz += v.nz;
    return *this;
}

template<class Coord>
inline GXCoVector3TC<Coord>&
GXCoVector3TC<Coord>::operator -= ( const GXCoVector3TC<Coord>& v )
{
    nx -= v.nx;
    ny -= v.ny;
    nz -= v.nz;
    return *this;
}

template<class Coord>
inline GXCoVector3TC<Coord>&
GXCoVector3TC<Coord>::operator *= ( const Coord& s )
{
    nx *= s;
    ny *= s;
    nz *= s;
    return *this;
}

template<class Coord>
inline GXCoVector3TC<Coord>&
GXCoVector3TC<Coord>::operator /= ( const Coord& s )
{
    Coord s1 = Coord(1.0f) / s;
    nx *= s1;
    ny *= s1;
    nz *= s1;
    return *this;
}

// -------------------------------------
// unary operators
// -------------------------------------

template<class Coord>
inline GXCoVector3TC<Coord>
GXCoVector3TC<Coord>::operator + () const
{
    return *this;
}

template<class Coord>
inline GXCoVector3TC<Coord>
GXCoVector3TC<Coord>::operator - () const
{
    return GXCoVector3TC<Coord>( -nx, -ny, -nz );
}

// -------------------------------------
// binary operators
// -------------------------------------

template<class Coord>
inline GXCoVector3TC<Coord>
operator + ( const GXCoVector3TC<Coord>& v1, const GXCoVector3TC<Coord>& v2 )
{
    return GXCoVector3TC<Coord>( v1.nx + v2.nx, v1.ny + v2.ny, v1.nz + v2.nz );
}

template<class Coord>
inline GXCoVector3TC<Coord>
operator - ( const GXCoVector3TC<Coord>& v1, const GXCoVector3TC<Coord>& v2 )
{
    return GXCoVector3TC<Coord>( v1.nx - v2.nx, v1.ny - v2.ny, v1.nz - v2.nz );
}

template<class Coord>
inline GXCoVector3TC<Coord>
operator * ( const Coord& s, const GXCoVector3TC<Coord>& v )
{
    return GXCoVector3TC<Coord>( v.nx * s, v.ny * s, v.nz * s );
}

template<class Coord>
inline GXCoVector3TC<Coord>
operator * ( const GXCoVector3TC<Coord>& v, const Coord& s )
{
    return GXCoVector3TC<Coord>( v.nx * s, v.ny * s, v.nz * s );
}

template<class Coord>
inline GXCoVector3TC<Coord>
operator / ( const GXCoVector3TC<Coord>& v, const Coord& s )
{
    Coord s1 = Coord(1.0f) / s;
    return GXCoVector3TC<Coord>( v.nx * s1, v.ny * s1, v.nz * s1 );
}

template<class Coord>
inline bool
operator == ( const GXCoVector3TC<Coord>& v1, const GXCoVector3TC<Coord>& v2 )
{
    return ((v1.nx == v2.nx) && (v1.ny == v2.ny) && (v1.nz == v2.nz));
}

template<class Coord>
inline bool
operator != ( const GXCoVector3TC<Coord>& v1, const GXCoVector3TC<Coord>& v2 )
{
    return ! (v1 == v2);
}

// -------------------------------------
// friend functions
// -------------------------------------

template<class Coord>
inline Coord
GXLengthSq( const GXCoVector3TC<Coord>& v )
{
    return v.nx*v.nx + v.ny*v.ny + v.nz*v.nz;
}

template<class Coord>
inline Coord
GXLength( const GXCoVector3TC<Coord>& v )
{
    return (Coord) GXSqrt(GXLengthSq(v));
}

template<class Coord>
inline GXCoVector3TC<Coord>
GXUnit( const GXCoVector3TC<Coord>& v )
{
    return v * GXInvSqrt(GXLengthSq(v));
}

template<class Coord>
inline UINT
GXMaxComponent(const GXCoVector3TC<Coord> &v)
{
    UINT i, iMax = 0;
    for (i = 1; i < v.  Dim(); i++)
        if (GXFAbs(v[i]) > GXFAbs(v[iMax]))
            iMax = i;
    return iMax;
}

template<class Coord>
inline Coord
GXMaximumNorm(const GXCoVector3TC<Coord> &v)
{
    return GXFAbs(v[GXMaxComponent(v)]);
}

template<class Coord>
inline Coord
GXDot( const GXCoVector3TC<Coord>& v1, const GXCoVector3TC<Coord>& v2 )
{
    return v1.nx * v2.nx + v1.ny * v2.ny + v1.nz * v2.nz;
}

template<class Coord>
inline GXCoVector3TC<Coord>
GXCross( const GXCoVector3TC<Coord>& v1, const GXCoVector3TC<Coord>& v2 )
{
    return GXCoVector3TC<Coord>(v1.ny*v2.nz - v1.nz*v2.ny,
                              v1.nz*v2.nx - v1.nx*v2.nz,
                              v1.nx*v2.ny - v1.ny*v2.nx);
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// GXPoint3TC Inline Functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// -------------------------------------
// constructors
// -------------------------------------

template<class Coord>
inline
GXPoint3TC<Coord>::GXPoint3TC ( const Coord& _x, const Coord& _y, const Coord& _z )
{
    x = _x; y = _y; z = _z;
}

template<class Coord>
inline
GXPoint3TC<Coord>::GXPoint3TC ( const Coord& _x )
{
    x = y = z = _x;
}

// -------------------------------------
// assignment operators
// -------------------------------------

template<class Coord>
inline GXPoint3TC<Coord>&
GXPoint3TC<Coord>::operator += ( const GXVector3TC<Coord>& vDelta )
{
    x += vDelta.dx;
    y += vDelta.dy;
    z += vDelta.dz;
    return *this;
}

template<class Coord>
inline GXPoint3TC<Coord>&
GXPoint3TC<Coord>::operator -= (const GXVector3TC<Coord>& vDelta )
{
    x -= vDelta.dx;
    y -= vDelta.dy;
    z -= vDelta.dz;
    return *this;
}

// -------------------------------------
// binary operators
// -------------------------------------

template<class Coord>
inline GXPoint3TC<Coord>
operator + ( const GXPoint3TC<Coord>& p, const GXVector3TC<Coord>& vDelta )
{
    return GXPoint3TC<Coord>( p.x + vDelta.dx, p.y + vDelta.dy, p.z + vDelta.dz );
}

template<class Coord>
inline GXPoint3TC<Coord>
operator + ( const GXVector3TC<Coord>& vDelta, const GXPoint3TC<Coord>& p )
{
    return GXPoint3TC<Coord>( p.x + vDelta.dx, p.y + vDelta.dy, p.z + vDelta.dz );
}

template<class Coord>
inline GXPoint3TC<Coord>
operator - ( const GXPoint3TC<Coord>& p, const GXVector3TC<Coord>& vDelta )
{
    return GXPoint3TC<Coord>( p.x - vDelta.dx, p.y - vDelta.dy, p.z - vDelta.dz );
}

template<class Coord>
inline GXVector3TC<Coord>
operator - ( const GXPoint3TC<Coord>& p1, const GXPoint3TC<Coord>& p2 )
{
    return GXVector3TC<Coord>( p1.x - p2.x, p1.y - p2.y, p1.z - p2.z );
}

template<class Coord>
inline bool
operator == ( const GXPoint3TC<Coord>& p1, const GXPoint3TC<Coord>& p2 )
{
    return ((p1.x == p2.x) && (p1.y == p2.y) && (p1.z == p2.z));
}

template<class Coord>
inline bool
operator != ( const GXPoint3TC<Coord>& p1, const GXPoint3TC<Coord>& p2 )
{
    return ! (p1 == p2);
}

// -------------------------------------
// point dominance
// -------------------------------------

template<class Coord>
inline bool
GXPoint3TC<Coord>::operator < ( const GXPoint3TC<Coord>& p ) const
{
    return ((x < p.x) && (y < p.y) && (z < p.z));
}

template<class Coord>
inline bool
GXPoint3TC<Coord>::operator <= ( const GXPoint3TC<Coord>& p ) const
{
    return ((x <= p.x) && (y <= p.y) && (z <= p.z));
}

template<class Coord>
inline bool
GXPoint3TC<Coord>::operator > ( const GXPoint3TC<Coord>& p ) const
{
    return ((x > p.x) && (y > p.y) && (z > p.z));
}

template<class Coord>
inline bool
GXPoint3TC<Coord>::operator >= ( const GXPoint3TC<Coord>& p ) const
{
    return ((x >= p.x) && (y >= p.y) && (z >= p.z));
}

// -------------------------------------
// friend functions
// -------------------------------------

template<class Coord>
inline GXPoint3TC<Coord>
GXLerp( const GXPoint3TC<Coord>&p1, const GXPoint3TC<Coord>&p2, Coord s )
{
    GXVector3TC<Coord> p1p2 = p2 - p1;
    return GXPoint3TC<Coord>( p1 + s*p1p2 );
}

template<class Coord>
inline GXPoint3TC<Coord>
GXBarycentric( const GXPoint3TC<Coord>& p, 
              const GXPoint3TC<Coord>& q, 
              const GXPoint3TC<Coord>& r,
              Coord f, Coord g)
{
    return p+f*(q-p)+g*(r-p);
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// GXMatrix3TC Inline Functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// -------------------------------------
// constructors
// -------------------------------------

template<class Coord>
inline
GXMatrix3TC<Coord>::GXMatrix3TC( const Coord& s )
{
    for ( int i = 0; i < 3; i++ )
        for ( int j = 0; j < 3; j++ )
            mat[i][j] = s;
}

template<class Coord>
inline
GXMatrix3TC<Coord>::GXMatrix3TC( const GXVector3TC<Coord>& vCol1,
                                 const GXVector3TC<Coord>& vCol2,
                                 const GXVector3TC<Coord>& vCol3)
{
    mat[0][0] = vCol1.dx; mat[0][1] = vCol2.dx; mat[0][2] = vCol3.dx;
    mat[1][0] = vCol1.dy; mat[1][1] = vCol2.dy; mat[1][2] = vCol3.dy;
    mat[2][0] = vCol1.dz; mat[2][1] = vCol2.dz; mat[2][2] = vCol3.dz;
}

template<class Coord>
inline
GXMatrix3TC<Coord>::GXMatrix3TC( const GXCoVector3TC<Coord>& cvRow1, 
                                 const GXCoVector3TC<Coord>& cvRow2,
                                 const GXCoVector3TC<Coord>& cvRow3 )
{
    mat[0][0] = cvRow1.nx; mat[0][1] = cvRow1.ny; mat[0][2] = cvRow1.nz;
    mat[1][0] = cvRow2.nx; mat[1][1] = cvRow2.ny; mat[1][2] = cvRow2.nz;
    mat[2][0] = cvRow3.nx; mat[2][1] = cvRow3.ny; mat[2][2] = cvRow3.nz;
}

// -------------------------------------
// unary operators
// -------------------------------------

template<class Coord>
inline GXMatrix3TC<Coord>
GXMatrix3TC<Coord>::operator + ( )
{
    return *this;
}

template<class Coord>
inline GXMatrix3TC<Coord>
GXMatrix3TC<Coord>::operator - ( )
{
    GXMatrix3TC<Coord> ret;
    
    for (UINT i = 0; i < 3; i += 1)
        for (UINT j = 0; j < 3; j += 1)
            ret(i, j) = -mat[i][j];
    return ret;
}


// -------------------------------------
// binary operators
// -------------------------------------

template<class Coord>
inline GXMatrix3TC<Coord>
operator + ( const GXMatrix3TC<Coord>& m1, const GXMatrix3TC<Coord>& m2 )
{
    GXMatrix3TC<Coord> ret;

    for (UINT i = 0; i < 3; i += 1)
        for (UINT j = 0; j < 3; j += 1)
            ret(i, j) = m1(i, j) + m2(i, j);
    return ret;
}

template<class Coord>
inline GXMatrix3TC<Coord>
operator - ( const GXMatrix3TC<Coord>& m1, const GXMatrix3TC<Coord>& m2 )
{
    GXMatrix3TC<Coord> ret;

    for (UINT i = 0; i < 3; i += 1)
        for (UINT j = 0; j < 3; j += 1)
            ret(i, j) = m1(i, j) - m2(i, j);
    return ret;
}

template<class Coord>
inline GXMatrix3TC<Coord>
operator * ( const GXMatrix3TC<Coord>& m, Coord s )
{
    GXMatrix3TC<Coord> ret;

    for (UINT i = 0; i < 3; i += 1)
        for (UINT j = 0; j < 3; j += 1)
            ret(i, j) = m(i, j) * s;
    return ret;
}

template<class Coord>
inline GXMatrix3TC<Coord>
operator * ( Coord s, const GXMatrix3TC<Coord>& m )
{
    GXMatrix3TC<Coord> ret;

    for (UINT i = 0; i < 3; i += 1)
        for (UINT j = 0; j < 3; j += 1)
            ret(i, j) = m(i, j) * s;
    return ret;
}


template<class Coord>
inline GXMatrix3TC<Coord> 
operator * ( const GXMatrix3TC<Coord>& m1, const GXMatrix3TC<Coord>& m2 )
{
    GXMatrix3TC<Coord> ret;
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            ret(i, j) = Coord(0.0);
            for (int k=0; k<3; k++) {
                ret(i, j) += m1(i, k) * m2(k, j);
            }
        }
    }
    return ret;
}

template<class Coord>
inline GXMatrix3TC<Coord>
operator / ( const GXMatrix3TC<Coord>& m, Coord s )
{
    GXMatrix3TC<Coord> ret;
    Coord s1 = Coord(1.0f) / s;

    for (UINT i = 0; i < 3; i += 1)
        for (UINT j = 0; j < 3; j += 1)
            ret(i, j) = m(i, j) * s1;
    return ret;
}

template<class mCoord, class vCoord>
inline GXVector3TC<vCoord>
operator * ( const GXMatrix3TC<mCoord>& m, const GXVector3TC<vCoord>& v)
{
    return GXVector3TC<vCoord>(v.dx*m(0,0) + v.dy*m(0,1) + v.dz*m(0,2),
                               v.dx*m(1,0) + v.dy*m(1,1) + v.dz*m(1,2),
                               v.dx*m(2,0) + v.dy*m(2,1) + v.dz*m(2,2)
                           );
}

template<class mCoord, class cvCoord>
inline GXCoVector3TC<cvCoord>
operator * ( const GXCoVector3TC<cvCoord>& cv, const GXMatrix3TC<mCoord>& m )
{
    return GXCoVector3TC<cvCoord>(cv.nx*m(0,0) + cv.ny*m(1,0) + cv.nz*m(2,0),
                                  cv.nx*m(0,1) + cv.ny*m(1,1) + cv.nz*m(2,1),
                                  cv.nx*m(0,2) + cv.ny*m(1,2) + cv.nz*m(2,2)
                           );
}

template<class mCoord, class pCoord>
inline GXPoint3TC<pCoord>
operator * ( const GXMatrix3TC<mCoord>& m, const GXPoint3TC<pCoord>& p )
{
    return GXPoint3TC<pCoord>(p.x*m(0,0) + p.y*m(0,1) + p.z*m(0,2),
                              p.x*m(1,0) + p.y*m(1,1) + p.z*m(1,2),
                              p.x*m(2,0) + p.y*m(2,1) + p.z*m(2,2)
                          );
}

template<class Coord>
inline bool
operator == ( const GXMatrix3TC<Coord>& m1, const GXMatrix3TC<Coord>& m2)
{
    for (UINT i = 0; i < 3; i += 1)
        for (UINT j = 0; j < 3; j += 1)
            if ( m1(i,j) != m2(i,j) )
                return false;
    return true;
}


// -------------------------------------
// assignment operators
// -------------------------------------

template<class Coord>
inline GXMatrix3TC<Coord>&
GXMatrix3TC<Coord>::operator += ( const GXMatrix3TC<Coord>& m )
{
    for (UINT i = 0; i < 3; i += 1)
        for (UINT j = 0; j < 3; j += 1)
            mat[i][j] += m(i, j);
    return *this;
}

template<class Coord>
inline GXMatrix3TC<Coord>&
GXMatrix3TC<Coord>::operator -= ( const GXMatrix3TC<Coord>& m )
{
    for (UINT i = 0; i < 3; i += 1)
        for (UINT j = 0; j < 3; j += 1)
            mat[i][j] -= m(i, j);
    return *this;
}

template<class Coord>
inline GXMatrix3TC<Coord>&
GXMatrix3TC<Coord>::operator *= ( const Coord& s )
{
    for (UINT i = 0; i < 3; i += 1)
        for (UINT j = 0; j < 3; j += 1)
            mat[i][j] *= s;
    return *this;
}

template<class Coord>
inline GXMatrix3TC<Coord>&
GXMatrix3TC<Coord>::operator *= ( const GXMatrix3TC<Coord>& m )
{
    *this = *this * m;
    return *this;
}

template<class Coord>
inline GXMatrix3TC<Coord>&
GXMatrix3TC<Coord>::operator /= ( const Coord& s )
{
    Coord s1 = Coord(1.0f) / s;
    for (UINT i = 0; i < 3; i += 1)
        for (UINT j = 0; j < 3; j += 1)
            mat[i][j] *= s1;
    return *this;
}

// -------------------------------------
// misc functions
// -------------------------------------

template<class Coord>
inline void
GXMatrix3TC<Coord>::SetToIdentity()
{
    for (int i = 0; i < 3; i += 1)
        for (int j = 0; j < 3; j += 1)
            mat[i][j] = ((i==j) ? Coord(1.0) : Coord(0.0));
}

template<class Coord>
inline bool
GXMatrix3TC<Coord>::IsIdentity() const
{
    for (int i = 0; i < 3; i += 1)
        for (int j = 0; j < 3; j += 1)
            if (mat[i][j] != ((i==j) ? Coord(1.0) : Coord(0.0)))
                return false;
    return true;
}

template<class Coord>
inline GXMatrix3TC<Coord>
GXTranspose( const GXMatrix3TC<Coord>& mat )
{
    GXMatrix3TC<Coord> ret;
    for (int i = 0; i < 3; i += 1)
        for (int j = 0; j < 3; j += 1)
            ret(i,j) = mat(j,i);
    return ret;
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// GXRay3 Inline Functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

inline
GXRay3::GXRay3(const GXPoint3& _pntOrg, const GXVector3& _vDir, 
               float _fminT, float _fmaxT)
{ 
    pntOrg = _pntOrg; 
    vDir = _vDir; 
    fminT = _fminT; 
    fmaxT = _fmaxT;
}

inline GXPoint3
GXRay3::Evaluate(float t) const
{ 
    return this->pntOrg + t * this->vDir;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// GXAffineMap3 Inline Functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// -------------------------------------
// constructors
// -------------------------------------

inline
GXAffineMap3::GXAffineMap3(const GXMatrix3& _mLinear, const GXMatrix3& _mLinearInv,
                           const GXVector3& _vTranslation)
{
    mLinear = _mLinear; 
    mLinearInv = _mLinearInv;
    vTranslation = _vTranslation; 
}


// -------------------------------------
// mapping functions
// -------------------------------------

inline GXPoint3
GXAffineMap3::MapPoint(const GXPoint3 &pnt) const
{
    return (mLinear * pnt) + vTranslation;
}

inline GXPoint3
GXAffineMap3::InvMapPoint(const GXPoint3 &pnt) const
{
    return mLinearInv * (pnt - vTranslation);
}

inline GXVector3
GXAffineMap3::MapVector(const GXVector3 &v) const
{
    return mLinear * v;
}

inline GXVector3
GXAffineMap3::InvMapVector(const GXVector3 &v) const
{
    return mLinearInv * v;
}

inline GXCoVector3
GXAffineMap3::MapCoVector(const GXCoVector3 &cv) const
{
    return cv * mLinearInv;
}

inline GXCoVector3
GXAffineMap3::InvMapCoVector(const GXCoVector3 &cv) const
{
    return cv * mLinear;
}

inline GXRay3
GXAffineMap3::MapRay(const GXRay3 &ray) const
{
    GXRay3 rayNew;
    rayNew.pntOrg = this->MapPoint(ray.pntOrg);
    rayNew.vDir = this->MapVector(ray.vDir);
    // N.B. t parameter is invariant under affine transform
    rayNew.fmaxT = ray.fmaxT;
    rayNew.fminT = ray.fminT;
    return rayNew;
}

inline GXRay3
GXAffineMap3::InvMapRay(const GXRay3 &ray) const
{
    GXRay3 rayNew;
    rayNew.pntOrg = this->InvMapPoint(ray.pntOrg);
    rayNew.vDir = this->InvMapVector(ray.vDir);
    // N.B. t parameter is invariant under affine transform
    rayNew.fmaxT = ray.fmaxT;
    rayNew.fminT = ray.fminT;
    return rayNew;
}


// -------------------------------------
// modification functions
// -------------------------------------

inline void
GXAffineMap3::ApplyTranslation(const GXVector3& v)
{
    this->vTranslation += mLinear * v;  // MT*NP = MN,MP+t
}

inline void
GXAffineMap3::ApplyRotationMatrix(const GXMatrix3& m)
{
    GXMatrix3 mLinearTmp = this->mLinear;
    GXMatrix3 mLinearInvTmp = this->mLinearInv;
    for (UINT i = 0; i < 3; i += 1)
    {
        for (UINT j = 0; j < 3; j += 1)
        {
            float fSumMat = 0.0f;
            float fSumInv = 0.0f;
            for (UINT k = 0; k < 3; k += 1)
            {
                fSumMat += mLinearTmp(i, k) * m(k, j);
                fSumInv += m(k, i) * mLinearInvTmp(k, j);
            }
            this->mLinear(i, j) = fSumMat;
            this->mLinearInv(i, j) = fSumInv;
        }
    }
}


inline void
GXAffineMap3::ApplyMatrix(const GXMatrix3& m, const GXMatrix3& mInv)
{
    this->mLinear *= m;
    this->mLinearInv = mInv * this->mLinearInv;
}

// (Mi,Ti) is affine map, (M1,T1)*(M2,T2) = (M1M2,M1T2+T1)
inline void
GXAffineMap3::ApplyAffine(const GXAffineMap3& mymap)
{
    this->vTranslation += this->mLinear * mymap.vTranslation;
    this->mLinearInv = mymap.mLinearInv * this->mLinearInv;
    this->mLinear *= mymap.mLinear;
}


//-----------------------------------------------------------------------------
// Function: LookAt
// Notes:
//     This function applies a transformation that maps the eye point to the
//  origin, looking at the interest point down the negative z axis.  
//  - Notice this is the inverse of the matrix generated by OpenGL's LookAt.
//-----------------------------------------------------------------------------
inline void 
GXAffineMap3::LookAt(const GXPoint3 &pntEye, const GXPoint3 &pntInterest, 
                          const GXVector3 &vUp)
{
    GXVector3 vInterestDir = GXUnit(pntInterest - pntEye);
    GXVector3 vRight = GXCross(vInterestDir, vUp);
    GXVector3 vNewUp = GXUnit(GXCross(vRight, vInterestDir));
    vRight = GXUnit(vRight);
    GXMatrix3 mRot(GXVecToCov(vRight), GXVecToCov(vNewUp), GXVecToCov(-vInterestDir));

    this->ApplyRotationMatrix(mRot);
	this->ApplyTranslation(-GXPntToVec(pntEye));
}


inline void        
GXAffineMap3::SetToIdentity()
{
    this->mLinear.SetToIdentity();
    this->mLinearInv.SetToIdentity();
    this->vTranslation = GXVector3(0.0f);
}

inline bool
GXAffineMap3::IsIdentity() const
{
    return (this->mLinear.IsIdentity() && 
        (this->vTranslation == GXVector3(0.f)));
}

#endif // #ifndef __GXAFFINE3_H

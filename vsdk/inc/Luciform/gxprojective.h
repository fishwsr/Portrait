#pragma once

#ifndef __GXPROJECTIVE_H
#define __GXPROJECTIVE_H

/*==========================================================================;
//
//  Copyright (C) 2000 Microsoft Corporation.  All Rights Reserved.
//
//  File:       GXProjective.h
//  Content:    2D and 3D projective space classes
//                GXHPoint2, GXHPoint3 - 2D and 3D homogeneous points
//                GXHLine3             - Pluecker lines
//
//
//**************************************************************************/


// =========================================================
//
// GXHPoint2TC
//
// =========================================================

template<class Coord>
class GXHPoint2TC
{
public:
                            GXHPoint2TC() { }
                            GXHPoint2TC( const Coord& _u, 
                                const Coord& _v, 
                                const Coord& _w ): u(_u), v(_v), w(_w) { }
                            GXHPoint2TC( const GXPoint2TC<Coord>& p );

    UINT                    Dim() const { return 3; }

    // access grants
    Coord&                  operator[] (UINT i) { return (&u)[i]; }
    const Coord&            operator[] (UINT i) const { return (&u)[i]; }

    // conversion operator
                            operator GXPoint2TC<Coord> ();
public:
    Coord u, v, w;
};

// matrix multiply with projective point
template<class mCoord, class pCoord> GXHPoint2TC<pCoord> 
    operator * ( const GXMatrix3TC<mCoord>& m, const GXHPoint2TC<pCoord>& p );


// =========================================================
//
// GXHPoint3TC
//
// =========================================================

template<class Coord>
class GXHPoint3TC
{
public:
                            GXHPoint3TC() { }
                            GXHPoint3TC( const Coord& _x, 
                                const Coord& _y, 
                                const Coord& _z, 
                                const Coord& _w );
                            GXHPoint3TC( const GXPoint3TC<Coord>& p );

    UINT                    Dim() const { return 4; }

    // access grants
    Coord&                  operator[] (UINT i) { return (&x)[i]; }
    const Coord&            operator[] (UINT i) const { return (&x)[i]; }

    // conversion operator
                            operator GXPoint3TC<Coord> ();
public:
    Coord x, y, z, w;
};

// matrix multiply with projective point
template<class mCoord, class pCoord> GXHPoint3TC<pCoord>
    operator * ( const GXMatrix4TC<mCoord>& m, const GXHPoint3TC<pCoord>& p );


// =========================================================
//
// GXHPoint3TC
//
// =========================================================

template<class Coord>
class GXHPlane3TC
{
public:
                            GXHPlane3TC() { }
                            GXHPlane3TC( const Coord& _x, 
                                const Coord& _y, 
                                const Coord& _z, 
                                const Coord& _w );
                            GXHPlane3TC( const GXPoint3TC<Coord>& p1,
                                const GXPoint3TC<Coord>& p2,
                                const GXPoint3TC<Coord>& p3);

    UINT                    Dim() const { return 4; }

    // access grants
    Coord&                  operator[] (UINT i) { return (&x)[i]; }
    const Coord&            operator[] (UINT i) const { return (&x)[i]; }

    // friend functions
    friend Coord            GXDot( const GXHPoint3TC<Coord>&, const GXHPlane3TC<Coord>& );
    friend Coord            GXDot( const GXHPlane3TC<Coord>&, const GXHPoint3TC<Coord>& );

public:
    Coord x, y, z, w;
};


//-----------------------------------------------------------------------------
// float-valued points, vectors, covectors
//-----------------------------------------------------------------------------
typedef GXHPoint2TC<float> GXHPoint2;
typedef GXHPoint3TC<float> GXHPoint3;
typedef GXHPlane3TC<float> GXHPlane3;


// =========================================================
//
// GXHLine3: Pluecker line class
//
// =========================================================

class GXHLine3
{
public:
                            GXHLine3() { }
                            GXHLine3( const GXHPoint3&, const GXHPoint3& );
                            GXHLine3( const GXHPlane3&, const GXHPlane3& );

    friend float            GXSignedDistance( const GXHLine3&, const GXHLine3& );

public:
    float pl[6];
};


// =========================================================
// inline function implementations
// =========================================================

template<class Coord>
inline
GXHPoint2TC<Coord>::GXHPoint2TC( const GXPoint2TC<Coord>& p )
{
    u = p.u; v = p.v; w = 1.0f;
}

template<class Coord>
inline
GXHPoint2TC<Coord>::operator GXPoint2TC<Coord> ( )
{
    return GXPoint2TC<Coord>( u / w, v / w );
}

template<class mCoord, class pCoord>
inline GXHPoint2TC<pCoord>
operator * ( const GXMatrix3TC<mCoord>& m, const GXHPoint2TC<pCoord>& p )
{
    return GXHPoint2TC<pCoord>(p.u*m(0,0) + p.v*m(0,1) + p.w*m(0,2),
                              p.u*m(1,0) + p.v*m(1,1) + p.w*m(1,2),
                              p.u*m(2,0) + p.v*m(2,1) + p.w*m(2,2)
                          );
}

template<class Coord>
inline
GXHPoint3TC<Coord>::GXHPoint3TC( const Coord& _x, const Coord& _y,
                             const Coord& _z, const Coord& _w)
{
    x = _x; y = _y; z = _z; w = _w;
}

template<class Coord>
inline
GXHPoint3TC<Coord>::GXHPoint3TC( const GXPoint3TC<Coord>& p )
{
    x = p.x; y = p.y; z = p.z; w = 1.0f;
}

template<class Coord>
inline
GXHPoint3TC<Coord>::operator GXPoint3TC<Coord> ( )
{
    return GXPoint3TC<Coord>( x / w, y / w, z / w );
}

template<class mCoord, class pCoord>
inline GXHPoint3TC<pCoord>
operator * ( const GXMatrix4TC<mCoord>& m, const GXHPoint3TC<pCoord>& p )
{
    return GXHPoint3TC<pCoord>(p.x*m(0,0) + p.y*m(0,1) + p.z*m(0,2) + p.w*m(0,3),
                               p.x*m(1,0) + p.y*m(1,1) + p.z*m(1,2) + p.w*m(1,3),
                               p.x*m(2,0) + p.y*m(2,1) + p.z*m(2,2) + p.w*m(2,3),
                               p.x*m(3,0) + p.y*m(3,1) + p.z*m(3,2) + p.w*m(3,3)
                          );
}

template<class Coord>
inline
GXHPlane3TC<Coord>::GXHPlane3TC( const Coord& _x, const Coord& _y,
                             const Coord& _z, const Coord& _w)
{
    x = _x; y = _y; z = _z; w = _w;
}

template<class Coord>
inline
GXHPlane3TC<Coord>::GXHPlane3TC( const GXPoint3TC<Coord>& p1,
                             const GXPoint3TC<Coord>& p2,
                             const GXPoint3TC<Coord>& p3)
{
    GXVector3 v2 = p2 - p1;
    GXVector3 v3 = p3 - p1;
    x = v2.dy*v3.dz-v2.dz*v3.dy;
    y = v2.dz*v3.dx-v2.dx*v3.dz;
    z = v2.dx*v3.dy-v2.dy*v3.dx;
    w = -p1.x*x - p1.y*y - p1.z*z;
}

template<class Coord>
inline Coord
GXDot( const GXHPoint3TC<Coord>& p1, const GXHPlane3TC<Coord>& p2 )
{
    return p1.x*p2.x + p1.y*p2.y + p1.z*p2.z + p1.w*p2.w;
}

template<class Coord>
inline Coord
GXDot( const GXHPlane3TC<Coord>& p1, const GXHPoint3TC<Coord>& p2 )
{
    return p1.x*p2.x + p1.y*p2.y + p1.z*p2.z + p1.w*p2.w;
}

inline
GXHLine3::GXHLine3( const GXHPoint3& p, const GXHPoint3& q )
{
    pl[0] = p.x*q.y - q.x*p.y;
    pl[1] = p.x*q.z - q.x*p.z;
    pl[2] = p.x*q.w - q.x*p.w;
    pl[3] = p.y*q.z - q.y*p.z;
    pl[4] = p.z*q.w - q.z*p.w;
    pl[5] = q.y*q.w - p.y*p.w;
}

inline
GXHLine3::GXHLine3( const GXHPlane3& p, const GXHPlane3& q )
{
    pl[0] = p.x*q.y - q.x*p.y;
    pl[1] = p.x*q.z - q.x*p.z;
    pl[2] = p.x*q.w - q.x*p.w;
    pl[3] = p.y*q.z - q.y*p.z;
    pl[4] = p.z*q.w - q.z*p.w;
    pl[5] = q.y*q.w - p.y*p.w;
}

inline
float
GXSignedDistance( const GXHLine3& a, const GXHLine3& b )
{
    return  a.pl[0]*b.pl[4] +
            a.pl[1]*b.pl[5] +
            a.pl[2]*b.pl[3] +
            a.pl[3]*b.pl[0] +
            a.pl[4]*b.pl[1] +
            a.pl[5]*b.pl[2];
}

#endif


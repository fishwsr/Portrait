// VisLuciform.h
//
// Copyright © 2000 Microsoft Corporation, All Rights Reserved
//
// This file includes the Luciform project header files used by the Vision SDK.

#pragma once


// The current version of the Luciform header files are from version 006
// (around April 1999).


// We would like to include just include gxmathcore here, but that would
// require us to add the Luciform directory to the include files path,
// and we don't want to do that yet.
//#include "Luciform\gxmathcore.h"

// For now we use relative paths to include the files that gxmathcore.h
// would include (except grrandom.h and dxcolor.h).  It appears that these
// files include inline definitions for all functions (and methods) that
// they declare, so we don't need to worry about linking to the Luciform
// libraries when using these files.

// GXCommon has typdefs for INT8, UINT8, INT16,..., UINT64
#include "Luciform\GXCommon.h"

// GXConstants defines 128-bit constants like GX_PI, GX_SQRT2, and GX_E
#include "Luciform\GXConstants.h"

#include "Luciform\GXAffine2.h"
#include "Luciform\GXAffine3.h"
#include "Luciform\GXAffine4.h"
#include "Luciform\GXProjective.h"
#include "Luciform\GXQuaternion.h"

// We can't include gxrandom.h or dxcolor.h without adding the Luciform
// include file directory to the include files path (and possibly linking
// with the Luciform LIBs), so we don't include these files.
//#include "Luciform\gxrandom.h"
//#include "Luciform\gxcolor.h"

#include "VisLineSegment.h"



// 2D projective line
// UNDONE:  Move this to VisLuciformWrappers.h
template<class Coord>
class VisHLine2TC 
{
public:
	VisHLine2TC() { }
	VisHLine2TC(const Coord &A, const Coord &B, const Coord &C): a(A), b(B), c(C) { }
	VisHLine2TC(const GXHPoint2TC<Coord> &p1, const GXHPoint2TC<Coord> &p2);

    UINT Dim() const { return 3; }
    Coord& operator[] (UINT i) { return (&a)[i]; }
    const Coord& operator[] (UINT i) const { return (&a)[i]; }


public:
	Coord a, b, c;
};

template<class Coord>
inline VisHLine2TC<Coord>::VisHLine2TC(const GXHPoint2TC<Coord> &p1,
		const GXHPoint2TC<Coord> &p2)
{
	a = p1.v*p2.w - p1.w*p2.v;
    b = p1.w*p2.u - p1..u*p2.w;
    c = p1.u*p2.v - p1.v*p2.u;
}

// one may want to add a function to compute the intersection of two lines


typedef VisHLine2TC<float> VisHLine2F;


// VisSDK wrappers for these classes are defined in the VisMatrix project.

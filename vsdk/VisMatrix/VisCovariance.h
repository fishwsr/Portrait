///////////////////////////////////////////////////////////////////////////
// @doc INTERNAL EXTERNAL VISCOVARIANCE
// @module VisCovariance.h |
//
//  Some miscellaneous functions regarding covariance matrix
//
// created on Jan 19, 1999, by zhang.
//
// Copyright © 1999 - 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////

#ifndef __VIS_MATRIX_COVARIANCE_H__
#define __VIS_MATRIX_COVARIANCE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "VisMatrixProj.h"

#ifndef VisMatrixExport
#define VisMatrixExport __declspec(dllimport)
#endif // VisMatrixExport


// These methods use the Luciform classes.
// UNDONE:  Should these work with feature classes, which include covariance?

#ifndef VIS_NO_LUC_FILES

// normalize a vector and 
// compute the corresponding covariance matrix if 'covV' is not a dumb matrix
VisMatrixExport void VisNormalizeVectorAndCovariance(
	const CVisVector2F & v,  // original vector
	const CVisMatrix2F &covV,
	CVisVector2F & u, // normalized vector (unit vector)
	CVisMatrix2F & covU
	);

VisMatrixExport void VisNormalizeVectorAndCovariance(
	const CVisVector3F & v,  // original vector
	const CVisMatrix3F &covV,
	CVisVector3F & u, // normalized vector (unit vector)
	CVisMatrix3F & covU
	);

VisMatrixExport void VisNormalizeVectorAndCovariance(
	const CVisVector4F & v,  // original vector
	const CVisMatrix4F &covV,
	CVisVector4F & u, // normalized vector (unit vector)
	CVisMatrix4F & covU
	);

// compute the spherical coordinates of a 3-vector and 
// their covariance matrix if 'covV' is not a dumb matrix
VisMatrixExport void Vis3VectorToSphericalCoordinates(
	const CVisVector3F & v,  // original vector
	const CVisMatrix3F & covV,
	CVisVector2F & spherical, // 2D vector (phi, theta)
	CVisMatrix2F & covSpherical
	);

// compute the spherical coordinates of a 3-vector and 
// their covariance matrix if 'covSpherical' is not a dumb matrix
VisMatrixExport void VisSphericalCoordinatesTo3Vector(
	const CVisDVector & spherical, // 2D vector (phi, theta)
	const CVisMatrix2F & covSpherical,
	CVisDVector & u,  // unit 3-vector
	CVisDMatrix & covU
	);

// merge 2 sets of spherical coordinates
VisMatrixExport void VisMergeSphericalCoordinates(
	const CVisVector2F & sph1, // 2D vector (phi, theta)
	const CVisMatrix2F & covSph1,
	const CVisVector2F & sph2, // 2D vector (phi, theta)
	const CVisMatrix2F & covSph2,
	CVisVector2F & sphMerged, // 2D vector (phi, theta)
	CVisMatrix2F & covSphMerged,
	const bool fHemisphere = false // set to 1 if they represent UNDIRECTED vectors
	);

#endif // VIS_NO_LUC_FILES



// These are the old methods, that use CVisDVector and CVisDMatrix.
// (We shouldn't use those classes when we know that we're working
// with 2D, 3D, or 4D vectors and matrices.)


// normalize a vector and 
// compute the corresponding covariance matrix if 'covV' is not a dumb matrix
VisMatrixExport void VisNormalizeVectorAndCovariance(
	const CVisDVector & v,  // original vector
	const CVisDMatrix &covV,
	CVisDVector & u, // normalized vector (unit vector)
	CVisDMatrix & covU
	);

// compute the spherical coordinates of a 3-vector and 
// their covariance matrix if 'covV' is not a dumb matrix
VisMatrixExport void Vis3VectorToSphericalCoordinates(
	const CVisDVector & v,  // original vector
	const CVisDMatrix & covV,
	CVisDVector & spherical, // 2D vector (phi, theta)
	CVisDMatrix & covSpherical
	);

// compute the spherical coordinates of a 3-vector and 
// their covariance matrix if 'covSpherical' is not a dumb matrix
VisMatrixExport void VisSphericalCoordinatesTo3Vector(
	const CVisDVector & spherical, // 2D vector (phi, theta)
	const CVisDMatrix & covSpherical,
	CVisDVector & u,  // unit 3-vector
	CVisDMatrix & covU
	);

// merge 2 sets of spherical coordinates
VisMatrixExport void VisMergeSphericalCoordinates(
	const CVisDVector & sph1, // 2D vector (phi, theta)
	const CVisDMatrix & covSph1,
	const CVisDVector & sph2, // 2D vector (phi, theta)
	const CVisDMatrix & covSph2,
	CVisDVector & sphMerged, // 2D vector (phi, theta)
	CVisDMatrix & covSphMerged,
	const int hemisphere = 0 // set to 1 if they represent UNDIRECTED vectors
	);

#endif // __VIS_MATRIX_COVARIANCE_H__

// VisLuciformTypedefs.h
//
// Typedefs for common types (derived from the Luciform types)
//
// Copyright © 2000 Microsoft Corporation, All Rights Reserved

#pragma once



typedef CVisPoint2TC<float> CVisPoint2F;
typedef CVisVector2TC<float> CVisVector2F;
typedef CVisCoVector2TC<float> CVisCoVector2F;
typedef CVisMatrix2TC<float> CVisMatrix2F;

typedef CVisPoint3TC<float> CVisPoint3F;
typedef CVisVector3TC<float> CVisVector3F;
typedef CVisCoVector3TC<float> CVisCoVector3F;
typedef CVisMatrix3TC<float> CVisMatrix3F;

typedef CVisPoint4TC<float> CVisPoint4F;
typedef CVisVector4TC<float> CVisVector4F;
typedef CVisCoVector4TC<float> CVisCoVector4F;
typedef CVisMatrix4TC<float> CVisMatrix4F;

typedef CVisHPoint2TC<float> CVisHPoint2F;
typedef CVisHPoint3TC<float> CVisHPoint3F;
typedef CVisHPlane3TC<float> CVisHPlane3F;

// Luciform also includes typedefs similar to these.
typedef CVisCoVector3TC<float> CVisNormal3F;
typedef CVisVector3TC<double> CVisVector3D;


// Our line-segment classes.
typedef CVisLineSegment<CVisPoint2F> CVisLineSegment2F;
typedef CVisLineSegment<CVisPoint3F> CVisLineSegment3F;
typedef CVisLineSegment<CVisPoint4F> CVisLineSegment4F;
typedef CVisLineSegment<CVisHPoint2F> CVisHLineSegment2F;
typedef CVisLineSegment<CVisHPoint3F> CVisHLineSegment3F;


// These typedefs use our CVisMatrixChain class
typedef CVisMatrixChain<CVisMatrix2F> CVisMatrixChain2F;
typedef CVisMatrixChain<CVisMatrix3F> CVisMatrixChain3F;
typedef CVisMatrixChain<CVisMatrix4F> CVisMatrixChain4F;


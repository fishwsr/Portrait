#pragma once

#ifndef _GXDLLMATHCORE_H
#define _GXDLLMATHCORE_H

////////////////////////////////////////////////////////////////////////////////
//
// File: GXDLLMathCore.h
//
// Copyright (C) 2000 Microsoft Corporation.  All Rights Reserved.
//
// Description:
//     This class the DLL entry points for the mathcore.
//
//
////////////////////////////////////////////////////////////////////////////////

#include <windef.h>
#include <GXMathCore.h>

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// GXAffine2.h
////////////////////////////////////////////////////////////////////////////////
HRESULT WINAPI GXMatrix2Identity( GXMatrix2 *pOut );
BOOL    WINAPI GXMatrix2IsIdentity(const GXMatrix2 &m);
HRESULT WINAPI GXMatVec2Multiply( GXVector2 *pOut, const GXMatrix2& m, const GXVector2& v );
HRESULT WINAPI GXMatPt2Multiply( GXPoint2 *pOut, const GXMatrix2& m, const GXPoint2& p );
HRESULT WINAPI GXCovMat2Multiply( GXCoVector2 *pOut, const GXCoVector2& cv, const GXMatrix2& m );
HRESULT WINAPI GXMatrix2Multiply( GXMatrix2 *pOut, const GXMatrix2& m1, const GXMatrix2& m2 );
HRESULT WINAPI GXMatrix2Inverse( GXMatrix2 *pOut, float *pfDeterminant, const GXMatrix2& m );
HRESULT WINAPI GXMatrix2Rotation( GXMatrix2 *pOut, GXMatrix2 *pInv, float fAngle );
HRESULT WINAPI GXMatrix2Scale( GXMatrix2 *pOut, GXMatrix2 *pInv, float fScaleU, float fScaleV );

////////////////////////////////////////////////////////////////////////////////
// GXAffine3.h
////////////////////////////////////////////////////////////////////////////////
HRESULT WINAPI GXMatrix3Identity( GXMatrix3 *pOut );
BOOL    WINAPI GXMatrix3IsIdentity(const GXMatrix3 &m);
HRESULT WINAPI GXMatVec3Multiply( GXVector3 *pOut, const GXMatrix3& m, const GXVector3& v );
HRESULT WINAPI GXMatPt3Multiply( GXPoint3 *pOut, const GXMatrix3& m, const GXPoint3& p );
HRESULT WINAPI GXCovMat3Multiply( GXCoVector3 *pOut, const GXCoVector3& cv, const GXMatrix3& m );
HRESULT WINAPI GXMatrix3Multiply( GXMatrix3 *pOut, const GXMatrix3& m1, const GXMatrix3& m2 );
HRESULT WINAPI GXMatrix3Inverse( GXMatrix3 *pOut, float *pfDeterminant, const GXMatrix3& m );
HRESULT WINAPI GXMatrix3RotationAxis( GXMatrix3 *pOut, GXMatrix3 *pInverse, const GXVector3& vAxis, float fAngle );
HRESULT WINAPI GXMatrix3RotationEuler( GXMatrix3 *pOut, GXMatrix3 *pInverse, float xRoll, float yPitch, float zYaw );
HRESULT WINAPI GXMatrix3RotationQuaternion( GXMatrix3 *pOut, GXMatrix3 *pInv, const GXQuaternion& q );
HRESULT WINAPI GXMatrix3Scale( GXMatrix3 *pOut, GXMatrix3 *pInverse, float xScale, float yScale, float zScale );
HRESULT WINAPI GXMatrix3ScaleDir( GXMatrix3 *pOut, GXMatrix3 *pInverse, const GXVector3& vDir, float fScale );

HRESULT WINAPI GXAffineMap3DIdentity( GXAffineMap3 *pMap );
BOOL    WINAPI GXAffineMap3DIsIdentity( const GXAffineMap3 &map );
HRESULT WINAPI GXAffineMap3DApplyTranslation( GXAffineMap3 *pMap, const GXVector3& v );
HRESULT WINAPI GXAffineMap3DApplyRotationMatrix( GXAffineMap3 *pMap, const GXMatrix3& m );
HRESULT WINAPI GXAffineMap3DApplyMatrix( GXAffineMap3 *pMap, const GXMatrix3& mLinear, const GXMatrix3& mInv );
HRESULT WINAPI GXAffineMap3DApplyAffine( GXAffineMap3 *pOut, const GXAffineMap3& map );
HRESULT WINAPI GXAffineMap3DLookAt(GXAffineMap3 *pMap, const GXPoint3 &pntEye, const GXPoint3 &pntInterest, const GXVector3 &vUp);

////////////////////////////////////////////////////////////////////////////////
// GXAffine4.h
////////////////////////////////////////////////////////////////////////////////
HRESULT WINAPI GXMatrix4Identity( GXMatrix4 *pOut );
BOOL    WINAPI GXMatrix4IsIdentity(const GXMatrix4 &m);
HRESULT WINAPI GXMatrix4Multiply( GXMatrix4 *pOut, const GXMatrix4& m1, const GXMatrix4& m2 );
HRESULT WINAPI GXMatrix4Inverse( GXMatrix4 *pOut, float *pfDeterminant, const GXMatrix4& m );


////////////////////////////////////////////////////////////////////////////////
// GXQuaternion.h
////////////////////////////////////////////////////////////////////////////////
//NOTE: angles are counterclockwise (positive axis out face of clock)
//      Euler angles follow the aeronautical "zyx" convention
extern HRESULT WINAPI GXQuaternionFromEulerAngles( GXQuaternion *pOut, float xRoll, float yPitch, float zYaw );
extern HRESULT WINAPI GXQuaternionFromAxisAngle( GXQuaternion *pOut, const GXVector3&, float );
extern HRESULT WINAPI GXQuaternionSlerp( GXQuaternion *pqBetween, const GXQuaternion& qStart, const GXQuaternion& qStop, float t );
extern HRESULT WINAPI GXQuaternionMultiply( GXQuaternion *pOut, const GXQuaternion& a, const GXQuaternion& b );
// convert quaternion to Matrix3
extern HRESULT WINAPI GXMatrix3RotationQuaternion( GXMatrix3 *pmatOut, GXMatrix3 *pmatInv, const GXQuaternion& );

#ifdef __cplusplus
}
#endif

#endif // #ifndef _GXDLLMATHCORE_H


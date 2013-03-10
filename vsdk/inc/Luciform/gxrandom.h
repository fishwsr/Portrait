#pragma once

#ifndef __GXRANDOM_H
#define __GXRANDOM_H

/*/////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 1998 - 2000 Microsoft Corporation.  All Rights Reserved.
//
// File: gxrandom.h
// Description:
//     This header file defines strong random number generators, suitable for
//  high-dimensional Monte Carlo work.
//
//
/////////////////////////////////////////////////////////////////////////////*/

#include <gxcommon.h>
#include <gxaffine2.h>
#include <gxaffine3.h>
#include <gxaffine4.h>
#include <gxquaternion.h>

#ifdef __cplusplus
extern "C" {
#endif
extern  VOID    WINAPI  GXInitializeRandom(INT nSeed);
extern  INT     WINAPI  GXRandomInt31();
extern  INT     WINAPI  GXRandomLessThanN(INT n);
extern  INT     WINAPI  GXRandomGeometrical(FLOAT p);
extern  INT     WINAPI  GXRandomBinomial(FLOAT p, INT nTrials);
extern  INT     WINAPI  GXRandomPoisson(FLOAT mu);

extern  DOUBLE  WINAPI  GXRandomDouble();
extern  FLOAT   WINAPI  GXRandomFloat();
extern  FLOAT   WINAPI  GXRandomNormal(FLOAT mean, FLOAT sigma);
extern  FLOAT   WINAPI  GXRandomExponential(FLOAT mu);
extern  FLOAT   WINAPI  GXRandomGamma(FLOAT a);
extern  FLOAT   WINAPI  GXRandomBeta(FLOAT a, FLOAT b);
extern  FLOAT   WINAPI  GXRandomChi(FLOAT nu);
extern  FLOAT   WINAPI  GXRandomFDistribution(FLOAT nu1, FLOAT nu2);
extern  FLOAT   WINAPI  GXRandomtDistribution(FLOAT nu);

extern  VOID    WINAPI  GXRandomDisk(GXPoint2 *ppnt);
extern  VOID    WINAPI  GXRandomPoint2(GXPoint2 *ppnt);
extern  VOID    WINAPI  GXRandomPoint3(GXPoint3 *ppnt);
extern  VOID    WINAPI  GXRandomUnitVector2(GXVector2 *pvec);
extern  VOID    WINAPI  GXRandomUnitVector3(GXVector3 *pvec);
extern  VOID    WINAPI  GXRandomUnitVector4(GXVector4 *pvec);
extern  VOID    WINAPI  GXRandomQuaternion(GXQuaternion *pquat);

extern  VOID    WINAPI  GXSquareToDisk(GXPoint2 *ppntDst, GXPoint2 *ppntSrc);
extern  VOID    WINAPI  GXIntervalToUnitVector2(GXVector2 *pvecDst, float f);
extern  VOID    WINAPI  GXSquareToUnitVector3(GXVector3 *pvecDst, GXPoint2 *ppntSquare);
extern  VOID    WINAPI  GXCubeToUnitVector4(GXVector4 *pvecDst, GXPoint3 *ppntCube);
extern  VOID    WINAPI  GXSquareToHemisphere(GXVector3 *pvecDst, GXPoint2 *ppntSquare);
extern  VOID    WINAPI  GXSquareToCosineWeightedHemisphere(GXVector3 *pvecDst, GXPoint2 *ppntSquare);
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
//
// C++ overloads
//
extern  GXPoint2        GXRandomDisk();
extern  GXPoint2        GXRandomPoint2();
extern  GXPoint3        GXRandomPoint3();
extern  GXVector2       GXRandomUnitVector2();
extern  GXVector3       GXRandomUnitVector3();
extern  GXVector4       GXRandomUnitVector4();
extern  GXQuaternion    GXRandomQuaternion();
extern  GXPoint2        GXSquareToDisk(const GXPoint2 &pntSquare);
extern  GXVector2       GXIntervalToUnitVector2(float f);
extern  GXVector3       GXSquareToUnitVector3(const GXPoint2 &pntSquare);
extern  GXVector4       GXCubeToUnitVector4(const GXPoint3 &pntCube);
extern  GXVector3       GXSquareToHemisphere(const GXPoint2 &pntSquare);
extern  GXVector3       GXSquareToCosineWeightedHemisphere(const GXPoint2 &pntSquare);


///////////////////////////////////////
// Inline Definitions for C++ Overloads
///////////////////////////////////////
inline GXPoint2
GXRandomDisk()
{
    GXPoint2 pnt;
    ::GXRandomDisk(&pnt);
    return pnt;
}

inline GXPoint2
GXRandomPoint2()
{
    GXPoint2 pnt;
    ::GXRandomPoint2(&pnt);
    return pnt;
}

inline GXPoint3        
GXRandomPoint3()
{
    GXPoint3 pnt;
    ::GXRandomPoint3(&pnt);
    return pnt;
}

inline GXVector2       
GXRandomUnitVector2()
{
    GXVector2 v;
    ::GXRandomUnitVector2(&v);
    return v;
}

inline GXVector3       
GXRandomUnitVector3()
{
    GXVector3 v;
    ::GXRandomUnitVector3(&v);
    return v;
}

inline GXVector4       
GXRandomUnitVector4()
{
    GXVector4 v;
    ::GXRandomUnitVector4(&v);
    return v;
}

inline GXQuaternion    
GXRandomQuaternion()
{
    GXQuaternion q;
    ::GXRandomQuaternion(&q);
    return q;
}

inline GXPoint2        
GXSquareToDisk(const GXPoint2 &pntSquare)
{
    GXPoint2 pnt;
    ::GXSquareToDisk(&pnt, (GXPoint2 *)&pntSquare);
    return pnt;
}

inline GXVector2       
GXIntervalToUnitVector2(float f)
{
    GXVector2 v;
    ::GXIntervalToUnitVector2(&v, f);
    return v;
}

inline GXVector3       
GXSquareToUnitVector3(const GXPoint2 &pntSquare)
{
    GXVector3 v;
    ::GXSquareToUnitVector3(&v, (GXPoint2 *)&pntSquare);
    return v;
}

inline GXVector4       
GXCubeToUnitVector4(const GXPoint3 &pntCube)
{
    GXVector4 v;
    ::GXCubeToUnitVector4(&v, (GXPoint3 *)&pntCube);
    return v;
}

inline GXVector3       
GXSquareToHemisphere(const GXPoint2 &pntSquare)
{
    GXVector3 v;
    ::GXSquareToHemisphere(&v, (GXPoint2 *)&pntSquare);
    return v;
}

inline GXVector3       
GXSquareToCosineWeightedHemisphere(const GXPoint2 &pntSquare)
{
    GXVector3 v;
    ::GXSquareToCosineWeightedHemisphere(&v, (GXPoint2 *)&pntSquare);
    return v;
}

#endif  // #ifdef __cplusplus

#endif  // #ifndef __GXRANDOM_H

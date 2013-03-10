#pragma once

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the GRAPHMATCH_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// GRAPHMATCH_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
//#ifdef IMGPROC_EXPORTS
#define IMGPROC_API __declspec(dllexport)
//#else
//#define IMGPROC_API __declspec(dllimport)

//#ifdef _DEBUG
//#pragma comment(lib, "ImgProcDB.lib")
//#else
//#pragma comment(lib, "ImgProc.lib")
//#endif // _DEBUG
//#endif // IMGPROC_EXPORTS

#include "GVisSupport.h"

class IMGPROC_API CGVisTPS
{
public:
	CGVisTPS(void); 
	virtual ~CGVisTPS(void);

protected:
	CGVisDVector    m_vecCtrlX;			// source control points
	CGVisDVector    m_vecCtrlY;			// source control points

	CGVisDVector	m_vecWeightX;		// for x coordinate	
	CGVisDVector	m_vecWeightY;		// for y coordinate
	CGVisDVector	m_vecCoefX;			// for x coordinate
	CGVisDVector	m_vecCoefY;			// for y coordinate

private:
	CGVisDVector UFun( const CGVisDVector & vecSqrDist ) const;
	double UFun( double vecSqrDist ) const;

public:
	double Create( CGVisDVector vecSrcX, CGVisDVector vecSrcY,
				   CGVisDVector vecTarX, CGVisDVector vecTarY,
				   double dLamda4Deform, bool bCompEnergy = false );

	double Create2( CGVisDVector vecSrcX, CGVisDVector vecSrcY,
					CGVisDVector vecTarX, CGVisDVector vecTarY,
					double dLamda4Deform, double dLambda4Affine, bool bCompEnergy = false );

	//Added by zhenyu, 2006-09-05
	// For regularized TPS, regularization term is computed outside and input into
	// the CreatSimple function directly
	double CreateSimple( CGVisDVector vecSrcX, CGVisDVector vecSrcY,
						 CGVisDVector vecTarX, CGVisDVector vecTarY,
						 double dRegularize, bool bCompEnergy = false );

	void   GetParameter(CGVisDVector& vecCtrlX, CGVisDVector& vecCtrlY, CGVisDVector& vecWeightX,	
						CGVisDVector& vecWeightY, CGVisDVector& vecCoefX, CGVisDVector&	vecCoefY);
	//End of zhenyu

	double ComputeUMatrix( const CGVisDVector & vecPosX, const CGVisDVector & vecPosY, CGVisDMatrix & matU );

	void Transform( const CGVisDVector & vecSrcX, const CGVisDVector & vecSrcY, 
					CGVisDVector & vecTarX, CGVisDVector & vecTarY ) const;
	void Transform( double dSrcX, double dSrcY, double & dTarX, double & dTarY ) const;
};

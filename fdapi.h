#ifndef __FACEDECTALL__H__
#define __FACEDECTALL__H__

#include "PreDefine.h"

//////////////////////自己添加部分////////////////////////////
//////////////////////自己添加部分////////////////////////////

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FACEDECTALL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FACEDECTALL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef FACEDECTALL_EXPORTS
#define FACEDECTALL_API __declspec(dllexport)
#else
#define FACEDECTALL_API __declspec(dllimport)
#endif
//
////////////////////////自己添加部分////////////////////////////


////////////////////////自己添加部分////////////////////////////

// This class is exported from the FaceDectAll.dll
class FACEDECTALL_API CFaceDectAll {
public:
	CFaceDectAll(void);
	// TODO: add your methods here.
};

extern FACEDECTALL_API int nFaceDectAll;

//////////////////////自己添加部分////////////////////////////

FACEDECTALL_API	bool FDGetAlgID(char * lpszID, int & nSize);
FACEDECTALL_API	int FDGetVersion (void);
FACEDECTALL_API	bool FDInitialize(const char* lpszFile/*=NULL*/);
FACEDECTALL_API	bool FDDetectFaces(const unsigned char * lpImageData, int nImageWidth, int nImageHeigh,  int nBitsPerPixel, int & nFace);
FACEDECTALL_API	bool FDGetFaces (FACEINFO * lpFaces, int & nFace);
FACEDECTALL_API	FaceAlgError FDGetLastError (void);
FACEDECTALL_API	bool FDClose (void);
FACEDECTALL_API	bool FDSetSpeed(const double lfSpeed=0);
FACEDECTALL_API	bool FDSetFalseAlarmRate (const double lfFAR, double & lfDR);
FACEDECTALL_API	bool FDSetDetectionRate (const double lfDR, double & lfFAR);
FACEDECTALL_API	bool FDGetStatistics (const LandmarkNames enumLandmark, FDSTATISTICS & Statistics);

//////////////////////自己添加部分////////////////////////////
#endif
//////////////////////自己添加部分////////////////////////////

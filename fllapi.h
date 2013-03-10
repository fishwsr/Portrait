#if !defined( _JDL_FLL_NZH_ )
#define _JDL_FLL_NZH_

#include "PreDefine.h"

enum FLLMode
{
	modeEyes = 0x0001, // 定位瞳孔
	modeNose = 0x0002, // 定位鼻子
	modeMouth = 0x0004 // 定位嘴
};



extern "C" __declspec(dllexport) bool FLLGetAlgID(char * lpszID, int & nSize);
extern "C" __declspec(dllexport) int  FLLGetVersion (void);
extern "C" __declspec(dllexport) bool FLLValidateFD (const char * lpszFDAlgID, int nFDVersion);
extern "C" __declspec(dllexport) bool FLLInitialize (const char * lpszFile = NULL, FLLMode nModeFlags = modeEyes);
extern "C" __declspec(dllexport) bool FLLSetAccuracy (double lfAccuracy, double & lfSpeed);
extern "C" __declspec(dllexport) bool FLLSetSpeed (double lfSpeed, double & lfAccuracy);
extern "C" __declspec(dllexport) bool FLLLocateLandmarks(const BYTE * lpImageData, int nImageWidth, int nImageHeight, int nBitsPerPixel, FACEINFO FaceInfo, int & nLandmark);
extern "C" __declspec(dllexport) bool FLLGetLandmarks (LANDMARKINFO * lpLandmarks, int & nLandmark);
extern "C" __declspec(dllexport) bool FLLClose (void);
extern "C" __declspec(dllexport) FaceAlgError FLLGetLastError (void);

#endif // !defined(_JDL_FLL_NZH_)

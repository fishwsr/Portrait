#pragma warning(disable : 4275)

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CSBASE_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CSBASE_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef CSBASE_EXPORTS
    #define CSBASE_API __declspec(dllexport)
#else
    #define CSBASE_API __declspec(dllimport)

	#ifdef _DEBUG
		#pragma comment(lib, "./Lib/CSBaseD.lib")
	#else
		#pragma comment(lib, "./Lib/CSBase.lib")
	#endif // _DEBUG
#endif // CSBASE_EXPORTS

#include <vector>
#include <time.h>

#include "VisMatrix.h"
#include "VisCore.h"
#include "VisImageProc.h"
//#include "VisDisplay.h"

#ifndef ULONG_PTR
#define ULONG_PTR ULONG *
#endif // ULONG_PTR

#ifndef LONG_PTR
#define LONG_PTR LONG *
#endif // LONG_PTR

#ifndef DWORD_PTR
#define DWORD_PTR DWORD *
#endif DWORD_PTR  // DWORD_PTR

#ifndef PUNICODE_STRING
#define PUNICODE_STRING WCHAR *
#endif // PUNICODE_STRING

#include "gdiplus.h"
#pragma comment(lib, "gdiplus.lib")

#ifndef CSBASE_EXPORT

#include "Point2.h"
#include "GeomTransform.h"
#include "CubicBezierSpline.h"
#include "SketchDefs.h"
#include "GeomTransforms.h"
#include "SketchVIR.h"

#include "Shape.h"
#include "DataFile.h"
#include "Geometry.h"
#include "Histogram.h"
#include "MatrixMultiband.h"

#include "Sketch.h"
#include "ConnectedModel.h"
#include "IndexedContour.h"
#include "VisFilterBank.h"
#include "VisAddin.h"
#include "IPLImage.h"
#include "VisImageIp.h"
#include "VisImageLinearIp.h"

#include "Defs.h"

#endif

//#include "./BaseModel/BaseModel.h"
//#include "GifEncoder.h"

void CSBASE_API TakeABreath();

int CSBASE_API VisDVectorMinIndex(const CVisDVector& data);
double CSBASE_API VisDVectorMinData(const CVisDVector& data);
int CSBASE_API VisDVectorMaxIndex(const CVisDVector& data);
double CSBASE_API VisDVectorMaxData(const CVisDVector& data);

double CSBASE_API VisDVectorMin(const CVisDVector& data, int& index);
double CSBASE_API VisDVectorMax(const CVisDVector& data, int& index);

double CSBASE_API VisDMatrixMin(CVisDMatrix& data, int& iRow, int& iCol);
double CSBASE_API VisDMatrixMax(CVisDMatrix& data, int& iRow, int& iCol);

double CSBASE_API VisDVectorSum(const CVisDVector& data);
double CSBASE_API VisDVectorSumL1(const CVisDVector& data);
double CSBASE_API VisDVectorSumL2(const CVisDVector& data);

double CSBASE_API VisDMatrixSum(CVisDMatrix& data);
double CSBASE_API VisDMatrixSumL1(CVisDMatrix& data);
double CSBASE_API VisDMatrixSumL2(CVisDMatrix& data);

CVisDVector CSBASE_API VisDMatrixSumRow(const CVisDMatrix& data); // sum over each row
CVisDVector CSBASE_API VisDMatrixSumColumn(const CVisDMatrix& data); // sum over each column

CVisDMatrix CSBASE_API VisDMatrixEntryProd(CVisDMatrix& refMatrixA, CVisDMatrix& refMatrixB);
CVisDMatrix CSBASE_API VisDMatrixEntryDiv(CVisDMatrix& refMatrixA, CVisDMatrix& refMatrixB);
CVisDMatrix CSBASE_API VisDMatrixEntryRecip(CVisDMatrix& refMatrix);

CVisDVector CSBASE_API VisDVectorEntryProd(const CVisDVector& refVectorA, const CVisDVector& refVectorB);
CVisDVector CSBASE_API VisDVectorEntryDiv(const CVisDVector& refVectorA, const CVisDVector& refVectorB);
CVisDVector CSBASE_API VisDVectorEntryRecip(const CVisDVector& refVector);

void CSBASE_API VisImage2Vector(const CVisGrayByteImage& img, CVisDVector& data);
void CSBASE_API VisVector2Image(const CVisDVector& data, CVisGrayByteImage& img, int width, int height);
void CSBASE_API VisImage2Vector(const CVisGrayFloatImage& img, CVisDVector& data);
void CSBASE_API VisVector2Image(const CVisDVector& data, CVisGrayFloatImage& img, int width, int height);
void CSBASE_API VisGeomTransBoundary(const CVisTransform4x4& trans, const RECT& inR, RECT& outR);

CSBASE_API COutputDataFile&  operator<<(COutputDataFile& ofs, const std::vector<CVisDVector*>& vpVector);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, std::vector<CVisDVector*>& vpVector);


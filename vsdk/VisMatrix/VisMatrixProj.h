// VisMatrixProj.h
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved


#ifndef __VIS_MATRIX_MATRIXDLL_H__
#define __VIS_MATRIX_MATRIXDLL_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifdef VIS_MATRIX_DLL
	#define VIS_MATRIX_BUILD
#endif // VIS_MATRIX_DLL

#ifdef VIS_SDK_LIBS
#define VIS_MATRIX_LIB
#else // VIS_SDK_LIBS
	#ifndef VIS_MATRIX_DLL
		#define VIS_MATRIX_DLL
	#endif // !VIS_MATRIX_DLL
#endif // VIS_SDK_LIBS

#include "VisMatrixVersion.h"

//#include "..\VisCore\VisCoreDLL.h"
#include "..\inc\VisCore.h"


#ifdef VIS_MATRIX_LIB
#define VisMatrixExport			
#define VisMatrixExportDeclspec	;
#elif defined (VIS_MATRIX_BUILD)
#define VisMatrixExport			_declspec(dllexport)
#define VisMatrixExportDeclspec	_declspec(dllexport)
#else // VIS_MATRIX_LIB
#define VisMatrixExport			_declspec(dllimport)
#define VisMatrixExportDeclspec	_declspec(dllimport)
#endif // VIS_MATRIX_LIB

#ifndef VIS_NO_LUC_FILES
	#include "VisLuciformWrappers.h"
	#include "VisLuciformTypedefs.h"
	#include "VisLuciformWrappers.inl"
#endif // VIS_NO_LUC_FILES


#include "VisDMatrix.h"
#include "VisTransform4x4.h"
#include "VisCovariance.h"




#endif // __VIS_MATRIX_MATRIXDLL_H__

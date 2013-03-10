

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ASMMODEL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ASMMODEL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef ASMMODEL_EXPORTS
    #define ASMMODEL_API __declspec(dllexport)
#else
    #define ASMMODEL_API __declspec(dllimport)

	#ifdef _DEBUG
		#pragma comment(lib, "./Lib/ASMModelD.lib")
	#else
		#pragma comment(lib, "./Lib/ASMModel.lib")
	#endif // _DEBUG
#endif // ASMMODEL_EXPORTS

#include <vector>
#include "Viscore.h"
#include "VisMatrix.h"
#include "VisImageProc.h"
#include "./BaseModel/BaseModel.h"

#include "ASMDll.h"

#ifndef ASMMODEL_EXPORTS

#include "ASMSearchOption.h"
#include "ASMML2DSearchModel.h"
#include "ASMMLSearchModelBayesian.h"

#include "LocalPatchModel.h"
#include "GenerativeShapeModel.h"

#endif


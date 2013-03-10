
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the BASEMODEL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// BASEMODEL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef BASEMODEL_EXPORTS
    #define BASEMODEL_API __declspec(dllexport)
#else
    #define BASEMODEL_API __declspec(dllimport)

	#ifdef _DEBUG
		#pragma comment(lib, "./Lib/BaseModelD.lib")
	#else
		#pragma comment(lib, "./Lib/BaseModel.lib")
	#endif // _DEBUG
#endif // BASEMODEL_EXPORTS

#include <vector>
#include "VisMatrix.h"
#include "VisCore.h"
#include "VisImageProc.h"

#include "./CSBase/CSBase.h"

#ifndef BASEMODEL_EXPORT

#include "PCAModel.h"
#include "PCA2Model.h"

#include "Myrandom.h"
#include "Distribution.h"
#include "DistributionMatrix.h"
#include "GaussianModel.h"
#include "MixtureGaussian.h"
#include "PPCAModel.h"
#include "MixturePPCA.h"
#include "MixtureGaussianKernel.h"
#include "ShapeModel.h"
#include "MixturedShapeModel.h"
#include "GibbsModel.h"
#include "NMFModel.h"

#endif



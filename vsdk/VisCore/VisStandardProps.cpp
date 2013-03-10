// VisStandardProps.cpp
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved

// UNDONE:  Move the daclarations of these proerties to VisStandardProps.h.


// This should be the first preprocessor statement in the file so that
// automatic precompiled headers work correctly.
#ifdef VIS_CORE_BUILD
	#include "VisCorePch.h"
	#include "VisCoreProj.h"
#else
	#include "VisPropListProj.h"
#endif


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG

// Helper function definitions for standard types.
VIS_DEFINE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, bool)
VIS_DEFINE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, char)
VIS_DEFINE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, signed char)
VIS_DEFINE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, unsigned char)
VIS_DEFINE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, short)
VIS_DEFINE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, unsigned short)
VIS_DEFINE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, int)
VIS_DEFINE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, unsigned int)
VIS_DEFINE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, long)
VIS_DEFINE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, unsigned long)
VIS_DEFINE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, __int64)
VIS_DEFINE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, unsigned __int64)
VIS_DEFINE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, float)
VIS_DEFINE_PROP_LIST_SIMPLE_CLASS(VisPropListExportDeclspec, double)

VIS_DEFINE_PROP_LIST_WO_STRING_CLASS(VisPropListExportDeclspec, char *)
VIS_DEFINE_PROP_LIST_WO_STRING_CLASS(VisPropListExportDeclspec, char const *)
VIS_DEFINE_PROP_LIST_STRING_CLASS(VisPropListExportDeclspec, std::string)

VIS_DEFINE_PROP_LIST_CLASS_VECTOR(VisPropListExportDeclspec, std::string)

VIS_DEFINE_PROP_LIST_CLASS_NO_IO(VisPropListExportDeclspec, void *)
VIS_DEFINE_PROP_LIST_CLASS_NO_IO(VisPropListExportDeclspec, void const *)


VIS_DEFINE_SD_CLASS_WITH_IO_PL_TYPEDEF(VisPropListExportDeclspec, POINT)
VisPropListExport void VisBuildObjPropList(POINT& refpoint,
		CVisPropList& refproplist)
{
	assert(refproplist.IsEmpty());
	// Note that we use names that will be ordered correctly.
	refproplist.AddPropReference("x", refpoint.x, false);
	refproplist.AddPropReference("y", refpoint.y, false);
}

VIS_DEFINE_SD_CLASS_WITH_IO_PL_TYPEDEF(VisPropListExportDeclspec, SIZE)
VisPropListExport void VisBuildObjPropList(SIZE& refsize,
		CVisPropList& refproplist)
{
	assert(refproplist.IsEmpty());
	// Note that we use names that will be ordered correctly.
	refproplist.AddPropReference("cx", refsize.cx, false);
	refproplist.AddPropReference("cy", refsize.cy, false);
}

VIS_DEFINE_SD_CLASS_WITH_IO_PL_TYPEDEF(VisPropListExportDeclspec, RECT)
VisPropListExport void VisBuildObjPropList(RECT& refrect,
		CVisPropList& refproplist)
{
	assert(refproplist.IsEmpty());

#ifdef OLD_CODE
	// Note that we use names that will be ordered correctly.
	// (And that CVisShape's NBands member will be odered after these names.)
	refproplist.AddPropReference("Ax", refrect.left, false);
	refproplist.AddPropReference("Ay", refrect.top, false);
	refproplist.AddPropReference("Bx", refrect.right, false);
	refproplist.AddPropReference("By", refrect.bottom, false);
#endif // OLD_CODE

	// With the fOrdered option, we can use the Windows names and still get
	// the right order.
	refproplist.AddPropReference("left", refrect.left, false);
	refproplist.AddPropReference("top", refrect.top, false);
	refproplist.AddPropReference("right", refrect.right, false);
	refproplist.AddPropReference("bottom", refrect.bottom, false);
}





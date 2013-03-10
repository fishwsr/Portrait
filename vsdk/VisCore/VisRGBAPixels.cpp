// VisRGBAPixels.cpp
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#include "VisCorePch.h"
#include "VisCoreProj.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG


VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisRGBA<BYTE> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisRGBA<BYTE> >(0, fAlwaysTypedef); }

VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisRGBA<int> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisRGBA<int> >(0, fAlwaysTypedef); }

VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisRGBA<float> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisRGBA<float> >(0, fAlwaysTypedef); }

VisCoreExport CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisRGBA<double> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisRGBA<double> >(0, fAlwaysTypedef); }


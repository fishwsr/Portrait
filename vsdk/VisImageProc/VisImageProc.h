// VisImageProc.h
//
// Copyright © 1999 - 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////


#include <math.h>
#include <memory.h>
#include <stdlib.h>


//#include "..\VisCore\VisCoreDLL.h"
//#include "..\inc\VisCore.h"
#include "VisImageProcVersion.h"
//#include "..\VisMatrix\VisMatrixDLL.h"
#include "..\inc\VisMatrix.h"


#include "VisFrame.h"

#include "VisProcMisc.h"
#include "VisRGBA.h"
#include "VisResample.h"
#include "VisConvolve.h"
#include "VisPyramid.h"
#include "VisPolygonRender.h"

#include "VisLineExtract.h"
#include "VisRegionExtract.h"

#include "VisMedian.h"
#include "VisBlend.h"
#include "VisHistogramEq.h"

#include "VisCollection.h"

#include "VisSprites.h"

#include "VisAlign.h"
#include "VisComposite.h"
#include "VisCylindrical.h"  // LATER:  Change file name?

#include "VisFreeRotation.h"
#include "VisBlockAdjust.h"
#include "VisColorAdjust.h"
#include "VisWarp.h"

// Moved "VisTrack.h", "VisLayer.h" and "VisLayeredComposite.h" 
// to VisSfM project (7/9/87 Qifa Ke and 10/26/98 Rick Szeliski)

#include "VisStereo.h"

// These files contain the connected region code
#include "VisObjList.h"
#include "VisPixRun.h"
#include "VisStrip.h"
#include "VisThresholdFnOps.h"
#include "VisBlob.h"

// This file contains a simple histogram class
#include "VisHistogram.h"

// This file contains a class that can be used to extract a rectangular
// grid from an image.
#include "VisRectangularGrid.h"

#include "VisBlendedSequence.h"

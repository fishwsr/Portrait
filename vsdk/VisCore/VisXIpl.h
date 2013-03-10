// VisXIpl.h
//
// Functions to convert between Vision SDK images and the IplImages
// used in Intel's Image Processing Library.
//
// Warning:  These functions have not been thoroughly tested.
//
// Copyright © 2000 Microsoft Corporation.  All Rights Reserved


#pragma once




#ifdef VIS_IPL_IPL_H
	// Include the IPL header file if VisLocalInfo.bat found it.
	#include VIS_IPL_IPL_H
#else // VIS_IPL_IPL_H
	// Forward declarations (for Intel's IPL structures).
	struct _IplImage;
	struct _IplROI;
	struct _IplTileInfo;
#endif // VIS_IPL_IPL_H


// Create an IplImage from a CVisImage.  It will use the same memory
// as the CVisImage if fCopyMemory is false (the default).
VisCoreExport _IplImage *VisIplCreateImage(CVisImageBase& refimage,
		bool fCopyMemory = false);

// Delete an IplImage that was created from a CVisImage.
VisCoreExport void VisIplDeleteImage(_IplImage *piplimage);


// Helper function to convert between Vision SDK padding enum (EVisPad)
// and the constants used in the IPL.
VisCoreExport int VisIplBorderModeFromPad(EVisPad evispad);


// These functions are used internally to get a reference to a CVisImage
// whose pointer is storedin a IplImage structure.  Use the CVisImageBase
// Alias method instead of these functions to get a CVisImage from an IplImage.
const CVisImageBase& VisImageFromIplImage(const _IplImage *piplimage);

CVisImageBase& VisImageFromIplImage(_IplImage *piplimage);

#include "VisXIpl.inl"


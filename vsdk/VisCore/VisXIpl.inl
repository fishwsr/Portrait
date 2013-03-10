// VisXIpl.inl
//
// Functions to convert between Vision SDK images and the IplImages
// used in Intel's Image Processing Library.
//
// Warning:  These functions have not been thoroughly tested.
//
// Copyright © 2000 Microsoft Corporation.  All Rights Reserved

inline const CVisImageBase& VisImageFromIplImage(const _IplImage *piplimage)
{
	assert(piplimage != 0);
#ifdef VIS_IPL_IPL_H
	return *((const CVisImageBase *) piplimage->imageId);
#else // VIS_IPL_IPL_H
	// UNDONE:  Throw an exception?
	assert(0);
	return *((const CVisImageBase *) 0);
#endif // VIS_IPL_IPL_H
}

inline CVisImageBase& VisImageFromIplImage(_IplImage *piplimage)
{
	assert(piplimage != 0);
#ifdef VIS_IPL_IPL_H
	return *((CVisImageBase *) piplimage->imageId);
#else // VIS_IPL_IPL_H
	// UNDONE:  Throw an exception?
	assert(0);
	return *((CVisImageBase *) 0);
#endif // VIS_IPL_IPL_H
}

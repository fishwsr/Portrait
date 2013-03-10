///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISENUMPIXEL
//
// @module VisEnumPixel.h |
//
// Objects used to enumerate pixels in images.
//
// <nl>
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @todo:(INTERNAL)
//
// Templated each pixel methods
//
// May need to addref both image memory and image structure if
// image memory can be changed.  (Or just take rect and memory from
// image, since image parameters could change.)
//
// Add a parameter to specify the rect to be enumerated.
//
// Add an enum to specify how to get Border pixels (from image or padding).
//
// Add methods to get padding settings and determine whether any copying
// needs to be done to enumerate the specified neighborhoods.
//
// Add variables to get cPixelRowCur and cPixelColCur directly.
//
// Enumerators optimized for all bands (by using ++) when type is known
// may be faster than general enumerator that adds cbPix or cbCol.
//
// Multi-image enumerators and operators.  (Like the VisMap functions.)
//
// Methods to skip pixels (possibly stopping at the start of the next
// row) and skip rows might be nice.
//
// Probably also want copy ctor, default ctor, and assignment operator
// for CVisEnumPixelOf.
//
// <nl>
//
// @xref <l VisEnumPixel\.inl> <l VisEnumPixel\.cpp>
//
// <nl>
// @index | VISENUMPIXEL
//
///////////////////////////////////////////////////////////////////////////


#ifndef __VIS_CORE_ENUMPIXEL_H__
#define __VIS_CORE_ENUMPIXEL_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "VisImageBase.h"


// Forward declarations.
template<class TPixel>
class CVisEnumPixelOf;


// @type CVisEnumBytePixel | CVisEnumPixelOf\<unsigned char>
typedef CVisEnumPixelOf<BYTE> CVisEnumBytePixel;

// @type CVisEnumColorBytePixel | CVisEnumPixelOf\<CVisRGBABytePixel>
typedef CVisEnumPixelOf<CVisRGBABytePixel> CVisEnumColorBytePixel;


////////////////////////////////////////////////////////////////////////////
//  
// @class
//
// This object will enumerate the pixels in (all bands or a specified
// band of) a rectangle in an image, from the top
// row to the bottom row and from the leftmost column to
// the rightmost column in each row.
//
// @xref <c CVisEnumPixelOf>
//
////////////////////////////////////////////////////////////////////////////
class CVisEnumPixel
{
// @access Public Members
public:
	// @cmember
	// Constructor.
	VisCoreExport CVisEnumPixel(const CVisImageBase& image, int band = -1,
			int dxBorder = 0, int dyBorder = 0,
			EVisPad pad = evispadNone);

	// @cmember
	// Constructor.
	VisCoreExport CVisEnumPixel(const CVisImageBase& image, const RECT& rect,
			int band = -1, int dxBorder = 0, int dyBorder = 0,
			EVisPad pad = evispadNone);

	// @cmember
	// Constructor.
	VisCoreExport CVisEnumPixel(const CVisImageBase& image, int xLeft, int yTop,
			int xRight, int yBottom, int band = -1,
			int dxBorder = 0, int dyBorder = 0,
			EVisPad pad = evispadNone);

	// @cmember
	// Copy constructor.
	VisCoreExport CVisEnumPixel(const CVisEnumPixel& refenumpixel);

	// @cmember
	// Default constructor.
	VisCoreExport CVisEnumPixel(void);

	// @cmember
	// Assignment operator.
	VisCoreExport CVisEnumPixel& operator=(const CVisEnumPixel& refenumpixel);

	// @cmember
	// Destructor.
	VisCoreExport ~CVisEnumPixel(void);

	// @cmember
	// Get a pointer to the first byte of the current pixel.
	inline BYTE *PbPixelCur(void) const;

	// @cmember
	// Get the number of bytes in each pixel.
	inline int CbPixel(void) const;

	// @cmember
	// Get the number of bytes between adjacent columns in the same row.
	inline int CbColCur(void) const;

	// @cmember
	// Get the number of bytes between adjacent rows in the same column.
	inline int CbRowCur(void) const;

	// @cmember
	// Get the number of pixels between adjacent columns in the same row.
	// (This may be equal to one or the number of bands in the image.)
	inline int CPixelColCur(void) const;

	// @cmember
	// Get the number of pixels between adjacent rows in the same column.
	inline int CPixelRowCur(void) const;

	// @cmember
	// Are there more pixels (in addition to the current pixel)
	// to be enumerated.
	inline BOOL FMore(void) const;

	// @cmember
	// Reset the enumerator.  If the image contains pixels, this will
	// make the left-top pixel the current pixel in the enumerator.
	inline BOOL Reset(void);

	// @cmember
	// Advance the current pixel.  If there are no more pixels to be
	// enumerated, the current pixel will not be valid after this method
	// is called.
	inline void Next(void);

	// @cmember
	// Return a reference to a copy of the image being enumerated.
	// (The address of this image will be different to the address
	// of the image given to the constructor of this object.)
	inline CVisImageBase& Image(void) const;

#ifdef LATER
	// LATER:  Templated function operator methods?

	template<class TPixOp>
		void ApplyPixelOperator(void);

	template<class TNbhdOp>
		void ApplyNeighborhoodOperator(void);

	// LATER:  Additional function operator methods (eg. for two images)?
#endif // LATER



	// Iterator comparison operators
	bool operator==(const CVisEnumPixel& refenumpixel) const;
	bool operator!=(const CVisEnumPixel& refenumpixel) const;


	// Helper method used to find the end of an enumeration.
	void GoToEnd(void);



// @access:(IMPL) Protected Members
protected:
	typedef void (__fastcall *VisPfnNextSubsection)(class CVisEnumPixel *);


	BYTE *m_pbCur;
	CVisImageBase *m_pImage;
	CVisRect m_rect;
	int m_dpbCur;
	int m_dxBorder;
	int m_dyBorder;
	int m_band;
	int m_cbPixelCur;
	int m_cbColCur;
	int m_cbRowCur;

	// The difference between the address of the first pixel enumerated
	// in a row and the address of the limit pixel in a row.
	int m_cbColEnumCur;
	BYTE *m_pbRowLim;
	BYTE *m_pbSubimageLim;
	VisPfnNextSubsection m_pfnNextSubsection;

	// LATER:  Temp memory for subimage when enumerations neighborhoods.
	// (Also, need methods to setup subimage memory and copy pixels to
	// subimage memory.)

	VisCoreExport void StartCenter(void);

#ifdef LATER
	inline void StartTop(void);

	inline void StartBottom(void);

	inline void StartLeft(void);

	inline void StartRight(void);


	// LATER:  Helper method used with iterator methods in the derived class.
	// inline void Prev(void);


	// Static methods assigned to m_pfnNextSubsection.
	static VisCoreExport void __fastcall StartBottomSubsection(
			CVisEnumPixel *pEnumPixel);

	static VisCoreExport void __fastcall StartLeftSubsection(
			CVisEnumPixel *pEnumPixel);

	static VisCoreExport void __fastcall StartRightSubsection(
			CVisEnumPixel *pEnumPixel);

	static VisCoreExport void __fastcall StartCenterSubsection(
			CVisEnumPixel *pEnumPixel);
#endif // LATER

	static VisCoreExport void __fastcall FinishedWithAllSubsections(
			CVisEnumPixel *pEnumPixel);
};


////////////////////////////////////////////////////////////////////////////
//  
// @class
//
// This object will enumerate the pixels in (all bands or a specified
// band of) a rectangle in an image, from the top
// row to the bottom row and from the leftmost column to
// the rightmost column in each row.  This class knows the type of
// the pixels being enumerated, so its methods can return pointers
// and references to objects of the pixel type.
//
// @tcarg class | TPixel |
// Type of the pixels in the image being enumerated.
//
// @base public | CVisEnumPixel
//
// @xref <c CVisEnumPixel>
//
////////////////////////////////////////////////////////////////////////////
template<class TPixel>
class CVisEnumPixelOf : public CVisEnumPixel
{
// @access Public Members
public:
	// @cmember
	// Constructor.
	CVisEnumPixelOf(const CVisImageBase& refImage, int band = -1,
			int dxBorder = 0, int dyBorder = 0,
			EVisPad pad = evispadNone);

	// @cmember
	// Constructor.
	CVisEnumPixelOf(const CVisImageBase& refImage, const RECT& rect,
			int band = -1, int dxBorder = 0, int dyBorder = 0,
			EVisPad pad = evispadNone);

	// @cmember
	// Copy constructor.
	CVisEnumPixelOf(const CVisImageBase& refImage, int xLeft, int yTop,
			int xRight, int yBottom, int band = -1,
			int dxBorder = 0, int dyBorder = 0,
			EVisPad pad = evispadNone);


	
	// STL iterator operators
	inline TPixel& operator*(void) const;

	// Prefix increment and decrement.
	inline CVisEnumPixelOf<TPixel>& operator++(void);
	// LATER:  inline CVisEnumPixelOf<TPixel>& operator--(void);

	// Postfix increment and decrement.
	inline CVisEnumPixelOf<TPixel> operator++(int);
	// LATER:  inline inline CVisEnumPixelOf<TPixel> operator--(int);


	// @cmember
	// Get a pointer to the current pixel.
	inline TPixel *PPixelCur(void) const;

	// @cmember
	// Get a reference to the current pixel.
	inline TPixel& PixelCur(void) const;

	// LATER:  Templated function operator methods.

protected:
	// LATER:  Member variables?
};



#include "VisEnumPixel.inl"


#endif // __VIS_CORE_ENUMPIXEL_H__
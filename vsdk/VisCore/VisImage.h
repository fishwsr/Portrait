///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISIMAGEOF
//
// @module VisImageOf.h |
//
// This file declares the typed image classes.  These classes
// support two-dimensional, optionally multi-banded,
// arrays of pixels of any C++ or user-defined type.
//
// The size of an image is set when an image is constructed.  Subimages
// which share the same memory can be constructed from images.
//
// <nl>
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @todo:(INTERNAL)
// File input/output, and stream input/output are also provided.
// We shouldn't need to use format strings with C++ file I / O.
//
// Need to document requirements for user-defined pixel types (including
// information about constructors and conversions to standard pixels).
//
// Copy ctor should only require that pix types match.  (It's okay to
// copy from a type derived from this type if the pixel types match.)
//
// Pixel methods without bands should assert that NBands() == 1.
//
// Can boarder space be specified in ctors or alloc method?
//
// Virtual pad methods.
//
// Subimage method (or "subimage" in description of SetRect).
//
// <nl>
//
// @xref <l VisImageOf\.inl> <l VisImage\.h> <l VisImage\.inl>
// <l VisImage\.cpp> <l VisFileIO\.cpp>
//
// <nl>
// @index | VISIMAGEOF
//
///////////////////////////////////////////////////////////////////////////


#ifndef __VIS_CORE_IMAGE_H__
#define __VIS_CORE_IMAGE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "VisImageBase.h"


////////////////////////////////////////////////////////////////////////////
//  
// @class
//  
// This templated class defines an image whose pixels values are of
// the type specified.  Users of the library can use this class to
// define images of any desired pixel type.
//
// Standard image types are defined in the <l VisImageTypes\.h> file.
//  
// @tcarg class | TPixel | Pixel type.
//  
// @base public | CVisImageBase
//
// @xref <c CVisImageBase>
//
////////////////////////////////////////////////////////////////////////////
template <class TPixel>
class CVisImage : public CVisImageBase
{
public:
	// @type T_Pixel | TPixel
	typedef TPixel T_Pixel;
	typedef class CVisImageBase T_BaseImage;
	typedef class CVisSequenceBase T_BaseSequence;

	typedef CVisEnumPixelOf<TPixel> iterator;


    //------------------------------------------------------------------
	// @group Constructors and Assignement Operator

	// @cmember
	// Default constructor.  The image will not be useable until another
	// image is assigned to it.
	CVisImage(void);

	// @cmember
	// Construct an image of a given size, allocating memory needed
	// for the image or using memory provided.
	CVisImage(int width, int height, int nbands = 1,
			int imopts = - 1, BYTE *pbData = 0);

	// @cmember
	// Construct an image of a given size, allocating memory needed
	// for the image or using memory provided.
	CVisImage(SIZE size, int nbands = 1,
			int imopts = - 1, BYTE *pbData = 0);

	// @cmember
	// Construct an image of a given size, allocating memory needed
	// for the image or using memory provided.
	CVisImage(const RECT& rect, int nbands = 1,
			int imopts = - 1, BYTE *pbData = 0);

	// @cmember
	// Construct an image of a given size, allocating memory needed
	// for the image or using memory provided.
	CVisImage(const CVisShape& shape,
			int imopts = - 1, BYTE *pbData = 0);

	// @cmember
	// Construct an image of a given size using the memory provided.
	CVisImage(CVisMemBlock& refmemblock, int width, int height,
			int nbands = 1, int imopts = - 1);

	// @cmember
	// Construct an image of a given size using the memory provided.
	CVisImage(CVisMemBlock& refmemblock, SIZE size, int nbands = 1,
			int imopts = - 1);

	// @cmember
	// Construct an image of a given size using the memory provided.
	CVisImage(CVisMemBlock& refmemblock, const RECT& rect,
			int nbands = 1, int imopts = - 1);

	// @cmember
	// Construct an image of a given size using the memory provided.
	CVisImage(CVisMemBlock& refmemblock, const CVisShape& shape,
			int imopts = - 1);

	// @cmember
	// Construct an image from another image.
	// Warning:  Derived classes should provide their own copy constructor
	// that first constructs a default object and then uses the assignment
	// operator to synchronize  image creation, assignment, and
	// destruction.  Derived classes should not call the copy constructor
	// in the base class.
	CVisImage(const CVisImage<TPixel>& imageSrc);

	// @cmember
	// Assignment operator.
	CVisImage& operator=(const CVisImage<TPixel>& imageSrc);


	//------------------------------------------------------------------
	// @group Pixel Access

	// @cmember
	// Given the coordinates of a point in the image, find the value
	// of the pixel (in the first image band) with the specified
	// coordinates.  In the debug builds, this will assert that the
	// pixel coordinates are in the image rectangle, but this function
	// may return an invalid pixel address if the pixel coordinates
	// are not in the image rectangle.
	const TPixel& Pixel(int x, int y) const;
	const TPixel& operator()(int x, int y) const;

	// @cmember
	// Given the coordinates of a point in the image, find the value
	// of the pixel (in the first image band) with the specified
	// coordinates.  In the debug builds, this will assert that the
	// pixel coordinates are in the image rectangle, but this function
	// may return an invalid pixel address if the pixel coordinates
	// are not in the image rectangle.
	const TPixel& Pixel(POINT point) const;
	const TPixel& operator()(POINT point) const;

	// @cmember
	// Given the coordinates of a point in the image, find the value
	// of the pixel (in the first image band) with the specified
	// coordinates.  In the debug builds, this will assert that the
	// pixel coordinates are in the image rectangle, but this function
	// may return an invalid pixel address if the pixel coordinates
	// are not in the image rectangle.
	TPixel& Pixel(int x, int y);
	TPixel& operator()(int x, int y);

	// @cmember
	// Given the coordinates of a point in the image, find the value
	// of the pixel (in the first image band) with the specified
	// coordinates.  In the debug builds, this will assert that the
	// pixel coordinates are in the image rectangle, but this function
	// may return an invalid pixel address if the pixel coordinates
	// are not in the image rectangle.
	TPixel& Pixel(POINT point);
	TPixel& operator()(POINT point);

	// @cmember
	// Given the coordinates of a point in the image, find the value
	// of the pixel in the specified band with the specified
	// coordinates.  In the debug builds, this will assert that the
	// pixel coordinates are in the image rectangle, but this function
	// may return an invalid pixel address if the pixel coordinates
	// are not in the image rectangle.
	const TPixel& Pixel(int x, int y, int band) const;
	const TPixel& operator()(int x, int y, int band) const;

	// @cmember
	// Given the coordinates of a point in the image, find the value
	// of the pixel in the specified band with the specified
	// coordinates.  In the debug builds, this will assert that the
	// pixel coordinates are in the image rectangle, but this function
	// may return an invalid pixel address if the pixel coordinates
	// are not in the image rectangle.
	const TPixel& Pixel(POINT point, int band) const;
	const TPixel& operator()(POINT point, int band) const;

	// @cmember
	// Given the coordinates of a point in the image, find the value
	// of the pixel in the specified band with the specified
	// coordinates.  In the debug builds, this will assert that the
	// pixel coordinates are in the image rectangle, but this function
	// may return an invalid pixel address if the pixel coordinates
	// are not in the image rectangle.
	TPixel& Pixel(int x, int y, int band);
	TPixel& operator()(int x, int y, int band);

	// @cmember
	// Given the coordinates of a point in the image, find the value
	// of the pixel in the specified band with the specified
	// coordinates.  In the debug builds, this will assert that the
	// pixel coordinates are in the image rectangle, but this function
	// may return an invalid pixel address if the pixel coordinates
	// are not in the image rectangle.
	TPixel& Pixel(POINT point, int band);
	TPixel& operator()(POINT point, int band);

	// @cmember
	// Find the address of the pixel with the specified coordinates.
	const TPixel *PixelAddress(POINT point) const;

	// @cmember
	// Find the address of the pixel with the specified coordinates.
	const TPixel *PixelAddress(int x, int y) const;

	// @cmember
	// Find the address of the pixel with the specified coordinates.
	const TPixel *PixelAddress(POINT point, int band) const;

	// @cmember
	// Find the address of the pixel with the specified coordinates.
	const TPixel *PixelAddress(int x, int y, int band) const;

	// @cmember
	// Find the address of the pixel with the specified coordinates.
	TPixel *PixelAddress(POINT point);

	// @cmember
	// Find the address of the pixel with the specified coordinates.
	TPixel *PixelAddress(int x, int y);

	// @cmember
	// Find the address of the pixel with the specified coordinates.
	TPixel *PixelAddress(POINT point, int band);

	// @cmember
	// Find the address of the pixel with the specified coordinates.
	TPixel *PixelAddress(int x, int y, int band);


	// @cmember
	// Given a row containing pixels in the image, return a pointer
	// to the pixel in the first band of column zero in the
	// row specified.  In the debug builds, this will assert that the
	// row does contain pixels in the image, but this function
	// may return an invalid pixel address if the row does not
	// contain pixels in the image.
	const TPixel *RowPointer(int y) const;

	// @cmember
	// Given a row containing pixels in the image, return a pointer
	// to the pixel in the first band of column zero in the
	// row specified.  In the debug builds, this will assert that the
	// row does contain pixels in the image, but this function
	// may return an invalid pixel address if the row does not
	// contain pixels in the image.
	TPixel *RowPointer(int y);


	// @cmember
	// Given a row containing pixels in the image, return a reference
	// to the pixel in the first band of the leftmost point in the
	// row specified.  In the debug builds, this will assert that the
	// row does contain pixels in the image, but this function
	// may return an invalid pixel address if the row does not
	// contain pixels in the image.
	const TPixel& FirstPixelInRow(int y) const;

	// @cmember
	// Given a row containing pixels in the image, return a reference
	// to the pixel in the first band of the leftmost point in the
	// row specified.  In the debug builds, this will assert that the
	// row does contain pixels in the image, but this function
	// may return an invalid pixel address if the row does not
	// contain pixels in the image.
	TPixel& FirstPixelInRow(int y);

	// @cmember
	// Given a row containing pixels in the image, return a pointer
	// to the pixel in the first band of the leftmost point in the
	// row specified.  In the debug builds, this will assert that the
	// row does contain pixels in the image, but this function
	// may return an invalid pixel address if the row does not
	// contain pixels in the image.
	const TPixel *PtrToFirstPixelInRow(int y) const;

	// @cmember
	// Given a row containing pixels in the image, return a pointer
	// to the pixel in the first band of the leftmost point in the
	// row specified.  In the debug builds, this will assert that the
	// row does contain pixels in the image, but this function
	// may return an invalid pixel address if the row does not
	// contain pixels in the image.
	TPixel *PtrToFirstPixelInRow(int y);

	// @cmember
	// Fill an Iliffe vector.  A pointer to the start of the Iliffe
	// vector memory is passed to this function.  Pointers to column
	// zero in each image row are placed in the Iliffe vector.  A
	// pointer that can be indexed by image row is returned.
	TPixel **FillIliffeVector(void **ppvIliffe) const;

	// @cmember
	// Fill an Iliffe vector.  A pointer to the start of the Iliffe
	// vector memory is passed to this function.  Pointers to column
	// zero in each image row are placed in the Iliffe vector.  A
	// pointer that can be indexed by image row is returned.
	TPixel **FillIliffeVector(void **ppvIliffe, int yFirst, int yLim)
			const;


	//------------------------------------------------------------------
	// @group SubImages

	// @cmember
	// Return a subimage of this image.  The image returned is of the
	// same type as this image and it uses the same memory block to
	// store pixel information.  The returned image's rectangle will
	// have the coordinates specified.  These coordinates must be
	// contained in the memory rectangle returned by the
	// <mf CVisImageBase::MemoryRect> method.
	CVisImage<TPixel> SubImage(int left, int top, int right,
			int bottom) const;

	// @cmember
	// Return a subimage of this image.  The image returned is of the
	// same type as this image and it uses the same memory block to
	// store pixel information.  The returned image's rectangle will
	// have the coordinates specified.  These coordinates must be
	// contained in the memory rectangle returned by the
	// <mf CVisImageBase::MemoryRect> method.
	CVisImage<TPixel> SubImage(POINT pointLeftTop,
			POINT pointRightBottom) const;

	// @cmember
	// Return a subimage of this image.  The image returned is of the
	// same type as this image and it uses the same memory block to
	// store pixel information.  The returned image's rectangle will
	// have the coordinates specified.  These coordinates must be
	// contained in the memory rectangle returned by the
	// <mf CVisImageBase::MemoryRect> method.
	CVisImage<TPixel> SubImage(POINT pointLeftTop,
			SIZE size) const;

	// @cmember
	// Return a subimage of this image.  The image returned is of the
	// same type as this image and it uses the same memory block to
	// store pixel information.  The returned image's rectangle will
	// have the coordinates specified.  These coordinates must be
	// contained in the memory rectangle returned by the
	// <mf CVisImageBase::MemoryRect> method.
	CVisImage<TPixel> SubImage(const RECT& rect) const;


	//------------------------------------------------------------------
	// @group Comparing Images

	// @cmember
	// Are the shapes and pixel values of two images the same?
	// (If the two images use different pixel memory, this operator
	// may need to look at all of the pixels in the images to find
	// that one image is a copy of another image.)
	bool operator==(const CVisImage<TPixel>& refimage) const;

	// @cmember
	// Do the shapes or pixel values of two images differ?
	// (If the two images use different pixel memory, this operator
	// may need to look at all of the pixels in the images to find
	// that one image is a copy of another image.)
	bool operator!=(const CVisImage<TPixel>& refimage) const;


	//------------------------------------------------------------------
	// @group Padding

	// @cmember
	// Pad pixels between the image rectangle and the specified
	// rectangle using the specified padding method.  The rectangle
	// specifed must be contained in the memory rectangle returned by
	// the <mf CVisImageBase::MemoryRect> method.
	void Pad(EVisPad evispad, int left, int top, int right, int bottom);

	// @cmember
	// Pad pixels between the image rectangle and the specified
	// rectangle using the specified padding method.  The rectangle
	// specifed must be contained in the memory rectangle returned by
	// the <mf CVisImageBase::MemoryRect> method.
	void Pad(EVisPad evispad, POINT pointLeftTop, POINT pointRightBottom);

	// @cmember
	// Pad pixels between the image rectangle and the specified
	// rectangle using the specified padding method.  The rectangle
	// specifed must be contained in the memory rectangle returned by
	// the <mf CVisImageBase::MemoryRect> method.
	void Pad(EVisPad evispad, POINT pointLeftTop, SIZE size);

	// @cmember
	// Pad pixels between the image rectangle and the specified
	// rectangle using the specified padding method.  The rectangle
	// specifed must be contained in the memory rectangle returned by
	// the <mf CVisImageBase::MemoryRect> method.
	void Pad(EVisPad evispad, const RECT& rect);

	// @cmember
	// Pad pixels between the image rectangle and the specified
	// rectangle by setting them to the specifed value.  The rectangle
	// specifed must be contained in the memory rectangle returned by
	// the <mf CVisImageBase::MemoryRect> method.
	void Pad(TPixel value, int left, int top, int right, int bottom);

	// @cmember
	// Pad pixels between the image rectangle and the specified
	// rectangle by setting them to the specifed value.  The rectangle
	// specifed must be contained in the memory rectangle returned by
	// the <mf CVisImageBase::MemoryRect> method.
	void Pad(TPixel value, POINT pointLeftTop, POINT pointRightBottom);

	// @cmember
	// Pad pixels between the image rectangle and the specified
	// rectangle by setting them to the specifed value.  The rectangle
	// specifed must be contained in the memory rectangle returned by
	// the <mf CVisImageBase::MemoryRect> method.
	void Pad(TPixel value, POINT pointLeftTop, SIZE size);

	// @cmember
	// Pad pixels between the image rectangle and the specified
	// rectangle by setting them to the specifed value.  The rectangle
	// specifed must be contained in the memory rectangle returned by
	// the <mf CVisImageBase::MemoryRect> method.
	void Pad(TPixel value, const RECT& rect);


    //------------------------------------------------------------------
	// @group C++ Type Information

	// @cmember
	// Information about the pixels in this image.
	virtual const type_info& PixelTypeInfo(void) const;

	// @cmember
	// Information about the type of this object.  The iLevel
	// argument specifies the class depth (with CVisImageBase being
	// at depth 0).  Use - 1 to find information about the deepest
	// class.
	virtual const type_info& ObjectTypeInfo(int iLevel = - 1) const;


	//------------------------------------------------------------------
	// @group Miscellaneous

	// @cmember
	// Set each pixel in the image rectangle to the specified value.
	CVisImage& FillPixels(TPixel value, int band = -1);

	// @cmember
	// Make a copy of this image.  Most users of the library should not
	// use this method.  Instead, they should use the
	// <mf CVisImage::SubImage> methods in the typed image classes.
	// The pointer returned must be deleted using the C++ <f delete>
	// operator when the caller has finished using the image.
	virtual CVisImageBase *Clone(const CVisShape *pshape = 0) const;


	//------------------------------------------------------------------
	// @group STL-like iterator methods

	iterator begin(int iband = - 1);
	iterator end(int iband = - 1);


    //------------------------------------------------------------------
	// SDStream I / O

	// These two methods are overridden in derived classes to give type
	// information (about the derived classes) to the property list code.
	virtual CVisPropTypeInfoBase *PropTypeInfoNew(void) const;
	virtual CVisPropTypeInfoBase *PropTypeInfoLookup(void) const;


	// Constant used to specify the depth of this object (for use with
	// ObjectTypeInfo).
	enum { eilevelImage = eilevelImageBase + 1 };
	enum { eiLevel = T_BaseImage::eiLevel + 1 };


// @access:(IMPL) Protected Members
protected:
	//------------------------------------------------------------------
	// @group:(IMPL) Copying Images

	// @cmember:(IMPL)
	// Copy this image to another image.  This may fail if the
	// pixels in the source image can't be converted to values
	// which can be used to set the pixels in the destination image.
	// This method is called by the public <mf CVisImageBase::CopyPixelsTo>
	// methods.
	virtual bool RealCopyPixelsTo(int bandSrc, int bandDest,
			CVisImageBase& refimageDest, const RECT& refrectSrc,
			const POINT pointDest, EVisNormalize evisnormalize,
			double dblMin = 1.0, double dblMax = - 1.0) const;


    //------------------------------------------------------------------
	// @group:(IMPL) Image Information

	// @cmember:(IMPL)
	// Return an <t EVisPixFmt> value that describes the type of the pixel
	// values used in this image.  Returns <e evispixfmtUnknown> if the type
	// can not be described using the <t EVisPixFmt> values.
	static EVisPixFmt StaticPixFmt(void);

	// @cmember:(IMPL)
	// Set the memory block used by this object.  This is called in the
	// constructor if a memory block of <t TPixel> objects needs to be
	// allocated for this image.
	void SetMemBlockFromImopts(int imopts);
};



// LATER:  The following functions may need to be changed since the
// Reallocate method has been removed.

template <class TYPEDIMAGE>
inline TYPEDIMAGE VisSetupResult(TYPEDIMAGE *resultptr, TYPEDIMAGE *src1);

template <class TYPEDIMAGE> 
inline TYPEDIMAGE VisSetupResult(TYPEDIMAGE *resultptr, TYPEDIMAGE *src1,
								 TYPEDIMAGE *src2, ...);

template <class TYPEDIMAGE>
inline void VisMakeSimilar(
                           TYPEDIMAGE &result,  // Result image.
                           TYPEDIMAGE &img,     // Source image.
                           int subsample);      // Subsample factor for source image.


///////////////////////////////////////////////////////////////////////////////
//
// Iostream input/output functions
//  We have to special case unsigned chars, else they print as symbols.
//
///////////////////////////////////////////////////////////////////////////////

#if (_MSC_VER >= 1100)	// VC 5.0 or greater

template <class TPixel>
static inline void pixel_out(std::ostream& os, TPixel& p);

static inline void pixel_out(std::ostream& os, unsigned char& p);

template <class TPixel>
inline std::ostream& operator<<(
                           std::ostream& os,                 // Output stream.
                           CVisImage<TPixel>& img); // Image to output.

template <class TPixel>
static inline void pixel_in(std::istream& os, TPixel& p);

static inline void pixel_in(std::istream& os, unsigned char& p);

template <class TPixel>
inline std::istream& operator>>(
                           std::istream& is,            // Input stream.
                           CVisImage<TPixel>& img); // Place to store image.

#else // (_MSC_VER >= 1100)

template <class TPixel>
static inline void pixel_out(ostream& os, TPixel& p);

static inline void pixel_out(ostream& os, unsigned char& p);

template <class TPixel>
inline ostream& operator<<(
                           ostream& os,                 // Output stream.
                           CVisImage<TPixel>& img); // Image to output.

template <class TPixel>
static inline void pixel_in(istream& os, TPixel& p);

static inline void pixel_in(istream& os, unsigned char& p);

template <class TPixel>
inline istream& operator>>(
                           istream& is,            // Input stream.
                           CVisImage<TPixel>& img); // Place to store image.

#endif // (_MSC_VER >= 1100)


// Helper function

// General case
template<class TPixel>
inline CVisImage<TPixel> VisBrightnessFromRGBA(
		const CVisImage<CVisRGBA<TPixel> >& refimageRGBA);

// Specialization (defined in VisImageBase.cpp).
VisCoreExport CVisImage<BYTE> VisBrightnessFromRGBA(
		const CVisImage<CVisRGBA<BYTE> >& refimageRGBA);

// template <class TNum>
// inline TNum VisBrightnessFromRGBA(const CVisRGBA<TNum>& refrgba);
// is declared in VisRGBAPixels.h.


////////////////////////////////////////////////////////////////////////////
// Overloaded operators

template<class TPixel>
inline CVisImage<TPixel> operator-(const CVisImage<TPixel>& refimage);

template<class TPixel>
inline CVisImage<TPixel>& operator+=(CVisImage<TPixel>& refimage,
		const TPixel& refpixel);
template<class TPixel>
inline CVisImage<TPixel>& operator+=(CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2);
template<class TPixel>
inline CVisImage<TPixel> operator+(const CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2);
template<class TPixel>
inline CVisImage<TPixel> operator+(const CVisImage<TPixel>& refimage,
		const TPixel& refpixel);

template<class TPixel>
inline CVisImage<TPixel>& operator-=(CVisImage<TPixel>& refimage,
		const TPixel& refpixel);
template<class TPixel>
inline CVisImage<TPixel>& operator-=(CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2);
template<class TPixel>
inline CVisImage<TPixel> operator-(const CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2);
template<class TPixel>
inline CVisImage<TPixel> operator-(const CVisImage<TPixel>& refimage,
		const TPixel& refpixel);

template<class TPixel>
inline CVisImage<TPixel>& operator*=(CVisImage<TPixel>& refimage,
		const TPixel& refpixel);
template<class TPixel>
inline CVisImage<TPixel>& operator*=(CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2);
template<class TPixel>
inline CVisImage<TPixel> operator*(const CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2);
template<class TPixel>
inline CVisImage<TPixel> operator*(const CVisImage<TPixel>& refimage,
		const TPixel& refpixel);

template<class TPixel>
inline CVisImage<TPixel>& operator/=(CVisImage<TPixel>& refimage,
		const TPixel& refpixel);
template<class TPixel>
inline CVisImage<TPixel>& operator/=(CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2);
template<class TPixel>
inline CVisImage<TPixel> operator/(const CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2);
template<class TPixel>
inline CVisImage<TPixel> operator/(const CVisImage<TPixel>& refimage,
		const TPixel& refpixel);

template<class TPixel>
inline CVisImage<TPixel> operator~(const CVisImage<TPixel>& refimage);

template<class TPixel>
inline CVisImage<TPixel>& operator&=(CVisImage<TPixel>& refimage,
		const TPixel& refpixel);
template<class TPixel>
inline CVisImage<TPixel>& operator&=(CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2);
template<class TPixel>
inline CVisImage<TPixel> operator&(const CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2);
template<class TPixel>
inline CVisImage<TPixel> operator&(const CVisImage<TPixel>& refimage,
		const TPixel& refpixel);

template<class TPixel>
inline CVisImage<TPixel>& operator|=(CVisImage<TPixel>& refimage,
		const TPixel& refpixel);
template<class TPixel>
inline CVisImage<TPixel>& operator|=(CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2);
template<class TPixel>
inline CVisImage<TPixel> operator|(const CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2);
template<class TPixel>
inline CVisImage<TPixel> operator|(const CVisImage<TPixel>& refimage,
		const TPixel& refpixel);

template<class TPixel>
inline CVisImage<TPixel>& operator<<=(CVisImage<TPixel>& refimage,
		const TPixel& refpixel);
template<class TPixel>
inline CVisImage<TPixel>& operator<<=(CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2);
template<class TPixel>
inline CVisImage<TPixel> operator<<(const CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2);
template<class TPixel>
inline CVisImage<TPixel> operator<<(const CVisImage<TPixel>& refimage,
		const TPixel& refpixel);

template<class TPixel>
inline CVisImage<TPixel>& operator>>=(CVisImage<TPixel>& refimage,
		const TPixel& refpixel);
template<class TPixel>
inline CVisImage<TPixel>& operator>>=(CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2);
template<class TPixel>
inline CVisImage<TPixel> operator>>(const CVisImage<TPixel>& refimage1,
		const CVisImage<TPixel>& refimage2);
template<class TPixel>
inline CVisImage<TPixel> operator>>(const CVisImage<TPixel>& refimage,
		const TPixel& refpixel);

// LATER:  We might operators that take a pixel followed by an image
// (as opposed to an image followed by a pixel).

// LATER:  Add global functions for (possibly weighted) averaging and
// math operations with clipped results (to prevent overflow / underflow).


#include "VisImage.inl"


#endif // __VIS_CORE_IMAGE_H__

///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISIMAGEBASE
//
// @module VisImage.h |
//
// This file declares the base image classes.
// Typed image classes, defined in VisImageOf.h, are derived
// from these base classes
//
// The base classes support two-dimensional, optionally multi-banded,
// arrays of pixels of any size (given in bytes).  The typed image
// classes support two-dimensional, optionally multi-banded,
// arrays of pixels of any C++ or user-defined type.
//
// <nl>
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @todo:(INTERNAL)
// Add asserts that enum masks don't interesct and that masks contain the
// appropriate enum values.
//
// Validity asserts
//
// Add copy from methods.
//
// What should happen if the user tried to change the number of bands
// in an image when changing its shape?
//
// Allocate method won't work when typed memory blocks are used.  Is
// this okay?  (They should handle that case by using the default
// allocator.)
//
// Check the conditions needed for DisplayInHdc to succeed.  It should
// work with multi-band images.
//
// We might want to add a band parameter to the RowPointer-type methods
// so that planar images might be supported in the future.
// 
// Make sure that TIF is supported by file handler to the same extent that
// TIFF is suppoered.
//
// Add explicit RECT params for Allocate and SetMemBlock?
//
// Add rectMemBlock to Copy method.
//
// LATER:  It might be nice to have protected DisplayInHdc helper methods
// that derived classes could override.
//
// Need to document assumptions about byte ordering in ints and about the
// sizes of standard types.
//
// <nl>
//
// @xref <l VisImage\.inl> <l VisImage\.cpp> <l VisFileIO\.cpp>
// <l VisImageOf\.h> <l VisImageOf\.inl>
//
// <nl>
// @index | VISIMAGE
//
///////////////////////////////////////////////////////////////////////////


#ifndef __VIS_CORE_IMAGEBASE_H__
#define __VIS_CORE_IMAGEBASE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include <typeinfo.h>
#include <stdio.h>      // needed by ReadWriteStream
#include <assert.h>
#include <string>


#include "VisShape.h"
#include "VisMemBlock.h"
#include "VisFileIO.h"

#include "VisPixFmt.h"

#ifndef VIS_CORE_DONT_USE_ILIFFE
#define VIS_CORE_IMAGE_USE_ILIFFE 1
#endif // VIS_CORE_DONT_USE_ILIFFE

// forward declaration
class CVisStreamHandler;
interface IDirectDrawInterface7;
interface IVisImage;

////////////////////////////////////////////////////////////////////////////
//  
// @enum EVisImOpt |
// Options specified when constructing images.
//
// Users can change the default options for images by calling the static
// CVisImageBase::SetDefaultOptions method.
//
// @todo:(INTERNAL)
//
// LATER:  Other imopt:  VolatileMem, canCopyPixMem
// LATER:  Combine with static flags for writing image files?
//
////////////////////////////////////////////////////////////////////////////
enum EVisImOpt
{
	// @emem Default options
	evisimoptDefault				= 0x0000,

    //------------------------------------------------------------------
	// @group Memory Allocation

	// @emem
	// If memory needs to be allocated for this image, use
	// <f CreateFileMapping> to allocate the memory so that the
	// memory can be shared with other processes.
	// Image memory used in Windows Bitmaps must be allocated using
	// this option, but there is less than 1GB of this type of
	// memory available on Windows 95.
	// This is the default option.
	evisimoptMemShared				= 0x0000,

	// @emem
	// If memory needs to be allocated for this image, use
	// <f GlobalAlloc>.  If this option is
	// used, the <mf CVisImageBase::HBitmapCreateForHdc> method for
	// this image will always fail.
	evisimoptMemNotShared			= 0x0001,

	// @emem
	// If memory needs to be allocated for this image, use the C++
	// <f new> operator to allocate a block of pixels of the type
	// specified in a templated class derived from the
	// <c CVisImageBase> base class.  This will ensure that the
	// default constructors of the image pixels are called to
	// properly initialize the pixel objects.  The
	// <e VisImOpt::visimoptPadZero> option is ignored if
	// <e VisImOpt::visimoptMemObjType> is specified.
	evisimoptMemObjType				= 0x0002,

	// @emem
	// Mask used to extract the Memory Allocation options from a
	// union of <t VisImOpt> values.
	evisimoptMemAllocMask			= 0x0007,

    //------------------------------------------------------------------
	// @group Align Image Columns

	// @emem
	// Make sure that the addresses any two pixels in adjacent
	// rows of the same column of this image differ by a multiple of
	// four bytes.  This is required for images used in Device
	// Independent Bitmaps.
	// This is the default option.
	evisimoptAlignMem4Byte			= 0x0000,

	// @emem
	// Make sure that the addresses any two pixels in adjacent
	// rows of the same column of this image differ by a multiple of
	// eight bytes.
	evisimoptAlignMem8Byte			= 0x0008,

	// @emem
	// Don't increase the memory required to store this image if
	// the image rows do not require a multiple of <f sizeof>(<c LONG>)
	// bytes.
	evisimoptDontAlignMemory		= 0x0018,

	// @emem
	// Mask used to extract the Align Image options from a
	// union of <t VisImOpt> values.
	evisimoptAlignMemMask			= 0x0018,

    //------------------------------------------------------------------
	// @group Memory Initialization

	// @emem
	// If memory is allocated for this image, it does not need to be
	// initialized.
	// This is the default option.
	evisimoptDontInitMemory			= 0x0000,

	// @emem
	// If memory is allocated for this image, all bytes of the memory
	// allocated should be initialized to zero.
	evisimoptZeroInitMemory			= 0x0020,

	// @emem
	// Mask used to extract the Memory Initialization options from a
	// union of <t VisImOpt> values.
	evisimoptInitMemMask			= 0x0020,
};





////////////////////////////////////////////////////////////////////////////
//  
// @enum EVisPad |
// Padding options.
//
////////////////////////////////////////////////////////////////////////////
enum EVisPad
{
	// @emem
	// No padding is needed for the pixels outside of the image rectangle.
	evispadNone,

	// @emem
	// Pad pixels outside of the image rectangle with zero values.
	evispadZero,

	// @emem
	// Pad pixels outside of the image rectangle by replicating the
	// border pixel values.
	evispadReplicate,

	// @emem
	// Pad pixels outside of the image rectangle by reflecting the
	// known pixel values.
	evispadReflect,

	// @emem
	// Pad pixels outside of the image rectangle by using a cyclic
	// padding of the known pixel values.  (So pixel values on the right
	// side of the image will be used when padding the left side of the
	// image.)
	evispadCyclic,

	evispadLim
};


////////////////////////////////////////////////////////////////////////////
//  
// @class
//
// This is the base image class.  All image classes are derived from
// this class.  The methods in this class apply to all images.
//
// Images have a shape, a block of memory containing pixel information,
// and a way of mapping coordinates to pixel addresses.  The shape of an
// image is a <c CVisShape> object (derived from <c CRect>) that specifies
// a rectangle and a number of bands for the image.
//
// @xref <c CVisImage>
//
////////////////////////////////////////////////////////////////////////////
class CVisImageBase
{
// @access Public Members
public:
    //------------------------------------------------------------------
	// @group Destructor

	// @cmember
	// Destrory this object.
	virtual ~CVisImageBase(void);


    //------------------------------------------------------------------
	// @group Assignment Operator

	// @cmember
	// Assignment is only allowed between images of the same pixel type.
	// The debug build will assert that the pixel types match.
	VisCoreExport CVisImageBase& operator=(const CVisImageBase& refImage);


	//------------------------------------------------------------------
	// @group Image Dimensions

	// @cmember
	// Get the width of the image.
	int Width(void) const;

	// @cmember
	// Get the height of the image.
	int Height(void) const;

	// @cmember
	// Get the number of bands in each point in the image.
	int NBands(void) const;


	// @cmember
	// Get the number of points in the image.  (This is the
	// product or the width and height of the image.)
	int NPoints(void) const;

	// @cmember
	// Get the number of pixels in the image.  (This is the product of
	// the image height, image width, and number of image bands.)
	int NPixels(void) const;


	// @cmember
	// Get a CSize structure giving the width and height of the image.
	CVisSize Size(void) const;


    //------------------------------------------------------------------
	// @group Image Coordinates

	// @cmember
	// Get the coordinate of the left column contained in the image
	// rectangle.
	int Left(void) const;

 	// @cmember
	// Get the coordinate of the top row contained in the image
	// rectangle.
	int Top(void) const;

	// @cmember
	// Get the coordinate of the right column bounding the image
	// rectangle.  This column is not contained in the image rectangle.
	int Right(void) const;

	// @cmember
	// Get the coordinate of the bottom row bounding the image
	// rectangle.  This row is not contained in the image rectangle.
	int Bottom(void) const;

	// @cmember
	// Get the location of the upper-left point contained in the
	// image rectangle.
	CVisPoint StartPoint(void) const;

	// @cmember
	// Get the location of the center point in the image.
	// @todo:(INTERNAL) Document rounding used with <mf CVisImageBase::CenterPoint>.
	CVisPoint CenterPoint(void) const;

	// @cmember
	// Get the location of the lower-right point contained in the
	// image rectangle.
	CVisPoint LastPoint(void) const;

	// @cmember
	// Get the location of the lower-right limit of the image rectangle.
	// This point is not contained in the image rectangle.
	CVisPoint LimPoint(void) const;

	// @cmember
	// Get a <c CRect> object giving the top, left, bottom, and right
	// limits of the image.  (The bottom row and right column are not
	// part of the image.)
	const CVisRect& Rect(void) const;

	// @cmember
	// Get a <c CVisShape> object giving the top, left, bottom, and right
	// limits of the image and the number of bands in the image.  (The
	// bottom row and right column are not part of the image.)
	const CVisShape& Shape(void) const;


	//------------------------------------------------------------------
	// @group Points Contained in the Image

	// @cmember
	// Is the specified point contained in the image rectangle?
	bool ContainsPoint(int x, int y) const;

	// @cmember
	// Is the specified point contained in the image rectangle?
	bool ContainsPoint(int x, int y, int band) const;

	// @cmember
	// Is the specified point contained in the image rectangle?
	bool ContainsPoint(POINT point) const;

	// @cmember
	// Is the specified point contained in the image rectangle?
	bool ContainsPoint(POINT point, int band) const;


	//------------------------------------------------------------------
	// @group Pixel Adresses

	// @cmember
	// The number of bytes between pixels in adjacent rows of the
	// same column and band.
	int CbRow(void) const;

	// @cmember
	// The number of bytes between pixels in adjacent columns
	// of the same row and band.
	int CbCol(void) const;

	// @cmember
	// The number of bytes between pixels in adjacent bands
	// of the same row and column.
	int CbBand(void) const;

	// @cmember
	// The size of an image point in bytes.  (This will be the same
	// as <mf CVisImageBase::CbCol>, since points in adjacent
	// columns of the same row are adjacent in memory.)
	int CbPoint(void) const;

	// @cmember
	// The size of an image pixel (in a row, col, and band) in bytes.
	int CbPixel(void) const;

	// @cmember
	// Find the address of the pixel with the specified coordinates.
	const BYTE *PbPixel(POINT point) const;

	// @cmember
	// Find the address of the pixel with the specified coordinates.
	const BYTE *PbPixel(int x, int y) const;

	// @cmember
	// Find the address of the pixel with the specified coordinates.
	const BYTE *PbPixel(POINT point, int band) const;

	// @cmember
	// Find the address of the pixel with the specified coordinates.
	const BYTE *PbPixel(int x, int y, int band) const;

	// @cmember
	// Find the address of the pixel with the specified coordinates.
	BYTE *PbPixel(POINT point);

	// @cmember
	// Find the address of the pixel with the specified coordinates.
	BYTE *PbPixel(int x, int y);

	// @cmember
	// Find the address of the pixel with the specified coordinates.
	BYTE *PbPixel(POINT point, int band);

	// @cmember
	// Find the address of the pixel with the specified coordinates.
	BYTE *PbPixel(int x, int y, int band);

	// @cmember
	// Given a row containing pixels in the image, find the address of
	// the pixel in the first band of the point in column zero of the
	// row specified.
	const BYTE *PbColZeroOfRow(int y) const;

	// @cmember
	// Given a row containing pixels in the image, find the address of
	// the pixel in the first band of the point in column zero of the
	// row specified.
	BYTE *PbColZeroOfRow(int y);

	// @cmember
	// Given a row containing pixels in the image, find the address
	// of the pixel in the first band of the leftmost point in the
	// row specified.
	const BYTE *PbFirstPixelInRow(int y) const;

	// @cmember
	// Given a row containing pixels in the image, find the address
	// of the pixel in the first band of the leftmost point in the
	// row specified.
	BYTE *PbFirstPixelInRow(int y);

	// @cmember
	// Does the given pixel address refer to a (byte in a) pixel
	// in the image?
	bool ContainsPixelAddress(const void *pixeladdress) const;

	// @cmember
	// Given the address of a (byte in a) pixel in the image, find
	// the row of the image that contains the specified pixel.
	int RowOfPixelAddress(const void *pixeladdress) const;

	// @cmember
	// Given the address of a (byte in a) pixel in the image, find
	// the column of the image that contains the specified pixel.
	int ColOfPixelAddress(const void *pixeladdress) const;

	// @cmember
	// Given the address of a (byte in a) pixel in the image, find
	// the band of the image that contains the specified pixel.
	int BandOfPixelAddress(const void *pixeladdress) const;

	// @cmember
	// Fill an Iliffe vector.  A pointer to the start of the Iliffe
	// vector memory is passed to this function.  Pointers to column
	// zero in each image row are placed in the Iliffe vector.  A
	// pointer that can be indexed by image row is returned.
	void **PpvFillIliffeVector(void **ppvIliffe) const;

	// @cmember
	// Fill an Iliffe vector.  A pointer to the start of the Iliffe
	// vector memory is passed to this function.  Pointers to column
	// zero in each image row are placed in the Iliffe vector.  A
	// pointer that can be indexed by image row is returned.
	VisCoreExport void **PpvFillIliffeVector(void **ppvIliffe, int yFirst,
			int yLim) const;


	//------------------------------------------------------------------
	// @group Coordinate Translation

	// @cmember
	// Translate the coordinate origin for this image to make the
	// left-top corner of the image have the coordinates specified.
	VisCoreExport CVisImageBase& ResetOrigin(int left, int top);

	// @cmember
	// Translate the coordinate origin for this image to make the
	// left-top corner of the image have the coordinates specified.
	CVisImageBase& ResetOrigin(POINT pointLeftTop);


	//------------------------------------------------------------------
	// @group Changing the Image Rectangle

	// @cmember
	// Set new coordinates for the image rectangle.
	VisCoreExport bool SetRect(int left, int top, int right, int bottom);

	// @cmember
	// Set new coordinates for the image rectangle.
	bool SetRect(POINT pointLeftTop, POINT pointRightBottom);

	// @cmember
	// Set new coordinates for the image rectangle.
	bool SetRect(POINT pointLeftTop, SIZE size);

	// @cmember
	// Set new coordinates for the image rectangle.
	bool SetRect(const RECT& rect);


	//------------------------------------------------------------------
	// @group Image Memory

	// @cmember
	// Get the width of the memory associated with this image.
	int MemoryWidth(void) const;

	// @cmember
	// Get the height of the memory associated with this image.
	int MemoryHeight(void) const;

	// @cmember
	// Get a <c CRect> object giving the top, left, bottom, and right
	// limits of the memory associated with this image.  (The bottom
	// row and right column are not part of the image.)
	const CVisRect& MemoryRect(void) const;

	// @cmember
	// Get a <c CVisShape> object giving the top, left, bottom, and right
	// limits of the memory associated with this image and the number of
	// bands in the image.  (The bottom row and right column are not part
	// of the image.)
	const CVisShape& MemoryShape(void) const;

	// @cmember
	// Get a reference to the <c CVisMemBlock> structure used to store
	// pixel data for this image.
	const CVisMemBlock& MemBlock(void) const;

	// @cmember
	// Allocate or reallocate the memory block for this image.  Most
	// users of the library will not need to use this method.
	void Allocate(int dx, int dy);

	// @cmember
	// Allocate or reallocate the memory block for this image.  Most
	// users of the library will not need to use this method.
	void Allocate(SIZE size);

	// @cmember
	// Allocate or reallocate the memory block for this image.  Most
	// users of the library will not need to use this method.
	void Allocate(const CVisShape& shapeImage);

	// @cmember
	// Allocate or reallocate the memory block for this image.  Most
	// users of the library will not need to use this method.
	VisCoreExport void Allocate(const CVisShape& shapeImage,
			const CVisShape& shapeMemBlock);

	// @cmember
	// Allocate the memory block for this image to use the same image
	// dimensions and coordinates as were in use before the image
	// memory was deallocated.  Most users of the library will not
	// need to use this method.
	VisCoreExport void Allocate(void);

	// @cmember
	// Free the memory block used by this image.  Most users of
	// the library will not need to use this method.
	VisCoreExport void Deallocate(bool fRememberShapes = true);

	// @cmember
	// Get the shape of the image before it was deallocated.  Most
	// users of the library will not need to use this method.
	const CVisShape& ShapeBeforeDeallocate(void) const;

	// @cmember
	// Get the shape of the memory before it was deallocated.  Most
	// users of the library will not need to use this method.
	const CVisShape& MemoryShapeBeforeDeallocate(void) const;

	// @cmember
	// This very specialized method will allocate memory for this image.
	// If the previous image or memory shape dimensions agree with the
	// dimension specified, the previous image and memory shape will be
	// used, otherwise the specified dimensions will be used.
	// Most users of the library will not need to use this method.
	void AllocateUsingPrevoiusShapeIfPossible(int dx, int dy, int nBands = 1);

	// @cmember
	// This very specialized method will allocate memory for this image.
	// If the previous image or memory shape dimensions agree with the
	// dimension specified, the previous image and memory shape will be
	// used, otherwise the specified dimensions will be used.
	// Most users of the library will not need to use this method.
	VisCoreExport void AllocateUsingPrevoiusShapeIfPossible(
			const CVisShape& refshape);

	// @cmember
	// Set the meory block (and memory block coordinates) used with
	// this image.
	void SetMemBlock(const CVisMemBlock& refmemblock,
			const CVisShape& refshapeMemBlock);

	// @cmember
	// Set the meory block (and memory block coordinates) used with
	// this image.
	VisCoreExport void SetMemBlock(const CVisMemBlock& refmemblock,
			const CVisShape& refshapeMemBlock,
			const CVisShape& refshapeImage);


	void SetMemBlock(const CVisMemBlock& refmemblock,
		int dxWidth, int dyHeight);


	//------------------------------------------------------------------
	// @group Copying Images

	// @cmember
	// Copy pixels from this image to another image.  This may fail
	// if the pixels in the source image can't be converted to values
	// which can be used to set the pixels in the destination image.
	bool CopyPixelsTo(CVisImageBase& refimageDest,
			EVisNormalize evisnormalize = evisnormalizeDefault,
			double dblMin = 1.0, double dblMax = - 1.0) const;

	// @cmember
	// Copy pixels from this image to another image.  This may fail
	// if the pixels in the source image can't be converted to values
	// which can be used to set the pixels in the destination image.
	bool CopyPixelsTo(CVisImageBase& refimageDest,
			const RECT& refrectSrc,
			EVisNormalize evisnormalize = evisnormalizeDefault,
			double dblMin = 1.0, double dblMax = - 1.0) const;

	// @cmember
	// Copy pixels from this image to another image.  This may fail
	// if the pixels in the source image can't be converted to values
	// which can be used to set the pixels in the destination image.
	bool CopyPixelsTo(CVisImageBase& refimageDest,
			const POINT pointDest,
			EVisNormalize evisnormalize = evisnormalizeDefault,
			double dblMin = 1.0, double dblMax = - 1.0) const;

	// @cmember
	// Copy pixels from this image to another image.  This may fail
	// if the pixels in the source image can't be converted to values
	// which can be used to set the pixels in the destination image.
	 bool CopyPixelsTo(CVisImageBase& refimageDest,
			const RECT& refrectSrc, const POINT pointDest,
			EVisNormalize evisnormalize = evisnormalizeDefault,
			double dblMin = 1.0, double dblMax = - 1.0) const;

	// @cmember
	// Copy pixels from this image to another image.  This may fail
	// if the pixels in the source image can't be converted to values
	// which can be used to set the pixels in the destination image.
	bool CopyPixelsTo(int bandSrc, int bandDest, CVisImageBase& refimageDest,
			EVisNormalize evisnormalize = evisnormalizeDefault,
			double dblMin = 1.0, double dblMax = - 1.0) const;

	// @cmember
	// Copy pixels from this image to another image.  This may fail
	// if the pixels in the source image can't be converted to values
	// which can be used to set the pixels in the destination image.
	bool CopyPixelsTo(int bandSrc, int bandDest, CVisImageBase& refimageDest,
			const RECT& refrectSrc,
			EVisNormalize evisnormalize = evisnormalizeDefault,
			double dblMin = 1.0, double dblMax = - 1.0) const;

	// @cmember
	// Copy pixels from this image to another image.  This may fail
	// if the pixels in the source image can't be converted to values
	// which can be used to set the pixels in the destination image.
	bool CopyPixelsTo(int bandSrc, int bandDest, CVisImageBase& refimageDest,
			const POINT pointDest,
			EVisNormalize evisnormalize = evisnormalizeDefault,
			double dblMin = 1.0, double dblMax = - 1.0) const;

	// @cmember
	// Copy pixels from this image to another image.  This may fail
	// if the pixels in the source image can't be converted to values
	// which can be used to set the pixels in the destination image.
	bool CopyPixelsTo(int bandSrc, int bandDest, CVisImageBase& refimageDest,
			const RECT& refrectSrc, const POINT pointDest,
			EVisNormalize evisnormalize = evisnormalizeDefault,
			double dblMin = 1.0, double dblMax = - 1.0) const;


	//------------------------------------------------------------------
	// @group Windows GDI Access

	// @cmember
	// Fill a Windows <t BITMAPINFOHEADER> structure with information
	// about this image.  This may fail for some nonstandard images.
	VisCoreExport bool FillBitmapInfoHeader(BITMAPINFOHEADER *pbmih,
			int cbitPixel = 0) const;

	// @cmember
	// If this image has 3-byte or 4-byte pixels containing unsigned
	// RGB (or RGBA) values, this method will copy the image pixel
	// values to a device context using the Windows
	// <f SetDIBitsToDevice> function.
	bool DisplayInHdc(HDC hdc, int xDest = 0, int yDest = 0,
			DWORD dwRop = SRCCOPY, int cbitPixel = 0) const;

	// @cmember
	// If this image has 3-byte or 4-byte pixels containing unsigned
	// RGB (or RGBA) values, this method will copy the image pixel
	// values to a device context using the Windows
	// <f SetDIBitsToDevice> function.
	bool DisplayInHdc(HDC hdc, const POINT& pointDest,
			DWORD dwRop = SRCCOPY, int cbitPixel = 0) const;

	// @cmember
	// If this image has 3-byte or 4-byte pixels containing unsigned
	// RGB (or RGBA) values, this method will copy the image pixel
	// values to a device context using the Windows
	// <f SetDIBitsToDevice> function.
	VisCoreExport bool DisplayInHdc(HDC hdc, const RECT& rectSrc,
			int xDest = 0, int yDest = 0, DWORD dwRop = SRCCOPY,
			int cbitPixel = 0) const;

	// @cmember
	// If this image has 3-byte or 4-byte pixels containing unsigned
	// RGB (or RGBA) values, this method will copy the image pixel
	// values to a device context using the Windows
	// <f SetDIBitsToDevice> function.
	bool DisplayInHdc(HDC hdc, const RECT& rectSrc,
			const POINT& pointDest, DWORD dwRop = SRCCOPY,
			int cbitPixel = 0) const;

	// @cmember
	// If this image has 3-byte or 4-byte pixels containing unsigned
	// RGB (or RGBA) values, this method will copy the image pixel
	// values to a device context using the Windows
	// <f StretchDIBits> function.
	// This version can mirror the image.
	VisCoreExport bool DisplayInHdc(HDC hdc, const RECT& rectSrc,
			const RECT& rectDest, DWORD dwRop = SRCCOPY,
			int cbitPixel = 0, bool fMirror = false) const;

	// @cmember
	// Create a Windows device context that can be used to draw into
	// this image.  If this method fails, 0 if returned.  The class
	// will manage the device context after it is created, so the
	// caller does not need to call Windows functions to destroy the
	// device context.  The coordinates used with the device context
	// will be relative to the StartPoint() of this image.
	VisCoreExport HDC Hdc(void);

	// @cmember
	// Destroy the Windows device context that was be used to draw into
	// this image.  This function will free resources used to draw into
	// this image.  Callers do not need to call this function, but they
	// may want to call it to free resources when they have finished
	// drawing into the image.
	VisCoreExport void DestroyHdc(void);

	// @cmember
	// Given a Windows memory device context, create a device-dependent
	// Windows <t BITMAP> using the Windows <f CreateDIBSection>
	// function.
	VisCoreExport HBITMAP HBitmapCreateForHdc(HDC hdcMem) const;


	//------------------------------------------------------------------
	// @group Pixel Type Information

	// @cmember
	// Returns a union of <t EVisPixFmt> values which give type
	// information about the pixels used in this image.
	EVisPixFmt PixFmt(void) const;


    //------------------------------------------------------------------
	// @group C++ Type Information

	// @cmember
	// Information about the pixels in this image.
	virtual VisCoreExport const type_info& PixelTypeInfo(void) const;

	// @cmember
	// Information about the type of this object.  The iLevel
	// argument specifies the class depth (with CVisImageBase being
	// at depth 0).  Use - 1 to find information about the deepest
	// class.
	virtual const type_info& ObjectTypeInfo(int iLevel = - 1) const;


    //------------------------------------------------------------------
	// @group Values Associated with Images

	// @cmember
	// Get a name associated with an image.
	VisCoreExport const char *Name(bool fIncludePath = false) const;

	// @cmember
	// Associate a name with an image.
	void SetName(const char *name);

	// @cmember
	// Get a time associated with an image.
	FILETIME Filetime(void) const;

	// @cmember
	// Associate the current time with an image.
	void SetFiletime(void);

	// @cmember
	// Associate a time with an image.
	void SetFiletime(FILETIME filetime);


	//------------------------------------------------------------------
	// @group Alias and Copy

	// @cmember
	// Change this image to use the information from another image.
	virtual VisCoreExport void Alias(const CVisImageBase& refimage);

	// @cmember
	// Change this image to use a copy of information from another image.
	virtual VisCoreExport void Copy(const CVisImageBase& refimage);


	VisCoreExport void Alias(IDirectDrawSurface* pIDDS);

	VisCoreExport void Alias(IVisImage* pVisImage, CVisMemBlock& refMemBlock);


	// Alias an image described using Intel's IPL image structure.
	VisCoreExport void Alias(struct _IplImage *piplimage);


	//------------------------------------------------------------------
	// @group Miscellaneous

	// @cmember
	// Is this a valid image?  (Does it have a non-empty memory block
	// allocated for pixels?)
	bool IsValid(void) const;

	// @cmember
	// Clear the image pixels by filling all bytes in the pixel memory
	// with zero values.  This method could cause problems if used with
	// images which use some used-defined pixel types.
	VisCoreExport void ClearPixels(void);

	// @cmember
	// Make a copy of this image or create an image of the same type as
	// this image with the specified dimensions.  Most users of the
	// library should not use this method.  Instead, they should use the
	// <mf CVisImage::SubImage> methods in the typed image classes.
	// The pointer returned must be deleted using the C++ <f delete>
	// operator when the caller has finished using the image.
	virtual VisCoreExport CVisImageBase *Clone(const CVisShape *pshape = 0)
			const;

	// @cmember
	// Copy the image options specified when this image was constructed.
	int Options(void) const;


	//------------------------------------------------------------------
	// @group File I/O (may change in final version)

	// @cmember
	// Read an image from a file.
	VisCoreExport void ReadFile(const char *szFilename = 0);

	// @cmember
	// Read an image from a file.
	void VisCoreExport ReadFile(SVisFileDescriptor& reffiledescriptor);

	// @cmember
	// Write the image to a file.
	VisCoreExport void WriteFile(const char *szFilename = 0,
			const char *szFormat = 0);

	// @cmember
	// Write the image to a file.
	void VisCoreExport WriteFile(SVisFileDescriptor& reffiledescriptor);

	// @cmember
	// Read an image from a file.  This method will not throw
	// an exception if there's an error reading the file.  Instead,
	// it returns true if the file could be read or false if there
	// was an error reading the file.
	// Warning:  This method is not documented and may not be
	// available in future releases.
	// LATER:  Add szBuf and cbBuf arguments to get the error message.
	VisCoreExport bool FReadFile(const char *szFilename = 0,
			bool fDisplayErrorMessage = false);

	// @cmember
	// Read an image from a file.  This method will not throw
	// an exception if there's an error reading the file.  Instead,
	// it returns true if the file could be read or false if there
	// was an error reading the file.
	// Warning:  This method is not documented and may not be
	// available in future releases.
	// LATER:  Add szBuf and cbBuf arguments to get the error message.
	bool VisCoreExport FReadFile(SVisFileDescriptor& reffiledescriptor,
			bool fDisplayErrorMessage = false);

	// @cmember
	// Write the image to a file.  This method will not throw
	// an exception if there's an error writing the file.  Instead,
	// it returns true if the file could be written or false if there
	// was an error writing the file.
	// Warning:  This method is not documented and may not be
	// available in future releases.
	// LATER:  Add szBuf and cbBuf arguments to get the error message.
	VisCoreExport bool FWriteFile(const char *szFilename = 0,
			const char *szFormat = 0,
			bool fDisplayErrorMessage = false);

	// @cmember
	// Write the image to a file.  This method will not throw
	// an exception if there's an error writing the file.  Instead,
	// it returns true if the file could be written or false if there
	// was an error writing the file.
	// Warning:  This method is not documented and may not be
	// available in future releases.
	// LATER:  Add szBuf and cbBuf arguments to get the error message.
	bool VisCoreExport FWriteFile(SVisFileDescriptor& reffiledescriptor,
			bool fDisplayErrorMessage = false);

	// @cmember
	// Associates a stream handler and frame number with the image.
    // [F][Read|Write]File should be used to actually read or write the 
    // frame.
    VisCoreExport void SetStreamHandler( CVisStreamHandler* pStrmHandler,
                                         UINT frame_num );

	// @cmember
	// Returns the currently associated stream handler.
    VisCoreExport CVisStreamHandler* StreamHandler(
        UINT *pFrameNum = NULL) const;


    //------------------------------------------------------------------
	// @group Self-describing stream I/O

    // LATER:  We might want to add colormap information or imopts.
	enum FieldType { eftName, eftPixFmt, eftShape, eftFilename, eftProperties,
			eftEnd};

	// @cmember
	// Read or write a field.  Returns a description of the field.
    const char *ReadWriteField(class CVisSDStream& s,
			int field_id);

	// @cmember
	// Read or write properties to a self-describing stream.
	virtual void SDReadWriteProperties(class CVisSDStream& s,
			bool fAddComma = false);

	//------------------------------------------------------------------
	// @group Color Map Support (may change in final version)

	// @cmember
	// Should a color map be used with this image, if one is
	// available?
	// Color maps are only supported for 8-bit pixels.
	// This may change in the final version of the SDK.
	bool UseColorMap(void) const;

	// @cmember
	// Should a color map be used with this image, if one is
	// available?
	// Color maps are only supported for 8-bit pixels.
	// This may change in the final version of the SDK.
	void SetUseColorMap(bool fUseColorMap = true);

	// @cmember
	// Get a <c CMemBlock> object containing a colormap to use with
	// this image.  The pointer in the <c CMemBlock> object may be 0.
	// Color maps are only supported for 8-bit pixels.
	// This may change in the final version of the SDK.
	const CVisMemBlock& ColorMap(void) const;

	// @cmember
	// Set a <c CMemBlock> object containing a colormap to use with
	// this image.  The pointer in the <c CMemBlock> object may be 0.
	// Color maps are only supported for 8-bit pixels.
	// This may change in the final version of the SDK.
	void SetColorMap(const CVisMemBlock& memblockColorMap);


	//------------------------------------------------------------------
	// @group Pixel Value Strings

	// @cmember
	// Given the location of a pixel value, get a string representing
	// the pixel value, if the derived class knows how to get such a
	// string.  This is used in the display code.  Most users of the
	// library will not need to use this method.
	int GetPixelValueString(const void *pixeladdress,
			char *szValue, int cbValue, int nFieldWidth = 1, int nDecPlaces = 6)
			const;

	// @cmember
	// Given the location of a pixel value, get a string representing
	// the pixel value, if the derived class knows how to get such a
	// string.  This is used in the display code.  Most users of the
	// library will not need to use this method.
	int GetPixelValueString(POINT point,
			char *szValue, int cbValue, int nFieldWidth = 1, int nDecPlaces = 6)
			const;

	// @cmember
	// Given the location of a pixel value, get a string representing
	// the pixel value, if the derived class knows how to get such a
	// string.  This is used in the display code.  Most users of the
	// library will not need to use this method.
	int GetPixelValueString(POINT point, int band,
			char *szValue, int cbValue, int nFieldWidth = 1, int nDecPlaces = 6)
			const;

	// @cmember
	// Given the location of a pixel value, get a string representing
	// the pixel value, if the derived class knows how to get such a
	// string.  This is used in the display code.  Most users of the
	// library will not need to use this method.
	int GetPixelValueString(int x, int y,
			char *szValue, int cbValue, int nFieldWidth = 1, int nDecPlaces = 6)
			const;

	// @cmember
	// Given the location of a pixel value, get a string representing
	// the pixel value, if the derived class knows how to get such a
	// string.  This is used in the display code.  Most users of the
	// library will not need to use this method.
	int GetPixelValueString(int x, int y, int band,
			char *szValue, int cbValue, int nFieldWidth = 1, int nDecPlaces = 6)
			const;


	//------------------------------------------------------------------
	// @group Property List for user-defined properties

	const CVisPropList& PropList(void) const;

	CVisPropList& PropList(void);


	//------------------------------------------------------------------
	// @group Default settings for all images

	// When writing RGBA images to a file, should Alpha information
	// be written?
	static VisCoreExport bool IsAlphaWritten(void);
	static VisCoreExport void SetIsAlphaWritten(bool f);

	// When writing BMP files for images whose row width is not a multiple
	// of 4 bytes, should the width in the BMP file header be increased to
	// account for the padding added when rows are written to the file.
	// UNDONE:  This is a temporary flag that may be removed in the future.
	static VisCoreExport bool DoesBMPFileWidthIncludePadding(void);
	static VisCoreExport void SetDoesBMPFileWidthIncludePadding(bool f);

	// Default image options (EVisImOpt values)
	static VisCoreExport int DefaultOptions(void);
	static VisCoreExport void SetDefaultOptions(int wOptions);



    //------------------------------------------------------------------
	// SDStream I / O

	// If IsDirty() is true, the image data will be written to a file
	// when the image information is written in WriteValue().
	bool IsDirty(void) const;
	void SetIsDirty(bool f);

	// If DelayRead() is false, the image data will be read from a file
	// when the image information is read in ReadValue().
	bool DelayRead(void) const;
	void SetDelayRead(bool f);

	virtual VisCoreExport void ReadValue(CVisSDIStream& refsdistream,
			bool fForceDelayRead = false);
	virtual VisCoreExport void WriteValue(CVisSDOStream& refsdostream) const;

	void VisCoreExport BuildPropList(CVisPropList& refproplist);

	// These two methods are overridden in derived classes to give type
	// information (about the derived classes) to the property list code.
	virtual VisCoreExport CVisPropTypeInfoBase *PropTypeInfoNew(void) const;
	virtual VisCoreExport CVisPropTypeInfoBase *PropTypeInfoLookup(void) const;


	// Constant used to specify the depth of this object (for use with
	// ObjectTypeInfo).
	enum { eilevelImageBase = 0 };
	enum { eiLevel = 0 };


// @access Protected Members
protected:

#ifdef VIS_INCLUDE_SDSTREAM
	// UNDONE (in Rel1):  Remove this when no longer needed.
	friend class CVisSDObject< CVisImageBase >;
#endif

	//------------------------------------------------------------------
	// @group Constructors

	// @cmember:(IMPL)
	// Constructor is protected.  Only derived classes can be created.
	VisCoreExport CVisImageBase(int cbPixel, EVisPixFmt evispixfmt,
			const RECT& rect, int nbands, int imopts, BYTE *pbData);

	// @cmember:(IMPL)
	// Constructor is protected.  Only derived classes can be created.
	VisCoreExport CVisImageBase(CVisMemBlock& refmemblock, int cbPixel,
			EVisPixFmt evispixfmt, const RECT& rect,
			int nbands, int imopts);

	// @cmember:(IMPL)
	// Constructor is protected.  Only derived classes can be created.

	// Default values added for use with with self-describing stream objects.
	// UNDONE (in Rel1):  Remove default parameters eventally.
	VisCoreExport CVisImageBase(int cbPixel = 1,
			EVisPixFmt evispixfmt = evispixfmtUnknown);

	
	// @cmember:(IMPL)
	// UNDONE:  Remove this later.  (It is kept here so that the exports from
	// VisCore.dll will not change from version 1.0, but this should never be
	// called.)
	VisCoreExport CVisImageBase(const CVisImageBase& refimage);

	// @cmember
	// Copy an image.  This is called by the assignment operator.
	// Derived classes should implement this
	// method to make sure that images get copied correctly when a
	// pointer to the base class is used.
	virtual VisCoreExport void Assign(const CVisImageBase& refimage);


	//------------------------------------------------------------------
	// @group:(IMPL) Copying Images

	// @cmember:(IMPL)
	// Copy this image to another image.  This may fail if the
	// pixels in the source image can't be converted to values
	// which can be used to set the pixels in the destination image.
	// This method is called by the public <mf CVisImageBase::CopyPixelsTo>
	// methods.
	virtual VisCoreExport bool RealCopyPixelsTo(int bandSrc, int bandDest,
			CVisImageBase& refimageDest, const RECT& refrectSrc,
			const POINT pointDest, EVisNormalize evisnormalize,
			double dblMin = 1.0, double dblMax = - 1.0) const;

	// @cmember:(IMPL)
	// Get a pointer to an array of <t unsigned long> values giving
	// a color map to use with this image.
	VisCoreExport const unsigned long *MpbulColorMap(int cbitPixel = 8) const;


    //------------------------------------------------------------------
	// @group:(IMPL) Helper Functions

	// @cmember
	// Read an image from a file or write the image to a file.
	virtual bool ReadWriteFile(SVisFileDescriptor& reffiledescriptor,
			bool fWrite, bool fThrowError, bool fDisplayErrorMessage);

	// @cmember
	// Helper function called by the base class's implementation of
	// ReadWriteFile.
	VisCoreExport bool RealReadWriteFile(SVisFileDescriptor& reffiledescriptor,
			bool fWrite, bool fThrowError, bool fDisplayErrorMessage);

	// @cmember:(IMPL)
	// Compute memory pointers after memory block, cbCol, or cbRow
	// are changed.
	VisCoreExport void ComputeMemPointers(void);

	VisCoreExport void ComputeAliasShapes(int cbPointSrc, const RECT& refrect, 
										  const RECT& refrectMem);


	// @cmember
	// Given the location of a pixel value, get a string representing
	// the pixel value, if the derived class knows how to get such a
	// string.  This is used in the display code.  Most users of the
	// library will not need to use this method.
	virtual VisCoreExport int RealGetPixelValueString(const void *pixeladdress,
			char *szValue, int cbValue, int nFieldWidth = 1, int nDecPlaces = 6)
			const;

	// Get a reference to a CVisShape object whose members will all be zero
	// when the image memory has been deallocated.  This method is used
	// to return references to empty shapes in methods like Shape().  It
	// makes assumptions about the order of CVisImageBase members (because
	// it casts a group of members to a CVisShape object).
	const CVisShape& ShapeInvalid(void) const;


	virtual VisCoreExport void PreReadValue(CVisSDIStream& refsdistream);
	virtual VisCoreExport void PostReadValue(CVisSDIStream& refsdistream);

	virtual VisCoreExport void PreWriteValue(CVisSDOStream& refsdostream)
			const;
	virtual VisCoreExport void PostWriteValue(CVisSDOStream& refsdostream)
			const;


	static VisCoreExport int GCD(int a, int b);

//	bool ReadFileUsingIPicture(SVisFileDescriptor& fd);

// @access:(IMPL) Private Members
private:
    //------------------------------------------------------------------
	// @group:(IMPL) Implementation Variables

	// @cmember:(IMPL)
	// The image rectangle and number of bands of pixels in each image
	// point.
	CVisShape m_shapeImage;

	// @cmember:(IMPL)
	// The type of this pixel (if it is a standard pixel type).
	EVisPixFmt m_evispixfmt;

	// @cmember:(IMPL)
	// The memory rectangle containing the image pixels and the number
	// of bands of pixels in each image point.
	CVisShape m_shapeMemBlock;

	// @cmember:(IMPL)
	// The image pixels.
	CVisMemBlock m_memblock;

	// @cmember:(IMPL)
	// The name associated with this image.
	std::string m_strName;

	// @cmember:(IMPL)
	// The stream handler associated with this image
    CVisStreamHandler* m_pVisStreamHandler;
	
    // @cmember:(IMPL)
	// The frame number within a stream associated with image
    UINT m_uStreamFrameNum;

	// @cmember:(IMPL)
	// The size of image pixels in bytes.
	int m_cbPixel;

	// @cmember:(IMPL)
	// The number of bytes between pixels in adjacent columns
	// of the same row and band.
	// This member must immediately follow m_cbPixel for methods
	// that call ShapeInvalid(), like the Shape() method, to work
	// correctly after the image has been deallocated.
	int m_cbCol;

	// @cmember:(IMPL)
	// The number of bytes between pixels in adjacent rows of the
	// same column and band.
	// This member must immediately follow m_cbCol for methods
	// that call ShapeInvalid(), like the Shape() method, to work
	// correctly after the image has been deallocated.
	int m_cbRow;

	// @cmember:(IMPL)
	// Pointer to the current origin of the image data.
	// This member must immediately follow m_cbRow for methods
	// that call ShapeInvalid(), like the Shape() method, to work
	// correctly after the image has been deallocated.
	BYTE *m_pbDataOrigin;

	// @cmember:(IMPL)
	// Pointer to the first pixel in row 0.  Used with
	// <mf CVisImageBase::PbFirstPixelInRow> to avoid
	// unneeded multiply operatins.
	// This member must immediately follow m_pbDataOrigin for methods
	// that call ShapeInvalid(), like the Shape() method, to work
	// correctly after the image has been deallocated.
	BYTE *m_pbFirstPixelInRow0;

	// @cmember:(IMPL)
	// Handle to the memory device context used with Windows GDI functions.
	// This member must immediately follow m_pbFirstPixelInRow0 for methods
	// that call ShapeInvalid(), like the Shape() method, to work
	// correctly after the image has been deallocated.
	HDC m_hdc;

	// @cmember:(IMPL)
	// Handle to the bitmap that was in the memory device context before
	// the bitmap for this image was selected into the memory device context.
	// This member must immediately follow m_hdc for methods
	// that call ShapeInvalid(), like the Shape() method, to work
	// correctly after the image has been deallocated.
	HBITMAP m_hbitmapOld;

	// @cmember:(IMPL)
	// A time associated with an image.
	FILETIME m_filetime;

	// @cmember:(IMPL)
	// If m_fDirty is true, the image data will be written to a file
	// when the image information is written in WriteValue().
	// UNDONE:  Maybe this should be a shared, non-printing property?
	bool m_fDirty;

	// @cmember:(IMPL)
	// If m_fDelayRead is false, the image data will be read from a file
	// when the image information is read in ReadValue().
	bool m_fDelayRead;

	// @cmember:(IMPL)
	// Unused.  Added to explicitly align data on 4-byte boundries (since
	// sizeof(bool) == 1).
	bool m_fReserved;

	// @cmember:(IMPL)
	// Should a color map be used with this image, if one is
	// available?
	// Color maps are only supported for 8-bit pixels.
	// This may change in the final version of the SDK.
	bool m_fUseColorMap;

	// @cmember:(IMPL)
	// The colormap for this image.
	// @comm:(IMPL) This may change in the final release of the SDK.
	CVisMemBlock m_memblockColorMap;

	// @cmember:(IMPL)
	// Options specified when this image was constructed.
	int m_imopts;

#ifdef VIS_CORE_IMAGE_USE_ILIFFE
	BYTE *m_pbOriginIliffe;
	BYTE **m_ppbIliffe;
	CVisMemBlockOf<BYTE *> m_memblockIliffe;
#endif // VIS_CORE_IMAGE_USE_ILIFFE

	// @cmember:(IMPL)
	// Property list (for user-defined properties).
	CVisPropList m_proplist;

	// @cmember:(IMPL)
	// Cleanup method called by the destructor.
	VisCoreExport void Cleanup(void);
};


#ifndef VIS_ONLY_DECLARE_PROPLIST

VIS_DECLARE_PROP_LIST_CLASS_VIRTUAL(VisCoreExportDeclspec, CVisImageBase)

#ifndef VIS_NO_SD_STREAM

// We need to use a macro for each I/O function because we want to
// use the class property list to find type information but use
// class ReadValue / WriteValue methods when reading and writing.
VIS_DECLARE_SDI_FIND_TYPES_PL(VisCoreExportDeclspec, CVisImageBase)
VIS_DECLARE_SDI_READ_VALUE_USING_CLASS_METHOD(VisCoreExportDeclspec, CVisImageBase)
VIS_DECLARE_SDI_READ_OBJ_DEFAULT(VisCoreExportDeclspec, CVisImageBase)
VIS_DECLARE_SDI_READ_OP_DEFAULT(VisCoreExportDeclspec, CVisImageBase)

VIS_DECLARE_SDO_FIND_TYPES_PL(VisCoreExportDeclspec, CVisImageBase)
VIS_DECLARE_SDO_WRITE_VALUE_USING_CLASS_METHOD(VisCoreExportDeclspec, CVisImageBase)
VIS_DECLARE_SDO_WRITE_OBJ_DEFAULT(VisCoreExportDeclspec, CVisImageBase)
VIS_DECLARE_SDO_WRITE_OP_DEFAULT(VisCoreExportDeclspec, CVisImageBase)

#endif // !VIS_NO_SD_STREAM

#endif // VIS_ONLY_DECLARE_PROPLIST


// Helper functions that assert that the specified images all have the
// same CVisShape.
void VisCheckConformal(const CVisImageBase *pimage1,
		const CVisImageBase *pimage2);
void VisCheckConformal(const CVisImageBase *pimage1,
		const CVisImageBase *pimage2,
		const CVisImageBase *pimage3, ...);


// This will call virtual functions to write the correct information
// for derived classes.
#ifdef VIS_INCLUDE_SDSTREAM
CVisSDStream& operator<<(CVisSDStream& s, CVisImageBase& refimage);
#endif

// Inline functions
#include "VisImageBase.inl"


#endif // __VIS_CORE_IMAGEBASE_H__

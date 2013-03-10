// VisNormalize.h
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved



////////////////////////////////////////////////////////////////////////////
//  
// @enum EVisNormalize |
// Options specified when making normalized copies of images in
// the typed-image subclasses.
//
// @devnote  Are there other normalization options that we want to add to
//  this list.
//
////////////////////////////////////////////////////////////////////////////
enum EVisNormalize
{
	// @emem
	// When images have the same pixel type, the pixel values are copied
	// without any normalization.
	// When copying (possibly unsigned) integer values to (possibly
	// unsigned) integer values, the source pixel bits will be shifted
	// if the source and destination pixel have different number of
	// significant pixel bits (e.g. from <t short> to <t long>), and
	// the a constant will be added if exactly one of the source and
	// destination pixel types is unsigned.  When at least one of the
	// source or destination pixel values are doubles, the pixel values
	// will be copied without any normalization.
	evisnormalizeAdjustNums = 0,
	evisnormalizeDefault = 0,
	evisnormalizeFirst = 0,

	// @emem
	// Use casts to convert pixels.  This may cause problems when
	// converting pixel values to <c BYTE> values for display.
	evisnormalizeCastValues,

	// @emem
	// Copy pixels from images having the same pixel type.  This will
	// fail if the images have different pixel types.
	evisnormalizeCopyBytesSameType,

	// @emem
	// Copy pixels from images having the same pixel size.  This will
	// fail if the images have different pixel sizes.
	evisnormalizeCopyBytesSameSize,

	// @emem
	// Find the minimum and maximum values of the source pixels to be copied.
	// Compute the destination pixel values by linearly scaling the source
	// pixel values so that the minimum source pixel values is mapped to the
	// lowest possible destination pixel value and the maximum source pixel
	// value is mapped to the largest possible destination pixel value.
	evisnormalizeRelative,

	// @emem
	// Find the maximum magnitude of the values of the source pixels to be copied.
	// Compute the destination pixel values by linearly scaling the source
	// pixel values.
	// For signed source pixel values, map the negative of the maximum magnitude
	// of the source pixel values to the lowest possible destination pixel value.
	// For unsigned source pixel values, map zero to the lowest possible
	// destination pixel value.
	// Map the maximum magnitude of the values of the source pixels to the
	// largest possible destination pixel value.
	evisnormalizeAbsolute,

	// @emem
	// Find the minimum magnitude and maximum magnitude of the values of the
	// source pixels to be copied.
	// Compute the destination pixel values by linearly scaling the source
	// pixel magnitudes so that the minimum magnitude of the source pixel
	// values is mapped to the lowest possible destination pixel value
	// and the maximum magnitude of the source pixel values is mapped
	// to the largest possible destination pixel value.
	evisnormalizeRelMagnitude,

	// @emem
	// Find the maximum magnitude of the values of the source pixels to be copied.
	// Compute the destination pixel values by linearly scaling the source
	// pixel magnitudes so that the zero is mapped to the lowest possible
	// destination pixel value and the maximum magnitude of the source pixel values
	// is mapped to the largest possible destination pixel value.
	evisnormalizeAbsMagnitude,

	evisnormalizeLim
};


#define VIS_USE_OLD_NORM_NAMES 1

#ifdef VIS_USE_OLD_NORM_NAMES

#define evisnormAdjustInts				evisnormalizeAdjustNums
#define evisnormCastValues				evisnormalizeCastValues
#define evisnormCopyBytesSameType		evisnormalizeCopyBytesSameType
#define evisnormCopyBytesSameSize		evisnormalizeCopyBytesSameSize
#define evisnormFixed0					evisnormalizeAbsolute
#define evisnormMagnitude				evisnormalizeRelMagnitude
#define evisnormFixed0Magnitude			evisnormalizeAbsMagnitude
#define evisnormLim						evisnormalizeLim

#endif // VIS_USE_OLD_NORM_NAMES


template<class TPixel>
inline bool VisIsSigned(const TPixel& refpixel);

template<class TPixel>
inline TPixel VisRangeMin(const TPixel& refpixel, bool fBoundFp = false);

template<class TPixel>
inline TPixel VisRangeMax(const TPixel& refpixel, bool fBoundFp = false);


template<class TPixel>
class CVisFnOpFindMinMax
{
public:
	inline CVisFnOpFindMinMax(void);
	inline void operator()(const TPixel& refpixel);
	inline TPixel Min(void) const;
	inline TPixel Max(void) const;
	inline TPixel MinAbs(void) const;
	inline TPixel MaxAbs(void) const;
	inline double DblMaxAbs(void) const; // Handles 1 << 31.
	inline double DblMin(EVisNormalize evisnormalize) const;
	inline double DblMax(EVisNormalize evisnormalize) const;
protected:
	TPixel m_pixelMin;
	TPixel m_pixelMax;
	TPixel m_pixelMinAbs;
	const bool m_fIsSigned;
};


#include "VisNormalize.inl"

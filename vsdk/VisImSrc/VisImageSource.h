////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISIMAGESOURCE
//
// @module VisImageSource.h |
//
// This file defines the <c CVisImageSource> class.  <c CVisImageSource>
// objects are created using the <f VisFindImageSource> function.  They
// handle communication with an object created in a device-specific DLL
// that provides images through a standard interface.  <c CVisSequence>
// objects are connected to <c CVisImageSource> objects to get images
// from a digitizer.
//
// <nl>
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisImageSource\.inl> <l VisImageSource\.cpp>
//
// <nl>
// @head3 Classes |
// @index class | VISIMAGESOURCE
//
// @head3 Methods |
// @index cmember | VISIMAGESOURCE
//
////////////////////////////////////////////////////////////////////////////


#ifndef __VIS_IMSRC_VISIMAGESOURCE_H__
#define __VIS_IMSRC_VISIMAGESOURCE_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// Forward delcaration.
class CVisSequenceBase;


////////////////////////////////////////////////////////////////////////////
//  
// @class
//  
// Image source objects are created using the <f VisFindImageSource>
// function.  These objects handle communication with an object created in a
// device-specific DLL that provides images through a standard interface.
//
// <c CVisSequence> objects are connected to <c CVisImageSource> objects
// to get images from a digitizer.
//
// @xref <c CVisSequence>
//
////////////////////////////////////////////////////////////////////////////
class CVisImageSource : public CVisImageSourceBase
{
// @access Public Members
public:
	// @cmember
	// Default constructor.  (This can also be used to construct an image
	// source object by specifying the interfaces that it should use
	// internally.)
	VisImSrcExport CVisImageSource(IVisImSrcDevice *pIVisImSrcDevice = 0,
			IVisImSrcSettings *pIVisImSrcSettings = 0);

	// @cmember
	// Construct this image source object by copying the internal interface
	// pointers used by another image source object.  The two image
	// source objects will then refer to the same device, so that a change
	// to the settings of one image source object will change the settings
	// of the other image source object as well.
	VisImSrcExport CVisImageSource(const CVisImageSource& refimagesource);

	// @cmember
	// Copy the internal interface
	// pointers used by another image source object.  The two image
	// source objects will then refer to the same device, so that a change
	// to the settings of one image source object will change the settings
	// of the other image source object as well.
	VisImSrcExport CVisImageSource& operator=(const CVisImageSource& refimagesource);

	// @cmember
	// Destructor.  This is virtual so that the the destructors of any
	// derived classes will be called when the base class is destroyed.
	VisImSrcExport virtual ~CVisImageSource(void);


	// @cmember
	// Is this image source object in a valid state?  An image source is
	// valid when it has valid pointers to the interfaces that it uses
	// internally.
	VisImSrcExport virtual bool IsValid(void) const;


	// @cmember
	// Is the specified pixel format a preffered pixel format for this image
	// source?  A preferred pixel format is one that is returned directly
	// from the device that the image source uses.
	VisImSrcExport bool IsPreferredPixFmt(EVisPixFmt evispixfmt) const;

	// @cmember
	// Is the specified pixel format a supported pixel format for this image
	// source?  A supported pixel format is one that the image source can
	// get (possibly by converting image formats) from the
	// device that the image source uses.
	VisImSrcExport bool IsSupportedPixFmt(EVisPixFmt evispixfmt) const;


	// @cmember
	// Does this image source have the specified dialog.
    inline bool HasDialog(EVisVidDlg evisviddlg) const;

	// @cmember
	// Display the specified dialog for this image source.
    inline long DoDialog(EVisVidDlg evisviddlg, HWND hwndParent = 0);


	// @cmember
	// Get the range of values used with the specifed setting.  (The range
	// will be empty if the setting is not used by the device that this
	// image source uses.)
	VisImSrcExport void GetImSrcRange(EVisImSrc evisimsrc, long *plMin, long *plMax,
			long *pdlStepping = 0, long *plDefault = 0,
			long *plFlags = 0) const;

	// @cmember
	// Get the value for the specified setting.
	inline long GetImSrcValue(EVisImSrc evisimsrc) const;

	// @cmember
	// Set the value for the specified setting.  This returns true if the value
	// could be set, false otherwise.
	inline bool SetImSrcValue(EVisImSrc evisimsrc, long lValue,
			long lFlags = evisimsrcflagAuto);


	// @cmember
	// Should we allow volatile image buffers in the images that are
	// stored in sequences?  If so, the pixel data for images in
	// sequences could become invalid after a short time.  If we choose
	// not to allow volatile images, we may need to copy pixel data
	// when adding images to sequences.
	inline bool AllowVolatileImages(void) const;

	// Indicate whether we shouldallow volatile image buffers in the images
	// that are stored in sequences.
	inline void SetAllowVolatileImages(bool f);

	// @cmember
	// Should we  continuously grab images in the background?  This may increase
	// the rate at which we can get images from the image source, but
	// it may also use up significant processor time, especially if
	// we don't need a high frame rate or if the driver that is grabbing
	// the images spends a lot of time in a high-priority thread.
	VisImSrcExport virtual bool UseContinuousGrab(void) const;

	// @cmember
	// Indicate whether we should continuously grab images in the background.
	VisImSrcExport virtual void SetUseContinuousGrab(bool f);


	// @cmember
	// Get a string used to identify the device used by this image source object.
	// After this device is destroyed (or in another program instance), this
	// string can be passed to the VisFindImageSource function to get an image
	// source object that uses the same device as this image source object uses.
    VisImSrcExport const char *SzID(void);

	// @cmember
	// Get a name used to describe the device used by this image source object.
	// This string can be displayed to the user.
    VisImSrcExport const char *SzName(void);

	VisImSrcExport virtual bool AddSequence(class CVisSequenceBase& refsequence,
			bool fModifyImEncoding = true, bool fUseContinuousGrab = true);
	VisImSrcExport void RemoveSequence(class CVisSequenceBase& refsequence);

// @access Protected Members
protected:
	friend class CVisSequenceBase;

	VisImSrcExport virtual void DeleteFromSequence(
			CVisSequenceBase& refsequence);
	VisImSrcExport virtual void NeedImage(CVisSequenceBase& refsequence);

	// Start getting another image.
	inline void GrabNextInBackground(void);

	static EVisImEncoding ImEncodingGetPixFmt(int evispixfmt);

	// These methods may be used by derived classes to access the
	// image source device without going through the CVisSequence
	// objects.  (For example, a derived class with its own frame
	// callback could with with raw data from a camera.)

	// @cmember
	// This protected member could be used by derived classes to access the
	// internal <i IVisImSrcDevice> interface used by this image source object.
	inline const IVisImSrcDevice *PIVisImSrcDeviceGet(void) const;

	// @cmember
	// This protected member could be used by derived classes to access the
	// internal <i IVisImSrcDevice> interface used by this image source object.
	inline IVisImSrcDevice *PIVisImSrcDeviceGet(void);

	// @cmember
	// This protected member could be used by derived classes to access the
	// internal <i IVisImSrcSettings> interface used by this image source
	// object.
	inline const IVisImSrcSettings *PIVisImSrcSettings(void) const;

	// @cmember
	// This protected member could be used by derived classes to access the
	// internal <i IVisImSrcSettings> interface used by this image source
	// object.
	inline IVisImSrcSettings *PIVisImSrcSettings(void);


private:
	IVisImSrcDevice *m_pIVisImSrcDevice;
	IVisImSrcSettings *m_pIVisImSrcSettings;
	CVisSequenceList *m_pSequenceList;
	std::string m_strID;
	std::string m_strName;
};


#endif // __VIS_IMSRC_VISIMAGESOURCE_H__

////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISIMSRCIFACE
//
// @module VisImSrcIFace.h |
//
// This file defines standard interfaces used to communicate with objects
// that provide images from digitizers.  (These objects are created in
// device-specific DLLs that are loaded explicitly as they are needed.)
// Users of the library will not need to use these interfaces unless they
// are implementing their own camera-specific DLL.
//
// <nl>
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @head3 Functions |
// @index func | VISIMSRCIFACE
//
// @head3 Interfaces |
// @index interface | VISIMSRCIFACE
//
// @head3 Constants |
// @index const enum | VISIMSRCIFACE
//
// @xref <l VisImSrcConst\.h>
//
////////////////////////////////////////////////////////////////////////////


#ifndef __VIS_IMSRC_VISIMSRCIFACE_H__
#define __VIS_IMSRC_VISIMSRCIFACE_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#ifndef VISAPI
#define VISAPI  __stdcall
#endif // !VISAPI



typedef void (VISAPI *VisImSrcMemCallback)(void *pvData, void *pvDeleteData);


#ifndef VIS_DEFINED_EVIS_CONV_YUV

	#define VIS_DEFINED_EVIS_CONV_YUV

	// See "VisConvertYUV for descriptions of these values.
	enum EVisConvYUV
	{
		evisconvyuvBasic			= 0,
		evisconvyuvFirst			= 0,
		evisconvyuvDigital,
		evisconvyuv422				= 1,  // Old name
		evisconvyuvDefault			= 1,  // Was 0 in the first SDK release
		evisconvyuvNTSCNoSat,
		evisconvyuvNTSCSat,
		evisconvyuvPALNoSat,
		evisconvyuvPALSat,
		evisconvyuvGrayscale,
		evisconvyuvUnscaled,
		evisconvyuvCbCrNoSat,
		evisconvyuvCbCrSat,
		evisconvyuvLim,
	};

#endif // VIS_DEFINED_EVIS_CONV_YUV


// @enum EVisImInf |
// Flags used to describe an image passed to a frame callback from
// a device that suppports the <i IVisImSrcDevice> interface.
enum EVisImInf
{
	// @emem
	// The image is vertically reflected.
	evisfiminfVertReflect			= 0x01,

	// @emem
	// The buffer for this image is volatile and may soon
	// become invalid.
	evisfiminfVolatileData			= 0x02,

	// @emem
	// The buffer for this image uses special DMA buffer memory.
	// (Since DMA memory is scarce, there can not be too many buffers
	// of this type in use at any one time.)
	evisfiminfDMABuffer				= 0x04
};


// @struct SVisImSrcFrame |
// This structure is passed to a frame callback function by an object
// that supports the <i IVisImSrcDevice> interface.
struct SVisImSrcFrame
{
	// @field
	// The offset and dimensions of this image.
	RECT m_rect;

	// @field
	// An image timestamp.  This may be anytime between the time that
	// the image was captured and the current time.
	FILETIME m_filetime;

	// @field
	// The image data.
	void *m_pvData;

	// @field
	// The size of the image data (in bytes).
	int m_cbData;

	// @field
	// The image encoding used to encode this image.
	// @xref <c EVisImEncoding>
	EVisImEncoding m_evisimencoding;

	// @field
	// The type of conversion recommended when converting YUV images to
	// RGB images.
	// @xref <c EVisConvYUV>
	EVisConvYUV m_evisconvyuv;

	// @field
	// A pointer to a function that can be called to delete the
	// image data.  If this is not NULL and the owner of the callback
	// function wants to take control of the image data (and be
	// responsible for calling this function when it is finished using
	// the image data), the callback function should set this field
	// and the next field to 0 before returning.
	VisImSrcMemCallback m_pfnDeleteData;

	// @field
	// Additional data that is passed to the function that deletes
	// the image data.
	void *m_pvDeleteData;

	// @field
	// An array of colormap values used with this image.  This pointer
	// may be NULL if not colormap is used (or if the system palette
	// is used).  This pointer may not be valid after the callback
	// function returns, so these values should be copied during the
	// callback function if they will be needed after the callback
	// function returns.
	RGBQUAD *m_prgbqColormap;

	// @field
	// The number of entries in the colormap.
	int m_crgbqColormap;

	// @field
	// <c EVisImInf> values ORed together to describe this image.
	DWORD m_dwFlags;

	// @field
	// Reserved for use by the object that supports the
	// <i IVisImSrcDevice> interface.
	DWORD dwReserved;
};



////////////////////////////////////////////////////////////////////////////
//  
// @func HRESULT | T_PfnVisGetImSrcProvider |
// Type of callback function used to get an <i IVisImSrcProvider>
// interface from a DLL.  A function of this type with the name
// "VisGetImSrcProvider" is exported from DLLs that create
// providers of image source devices.
//
// @parm <i IUnknown> * | pUnkOuter |
// Pointer to a <i IUnknown> interface for the provider object to use
// if it supports aggregation.
//
// @parm <c const char> * | szProvider |
// String identifying the provider.  This should match the name of
// the registrty key under the VIS_SZ_REG_IMSRC_DEVS registry key
// that describes the provider.  (This parameter can be used to allow
// a single DLL to support multiple providers.)
//
// @parm <c REFIID> * | riid |
// Reference to the interface ID for this provider.  Usually this is
// IID_IUnknown or IID_IVisImSrcProvider.
//
// @parm void * * | ppvObject |
// Pointer given to a pointer to the interface of the type specified
// by <p riid>.  This parameter is used to return the requested interface
// to the caller.  It should point to a NULL pointer if this function
// fails to create the requested interface on the specified provider.
//
// @xref <l VisImSrcConst\.h>
//
////////////////////////////////////////////////////////////////////////////
typedef HRESULT (STDAPICALLTYPE *T_PfnVisGetImSrcProvider)
		(LPUNKNOWN pUnkOuter, const char *szProvider, REFIID riid,
		void **ppvObject);


////////////////////////////////////////////////////////////////////////////
//  
// @func void | T_PfnVisImSrcFrameCallback |
// Type of callback function used to get images from an object that
// supports the <i IVisImSrcDevice> interface. 
//
// @parm <i IVisImSrcDevice> * | pIVisImSrcDevice |
// Pointer to the <i IVisImSrcDevice> interface of the object that is
// calling the callback function.
//
// @parm <c SVisImSrcFrame> * | pimsrcframe |
// Structure containing information describing the current image.
//
// @parm void * | pvUser |
// Pointer given to the <i IVisImSrcDevice> interface in the method
// that set the callback function.
//
// @xref <c SVisImSrcFrame>
//
////////////////////////////////////////////////////////////////////////////
typedef void (STDAPICALLTYPE *T_PfnVisImSrcFrameCallback)
		(interface IVisImSrcDevice *pIVisImSrcDevice,
		SVisImSrcFrame *pimsrcframe, void *pvUser);


// {BFDF2E00-0216-11d1-AA80-0000F803FF4F}  IID_IVisListSz
DEFINE_GUID(IID_IVisListSz, 
0xbfdf2e00, 0x216, 0x11d1, 0xaa, 0x80, 0x0, 0x0, 0xf8, 0x3, 0xff, 0x4f);

// {BFDF2E01-0216-11d1-AA80-0000F803FF4F}  IID_IVisImSrcProvider
DEFINE_GUID(IID_IVisImSrcProvider, 
0xbfdf2e01, 0x216, 0x11d1, 0xaa, 0x80, 0x0, 0x0, 0xf8, 0x3, 0xff, 0x4f);

// {BFDF2E02-0216-11d1-AA80-0000F803FF4F}  IID_IVisImSrcDevice
DEFINE_GUID(IID_IVisImSrcDevice, 
0xbfdf2e02, 0x216, 0x11d1, 0xaa, 0x80, 0x0, 0x0, 0xf8, 0x3, 0xff, 0x4f);

// {BFDF2E03-0216-11d1-AA80-0000F803FF4F}  IID_IVisImSrcSettings
DEFINE_GUID(IID_IVisImSrcSettings, 
0xbfdf2e03, 0x216, 0x11d1, 0xaa, 0x80, 0x0, 0x0, 0xf8, 0x3, 0xff, 0x4f);

// {BFDF2E04-0216-11d1-AA80-0000F803FF4F}  IID_IVisVideoProc
DEFINE_GUID(IID_IVisVideoProc, 
0xbfdf2e04, 0x216, 0x11d1, 0xaa, 0x80, 0x0, 0x0, 0xf8, 0x3, 0xff, 0x4f);

// {BFDF2E05-0216-11d1-AA80-0000F803FF4F}  IID_IVisCameraControl
DEFINE_GUID(IID_IVisCameraControl, 
0xbfdf2e05, 0x216, 0x11d1, 0xaa, 0x80, 0x0, 0x0, 0xf8, 0x3, 0xff, 0x4f);

// {BFDF2E02-0216-11d1-AA80-0000F803FF4F}  IID_IVisImSrcDevice
DEFINE_GUID(IID_IVisImSrcDevice2, 
0xbfdf2e06, 0x216, 0x11d1, 0xaa, 0x80, 0x0, 0x0, 0xf8, 0x3, 0xff, 0x4f);



#ifndef VISDEFAULT
#define VISDEFAULT(x)		= (x)
#endif // VISDEFAULT



#undef  INTERFACE
#define INTERFACE   IVisListSz

// @interface IVisListSz |
// This interface allows access to an ordered list of strings.  It is
// implemented by the <c CVisListSz> class.
DECLARE_INTERFACE_(IVisListSz, IUnknown)
{
	//------------------------------------------------------------------
	// @group IUnknown methods

    // @meth <c HRESULT> | QueryInterface |
	// This method is used to find other interfaces supported by this
	// OLE object.
	STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObject) PURE;

    // @meth <c ULONG> | AddRef |
	// This method is called to add a reference to this OLE object.
    STDMETHOD_(ULONG, AddRef) (THIS) PURE;

    // @meth <c ULONG> | Release |
	// This method is called to remove a reference to this OLE object.
    STDMETHOD_(ULONG, Release) (THIS) PURE;


	//------------------------------------------------------------------
	// @group IVisListSz methods

    // @meth <c LONG> | CszGet |
	// Returns the number of strings in the list.
    STDMETHOD_(LONG, CszGet) (THIS) CONST PURE;

    // @meth <c LONG> | IGetSz |
	// Given a string, find its index in the list.  If the string is
	// not in the list, this method will return a negative number.
    STDMETHOD_(LONG, IGetSz) (THIS_ LPCSTR sz) CONST PURE;

    // @meth <c LPCSTR> | SzGetI |
	// Get an indexed string from the list.  If the index is not valid,
	// a NULL string pointer will be returned.
    STDMETHOD_(LPCSTR, SzGetI) (THIS_ LONG i) CONST PURE;
};



#undef  INTERFACE
#define INTERFACE   IVisImSrcDevice

// @interface IVisImSrcDevice |
// This interface represents a device, like a camera, that produces
// images.  The <c CVisImageSource> class uses this interface internally
// to communicate with digitizers.
DECLARE_INTERFACE_(IVisImSrcDevice, IUnknown)
{
	//------------------------------------------------------------------
	// @group IUnknown methods

    // @meth <c HRESULT> | QueryInterface |
	// This method is used to find other interfaces supported by this
	// OLE object.
	STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObject) PURE;

    // @meth <c ULONG> | AddRef |
	// This method is called to add a reference to this OLE object.
    STDMETHOD_(ULONG, AddRef) (THIS) PURE;

    // @meth <c ULONG> | Release |
	// This method is called to remove a reference to this OLE object.
    STDMETHOD_(ULONG, Release) (THIS) PURE;


	//------------------------------------------------------------------
	// @group IVisImSrcDevice methods

    // @meth <c HRESULT> | IsPreferredImEncoding |
	// Is the specified image encoding a preferred image encoding for
	// this device?  A preferred image encoding is an image encoding
	// used to encode images returned by the device driver.
	STDMETHOD(IsPreferredImEncoding) (THIS_
			EVisImEncoding evisimencoding) CONST PURE;

    // @meth <c HRESULT> | IsSupportedImEncoding |
	// Is the specified image encoding a supported image encoding for
	// this device?  A supported image encoding is an image encoding
	// used to encode images passed to the frame callback function.
	// Software conversion may be needed to convert images from a
	// preferred image encoding to a supported image encoding.
	STDMETHOD(IsSupportedImEncoding) (THIS_
			EVisImEncoding evisimencoding) CONST PURE;

    // @meth <c HRESULT> | GrabNext |
	// Grab a frame.  The frame will be passed to the frame callback
	// function after it has been grabbed.  This method may return
	// before the frame has been grabbed or it may not return until
	// after the frame has been grabbed.
    STDMETHOD(GrabNext) (THIS) PURE;

    // @meth <c HRESULT> | AddBuffer |
	// Give a buffer to this device.  If this succeeds, the device
	// will use the buffer to store an image and return the buffer
	// in the frame callback function.  This method may fail if the
	// device does not support buffers allocated by the caller.
	STDMETHOD(AddBuffer) (THIS_ void *pvBuffer, ULONG cbBuffer,
			VisImSrcMemCallback pfnDeleteBuffer VISDEFAULT(0),
			void *pvDeleteBuffer VISDEFAULT(0)) PURE;

    // @meth <c HRESULT> | GetFrameCallback |
	// Get the current frame callback function.  This should be called
	// before setting the frame callback function.
	STDMETHOD(GetFrameCallback) (THIS_
			T_PfnVisImSrcFrameCallback *ppfnCallback, void **ppvUser)
			CONST PURE;

    // @meth <c HRESULT> | SetFrameCallback |
	// Set the frame callback function.  This will replace any existing
	// callback function.  Before setting the frame callback function,
	// the caller should see if there is an existing frame callback
	// function.  If there is, the new frame callback is responsible for
	// calling the existing frame callback function when it is called.
	// @xref <f T_PfnVisImSrcFrameCallback>
	STDMETHOD(SetFrameCallback) (THIS_
			T_PfnVisImSrcFrameCallback pfnCallback, void *pvUser) PURE;

    // @meth <c HRESULT> | HasDialog |
	// Does this device have the specified dialog?
    STDMETHOD(HasDialog) (THIS_ EVisVidDlg evisviddlg) PURE;

    // @meth <c LONG> | DoDialog |
	// Display the specified dialog, if this device has a dislog of the
	// specified type.
    STDMETHOD_(LONG, DoDialog) (THIS_ EVisVidDlg evisviddlg,
			HWND hwndParent VISDEFAULT(0)) PURE;

    // @meth <c HRESULT> | GetSzID |
	// Get a string that can be used to identify this device.  The string
	// should consist of a provider ID, a colon, and a string that can
	// be used to get this device from its provider.  (The caller should
	// be able to pass the sting to the <f VisFindImageSource> function
	// to construct a <c CVisImageSource> object that uses this device.)
    STDMETHOD(GetSzID) (THIS_ LPSTR sz, LONG *pcb) PURE;

    // @meth <c HRESULT> | GetSzName |
	// Get a string that can be displayed to the user to describe this device.
    STDMETHOD(GetSzName) (THIS_ LPSTR sz, LONG *pcb) PURE;
};


#undef  INTERFACE
#define INTERFACE   IVisImSrcDevice2

// @interface IVisImSrcDevice2 |
// This interface represents a device, like a camera, that produces
// images.  It may support property pages to change camera settings.
// The <c CVisImageSource> class uses this interface internally to
// communicate with digitizers.
DECLARE_INTERFACE_(IVisImSrcDevice2, IVisImSrcDevice)
{
	//------------------------------------------------------------------
	// @group IUnknown methods

    // @meth <c HRESULT> | QueryInterface |
	// This method is used to find other interfaces supported by this
	// OLE object.
	STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObject) PURE;

    // @meth <c ULONG> | AddRef |
	// This method is called to add a reference to this OLE object.
    STDMETHOD_(ULONG, AddRef) (THIS) PURE;

    // @meth <c ULONG> | Release |
	// This method is called to remove a reference to this OLE object.
    STDMETHOD_(ULONG, Release) (THIS) PURE;


	//------------------------------------------------------------------
	// @group IVisImSrcDevice methods

    // @meth <c HRESULT> | IsPreferredImEncoding |
	// Is the specified image encoding a preferred image encoding for
	// this device?  A preferred image encoding is an image encoding
	// used to encode images returned by the device driver.
	STDMETHOD(IsPreferredImEncoding) (THIS_
			EVisImEncoding evisimencoding) CONST PURE;

    // @meth <c HRESULT> | IsSupportedImEncoding |
	// Is the specified image encoding a supported image encoding for
	// this device?  A supported image encoding is an image encoding
	// used to encode images passed to the frame callback function.
	// Software conversion may be needed to convert images from a
	// preferred image encoding to a supported image encoding.
	STDMETHOD(IsSupportedImEncoding) (THIS_
			EVisImEncoding evisimencoding) CONST PURE;

    // @meth <c HRESULT> | GrabNext |
	// Grab a frame.  The frame will be passed to the frame callback
	// function after it has been grabbed.  This method may return
	// before the frame has been grabbed or it may not return until
	// after the frame has been grabbed.
    STDMETHOD(GrabNext) (THIS) PURE;

    // @meth <c HRESULT> | AddBuffer |
	// Give a buffer to this device.  If this succeeds, the device
	// will use the buffer to store an image and return the buffer
	// in the frame callback function.  This method may fail if the
	// device does not support buffers allocated by the caller.
	STDMETHOD(AddBuffer) (THIS_ void *pvBuffer, ULONG cbBuffer,
			VisImSrcMemCallback pfnDeleteBuffer VISDEFAULT(0),
			void *pvDeleteBuffer VISDEFAULT(0)) PURE;

    // @meth <c HRESULT> | GetFrameCallback |
	// Get the current frame callback function.  This should be called
	// before setting the frame callback function.
	STDMETHOD(GetFrameCallback) (THIS_
			T_PfnVisImSrcFrameCallback *ppfnCallback, void **ppvUser)
			CONST PURE;

    // @meth <c HRESULT> | SetFrameCallback |
	// Set the frame callback function.  This will replace any existing
	// callback function.  Before setting the frame callback function,
	// the caller should see if there is an existing frame callback
	// function.  If there is, the new frame callback is responsible for
	// calling the existing frame callback function when it is called.
	// @xref <f T_PfnVisImSrcFrameCallback>
	STDMETHOD(SetFrameCallback) (THIS_
			T_PfnVisImSrcFrameCallback pfnCallback, void *pvUser) PURE;

    // @meth <c HRESULT> | HasDialog |
	// Does this device have the specified dialog?
    STDMETHOD(HasDialog) (THIS_ EVisVidDlg evisviddlg) PURE;

    // @meth <c LONG> | DoDialog |
	// Display the specified dialog, if this device has a dislog of the
	// specified type.
    STDMETHOD_(LONG, DoDialog) (THIS_ EVisVidDlg evisviddlg,
			HWND hwndParent VISDEFAULT(0)) PURE;

    // @meth <c HRESULT> | GetSzID |
	// Get a string that can be used to identify this device.  The string
	// should consist of a provider ID, a colon, and a string that can
	// be used to get this device from its provider.  (The caller should
	// be able to pass the sting to the <f VisFindImageSource> function
	// to construct a <c CVisImageSource> object that uses this device.)
    STDMETHOD(GetSzID) (THIS_ LPSTR sz, LONG *pcb) PURE;

    // @meth <c HRESULT> | GetSzName |
	// Get a string that can be displayed to the user to describe this device.
    STDMETHOD(GetSzName) (THIS_ LPSTR sz, LONG *pcb) PURE;


	//------------------------------------------------------------------
	// @group IVisImSrcDevice2 methods

    // @meth <c HRESULT> | HasPropertyPage |
	// Does this device have the specified Property page?
    STDMETHOD(HasPropertyPage) (THIS_ EVisDSPropPage evisdsproppage) PURE;

    // @meth <c LONG> | DoPropertyPage |
	// Display the specified property page, if this device has a property
	// page of the specified type.
    STDMETHOD_(LONG, DoPropertyPage) (THIS_ EVisDSPropPage evisdsproppage,
			HWND hwndParent VISDEFAULT(0)) PURE;
};



#undef  INTERFACE
#define INTERFACE   IVisImSrcProvider

// @interface IVisImSrcProvider |
// This interface is used to get a list of devices that are available
// from a device-specific DLL.  It is returned from the
// <f VisGetImSrcProvider> function that is exported from a
// device-specific DLL.
DECLARE_INTERFACE_(IVisImSrcProvider, IUnknown)
{
	//------------------------------------------------------------------
	// @group IUnknown methods

    // @meth <c HRESULT> | QueryInterface |
	// This method is used to find other interfaces supported by this
	// OLE object.
	STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObject) PURE;

    // @meth <c ULONG> | AddRef |
	// This method is called to add a reference to this OLE object.
    STDMETHOD_(ULONG, AddRef) (THIS) PURE;

    // @meth <c ULONG> | Release |
	// This method is called to remove a reference to this OLE object.
    STDMETHOD_(ULONG, Release) (THIS) PURE;


	//------------------------------------------------------------------
	// @group IVisImSrcProvider methods


    // @meth <c HRESULT> | HasConnectedDevices |
	// Are there devices that are currently available?
    STDMETHOD(HasConnectedDevices) (THIS) PURE;

    // @meth <c HRESULT> | HasSelectionDialog |
	// Does this device have its own dialog that can be used to
	// select a device?
    STDMETHOD(HasSelectionDialog) (THIS) PURE;

    // @meth <c LONG> | DoSelectionDialog |
	// Do the device selection dialog if such a dialog is supported.
	// Return the index of the device selected or a negative number
	// if no device was selected (or if not device selection dialog
	// is supported).
    STDMETHOD_(LONG, DoSelectionDialog) (THIS_
			HWND hwndParent VISDEFAULT(0)) PURE;

    // @meth <c HRESULT> | GetConnectedDeviceList |
	// Get a list of the names of available devices.
    STDMETHOD(GetConnectedDeviceList) (THIS_ IVisListSz **ppIVisListSz)
			PURE;

    // @meth <c HRESULT> | GetDeviceSz |
	// Given a string, return an interface to the available device
	// that it describes.  This may return a NULL interfaces pointer
	// if the string does not describe an available device or if
	// there was an error creating the interface for the device.
    STDMETHOD(GetDeviceSz) (THIS_ IVisImSrcDevice **ppIVisImSrcDevice,
			LPCSTR sz) PURE;

    // @meth <c HRESULT> | GetDeviceI |
	// Given an index in the list of available devices, return an
	// interface to the specified available device.  This may return
	// a NULL interfaces pointer if the index is invalid or if
	// there was an error creating the interface for the device.
    STDMETHOD(GetDeviceI) (THIS_ IVisImSrcDevice **ppIVisImSrcDevice,
			LONG i) PURE;

    // @meth <c HRESULT> | GetSzID |
	// Get a string that can be used to identify this provider.  The
	// string should be the name of the rgistry subkey of the key
	// "HKEY_LOCAL_MACHINE\\Software\\Microsoft\\VisSDK\\97Beta1\\ImSrc\\Devs" (or
	// "HKEY_LOCAL_MACHINE\\Software\\Microsoft\\VisSDK\\Debug\\97Beta1\\ImSrc\\Devs"
	// in _DEBUG builds).
    STDMETHOD(GetSzID) (THIS_ LPSTR sz, LONG *pcb) PURE;

    // @meth <c HRESULT> | GetSzName |
	// Get a string that can be displayed to the user to describe this
	// provider.
    STDMETHOD(GetSzName) (THIS_ LPSTR sz, LONG *pcb) PURE;
};



#undef  INTERFACE
#define INTERFACE   IVisImSrcSettings

// @interface IVisImSrcSettings |
// This interface is used to query and change the settings used with a
// device that supports the <i IVisImSrcDevice> interface.
DECLARE_INTERFACE_(IVisImSrcSettings, IUnknown)
{
	//------------------------------------------------------------------
	// @group IUnknown methods

    // @meth <c HRESULT> | QueryInterface |
	// This method is used to find other interfaces supported by this
	// OLE object.
	STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObject) PURE;

    // @meth <c ULONG> | AddRef |
	// This method is called to add a reference to this OLE object.
    STDMETHOD_(ULONG, AddRef) (THIS) PURE;

    // @meth <c ULONG> | Release |
	// This method is called to remove a reference to this OLE object.
    STDMETHOD_(ULONG, Release) (THIS) PURE;


	//------------------------------------------------------------------
	// @group IVisImageSourceSettings methods


    // @meth <c HRESULT> | GetImSrcRange |
	// Get information about the valid values for the specified
	// setting.  The range will be empty for settings that are
	// not supported.
	STDMETHOD(GetImSrcRange) (THIS_ EVisImSrc evisimsrc, LONG *plMin,
			LONG *plMax, LONG *pdlStepping VISDEFAULT(0),
			LONG *plDefault VISDEFAULT(0), LONG *plFlags VISDEFAULT(0))
			CONST PURE;

    // @meth <c HRESULT> | GetImSrcValue |
	// Get the current value of the specified setting.  This may fail
	// if the setting is not supported.
	STDMETHOD(GetImSrcValue) (THIS_ EVisImSrc evisimsrc, LONG *plValue)
			CONST PURE;

    // @meth <c HRESULT> | SetImSrcValue |
	// Set the value of the specified setting.  This may fail.
	STDMETHOD(SetImSrcValue) (THIS_ EVisImSrc evisimsrc, LONG lValue,
			LONG lFlags VISDEFAULT(evisimsrcflagAuto)) PURE;

    // @meth <c HRESULT> | SaveSettings |
	// Save the current settings in the registry.
	STDMETHOD(SaveSettings) (THIS_ bool fForAllApps VISDEFAULT(false)) PURE;
};



#endif // __VIS_IMSRC_VISIMSRCIFACE_H__

// VisImageInfo.h
//
// Copyright © 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////

#pragma once

// {2ACD7927-B7E7-41e4-9633-13BED3FE5382}	IID_IVisMemBlock
DEFINE_GUID(IID_IVisMemBlock, 
0x2acd7927, 0xb7e7, 0x41e4, 0x96, 0x33, 0x13, 0xbe, 0xd3, 0xfe, 0x53, 0x82);

// {6337E4B4-3BA4-45c5-B4A6-E0A596A1F320}	IID_IVisImage
DEFINE_GUID(IID_IVisImage, 
0x6337e4b4, 0x3ba4, 0x45c5, 0xb4, 0xa6, 0xe0, 0xa5, 0x96, 0xa1, 0xf3, 0x20);


#undef  INTERFACE
#define INTERFACE   IVisMemBlock

// @interface IVisMemBlock |
// This interface provides information about a MemBlock.  It is
// implemented by the <c CVisImageInfo> class.
DECLARE_INTERFACE_(IVisMemBlock, IUnknown)
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
	// @group IVisMemBlock methods

	STDMETHOD_(EVisMemBlock, MemBlockOptions) (THIS) CONST PURE;

	STDMETHOD_(HANDLE, HFileMapping) (THIS) CONST PURE;

	STDMETHOD_(LONG, ProcessID) (THIS) CONST PURE;

	STDMETHOD_(LONG, FileMappingID) (THIS) CONST PURE;

	STDMETHOD_(int, CbData) (THIS) CONST PURE;

	STDMETHOD_(int, IbOffset) (THIS) CONST PURE;

	STDMETHOD_(const CVisMemBlock*, MemBlock) (THIS) CONST PURE;

};

#undef  INTERFACE
#define INTERFACE   IVisImage

// @interface IVisImage |
// This interface provides information about a MemBlock.  It is
// implemented by the <c CVisImageInfo> class.
DECLARE_INTERFACE_(IVisImage, IUnknown)
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
	// @group IVisImage methods

	STDMETHOD_(CVisShape, Rect) (THIS) CONST PURE;

	STDMETHOD_(CVisShape, MemoryRect) (THIS) CONST PURE;

	STDMETHOD_(int, NBands) (THIS) CONST PURE;

	STDMETHOD_(EVisPixFmt, PixFmt) (THIS) CONST PURE;

	STDMETHOD_(int, CbPoint) (THIS) CONST PURE;

	STDMETHOD_(CVisImageBase* , PImageBase) (THIS) CONST PURE ;

};

class CVisImageInfo : public IVisMemBlock, public IVisImage
{
public:
	//------------------------------------------------------------------
	// @group Constructor and destructor

	// @cmember
	// Constructor
	VisCoreExport CVisImageInfo(void);

	// @cmember
	// Destructor
	virtual ~CVisImageInfo(void);


	//------------------------------------------------------------------
	// @group IUnknown methods

    // @cmember <c HRESULT> | QueryInterface |
	//	(<c REFIID> riid, <c void> * * ppvObject) |
	// This method is used to find other interfaces supported by this
	// OLE object.
	STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObject);

    // @cmember <c ULONG> | AddRef |
	//	(void) |
	// This method is called to add a reference to this OLE object.
    STDMETHOD_(ULONG, AddRef) (THIS);

    // @cmember <c ULONG> | Release |
	//	(void) |
	// This method is called to remove a reference to this OLE object.
    STDMETHOD_(ULONG, Release) (THIS);


	//------------------------------------------------------------------
	// @group IVisMemBlock methods

	STDMETHOD_(EVisMemBlock, MemBlockOptions) (THIS) CONST ;

	STDMETHOD_(HANDLE, HFileMapping) (THIS) CONST ;

	STDMETHOD_(LONG, ProcessID) (THIS) CONST ;

	STDMETHOD_(LONG, FileMappingID) (THIS) CONST ;

	STDMETHOD_(int, CbData) (THIS) CONST ;

	STDMETHOD_(int, IbOffset) (THIS) CONST ;

	STDMETHOD_(const CVisMemBlock*, MemBlock) (THIS) CONST ;

	//------------------------------------------------------------------
	// @group IVisImage methods

	STDMETHOD_(CVisShape, Rect) (THIS) CONST ;

	STDMETHOD_(CVisShape, MemoryRect) (THIS) CONST ;

	STDMETHOD_(int, NBands) (THIS) CONST ;

	STDMETHOD_(EVisPixFmt, PixFmt) (THIS) CONST ;

	STDMETHOD_(int, CbPoint) (THIS) CONST ;

	STDMETHOD_(CVisImageBase* , PImageBase) (THIS) CONST;

	//------------------------------------------------------------------
	// @group CVisImageInfo methods
 
    STDMETHOD(SetImage) (THIS_ CVisImageBase* pImage);

private:

	long m_cRef;
	CVisImageBase*	 m_pImage;
};




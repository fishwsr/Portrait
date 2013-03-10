// VisImageInfo.inl
//
// Copyright © 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////

#include "VisCorePch.h"
#include "VisCoreProj.h"

#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG

const IID IID_IVisMemBlock
	= {0x2acd7927, 0xb7e7, 0x41e4, 0x96, 0x33, 0x13, 0xbe, 0xd3, 0xfe, 0x53, 0x82};

// {6337E4B4-3BA4-45c5-B4A6-E0A596A1F320}	IID_IVisImage
const IID IID_IVisImage
	= {0x6337e4b4, 0x3ba4, 0x45c5, 0xb4, 0xa6, 0xe0, 0xa5, 0x96, 0xa1, 0xf3, 0x20};


VisCoreExport CVisImageInfo::CVisImageInfo(void)
	: m_pImage(0),
	  m_cRef(0)
{
}

// @cmember
// Destructor
CVisImageInfo::~CVisImageInfo(void)
{
	if(m_pImage != 0)
	{
		delete m_pImage;
		m_pImage = NULL;
	}
}

STDMETHODIMP CVisImageInfo::QueryInterface(THIS_ REFIID riid, void **ppvObject)
{
	HRESULT hrRet;

	if (ppvObject == 0)
	{
		hrRet = ResultFromScode(E_POINTER);
	}
	else if ((riid == IID_IUnknown)
				|| (riid == IID_IVisMemBlock))
	{
		hrRet = ResultFromScode(S_OK);
		*ppvObject = (IVisMemBlock *)this;
		AddRef();
	}
	else if (riid == IID_IVisImage)
	{
		hrRet = ResultFromScode(S_OK);
		*ppvObject = (IVisImage *)this;
		AddRef();
	}
	else
	{
		*ppvObject = 0;
		hrRet = ResultFromScode(E_NOINTERFACE);
	}

	return hrRet;
}

STDMETHODIMP_(ULONG) CVisImageInfo::AddRef(THIS)
{
	return InterlockedIncrement( &m_cRef );
}

STDMETHODIMP_(ULONG) CVisImageInfo::Release(THIS)
{
	ULONG cRef = InterlockedDecrement(&m_cRef);

	if (cRef == 0)
		delete this;

	return cRef;
}

STDMETHODIMP_(EVisMemBlock) CVisImageInfo::MemBlockOptions(THIS) CONST 
{
	return (m_pImage != 0) ? MemBlock()->MemBlockOptions() : (EVisMemBlock) 0;
}

STDMETHODIMP_(HANDLE) CVisImageInfo::HFileMapping (THIS) CONST
{
	return (m_pImage != 0) ? MemBlock()->HFileMapping() : (HANDLE) 0;
}

// Is this needed
STDMETHODIMP_(LONG) CVisImageInfo::ProcessID (THIS) CONST
{
	return GetCurrentProcessId();
}

STDMETHODIMP_(LONG) CVisImageInfo::FileMappingID (THIS) CONST
{
	return (m_pImage != 0) ? MemBlock()->FileMappingID() : 0;
}

STDMETHODIMP_(int) CVisImageInfo::CbData (THIS) CONST
{
	return (m_pImage != 0) ? MemBlock()->CbData() : 0;
}

STDMETHODIMP_(int) CVisImageInfo::IbOffset (THIS) CONST
{
	return (m_pImage != 0) ? MemBlock()->IbOffset() : 0;
}

STDMETHODIMP_(const CVisMemBlock*) CVisImageInfo::MemBlock (THIS) CONST
{
	return (m_pImage != 0) ? &(m_pImage->MemBlock()) : 0;
}

STDMETHODIMP_(CVisShape) CVisImageInfo::Rect (THIS) CONST 
{
	return (m_pImage != 0) ? m_pImage->Rect() : CVisShape(0, 0, 0, 0);
}

STDMETHODIMP_(CVisShape) CVisImageInfo::MemoryRect (THIS) CONST 
{
	return (m_pImage != 0) ? m_pImage->MemoryRect() : CVisShape(0, 0, 0, 0);
}

STDMETHODIMP_(int) CVisImageInfo::NBands (THIS) CONST 
{
	return (m_pImage != 0) ? m_pImage->NBands() : 0;
}

STDMETHODIMP_(EVisPixFmt) CVisImageInfo::PixFmt (THIS) CONST 
{
	return (m_pImage != 0) ? m_pImage->PixFmt() : evispixfmtNil;
}

STDMETHODIMP_(int) CVisImageInfo::CbPoint (THIS) CONST
{
	return (m_pImage != 0) ? m_pImage->CbPoint() : 0;
}

STDMETHODIMP_(CVisImageBase* ) CVisImageInfo::PImageBase (THIS) CONST
{
	return m_pImage;
}

STDMETHODIMP CVisImageInfo::SetImage (THIS_ CVisImageBase* pImage)
{
	m_pImage = pImage;
	return S_OK;
}

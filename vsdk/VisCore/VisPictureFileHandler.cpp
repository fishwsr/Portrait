// VisPictureFileHandler.cpp
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved


#include "VisCorePch.h"
#include "VisCoreProj.h"


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CVisPictureFileHandler::CVisPictureFileHandler(void)
  : m_pIPicture(0)
{
}

CVisPictureFileHandler::~CVisPictureFileHandler(void)
{
	if (m_pIPicture != 0)
	{
		m_pIPicture->Release();
	}
}

BOOL CVisPictureFileHandler::MatchExtension(const char *szFiletype)
{
	assert(0);
	return false;
}

BOOL CVisPictureFileHandler::ExternallyOpenStream(void)
{
    return FALSE;
}

BOOL CVisPictureFileHandler::SupportsPixelType(int evispixfmt, int nbands)
{
    assert(0);
	return false;
}

BOOL CVisPictureFileHandler::ReadHeader(SVisFileDescriptor &fd, CVisImageBase &img) 
{
	bool fReadHeader = true;

	HANDLE hFile = CreateFile(fd.filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	assert(hFile != 0);

	DWORD dwFileSize = GetFileSize(hFile, NULL);
	assert(dwFileSize != -1);

	void *pvData = NULL;
	// alloc memory based on file size
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
	assert(hGlobal != NULL);

	pvData = GlobalLock(hGlobal);
	assert(pvData != NULL);

	DWORD dwBytesRead = 0;
	bool fReadFile
			= (::ReadFile(hFile, pvData, dwFileSize, &dwBytesRead, NULL) != 0);
	assert(fReadFile != FALSE);

	GlobalUnlock(hGlobal);
	CloseHandle(hFile);

	IStream *pstm = NULL;

	HRESULT hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pstm);
	assert(SUCCEEDED(hr) && pstm);

	// Create IPicture from image file
	hr = ::OleLoadPicture(pstm, dwFileSize, FALSE, IID_IPicture, (void** )&m_pIPicture);
	if(SUCCEEDED(hr) && m_pIPicture)
	{
		pstm->Release();
		long hmWidth = 0;
		long hmHeight = 0;
		m_pIPicture->get_Width(&hmWidth);
		m_pIPicture->get_Height(&hmHeight);

		// convert himetric to pixels
		HDC hdc = GetDC(NULL);
		int nWidth	= MulDiv(hmWidth, GetDeviceCaps(hdc, LOGPIXELSX), 2540);
		int nHeight	= MulDiv(hmHeight, GetDeviceCaps(hdc, LOGPIXELSY), 2540);

		ReleaseDC(NULL, hdc);
		// Fill in the fd structure
		fd.rows = nHeight;
		fd.cols = nWidth;
		fd.bands = 1; 
		fd.evispixfmt = img.PixFmt();
		fReadHeader = false;
	}	
	
    return fReadHeader;
}

BOOL CVisPictureFileHandler::ReadBody(SVisFileDescriptor &fd, CVisImageBase &img)
{
	assert(m_pIPicture != 0);
	bool fReadBody = true;
	HDC hdc = img.Hdc();
	if(hdc != 0)
	{
		long hmWidth = 0;
		long hmHeight = 0;
		m_pIPicture->get_Width(&hmWidth);
		m_pIPicture->get_Height(&hmHeight);

		HRESULT hr = m_pIPicture->Render(hdc, 0, 0, img.Width(), img.Height(), 
											  0, hmHeight, hmWidth, -hmHeight, NULL);

		img.DestroyHdc();

		fReadBody = (hr == S_OK) ? false : true;
	}
	m_pIPicture->Release();
	m_pIPicture = NULL;
	return fReadBody;
}

BOOL CVisPictureFileHandler::WriteHeader(SVisFileDescriptor &fd, CVisImageBase &img)
{
    assert(0);
	return false;
}

BOOL CVisPictureFileHandler::WriteBody(SVisFileDescriptor &fd, CVisImageBase &img)
{
    assert(0);
	return false;
}

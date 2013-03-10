// VisDDrawConversion.inl
//
// Copyright © 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////

#pragma once

const IID DDPF_ARGB32
	= {0x773c9ac0, 0x3274, 0x11d0, 0xb7, 0x24, 0x0, 0xaa, 0x0, 0x6c, 0x1a, 0x1 };

inline bool VisInlineAliasDDSurfaceInImage(IDirectDrawSurface7* pIDDS, 
											CVisImageBase& refimagebase);

inline bool VisInlineCopyDDSurfacePixelsToImage(IDirectDrawSurface7* pIDDS, 
												CVisImageBase& refimagebase);

inline IDirectDrawSurface7* VisInlinePIDDSAliasFromImage(const CVisImageBase& refimagebase);

inline bool VisInlineCopyImagePixelsToDDSurface(const CVisImageBase& refimagebase, 
												IDirectDrawSurface7** ppIDDS);

#ifdef VIS_USE_DXMEDIA

inline bool VisInlineCopyImagePixelsToDXSurface(const CVisImageBase& refimagebase, 
												IDXSurface** ppIDXS);

inline bool VisInlineCopyDXSurfacePixelsToImage(IDXSurface* pIDXS, 
												CVisImageBase& refimagebase);

#endif // VIS_USE_DXMEDIA

inline bool VisInlineAliasDDSurfaceInImage(IDirectDrawSurface7* pIDDS, 
											CVisImageBase& refimagebase)
{
	assert(pIDDS != 0);
	
	IDirectDraw7* pIDD = PGlobals()->IDDraw();
	
	IDirectDrawSurface7* pIDDSDuplicate = NULL;
	HRESULT hr = pIDD->DuplicateSurface(pIDDS, &pIDDSDuplicate);
	
	if(hr != DD_OK)
		return false;

	IVisMemBlock* pVisMemblock = 0;
	IUnknown* pVisUnknown = 0;
	DWORD dwBufferSize = sizeof(IVisMemBlock*);
	hr = pIDDS->GetPrivateData(IID_IVisMemBlock, (void *)&pVisMemblock, &dwBufferSize);
	
	HANDLE hFileMapping = 0;
	if(hr == DD_OK)
	{
		hr = pIDDSDuplicate->SetPrivateData(IID_IVisMemBlock, pVisMemblock, sizeof(IVisMemBlock*), DDSPD_IUNKNOWNPOINTER);
		pVisMemblock->AddRef();
		IVisImage* pVisImage = 0;
		hr = pVisMemblock->QueryInterface(IID_IVisImage, (void**)&pVisImage);
		assert(hr == S_OK);
		if(pVisMemblock->ProcessID() == GetCurrentProcessId())
		{
			if(refimagebase.PixFmt() == pVisImage->PixFmt())
			{
				refimagebase = *(pVisImage->PImageBase());
			}
			else
			{
				refimagebase.Alias(*(pVisImage->PImageBase()));
			}
		}
		else if(hFileMapping = pVisMemblock->HFileMapping())
		{
			CVisMemBlock memblock = VisCreateMemBlock(pVisMemblock->CbData(), 
													pVisMemblock->IbOffset(),
													pVisMemblock->MemBlock()->FileMappingID(),
													pVisMemblock->MemBlock()->FileMappingAppID());
			
			if (refimagebase.PixFmt() == pVisImage->PixFmt())
			{
				refimagebase.SetMemBlock(memblock, 
								CVisShape(pVisImage->MemoryRect(), pVisImage->NBands()),
								CVisShape(pVisImage->Rect(), pVisImage->NBands()));
			}
			else
			{
				refimagebase.Alias(pVisImage, memblock);
			}
		}
		pVisImage->Release();
		pVisMemblock->Release();
	}
	else
	{
		refimagebase.Alias(pIDDSDuplicate);
	}
	return true;
}

inline bool  VisInlineCopyDDSurfacePixelsToImage(IDirectDrawSurface7* pIDDS, 
												CVisImageBase& refimagebase)
{
	IVisMemBlock* pVisMemblock = 0;
	DWORD dwBufferSize = sizeof(IVisMemBlock*);
	HRESULT hr = pIDDS->GetPrivateData(IID_IVisMemBlock, (void *)&pVisMemblock, &dwBufferSize);
	bool fResult = false;
	if((hr == DD_OK) && (pVisMemblock->ProcessID() == GetCurrentProcessId()))
	{
		pVisMemblock->AddRef();
		IVisImage* pVisImage = 0;
		hr = pVisMemblock->QueryInterface(IID_IVisImage, (void**)&pVisImage);
		assert(hr == S_OK);
		refimagebase.Allocate(pVisImage->PImageBase()->Shape());
		fResult = pVisImage->PImageBase()->CopyPixelsTo(refimagebase);
		pVisImage->Release();
		pVisMemblock->Release();
	}
	else
	{
		assert((refimagebase.PixFmt() == evispixfmtRGBAByte) || 
				(refimagebase.PixFmt() == evispixfmtGrayByte));

		HDC hdcSrc;
		hr = pIDDS->GetDC(&hdcSrc);
		if(hr == DD_OK)
		{
			HDC hdcDest;
			DDSURFACEDESC2 ddsd2;
			ddsd2.dwSize = sizeof(DDSURFACEDESC2);
			ddsd2.dwFlags = DDSD_PITCH | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;

			HRESULT hr = pIDDS->GetSurfaceDesc(&ddsd2);

			// Check this out, Width & Height to be compared (Look at CopyPixelsTo method)
			if(refimagebase.Shape() != CVisShape(0, 0, ddsd2.dwWidth, ddsd2.dwHeight))
				refimagebase.Allocate(ddsd2.dwWidth, ddsd2.dwHeight);
			hdcDest = refimagebase.Hdc();
			if(hdcDest != 0)
			{
				fResult = (BitBlt(hdcDest, 0, 0, refimagebase.Width(), refimagebase.Height(),
									hdcSrc,  0, 0, SRCCOPY) != 0);

				refimagebase.DestroyHdc();
			}
		}
	}
	return fResult;
}

inline IDirectDrawSurface7* VisInlinePIDDSAliasFromImage(const CVisImageBase& refimagebase)
{
	IDirectDrawSurface7* pIDDImage = NULL;
	IVisMemBlock* pIVisMemBlock = NULL;
	HRESULT hr;

	if(pIDDImage = refimagebase.MemBlock().PIDDS())
	{
		pIDDImage->AddRef();
		DWORD dwBufferSize = sizeof(IVisMemBlock*);
		hr = pIDDImage->GetPrivateData(IID_IVisMemBlock, (void *)&pIVisMemBlock, &dwBufferSize);
		if(hr != DD_OK)
			pIVisMemBlock = NULL;
	}
	else
	{
		IDirectDraw7* pIDD = PGlobals()->IDDraw();
		DDSURFACEDESC2 ddsd;
		ZeroMemory(&ddsd, sizeof(DDSURFACEDESC2));
		ddsd.dwSize = sizeof(DDSURFACEDESC2);

		ddsd.dwFlags         = DDSD_LPSURFACE | DDSD_CAPS | DDSD_PITCH | 
								DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
 		ddsd.ddsCaps.dwCaps  = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;

		ddsd.dwWidth = refimagebase.Width();
		ddsd.dwHeight = refimagebase.Height();

		ddsd.lPitch = refimagebase.CbRow();

		assert(refimagebase.PixFmt() == evispixfmtRGBAByte);

		ddsd.ddpfPixelFormat.dwSize            = sizeof(DDPIXELFORMAT);
		ddsd.ddpfPixelFormat.dwFlags           = DDPF_RGB;
		ddsd.ddpfPixelFormat.dwRGBBitCount     = refimagebase.CbPixel() * 8;
		ddsd.ddpfPixelFormat.dwRBitMask        = 0xFF0000;
		ddsd.ddpfPixelFormat.dwGBitMask        = 0x00FF00;
		ddsd.ddpfPixelFormat.dwBBitMask        = 0x0000FF;
		
		ddsd.lpSurface = refimagebase.MemBlock().PbData();

		IDirectDrawSurface7* pIDDS = NULL;
		hr = pIDD->CreateSurface(&ddsd, &pIDDS, NULL);
		if(hr == DD_OK)
		{
			pIDDImage = pIDDS;
		}
	}
	
	if(pIDDImage && (!pIVisMemBlock))
	{
		CVisImageInfo* pImageInfo = new CVisImageInfo();

		pImageInfo->SetImage(refimagebase.Clone());


		hr = pImageInfo->QueryInterface(IID_IVisMemBlock, (void**)&pIVisMemBlock);
		assert(hr == S_OK);

		hr = pIDDImage->SetPrivateData(IID_IVisMemBlock, pIVisMemBlock, sizeof(IVisMemBlock*), DDSPD_IUNKNOWNPOINTER);
		pImageInfo->Release();
	}

	return pIDDImage;
}

inline bool VisInlineCopyImagePixelsToDDSurface(const CVisImageBase& refimagebase, 
												IDirectDrawSurface7** ppIDDS)
{
	IDirectDrawSurface7* pIDDCacheImage = NULL;

	IVisMemBlock* pIVisMemBlock = NULL;
	HRESULT hr;

	bool fResult = false;
	
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(DDSURFACEDESC2));
	
	ddsd.dwSize = sizeof(DDSURFACEDESC2);

	// Compressors, Rendering(VisXRenderFill) or Blt
	if(pIDDCacheImage = refimagebase.MemBlock().PIDDS())
	{
		if(*ppIDDS == 0)
		{
			ddsd.dwFlags         = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
 			ddsd.ddsCaps.dwCaps  = DDSCAPS_OFFSCREENPLAIN;

			pIDDCacheImage->GetSurfaceDesc(&ddsd);
 			ddsd.ddsCaps.dwCaps  = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;

			hr = PGlobals()->IDDraw()->CreateSurface(&ddsd, ppIDDS, NULL);
		}

		// DirectDraw blit methods, IDirectDrawSurface7::Blt and IDirectDrawSurface7::BltFast, 
		// automatically perform conversion of compressed formats to standard RGBA formats
		hr = (*ppIDDS)->Blt(NULL, pIDDCacheImage, NULL, DDBLT_WAIT, NULL);		
		fResult = ((hr == DD_OK) ? true : false);
	}
	else
	{
		ddsd.dwFlags         = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
 		ddsd.ddsCaps.dwCaps  = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;

		if(*ppIDDS == 0)
		{
			ddsd.dwWidth = refimagebase.Width();
			ddsd.dwHeight = refimagebase.Height();

			ddsd.ddpfPixelFormat.dwSize            = sizeof(DDPIXELFORMAT);
			ddsd.ddpfPixelFormat.dwFlags           = DDPF_RGB;
			ddsd.ddpfPixelFormat.dwRGBBitCount     = 32;
			ddsd.ddpfPixelFormat.dwRBitMask        = 0xFF0000;
			ddsd.ddpfPixelFormat.dwGBitMask        = 0x00FF00;
			ddsd.ddpfPixelFormat.dwBBitMask        = 0x0000FF;

			hr = PGlobals()->IDDraw()->CreateSurface(&ddsd, ppIDDS, NULL);
		}
		else
		{
			(*ppIDDS)->GetSurfaceDesc(&ddsd);
		}

		if((refimagebase.PixFmt() == evispixfmtRGBAByte) && 
		   (refimagebase.NBands() == 1) && 
			(ddsd.ddpfPixelFormat.dwFlags == DDPF_RGB) &&
			(ddsd.ddpfPixelFormat.dwRGBBitCount == 32))
		{
			ddsd.dwFlags |= DDSD_LPSURFACE;
			hr = (*ppIDDS)->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR, NULL);
			BYTE* pbDest = (BYTE* )ddsd.lpSurface;
			int wWidth = min(((int) ddsd.dwWidth), refimagebase.Width());
			int wHeight = min(((int) ddsd.dwHeight), refimagebase.Height());
			for(int iRow = 0; iRow < wHeight; iRow++)
			{
				const BYTE* pbSrc = refimagebase.PbFirstPixelInRow(
						refimagebase.Top() + iRow);
				CopyMemory(pbDest, pbSrc, wWidth * 4);
				pbDest += ddsd.lPitch;
			}
			fResult = true;
			(*ppIDDS)->Unlock(NULL);
		}
		else
		{
			IDirectDrawSurface7* pIDDDuplicate = VisPIDDSAliasFromImage(refimagebase);
			if(pIDDDuplicate)
			{
				hr = (*ppIDDS)->Blt(NULL, pIDDDuplicate, NULL, 0, NULL);		
				fResult = ((hr == DD_OK) ? true : false);
			}
		}
	}
	
	return fResult;
}

#ifdef VIS_USE_DXMEDIA

inline bool VisInlineCopyImagePixelsToDXSurface(const CVisImageBase& refimagebase, 
												IDXSurface** ppIDXS)
{
	HRESULT hr;
	bool fResult = false;
	if(*ppIDXS == 0)
	{
		IDXSurfaceFactory* pIDXSFactory = PGlobals()->IDXSFactory();
		CDXDBnds bounds;
		bounds.SetXYSize(refimagebase.Width(), refimagebase.Height());
		hr = pIDXSFactory->CreateSurface(NULL, NULL, 
										 &DDPF_ARGB32, 
										 &bounds, 0, NULL,
										 IID_IDXSurface, 
										 (void **)ppIDXS);
	}
	
    if((*ppIDXS != 0) && (refimagebase.PixFmt() == evispixfmtRGBAByte))
	{
		CDXDBnds bnds(*ppIDXS, hr);
		if (SUCCEEDED(hr))
		{
			IDXARGBReadWritePtr* pIReadWriteBuf = NULL;
			hr = (*ppIDXS)->LockSurface(NULL, INFINITE, DXLOCKF_READWRITE, 
									   IID_IDXARGBReadWritePtr, 
									   (void **)&pIReadWriteBuf, NULL);
			if (SUCCEEDED(hr))
			{
				ULONG uHeight = bnds.Height();
				ULONG uWidth = bnds.Width();

				DXPACKEDRECTDESC rectDesc;
				rectDesc.pSamples = (DXBASESAMPLE *)refimagebase.PbFirstPixelInRow(0);
				rectDesc.bPremult = false;
				rectDesc.rect.left = 0;
				rectDesc.rect.top = 0;
				rectDesc.rect.right = uWidth;
				rectDesc.rect.bottom = uHeight;

				rectDesc.lRowPadding = (refimagebase.CbRow())/4 - uWidth;

				pIReadWriteBuf->PackRect(&rectDesc);

				pIReadWriteBuf->Release();
				fResult = true;
			}
		}
	}
	else
	{
		IDirectDrawSurface7* pIDDS = NULL;
		hr = (*ppIDXS)->GetDirectDrawSurface(IID_IDirectDrawSurface7, (void **)&pIDDS);
		if(hr == S_OK)
		{
			fResult = VisInlineCopyImagePixelsToDDSurface(refimagebase, &pIDDS);
		}
	}
	return fResult;
}

inline bool VisInlineCopyDXSurfacePixelsToImage(IDXSurface* pIDXS, 
												CVisImageBase& refimagebase)
{
    assert(pIDXS != 0);
	
	HRESULT hr;
	CDXDBnds bnds(pIDXS, hr);
	bool fResult = false;
	ULONG uHeight = bnds.Height();
	ULONG uWidth = bnds.Width();
    if(refimagebase.PixFmt() == evispixfmtRGBAByte)
	{
		if (SUCCEEDED(hr))
		{
			IDXARGBReadPtr* pIReadBuf = NULL;
			hr = pIDXS->LockSurface(NULL, INFINITE, DXLOCKF_READ, 
									   IID_IDXARGBReadPtr, 
									   (void **)&pIReadBuf, NULL);
			if (SUCCEEDED(hr))
			{
				if(refimagebase.Shape() != CVisShape(0, 0, uWidth, uHeight))
					refimagebase.Allocate(uWidth, uHeight);
				DXPACKEDRECTDESC rectDesc;
				rectDesc.pSamples = (DXBASESAMPLE *)refimagebase.PbFirstPixelInRow(0);
				rectDesc.bPremult = false;
				rectDesc.rect.left = 0;
				rectDesc.rect.top = 0;
				rectDesc.rect.right = uWidth;
				rectDesc.rect.bottom = uHeight;

				rectDesc.lRowPadding = (refimagebase.CbRow())/4 - uWidth;
				pIReadBuf->UnpackRect(&rectDesc);
			
				pIReadBuf->Release();
				fResult = true;
			}
		}
	}
	else
	{
		if(refimagebase.Shape() != CVisShape(0, 0, uWidth, uHeight))
			refimagebase.Allocate(uWidth, uHeight);
		HDC hdc = refimagebase.Hdc();
		if(hdc != 0)
		{
			hr = DXSrcCopy(hdc, 0, 0, refimagebase.Width(), refimagebase.Height(), pIDXS, 0, 0);
			fResult = (hr == S_OK) ? true : false;
		}
	}
	return fResult;
}

#endif // VIS_USE_DXMEDIA
/***************************************************************/
/*文件名：     image.cpp               				           */
/*文件描述：                                                   */
/*相关文件：   global.h, global.cpp, image.h	               */
/*主要函数：                                                   */
/*提交日期：                        作  者：刘岩、山世光       */
/*编 辑 器： Visual C++                                        */
/*版权：哈尔滨工业大学智能接口实验室 中科院计算所数字媒体实验室*/
/*版权说明：任何使用必须保留版权信息和历史纪录信息			   */
/*修改纪录：												   */
/*日  期        版本        修改人         修改内容            */
/*03/22/1999    2.0         山世光         ---------           */
/*05/22/2000    3.0         山世光         ---------           */
/***************************************************************/

#include "stdafx.h"
#include "math.h"

#include "image.h"
#include "globalfunc.h"
#include <math.h>  // for sqrt()
#include <windowsx.h>  // for GlobalAllocPtr

#define		MAXIMAGESIZE    (3 * 65536) //
const int CImage::GREY=256;

IMPLEMENT_SERIAL(CImage, CObject, 0)

///////////////////////////////////////////////////////////////////
CImage::CImage(): m_bIsDIB(TRUE),m_dwLength(0L),
				  m_pDib (NULL),m_pData(NULL)
{
	m_pPal	=new CPalette;
}
    
///////////////////////////////////////////////////////////////////
CImage::CImage(const CImage& img)//copy constructor
{
	m_bIsDIB=img.m_bIsDIB;
	m_dwLength=img.m_dwLength;
	m_pDib=0;
	if(img.m_pDib){
		m_pDib=(BYTE*) GlobalAllocPtr(GHND, m_dwLength);
		if(!m_pDib)
		{
	        AfxMessageBox("Unable to allocate DIB memory");
			return;
		}
		memcpy(m_pDib,img.m_pDib,m_dwLength);
	    m_pData = (BYTE*) m_pDib + sizeof(BITMAPINFOHEADER) +
               sizeof(RGBQUAD) * img.NumColors();
	}
	m_pPal=new CPalette;
	CreateDIBPalette();
}
///////////////////////////////////////////////////////////////////
CImage::CImage(CImage& img,	CPoint start,	CSize size)//从原始图像中从某一点去大小为size的一块出来
{
	int		bpp;
	int		bytepp;
	long	hdlen;/*size of the DIB header info*/
	LPBITMAPINFOHEADER pBMIH;

	ASSERT(img.m_pDib);
	WORD dww=img.Width();
	WORD dwh=img.Height();
	if(( (size.cx+start.x)>dww )||( (size.cy+start.y)>dwh )){
		AfxMessageBox("Image size is too large!");
		return;
	}
	img.Data();
	
	m_bIsDIB = FALSE;
    hdlen = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * img.NumColors();

	bpp		= img.Bits();
	bytepp	= bpp / 8;

	ASSERT(bpp != 16 && bpp != 32);

    DWORD bypeinoneline = ByteNumForOneLine((WORD)size.cx, bpp);//Bytes per line
	
    m_dwLength = hdlen + bypeinoneline * size.cy;

    if (!AllocateMemory( )){
		return;
	}
	memcpy(m_pDib, img.m_pDib, hdlen);

    pBMIH = BMInfoHdPtr();
    pBMIH->biWidth = size.cx;
    pBMIH->biHeight = size.cy;
    pBMIH->biSizeImage = size.cx * size.cy * bytepp;

    m_pData =  (BYTE*)(m_pDib + hdlen);

	BYTE* p1, *p2;
	long  t1, t2;
	t1 = dww * bytepp;
	t2 = size.cx * bytepp;
	p1 = ( BYTE* ) ( img.m_pData + (start.y * dww + start.x) * bytepp);
	p2 = m_pData;
	for(int i=0 ; i<(int)size.cy; i++)
	{
		memcpy(p2 , p1 , t2);
		p1 += t1;
		p2 += t2;
	}

	Dib();	
	img.Dib();
	m_pPal=new CPalette;
	CreateDIBPalette();
}
///////////////////////////////////////////////
/*the constructor used to create a DIB of size 'size'
 with no initializing image data*/
CImage::CImage(CSize size,int NumColor,int Bits)//构造一个黑白图像大小为size??可以是彩色的八？只要是r,g,b不同。每一个像素都用八位来表示（调色版的序号），但是调色版中的rgbz值可以相同，也可以不相同
{
    LPBITMAPINFOHEADER pBMIH;
    m_dwLength = sizeof(BITMAPINFOHEADER) + 
					sizeof(RGBQUAD) * NumColor;
    if (!AllocateMemory()) return;
	DWORD dwBytes = Transform((WORD)size.cx);

    m_dwLength += dwBytes * size.cy * Bits / 8;
    if (!AllocateMemory(TRUE)){
		return;
	}
    pBMIH=BMInfoHdPtr();
	pBMIH->biSize = sizeof(BITMAPINFOHEADER);
    pBMIH->biWidth = size.cx;
    pBMIH->biHeight = size.cy;
    pBMIH->biSizeImage = size.cx*size.cy;
    pBMIH->biPlanes = 1;
    pBMIH->biBitCount = Bits; // 1, 4, 8, or 24
    pBMIH->biCompression = BI_RGB;
    pBMIH->biXPelsPerMeter = 0;
    pBMIH->biYPelsPerMeter = 0;
    pBMIH->biClrUsed = 0;
    pBMIH->biClrImportant = 0;
    m_pData =  (BYTE*)(m_pDib + sizeof(BITMAPINFOHEADER) +
              sizeof(RGBQUAD) * NumColor);
	SetDIB();
	m_pPal=new CPalette;
	CreateGreyPalette();
}
///////////////////////////////////////////////////////////////////
	//part copy constructor
CImage& CImage::operator=(const CImage& img)
{
	m_bIsDIB=img.m_bIsDIB;
	m_dwLength=img.m_dwLength;
	if(img.m_pDib){
		TRY	{
			if(m_pDib){
			m_pDib = (BYTE*) GlobalReAllocPtr(m_pDib,
                                            m_dwLength, GHND);
			}else
			{
				m_pDib=(BYTE*) GlobalAllocPtr(GHND, m_dwLength);
			}
		}
		CATCH (CMemoryException, e) {
	        AfxMessageBox("Unable to allocate DIB memory");
			return *this;
		}
		END_CATCH

		memcpy(m_pDib,img.m_pDib,m_dwLength);
	    m_pData = (BYTE*)((BYTE*) m_pDib + sizeof(BITMAPINFOHEADER) +
               sizeof(RGBQUAD) * img.NumColors());
	}else
	{
		GlobalFree(m_pDib);
		m_pDib=0;
	}
	if(m_pPal) delete m_pPal;
	m_pPal=new CPalette;
	CreateDIBPalette();
	return *this;
}
///////////////////////////////////////////////////////////////////
CImage::CImage(CDC* pDC, int nBt, BOOL bCompr):m_bIsDIB(TRUE)
/* pDC is memory DC ptr	and  Cbitmap object has been selected into it 
   nBt is color bits per pixel (default = 0)
   bCompr is compression (default = TRUE)
*/
{
   // constructs a DIB from the contents of a bitmap
    BITMAP bm;
    int    nPaletteSize,wBits;
	LPBITMAPINFOHEADER pBMIH;
	LPBITMAPINFO pBMI;

    CBitmap* pEmptyBitmap = new CBitmap;
    pEmptyBitmap->CreateCompatibleBitmap(pDC, 0, 0);
    CBitmap* pBitmap = (CBitmap*) (pDC->SelectObject(pEmptyBitmap));
    pBitmap->GetObject(sizeof(bm), &bm);
    if ((nBt == 1) || (nBt == 4) || (nBt == 8) || (nBt == 24)) {
        wBits = nBt;
    }
    else {   // nBt = 0
        wBits = bm.bmPlanes * bm.bmBitsPixel; // color bits per pixel
    }
    if (wBits == 1) {
        nPaletteSize = 2;
    }
    else {
        if (wBits == 4) {
            nPaletteSize = 16;
        }
        else {
            if (wBits == 8) {
                nPaletteSize = 256;
            }
            else {
                nPaletteSize = 0; // no palette for 24-bit display
            } 
        }
    }
    // fills out row to 4-byte boundary
    DWORD dwBytes = ((DWORD) bm.bmWidth * wBits) / 32;
    if (((DWORD) bm.bmWidth * wBits) % 32) {
        dwBytes ++;
    }
    dwBytes *= 4;//pixels per line

    m_dwLength = sizeof(BITMAPINFOHEADER) + 
					sizeof(RGBQUAD) * nPaletteSize;
    if (!AllocateMemory()) return;
    
    pBMIH=BMInfoHdPtr();
	pBMIH->biSize = sizeof(BITMAPINFOHEADER);
    pBMIH->biWidth = bm.bmWidth;
    pBMIH->biHeight = bm.bmHeight;
    pBMIH->biPlanes = 1;
    pBMIH->biBitCount = wBits; // 1, 4, 8, or 24
    if (bCompr && (wBits == 4)) {
        pBMIH->biCompression = BI_RLE4;
    }
    else {
        if (bCompr && (wBits == 8)) { 
            pBMIH->biCompression = BI_RLE8;
        }
        else {
            pBMIH->biCompression = BI_RGB;
        } 
    }
    pBMIH->biSizeImage = 0;
    pBMIH->biXPelsPerMeter = 0;
    pBMIH->biYPelsPerMeter = 0;
    pBMIH->biClrUsed = 0;
    pBMIH->biClrImportant = 0;

    // calls GetDIBits with null data pointer to get size of DIB
    pBMI=BMInfoPtr();
	::GetDIBits(pDC->GetSafeHdc(), (HBITMAP) pBitmap->GetSafeHandle(),
                0, (WORD) bm.bmHeight, NULL, pBMI, DIB_RGB_COLORS);

    if (pBMIH->biSizeImage == 0) {//pBMIH????orpBMI
        m_dwLength += dwBytes * bm.bmHeight;
        pBMIH->biCompression = BI_RGB;
        // escape route for device drivers that don't do compression
        TRACE("Can't do compression\n");
    }
    else {
        m_dwLength += pBMIH->biSizeImage;
    }
    if (!AllocateMemory(TRUE)) {
        return;
    }
    pBMIH=BMInfoHdPtr();
    pBMI=BMInfoPtr();
    m_pData = (BYTE*) pBMIH + sizeof(BITMAPINFOHEADER) +
               sizeof(RGBQUAD) * nPaletteSize;

    // second GetDIBits call to make DIB
    if (!::GetDIBits(pDC->GetSafeHdc(), (HBITMAP)
        pBitmap->GetSafeHandle(), 0, (WORD) bm.bmHeight, m_pData,
        pBMI, DIB_RGB_COLORS)) {
        m_dwLength = 0L;
    }
    delete pDC->SelectObject(pBitmap); // delete pEmptyBitmap
	Dib();
	m_pPal=new CPalette;
	CreateDIBPalette();
}
///////////////////////////////////////////////////////////////////
CImage::CImage(CImage& img, float scale)
{
	WORD scrw,scrh,destw,desth;
	DWORD dwDIBHeadLength;

	img.Data();
	m_bIsDIB = img.m_bIsDIB;

	scrw  = img.Width();
	scrh  = img.Height();
	destw = (WORD) (scrw * scale);
	desth = (WORD) (scrh * scale);

	dwDIBHeadLength = sizeof(BITMAPINFOHEADER) + 
					  sizeof(RGBQUAD) * img.NumColors();
	DWORD dwBytes = Transform(destw);
	m_dwLength = dwDIBHeadLength + (DWORD) dwBytes * desth;

	m_pDib = NULL;
	if(img.m_pDib)
	{
		m_pDib=(BYTE*) GlobalAllocPtr(GHND, m_dwLength);
		if(!m_pDib){
	        AfxMessageBox("Unable to allocate DIB memory");
			return;
		}
		memcpy(m_pDib, img.m_pDib, dwDIBHeadLength);
	    m_pData = (BYTE*) m_pDib + sizeof(BITMAPINFOHEADER) +
               sizeof(RGBQUAD) * img.NumColors();
	}
	//shrink the image
	ResizeImage(img.m_pData, scrw, scrh, m_pData, destw, desth);

    LPBITMAPINFOHEADER pBMIH;
    pBMIH=BMInfoHdPtr();
    pBMIH->biWidth = destw;
    pBMIH->biHeight = desth;

	Dib();						img.Dib();
	m_pPal=new CPalette;		CreateDIBPalette();		
}
///////////////////////////////////////////////////////////////////
/*CImage::CImage(CImage & img, CSize newSize)
{	
	WORD scrw,scrh,destw,desth;
	DWORD dwDIBHeadLength;

	img.Data();
	m_bIsDIB = img.m_bIsDIB;

	destw  = (WORD) newSize.cx;
	desth  = (WORD) newSize.cy;
	scrw   = img.Width();
	scrh   = img.Height();

	dwDIBHeadLength = sizeof(BITMAPINFOHEADER) + 
					  sizeof(RGBQUAD) * img.NumColors();

	DWORD dwBytes = Transform(destw);
	m_dwLength = dwDIBHeadLength + (DWORD) dwBytes * desth;

	m_pDib = NULL;
	if(img.m_pDib)
	{
		m_pDib=(BYTE*) GlobalAllocPtr(GHND, m_dwLength);
		if(!m_pDib){
	        AfxMessageBox("Unable to allocate DIB memory");
			return;
		}
		memcpy(m_pDib, img.m_pDib, dwDIBHeadLength);
	    m_pData = (BYTE*) m_pDib + sizeof(BITMAPINFOHEADER) +
               sizeof(RGBQUAD) * img.NumColors();
	}
	if((destw < scrw)&&(desth < scrh))
		ShrinkImage(img.m_pData, m_pData, scrw, scrh, destw, desth);
	else	//Magnify the image
		MagnifyImage(img.m_pData, m_pData, scrw, scrh, destw, desth);

    LPBITMAPINFOHEADER pBMIH;
    pBMIH=BMInfoHdPtr();
    pBMIH->biWidth = destw;
    pBMIH->biHeight = desth;

	Dib();						img.Dib();
	m_pPal=new CPalette;		CreateDIBPalette();		
}
*/
///////////////////////////////////////////////////////////////////
//before usere call this construction, must guarentee img.Bits() == 8 or 24
CImage::CImage(CImage & img, CSize newSize)//把已有的图像变成自己想要的大小
{	
	int		bpp, byteperline;
	WORD	scrw,	scrh,	destw,	desth;
	DWORD	dwDIBHeadLength;

	img.Data();
	m_bIsDIB = img.m_bIsDIB;

	bpp = img.Bits(); 
	if(bpp != 8 && bpp != 24)
		return;
	destw  = (WORD) newSize.cx;
	desth  = (WORD) newSize.cy;
	scrw   = img.Width();
	scrh   = img.Height();

	if((destw-scrw)*(desth-scrh) < 0)
	{
		//AfxMessageBox("Current version only support!");
		//return;
	}
	byteperline = ByteNumForOneLine(destw, bpp);

	dwDIBHeadLength = sizeof(BITMAPINFOHEADER) + 
					  sizeof(RGBQUAD) * img.NumColors();

	m_dwLength = dwDIBHeadLength + (DWORD) byteperline * desth;

	m_pDib = NULL;
	if(img.m_pDib)
	{
		m_pDib=(BYTE*) GlobalAllocPtr(GHND, m_dwLength);
		if(!m_pDib){
	        AfxMessageBox("Unable to allocate DIB memory");
			return;
		}
		memcpy(m_pDib, img.m_pDib, dwDIBHeadLength);
	    m_pData = (BYTE*) m_pDib + sizeof(BITMAPINFOHEADER) +
               sizeof(RGBQUAD) * img.NumColors();
	}

	Data();
	ResizeImage(img.Data(), scrw, scrh, m_pData, destw, desth);

    LPBITMAPINFOHEADER pBMIH;
    pBMIH=BMInfoHdPtr();
    pBMIH->biWidth = destw;
    pBMIH->biHeight = desth;

	Dib();						img.Dib();
	m_pPal=new CPalette;		CreateDIBPalette();		
}
///////////////////////////////////////////////////////////////////
CImage::~CImage()
{
    if(m_dwLength != 0L) 
        GlobalFreePtr(m_pDib);  // free the DIB memory
    if(m_pPal)
		delete m_pPal;
}

///////////////////////////////////////////////////////////////////
void CImage::Serialize(CArchive& ar)
{
    ar.Flush();
    if (ar.IsStoring()) 
	{
		WriteToFile(ar.GetFile());
    }
    else 
	{
		ReadFromFile(ar.GetFile()); 
    }
}

///////////////////////////////////////////////////////////////////
BOOL CImage::WriteAsBMP(CFile* pFile)
{
	Dib();
	BITMAPFILEHEADER bmfHdr; // Header for Bitmap file
	LPBITMAPINFOHEADER pBMIH=BMInfoHdPtr();   // Pointer to DIB info structure
	
	if (m_pDib == NULL)
		return FALSE;
	
	/* Fill in file type (first 2 bytes must be "BM" for a bitmap) */
	bmfHdr.bfType = 0x4d42;  // "BM"

	// Calculate the file size by adding the DIB size to sizeof(BITMAPFILEHEADER)

	bmfHdr.bfSize = m_dwLength + sizeof(BITMAPFILEHEADER);
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;

	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + pBMIH->biSize
											  + NumColors()*sizeof(RGBQUAD);
	WORD hdLength=sizeof(BITMAPINFOHEADER)
					+ NumColors()*sizeof(RGBQUAD);
	TRY {
		// Write the file header
		pFile->Write((BYTE*)&bmfHdr, sizeof(BITMAPFILEHEADER));
		// Write the DIB header
        pFile->Write(m_pDib, hdLength);
		// Write the DIB bits
        pFile->Write(m_pData, m_dwLength-hdLength);
    }
    CATCH (CFileException, e) {
        AfxMessageBox("Write error--possible disk full condition");
        return FALSE;
    }
    END_CATCH
    return TRUE;
}

///////////////////////////////////////////////////////////////////
/* The MakeBitmap()
   CDC* pDoc :    Device Context
   bmSize    :    Change the parameter and return it
   Function:      Replaces the DC's old existing bitmap 
                  with the new one construced from this DIB
   Return Value:  the old bitmap in DC
*/
////////////////////////////////////////////////////////    
CBitmap* CImage::MakeBitmap(CDC* pDC, CSize& bmSize)
{
    BITMAP bm;
    DWORD  dwFore, dwBack;
	LPBITMAPINFOHEADER pBMIH = BMInfoHdPtr();
	LPBITMAPINFO pBMI = BMInfoPtr();
    // checks to see whether DIB buffer is properly loaded
    if (m_dwLength == 0L) {
        bmSize.cx = bmSize.cy = 0;
        return NULL;
    }

    // this code conditions the DC for mono or color
	SetPalette(pDC);
    int nPlanes = pDC->GetDeviceCaps(PLANES);
    int nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
    CBitmap* pConfigBitmap = new CBitmap;
    char bits[100];
    if (pBMIH->biBitCount == 1) {
        pConfigBitmap->CreateBitmap(1, 1, 1, 1, bits);
    }
    else {
        pConfigBitmap->CreateBitmap(1, 1, nPlanes, nBitsPixel, bits);
    }
    CBitmap* pOriginalBitmap =
        (CBitmap*) pDC->SelectObject(pConfigBitmap);

// CreateDIBitmap "switches bits" for mono bitmaps, depending on colors,
//  so we'll fool it
    if (GetMonoColors(dwFore, dwBack)) {
        SetMonoColors(0L, 0xFFFFFFL);
    }
//the following CreateDIBitmap() creates a DDB from this DIB    
//#ifdef _WIN32
    HBITMAP hBitmap = ::CreateDIBitmap(pDC->GetSafeHdc(), pBMIH,
            CBM_INIT, (CONST BYTE*) m_pData, pBMI, DIB_RGB_COLORS);
//#else
//    HBITMAP hBitmap = ::CreateDIBitmap(pDC->GetSafeHdc(), pBMIH,
//            CBM_INIT, (BYTE*) m_pData, pBMI, DIB_RGB_COLORS);
//#endif
    if (hBitmap == NULL) {
        TRACE("null bitmap\n");  
        delete pDC->SelectObject(pOriginalBitmap); // delete config bitmap
        return NULL; // untested error logic
    }
    
    SetMonoColors(dwFore, dwBack);
    
    // Can't use CBitmap::FromHandle here because we need to
    //  delete the object later
    CBitmap* pBitmap = new CBitmap;
    pBitmap->Attach(hBitmap);
    pBitmap->GetObject(sizeof(bm), &bm);
    bmSize.cx = bm.bmWidth;
    bmSize.cy = bm.bmHeight;
    delete pDC->SelectObject(pBitmap); // delete configuration bitmap
    return pOriginalBitmap;
}
///////////////////////////////////////////////////////////////////
//create a DIBpalette according to the palette info in the inforhead of object
BOOL CImage::CreateDIBPalette()
{
	LPLOGPALETTE lpPal;      // pointer to a logical palette
	LPBITMAPINFO pBMI = BMInfoPtr();
	int i;                   // loop index
	WORD wNumColors;         // number of colors in color table
	BOOL bResult=FALSE;

	/* if handle to DIB is invalid, return FALSE */

	if (!m_pDib)  return FALSE;

   /* get the number of colors in the DIB */
	Dib();
	wNumColors = NumColors();
	if (wNumColors != 0)
	{
		/* allocate memory block for logical palette */
		/* if not enough memory, clean up and return NULL */

		lpPal = (LPLOGPALETTE) GlobalAllocPtr(GHND, sizeof(LOGPALETTE)
									+ sizeof(PALETTEENTRY)
									* wNumColors);
		if (lpPal == 0) return FALSE;

		/* set version and number of palette entries */
		lpPal->palVersion = 0x300;
		lpPal->palNumEntries = wNumColors;

		for (i = 0; i < (int)wNumColors; i++)
		{
			lpPal->palPalEntry[i].peRed = pBMI->bmiColors[i].rgbRed;
			lpPal->palPalEntry[i].peGreen = pBMI->bmiColors[i].rgbGreen;
			lpPal->palPalEntry[i].peBlue = pBMI->bmiColors[i].rgbBlue;
			lpPal->palPalEntry[i].peFlags = 0;
		}

		/* create the palette and get handle to it */
		delete m_pPal;
		m_pPal=new CPalette;
		bResult = m_pPal->CreatePalette(lpPal);
		GlobalFreePtr(lpPal);
	}

	return bResult;
}
///////////////////////////////////////////////////////////////////
BOOL CImage::CreateGreyPalette()
{
	LPLOGPALETTE lpPal;      // pointer to a logical palette
	LPBITMAPINFO pBMI = BMInfoPtr();
	HANDLE hLogPal;          // handle to a logical palette
	int i;                   // loop index
	WORD wNumColors=NumColors();         // number of colors in color table
	BOOL bResult=FALSE;
	/* allocate memory block for logical palette */
	hLogPal = ::GlobalAlloc(GHND, sizeof(LOGPALETTE)
								+ sizeof(PALETTEENTRY)
								* wNumColors);

	/* if not enough memory, clean up and return NULL */
	if (hLogPal == 0) return FALSE;
	lpPal = (LPLOGPALETTE) ::GlobalLock((HGLOBAL) hLogPal);

	/* set version and number of palette entries */
	lpPal->palVersion = 0x300;
	lpPal->palNumEntries = wNumColors;
	//set the grey value of palette  
	for (i = 0; i < (int)wNumColors; i++)
	{
		lpPal->palPalEntry[i].peRed = i;
		lpPal->palPalEntry[i].peGreen = i;
		lpPal->palPalEntry[i].peBlue = i;
		lpPal->palPalEntry[i].peFlags = 0;
		pBMI->bmiColors[i].rgbRed=i;
		pBMI->bmiColors[i].rgbGreen=i;
		pBMI->bmiColors[i].rgbBlue=i;
	}

		/* create the palette and get handle to it */
	delete m_pPal;
	m_pPal=new CPalette;
	bResult = m_pPal->CreatePalette(lpPal);
	::GlobalUnlock((HGLOBAL) hLogPal);
	::GlobalFree((HGLOBAL) hLogPal);
	return bResult;
}
///////////////////////////////////////////////////////////////////
//create a grey DIB  according to the color bmp 
//and creat a grey palette according to the 
//inforhead of  the color bmp
BOOL CImage::CreateGreyDib()
{
	BYTE	y[256];
	WORD	i, j, w, h; 
	DWORD	dwL;
	BOOL	bResult = FALSE;
	LPBITMAPINFO pBMI;

	if ( !m_pDib )  		return FALSE;

	w		= Width();
	h		= Height();
	dwL		= w * h;
	pBMI	= BMInfoPtr();

	switch(Bits())
	{
	case 8:
		{
			Data();
			for (i = 0; i < NumColors() ; i++)
			{
				y[i] = (BYTE)( float( pBMI->bmiColors[i] . rgbRed)  * 0.30
							 + float( pBMI->bmiColors[i] . rgbGreen)* 0.59
							 + float( pBMI->bmiColors[i] . rgbBlue) * 0.11);
				if(y[i] > 255) 
					y[i] = 255;
			}
			CreateGreyPalette();
			for(DWORD d=0 ; d<dwL ; d++) 
			{
				m_pData[d] = y[m_pData[d]];
			}

			Dib();
			return TRUE;
		}
	case 24:
		{
			CImage	*pGreyImage;
			DWORD	t1, t2;
			BYTE	*pGreyData, *pColData;
			
			pGreyImage	= new CImage(CSize(w, h), 256, 8);
			pColData	= Data();
			pGreyData	= pGreyImage -> Data();

			for(i=0; i<h; i++)
			{
				t1 = DWORD(i) * w * 3;
				for(j=0; j<w; j++)
				{
					t2 = t1 + 3 * j;
					pGreyData [ i * w + j ] = BYTE(
						  (float)(pColData[ t2 ] * 0.11)
						+ (float)(pColData[t2+1] * 0.59)
						+ (float)(pColData[t2+2] * 0.30));
				}
			}
			ImageClear();
			*this = *pGreyImage;
			Dib();
			delete pGreyImage;
			return TRUE;
		}
	default:
		{
			AfxMessageBox("Only convert 8 bits or 24 bits image to grey image");
			return FALSE;
		}
	}
}
///////////////////////////////////////////////////////////////////
BOOL CImage::Display(CDC* pDC, CPoint origin) const
{
    // direct to device--bypass the GDI bitmap
	LPBITMAPINFOHEADER pBMIH = BMInfoHdPtr();
	LPBITMAPINFO pBMI = BMInfoPtr();
    if (!m_pDib) {
        return FALSE; // nothing to display
    }
	if (!::SetDIBitsToDevice(pDC->GetSafeHdc(), origin.x, origin.y,
       (WORD) pBMIH->biWidth, (WORD) pBMIH->biHeight, 0, 0, 0,
       (WORD) pBMIH->biHeight, m_pData, pBMI,
       DIB_RGB_COLORS)) {
        return FALSE;
    }
	return TRUE;
}

///////////////////////////////////////////////////////////////////
BOOL CImage::Stretch(CDC* pDC, CPoint origin, CSize size) const
{
    // direct to device--bypass the GDI bitmap
	LPBITMAPINFOHEADER pBMIH = BMInfoHdPtr();
	LPBITMAPINFO pBMI = BMInfoPtr();
    if (!m_pDib) {
        return FALSE; // nothing to display
    }
	//::SetStretchBltMode (pDC->GetSafeHdc(), COLORONCOLOR);
    if (!::StretchDIBits(pDC->GetSafeHdc(), origin.x, origin.y,
        size.cx, size.cy, 0, 0, (WORD) pBMIH->biWidth,
        (WORD) pBMIH->biHeight, m_pData, pBMI,
        DIB_RGB_COLORS, SRCCOPY)) {
        return FALSE;
    }
    return TRUE;
}
///////////////////////////////////////////////////////////////////
WORD CImage::Bits() const
{
	ASSERT(m_pDib);
    return ((LPBITMAPINFOHEADER)m_pDib)->biBitCount;
}

///////////////////////////////////////////////////////////////////
DWORD CImage::Length() const
{
    return m_dwLength;
}
///////////////////////////////////////////////////////////////////

CSize CImage::Size() const
{
    ASSERT(m_pDib);
	LPBITMAPINFOHEADER pBMIH = BMInfoHdPtr();
    return CSize((int) pBMIH->biWidth, (int) pBMIH->biHeight);
}
///////////////////////////////////////////////////////////////////

WORD CImage::Height() const
{
    ASSERT(m_pDib);
	return (WORD)(((LPBITMAPINFOHEADER)m_pDib)->biHeight);
}
///////////////////////////////////////////////////////////////////

WORD CImage::Width() const
{
    ASSERT(m_pDib);
    return (WORD) (((LPBITMAPINFOHEADER)m_pDib)->biWidth);
}
///////////////////////////////////////////////////////////////////
//return the number of entries in the color pallete
WORD CImage::NumColors() const
{
    ASSERT(m_pDib);
	LPBITMAPINFOHEADER pBMIH = BMInfoHdPtr();
	if(m_pDib==NULL) return NULL;
	if(pBMIH->biClrUsed!=0)
		return (WORD)pBMIH->biClrUsed;
	switch(pBMIH->biBitCount)
	{
	case 1:
		return 2;
	case 4:
		return 16;
	case 8:
		return 256;
	default:
		return 0;
	}
}
///////////////////////////////////////////////////////////////////
LPBITMAPINFOHEADER CImage::BMInfoHdPtr() const
{
	return (LPBITMAPINFOHEADER)m_pDib;
}
///////////////////////////////////////////////////////////////////
LPBITMAPINFO CImage::BMInfoPtr() const
{
	return (LPBITMAPINFO)m_pDib;
}
///////////////////////////////////////////////////////////////////
RGBQUAD* CImage::BMColorTblPtr() const
{
	return (RGBQUAD*)((int)m_pDib+sizeof(BITMAPINFOHEADER));
}
///////////////////////////////////////////////////////////////////
void CImage::SetDIB(BOOL Flag)
{
	if(Flag) m_bIsDIB=TRUE;
	else m_bIsDIB=FALSE;
}
///////////////////////////////////////////////////////////////////
// To convert the non-DIB to DIB type
BYTE* CImage::Dib()
{
	if(!m_pDib) return NULL;
	if(m_bIsDIB) return m_pData;

	WORD		i, j;
	BYTE		*p1, *p2;
	WORD bpp	=	Bits();
	WORD bytepp =	bpp / 8;
	WORD w		=	Width();
	WORD h		=	Height();
	long lBpl	=	w * bytepp;
	WORD halfline = h / 2;

	//reverse the order of scanning
	//for(i=0 ; i<halfline-1 ; i++){/// Ms. Yao changed it!
	BYTE temp;
	p1 = m_pData;
	p2 = m_pData + lBpl * h;
	for(i=0 ; i<halfline; i++)
	{
		p2 -= lBpl;
		for(j=0; j<lBpl; j++)
		{
			temp  = p1[j];
			p1[j] = p2[j];
			p2[j] = temp;
		}
		p1 += lBpl;
	}

	if(((w * bpp) % 32)==0)
	{
		SetDIB();
		return m_pData;
	}
	/*otherwise, it is needed to pad data*/
    DWORD dwBNFOL = ByteNumForOneLine(w, bpp);
	for(i=h-1; i>0; i--)
	{
		p1 = m_pData + i * dwBNFOL;
		p2 = m_pData + i * lBpl;
		memmove(p1, p2, lBpl);
		memset(p1+lBpl, 0x00, (dwBNFOL - lBpl));
	}

	SetDIB();
	return m_pData;
}
/////////////////////////////////////////
//implement the reverse function of dib()
BYTE* CImage::Data()
{
	if(!m_pDib) return NULL;
	if(!m_bIsDIB) return m_pData;

	WORD	i,j;//WORD   A 16-bit unsigned integer
	BYTE	*p1, *p2;
	WORD	bpp		= Bits();//bpp
	WORD	Bytepp	= bpp / 8;//??
	WORD	w		= Width();
	WORD	h		= Height();
	WORD	halfline= h/2;

	LPBITMAPINFOHEADER pBMIH = BMInfoHdPtr( );

	if(pBMIH->biCompression!=BI_RGB)
	{
		AfxMessageBox("Con't process compressed image");
		return NULL;
	}

	if((w * bpp) % 32)//每一行所占的字节数不是4的倍数
	{
		long	t		= w * Bytepp;//每行按照象素点的个数计算的占用的字节数
		DWORD dwBNFOL	= ByteNumForOneLine(w, bpp);//每行在内存中占用的字节数
		p2 = p1 = m_pData;//m_pData到底指的是什么？？
		for(i=0; i<h-1; i++)
		{
			p1 += dwBNFOL;//我解释是两个buffer的指针不断的变化，从而每次把一个buffer中从相应位置（p1)开始的t这么多的单元值取进到另一个buffer（p2)中来,并且放到相应的位置中去（p2)
			p2 += t;      //同上面的注释
			memmove(p2, p1, t);//Moves one buffer to another.


		}
	}
/*/
	if((w * bpp) % 32)
	{
		long	lbpl	= w * Bytepp;
		DWORD dwBNFOL	= ByteNumForOneLine(w, bpp);
		for(i=0; i<h-1; i++)
		{
			p1 = m_pData + ( i + 1 ) * dwBNFOL;
			p2 = m_pData + ( i + 1 ) * lbpl;
			memmove(p2, p1, lbpl);
		}
	}
*/
	/*for(i=0 ; i<halfline-1 ; i++)**Ms. Yao changed!**/
	long lBytePerLine	= w * Bytepp;
	p1	= m_pData;//？？                                                                
	p2	= m_pData + h * lBytePerLine;
	BYTE temp;
	for(i = 0 ; i < halfline; i++)//？？why half
	{
		p2 -= lBytePerLine;
		for(j = 0; j < lBytePerLine; j++)
		{
			temp  = p1[j];
			p1[j] = p2[j];
			p2[j] = temp;
		}
		p1 += lBytePerLine;
	}

	SetDIB(FALSE);
	return m_pData;
}
////////////////// /////////////////////////////////////////////////
CPalette* CImage::Palette() const
{
	return m_pPal;
}
///////////////////////////////////////////////////////////////////

UINT CImage::SetPalette(CDC* pDC,BOOL bBackground) const
{
	if(!m_pPal->m_hObject) return FALSE;
	VERIFY(pDC->SelectPalette(m_pPal,bBackground));
	return(pDC->RealizePalette());
}
///////////////////////////////////////////////////////////////////
void CImage::SetMonoColors(DWORD dwForeground, DWORD dwBackground)
{
    if (Bits() != 1) {
        return;
    }
    unsigned long far* pPalette = (unsigned long far*)
                       BMColorTblPtr();
    *pPalette = dwForeground;
    *(++pPalette) = dwBackground;
    return;
}

///////////////////////////////////////////////////////////////////
BOOL CImage::GetMonoColors(DWORD& dwForeground, DWORD& dwBackground)
{
    if (Bits() != 1) {
        return FALSE;
    }
    unsigned long far* pPalette = (unsigned long far*)
                       BMColorTblPtr();
    dwForeground = *pPalette;
    dwBackground = *(++pPalette);
    return TRUE;
}

///////////////////////////////////////////////////////////////////
BOOL CImage::AllocateMemory(BOOL bRealloc) // bRealloc default = FALSE
{
    if (bRealloc) {  
        m_pDib = (BYTE*) GlobalReAllocPtr(m_pDib,
                                                m_dwLength, GHND);
    }
    else {
        m_pDib = (BYTE*) GlobalAllocPtr(GHND, m_dwLength);
    }
    if (!m_pDib) {
        AfxMessageBox("Unable to allocate DIB memory");
        m_dwLength = 0L;
        return FALSE;
    }
    return TRUE;
}
/*Record of Modify
 *	Date		Operator	Modify			
 * 1999.4.9		S.S.G		if the width of the bitmap can not be inter-divided by 8
 *							We must alloc enough additional memory!! and some situations
 *							must be processed
*/
BOOL CImage::ReadFromBMP(CFile * pFile)
{
	BITMAPFILEHEADER bmfHdr; // Header for Bitmap file
	BITMAPINFOHEADER bmhdr;	 //header for bitmap info
	long   biWidth;    
	long   biHeight; 

    ASSERT(m_dwLength == 0L); // DIB must be empty

	if ((pFile->Read((BYTE*)&bmfHdr, sizeof(bmfHdr)) !=
		sizeof(bmfHdr))) {
        AfxMessageBox("Read error");
        return FALSE;
    }
	if (bmfHdr.bfType != 0x4d42)
	{
        AfxMessageBox("Invalid bitmap file");
		return FALSE;
	}
	//1999.4.9 added
	if ((pFile->Read((BYTE*)&bmhdr, sizeof(bmhdr)) !=
		sizeof(bmhdr))) {
        AfxMessageBox("Read error");
        return FALSE;
    }
	if(bmhdr.biSize != sizeof(bmhdr))
	{
        AfxMessageBox("Invalid bitmap file");
		return FALSE;
	}
	if(bmhdr.biBitCount == 16 && bmhdr.biBitCount == 32)
	{
        AfxMessageBox("Sorry! Current version only support\n 8bpp and 24bpp BMP!");
		return FALSE;
	}

	if(bmhdr.biCompression != BI_RGB )
	{
        AfxMessageBox("Sorry! Current version do NOT support\n compressed BMP!");
		return FALSE;
	}
	biWidth = bmhdr.biWidth;
	biHeight= abs(bmhdr.biHeight); 
	DWORD dwBytes = ByteNumForOneLine(WORD(biWidth), bmhdr.biBitCount);
    //->|
    m_dwLength = pFile->GetLength();
	m_dwLength -= sizeof(BITMAPFILEHEADER);
	/*1999.4.9 added the following line*/
	//add the needed extent memory!!!
	m_dwLength += (dwBytes - (bmhdr.biBitCount / 8) * bmhdr.biWidth) * bmhdr.biHeight;

    if (!AllocateMemory()) {
        return FALSE;       
    }

	pFile->Seek(sizeof(BITMAPFILEHEADER), CFile::begin);
    TRY {
		pFile->Read(m_pDib, m_dwLength);//virtual UINT Read( void* lpBuf, UINT nCount );
		                                //lpBuf：Pointer to the user-supplied buffer that is to receive the data read from the file.
		                                //nCount：The maximum number of bytes to be read from the file. For text-mode files, carriage return–linefeed pairs are counted as single characters.





	}
	CATCH (CFileException, e) {
        AfxMessageBox("Read error");
        GlobalFreePtr(m_pDib);  // free the DIB memory
        return FALSE;
	}
	END_CATCH

    m_pData = (BYTE*) m_pDib + sizeof(BITMAPINFOHEADER) +
               sizeof(RGBQUAD) * NumColors();
	CreateDIBPalette();
	SetDIB(); 
    return TRUE;
}

 BOOL CImage::ReadFromPGM(CFile * pFile)
{
    // file assumed to be open
	LPBITMAPINFOHEADER pBMIH;
	char ch;	
	WORD w, h, i=0, j=0;
	char str[10];
	CString sfilehead;

	ASSERT(m_dwLength == 0);

    DWORD dwFileLength = pFile->GetLength();
	DWORD dwDataPos=0;
	pFile->Seek(0,CFile::begin);
    TRY {
		pFile->Read(sfilehead.GetBuffer(100), 100);
    	}
	CATCH (CFileException, e) {
        AfxMessageBox("Read error");
        return FALSE;
	}
	END_CATCH
	
	sfilehead.ReleaseBuffer();
	if(sfilehead == "")	{
		AfxMessageBox("Invalid PGM image file");
		return false;
	}
	if(sfilehead.GetAt(0) != 'P'||sfilehead.GetAt(1) != '5') {
		AfxMessageBox("Invalid PGM image file");
		return false;
	}

	int tt = 0;	dwDataPos = 0;
	while(tt != 4)//4: for YaleFaceBaseB, 3: for others
	{
		if(sfilehead.GetAt(dwDataPos) == '\n')
			tt++;
		dwDataPos++;
		if(dwDataPos > 100)
		{
			AfxMessageBox("The format is wrong!");
			return false;
		}
	}

	dwFileLength -= dwDataPos;

	i=0;	ch=sfilehead.GetAt(i++);
	while(ch!='\n')  ch=sfilehead.GetAt(i++);
	//for Yale FaceBaseB only
	ch=sfilehead.GetAt(i++);
	while(ch!='\n')  ch=sfilehead.GetAt(i++);

	j=0;  	ch=sfilehead.GetAt(i++);
	while(ch!=' ')
	   { str[j++]=ch; ch=sfilehead.GetAt(i++);}
	str[j]='\0';	w=atoi(str);

	j=0;	ch=sfilehead.GetAt(i++);
	while(ch!='\n') 
		{str[j++]=ch;	ch=sfilehead.GetAt(i++);}
	str[j]='\0';	h=atoi(str);

	if( DWORD(w*h) > dwFileLength)
	{
		AfxMessageBox("w*h != DataLength, The image data is wrong!");
		return false;
	}
	//make the number of bytes in one line can be divided exactly by 4
    DWORD dwBytes = Transform(w);//bytes per line

	m_dwLength = DWORD(dwBytes * h) + sizeof(BITMAPINFOHEADER)
				              + sizeof(RGBQUAD) * 256;
    if (!AllocateMemory()) {
		AfxMessageBox("Allocmemory Error");
        return FALSE;}

    pBMIH=BMInfoHdPtr();
	pBMIH->biSize = sizeof(BITMAPINFOHEADER);
    pBMIH->biWidth = w;
    pBMIH->biHeight = h;
    pBMIH->biPlanes = 1;
    pBMIH->biBitCount = 8;
    pBMIH->biCompression = BI_RGB;
    pBMIH->biSizeImage = 0;
    pBMIH->biXPelsPerMeter = 0;
    pBMIH->biYPelsPerMeter = 0;
    pBMIH->biClrUsed = 0;
    pBMIH->biClrImportant = 0;
    m_pData = (BYTE*) m_pDib + sizeof(BITMAPINFOHEADER) +
               sizeof(RGBQUAD) * NumColors();
    pFile->Seek(dwDataPos/*+1 980518 modified!!*/,CFile::begin);
    long dd;
	TRY {
			dd = pFile->Read(m_pData, DWORD(w * h));
			//pFile->Read(m_pData, dwFileLength);
		}
	CATCH (CFileException, e) {
		AfxMessageBox("Read error");
        GlobalFreePtr(m_pDib);  // free the DIB memory
        return FALSE;	}
	END_CATCH

	CreateGreyPalette();
	SetDIB(FALSE);
	Dib();   
    return TRUE;
}

BOOL CImage::WriteAsPGM(CFile * pFile)
{
	WORD w, h;
	char cWidth[5];
	char cHeight[5];
	char* PgmHead = new char[30];
	Data();

	strcpy(PgmHead, "P5\n");
	if (m_pDib == NULL)
		return FALSE;
	if(Bits() != 8) 
	{
		AfxMessageBox("It is not 8-greylevel image!");		
		return false;	}

	w = Width();		h = Height();
	itoa(w,cWidth,10);	itoa(h,cHeight,10);
	strcat(PgmHead,cWidth);		strcat(PgmHead," ");
	strcat(PgmHead,cHeight);	strcat(PgmHead,"\n255\n");

	// Calculate the file size by adding the DIB size to sizeof(BITMAPFILEHEADER)

	WORD hdLength = sizeof(BITMAPINFOHEADER)
					 + NumColors()*sizeof(RGBQUAD);
	TRY {
		pFile->Write(PgmHead,strlen(PgmHead));
        pFile->Write(m_pData, (long)w*h);
    }
    CATCH (CFileException, e) {
        AfxMessageBox("Write error--possible disk full condition");
        return FALSE;
    }
    END_CATCH
	Dib();
	delete PgmHead;
    return TRUE;
}

BOOL CImage::WriteAsData(CFile* pFile)
{
	WORD w = Width();
	WORD h = Height();
	BYTE* pData = Data();
	TRY 
	{
        pFile->Write(pData, (long)w*h);
    }
    CATCH (CFileException, e) 
	{
        AfxMessageBox("Write error--possible disk full condition");
        return FALSE;
    }
    END_CATCH
	Dib();
    return TRUE;
}

BOOL CImage::ReadFromFile(CFile * pFile)
{
	//if(ReadFromDAT(pFile))
	//	return TRUE;

	BOOL	bReturn;
	CString FileName=pFile->GetFilePath();
	
	int nPointPos=FileName.Find(".");
	FileName.MakeLower();
	FileName=FileName.Mid(nPointPos+1);
	bReturn = FALSE;

	if(FileName=="bmp")	
		return ReadFromBMP(pFile);
	if(FileName=="pgm")  
		return ReadFromPGM(pFile);
	if(FileName == "img")
		return ReadFromIMG(pFile);
	if(FileName == "dat")
		return ReadFromDAT(pFile);
	if(FileName == "raw")
		return ReadFromDAT(pFile);
	if(ReadFromBMP(pFile))
		return TRUE;
	return FALSE;
}

BOOL CImage::ReadFromFile(CString filename)
{
	CString	ext  = filename;
	int nPointPos=ext.Find(".");//CString::find-Finds a character or substring(子链) inside a larger string
	                            //Return Value:The zero-based index of the first character in this CString object that matches the requested substring or characters; -1 if the substring or character is not found.


	ext.MakeLower();//Converts this CString object to a lowercase(小写字母) string
	ext=ext.Mid(nPointPos+1);//This method extracts a substring of length nCount characters from this CString object, starting at position nFirst (zero-based). The method returns a copy of the extracted substring
	//if(ext == "jpg" || ext == "gif" )
	//	return ReadFromJPG(filename);

	CFile* pFile = new CFile;
	if(!pFile->Open(filename,CFile::modeRead))
	{
		AfxMessageBox("In CImage::ReadFromFile(),Can not opent the file!");
		return false;
	}

	BOOL b = ReadFromFile(pFile);//调用DOC中的另一个ReadFromFile函数，但是它的参数是一个CFile类的指针
	
	pFile->Close();
	delete pFile;
	return b;
}

BOOL	CImage::ReadFromJPG(CString filename)
{
	return FALSE;
}

BOOL CImage::WriteToFile(CFile * pFile)
{
	CString FileName=pFile->GetFileName();
	int nPointPos=FileName.Find(".");
	FileName.MakeLower();
	FileName=FileName.Mid(nPointPos+1);

	if(FileName=="bmp")	
		return WriteAsBMP(pFile);
	if(FileName=="pgm")  
		return WriteAsPGM(pFile);
	if(FileName=="bin" || FileName=="dat" || FileName=="raw" )
		return WriteAsData(pFile);
	if(FileName=="txt")
	{
		CString temp = pFile->GetFilePath();
		pFile -> Close();
		BOOL b = SaveAsText(temp, FALSE);
		pFile -> Open(temp, CFile::modeWrite);
		return b;
	}
	
	return WriteAsBMP(pFile);
}

BOOL CImage::WriteToFile(CString filename)
{
	CFile* pFile = new CFile;
	if(!pFile->Open(filename,CFile::modeWrite|CFile::modeCreate))
	{
		AfxMessageBox("In CImage::WriteToFile(),Can not opent the file!");
		return false;
	}

	BOOL b = WriteToFile(pFile);
	
	pFile->Close();
	delete pFile;
	return b;	
}

void CImage::ImageClear()
{
    if (m_pDib) 
        GlobalFreePtr(m_pDib);  // free the DIB memory
	m_pDib     = NULL;
	m_pData    = NULL;
	m_bIsDIB   = TRUE;
	m_dwLength = 0L; 
}

DWORD CImage::Transform(WORD width)
{
    DWORD dwBytes = (width * 8) / 32;
    if ((width * 8) % 32) {
        dwBytes ++;
    }
    dwBytes *= 4; //bytes per line	
	return dwBytes;
}

BOOL CImage::ReadFromIMG(CFile* pFile)
{
	int		bpp;
	LPBITMAPINFOHEADER pBMIH;
    DWORD dwFileLength = pFile->GetLength();
    WORD w,h;

	pFile->Read((WORD* )&w,sizeof(WORD));
	pFile->Read((WORD* )&h,sizeof(WORD));

	dwFileLength -= 2 * sizeof(WORD);
	bpp = int(dwFileLength / (w * h));
	if(bpp != 1 && bpp != 3)/* 8 or 24 */
	{
		AfxMessageBox("Wrong Data!");
		return false;
	}

    DWORD dwBytes = Transform(w);//bytes per line

	m_dwLength = DWORD(dwBytes * h * bpp) + sizeof(BITMAPINFOHEADER)
				              + sizeof(RGBQUAD) * 256;
    if (!AllocateMemory()) {
		AfxMessageBox("Allocmemory Error");
        return FALSE;}

    pBMIH=BMInfoHdPtr();
	pBMIH->biSize = sizeof(BITMAPINFOHEADER);
    pBMIH->biWidth = w;
    pBMIH->biHeight = h;
    pBMIH->biPlanes = 1;
    pBMIH->biBitCount = 8 * bpp;
    pBMIH->biCompression = BI_RGB;
    pBMIH->biSizeImage = 0;
    pBMIH->biXPelsPerMeter = 0;
    pBMIH->biYPelsPerMeter = 0;
    pBMIH->biClrUsed = 0;
    pBMIH->biClrImportant = 0;
    m_pData = (BYTE*) m_pDib + sizeof(BITMAPINFOHEADER) +
               sizeof(RGBQUAD) * NumColors();
	CreateGreyPalette();//？？

    TRY {
		pFile->Read(m_pData, dwFileLength);
	}
	CATCH (CFileException, e) {
        AfxMessageBox("Read error");
        GlobalFreePtr(m_pDib);  // free the DIB memory
        return FALSE;
	}
	END_CATCH

	SetDIB(true);
	Dib();
    return TRUE;
}

BOOL CImage::ReadFromDAT(CFile* pFile)
{
	LPBITMAPINFOHEADER pBMIH;
    DWORD dwFileLength = pFile->GetLength();
    WORD w,h;
	///////just for MIT facebase!!!
	w = 256;
	h = 240;
	//w = 128;
	//h = 120;
	//for bern faces
	w = 512;
	h = 342;

	if(dwFileLength <= (DWORD)(w * h))
	{
		AfxMessageBox("Wrong Data!");
		return false;
	}

    DWORD dwBytes = Transform(w);//bytes per line

	m_dwLength = DWORD(dwBytes * h) + sizeof(BITMAPINFOHEADER)
				              + sizeof(RGBQUAD) * 256;
    if (!AllocateMemory()) {
		AfxMessageBox("Allocmemory Error");
        return FALSE;}

    pBMIH=BMInfoHdPtr();
	pBMIH->biSize = sizeof(BITMAPINFOHEADER);
    pBMIH->biWidth = w;
    pBMIH->biHeight = h;
    pBMIH->biPlanes = 1;
    pBMIH->biBitCount = 8;
    pBMIH->biCompression = BI_RGB;
    pBMIH->biSizeImage = 0;
    pBMIH->biXPelsPerMeter = 0;
    pBMIH->biYPelsPerMeter = 0;
    pBMIH->biClrUsed = 0;
    pBMIH->biClrImportant = 0;
    m_pData = (BYTE*) m_pDib + sizeof(BITMAPINFOHEADER) +
               sizeof(RGBQUAD) * NumColors();
	CreateGreyPalette();

	//for bern faces
	pFile->Seek(32L, CFile::begin);

    TRY {
		pFile->Read(m_pData, (long(w) * h));
	}
	CATCH (CFileException, e) {
        AfxMessageBox("Read error");
        GlobalFreePtr(m_pDib);  // free the DIB memory
        return FALSE;
	}
	END_CATCH

	SetDIB(false);
	Dib();
    return TRUE;
}
/////////////////the property of the image////////////////
/////////Mean grey value
BYTE	CImage::MeanGrey()//我理解是计算黑白图片各个点灰度值的平均值，把各个点的灰度值加起来然后除以图像中象素点总的个数
{
	BYTE	avegrey;
	double sum = 0.0;
	DWORD i, imgsize;
	imgsize = (DWORD) Width() * Height();
	BYTE*	pData = Data();
	for(i=0;i<imgsize;i++)
		sum += pData[i];
	avegrey = (BYTE)(sum / imgsize);
	Dib();
	return avegrey;
}

double	CImage::Contrast()
{
	double	contrast;
	BYTE	low, high, *pData;
	double	scale;
	DWORD	i, imgSize, scaleNum, lc, rc;
	WORD	w, h;
	WORD	GreyDotNum[256];//？？

	w		=	Width();
	h		=	Height();
	imgSize	=	(DWORD) (w * h);
	scale	=	0.1;
	scaleNum=	(DWORD)(scale * imgSize);
	for(i=0;i<256;i++)
		GreyDotNum[i] = 0;
	pData	=	Data();
	for(i=0;i<imgSize;i++)//是不是保存的是整个图像某一个灰度值有几个点，累加??pData[i]中放的是不是灰度值
		GreyDotNum[pData[i]]++;
	for(i=0,lc=0;i<256;i++)
	{
		lc += GreyDotNum[i];//？？lc到底是什么
		if(lc >= scaleNum)
		{
			low = (BYTE)i;
			break;
		}
	}
	for(i=255,rc=0; i>0; i--)
	{
		rc += GreyDotNum[i];
		if(rc >= scaleNum)
		{
			high = (BYTE)i;
			break;
		}
	}
	contrast = (double)(high - low);

	Dib();
	return contrast;
}

double	CImage::CleanMeasure()//我的理解是：计算整个黑白图像每个点距离整个图像灰度值的平均值的差值，再求平方。先把每个点灰度值的均放差求和，再开方，然后再除以图片的大小（整个图片灰度值均放差的相对大小）
                              //实现，图像当然没有反应
{
	double	diff	=	0.0;
	DWORD	i;
	BYTE	mean	=	MeanGrey();
	BYTE*	pData	=	Data();
	DWORD	imgSize	=	Width() * Height();
	for(i=0,diff=0.0; i<imgSize; i++)
		diff += (pData[i] - mean) * (pData[i] - mean);
	diff = sqrt(diff) / imgSize;
	Dib();
	return diff;
}

double	CImage::NoiseMeasure()
{
	double	noise, dif;
	BYTE	*t;
	BYTE	mean	=	MeanGrey();
	BYTE*	pData	=	Data();
	WORD	i, j, w, h;
	long	imgSize, count;
	w = Width();
	h = Height();
	imgSize = (DWORD)(w * h);
	noise = 0.0;
	for(i=1, t = pData + w, count = 0; i < h - 1; i++, t += w) {
		for(j= 1; j < w - 1; j++)
		{
			dif = fabs( (double) (((int)*(t + j)) * 2 - (*(t + j -1)) - (*(t + j +1))) );
			dif += fabs( (double)(((int)*(t + j)) * 2 - (*(t + j - w)) - (*(t + j +w))) );
			dif += fabs( (double)(((int)*(t + j)) * 2 - (*(t + j - w - 1)) - (*(t + j + w + 1))) );
			dif += fabs( (double)(((int)*(t + j)) * 2 - (*(t + j - w + 1)) - (*(t + j + w - 1))) );
			if (dif / 4 < 30) {
				count++;
				noise += dif / 4;
			}
		}
	}
	noise /= count;

	Dib();
	return noise;
}

BYTE	CImage::Get8NeiborAver(BYTE* pGreyData, WORD w, WORD h, CPoint pos)
{
	double temp;
	int x = pos.x;
	int y = pos.y;
	int t = y * w + x;
	if(x == 0 && y != 0)
	{
		temp  =	pGreyData[t-w] + pGreyData[t-w+1] +
				pGreyData[t+1] + 
				pGreyData[t+w] + pGreyData[t+w+1];
		temp /= 5;
		return (BYTE)temp;
	}
	if(x != 0 && y == 0)
	{
		temp  =	pGreyData[t-1] + pGreyData[t+1] + 
				pGreyData[t+w-1] + pGreyData[t+w] + pGreyData[t+w+1];
		temp /= 5;
		return (BYTE)temp;
	}
	if(x == 0 && y != 0)
	{
		temp  =	pGreyData[t+1] +
				pGreyData[t+w] + pGreyData[t+w+1];
		temp /= 3;
		return (BYTE)temp;
	}

	if(x == w-1 && y != h-1)
	{
		temp  =	pGreyData[t-w-1] + pGreyData[t-w] +
				pGreyData[t-1] + 
				pGreyData[t+w-1] + pGreyData[t+w];
		temp /= 5;
		return (BYTE)temp;
	}
	if(x != w-1 && y == h-1)
	{
	temp  =	pGreyData[t-w-1] + pGreyData[t-w] + pGreyData[t-w+1] +
			pGreyData[t-1] + pGreyData[t+1];
	temp /= 5;
	return (BYTE)temp;
	}
	if(x = w-1 && y == h-1)
	{
		temp  =	pGreyData[t-w-1] + pGreyData[t-w] + 
				pGreyData[t-1];
		temp /= 3;
		return (BYTE)temp;
	}
	temp  =	pGreyData[t-w-1] + pGreyData[t-w] + pGreyData[t-w+1] +
			pGreyData[t-1]   + pGreyData[t+1] +
			pGreyData[t+w-1] + pGreyData[t+w] + pGreyData[t+w+1];
	temp /= 8;
	return (BYTE)temp;
}

double	CImage::Entropy()
{
	int  i, h, w;
	long Num[256], counter, pixel_sum;
	unsigned char  *img;
	double	p[256];
	double  Entropy;

	if(Bits() != 8) return false;
	img = Data();
	w = Width();
	h = Height();
	for(i=0;i<256;i++)	    
		Num[i] = 0;
	pixel_sum = (long)h*w;
	//calculate the Histogram
	long sum = 0;
	for (counter=0;counter<pixel_sum;counter++)
		Num[img[counter]]++;
	for(Entropy = 0, i=0;i<256;i++) {
		p[i] = (double)Num[i] / (double)pixel_sum;
		if(p[i] != 0)
			Entropy += p[i] * log(p[i]);
	}
	return -Entropy;
}

double	CImage::ImageQuality()
{
	double	imgQulity;

	BYTE	meangrey = MeanGrey();
	double	contrast = Contrast();
	double	noise	 = NoiseMeasure();
	double	entropy  = Entropy();
	TRACE("Meangrey=%d\n", meangrey);	
	TRACE("contrast=%f\n", contrast);	
	TRACE("noise=%f\n", noise);	
	TRACE("entropy=%f\n", entropy);	
	//imgQulity = (contrast/200.0) * (1.0/fabs(meangrey-128.0)) * (entropy/8.0) * (1.0/noise);

	//return imgQulity;

	double temp = log((entropy * entropy) / ( noise));
	if(temp > 0.0)
		//imgQulity = 20.0 * temp;
		imgQulity = temp;
	else
		imgQulity = 10.0;

	//if(meangrey < 64 || meangrey > 200 || contrast < 64)
		//imgQulity *= 0.5;
	imgQulity *= 20 * exp(-(meangrey-160.0)*(meangrey-160.0)/5832.0);
	imgQulity *= exp(-(contrast-180.0)*(contrast-180.0)/5832.0);
	return imgQulity;
}

DWORD CImage::ByteNumForOneLine(WORD nDIBWidth, int nBpp)
{
    DWORD dwBytes = (nDIBWidth * nBpp) / 32;
	if ((nDIBWidth * nBpp) % 32)//每一行图像所占用的字节（byte)是不是4的倍数，那么也就是说看看所占的位数(bit)是不是32的倍数 
		dwBytes ++;//在内存中，每一行所占的字节数是4的多少倍，

    dwBytes *= 4;//bytes per line//dwByte=dwByte*4
	
	return	dwBytes;
}
///////////////////////////////////////////////
/*
 * SSG, 1999.6. 2
 * Save the data as text
*/
///////////////////////////////////////////////
BOOL CImage::SaveAsText(CString	 filename, BOOL	bIsBinary)
{
	int		w;
	long	l, size;
	char	buf[20];
	CFile	f;
	BYTE	*pData;

	pData = Data();
	
	if (m_pDib == NULL)
		return FALSE;
	if(Bits() != 8) {
		AfxMessageBox("It is not 8-greylevel image!");		
		return false;	
	}
	w = Width();
	size = w * Height();

	f.Open(filename, CFile::modeCreate | CFile::modeWrite );
	if(bIsBinary)
	{
		for(l=0; l<size; l++)
		{
			if(*(pData + l) != 0)
				sprintf(buf, "*");
			else
				sprintf(buf, ".");
			f.Write(buf, strlen(buf));
			if( (l+1)%w == 0){
				f.Write("\n", 1);
			}
		}
	}
	else
	{
		for(l=0; l<size; l++)
		{
			sprintf(buf, "%3d ", *(pData + l) );
			f.Write(buf, strlen(buf));
			if( (l+1)%w == 0){
				f.Write("\n", 1);
			}
		}
	}
	f.Close();
    
	Dib();
    return TRUE;
}

BOOL CImage::ValidImage()
{
	if(m_dwLength == 0L || m_pData == NULL || m_pDib == NULL)
		return FALSE;
	else
		return TRUE;
}

/********************************************************************
///////////////////////////from the CImage///////////////////
********************************************************************/


void CImage::Sobel()
{
	int conv1[3][3],conv2[3][3];
	int i,j,value,v1,v2,v3;
	long j0,j1,j2,target_line_offset,source_line1_offset,source_line2_offset;//A 32-bit signed integer
	unsigned char  *img, *sobbed;
	int high, width;

	width = Width();
	high = Height();
	CSize size=CSize(width,high);
	CImage* sobel=new CImage(size);

	img = Data();//??
	sobbed = sobel->Data();//??

	memset(sobbed, '\0', (long)high * width);

	/* inter pixel */
	conv1[0][0]=1; conv1[0][1]=2; conv1[0][2]=1;
	conv1[1][0]=0; conv1[1][1]=0; conv1[1][2]=0;
	conv1[2][0]=-1; conv1[2][1]=-2; conv1[2][2]=-1;

	conv2[0][0]=1; conv2[0][1]=0; conv2[0][2]=-1;
	conv2[1][0]=2; conv2[1][1]=0; conv2[1][2]=-2;
	conv2[2][0]=1; conv2[2][1]=0; conv2[2][2]=-1;

	target_line_offset=(long)-width;//??
	source_line1_offset=0;
	source_line2_offset=(long)width;
	for (i=1;i<high-1;i++) {
		target_line_offset=source_line1_offset; 
		source_line1_offset=source_line2_offset;
		source_line2_offset+=(long)width;
		j1=0;	  j2=1;
		for (j=1;j<width-1;j++) {
			j0=j1;	j1=j2;	j2++;
			value=0;
			v1=(int)*(img+target_line_offset+j0)*conv1[0][0];
			v1+=(int)*(img+target_line_offset+j1)*conv1[0][1];
			v1+=(int)*(img+target_line_offset+j2)*conv1[0][2];
			v2=(int)*(img+source_line1_offset+j0)*conv1[1][0];
			v2+=(int)*(img+source_line1_offset+j1)*conv1[1][1];
			v2+=(int)*(img+source_line1_offset+j2)*conv1[1][2];
			v3=(int)*(img+source_line2_offset+j0)*conv1[2][0];
			v3+=(int)*(img+source_line2_offset+j1)*conv1[2][1];
			v3+=(int)*(img+source_line2_offset+j2)*conv1[2][2];
			value=abs(v1+v2+v3);
			v1=(int)*(img+target_line_offset+j0)*conv2[0][0];
			v1+=(int)*(img+target_line_offset+j1)*conv2[0][1];
			v1+=(int)*(img+target_line_offset+j2)*conv2[0][2];
			v2=(int)*(img+source_line1_offset+j0)*conv2[1][0];
			v2+=(int)*(img+source_line1_offset+j1)*conv2[1][1];
			v2+=(int)*(img+source_line1_offset+j2)*conv2[1][2];
			v3=(int)*(img+source_line2_offset+j0)*conv2[2][0];
			v3+=(int)*(img+source_line2_offset+j1)*conv2[2][1];
			v3+=(int)*(img+source_line2_offset+j2)*conv2[2][2];
			v3=abs(v1+v2+v3);

//?			value=sqrt(value*value+v3*v3);
			value=(value>v3)?value:v3;
			if (value>255) value=255;
				*(sobbed+target_line_offset+j0)=value;
		}
	}
	memcpy(img, sobbed, (long)high * width);
	Dib();
	delete sobel;
}
int  CImage::DajinThre()
{
	long	largest = 0L;
	int		i, grey, r_v, h, w;
	long	p[GREY], pixel_sum1, pixel_sum2, grey_sum1, grey_sum2;
	double	cigma, dif_cigma, mean1, mean2;
	BYTE	*img;

	img = Data();
	w	= Width();
	h	= Height();

	for(i = 0; i < GREY; i++)	    
		p[i]=0;
	pixel_sum2 = long(h) * w;
	grey_sum1  = grey_sum2  = 0;
	for(pixel_sum1=0; pixel_sum1<pixel_sum2; pixel_sum1++)
	{
		grey = *(img+pixel_sum1) + 1;
		grey_sum2 += grey;
		p[grey]++;
	}

	pixel_sum1=0;
	dif_cigma=double(0);
	for (i=0;i<254;i++)		
	{
		pixel_sum1+=p[i];     pixel_sum2-=p[i];
		grey_sum1+=(p[i]*(i+1));  grey_sum2-=(p[i]*(i+1));
		if (pixel_sum2!=0)	{
			if (pixel_sum1!=0)	{
				mean1=double(grey_sum1/pixel_sum1);
				mean2=double(grey_sum2/pixel_sum2);
				cigma=(double)pixel_sum1*(double)pixel_sum2*fabs(mean1-mean2);
				if (cigma<0)     return(256);
				if (cigma>dif_cigma)	{
					dif_cigma=cigma; r_v=i;
				}
			}
		}
		else      break;
	}

/*	for(i = 0; i < GREY; i++)
		if(p[i] > largest)
			largest = p[i]; */
	Dib();
	return(r_v);
}

void CImage::Binary(int t, BOOL bBackground)//从程序上来看，我认为是对于黑白图像来说的，一个点的灰度值>t,那么该店的灰度值被赋予255 or 0,具体要看bBackground的值
                                            //从自己在菜单中建立该函数应用来看，好像对于彩色图像变为黑白图像再调用该函数的效果与自己的分析有差距。而且对于黑白图像效果也不是很好
{//二值函数
	                                        //但是改变t=100值，对于彩色图片效果明显好转
	unsigned char* img;
	long l1,l2;
	int h, w;

	img = Data();
	h = Height();
	w = Width();

	l2=(long)h*w;
	if(bBackground)
		for (l1=0;l1<l2;l1++)
			if (*(img+l1)>t)
					*(img+l1)=255;
			else
					*(img+l1)=0;

	if(!bBackground)
		for (l1=0;l1<l2;l1++)
			if (*(img+l1)>t)
				*(img+l1)=0;
			else
				*(img+l1)=255;
	Dib();
}

void CImage::MidFilter()
{
	BYTE* pImageData = Data();
	DWORD dwWidth = Width();
	DWORD dwHeight = Height();
	DWORD dwImageLength = dwWidth*dwHeight;
/////////////////////////////////////
	BYTE *lpTempData = new BYTE[dwImageLength];	 
	int nWinSize = 3;
	int nHist[256];
	BYTE cSortData[100];
	int nCount;
	int nWinWid,nWinHei;
	BYTE cMidValue;
	int nNumLessMid;
	for(nCount=0;nCount<256;nCount++)
		nHist[nCount]=0;
	for(int nHeiCount=0;nHeiCount<(int)dwHeight;nHeiCount++)
		{for(nCount=0;nCount<256;nCount++)
			nHist[nCount]=0;
		 for(nWinHei=0;nWinHei<nWinSize;nWinHei++)
		   	for(nWinWid=0;nWinWid<nWinSize;nWinWid++)
				{cSortData[nWinHei*nWinSize+nWinWid]=
							GetBit(pImageData,dwWidth,dwHeight,0-(nWinSize-1)/2+nWinWid,nHeiCount-(nWinSize-1)/2+nWinHei);
			 	 nHist[cSortData[nWinHei*nWinSize+nWinWid]]++;
			 	 for(nCount=nWinHei*nWinSize+nWinWid;nCount>0;nCount--)
			 		{if(cSortData[nCount]<cSortData[nCount-1])
			 			{unsigned char cTemp=cSortData[nCount];
			 		 	 cSortData[nCount]=cSortData[nCount-1];
			 		 	 cSortData[nCount-1]=cTemp;
			 		 	}
				 	 else break;
				 	}
			 	}  
		 *(lpTempData+DWORD(nHeiCount)*dwWidth) = cSortData[(nWinSize*nWinSize-1)/2];
		 cMidValue = cSortData[(nWinSize*nWinSize-1)/2];			 	     
		 nNumLessMid = 0;
		 for(nCount=0;nCount<=(nWinSize*nWinSize-1)/2;nCount++)
		 	{if(cMidValue>cSortData[nCount])
		 		nNumLessMid++;
		 	 else break;
		 	 }		 
		 for(int nWidCount=1;nWidCount<(int)dwWidth;nWidCount++)
		 	{for(nCount=0-(nWinSize-1)/2;nCount<=(nWinSize-1)/2;nCount++)
		 		{nHist[GetBit(pImageData,dwWidth,dwHeight,nWidCount-(nWinSize+1)/2,nHeiCount+nCount)]--;
		 		 if(GetBit(pImageData,dwWidth,dwHeight,nWidCount-(nWinSize+1)/2,nHeiCount+nCount)<cMidValue)
		 		 	nNumLessMid--;
		 		 }
		 	 for(nCount=0-(nWinSize-1)/2;nCount<=(nWinSize-1)/2;nCount++)
		 		{nHist[GetBit(pImageData,dwWidth,dwHeight,nWidCount+(nWinSize-1)/2,nHeiCount+nCount)]++;
		 		 if(GetBit(pImageData,dwWidth,dwHeight,nWidCount+(nWinSize-1)/2,nHeiCount+nCount)<cMidValue)
		 		 	nNumLessMid++;
		 		 }               
		 	 if(nNumLessMid>(nWinSize*nWinSize-1)/2)
		 	 	do{cMidValue--;
				   nNumLessMid-=nHist[cMidValue];
				 }while(nNumLessMid>(nWinSize*nWinSize-1)/2);
		 	 	
		 	 else if(nNumLessMid<(nWinSize*nWinSize-1)/2)
		 	 	{while(nNumLessMid<((nWinSize*nWinSize-1)/2)&&(nNumLessMid+nHist[cMidValue])<=(nWinSize*nWinSize-1)/2)
					{nNumLessMid+=nHist[cMidValue];
					 cMidValue++;
					 }
		 	 	} 
		 	 *(lpTempData+DWORD(nHeiCount)*dwWidth+nWidCount) = cMidValue;

		 	 }		 	 	
		}

	for(DWORD dwCount=0;dwCount<dwImageLength;dwCount++)
		*(pImageData+dwCount) = *(lpTempData+dwCount);
	delete lpTempData;	
	Dib();
}


void CImage::Smoothing()
{
	int conv[3][3];
	int i,j,value,v1,v2,v3,high,width;
	long j0,j1,j2,target_line_offset,source_line1_offset,source_line2_offset;
	BYTE *img, *smooth;

	high = Height();
	width = Width();
	img = Data();
	CSize size=CSize(width,high);
	CImage* dest=new CImage;//(size);
	*dest = *this;
	smooth = dest->Data();

	//memset(smooth, '\0', (long)high * width);

	/* inter pixel */
	conv[0][0]=1; conv[0][1]=1; conv[0][2]=1;
	conv[1][0]=1; conv[1][1]=1; conv[1][2]=1;
	conv[2][0]=1; conv[2][1]=1; conv[2][2]=1;

	target_line_offset=(long)-width;
	source_line1_offset=0;
	source_line2_offset=(long)width;
	for (i=1;i<high;i++)	{
		target_line_offset=source_line1_offset;
		source_line1_offset=source_line2_offset;
		source_line2_offset+=(long)width;
		j1=0;	  j2=1;
		for (j=1;j<width;j++)		{
			j0=j1;	j1=j2;	j2++;
			value=0;
			v1=(int)*(img+target_line_offset+j0)*conv[0][0];
			v1+=(int)*(img+target_line_offset+j1)*conv[0][1];
			v1+=(int)*(img+target_line_offset+j2)*conv[0][2];
			v2=(int)*(img+source_line1_offset+j0)*conv[1][0];
			v2+=(int)*(img+source_line1_offset+j1)*conv[1][1];
			v2+=(int)*(img+source_line1_offset+j2)*conv[1][2];
			v3=(int)*(img+source_line2_offset+j0)*conv[2][0];
			v3+=(int)*(img+source_line2_offset+j1)*conv[2][1];
			v3+=(int)*(img+source_line2_offset+j2)*conv[2][2];
			value=(v1+v2+v3)/9;
			if (value>255) value=255;
			*(smooth+target_line_offset+j0)=value;
		}
	}
	memcpy(img, smooth, (long)high * width);
	Dib();
	delete dest;
}

void CImage::Thin()
{
	int	Bnum, Anum, CN, max;
	register int finish, x, y,	TW = 1;

	int height, width;
	BYTE* binimg;

	BYTE *up, *down, *left, *right, *curr;
	BYTE *upleft, *upright, *downleft, *downright;
	BYTE	*offset, *temp;
	DWORD offset1;
	long	size;

	binimg = Data();
	height = Height();
	width = Width();

	offset = binimg + (long)width - 1L;
	finish = 0;	 //thinner is not finished
	size = (long)height * width;
	CImage* tmp=new CImage(CSize(width,height));
	temp=tmp->Data();
	memset(temp, '\0', size);

	// add weight calculation:    source--binimg ==> dest--binimg
	while (finish != 1) {

		finish = 1;
		offset1 = (long)width - 1L;

		for (x = 1; x < height - 1; x++) {
			offset1 += 2L;
			for (y = 1; y < width - 1; y++) {
				if (*(binimg + offset1) == TW) {
					curr = binimg + offset1;
					up   = curr - (unsigned long)width;
					down = curr + (unsigned long)width;
					left = curr - 1L;
					right= curr + 1L;
					if (TW >= 64)	{
						max = *up + *down + *left + *right;
						if (max > 255)
							*(temp + offset1) = 255;
						else
							*(temp + offset1) = (BYTE)max;
						finish = 2;
					}
					else	{
						*(temp + offset1) = *up + *down + *left + *right;
						finish = 0;
					}
				}// if
				offset1++;
			}// for y
		}// for x

		if (finish == 0) {
			TW *= 4;
			memcpy(binimg, temp, size);
		}// if
		else	if(finish == 2)
						memcpy(binimg, temp, size);
	}// while

	// get raletive weight:		source--binimg ==> dest--temp
	memset(temp, '\0', size);
	offset1 = (long)width - 1L;
	for (x = 1; x < height - 1; x++) {
		offset1 += 2L;
		for (y = 1; y < width - 1; y++) {
			if (*(binimg + offset1) != 0) {
				if (*(binimg + offset1) == 255)
					*(temp + offset1) = 4;
				else {
					curr = binimg + offset1;
					up   = curr - (unsigned long)width;
					down = curr + (unsigned long)width;
					left = curr - 1L;
					right= curr + 1L;
					upleft = up - 1L;
					upright = up + 1L;
					downleft = down - 1L;
					downright = down + 1L;

					if (*curr >= *up && *curr >= *down)
						(*(temp + offset1))++;
					if (*curr >= *left && *curr >= *right)
						(*(temp + offset1))++;
					if (*curr >= *upleft && *curr >= *downright)
						(*(temp + offset1))++;
					if (*curr >= *downleft && *curr >= *upright)
						(*(temp + offset1))++;
				}// else
			}// if *binimg+offset1
			offset1++;
		}// for y
	}// for x

	// delete non_skel pixel:		source--temp ==> dest--temp
	offset1 = (long)width - 1L;
	for (x = 1; x < height - 1; x++) {
		offset1 += 2L;
		for (y = 1; y < width - 1; y++) {
			if ((*(temp + offset1) > 0) && (*(temp + offset1) < 3)) {

				curr = temp + offset1;

				up   = curr - (unsigned long)width;
				down = curr + (unsigned long)width;
				left = curr - 1L;
				right= curr + 1L;
				upleft = up - 1L;
				upright = up + 1L;
				downleft = down - 1L;
				downright = down + 1L;

				Bnum = 0;
				if (*up)  Bnum++;
				if (*down) Bnum++;
				if (*left)  Bnum++;
				if (*right)  Bnum++;
				if (*upleft)  Bnum++;
				if (*upright)  Bnum++;
				if (*downleft)  Bnum++;
				if (*downright)  Bnum++;

				if (Bnum > 1) {
					Anum = 0;
					if (*up == 0)
						if ((*upright    != 0) || (*right != 0))  Anum++;
					if (*right == 0)
						if ((*downright != 0) || (*down != 0))  Anum++;
					if (*down == 0)
						if ((*downleft != 0) || (*left != 0)) Anum++;
					if (*left == 0)
						if ((*upleft != 0) || (*up != 0))  Anum++;
					if (Anum == 1)  *curr = 0;
				}//if Bnum
			}// if *temp+offset1
			offset1++;
		}// for y
	}// for x

	do {
		finish = 0;
		offset1 = (long)width - 1L;
		for (x = 1; x < height - 1; x++) {
			offset1 += 2L;
			for (y = 1; y < width - 1; y++) {
				if ((*(temp + offset1) > 0) && (*(temp + offset1) < 3)) {
					curr = temp + offset1;
					up   = curr - (unsigned long)width;
					down = curr + (unsigned long)width;
					left = curr - 1L;
					right= curr + 1L;
					upleft = up - 1L;
					upright = up + 1L;
					downleft = down - 1L;
					downright = down + 1L;
					Bnum = 0;
					if (*up)  Bnum++;
					if (*down) Bnum++;
					if (*left)  Bnum++;
					if (*right)  Bnum++;
					if (*upleft)  Bnum++;
					if (*upright)  Bnum++;
					if (*downleft)  Bnum++;
					if (*downright)  Bnum++;
					if (Bnum < 5) {
						Anum = 0;
						if (*up == 0)
							if ((*upright    != 0) || (*right != 0))  Anum++;
						if (*right == 0)
							if ((*downright != 0) || (*down != 0))  Anum++;
						if (*down == 0)
							if ((*downleft != 0) || (*left != 0)) Anum++;
						if (*left == 0)
							if ((*upleft != 0) || (*up != 0))  Anum++;
						if (Anum == 1)  {
							*curr = 0;
							finish = 1;
						}//if Anum
					}//if Bnum
				}// if *temp+offset1
				offset1++;
			}// for y
		}// for x
	}while (finish != 0);

	// post_processing
	memcpy(binimg, temp, size);
	offset = binimg + (long)width - 1L;
	for (x = 1; x < height - 1; x++) {
		offset += 2L;
		for (y = 1; y < width - 1; y++) {
			if (*offset > 0) {

				up   = offset - (unsigned long)width;
				down = offset + (unsigned long)width;
				left = offset - 1L;
				right= offset + 1L;
				upleft = up - 1L;
				upright = up + 1L;
				downleft = down - 1L;
				downright = down + 1L;

				CN = 0;
				if (*up)
					if ((*upright) == 0)   CN++;  else ;
				else
					if (*upright)          CN++;

				if (*upright)
					if ((*right) == 0)     CN++;  else ;
				else
					if (*right)            CN++;

				if (*right)
					if ((*downright) == 0) CN++;  else ;
				else
					if (*downright)        CN++;

				if (*downright)
					if ((*down) == 0)      CN++;  else ;
				else
					if (*down)             CN++;

				if (*down)
					if ((*downleft) == 0)  CN++;  else ;
				else
					if (*downleft)         CN++;

				if (*downleft)
					if ((*left) == 0)      CN++;  else ;
				else
					if (*left)             CN++;

				if (*left)
					if ((*upleft) == 0)    CN++;  else ;
				else
					if (*upleft)           CN++;

				if (*upleft)
					if ((*up) == 0)        CN++;  else ;
				else
					if (*up)               CN++;

				Bnum = 0;
				if (*up)  Bnum++;
				if (*down) Bnum++;
				if (*left)  Bnum++;
				if (*right)  Bnum++;
				if (*upleft)  Bnum++;
				if (*upright)  Bnum++;
				if (*downleft)  Bnum++;
				if (*downright)  Bnum++;

				if (CN == 2) {
					if (Bnum >= 3)	*offset = 0;
					else if (Bnum > 0)
							if ((*offset) == 1)
								*offset = 0;
				}//if CN == 2

				if ((CN == 4) && (Bnum >= 2)) {
					Anum = 0;
					if (*up == 0)
						if ((*upright		!= 0) || (*right != 0))  Anum++;
					if (*right == 0)
						if ((*downright != 0) || (*down != 0))  Anum++;
					if (*down == 0)
						if ((*downleft	!= 0) || (*left != 0)) Anum++;
					if (*left == 0)
						if ((*upleft != 0) || (*up != 0))  Anum++;
					if (Anum == 1)  *offset = 0;
				}// if CN == 4

				if (*offset == 1 && Bnum == 0 )  *offset = 0;

				if (*offset == 3 && Bnum == 1) {
					Bnum = 0;
					if (*up == 1)  Bnum++;
					if (*down == 1) Bnum++;
					if (*left == 1)  Bnum++;
					if (*right == 1)  Bnum++;
					if (*upleft == 1)  Bnum++;
					if (*upright == 1)  Bnum++;
					if (*downleft == 1)  Bnum++;
					if (*downright == 1)  Bnum++;
					if (Bnum == 1)	*offset = 0;
				}//if Bnum == 1
			 }// if *offset > 0
			offset++;
		}// for y
	}// for x

	offset = binimg + (long)width - 1L;
	for (x = 1; x < height - 1; x++) {
		offset += 2L;
		for (y = 1; y < width - 1; y++) {
			if (*offset != 0) 	*offset = 255;
//			if (*offset != 0) 	*offset = 1;
			offset++;
		}// for y
	}// for x
	delete tmp;
}// thin

BOOL CImage::NormalHistogram(BYTE step)
{
	BYTE g;
	int h, w;
	BYTE newgrey[GREY];
	long counter, pixel_sum;
 	if(Bits() !=8)
	{
		AfxMessageBox("can't process non-8bits bitmap!");
		return false;
	}

	BYTE* img = Data();
	w = Width();
	h = Height();
 	pixel_sum = (long)h*w;

	HistogramEqualize(newgrey, step);

	for (counter=0;counter<pixel_sum;counter++)
	{
		g = *(img+counter);
		*(img+counter) = newgrey[g];
	}
	Dib();
	return true;
}

BOOL CImage::DisplayHistogram(CDC * pDC, CPoint pos)
{
	int  i, max, grey, h, w;
	long Num[GREY], counter, pixel_sum;
	unsigned char  *img;
	double	fDelta = 0.0;

	if(Bits() != 8) return false;
	img = Data();
	w = Width();
	h = Height();
	for(i=0;i<GREY;i++)	    
		Num[i] = 0;
	pixel_sum = (long)h*w;
	//calculate the Histogram
	long sum = 0;
	for (counter=0;counter<pixel_sum;counter++)
	{
		grey=*(img+counter);
		sum += grey;
		Num[grey]++;
	}
	sum /= pixel_sum;
	max = 0;
	for(i=0;i<GREY;i++)
	{
		if(max < Num[i])
			max = Num[i];
		fDelta += double(Num[i]*((i-sum)*(i-sum)));
	}
	fDelta = sqrt(fDelta/pixel_sum);
	double scale = (double)100 / max;
	max = 100;
	//display it
	pos += CPoint(0,max+20);
	pDC->MoveTo(pos);
	pDC->LineTo(pos + CPoint(GREY+20,0) );
	pDC->LineTo(pos + CPoint(GREY+20,0) + CPoint(-10,-5) );
	pDC->MoveTo(pos + CPoint(GREY+20,0) );
	pDC->LineTo(pos + CPoint(GREY+20,0) + CPoint(-10,5) );
	pDC->MoveTo(pos);
	pDC->LineTo(pos + CPoint(0,-(max+10)) );
	pDC->LineTo(pos + CPoint(0,-(max+10)) + CPoint(-5,10));
	pDC->MoveTo(pos + CPoint(0,-(max+10)) );
	pDC->LineTo(pos + CPoint(0,-(max+10)) + CPoint(5,10));
	pDC->MoveTo(pos);
	for(i=0;i<GREY;i++)
	{
		pDC->SetPixel(pos+CPoint(i, -(int)(Num[i] * scale) ), 0);
		//if(Num[i] != 0)
		pDC->MoveTo(pos+CPoint(i, -(int)(Num[i] * scale)));
		pDC->LineTo(pos+CPoint(i, 0)); 
	}
	char str[100];
	sprintf(str, "u=%d, s=%f", sum, fDelta);
	pDC->TextOut(pos.x+40, pos.y+10, str);
	Dib();
	return true;
}

BOOL CImage::LaplaceOperator(int*	pnEdge)
{
	int conv[3][3] = { -1,  -1, -1,
					   -1,   8,  -1,
					   -1,  -1, -1};
	int		i, j, w, h;
	long	val, off, l;
	BYTE	*img, *pLap;
	BOOL	flag = FALSE;
	int		*pn;

	if(pnEdge != NULL)
		flag = TRUE;
	pn	= pnEdge;
	h	= Height();
	w	= Width();
	img = Data();
	CSize size	=	CSize(w,h);
	CImage* newimg = new CImage(size);
	pLap = newimg->Data();

	memcpy(pLap, img, (long)h * w);
	if(flag == TRUE)
		for(l=0; l<long(w)*h; l++)
			*pn++ = 0;

	for(j=1,off=w;j<h-1;j++,off+=w)
		for(i=1;i<w-1;i++)
		{
			val  = 0;
			val += *(img + (off - w) + i-1) *  conv[0][0];
			val += *(img + (off - w) +  i ) *  conv[0][1];
			val += *(img + (off - w) + i+1) *  conv[0][2];
			
			val += *(img +   off     + i-1) *  conv[1][0];
			val += *(img +	 off	 + i  ) *  conv[1][1];
			val += *(img +   off     + i+1) *  conv[1][2];
			
			val += *(img + (off + w) + i-1) *  conv[2][0];
			val += *(img + (off + w) +  i ) *  conv[2][1];
			val += *(img + (off + w) + i+1) *  conv[2][2];
			
			if(flag == TRUE)
				*(pnEdge + off + i) = int(val);

			val = abs(val);
			if( val > 255) 
				*(pLap + off + i) = 255;
			else
				*(pLap + off + i) = (BYTE) val;
		}

	memcpy(img, pLap , (long)h * w);
	Dib();
	delete newimg;
	return true;
}

BOOL CImage::XDirectionGradient()
{
	int conv[3][3] = { 0,   0,   0,
					   1,   1,   1,
					  -1,  -1,  -1	};
	int i, j, w, h;
	BYTE g;
	long val, off;
	BYTE *img, *pLap;
	double cor = 0.7;

	h = Height();
	w = Width();
	img = Data();
	CSize size=CSize(w,h);
	CImage* newimg = new CImage(size);
	pLap = newimg->Data();

	memcpy(pLap, img, (long)h * w);

	for(j=1;j<h-1;j++)
	{
		off = (long)j*w;
		for(i=1;i<w-1;i++)
		{
			g = *(img + off + i);
			val  = 0;
			val += *(img + (off - w) + i-1) *  conv[0][0];
			val += *(img + (off - w) +  i ) *  conv[0][1];
			val += *(img + (off - w) + i+1) *  conv[0][2];

			val += *(img +   off     + i-1) *  conv[1][0];
			val +=                      g   *  conv[1][1];
			val += *(img +   off     + i+1) *  conv[1][2];

			val += *(img + (off + w) + i-1) *  conv[2][0];
			val += *(img + (off + w) +  i ) *  conv[2][1];
			val += *(img + (off + w) + i+1) *  conv[2][2];

			//if( (g - cor * val) > 255) 
			val = abs(val);
			if( val > 255) 
				*(pLap + off + i) = 255;
			else
			    //*(pLap + off + i) = (BYTE)(g - cor * val);
				*(pLap + off + i) = (BYTE) val;
		}
	}
	memcpy(img, pLap , (long)h * w);
	Dib();
	delete newimg;
	return true;
}

BOOL CImage::YDirectionGradient()
{
	int conv[3][3] = {  0,   0,  0,
					   -1,   2,  -1,
					    0,   0,  0};
	int i, j, w, h;
	BYTE g;
	long val, off;
	BYTE *img, *pLap;
	double cor = 0.7;

	h = Height();
	w = Width();
	img = Data();
	CSize size=CSize(w,h);
	CImage* newimg = new CImage(size);
	pLap = newimg->Data();

	memcpy(pLap, img, (long)h * w);

	for(j=1;j<h-1;j++)
	{
		off = (long)j*w;
		for(i=1;i<w-1;i++)
		{
			g = *(img + off + i);
			val  = 0;
			val += abs(*(img + (off - w) + i-1) *  conv[0][0]);
			/*val += *(img + (off - w) +  i ) *  conv[0][1];
			val += *(img + (off - w) + i+1) *  conv[0][2];

			val += *(img +   off     + i-1) *  conv[1][0];*/
			val +=                      g   *  conv[1][1];
			val += abs(*(img +   off     + i+1) *  conv[1][2]);

			/*val += *(img + (off + w) + i-1) *  conv[2][0];
			val += *(img + (off + w) +  i ) *  conv[2][1];
			val += *(img + (off + w) + i+1) *  conv[2][2];

			//if( (g - cor * val) > 255) 
			//val = abs(val);*/
			if( val > 255) 
				*(pLap + off + i) = 255;
			else
			    //*(pLap + off + i) = (BYTE)(g - cor * val);
				*(pLap + off + i) = (BYTE) val;
		}
	}
	memcpy(img, pLap , (long)h * w);
	Dib();
	delete newimg;
	return true;
}

void CImage::HistogramEqualize(BYTE * newgrey, BYTE q_step)
{
	int  i, j, grey, h, w;
	long Num[GREY],  counter, pixel_sum;
	float /*r[GREY],*/p[GREY],s[GREY]/*, newcdf[GREY]*/;
	unsigned char  *img;
	if(Bits() !=8)
	{
		AfxMessageBox("can't process non-8bits bitmap!");
		return;
	}
	img = Data();
	w = Width();
	h = Height();

	for(i=0;i<GREY;i++)	    
	{
		Num[i]	= 0;
		/*newcdf[i]	= 0.0;*/
		s[i]	= 0.0;
	}
 	pixel_sum = (long)h*w;
	for (counter=0;counter<pixel_sum;counter++)
	{
		grey=*(img+counter);
		Num[grey]++;
	}//calculate the Histogram
	for(i=0;i<GREY;i++)
	{
		p[i] = ((float)Num[i])/pixel_sum; 
		////r[i] = ((float)i)/(GREY-1);
	}
	/////////////////////////cdf//////////////////////////////
	
	for(i=0;i<GREY;i++)
		for(j=0;j<=i;j++)
			s[i] += p[j];
			
	for(i=0;i<GREY;i++)
		newgrey[i] = ((BYTE)( s[i] * (GREY-1)/q_step)) * q_step;	
/*
	//////////////////////////new cdf/////////////////////////
	double h2 = 2.0;
	double av;
	for(i=0;i<GREY;i++)
		for(j=0;j<GREY;j++)
		{
			av =  (double) (i-j) / h2;
			if(av<=1.0 && av>=-1.0)
				newcdf[i] += p[j] * (1 - abs(av) ) / h2;
		}
	//for(i=0;i<GREY;i++)
		//newcdf[i] /= GREY;
	for(i=0;i<GREY;i++)
		newgrey[i] = ((BYTE)( newcdf[i] * (GREY-1)/q_step)) * q_step;
*/
	Dib();
}

void CImage::ModifyHistogramTo(CImage& DestHistImg,BYTE q_step)
{
	if(Bits() !=8)
	{
		AfxMessageBox("can't process non-8bits bitmap!");
		return;
	}

	WORD i, j;
	DWORD counter;
	BYTE g;
	DWORD pixel_sum = (DWORD) Width() * Height();
	BYTE DestTransform[GREY];
	BYTE DestRevert[GREY];
	BYTE ScrTransform[GREY];

	DestHistImg.HistogramEqualize(DestTransform, q_step);
	/////下面过程实现多对一函数的反函数操作!
	WORD	tx1, tx2, x1, x2, tg;
	x2  = x1  = 0;
	tx2 = tx1 = DestTransform[0];
	for(i=0;i<GREY;i++)
	{
		tx2 = DestTransform[i];
		if((tx2 == tx1) && (i != GREY-1))
			continue;
		if(i == GREY-1)
		{	
			x2 = i;
			for(j=x1;j<=x2;j++)
				DestRevert[j] = BYTE(x1);
			break;
		}
		x2 = i;
		for(j=tx1;j<=tx2;j++)
		{
			tg = (BYTE) (x1 + (x2-x1) * (j - tx1) / (tx2 - tx1));
			DestRevert[j] =(BYTE) (((BYTE)(tg / q_step)) * q_step);
		}
		x1 = x2;
		tx1 = tx2;
	}  
	for(i=0;i<GREY;i++)
		DestRevert[DestTransform[i]] = BYTE(i);//no use but keep here!
	///////////////反函数结束了！/////////////////////////

	HistogramEqualize(ScrTransform, q_step);

	BYTE* img = Data();
	for (counter=0;counter<pixel_sum;counter++)
	{
		g = *(img+counter);
		*(img+counter) = DestRevert[ScrTransform[g]];
	}
	Dib();
}

void CImage::IntraIlluminationNormal(BYTE q_step)
{
	WORD w = Width();
	WORD h = Height();
	WORD w2 = w/2;
	CImage* pLeftPart  = new CImage(*this, 
					CPoint(0, 0), CSize(w2,h));
	CImage* pRightPart = new CImage(*this, 
					CPoint(w2, 0), CSize(w-w2,h));

	pLeftPart->ModifyHistogramTo(*pRightPart, q_step);
	CImage pTemp = *pRightPart;
	pTemp.ModifyHistogramTo(*pRightPart, q_step);


	BYTE* img = Data();
	BYTE* leftimg = pLeftPart->Data();
	WORD i, j;
	DWORD tt  = 0;
	DWORD tt2 = 0;
	for(j=0; j<h; j++)
	{
		for(i=0; i<w2; i++)
		{
			img[tt+i] = leftimg[tt2+i];
		}
		tt += w;
		tt2 += w2;
	}

	tt  = 0;
	tt2 = 0;
	BYTE* ptt = pTemp.Data();
	for(j=0; j<h; j++)
	{
		for(i=w2; i<w; i++)
			img[tt+i] = ptt[tt2+i-w2];
		tt += w;
		tt2 += w - w2;
	}

	ModifyHistogramTo(*pRightPart, q_step);
	delete pRightPart;
	delete pLeftPart;
}

BOOL	CImage::CannyEdgeDetect(double sigma, BOOL bBinary)
{
	return FALSE;
}

void	CImage::GammaTransform(double lfGamma)
{
/*
      f(x)=c * x ^ (1/gamma)  
*/
	ASSERT(lfGamma>=0);

	int		i;
	int		nImageSize = Width() * Height();
	double	c; //used to stretch the gray range to [0, 255]
	
	c = 255.0 / exp(log(255.0) / lfGamma);

	BYTE* pData = Data();
	for( i=0; i<nImageSize; i++, pData++)
	{
		if(*pData != 0)
			*pData = (BYTE) (c * exp(log( (double)(*pData ) )/lfGamma));
	}

	Dib();
}

void	CImage::LogProcess()
{
	int i;
	double maxg=0, ming=255;
	//double coef = 255.0/log(255.0);// * log(1.2);
	double coef = 255.0/log(255.0) * log(1.2);
	int nImageSize = Width() * Height();
	//GreyStratch();

	BYTE* pData = Data();
	for( i=0; i<nImageSize; i++, pData++)
	{
		if(*pData != 0)
			*pData = (BYTE) ((coef * (log((double) *pData) / log(1.2))));
		//*pData = (BYTE) (coef * (log((double) *pData)));
	}	
	Dib();
	GreyStratch();
}

void	CImage::GreyStratch()
{
	double maxg=0, ming=255;
	double coef;
	int nImageSize = Width() * Height();
	BYTE* pData = Data();
	for(int i=0; i<nImageSize; i++, pData++)
	{
		if(*pData>maxg)
			maxg = *pData;
		if(*pData<ming)
			ming = *pData;
	}
	coef = 254.0 / (maxg - ming);
	pData -= nImageSize;
	for( i=0; i<nImageSize; i++, pData++)
	{
		*pData = (BYTE) (coef * (*pData - ming) + 1.0);
	}
	Dib();
}

void	CImage::SegmentedGrayStretch(int nSegNum)
{
	int		i, n=0;
	int		w = Width();
	int		h = Height();
	long	l, s = w*h;
	
	BYTE*	pSegSrcPnt = new BYTE[nSegNum+1]; 
	BYTE*	pSegDstPnt = new BYTE[nSegNum+1]; 
	double  lfProportion = 1.0/nSegNum;

	BYTE* pImgData = Data();

	long	lGrayCntr[256];
	double	lfAccumCntr[256];

	memset(&lGrayCntr[0], 0, 256*sizeof(long));

	for(l=0; l<s; l++)
		lGrayCntr[*pImgData++]++;

	lfAccumCntr[0] = 1.0*lGrayCntr[0]/s;
	pSegSrcPnt[n++] = 0;
	for(i=1; i<256; i++)
	{
		lfAccumCntr[i] = 1.0*lGrayCntr[i]/s + lfAccumCntr[i-1];
		if(lfAccumCntr[i] >= lfProportion * n)
			pSegSrcPnt[n++] = i;
	}
	pSegSrcPnt[n] = 255;

	pSegDstPnt[0] = 0;
	for(i=1; i<nSegNum; i++)
		pSegDstPnt[i] = int(i*256.0/nSegNum);
	pSegDstPnt[nSegNum] = 255;

	BYTE	g;
	pImgData = Data();
	for(l=0; l<s; l++, pImgData++)
	{
		g = *pImgData;
		for(i=0; i<nSegNum; i++)
		{
			if(g >= pSegSrcPnt[i] && g <= pSegSrcPnt[i+1])
				break;
		}
		*pImgData = BYTE((g-pSegSrcPnt[i])*(1.0+pSegDstPnt[i+1]-pSegDstPnt[i])/(pSegSrcPnt[i+1]-pSegSrcPnt[i])+pSegDstPnt[i]);
	}

	Dib();
}

BYTE CImage::GetBit(BYTE* pImageData,DWORD dwWidth,DWORD dwHeight,int x,int y)
{   
	DWORD dwTempWid;
	DWORD dwTempHei;
	if(x <= -1)					dwTempWid = 0;
	else if(x >= (int)dwWidth)	dwTempWid = dwWidth-1;
		 else					dwTempWid = DWORD(x);
	if(y <= -1)					dwTempHei = 0;
	else if(y >= (int)dwHeight)	dwTempHei = dwHeight-1;
		 else					dwTempHei = DWORD(y);
	return *(pImageData+dwTempHei*dwWidth+dwTempWid);
}

BOOL	CImage::Kirsch(int*	pnEdge, BYTE *pucDir)
{
	int conv[8][3][3] = 
	{
		{ +5,  +5,  +5, -3, 0, -3,  -3,  -3,  -3},
		{ -3,  +5,  +5, -3, 0, +5,  -3,  -3,  -3},
		{ -3,  -3,  +5, -3, 0, +5,  -3,  -3,  +5},
		{ -3,  -3,  -3, -3, 0, +5,  -3,  +5,  +5},

		{ -3,  -3,  -3, -3, 0, -3,  +5,  +5,  +5},
		{ -3,  -3,  -3, +5, 0, -3,  +5,  +5,  -3},
		{ +5,  -3,  -3, +5, 0, -3,  +5,  -3,  -3},
		{ +5,  +5,  -3, +5, 0, -3,  -3,  -3,  -3},
	};

	int		i, j, k, w, h;
	long	val, off, l, ls, max;
	BYTE	*img;
	BYTE	ucDir;

	h		= Height();		
	w		= Width();
	ls		= long(w) * h;	
	img		= Data();
	CSize size	=	CSize(w,h);
	for(l=0; l<long(w)*h; l++)
	{
		*(pnEdge+l) = 0;
		*(pucDir+l) = 0;
	}

	max = 0;
	for(j=1,off=w;j<h-1;j++,off+=w)
		for(i=1;i<w-1;i++)
		{
			max = 0;	ucDir = 9;
			for(k=0; k<8; k++)
			{
				val  = 0;
				val += *(img + (off - w) + i-1) *  conv[k][0][0];
				val += *(img + (off - w) +  i ) *  conv[k][0][1];
				val += *(img + (off - w) + i+1) *  conv[k][0][2];
				
				val += *(img +   off     + i-1) *  conv[k][1][0];
				val += *(img +	 off	 + i  ) *  conv[k][1][1];
				val += *(img +   off     + i+1) *  conv[k][1][2];
				
				val += *(img + (off + w) + i-1) *  conv[k][2][0];
				val += *(img + (off + w) +  i ) *  conv[k][2][1];
				val += *(img + (off + w) + i+1) *  conv[k][2][2];
				val /= 9/*15*/;
				if( abs(val) >= abs(max) && val !=0 )
				{
					max   = val;
					ucDir = k;
				}
			}
			*(pnEdge + off + i) = int(max);
			*(pucDir + off + i) = ucDir;
		}
	Dib();

	return true;
}
BOOL	CImage::Prewitt(int*	pnEdge, BYTE *pucDir)
{
	int conv[4][3][3] = 
	{
		{  1,   1,  1,   0, 0,  0,  -1,  -1,  -1},
		{ -1,   0,  1,  -1, 0,  1,  -1,   0,   1},
		{ -1,  -1, -1,   0, 0,  0,   1,   1,   1},
		{  1,  0,  -1,   1, 0, -1,   1,   0,  -1}
	};

	int		i, j, k, w, h;
	long	val, off, l, ls, max;
	BYTE	*img;
	BYTE	ucDir;

	h		= Height();		
	w		= Width();
	ls		= long(w) * h;	
	img		= Data();
	CSize size	=	CSize(w,h);
	for(l=0; l<long(w)*h; l++)
	{
		*(pnEdge+l) = 0;
		*(pucDir+l) = 5;
	}

	max = 0;
	for(j=1,off=w;j<h-1;j++,off+=w)
		for(i=1;i<w-1;i++)
		{
			max = 0;	ucDir = 5;
			for(k=0; k<4; k++)
			{
				val  = 0;
				val += *(img + (off - w) + i-1) *  conv[k][0][0];
				val += *(img + (off - w) +  i ) *  conv[k][0][1];
				val += *(img + (off - w) + i+1) *  conv[k][0][2];
				
				val += *(img +   off     + i-1) *  conv[k][1][0];
				val += *(img +	 off	 + i  ) *  conv[k][1][1];
				val += *(img +   off     + i+1) *  conv[k][1][2];
				
				val += *(img + (off + w) + i-1) *  conv[k][2][0];
				val += *(img + (off + w) +  i ) *  conv[k][2][1];
				val += *(img + (off + w) + i+1) *  conv[k][2][2];
				val /= 3;
				if( abs(val) >= abs(max) && val !=0 )
				{
					max   = val;
					ucDir = k;
				}
			}
			*(pnEdge + off + i) = int(max);
			*(pucDir + off + i) = ucDir;
		}
	Dib();

	return true;
}

#define Noise()      (2.0*((double) rand())/((double)RAND_MAX)-1)

void	CImage::AddNoise(double	nNoiseMag)//经过试验我认为nNoiseMag是指噪音的强度
                                          //其适用于彩色图片和黑白的图片
{
	int			newg;
	long		l, s;

	BYTE		*pData = Data();//Data()函数也是CImage里定义的

	srand( 0 );//Sets a random starting point
	s = (long)Width() * Height() * Bits() / 8;//Width()和Heigh()是CImage类里的函数 s=整个bmp文件所占的字节个数
	for(l=0; l<s; l++)
	{
		if(pData[l] == 0)
			continue;
		newg = int(pData[l]+ int(nNoiseMag * Noise()));//??
		if(newg>=0)
			pData[l] = newg>255 ? BYTE(255) : BYTE(newg);//Constructs a Byte object initialized to the specified byte value.
		else
			pData[l] = 0;
	}

	Dib();
}

void	CImage::SecondOrderEdgeDetect()
{
	double	dif;
	BYTE	*t, *p;
	WORD	i, j, w, h;
	long	imgSize;
	
	w = Width();
	h = Height();
	imgSize = (long)(w * h);
	BYTE*	pEdgeData = new BYTE[imgSize];
	BYTE*	pData	=	Data();

	for(i=1, p=pEdgeData+w, t = pData + w; i < h - 1; i++, t+=w, p+=w) {
		for(j= 1; j < w - 1; j++)
		{
			dif = fabs( (double)(((int)*(t + j)) * 2 - (*(t + j -1)) - (*(t + j +1))) );
			dif += fabs( (double)(((int)*(t + j)) * 2 - (*(t + j - w)) - (*(t + j +w))) );
			if (dif > 100)
				*(p+j) = 255;
			else
				*(p+j) = 0;
		}
	}
	memcpy(pData, pEdgeData, imgSize);

	Dib();

}

void CImage::ImageSmoothing()
{
	if(Bits() == 8)	{
		Smoothing();
		return;
	}

	if(Bits() != 24)
		return;

	CImage	*pRImg = new CImage(*this);
	CImage	*pGImg = new CImage(*this);
	CImage	*pBImg = new CImage(*this);
	BYTE		*pRData = pRImg->Data();
	BYTE		*pGData = pGImg->Data();
	BYTE		*pBData = pBImg->Data();
	BYTE		*pColorData = Data();

	long			size, l;
	size = long(Width() * Height());
	for(l=0; l<size; l++)
	{
		*pBData++ = *pColorData++;
		*pGData++ = *pColorData++;
		*pRData++ = *pColorData++;
	}

	pRImg->Smoothing();
	pGImg->Smoothing();
	pBImg->Smoothing();

	pRData = pRImg->Data();
	pGData = pGImg->Data();
	pBData = pBImg->Data();
	pColorData = Data();

	for(l=0; l<size; l++)
	{
		*pColorData++ = *pBData++;
		*pColorData++ = *pGData++;
		*pColorData++ = *pRData++;
	}

	Dib();
	delete	pBImg;
	delete	pGImg;
	delete	pRImg;

}

void CImage::ImageMidValueFilter()
{
	if(Bits() == 8)	{
		MidFilter();
		return;
	}

	if(Bits() != 24)
		return;

	CImage	*pRImg = new CImage(*this);
	CImage	*pGImg = new CImage(*this);
	CImage	*pBImg = new CImage(*this);
	BYTE		*pRData = pRImg->Data();
	BYTE		*pGData = pGImg->Data();
	BYTE		*pBData = pBImg->Data();
	BYTE		*pColorData = Data();

	long			size, l;
	size = long(Width() * Height());
	for(l=0; l<size; l++)
	{
		*pBData++ = *pColorData++;
		*pGData++ = *pColorData++;
		*pRData++ = *pColorData++;
	}

	pRImg->MidFilter();
	pGImg->MidFilter();
	pBImg->MidFilter();

	pRData = pRImg->Data();
	pGData = pGImg->Data();
	pBData = pBImg->Data();
	pColorData = Data();

	for(l=0; l<size; l++)
	{
		*pColorData++ = *pBData++;
		*pColorData++ = *pGData++;
		*pColorData++ = *pRData++;
	}

	Dib();
	delete	pBImg;
	delete	pGImg;
	delete	pRImg;

}

BOOL CImage::ColorImageCannyEdgeDetector(double sigma, BOOL bBinary)//？？canny边缘提取平时做的是黑白图像的canny边缘提取，这里是不是说是彩色图片不变成黑白图片，直接进行canny算子提取 
{                                                                   //因为CannyEdgeDetect(sigma,bBinary)函数是空的，所以这里执行看不出来效果，暂且没看
	if(Bits() == 8)
		return CannyEdgeDetect();

	if(Bits() != 24)
		return FALSE;

	int		w = Width();
	int		h = Height();

	CImage	*pBinaryImg = new CImage(CSize(w, h), 256, 8);
	CImage	*pRImg = new CImage(CSize(w, h), 256, 8);
	CImage	*pGImg = new CImage(CSize(w, h), 256, 8);
	CImage	*pBImg = new CImage(CSize(w, h), 256, 8);
	BYTE		*pRData = pRImg->Data();
	BYTE		*pGData = pGImg->Data();
	BYTE		*pBData = pBImg->Data();
	BYTE		*pColorData = Data();
	BYTE		*pBinaryData= pBinaryImg -> Data();

	long			size, l;
	size = long(Width() * Height());
	for(l=0; l<size; l++)
	{
		*pBData++ = *pColorData++;
		*pGData++ = *pColorData++;
		*pRData++ = *pColorData++;
	}

	pRImg->CannyEdgeDetect(sigma, bBinary);
	pGImg->CannyEdgeDetect(sigma, bBinary);
	pBImg->CannyEdgeDetect(sigma, bBinary);

	pRData = pRImg->Data();
	pGData = pGImg->Data();
	pBData = pBImg->Data();

	int		sum;
	for(l=0; l<size; l++)
	{
		sum = (*pBData++) + (*pGData++) + (*pRData++);
		if(bBinary)
		{
			if(sum != 0)
				*pBinaryData++ = 255;
			else
				*pBinaryData++ = 0;
		}
		else
		{
			*pBinaryData++ = BYTE( sum / 3 );
		}
	}

	Dib();
	pBinaryImg -> Dib();
	*this = *pBinaryImg;

	delete	pBImg;
	delete	pGImg;
	delete	pRImg;
	delete	pBinaryImg;

	return  TRUE;
}


BOOL CImage::ColorFaiImgCannyEdge(double sigma, BOOL bBinary)
{
	return  FALSE;
}

BOOL CImage::Convert2FaiImage()
{
	if(Bits() != 24)
		return FALSE;

	int		w = Width();
	int		h = Height();

	CImage	*pFaiImg = new CImage(CSize(w, h), 256, 8);

	BYTE		*pFaiData = pFaiImg->Data();
	BYTE		*pColorData = Data();

	long		size, l;
	int			fai;
	BYTE		R, G, B;
	size = long(Width() * Height());
	for(l=0; l<size; l++)
	{
		B = *pColorData++;
		G = *pColorData++;
		R = *pColorData++;
		RGB2Fai(R, G, B, fai);
		*pFaiData++ = BYTE(fai*255.0/360.0);
	}

	pFaiData = pFaiImg->Dib();
	*this = *pFaiImg;
	delete	pFaiImg;
	return TRUE;
}


BOOL CImage::Convert2I_Image()
{
	if(Bits() != 24)
		return FALSE;

	int		w = Width();
	int		h = Height();

	CImage	*pI_Img = new CImage(CSize(w, h), 256, 8);

	BYTE		*pI_Data = pI_Img->Data();
	BYTE		*pColorData = Data();

	long		size, l;
	double		I, Q;
	BYTE		R, G, B, Y;
	size = long(Width() * Height());
	for(l=0; l<size; l++)
	{
		B = *pColorData++;
		G = *pColorData++;
		R = *pColorData++;
		RGB2YIQ(R, G, B, Y, I, Q);
		*pI_Data++ = BYTE( I );
	}

	pI_Data = pI_Img->Dib();
	*this = *pI_Img;
	delete pI_Img;

	return TRUE;
}

BOOL CImage::GetRImage(CImage * pRImg)
{
	if(Bits() != 24)
		return FALSE;

	int		w = Width();
	int		h = Height();

	BYTE		*pRData = pRImg->Data();
	BYTE		*pColorData = Data();

	long		size, l;
	size = long(Width() * Height() * 3);

	for(l=0; l<size; l+=3)
		*pRData++ = *(pColorData+l+2);

	Dib();
	pRImg->Dib();

	return TRUE;
}

BOOL CImage::GetGImage(CImage * pGImg)
{
	if(Bits() != 24)
		return FALSE;

	int		w = Width();
	int		h = Height();

	BYTE		*pGData = pGImg->Data();
	BYTE		*pColorData = Data();

	long		size, l;
	size = long(Width() * Height() * 3);

	for(l=0; l<size; l+=3)
		*pGData++ = *(pColorData+l+1);

	Dib();
	pGImg->Dib();

	return TRUE;
}

BOOL CImage::GetBImage(CImage * pBImg)
{
	if(Bits() != 24)
		return FALSE;

	int		w = Width();
	int		h = Height();

	BYTE		*pBData = pBImg->Data();
	BYTE		*pColorData = Data();

	long		size, l;
	size = long(Width() * Height() * 3);

	for(l=0; l<size; l+=3)
		*pBData++ = *(pColorData+l);

	Dib();
	pBImg->Dib();
	
	return TRUE;
}

BOOL CImage::Mask3x3ConvFilter(int pMaskMatrix[3][3], int nNormalFactor)
{
	int		i, j, w, h;
	long	val, off;
	BYTE	*img, *pNewImgData;
	BOOL	flag = FALSE;

	h	= Height();
	w	= Width();
	CImage* newimg = new CImage(CSize(w,h));
	
	img = Data();
	pNewImgData = newimg->Data();
		
	for(j=1,off=w; j<h-1; j++,off+=w)
		for(i=1; i<w-1; i++)
		{
			val  = 0;
			val += *(img + (off - w) + i-1) *  pMaskMatrix[0][0];
			val += *(img + (off - w) +  i ) *  pMaskMatrix[0][1];
			val += *(img + (off - w) + i+1) *  pMaskMatrix[0][2];
			
			val += *(img +   off     + i-1) *  pMaskMatrix[1][0];
			val += *(img +	 off	 + i  ) *  pMaskMatrix[1][1];
			val += *(img +   off     + i+1) *  pMaskMatrix[1][2];
			
			val += *(img + (off + w) + i-1) *  pMaskMatrix[2][0];
			val += *(img + (off + w) +  i ) *  pMaskMatrix[2][1];
			val += *(img + (off + w) + i+1) *  pMaskMatrix[2][2];
			
			val = abs(val/nNormalFactor);
			if( val > 255) 
				*(pNewImgData + off + i) = 255;
			else
				*(pNewImgData + off + i) = (BYTE) val;
		}
	memcpy(img, pNewImgData, (long)h * w);

	Dib();
	delete newimg;
	return TRUE;

}

BOOL CImage::Mask5x5ConvFilter(int pMaskMatrix[5][5], int nNormalFactor)
{
	int		i, j, w, h;
	long	val, off;
	BYTE	*img, *pNewImgData;
	BOOL	flag = FALSE;

	h	= Height();
	w	= Width();
	CImage* newimg = new CImage(CSize(w,h));
	
	img = Data();
	pNewImgData = newimg->Data();
		
	for(j=2,off=w; j<h-2; j++,off+=w)
		for(i=2; i<w-2; i++)
		{
			val  = 0;
			val += *(img + (off-2*w) + i-2) *  pMaskMatrix[0][0];
			val += *(img + (off-2*w) + i-1) *  pMaskMatrix[0][1];
			val += *(img + (off-2*w) +  i ) *  pMaskMatrix[0][2];
			val += *(img + (off-2*w) + i+1) *  pMaskMatrix[0][3];
			val += *(img + (off-2*w) + i+2) *  pMaskMatrix[0][4];

			val += *(img + (off - w) + i-2) *  pMaskMatrix[1][0];
			val += *(img + (off - w) + i-1) *  pMaskMatrix[1][1];
			val += *(img + (off - w) +  i ) *  pMaskMatrix[1][2];
			val += *(img + (off - w) + i+1) *  pMaskMatrix[1][3];
			val += *(img + (off - w) + i+2) *  pMaskMatrix[1][4];
			
			val += *(img +   off     + i-2) *  pMaskMatrix[2][0];
			val += *(img +   off     + i-1) *  pMaskMatrix[2][1];
			val += *(img +	 off	 + i  ) *  pMaskMatrix[2][2];
			val += *(img +   off     + i+1) *  pMaskMatrix[2][3];
			val += *(img +   off     + i+2) *  pMaskMatrix[2][4];
			
			val += *(img + (off + w) + i-2) *  pMaskMatrix[3][0];
			val += *(img + (off + w) + i-1) *  pMaskMatrix[3][1];
			val += *(img + (off + w) +  i ) *  pMaskMatrix[3][2];
			val += *(img + (off + w) + i+1) *  pMaskMatrix[3][3];
			val += *(img + (off + w) + i+2) *  pMaskMatrix[3][4];

			val += *(img + (off+2*w) + i-2) *  pMaskMatrix[4][0];
			val += *(img + (off+2*w) + i-1) *  pMaskMatrix[4][1];
			val += *(img + (off+2*w) +  i ) *  pMaskMatrix[4][2];
			val += *(img + (off+2*w) + i+1) *  pMaskMatrix[4][3];
			val += *(img + (off+2*w) + i+2) *  pMaskMatrix[4][4];
			
			val = abs(val/nNormalFactor);
			if( val > 255) 
				*(pNewImgData + off + i) = 255;
			else
				*(pNewImgData + off + i) = (BYTE) val;
		}
	memcpy(img, pNewImgData, (long)h * w);

	Dib();
	delete newimg;
	return TRUE;
}
/*
 * return paras:
 * pfr:  f(u,v) 实部
 * pfi:  f(u,v) 虚部
 * pfm:  模
 * pfa:  幅角
*/
BOOL	CImage::FFT(double* pfr, double* pfi, double* pfm, double* pfa)
{
	return FALSE;
}

BOOL CImage::MirrorImage()
{
	BYTE			*data;
	unsigned long	row, col, w, h, size, t;
	BYTE			*bakData;

	if(Bits() != 8)
	{
		AfxMessageBox("只能处理graylevel图像");
		return FALSE;
	}

	h = Height();
	w = Width();
	size = w * h;

	bakData = new BYTE[size];
	data = Data();
	memcpy(bakData, data, size);//把原图像拷贝到了一个新的缓存里（bakData),而原来存放图像数据的m_pData用来存放倒过来的图像值。。一行一行的倒过来
	
	for(t=0,row=0; row<h; row++,t+=w)
	{
		for(col=0;col<w;col++)
		{
			*(data+t+col) = *(bakData+t+w-col-1);//
		}
	}

	Dib();
	delete bakData;
	return TRUE;
}

BOOL CImage::CenterRotateInImagePlane(double angle, BOOL bClockWise)//从实现来看，也是对彩色图片不好使，要先变换成黑白的照片，第一个参数是旋转的角度，第二个参数是是顺时针还是逆时针
                                                                    //程序代码具体的算法暂且没太看
{
	double	radianAngle;
	double	oldx, oldy;
	double  cofX, cofY, FTmp;
	double  SinCita, CosCita;
	int		i, j, x1;
	long	w, h, ImageSize;

	w = Width();
	h = Height();
	ImageSize = (long)w*h;

	CPoint fixpos = CPoint(w/2, h/2);
	if(bClockWise)
		radianAngle = angle*3.1415926/180.0;//换算成弧度，顺时针
	else
		radianAngle = -angle*3.1415926/180.0;//换算成弧度，逆时针 
	SinCita = sin(radianAngle);
	CosCita = cos(radianAngle);

	CImage* pTempImg = new CImage;
	*pTempImg = *this;

	BYTE* pOriData = Data();//？？
	BYTE* pNewData = pTempImg->Data();//？？本质没有区别吧，就一个Data()函数

	double t1, t2;
	long t3 = 0;
	for(j=0; j<h; j++)
	{
		t1 = SinCita * (j - fixpos.y);
		t2 = CosCita * (j - fixpos.y);
		t3 = j * w;
		for(i=0; i<w; i++)
		{
			oldx = fixpos.x +  (CosCita * (i - fixpos.x) + t1);
			oldy = fixpos.y +  (t2 - SinCita * (i - fixpos.x));
			if(oldx < w-1 && oldy < h-1 && oldx >= 0 && oldy >= 0)
			{	
				cofX = oldx - int(oldx);
				cofY = oldy - int(oldy);
				x1 = ((int)oldy) * w + (int)oldx;
				FTmp = (1-cofX) * (double)pOriData[x1] + cofX * (double)pOriData[x1 + 1];
				FTmp = (1-cofY) * FTmp + cofY * ((1-cofX) * (double)pOriData[x1 + w] + cofX * (double)pOriData[x1 + w + 1]);
				pNewData[j * w + i] = int (FTmp + 0.5);
			}
			else 
				pNewData[j * w + i] = 0;
		}
	}

	pTempImg->Dib();
	*this = *pTempImg;
	Dib();
	delete pTempImg;
	return true;

}

BOOL CImage::ImageEnhance()
{
	CreateGreyDib();

	int		i, j, w, h;
	int		nextHval, nextVval, prevHval, prevVval;
	int		absNextHval, absNextVval, absPrevHval, absPrevVval;
	int		maxNextVal, maxPrevVal;
	long	val, newval, off;
	BYTE	*img, *pNewImgData;
	BOOL	flag = FALSE;

	h	= Height();
	w	= Width();
	if(Bits() != 8)
	{
		return FALSE;
	}

	CImage* newimg = new CImage(CSize(w,h));
	
	img = Data();
	pNewImgData = newimg->Data();
	//border pixels
	memcpy(pNewImgData, img, w);
	memcpy(pNewImgData+w*h-w, img+w*h-w, w);
	for(j=1,off=w; j<h-1; j++,off+=w)
	{
		*(pNewImgData+off) = *(img+off);
		*(pNewImgData+off-1) = *(img+off-1);
	}
	//inner pixels	
	for(j=1,off=w; j<h-1; j++,off+=w)
	{
		for(i=1; i<w-1; i++)
		{
			val  = *(img + off + i);
			
			nextHval = val - *(img + off + i + 1); 
			nextVval = val - *(img + off + w + i);
			prevHval = val - *(img + off + i - 1); 
			prevVval = val - *(img + off - w + i); 

			absNextHval = abs(nextHval);
			absNextVval = abs(nextVval);
			maxNextVal = (absNextHval>absNextVval)?absNextHval:absNextVval;

			absPrevHval = abs(prevHval);
			absPrevVval = abs(prevVval);
			maxPrevVal = (absPrevHval>absPrevVval)?absPrevHval:absPrevVval;

			if(maxNextVal > maxPrevVal)
			{
				if(maxNextVal >= 10)
					newval = val + ((absNextHval>absNextVval)?nextHval:nextVval);
				else
					newval = val;
			}
			else
			{
				if(maxPrevVal >= 10)
					newval = val + ((absPrevHval>absPrevVval)?prevHval:prevVval);
				else
					newval = val;
			}

			newval = (newval>255)? 255 : newval;
			newval = (newval<0) ? 0 : newval;
			
			*(pNewImgData + off + i) = BYTE(newval);
		}
	}

	memcpy(img, pNewImgData, (long)h * w);

	Dib();
	delete newimg;
	return TRUE;

}

BOOL CImage::IntensityAverage128()
{
	CreateGreyDib();

	BYTE	g, m, min=255, max=0;
	int		h, w;
	double	sum=0;
	long counter, pixel_sum;
 	if(Bits() !=8)
	{
		AfxMessageBox("can't process non-8bits bitmap!");
		return false;
	}

	BYTE* img = Data();
	w = Width();
	h = Height();
 	pixel_sum = (long)h*w;

	for (counter=0;counter<pixel_sum;counter++)
	{
		g = *img++;
		sum += g;
		min=(g<min)?g:min;
		max=(g>max)?g:max;
	}
	m = BYTE(sum/pixel_sum);

	img = Data();
	if(m==min || m==max)
	{
		for (counter=0;counter<pixel_sum;counter++)
			*img++ = 128;
		Dib();
		return true;
	}

	for (counter=0;counter<pixel_sum;counter++)
	{
		g = *img;
		if(g<=m)
			*img++ = BYTE(128*(g-min)/(m-min));
		else
			*img++ = BYTE(127*(g-m)/(max-m)+128);
	}

	Dib();
	return true;
}

BOOL CImage::ContrastEnhance(int factor)
{
	CreateGreyDib();

	BYTE	g, min=255, max=0;
	int		h, w, gr;
	double	sum=0;
	long	counter, pixel_sum;

 	if(Bits() !=8)
	{
		AfxMessageBox("目前版本只能支持灰度图像！");
		return false;
	}

	if(factor<0 || factor> 128)
	{
		AfxMessageBox("对比度增强范围为0-128！");
		return false;
	}

	w = Width();	h = Height();
 	pixel_sum = (long)h*w;

	BYTE* img = Data();
	for (counter=0;counter<pixel_sum;counter++)
	{
		g = *img++;			sum += g;
		min=(g<min)?g:min;	max=(g>max)?g:max;
	}
	//m = BYTE(sum/pixel_sum);
	gr = max-min;
	if(gr <= 0)
		return FALSE;

	BYTE fctr = BYTE((max-min)*factor/256);
	BYTE minGray = min+fctr;
	BYTE maxGray = max-fctr;

	img = Data();
	gr = maxGray-minGray;
	if(gr <= 0 )
		return FALSE;
	for (counter=0;counter<pixel_sum;counter++)
	{
		g = *img;
		if(g<minGray)
			*img++ = BYTE(0);
		else
		{
			if(g>maxGray)
			{
				*img++ = BYTE(255);
			}
			else
			{
				*img++ = BYTE(255*(g-minGray)/gr);
			}
		}
	}

	Dib();
	return TRUE;
}

BOOL CImage::GreyTranslation(int GreyBase)
{
	CreateGreyDib();

	BYTE	g;
	int		h, w;
	long	counter, pixel_sum;

 	if(Bits() !=8)
	{
		AfxMessageBox("目前版本只能支持灰度图像！");
		return false;
	}

	if(GreyBase<-256 || GreyBase> 256)
	{
		AfxMessageBox("灰度平移范围为-256到256！");
		return false;
	}

	w = Width();	h = Height();
 	pixel_sum = (long)h*w;

	BYTE* img = Data();
	int	ng;
	for (counter=0;counter<pixel_sum;counter++)
	{
		g = *img;
		ng = g+GreyBase;
		ng = ng<0?0:ng;
		ng = ng>255?255:ng;
		*img++ = BYTE(ng);
	}

	Dib();
	return TRUE;
}

void	CImage::Resize(int w, int h)
{
	if(w == Width() && h == Height())
		return;

	CImage* pNewImage = new CImage(*this, CSize(w, h));//把已有的图像变成自己想要的大小，图像不是空的，因为参数里面有原图像的地址
	*this = *pNewImage;
	delete pNewImage;
}

double CImage::DistanceBetween(CImage* img)
{
	long	l, s;
	double dis = 0.0;
	CImage*	p1 = new CImage(*this);
	CImage* p2 = new CImage(*img);

	p1->CreateGreyDib();
	p2->CreateDIBPalette();
	BYTE*	pSrcData = p1->Data();
	BYTE*	pDstData = p2->Data();
	int		w = Width();
	int		h = Height();
	ASSERT(w==img->Width() && h == img->Height());

	s = w*h;
	double* pf1 = new double[s];
	double* pf2 = new double[s];
	for(l=0; l<s; l++)
	{
		*pf1++ = *pSrcData++;
		*pf2++ = *pDstData++;
	}
	pf1 -= s;
	pf2 -= s;
	NormalVector2ZeroMeanUnitVar(pf1, s);
	NormalVector2ZeroMeanUnitVar(pf2, s);

	dis = VectorSimilarity(pf1, pf2, s);

	//dis = sqrt(dis);

	delete p1;
	delete p2;
	delete pf1;
	delete pf2;
	return dis;
}

BOOL	CImage::VisualizeDataAsImage(double* pData, int cols, int rows)
{
	int		r, c;
	BYTE*	pImgData;
	CImage* pImage = new CImage(CSize(cols, rows), 256, 8);
	double	lfMin, lfMax;
	lfMin = lfMax = *pData;
	for(r=0; r<rows; r++)
	{
		for(c=0; c<cols; c++)
		{
			if(*pData > lfMax)	
				lfMax = *pData;
			if(*pData < lfMin)	
				lfMin = *pData;
			pData++;
		}
	}
	pData -= rows*cols;
	pImgData = pImage->Data();
	for(r=0; r<rows; r++)
	{
		for(c=0; c<cols; c++)
		{
			*pImgData++ = BYTE((*pData++-lfMin)*255/(lfMax-lfMin));
		}
	}
	pData -= rows*cols;
	*this = *pImage;
	delete pImage;
	return TRUE;
}

void CImage::DisplayImage(CWnd *pWnd, double &dScale)
{
	CDC *pIMAGEDC = pWnd->GetDC();
	pIMAGEDC->SetBkMode(TRANSPARENT);

	CRect rect;
	pWnd->GetClientRect(&rect);
	int nRectWidth = rect.Width();
	int nRectHeight = rect.Height();
	double dRectScale = (double)(nRectHeight) / nRectWidth;

	int nImgWidth = this->Width();
	int nImgHeight = this->Height();
	double dImgScale = (double)(nImgHeight) / nImgWidth;

	int nDisHeight, nDisWidth;
	if(dImgScale > dRectScale)
	{
		nDisHeight = nRectHeight;
		nDisWidth = (int)(nDisHeight / dImgScale);
		dScale = (double)nRectHeight / nImgHeight;
	}
	else
	{
		nDisWidth = nRectWidth;
		nDisHeight = (int) (nDisWidth * dImgScale);
		dScale = (double)nRectWidth / nImgWidth;
	}

	
	//Stretch(pIMAGEDC, rect.CenterPoint() - CPoint(nDisWidth / 2, nDisHeight / 2),
	//	CSize(nDisWidth, nDisHeight) );

	pWnd->ReleaseDC(pIMAGEDC);
}

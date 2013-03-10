
#include "StdAfx.h"
#include <assert.h>

#include <cv.h>
#include <highgui.h>

#include "IPLImage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CIPLImage::CIPLImage()
{
	ZeroMemory(this, sizeof(IplImage));	
}

CIPLImage::CIPLImage(CVisImageBase &imageBase)
{
	CreateFromVisImage(imageBase);
}

CIPLImage& CIPLImage::operator=(CVisImageBase &imageBase)
{
	CreateFromVisImage(imageBase);
	return *this;
}

void CIPLImage::CopyToIplImage( IplImage** ppiplImage )
{
	if ( *ppiplImage==this ) return;
	if ( *ppiplImage!=NULL ) cvReleaseImage(ppiplImage);

	*ppiplImage = cvCreateImage(cvSize(width, height), depth, nChannels);

	for (int y=0; y<height; y++)
	{
		for (int x=0; x<width; x++)
		{
			uchar* temp_ptr = &((uchar*)(imageData + widthStep*y))[x*nChannels];
			uchar* des_ptr = &((uchar*)((*ppiplImage)->imageData + widthStep*y))[x*nChannels];

			for (int i=0; i<nChannels; i++)
			{
				des_ptr[i] = temp_ptr[i];
			}
		}
	}
}

void CIPLImage::CopyToVisImage( CVisImageBase &imageBase )
{
	EVisPixFmt pixfmt = imageBase.PixFmt();
	
	int x, y;
	switch( pixfmt & evispixfmtStructureMask)
	{
	case evispixfmtRGBA:
		imageBase.Deallocate();
		imageBase.Allocate(width, height);
		for (y=0; y<height; y++)
		{
			for (x=0; x<width; x++)
			{
				uchar* temp_ptr = &((uchar*)(imageData + widthStep*y))[x*4];
//				*imageBase.PbPixel(x, y, 0) = temp_ptr[0];
//				*imageBase.PbPixel(x, y, 1) = temp_ptr[1];
				*imageBase.PbPixel(x, y, 2) = temp_ptr[2];
//				*imageBase.PbPixel(x, y, 3) = temp_ptr[3];
			}
		}
		break;
	case evispixfmtGray:
		imageBase.Deallocate();
		imageBase.Allocate(width, height);
		for (y=0; y<height; y++)
		{
			for (x=0; x<width; x++)
			{
				*imageBase.PbPixel(x, y) = ((uchar*)(imageData + widthStep*y))[x];				
			}
		}
		break;
	default: return;
	}
}

void CIPLImage::CreateFromVisImage( CVisImageBase &imageBase)
{
	// The date memory block of this image should not be empty
	assert( imageBase.IsValid());	

	// Fill in the associated fields of the IplImage struct.
	nSize		= sizeof(IplImage);
	ID			= 0;
	dataOrder	= IPL_DATA_ORDER_PIXEL; // VisSDK only supports pixel data order.
	origin		= IPL_ORIGIN_TL; // VisSDK only supports the top-left origin.

	int imopts = imageBase.Options();
	switch (imopts & evisimoptAlignMemMask)
	{
	case evisimoptAlignMem4Byte:
		align = IPL_ALIGN_DWORD;
		break;
	case evisimoptAlignMem8Byte:
		align = IPL_ALIGN_QWORD;
		break;
	default:
		// the IplImage can only support 4 or 8 bytes alignment.
		assert(false);
	}

	width = imageBase.Width();
	height = imageBase.Height();

	roi			= NULL; // can be set by other function later
	maskROI		= NULL; // have not been used at this version
	imageId		= NULL; // have not been used at this version
	tileInfo	= NULL; // have not been used at this version

	const CVisMemBlock &mem = imageBase.MemBlock();

	imageSize = mem.CbData();
	imageData = reinterpret_cast<char*>( mem.PbData());
	// the image data should not be empty
	assert( imageData != NULL);

	widthStep = imageBase.CbRow();
	ZeroMemory(BorderMode, sizeof(int)*4); // can be set by other function later
	ZeroMemory(BorderConst, sizeof(int)*4); // can be set by other function later
	imageDataOrigin = imageData;
	
	EVisPixFmt pixfmt = imageBase.PixFmt();

	switch( pixfmt & evispixfmtStructureMask)
	{
	case evispixfmtGray:
		nChannels = 1;
		alphaChannel = 0;
		CopyMemory( colorModel, "Gray", 4);
		CopyMemory( channelSeq, "GRAY", 4);
		break;
	case evispixfmtRGBA:
		nChannels = 4;
		alphaChannel = 4;
		CopyMemory( colorModel, "RGBA", 4);
		CopyMemory( channelSeq, "BGRA", 4);
		break;
	case evispixfmtRGB:
		// not supported by VisSDK yet
		assert(false);
		break;
	case evispixfmtYUVA:
		// not supported by IPL yet
		assert(false);
		break;
	case evispixfmtYUV:
		// not supprted by VisSDK yet
		assert(false);
		break;
	case evispixfmtImEncoding:
		// not supported by IPL yet
		assert(false);
		break;
	case evispixfmtOther:
		// not supported by IPL yet
		assert(false);
		break;
	default:
		// should not go here
		assert(false);
		break;
	}

	switch( pixfmt)
	{
		// Common Grayscale Pixel Types
	case evispixfmtGrayChar:
		depth = IPL_DEPTH_8S;
		break;
	case evispixfmtGrayShort:
		depth = IPL_DEPTH_16S;
		break;
	case evispixfmtGrayInt:
		depth = IPL_DEPTH_32S;
		break;
	case evispixfmtGrayLong:
		depth = IPL_DEPTH_32S;
		break;
	case evispixfmtGrayByte:
		depth = IPL_DEPTH_8U;
		break;
	case evispixfmtGrayUShort:
		depth = IPL_DEPTH_16U;
		break;
	case evispixfmtGrayUInt:
		// IPL do not support unsigned int
		assert(false);
		break;
	case evispixfmtGrayULong:
		// IPL do not support unsigned long
		assert(false);
		break;
	case evispixfmtGrayFloat:
		depth = IPL_DEPTH_32F;
		break;
	case evispixfmtGrayDouble:
		// IPL do not support 'double'.
		assert(false);
		break;

		// Common Color (RGBA) Pixel Types
	case evispixfmtRGBAChar:
		depth = IPL_DEPTH_8S;
		break;
	case evispixfmtRGBAShort:
		depth = IPL_DEPTH_16S;
		break;
	case evispixfmtRGBAInt:
		depth = IPL_DEPTH_32S;
		break;
	case evispixfmtRGBALong:
		depth = IPL_DEPTH_32S;
		break;
	case evispixfmtRGBAByte:
		depth = IPL_DEPTH_8U;
		break;
	case evispixfmtRGBAUShort:
		depth = IPL_DEPTH_16U;
		break;
	case evispixfmtRGBAUInt:
		// IPL do not support unsigned int
		assert(false);
		break;
	case evispixfmtRGBAULong:
		// IPL do not support unsigned long
		assert(false);
		break;
	case evispixfmtRGBAFloat:
		depth = IPL_DEPTH_32F;
		break;
	case evispixfmtRGBADouble:
		// IPL do not support 'double'.
		assert(false);
		break;

		// Common Color (YUVA) Pixel Types
	case evispixfmtYUVAChar:
	case evispixfmtYUVAShort:
	case evispixfmtYUVAInt:
	case evispixfmtYUVALong:
	case evispixfmtYUVAByte:
	case evispixfmtYUVAUShort:
	case evispixfmtYUVAUInt:
	case evispixfmtYUVAULong:
	case evispixfmtYUVAFloat:
	case evispixfmtYUVADouble:
		// IPL do not support YUVA color model
		assert(false);
		break;
	default:
		// should not go here
		assert(false);
		break;
	}

	// supports for multi-band image of VisSDK
	nChannels *= imageBase.NBands();
}

void CIPLImage::SetROI( int coi, int xOffset, int yOffset, int nWidth, int nHeight)
{
	assert( coi >= 0 && coi <= nChannels);
	assert( xOffset >= 0 && xOffset < width);
	assert( yOffset >= 0 && yOffset < height);
	assert( nWidth > 0);
	assert( nHeight > 0);
	assert( xOffset + nWidth < width);
	assert( yOffset + nHeight < height);
	
	m_ROI.coi = coi;
	m_ROI.xOffset = xOffset;
	m_ROI.yOffset = yOffset;
	m_ROI.width = nWidth;
	m_ROI.height = nHeight;

	roi = &m_ROI;
}

void CIPLImage::SetBorderMode( int mode, int border, int constVal)
{
	assert( mode == IPL_BORDER_CONSTANT ||
			mode == IPL_BORDER_REPLICATE ||
			mode == IPL_BORDER_REFLECT ||
			mode == IPL_BORDER_WRAP);

	switch(border)
	{
	case IPL_SIDE_TOP:
	case IPL_SIDE_BOTTOM:
	case IPL_SIDE_LEFT:
	case IPL_SIDE_RIGHT:
		BorderMode[border] = mode;
		BorderConst[border] = constVal;
		break;
	case IPL_SIDE_ALL:
		int i;
		for( i = 0; i < 4; i++)
		{
			BorderMode[i] = mode;
			BorderConst[i] = constVal;
		}
		break;
	default:
		// should not go here, the border style should only be top, bottom,
		// left, right or all.
		assert(false);
		break;
	}
}
// VisAddin.cpp: implementation of the CVisAddin class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VisAddin.h"
#include <cv.h>

void VisNormalizeImage(CVisFloatImage& floatimage, CVisGrayByteImage& grayimage)
{
	if ( floatimage.Width()<=0 || floatimage.Height()<=0 ) return;

	int x, y;
	float min = floatimage.Pixel(0,0);
	float max = floatimage.Pixel(0,0);
	for ( y=0; y<floatimage.Height(); y++ )
	{
		for ( x=0; x<floatimage.Width(); x++ )
		{
			if ( min>floatimage.Pixel(x,y) )
			{
				min = floatimage.Pixel(x,y);
			}
			if ( max<floatimage.Pixel(x,y) )
			{
				max = floatimage.Pixel(x,y);
			}
		}
	}
	float range = max-min;
	if ( range<=0.000001 ) range = 1.;

	grayimage.Allocate(floatimage.Shape());
	for ( y=0; y<floatimage.Height(); y++ )
	{
		for ( x=0; x<floatimage.Width(); x++ )
		{
			float tempf = 255*(floatimage.Pixel(x,y)-min)/range;
			grayimage.Pixel(x,y) = tempf>255 ? 255 : (tempf<0 ? 0 : BYTE(tempf));	
		}
	}
}

bool CopyImage2Buffer(CVisImageBase& image, BYTE* pBytes, int nByte, int storage_type)
{
	if ( image.Width()<=0 || image.Height()<=0 ) return false;
	if ( pBytes==NULL ) return false;
	BYTE* buffer_ptr = pBytes;

	EVisPixFmt pixfmt = image.PixFmt();
	switch( pixfmt & evispixfmtStructureMask )
	{
	case evispixfmtGray:
		if ( nByte!=image.Width()*image.Height() ) return false;

		if ( storage_type==0 )
		{
			for (int y=0; y<image.Height(); y++)
			{
				for (int x=0; x<image.Width(); x++)
				{
					*buffer_ptr++ = *image.PbPixel(x,y);
				}
			}
		}
		else
		{
			for (int x=0; x<image.Width(); x++)
			{
				for (int y=0; y<image.Height(); y++)
				{
					*buffer_ptr++ = *image.PbPixel(x,y);
				}
			}
		}
		break;
	case evispixfmtRGBA:
	case evispixfmtRGB:
		if ( nByte!=image.Width()*image.Height()*3 ) return false;

		if ( storage_type==0 )
		{
			for (int y=0; y<image.Height(); y++)
			{
				for (int x=0; x<image.Width(); x++)
				{
					BYTE* pixel_ptr = image.PbPixel(x,y);
					for ( int i=0; i<3; i++ )
					{
						*buffer_ptr++ = *pixel_ptr++;
					}
				}
			}
		}
		else
		{
			for (int x=0; x<image.Width(); x++)
			{
				for (int y=0; y<image.Height(); y++)
				{
					BYTE* pixel_ptr = image.PbPixel(x,y);
					for ( int i=0; i<3; i++ )
					{
						*buffer_ptr++ = *pixel_ptr++;
					}
				}
			}
		}
		break;
	default:
		return false;
	}

	return true;
}

bool CopyImage2Buffer(IplImage* image, BYTE* pBytes, int nByte, int storage_type)
{
	if ( image->width<=0 || image->height<=0 ) return false;
	if ( pBytes==NULL ) return false;
	if ( nByte!=image->width*image->height*image->nChannels ) return false;

	BYTE* buffer_ptr = pBytes;

	if ( storage_type==0 )
	{
		if ( image->origin==IPL_ORIGIN_TL )
		{
			for (int y=0; y<image->height; y++)
			{
				for (int x=0; x<image->width; x++)
				{
					uchar* pixel_ptr = &((uchar*)(image->imageData + image->widthStep*y))[x*image->nChannels];
					for (int i=0; i<image->nChannels; i++)
					{
						*buffer_ptr++ = pixel_ptr[i];
					}
				}
			}
		}
		else
		{
			for (int y=0; y<image->height; y++)
			{
				int tempy = image->height-1-y;
				for (int x=0; x<image->width; x++)
				{
					uchar* pixel_ptr = &((uchar*)(image->imageData + image->widthStep*tempy))[x*image->nChannels];
					for (int i=0; i<image->nChannels; i++)
					{
						*buffer_ptr++ = pixel_ptr[i];
					}
				}
			}
		}
	}
	else
	{
		if ( image->origin==IPL_ORIGIN_TL )
		{
			for (int x=0; x<image->width; x++)
			{
				for (int y=0; y<image->height; y++)
				{
					uchar* pixel_ptr = &((uchar*)(image->imageData + image->widthStep*y))[x*image->nChannels];
					for (int i=0; i<image->nChannels; i++)
					{
						*buffer_ptr++ = pixel_ptr[i];
					}
				}
			}
		}
		else
		{
			for (int x=0; x<image->width; x++)
			{
				for (int y=0; y<image->height; y++)
				{
					uchar* pixel_ptr = &((uchar*)(image->imageData + image->widthStep*(image->height-1-y)))[x*image->nChannels];
					for (int i=0; i<image->nChannels; i++)
					{
						*buffer_ptr++ = pixel_ptr[i];
					}
				}
			}
		}
	}
	return true;
}

bool CopyBuffer2Image(BYTE* pBytes, int nByte, CVisImageBase& image, int storage_type/* =0 */)
{
	if ( image.Width()<=0 || image.Height()<=0 ) return false;
	if ( pBytes==NULL ) return false;
	BYTE* buffer_ptr = pBytes;
	
	EVisPixFmt pixfmt = image.PixFmt();
	switch( pixfmt & evispixfmtStructureMask )
	{
	case evispixfmtGray:
		if ( nByte!=image.Width()*image.Height() ) return false;
		
		if ( storage_type==0 )
		{
			for (int y=0; y<image.Height(); y++)
			{
				for (int x=0; x<image.Width(); x++)
				{
					*image.PbPixel(x,y) = *buffer_ptr;
				}
			}
		}
		else
		{
			for (int x=0; x<image.Width(); x++)
			{
				for (int y=0; y<image.Height(); y++)
				{
					*image.PbPixel(x,y) = *buffer_ptr++;
				}
			}
		}
		break;
	case evispixfmtRGBA:
	case evispixfmtRGB:
		if ( nByte!=image.Width()*image.Height()*3 ) return false;
		
		if ( storage_type==0 )
		{
			for (int y=0; y<image.Height(); y++)
			{
				for (int x=0; x<image.Width(); x++)
				{
					BYTE* pixel_ptr = image.PbPixel(x,y);
					for ( int i=0; i<3; i++ )
					{
						BYTE tempb = *buffer_ptr++;
						*pixel_ptr++ = tempb;
//						 *pixel_ptr++ = *buffer_ptr++;
					}
				}
			}
		}
		else
		{
			for (int x=0; x<image.Width(); x++)
			{
				for (int y=0; y<image.Height(); y++)
				{
					BYTE* pixel_ptr = image.PbPixel(x,y);
					for ( int i=0; i<3; i++ )
					{
						*pixel_ptr++ = *buffer_ptr++;
					}
				}
			}
		}
		break;
	default:
		return false;
	}
	
	return true;
}

bool CopyBuffer2Image(BYTE* pBytes, int nByte, IplImage* image, int storage_type/* =0 */)
{
	if ( image->width<=0 || image->height<=0 ) return false;
	if ( pBytes==NULL ) return false;
	if ( nByte!=image->width*image->height*image->nChannels ) return false;
	
	BYTE* buffer_ptr = pBytes;
	
	if ( storage_type==0 )
	{
		if ( image->origin==IPL_ORIGIN_TL )
		{
			for (int y=0; y<image->height; y++)
			{
				for (int x=0; x<image->width; x++)
				{
					uchar* pixel_ptr = &((uchar*)(image->imageData + image->widthStep*y))[x*image->nChannels];
					for (int i=0; i<image->nChannels; i++)
					{
						pixel_ptr[i] = *buffer_ptr++;
					}
				}
			}
		}
		else
		{
			for (int y=0; y<image->height; y++)
			{
				int tempy = image->height-1-y;
				for (int x=0; x<image->width; x++)
				{
					uchar* pixel_ptr = &((uchar*)(image->imageData + image->widthStep*tempy))[x*image->nChannels];
					for (int i=0; i<image->nChannels; i++)
					{
						pixel_ptr[i] = *buffer_ptr++;
					}
				}
			}
		}
	}
	else
	{
		if ( image->origin==IPL_ORIGIN_TL )
		{
			for (int x=0; x<image->width; x++)
			{
				for (int y=0; y<image->height; y++)
				{
					uchar* pixel_ptr = &((uchar*)(image->imageData + image->widthStep*y))[x*image->nChannels];
					for (int i=0; i<image->nChannels; i++)
					{
						pixel_ptr[i] = *buffer_ptr++;
					}
				}
			}
		}
		else
		{
			for (int x=0; x<image->width; x++)
			{
				for (int y=0; y<image->height; y++)
				{
					uchar* pixel_ptr = &((uchar*)(image->imageData + image->widthStep*(image->height-1-y)))[x*image->nChannels];
					for (int i=0; i<image->nChannels; i++)
					{
						pixel_ptr[i] = *buffer_ptr++;
					}
				}
			}
		}
	}
	return true;
}

// storage_type: 0, store rows(left-right); 1, store columns(top-bottom)
BYTE* GetImageBuffer(IplImage* image, int& nByte, int storage_type /* = 0 */)
{
	nByte = 0;
	if ( image->width<=0 || image->height<=0 ) return NULL;	
	BYTE* pBytes = NULL;

	nByte = image->width * image->height * image->nChannels;
	pBytes = new BYTE[nByte];
	BYTE* buffer_ptr = pBytes;

	if ( storage_type==0 )
	{
		if ( image->origin==IPL_ORIGIN_TL )
		{
			for (int y=0; y<image->height; y++)
			{
				for (int x=0; x<image->width; x++)
				{
					uchar* pixel_ptr = &((uchar*)(image->imageData + image->widthStep*y))[x*image->nChannels];
					for (int i=0; i<image->nChannels; i++)
					{
						*buffer_ptr++ = pixel_ptr[i];
					}
				}
			}
		}
		else
		{
			for (int y=0; y<image->height; y++)
			{
				int tempy = image->height-1-y;
				for (int x=0; x<image->width; x++)
				{
					uchar* pixel_ptr = &((uchar*)(image->imageData + image->widthStep*tempy))[x*image->nChannels];
					for (int i=0; i<image->nChannels; i++)
					{
						*buffer_ptr++ = pixel_ptr[i];
					}
				}
			}
		}
	}
	else
	{
		if ( image->origin==IPL_ORIGIN_TL )
		{
			for (int x=0; x<image->width; x++)
			{
				for (int y=0; y<image->height; y++)
				{
					uchar* pixel_ptr = &((uchar*)(image->imageData + image->widthStep*y))[x*image->nChannels];
					for (int i=0; i<image->nChannels; i++)
					{
						*buffer_ptr++ = pixel_ptr[i];
					}
				}
			}
		}
		else
		{
			for (int x=0; x<image->width; x++)
			{
				for (int y=0; y<image->height; y++)
				{
					uchar* pixel_ptr = &((uchar*)(image->imageData + image->widthStep*(image->height-1-y)))[x*image->nChannels];
					for (int i=0; i<image->nChannels; i++)
					{
						*buffer_ptr++ = pixel_ptr[i];
					}
				}
			}
		}
	}

	return pBytes;
}

// storage_type: 0, store rows(left-right); 1, store columns(top-bottom)
BYTE* GetImageBuffer(CVisImageBase& image, int& nByte, int storage_type/* =0 */)
{
	nByte = 0;
	if ( image.Width()<=0 || image.Height()<=0 ) return NULL;
	BYTE* pBytes = NULL;
	BYTE* buffer_ptr = NULL;

	EVisPixFmt pixfmt = image.PixFmt();
	switch( pixfmt & evispixfmtStructureMask )
	{
	case evispixfmtGray:
		nByte = image.Width() * image.Height();
		pBytes = new BYTE[nByte];
		buffer_ptr = pBytes;

		if ( storage_type==0 )
		{
			for (int y=0; y<image.Height(); y++)
			{
				for (int x=0; x<image.Width(); x++)
				{
					*buffer_ptr++ = *image.PbPixel(x,y);
				}
			}
		}
		else
		{
			for (int x=0; x<image.Width(); x++)
			{
				for (int y=0; y<image.Height(); y++)
				{
					*buffer_ptr++ = *image.PbPixel(x,y);
				}
			}
		}
		break;
	case evispixfmtRGBA:
	case evispixfmtRGB:
		nByte = image.Width() * image.Height() * 3;
		pBytes = new BYTE[nByte];
		buffer_ptr = pBytes;

		if ( storage_type==0 )
		{
			for (int y=0; y<image.Height(); y++)
			{
				for (int x=0; x<image.Width(); x++)
				{
					BYTE* pixel_ptr = image.PbPixel(x,y);
					for ( int i=0; i<3; i++ )
					{
						*buffer_ptr++ = *pixel_ptr++;
					}
				}
			}
		}
		else
		{
			for (int x=0; x<image.Width(); x++)
			{
				for (int y=0; y<image.Height(); y++)
				{
					BYTE* pixel_ptr = image.PbPixel(x,y);
					for ( int i=0; i<3; i++ )
					{
						*buffer_ptr++ = *pixel_ptr++;
					}
				}
			}
		}
		break;
	default:
		{
			printf("\n Error: Unsupported format image!");
		}
	}

	return pBytes;
}

void CopyImage(IplImage* srcIpl, CVisImageBase& imageBase)
{
	assert(NULL != srcIpl);
	EVisPixFmt pixfmt = imageBase.PixFmt();
	
	int x, y;
	switch( pixfmt & evispixfmtStructureMask)
	{
	case evispixfmtRGBA:
		assert(srcIpl->nChannels==3);

		imageBase.Deallocate();
		imageBase.Allocate(srcIpl->width, srcIpl->height);

		if ( srcIpl->origin==IPL_ORIGIN_TL )
		{
			for (y=0; y<srcIpl->height; y++)
			{
				for (x=0; x<srcIpl->width; x++)
				{
					uchar* temp_ptr = &((uchar*)(srcIpl->imageData + srcIpl->widthStep*y))[x*srcIpl->nChannels];					
					BYTE* pixel_ptr = imageBase.PbPixel(x,y);
					for ( int i=0; i<srcIpl->nChannels; i++ )
					{
						*pixel_ptr++ = temp_ptr[i];
					}
				}
			}
		}
		else
		{
			for (y=0; y<srcIpl->height; y++)
			{
				int tempy = srcIpl->height-1-y;
				for (x=0; x<srcIpl->width; x++)
				{
					uchar* temp_ptr = &((uchar*)(srcIpl->imageData + srcIpl->widthStep*y))[x*srcIpl->nChannels];
					BYTE* pixel_ptr = imageBase.PbPixel(x,tempy);
					for ( int i=0; i<srcIpl->nChannels; i++ )
					{
						*pixel_ptr++ = temp_ptr[i];
					}
				}
			}
		}

		break;
	case evispixfmtGray:
		assert(srcIpl->nChannels==1);

		imageBase.Deallocate();
		imageBase.Allocate(srcIpl->width, srcIpl->height);

		if ( srcIpl->origin==IPL_ORIGIN_TL )
		{
			for (y=0; y<srcIpl->height; y++)
			{
				for (x=0; x<srcIpl->width; x++)
				{
					*imageBase.PbPixel(x, y) = ((uchar*)(srcIpl->imageData + srcIpl->widthStep*y))[x];				
				}
			}
		}
		else
		{
			for (y=0; y<srcIpl->height; y++)
			{
				int tempy = srcIpl->height-1-y;
				for (x=0; x<srcIpl->width; x++)
				{
					*imageBase.PbPixel(x, tempy) = ((uchar*)(srcIpl->imageData + srcIpl->widthStep*y))[x];				
				}
			}
		}
		break;
	default: return;
	}
}

void CopyImage(IplImage* srcIpl, IplImage** desIpl)
{
	if ( *desIpl==srcIpl ) return;
	if ( *desIpl!=NULL ) cvReleaseImage(desIpl);
	
	*desIpl = cvCreateImage(cvSize(srcIpl->width, srcIpl->height), srcIpl->depth, srcIpl->nChannels);
	(*desIpl)->origin = srcIpl->origin;
	
	for (int y=0; y<srcIpl->height; y++)
	{
		for (int x=0; x<srcIpl->width; x++)
		{
			uchar* temp_ptr = &((uchar*)(srcIpl->imageData + srcIpl->widthStep*y))[x*srcIpl->nChannels];
			uchar* des_ptr = &((uchar*)((*desIpl)->imageData + srcIpl->widthStep*y))[x*srcIpl->nChannels];
			
			for (int i=0; i<srcIpl->nChannels; i++)
			{
				des_ptr[i] = temp_ptr[i];
			}
		}
	}	
}

CVisConverter::CVisConverter()
{

}

CVisConverter::~CVisConverter()
{

}

// RGB2YUV Matrix (from matlab)

// Digital YUV2RGB Conversion (0 \<= Y \<= 255, - 128 \<= U \<= 127, - 128 \<= V \<= 127)
// <nl> <tab>  R = Y + 1.402 (V)
// <nl> <tab>  G = Y - 0.344 (U) - 0.714 (V)
// <nl> <tab>  B = Y + 1.772 (U)

// Digital RGB2YUV Conversion (0 \<= R \<= 255, 0 \<= G \<= 255, 0 \<= B \<= 255)
// <nl> <tab>  Y = 0.2991 * R + 0.5873 * G + 0.1136 * B
// <nl> <tab>  U =  ( B - Y ) * 0.5643
// <nl> <tab>  V =  ( R - Y ) * 0.7133

void CVisYUV2RGB::operator()(CVisRGBABytePixel& out, const CVisYUVABytePixel& in)
{
	out.SetA(in.A());
	int Y, U, V, R, G, B;

	Y = in.Y();
	U = in.U();
	V = in.V();
	U -= 128;
	V -= 128;
	
	R = Y+ 1.402 * V;
	G = Y - 0.344 * U - 0.714 * V;
	B = Y + 1.772 * U;

	out.SetR( min(255, max(0, R)));
	out.SetG( min(255, max(0, G)));
	out.SetB( min(255, max(0, B)));
}


void CVisRGB2YUV::operator()(CVisYUVABytePixel& out, const CVisRGBABytePixel& in)
{
	out.SetA(in.A());

	int R, G, B, Y, U, V;
	R = in.R();
	G = in.G();
	B = in.B();

	Y = 0.2991 * R + 0.5873 * G + 0.1136 * B;
	U = ( B - Y ) * 0.5643 + 128;
	V = ( R - Y ) * 0.7133 + 128;

	out.SetY( min(255, max(0, Y)));
	out.SetU( min(255, max(0, U)));
	out.SetV( min(255, max(0, V)));
}

void Instantiation()
{
	// copy
	{
		CVisRGBAByteImage srcImg, desImg;
		VisImageCopy(srcImg, desImg);
	}
	{
		CVisGrayByteImage srcImg, desImg;
		VisImageCopy(srcImg, desImg);
	}
	{
		CVisRGBAFloatImage srcImg, desImg;
		VisImageCopy(srcImg, desImg);
	}
	{
		CVisGrayFloatImage srcImg, desImg;
		VisImageCopy(srcImg, desImg);
	}

	// FlipHorizontal
	{
		CVisGrayFloatImage srcImg, desImg;
		VisImageFlipHorizontal(srcImg, desImg);
	}
	{
		CVisGrayByteImage srcImg, desImg;
		VisImageFlipHorizontal(srcImg, desImg);
	}
	{
		CVisRGBAFloatImage srcImg, desImg;
		VisImageFlipHorizontal(srcImg, desImg);
	}
	{
		CVisRGBAByteImage srcImg, desImg;
		VisImageFlipHorizontal(srcImg, desImg);
	}

	// FlipVertical
	{
		CVisGrayFloatImage srcImg, desImg;
		VisImageFlipVertical(srcImg, desImg);
	}
	{
		CVisGrayByteImage srcImg, desImg;
		VisImageFlipVertical(srcImg, desImg);
	}
	{
		CVisRGBAFloatImage srcImg, desImg;
		VisImageFlipVertical(srcImg, desImg);
	}
	{
		CVisRGBAByteImage srcImg, desImg;
		VisImageFlipVertical(srcImg, desImg);
	}

	// Zoom
	{
//		CVisGrayFloatImage srcImg, desImg;
//		VisImageZoom(srcImg, desImg);
	}
	{
//		CVisGrayByteImage srcImg, desImg;
//		VisImageZoom(srcImg, desImg);
	}
	{
//		CVisRGBAFloatImage srcImg, desImg;
//		VisImageZoom(srcImg, desImg);
	}
	{
//		CVisRGBAByteImage srcImg, desImg;
//		VisImageZoom(srcImg, desImg);
	}
}
///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisRGBA.cpp -- special functions for RGBA arithmetic

// DESCRIPTION
//  These routines implement fast basic operations on color
//  pixels, such as pixel averaging, linear interpolation,
//  convolution, and addition/subtraction.
//  
//  The basic strategy used is to use integer operations, and to
//  gang up two or four such operation per integer word.
//
// SEE ALSO
//  VisRGBA.h     description of functionality and definition of inlines
//
//  Fast Color Pixel Blends, Technical Note No. 2 (Vision Technology Group)
//   a study of algorithms for quickly blending color pixels
//
// BUGS
//  
//
// DESIGN
//  The current design is based on using 32-bit integer arithmetic
//  to do multiple (2 or 4) bands concurrently.
//  On a 64-bit architecture, double longs may be more efficient.
//  When the MMX architecture is available, the routines could
//  be re-written to optionally take advantage of this.
//
// HISTORY
//  23-May-96  Richard Szeliski (szeliski) at Microsoft
//  Created from code in VisResample.cpp.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////


#include <fstream.h>
#include "VisImageProcPch.h"
#include "VisImageProc.h"

//
//  These are used in VisConvolve.cpp:
//

void VisConvolveRow11(CVisRGBABytePixel *csrc, CVisRGBABytePixel *cdst, int n, 
                   int stride, int upsample, int o)
{
    for (int i = 0; i < n; i++, cdst += upsample, csrc += stride)
        *cdst =  VisPixelAverage(csrc[0], csrc[o]);
}

void VisConvolveRow121(CVisRGBABytePixel *csrc, CVisRGBABytePixel *cdst, int n, 
                    int stride, int upsample, int o)
{
#ifdef SLOWER // I think?
    for (int i = 0; i < n; i++, cdst += upsample, csrc += stride)
        *cdst = VisPixelAverage(csrc[1*o], VisPixelAverage(csrc[0*o], csrc[2*o]));
#else
    int *src = (int *) csrc, *dst = (int *) cdst;
    for (int i = 0; i < n; i++, dst += upsample, src += stride) {
        int r0 = (2* (src[1*o] & 0x00ff00ff) +
                    ((src[0*o] & 0x00ff00ff) + 
                     (src[2*o] & 0x00ff00ff)) +
                     0x00020002) >> 2;
        int r1 = (  ((src[1*o] & 0xff00ff00) >> 1) +
                    ((src[0*o] & 0xff00ff00) >> 2) + 
                    ((src[2*o] & 0xff00ff00) >> 2) +
                     0x00800080);
        dst[0] = (r0 & 0x00ff00ff) + (r1 & 0xff00ff00);
    }
#endif
}

void VisConvolveRow161(CVisRGBABytePixel *csrc, CVisRGBABytePixel *cdst, int n, 
                    int stride, int upsample, int o)
{
    int *src = (int *) csrc, *dst = (int *) cdst;
    for (int i = 0; i < n; i++, dst += upsample, src += stride) {
        int r0 = (6* (src[1*o] & 0x00ff00ff) +
                    ((src[0*o] & 0x00ff00ff) + 
                     (src[2*o] & 0x00ff00ff)) +
                     0x00040004) >> 3;
        int r1 = (3*((src[1*o] & 0xff00ff00) >> 2) +
                    ((src[0*o] & 0xff00ff00) >> 3) + 
                    ((src[2*o] & 0xff00ff00) >> 3) +
                     0x00800080);
        dst[0] = (r0 & 0x00ff00ff) + (r1 & 0xff00ff00);
    }
}

void VisConvolveRow14641(CVisRGBABytePixel *csrc, CVisRGBABytePixel *cdst, int n,
                      int stride, int upsample, int o)
{
    int *src = (int *) csrc, *dst = (int *) cdst;
    for (int i = 0; i < n; i++, dst += upsample, src += stride) {
        int r0 = (6* (src[2*o] & 0x00ff00ff) +
                  4*((src[1*o] & 0x00ff00ff) + 
                     (src[3*o] & 0x00ff00ff)) +
                    ((src[0*o] & 0x00ff00ff) +
                     (src[4*o] & 0x00ff00ff)) +
                     0x00080008) >> 4;
        int r1 = (3*((src[2*o] & 0xff00ff00) >> 3) +
                    ((src[1*o] & 0xff00ff00) >> 2) + 
                    ((src[3*o] & 0xff00ff00) >> 2) +
                    ((src[0*o] & 0xff00ff00) >> 4) +
                    ((src[4*o] & 0xff00ff00) >> 4) +
                     0x00800080);
        dst[0] = (r0 & 0x00ff00ff) + (r1 & 0xff00ff00);
    }
}

CVisGrayByteImage VisAlphaImageGray(const CVisRGBAByteImage& img)
{
	CVisGrayByteImage grayimg(img.Shape());
	for (int i = img.Left(); i < img.Right(); i++)
		for (int j = img.Top(); j < img.Bottom(); j++)
			grayimg.Pixel(i,j) = img.Pixel(i,j).A();

	return grayimg;
}

CVisRGBAByteImage VisAlphaImageRGBA(const CVisRGBAByteImage& img)
{
	CVisRGBAByteImage grayRGBAimg(img.Shape());
	for (int i = img.Left(); i < img.Right(); i++)
		for (int j = img.Top(); j < img.Bottom(); j++)
			grayRGBAimg.Pixel(i,j).SetRGBA(img.Pixel(i,j).A(), img.Pixel(i,j).A(), img.Pixel(i,j).A(), 255);

	return grayRGBAimg;
}

void VisRGBAFloatImageFileTextWrite(const CVisRGBAFloatImage& img, char* filename)
{
	int i, j;
	ofstream numberoutput(filename);

	for (i = img.Left(); i < img.Right(); i++)
		for (j = img.Top(); j < img.Bottom(); j++)
		{
			numberoutput << i << " " << j << " ";
			numberoutput << img.Pixel(i,j).R() << " ";
			numberoutput << img.Pixel(i,j).G() << " ";
			numberoutput << img.Pixel(i,j).B() << " ";
			numberoutput << img.Pixel(i,j).A() << endl;
		}
	numberoutput.close();
}


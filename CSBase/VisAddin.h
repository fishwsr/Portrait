// VisAddin.h: interface for the CVisAddin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VISADDIN_H__3B130397_B9F8_4E71_9EE6_C753C6D4646F__INCLUDED_)
#define AFX_VISADDIN_H__3B130397_B9F8_4E71_9EE6_C753C6D4646F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VisCore.h"
#include "../VisImageProc/VisResample.h"

void CSBASE_API CopyImage(IplImage* srcIpl, IplImage** desIpl);
void CSBASE_API CopyImage(IplImage* srcIpl, CVisImageBase& imageBase);

CSBASE_API bool CopyImage2Buffer(IplImage* image, BYTE* pBytes, int nByte, int storage_type=0);
CSBASE_API bool CopyImage2Buffer(CVisImageBase& image, BYTE* pBytes, int nByte, int storage_type=0);

CSBASE_API bool CopyBuffer2Image(BYTE* pBytes, int nByte, IplImage* image, int storage_type=0);
CSBASE_API bool CopyBuffer2Image(BYTE* nBytes, int nByte, CVisImageBase& image, int storage_type=0);

CSBASE_API BYTE* GetImageBuffer(IplImage* image, int& nByte, int storage_type=0);
CSBASE_API BYTE* GetImageBuffer(CVisImageBase& image, int& nByte, int storage_type=0);

CSBASE_API void VisNormalizeImage(CVisFloatImage& floatimage, CVisGrayByteImage& grayimage);

template<class pixelType>
void CSBASE_API VisImageCopy(const CVisImage<pixelType> &srcImage, CVisImage<pixelType> &desImg);

template <class pixelType>
void CSBASE_API VisImageZoom(const CVisImage<pixelType> &srcImage, CVisImage<pixelType> &desImg, double sx=1., double sy=1., TVisInterpolation interp=VIS_INTERP_CUBIC);

template<class pixelType>
void CSBASE_API VisImageFlipHorizontal(CVisImage<pixelType> &in_image, CVisImage<pixelType> &out_image);

template<class pixelType>
void CSBASE_API VisImageFlipVertical(CVisImage<pixelType> &in_image, CVisImage<pixelType> &out_image);

inline bool inside(float l, float r, float t, float b, float x, float y)
{
	return ((x>=l)&&(y>=t)&&(x<=r)&&(y<=b));
}

inline bool inside(int l, int r, int t, int b, int x, int y)
{
	return ((x>=l)&&(y>=t)&&(x<=r)&&(y<=b));
}


struct CSBASE_API CVisSum
{
	inline void operator()(float& out, const float& in0, const float& in1);
	inline void operator()(float& out, const float& in1);
};

struct CSBASE_API CVisMul
{
	inline void operator()(float& out, const float& in0, const float& in1);
	inline void operator()(float& out, const float& in);
};

struct CSBASE_API CVisMax
{
	void Reset() {m_fMaxValue = - FLT_MAX; }; 
	CVisMax() { m_fMaxValue = - FLT_MAX;};
	inline void operator()(float& in);
	float m_fMaxValue;
};

inline void CVisSum::operator()(float& out, const float& in)
{
	out += in;
}

inline void CVisSum::operator()(float& out, const float& in0, const float& in1)
{
	out = in0 + in1;
}

inline void CVisMul::operator() (float& out, const float& in)
{
	out *= in;
}

inline void CVisMul::operator()(float& out, const float& in0, const float& in1)
{
	out = in0 * in1;
}

inline void CVisMax::operator()(float& in)
{
	if (in > m_fMaxValue)
	{
		m_fMaxValue = in;
	}
}

template<class T>
bool GVisImgWriteFile( const CVisImage<T> & imRef, const char* csFileName );
template<class T>
bool GVisImgReadFile( CVisImage<T> & imRef, const char* csFileName );

//
//  Implenmention of template functions
//                  
template<class T>
bool GVisImgWriteFile( const CVisImage<T> & imRef, const char* csFileName)
{
	FILE * pFile = fopen( csFileName, "wb" );

	if ( pFile == NULL )
	{
		return false;
	}
	else
	{
		int  nBytesPerT   = sizeof(T);
		int  nBytesPerRow = imRef.Width() * nBytesPerT;
		RECT rcImg        = imRef.Rect();

		fwrite( &nBytesPerT, sizeof(int), 1, pFile );
		fwrite( &rcImg, sizeof(RECT), 1, pFile );

		int x = rcImg.left;
		for ( int y = rcImg.top; y < rcImg.bottom; y++ )
		{
			fwrite( imRef.PbPixel(x,y), nBytesPerRow, 1, pFile );
		}

		fclose( pFile );
		return true;
	}
}

template<class T>
bool GVisImgReadFile( CVisImage<T> & imRef, const char* csFileName)
{
	FILE * pFile = fopen( csFileName, "rb" );

	if ( pFile != NULL )
	{
		int  nBytesPerT;
		int  nBytesPerRow;
		RECT rcImg;

		fread( &nBytesPerT, sizeof(int), 1, pFile );

		if ( nBytesPerT == sizeof(T) )
		{
			fread( &rcImg, sizeof(RECT), 1, pFile );
			imRef.Allocate( rcImg );

			nBytesPerRow = imRef.Width() * nBytesPerT;

			int x = rcImg.left;
			for ( int y = rcImg.top; y < rcImg.bottom; y++ )
			{
				fread( imRef.PbPixel(x,y), nBytesPerRow, 1, pFile );
			}

			fclose( pFile );
			return true;
		}
	}

	fclose( pFile );
	return false;
}

class CVisConverter  
{
public:
	CVisConverter();
	virtual ~CVisConverter();

};

struct CSBASE_API CVisYUV2RGB
{
	void operator()(CVisRGBABytePixel& out, const CVisYUVABytePixel& in);
};
struct CSBASE_API CVisRGB2YUV
{
	void operator()(CVisYUVABytePixel& out, const CVisRGBABytePixel& in);
};

struct CSBASE_API CVisByte2Float
{
	inline void operator()(float& out, BYTE& in);
};

inline void CVisByte2Float::operator()(float& out, BYTE& in)
{
	out = (float)in / 255.0;
}

struct CSBASE_API CVisGray2RGB
{
	inline void operator()(CVisRGBABytePixel& out, BYTE& in);
};

inline void CVisGray2RGB::operator()(CVisRGBABytePixel& out, BYTE& in)
{
	out.SetRGBA(in, in, in, 255);
};

struct CSBASE_API CVisYUVAGetYBand
{
	void operator()(BYTE& out, const CVisYUVABytePixel& in);
};

inline void CVisYUVAGetYBand::operator()(BYTE& out, const CVisYUVABytePixel& in)
{
	out = in.Y();
};

struct CSBASE_API CVisYUVAGetUBand
{
	void operator()(BYTE& out, const CVisYUVABytePixel& in);
};

inline void CVisYUVAGetUBand::operator()(BYTE& out, const CVisYUVABytePixel& in)
{
	out = in.U();
};

struct CSBASE_API CVisYUVAGetVBand
{
	void operator()(BYTE& out, const CVisYUVABytePixel& in);
};

inline void CVisYUVAGetVBand::operator()(BYTE& out, const CVisYUVABytePixel& in)
{
	out = in.V();
};

#include "VisAddin.inl"

#endif // !defined(AFX_VISADDIN_H__3B130397_B9F8_4E71_9EE6_C753C6D4646F__INCLUDED_)

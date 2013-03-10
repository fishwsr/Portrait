// GVisSupport.h: interface for the CGVisSupport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GVISSUPPORT_H__DBFA146B_5052_4A9B_A5EB_927723FFF0BE__INCLUDED_)
#define AFX_GVISSUPPORT_H__DBFA146B_5052_4A9B_A5EB_927723FFF0BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GVisDVector.h"
#include "GVisDMatrix.h"
#include "GVisFilterBank.h"
#include "GVisHough.h"
#include "GVisPCA.h"
#include "GVisHistogram2D.h"
#include "GVisDataFile.h"
#include "GVisImgArray.h"
#include "GVisImg.h"
#include "QSort.h"

class CGVisSupport  
{
public:
	CGVisSupport();
	virtual ~CGVisSupport();

};


//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//										Important Functions										//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

typedef std::vector<CPoint>			 STL_CPointVector;
typedef std::vector<POINT>			 STL_POINTVector;
typedef std::vector<int>			 STL_IntVector;
typedef std::vector<float>			 STL_FloatVector;

CVisByteImage  GVisImgFromImg( const CVisFloatImage & imRef, BYTE nMin, BYTE nMax );
void		   GVisImgFromImg( CVisByteImage imDst, const CVisFloatImage & imSrc, double (* func)(double a) );

CVisFloatImage GVisImgFromImg( const CVisFloatImage & imRef, double (* func)(double a) );

CVisFloatImage GVisImgFromMatrix( const CGVisDMatrix & matRef, CRect rcImg );
CVisFloatImage GVisImgFromVector( const CGVisDVector & vecRef, CRect rcImg );

CGVisDMatrix   GVisMatrixFromImg( const CVisFloatImage & imRef );

CGVisDVector   GVisVectorFromImg( const CVisFloatImage & imRef );
CGVisDVector   GVisVectorFromImg( const CVisByteImage  & imRef );
void		   GVisImgResample( CVisByteImage imRef, CVisByteImage imTar, CVisFloatImage imXPos, CVisFloatImage imYPos, BYTE outlier_value = 0);


CGVisDVector   GVisHistogramFromFloat( const CVisFloatImage & imRef, CRect rcROI, int nNumBins, double dMax = -1.0, double dMin = 1.0 );

CRect		   GVisMaxMinPixelFromFloat( const CVisFloatImage & imRef, CRect rcROI, double & dMax, double & dMin );
void		   GVisDrawHistogram( CVisRGBAByteImage imColor, CPoint ptOriginToDraw, CGVisDVector vecHistogram, int nBinWidth, int nBinMaxHeight, bool bNeedFrame = true, COLORREF color = RGB(0, 0, 0) );
CVisFloatImage ReconstructionOfImage( const CVisFloatImage imResponses[], const CVisFloatImage imKernels[], int nNumKernels );

template<class T>
void GVisImgAbstraction( const CVisImage<T> & imSrc, CVisImage<T> & imDst, int nGridSizeX, int nGridSizeY )
{
	CRect rcImgSrc = imSrc.Rect();
	CRect rcImgDst = rcImgSrc;

	rcImgDst.right = rcImgDst.left + ( imSrc.Width() + nGridSizeX - 1 ) /  nGridSizeX;
	rcImgDst.bottom= rcImgDst.top + ( imSrc.Height() + nGridSizeY - 1 ) /  nGridSizeY;

	imDst.Allocate( rcImgDst );

	CPoint ptInSrc;
	CPoint ptInDst;

	for ( ptInDst.y = rcImgDst.top, ptInSrc.y = rcImgSrc.top; ptInDst.y < rcImgDst.bottom; ptInDst.y++, ptInSrc.y += nGridSizeY )
	for ( ptInDst.x = rcImgDst.left, ptInSrc.x = rcImgSrc.left; ptInDst.x < rcImgDst.right; ptInDst.x++, ptInSrc.x += nGridSizeX )
	{
		imDst.Pixel( ptInDst) = imSrc.Pixel( ptInSrc );
	}
}

template<class T>
bool GVisImgWriteFile( const CVisImage<T> & imRef, CString csFileName )
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
bool GVisImgReadFile( CVisImage<T> & imRef, CString csFileName )
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

/*
template<class T>
void GVisImgMlgSetPoints( CVisImage<T> & imRef, const T& value, const CPtList& lsPoints )
{
	POSITION pos = lsPoints.GetHeadPosition();
	while (pos)
	{
		imRef.Pixel( lsPoints.GetNext(pos) ) = value;
	}
}
//*/


template<class T>
void GVisImgMlgSetPoints( CVisImage<T> & imRef, const T& value, const STL_POINTVector& ptvPoints )
{
	int nNumPoints = ptvPoints.size();

	for ( int i = 0; i < nNumPoints; i++ )
	{
		imRef.Pixel( ptvPoints[i] ) = value;
	}
}
// This should be used as less as possible, since it will allocate a big mask image.
template<class T>
void GVisImgMlgFindNbrWithMask( const STL_POINTVector& ptvRefPoints, STL_POINTVector& ptvNbrPoints, const STL_POINTVector& ptvNbrRelation , const CVisImage<T>& imMask, T MaskValue )
{
	int nNumNbrRelations = ptvNbrRelation.size();
	int nNumRefPoints	 = ptvRefPoints.size();

	CRect rcMask = imMask.Rect();
	CVisByteImage imRefMask( rcMask );

	// Init imRefMask
	imRefMask.FillPixels( 0 );
	GVisImgMlgSetPoints( imRefMask, (BYTE)255, ptvRefPoints );

	ptvNbrPoints.clear();

	// Find Neighbor now
	for ( int i = 0; i < nNumRefPoints; i++ )
	{
		CPoint ptInRef = ptvRefPoints[i];

		for ( int j = 0; j < nNumNbrRelations; j++ )
		{
			CPoint ptInNbr = ptInRef + ptvNbrRelation[j];
		
			if ( rcMask.PtInRect( ptInNbr ) && imRefMask.Pixel( ptInNbr ) == 0 && imMask.Pixel( ptInNbr ) == MaskValue )
			{
				imRefMask.Pixel( ptInNbr ) = 255;
				ptvNbrPoints.push_back( ptInNbr );
			}
		}
	}
}

template<class T>
void GVisImgMlgFindNbrWithMask( const CPoint& ptRef, STL_POINTVector& ptvNbrPoints, const STL_POINTVector& ptvNbrRelation , const CVisImage<T>& imMask, T MaskValue )
{
	int nNumNbrRelations = ptvNbrRelation.size();
	CRect rcMask = imMask.Rect();

	ptvNbrPoints.clear();

	// Find Neighbor now
	for ( int i = 0; i < nNumNbrRelations; i++ )
	{
		CPoint ptInNbr = ptRef + ptvNbrRelation[i];
	
		if ( rcMask.PtInRect( ptInNbr ) && imMask.Pixel( ptInNbr ) == MaskValue )
		{
			ptvNbrPoints.push_back( ptInNbr );
		}
	}
}

#endif // !defined(AFX_GVISSUPPORT_H__DBFA146B_5052_4A9B_A5EB_927723FFF0BE__INCLUDED_)

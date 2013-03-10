
// CSBase.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "CSBase.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{
    switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
    }
    return TRUE;
}

////////////////////////////// some auxiliary functions //////////////////////
void TakeABreath()
{
	MSG msg;
	while (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

// sum all columns
CVisDVector VisDMatrixSumColumn(const CVisDMatrix& data)
{
	CVisDVector vectorRet(data.NRows());

	int nLength = vectorRet.Length();
	double* vecRetPtr = vectorRet.data();
	for ( int i=0; i<nLength; i++ )
	{
		*vecRetPtr++ = VisDVectorSum(data.Row(i));
	}
	return vectorRet;
}

// sum all rows
CVisDVector VisDMatrixSumRow(const CVisDMatrix& data)
{
	CVisDVector vectorRet(data.NCols());

	int nLength = vectorRet.Length();
	double* vecRetPtr = vectorRet.data();
	for ( int i=0; i<nLength; i++ )
	{
		*vecRetPtr++ = VisDVectorSum(data.Column(i));
	}
	return vectorRet;
}

CVisDMatrix VisDMatrixEntryRecip(CVisDMatrix& refMatrix)
{
	CVisDMatrix matrixRet(refMatrix.NRows(), refMatrix.NCols());

	int nRows = refMatrix.NRows();
	int nCols = refMatrix.NCols();
	for ( int r=0; r<nRows; r++ )
	{
		double *thisrowptr = &matrixRet[r][0];
		double *refrowptr = &refMatrix[r][0];
		for ( int c=0; c<nCols; c++ )
		{
			if ( *refrowptr > 1e-9 )
			{
				*thisrowptr++ = 1./(*refrowptr++);
			}
			else
			{
				*thisrowptr++ = 1./(1e-9);
				refrowptr++;
			}
		}
	}
	return matrixRet;
}

CVisDMatrix VisDMatrixEntryProd(CVisDMatrix& refMatrixA, CVisDMatrix& refMatrixB)
{
	CVisDMatrix matrixRet(refMatrixA);
	assert(matrixRet.NRows() == refMatrixB.NRows());
	assert(matrixRet.NCols() == refMatrixB.NCols());

	int nRows = matrixRet.NRows();
	int nCols = matrixRet.NCols();
	for ( int r=0; r<nRows; r++ )
	{
		double *thisrowptr = &matrixRet[r][0];
		double *refrowptr = &refMatrixB[r][0];
		for ( int c=0; c<nCols; c++ )
		{
			*thisrowptr++ *= *refrowptr++;
		}
	}
	return matrixRet;
}

CVisDMatrix VisDMatrixEntryDiv(CVisDMatrix& refMatrixA, CVisDMatrix& refMatrixB)
{
	CVisDMatrix matrixRet(refMatrixA);
	assert(matrixRet.NRows() == refMatrixB.NRows());
	assert(matrixRet.NCols() == refMatrixB.NCols());
	
	int nRows = matrixRet.NRows();
	int nCols = matrixRet.NCols();
	for ( int r=0; r<nRows; r++ )
	{
		double *thisrowptr = &matrixRet[r][0];
		double *refrowptr = &refMatrixB[r][0];
		for ( int c=0; c<nCols; c++ )
		{
			*thisrowptr++ /= 1e-9 + (*refrowptr++);
		}
	}
	return matrixRet;
}

CVisDVector VisDVectorEntryProd(const CVisDVector& refVectorA, const CVisDVector& refVectorB)
{
	CVisDVector vectorRet(refVectorA);
	assert(vectorRet.Length() == refVectorB.Length());

	double* thisdataptr = vectorRet.data();
	double* refdataptr = refVectorB.data();

	int nLength = vectorRet.Length();
	for ( int i=0; i<nLength; i++ )
	{
		*thisdataptr++ *= *refdataptr++;
	}
	return vectorRet;
}

CVisDVector VisDVectorEntryDiv(const CVisDVector& refVectorA, const CVisDVector& refVectorB)
{
	CVisDVector vectorRet(refVectorA);
	assert(vectorRet.Length() == refVectorB.Length());
	
	double* thisdataptr = vectorRet.data();
	double* refdataptr = refVectorB.data();
	
	int nLength = vectorRet.Length();
	for ( int i=0; i<nLength; i++ )
	{
		if ( *refdataptr > 1e-9 )
		{
			*thisdataptr /= *refdataptr++;
		}
		else
		{
			*thisdataptr++ /= (1e-9);
			refdataptr++;
		}
	}
	return vectorRet;
}

CVisDVector VisDVectorEntryRecip(const CVisDVector& refVector)
{
	CVisDVector vectorRet(refVector.Length());
	
	double* thisdataptr = vectorRet.data();
	double* refdataptr = refVector.data();
	
	int nLength = vectorRet.Length();
	for ( int i=0; i<nLength; i++ )
	{
		if ( *refdataptr > 1e-9 )
		{
			*thisdataptr++ = 1./(*refdataptr++);
		}
		else
		{
			*thisdataptr++ = 1./(1e-9);
			refdataptr++;
		}
	}
	return vectorRet;
}

double VisDVectorSum(const CVisDVector& data)
{
	double dSum = 0.;
	double* dataptr = data.data();

	int nLength = data.Length();
	for ( int i=0; i<nLength; i++ )
	{
		dSum += *dataptr++;
	}
	return dSum;
}

double VisDVectorSumL1(const CVisDVector& data)
{
	double dSum = 0.;
	double* dataptr = data.data();
	
	int nLength = data.Length();
	for ( int i=0; i<nLength; i++ )
	{
		dSum += fabs(*dataptr++);
	}
	return dSum;
}

double VisDVectorSumL2(const CVisDVector& data)
{
	double dSum = 0.;
	double* dataptr = data.data();
	
	int nLength = data.Length();
	for ( int i=0; i<nLength; i++ )
	{
		dSum += (*dataptr) * (*dataptr);
		dataptr++;
	}
	return dSum;
}

double VisDMatrixSum(CVisDMatrix& data)
{
	double dSum = 0.;
	
	int nRows = data.NRows();
	int nCols = data.NCols();
	for ( int r=0; r<nRows; r++ )
	{
		double* rowptr = &data[r][0];
		for ( int c=0; c<nCols; c++ )
		{
			dSum += *rowptr++;
		}
	}
	return dSum;
}

double VisDMatrixSumL1(CVisDMatrix& data)
{
	double dSum = 0.;
	
	int nRows = data.NRows();
	int nCols = data.NCols();
	for ( int r=0; r<nRows; r++ )
	{
		double* rowptr = &data[r][0];
		for ( int c=0; c<nCols; c++ )
		{
			dSum += fabs(*rowptr++);
		}
	}
	return dSum;
}

double VisDMatrixSumL2(CVisDMatrix& data)
{
	double dSum = 0.;
	
	int nRows = data.NRows();
	int nCols = data.NCols();
	for ( int r=0; r<nRows; r++ )
	{
		double* rowptr = &data[r][0];
		for ( int c=0; c<nCols; c++ )
		{
			dSum += (*rowptr) * (*rowptr);
			rowptr++;
		}
	}
	return dSum;
}

double VisDMatrixMin(CVisDMatrix& data, int& iRow, int& iCol)
{
	iRow = 0;
	iCol = 0;
	double fMinData = data[0][0];
	
	int nRows = data.NRows();
	int	nCols = data.NCols();
	for ( int r=0; r<nRows; r++ )
	{
		double* rowptr = &data[r][0]; 
		for ( int c=0; c<nCols; c++ )
		{
			if ( fMinData > *rowptr )
			{
				fMinData = *rowptr;
				iRow = r;
				iCol = c;
			}
			rowptr++;
		}
	}
	return fMinData;
}

double VisDMatrixMax(CVisDMatrix& data, int& iRow, int& iCol)
{
	iRow = 0;
	iCol = 0;
	double fMaxData = data[0][0];
	
	int nRows = data.NRows();
	int	nCols = data.NCols();
	for ( int r=0; r<nRows; r++ )
	{
		double* rowptr = &data[r][0]; 
		for ( int c=0; c<nCols; c++ )
		{
			if ( fMaxData < *rowptr )
			{
				fMaxData = *rowptr;
				iRow = r;
				iCol = c;
			}
			rowptr++;
		}
	}
	return fMaxData;
}

double VisDVectorMin(const CVisDVector& data, int& index)
{
	double* dataptr = data.data();

	index = 0;
	double fMinData = *dataptr++;

	int nLength = data.Length();
	for ( int i=1; i<nLength; i++ )
	{
		if ( fMinData > *dataptr )
		{
			fMinData = *dataptr;
			index = i;
		}
		dataptr++;
	}

	return fMinData;
}

double VisDVectorMax(const CVisDVector& data, int& index)
{
	double* dataptr = data.data();

	index = 0;
	double fMaxData = *dataptr++;

	int nLength = data.Length();
	for ( int i=1; i<nLength; i++ )
	{
		if ( fMaxData < *dataptr )
		{
			fMaxData = *dataptr;
			index = i;
		}
		dataptr++;
	}

	return fMaxData;
}

int VisDVectorMinIndex(const CVisDVector& data)
{
	int iMinIndex = 0;
	double fMinData = data[0];

	for (int iIndex = 1; iIndex < data.Length(); iIndex++ )
	{
		if (fMinData > data[iIndex])
		{
			fMinData = data[iIndex];
			iMinIndex = iIndex;
		}
	}
	return iMinIndex;
}

double VisDVectorMinData(const CVisDVector& data)
{
	double fMinData = data[0];

	for (int iIndex = 1; iIndex < data.Length(); iIndex++ )
	{
		fMinData = min(fMinData, data[iIndex]);
	}
	return fMinData;
}

int VisDVectorMaxIndex(const CVisDVector& data)
{
	int iMaxIndex = 0;
	double fMaxData = data[0];

	for (int iIndex = 1; iIndex < data.Length(); iIndex++ )
	{
		if (fMaxData < data[iIndex])
		{
			fMaxData = data[iIndex];
			iMaxIndex = iIndex;
		}
	}
	return iMaxIndex;
}

double VisDVectorMaxData(const CVisDVector& data)
{
	double fMaxData = data[0];

	for (int iIndex = 1; iIndex < data.Length(); iIndex++ )
	{
		fMaxData = max(fMaxData, data[iIndex]);
	}
	return fMaxData;
}

void VisImage2Vector(const CVisGrayByteImage& img, CVisDVector& data)
{
	data.Resize(img.Width() * img.Height());

	double * desptr = data.data();
	for (int iy = 0; iy < img.Height(); iy++)
	{
		const BYTE * srcptr = img.PbFirstPixelInRow(iy);
		for (int ix = 0; ix < img.Width(); ix++ )
		{
			*desptr++ = *srcptr++;
		}
	}
	data /= 255;
}

void VisImage2Vector(const CVisGrayFloatImage& img, CVisDVector& data)
{
	data.Resize(img.Width() * img.Height());

	double * desptr = data.data();
	for (int iy = 0; iy < img.Height(); iy++)
	{
		float * srcptr = (float *)img.PbFirstPixelInRow(iy);
		for (int ix = 0; ix < img.Width(); ix++ )
		{
			*desptr++ = *srcptr++;
		}
	}
}

void VisVector2Image(const CVisDVector& data, CVisGrayByteImage& img, int width, int height)
{
	if ((width!=0) && (height!=0))
	{
		img.Allocate(width , height);
	}
	else
	{
		width = img.Width();
		height = img.Height();
	}
	if (data.Length() != (width*height))
	{
		std::cout << "error in VisVector2Image!";
		return;
	}

	const double * srcptr = data.data();
	for (int iy = 0; iy < height; iy++ )
	{
		BYTE * desptr = img.PbFirstPixelInRow(iy);
		for (int ix = 0; ix < width; ix++ )
		{
			//*desptr++ = *srcptr++ ;
			*desptr++ = 255 * max( 0, min( 1.0 , *srcptr));
			srcptr++;
		}
	}
}

void VisVector2Image(const CVisDVector& data, CVisGrayFloatImage& img, int width, int height)
{
	if ((width!=0) && (height!=0))
	{
		img.Allocate(width , height);
	}
	else
	{
		width = img.Width();
		height = img.Height();
	}
	if (data.Length() != (width*height))
	{
		std::cout << "error in VisVector2Image!";
		return;
	}

	const double * srcptr = data.data();
	for (int iy = 0; iy < height; iy++ )
	{
		float * desptr = (float *)img.PbFirstPixelInRow(iy);
		for (int ix = 0; ix < width; ix++ )
		{
			*desptr++ = *srcptr;
			srcptr++;
		}
	}
}

void VisGeomTransBoundary(const CVisTransform4x4& trans, const RECT& inR, RECT& outR)
{
	CVisDVector outX(4);
	CVisDVector outY(4);
	Apply2DRTSTransform(const_cast<CVisTransform4x4&>(trans), inR.left, inR.bottom, outX[0], outY[0]);
	Apply2DRTSTransform(const_cast<CVisTransform4x4&>(trans), inR.left, inR.top , outX[1], outY[1]);
	Apply2DRTSTransform(const_cast<CVisTransform4x4&>(trans), inR.right, inR.bottom, outX[2], outY[2]);
	Apply2DRTSTransform(const_cast<CVisTransform4x4&>(trans), inR.right, inR.top, outX[3], outY[3]);

	outR.left = VisDVectorMinData(outX);
	outR.right = VisDVectorMaxData(outX) + 1;
	outR.top =  VisDVectorMinData(outY);
	outR.bottom = VisDVectorMaxData(outY) + 1;
}

COutputDataFile& Output(COutputDataFile& ofs, const std::vector<CVisDVector*>& vpVector)
{
	ofs << ClassTag("VectorArray") << int(vpVector.size()) << sep_endl;
	
	for ( int i=0; i<vpVector.size(); i++ )
	{
		if ( vpVector[i]==NULL )
		{
			ofs << false << sep_endl;
		}
		else
		{
			ofs << true << sep_endl;
			ofs << *vpVector[i] << sep_endl;
		}
	}
	
	return ofs;
}

COutputDataFile& operator<<(COutputDataFile& ofs, const std::vector<CVisDVector*>& vpVector)
{
	return Output(ofs, vpVector);
}

CInputDataFile& Input(CInputDataFile& ifs, std::vector<CVisDVector*>& vpVector)
{
	int nVec;
	
	ifs >> ClassTag("VectorArray") >> nVec >> sep_endl;
	vpVector.resize(nVec);
	
	for ( int i=0; i<vpVector.size(); i++ )
	{
		bool valid;
		ifs >> valid >> sep_endl;
		
		if ( valid )
		{
			if ( vpVector[i]!=NULL )
			{
				delete vpVector[i];
			}
			vpVector[i] = new CVisDVector;
			ifs >> *vpVector[i] >> sep_endl;
		}
		else
		{
			if ( vpVector[i]!=NULL )
			{
				delete vpVector[i];
			}
			vpVector[i] = NULL;
		}
	}
	
	return ifs;
}

CInputDataFile& operator>>(CInputDataFile& ifs, std::vector<CVisDVector*>& vpVector)
{
	return Input(ifs, vpVector);
}


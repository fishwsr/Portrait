// MatrixMultiband.cpp: implementation of the CMatrixMultiband class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MatrixMultiband.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMatrixMultiband::CMatrixMultiband(int nBand /*=4*/)
: m_nBandNum(max(1, nBand))
{
	m_vMatrix.resize(m_nBandNum);	
}

CMatrixMultiband::~CMatrixMultiband()
{
	m_vMatrix.clear();
}

void CMatrixMultiband::Init(int nBand)
{
	if ( nBand == m_nBandNum ) 
		return;
	else
	{
		if ( nBand>0 ) m_nBandNum = nBand;
		m_vMatrix.resize(nBand);
	}
}

CMatrixMultiband& CMatrixMultiband::operator=(const CMatrixMultiband& ref)
{
	if ( &ref != this )
	{
		m_nBandNum = ref.m_nBandNum;

		if ( m_nBandNum != ref.m_nBandNum )	
		{
			m_vMatrix.resize(m_nBandNum);
		}
		
		for ( int iBand=0; iBand<m_nBandNum; iBand++ )
		{
			if ( m_vMatrix[iBand].NCols()!=ref.m_vMatrix[iBand].NCols() || 
				 m_vMatrix[iBand].NRows()!=ref.m_vMatrix[iBand].NRows() )
			{
				m_vMatrix[iBand].Resize(ref.m_vMatrix[iBand].NRows(), ref.m_vMatrix[iBand].NCols()); 
			}
			m_vMatrix[iBand] = ref.m_vMatrix[iBand];
		}
	}
	return *this;
}

CVisDMatrix& CMatrixMultiband::operator[](int iBand)
{
	assert(iBand>=0 && iBand<m_nBandNum);
	
	return m_vMatrix[iBand];
}

int CMatrixMultiband::NCols() const
{
	assert(m_nBandNum>0);
	return m_vMatrix[0].NCols();
}

int CMatrixMultiband::NRows() const
{
	assert(m_nBandNum>0);
	return m_vMatrix[0].NRows();
}

void CMatrixMultiband::Resize(int rows, int cols)
{
	for (int iBand=0; iBand<m_nBandNum; iBand++)
	{
		if ( m_vMatrix[iBand].NCols()!=cols || m_vMatrix[iBand].NRows()!=rows )
			m_vMatrix[iBand].Resize(rows, cols);
	}
}

CMatrixMultiband& CMatrixMultiband::operator=(double d)
{
	for (int iBand=0; iBand<m_nBandNum; iBand++)
	{
		m_vMatrix[iBand] = d;
	}	

	return *this;
}

CMatrixMultiband& CMatrixMultiband::operator=(const CVisDMatrix& mat)
{
	for (int iBand=0; iBand<m_nBandNum; iBand++)
	{
		if ( m_vMatrix[iBand].NCols()!=mat.NCols() || m_vMatrix[iBand].NRows()!=mat.NRows() )
			m_vMatrix[iBand].Resize(mat.NRows(), mat.NCols());
		m_vMatrix[iBand] = mat;
	}	
	
	return *this;
}

bool CMatrixMultiband::AllBandsOfSameDim() const
{
	assert(m_nBandNum<0);
	if ( m_nBandNum<0 ) return false;

	int rows = m_vMatrix[0].NRows();
	int cols = m_vMatrix[0].NCols();

	for ( int iBand=0; iBand<m_nBandNum; iBand++ )
	{
		if ( m_vMatrix[iBand].NRows()!=rows || m_vMatrix[iBand].NCols()!=cols ) 
			return false;
	}

	return true;
}

void Matrix3bandToImageRGBA(const CMatrixMultiband& mat, CVisRGBAByteImage& image)
{
	assert(mat.NBand() == 4);
	assert(mat.AllBandsOfSameDim());

	if (mat.NBand()!=4 || !mat.AllBandsOfSameDim()) return;

	int width = const_cast<CMatrixMultiband&>(mat)[0].NCols();
	int height = const_cast<CMatrixMultiband&>(mat)[0].NRows();

	if ( image.Width()!=width || image.Height()!=height )
	{
		image.Deallocate(false);
		image.Allocate(width, height);
	}
	
	for ( int y=0; y<height; y++ )
	{
		for ( int x=0; x<width; x++ )
		{
			image.Pixel(x,y).SetR(unsigned char(const_cast<CMatrixMultiband&>(mat)[0][y][x]));
			image.Pixel(x,y).SetG(unsigned char(const_cast<CMatrixMultiband&>(mat)[1][y][x]));
			image.Pixel(x,y).SetB(unsigned char(const_cast<CMatrixMultiband&>(mat)[2][y][x]));
			image.Pixel(x,y).SetA(unsigned char(const_cast<CMatrixMultiband&>(mat)[3][y][x]));
		}
	}
}

void ImageRGBAToMatrix3band(const CVisRGBAByteImage& image, CMatrixMultiband& mat)
{
	assert(mat.NBand() == 4);
	if ( mat.NBand() != 4 ) return;
	
	int width = image.Width();
	int height = image.Height();
	
	assert(width>0 && height>0);
	if ( width<=0 || height<=0 ) return;
	
	mat.Resize(height, width);

	for ( int y=0; y<height; y++ )
	{
		for ( int x=0; x<width; x++ )
		{
			mat[0][y][x] = image.Pixel(x,y).R();
			mat[1][y][x] = image.Pixel(x,y).G();
			mat[2][y][x] = image.Pixel(x,y).B();
			mat[3][y][x] = image.Pixel(x,y).A();
		}
	}
}

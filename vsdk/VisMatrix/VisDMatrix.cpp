///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisMatrix.cpp -- implementation of matrix/vector ops
//
// DESCRIPTION
//  The CVisDMatrix class provides some basic matrix operations, using calls
//  to external software (IMSL for the moment) to perform the more
//  complicated operations.
//
// SEE ALSO
//  CVisDMatrix.h        more complete description
//
// BUGS
//  I sure hope that the IMSL routines don't modify their input
//  parameters!
//
// DESIGN
//  Right now, we call out to IMSL to perform the linear systems
//  solving.  We could change this later.
//
//  To use IMSL, include the following in your 
//  Build Settings Link Object/Library modules:
//      mathd.lib libf.lib
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////


#include "VisMatrixPch.h"
#include "VisMatrixProj.h"


//  IMSL routines used (double precision)

#ifdef VIS_USE_IMSL
#include "..\VisXIMSL\VisXIMSL.h"
#else // VIS_USE_IMSL
static void VisGaussJordanSolveDMatrix(CVisDMatrix& A);
#endif // VIS_USE_IMSL

#include "..\VisXCLAPACK\VisXCLAPACK.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// LATER:  The vector and matrix arithmetic operators assume that both
// arguments are of the same size.  We could easily change many of these
// operators to do the right thing when the arguments are not the same
// size.  (For example, adding a 3-vector to a 4-vector.)


// LATER:  We could add additional storage to the vector and matrix classes
// so that additional memory would not need to be allocated for small
// vectors and matrices.

// LATER:  We might be able to get slightly better performance by changing
// some loops to count down to zero (so that the >= test is faster).
// We also might make things faster by using stack variables (eg rows)
// instead of member variables (eg m_nRows) in a few places.


bool gvis_fLookedForXCLAPACK = false;
CVisXCLAPACK *gvis_pxclapack = 0;


void VisLookForPxclapack(void)
{
	if (!gvis_fLookedForXCLAPACK)
	{
		gvis_pxclapack = VisFindPXCLAPACK();
		gvis_fLookedForXCLAPACK = true;
	}
}


inline CVisXCLAPACK *VisPxclapack(void)
{
	if (!gvis_fLookedForXCLAPACK)
	{
		VisLookForPxclapack();
	}

	return gvis_pxclapack;
}

//
// Memory allocation methods
//
VisMatrixExport void CVisDVector::FreeBuffer(void)
{
	assert((m_length > 0) || (m_data == 0));
	if ((m_data != 0) && (!m_fExternalStorage))
	{
#ifdef VIS_DVECTOR_CDBL_BUF
		if (m_data != m_rgdblBuf)
#endif // VIS_DVECTOR_CDBL_BUF
			delete m_data;
	}
}

VisMatrixExport void CVisDVector::SetSize(int length, double *storage)
{
	// This method is called by the constructors and the Resize method.
	// It assumes that the current buffer has already been freed.
	assert(length >= 0);
	m_length = length;
	
	// Get a new buffer.
	if (m_length > 0)
	{
		m_fExternalStorage = (storage != 0);
		if (!m_fExternalStorage)
		{
#ifdef VIS_DVECTOR_CDBL_BUF
			if (m_length <= VIS_DVECTOR_CDBL_BUF)
			{
				m_data = m_rgdblBuf;
				
				ZeroMemory(m_rgdblBuf, m_length * sizeof(double));
			}
			else
#endif // VIS_DVECTOR_CDBL_BUF
				m_data = new double[m_length];
				ZeroMemory(m_data, m_length * sizeof(double));
		}
		else
		{
			m_data = storage;
		}
	}
	else
	{
		m_data = 0;
		m_fExternalStorage = false;
	}
}

VisMatrixExport void CVisDVector::Resize(int length, double *storage)
{
	if ((m_length != length) || (storage != 0) || (m_fExternalStorage))
	{
		// Delete the old buffer, if needed.
		FreeBuffer();

		// Find the new buffer.
		SetSize(length, storage);
	}
}


VisMatrixExport void CVisDMatrix::FreeBuffer(void)
{
	assert(((m_nRows > 0) && (m_nCols > 0)) || (m_data == 0));
	if (m_data != 0)
	{
		assert(*m_data != 0);

#if defined(VIS_DMATRIX_NROWS_DBL_BUF) && defined(VIS_DMATRIX_NCOLS_DBL_BUF)
#error UNDONE:  Temporary buffer for use with CVisDMatrix
		if (m_data != m_rgdblBuf)
#endif // defined(VIS_DMATRIX_NROWS_DBL_BUF) && defined(VIS_DMATRIX_NCOLS_DBL_BUF)
		{
			if (!m_fExternalStorage)
				delete[] m_data[0];
		}

		delete[] m_data;
	}
}

VisMatrixExport void CVisDMatrix::SetSize(int rows, int cols, double *storage)
{
	// Typedef used with operator new below.
	typedef double *TVisPdbl;
	
	// This method is called by the constructors and the Resize method.
	// It assumes that the current buffer has already been freed.
	assert(rows >= 0);
	assert(cols >= 0);
	m_nRows = rows;
	m_nCols = cols;
	
	// Get a new buffer.
	// LATER:  We could try to be tricky here and use one memory allocation
	// to get the Iliffe vector and matrix data in a single memory block.
	// (We'd need to use type casts and calcualte the memory block size in bytes.)
	if ((rows > 0) && (cols > 0))
	{
#if defined(VIS_DMATRIX_NROWS_DBL_BUF) && defined(VIS_DMATRIX_NCOLS_DBL_BUF)
#error UNDONE:  Temporary buffer for use with CVisDMatrix Iliffe vector
#else // defined(VIS_DMATRIX_NROWS_DBL_BUF) && defined(VIS_DMATRIX_NCOLS_DBL_BUF)
		m_data = new TVisPdbl[rows];
#endif // defined(VIS_DMATRIX_NROWS_DBL_BUF) && defined(VIS_DMATRIX_NCOLS_DBL_BUF)
		double *pdbl;

		m_fExternalStorage = (storage != 0);
		if (!m_fExternalStorage)
		{
			int cdbl = rows * cols;
#if defined(VIS_DMATRIX_NROWS_DBL_BUF) && defined(VIS_DMATRIX_NCOLS_DBL_BUF)
#error UNDONE:  Temporary buffer for use with CVisDMatrix
			if ()
			{
				pdbl = m_rgdblBuf;
				
				ZeroMemory(m_rgdblBuf, cdbl * sizeof(double));
			}
			else
#endif // defined(VIS_DMATRIX_NROWS_DBL_BUF) && defined(VIS_DMATRIX_NCOLS_DBL_BUF)
				pdbl = new double[cdbl];
				ZeroMemory(pdbl, cdbl * sizeof(double));
		}
		else
		{
			pdbl = storage;
		}

		// Fill in the Iliffe vector.
		for (int iRow = 0; iRow < rows; ++iRow, pdbl += cols)
			m_data[iRow] = pdbl;
	}
	else
	{
		assert(m_nRows == 0);
		assert(m_nCols == 0);
		m_data = 0;
		m_fExternalStorage = false;
	}
}

VisMatrixExport void CVisDMatrix::Resize(int rows, int cols, double *storage)
{
	if ((m_nRows != rows) || (m_nCols != cols) || (storage != 0)
			|| (m_fExternalStorage))
	{
		// Delete the old buffer, if needed.
		FreeBuffer();

		// Find the new buffer.
		SetSize(rows, cols, storage);
	}
}

//
// Assignment
//

////////////////////////////////////////////////////////////////////////////
//  
//  FUNCTION:        operator=
//  
//  DECLARATION:
//          CVisDVector& CVisDVector::operator=(const CVisDVector &vec);
//  
//  RETURN VALUE:
//      vector being copied
//  INPUT:
//      &vec (const CVisDVector) - vector being copied
//  
//  DISCRIPTION:
//      assignment operator
//  
////////////////////////////////////////////////////////////////////////////
VisMatrixExport CVisDVector& CVisDVector::operator=(const CVisDVector &vec)
{
	// This "if" statement was added on Nov. 3, 1999 to allow templated
	// array classes to copy elements when resizing the array.
	if (Length() == 0)
		Resize(vec.Length());

    assert(Length() == vec.Length());

	if (m_length != 0)
	{
		assert(m_length > 0);
		memcpy(m_data, vec.m_data, m_length*sizeof(double));
	}

    return *this;
}

////////////////////////////////////////////////////////////////////////////
//  
//  FUNCTION:        operator=
//  
//  DECLARATION:
//          CVisDVector& CVisDVector::operator=(double value);
//  
//  RETURN VALUE:
//      reference to l.h.s.
//  INPUT:
//      value (double) - fill value
//  
//  DISCRIPTION:
//      Fill vector with constant value
//  
////////////////////////////////////////////////////////////////////////////
VisMatrixExport CVisDVector& CVisDVector::operator=(double value)
{
	if ((value == 0.0) && (m_length != 0))
	{
		assert(m_length > 0);

		// IEEE float
		memset(m_data, 0, m_length*sizeof(double));
	}
	else
	{
		for (int i = 0; i < m_length; i++)
			m_data[i] = value;
	}

	return *this;
}

////////////////////////////////////////////////////////////////////////////
//  
//  FUNCTION:        operator=
//  
//  DECLARATION:
//          CVisDMatrix& CVisDMatrix::operator=(const CVisDMatrix &mat);
//  
//  RETURN VALUE:
//      reference to l.h.s.
//  INPUT:
//      &mat (const CVisDMatrix) - matrix being copied
//  
//  DISCRIPTION:
//      Assignment operator
//  
////////////////////////////////////////////////////////////////////////////
VisMatrixExport CVisDMatrix& CVisDMatrix::operator=(const CVisDMatrix &mat)
{
	// This "if" statement was added on Nov. 3, 1999 to allow templated
	// array classes to copy elements when resizing the array.
	if (NRows() == 0 && NCols() == 0)
		Resize(mat.NRows(), mat.NCols());

	assert(NRows() == mat.NRows() && NCols() == mat.NCols());

	if (m_nRows != 0)
	{
		assert(m_nRows > 0);
		assert(m_nCols > 0);
		memcpy(m_data[0], mat.m_data[0], m_nRows*m_nCols*sizeof(double));
	}

	return *this;
}

////////////////////////////////////////////////////////////////////////////
//  
//  FUNCTION:        operator=
//  
//  DECLARATION:
//          CVisDMatrix& CVisDMatrix::operator=(double value);
//  
//  RETURN VALUE:
//      reference to l.h.s.
//  INPUT:
//      value (double) - fill value
//  
//  DISCRIPTION:
//      Assignment operator
//  
////////////////////////////////////////////////////////////////////////////
VisMatrixExport CVisDMatrix& CVisDMatrix::operator=(double value)
{
	if ((value == 0.0) && (m_nRows != 0))
	{
		assert(m_nRows > 0);
		assert(m_nCols > 0);

		// IEEE float
        memset(m_data[0], 0, m_nRows*m_nCols*sizeof(double));
	}
	else
	{
		int n = m_nRows * m_nCols;
		double *p = m_data[0];
		for (int i = 0; i < n; i++)
			p[i] = value;
	}

	return *this;
}

//
//  Comparison operators
//

VisMatrixExport bool CVisDVector::operator==(const CVisDVector& refvector) const
{
    if (Length() == refvector.Length())
	{
		if (Length() == 0)
			return true;

		if (memcmp(m_data, refvector.m_data, Length() * sizeof(double)) == 0)
			return true;
	}

    return false; 
}

VisMatrixExport bool CVisDMatrix::operator==(const CVisDMatrix& refmatrix) const
{
    if ((NRows() == refmatrix.NRows())
			&& (NCols() == refmatrix.NCols()))
	{
		if ((NRows() == 0) || (NCols() == 0))
			return true;

		int cbRow = NCols()  * sizeof(double);
		for (int iRow = 0; iRow < NRows(); ++iRow)
		{
			if (memcmp(m_data[iRow], refmatrix.m_data[iRow], cbRow) != 0)
				return false;
		}
		
		return true;
	}


    return false; 
}


VisMatrixExport bool CVisDVector::operator<(const CVisDVector& refvector) const
{
    if (Length() == refvector.Length())
	{
		if (Length() == 0)
			return false;

		return (memcmp(m_data, refvector.m_data,
				Length() * sizeof(double)) < 0);
	}

    return (Length() < refvector.Length()); 
}

VisMatrixExport bool CVisDMatrix::operator<(const CVisDMatrix& refmatrix) const
{
    if (NRows() == refmatrix.NRows())
	{
		if (NCols() == refmatrix.NCols())
		{
			if ((NRows() == 0) || (NCols() == 0))
				return false;

			int cbRow = NCols()  * sizeof(double);
			for (int iRow = 0; iRow < NRows(); ++iRow)
			{
				int wCmp = memcmp(m_data[iRow], refmatrix.m_data[iRow], cbRow);
				if (wCmp != 0)
					return (wCmp < 0);
			}
			
			return false;
		}
    
		return (NCols() < refmatrix.NCols()); 
	}

    return (NRows() < refmatrix.NRows()); 
}


//
//  Matrix / vector products, dot product
//

VisMatrixExport CVisDVector& CVisDVector::EqSum(const CVisDVector &refvectorA, const CVisDVector &refvectorB)
{
    int n = Length(); 
    assert(n == refvectorA.Length()); 
    assert(n == refvectorB.Length()); 

    for (int i=0; i<n; i++)
        (*this)[i] = refvectorA[i] + refvectorB[i];

    return *this; 
}

VisMatrixExport CVisDVector& CVisDVector::EqDiff(const CVisDVector &refvectorA, const CVisDVector &refvectorB)
{
    int n = Length(); 
    assert(n == refvectorA.Length()); 
    assert(n == refvectorB.Length()); 

    for (int i=0; i<n; i++)
        (*this)[i] = refvectorA[i] - refvectorB[i];

    return *this; 
}


VisMatrixExport CVisDVector& CVisDVector::EqProd(const CVisDMatrix &refmatrixA, const CVisDVector &refvectorB)
{
    int nRows = refmatrixA.NRows(); 
    int nCols = refmatrixA.NCols(); 
    assert(nRows == Length()); 
    assert(refmatrixA.NCols() == refvectorB.Length());

    for (int i = 0; i < nRows; i++)
	{
        double dblSum = 0.0;
        for (int j = 0; j < nCols; j++)
            dblSum += refmatrixA[i][j] * refvectorB[j];
        (*this)[i] = dblSum;
    }

	return (*this);
}


VisMatrixExport CVisDVector& CVisDVector::operator*=(double dbl)
{
    for (int i=0; i<Length(); i++)
        (*this)[i] *= dbl;

    return *this; 
}

VisMatrixExport CVisDVector& CVisDVector::operator/=(double dbl)
{
	assert(dbl != 0);

    for (int i=0; i<Length(); i++)
        (*this)[i] /= dbl;

    return *this; 
}

VisMatrixExport CVisDVector CVisDVector::operator-(void) const
{
    CVisDVector vectorRet(Length());

    for (int i=0; i < Length(); i++)
        vectorRet[i] = - (*this)[i];

    return vectorRet; 
}


VisMatrixExport double CVisDVector::operator*(const CVisDVector& refvector) const
{
    double sum = 0.0;
    assert(Length() == refvector.Length());

    for (int i = 0; i < Length(); i++)
        sum += (*this)[i] * refvector[i];

    return sum;
}


VisMatrixExport CVisDMatrix& CVisDMatrix::operator+=(const CVisDMatrix& refmatrix)
{
	assert((NRows() == refmatrix.NRows())
			&& (NCols() == refmatrix.NCols()));

    for (int i = 0; i < NRows(); i++)
        for (int j = 0; j < NCols(); j++)
            (*this)[i][j] += refmatrix[i][j];

    return *this;
}

VisMatrixExport CVisDMatrix& CVisDMatrix::operator-=(const CVisDMatrix& refmatrix)
{
	assert((NRows() == refmatrix.NRows())
			&& (NCols() == refmatrix.NCols()));

    for (int i = 0; i < NRows(); i++)
        for (int j = 0; j < NCols(); j++)
            (*this)[i][j] -= refmatrix[i][j];

    return *this;
}

VisMatrixExport CVisDMatrix& CVisDMatrix::operator*=(const CVisDMatrix& refmatrix)
{
	// LATER:  Add an optimization to avoid  allocating memory for small matrices.
	CVisDMatrix matrixT(*this);

    assert(matrixT.NCols() == refmatrix.NRows());

	if (refmatrix.NCols() != refmatrix.NRows())
		Resize(NRows(), refmatrix.NCols());

    for (int i = 0; i < matrixT.NRows(); i++)
	{
        for (int j = 0; j < refmatrix.NCols(); j++)
		{
            double sum = 0.0;
            for (int k = 0; k < NCols(); k++)
                sum += matrixT[i][k] * refmatrix[k][j];
            (*this)[i][j] = sum;
        }
    }

	return *this;
}

VisMatrixExport CVisDMatrix& CVisDMatrix::operator*=(double dbl)
{
    for (int i = 0; i < NRows(); i++)
        for (int j = 0; j < NCols(); j++)
            (*this)[i][j] *= dbl;

    return *this;
}

VisMatrixExport CVisDMatrix& CVisDMatrix::operator/=(double dbl)
{
	assert(dbl != 0);

    for (int i = 0; i < NRows(); i++)
        for (int j = 0; j < NCols(); j++)
            (*this)[i][j] /= dbl;

    return *this;
}

VisMatrixExport CVisDMatrix CVisDMatrix::operator-(void) const
{
    CVisDMatrix matrixRet(NRows(), NCols());

    for (int i = 0; i < NRows(); i++)
        for (int j = 0; j < NCols(); j++)
            matrixRet[i][j] = - (*this)[i][j];

    return matrixRet;
}

	
VisMatrixExport CVisDMatrix& CVisDMatrix::EqSum(const CVisDMatrix &refmatrixA, const CVisDMatrix &refmatrixB)
{
    int nRows = NRows(); 
    int nCols = NCols(); 
    assert(nRows == refmatrixA.NRows());
    assert(nCols == refmatrixA.NCols());
    assert(nRows == refmatrixB.NRows());
    assert(nCols == refmatrixB.NCols());

    for (int i = 0; i < nRows; i++)
        for (int j = 0; j < nCols; j++)
            (*this)[i][j] = refmatrixA[i][j] + refmatrixB[i][j];

    return (*this);
}

VisMatrixExport CVisDMatrix& CVisDMatrix::EqDiff(const CVisDMatrix &refmatrixA, const CVisDMatrix &refmatrixB)
{
    int nRows = NRows(); 
    int nCols = NCols(); 
    assert(nRows == refmatrixA.NRows());
    assert(nCols == refmatrixA.NCols());
    assert(nRows == refmatrixB.NRows());
    assert(nCols == refmatrixB.NCols());

    for (int i = 0; i < nRows; i++)
        for (int j = 0; j < nCols; j++)
            (*this)[i][j] = refmatrixA[i][j] - refmatrixB[i][j];

    return (*this);
}

VisMatrixExport CVisDMatrix& CVisDMatrix::EqProd(const CVisDMatrix &refmatrixA, const CVisDMatrix &refmatrixB)
{
    int nRows = NRows(); 
    int nCols = NCols(); 
    int nTerms = refmatrixA.NCols(); 
    assert(nRows == refmatrixA.NRows());
    assert(nTerms == refmatrixB.NRows());
    assert(refmatrixB.NCols() == nCols);

    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            double sum = 0.0;
            for (int k = 0; k < nTerms; k++)
                sum += refmatrixA[i][k] * refmatrixB[k][j];
            (*this)[i][j] = sum;
        }
    }

    return (*this);
}


//
//  Matrix inverse
//

VisMatrixExport CVisDMatrix& CVisDMatrix::Invert(void)
{
	assert ((NRows() == NCols()) && (NRows() > 0));

	CVisXCLAPACK *pxclapack = VisPxclapack();
	if (pxclapack != 0)
	{
		// With CLAPACK, we have the code in the Invert method.
		int info = pxclapack->Dgetrf(NRows(), (*this)[0], 0);
		if (info) 
			throw CVisError("Matrix not inversible",
                    eviserrorOpFailed, "CVisDMatrix::Inverted",
                    __FILE__, __LINE__);
	}
	else
	{
		// Without CLAPACK, we have the code in the Inverted method.
		*this = Inverted();
	}

	return *this;
}

VisMatrixExport CVisDMatrix CVisDMatrix::Inverted(void) const
{
	assert ((NRows() == NCols()) && (NRows() > 0));

	CVisXCLAPACK *pxclapack = VisPxclapack();
	if (pxclapack != 0)
	{
		// With CLAPACK, we have the code in the Invert method.
		return CVisDMatrix(*this).Invert();
	}
	else
	{
		// Without CLAPACK, we have the code in the Inverted method.
		CVisDMatrix matrixInverse(NRows(), NCols());
#ifdef VIS_USE_IMSL

		VisIMSL_dlinrg(NRows(), (*this)[0], NRows(),
				matrixInverse[0], NRows());

#else // VIS_USE_IMSL

		// Use Gauss-Jordan elimination
		int i, j, n = NRows();
		CVisDMatrix matrixT(n, 2*n);
		for (i = 0; i < n; i++)       // copy into a single system
			for (j = 0; j < n; j++)
				matrixT[i][j] = (*this)[i][j], matrixT[i][j+n] = (i == j);
		VisGaussJordanSolveDMatrix(matrixT);
		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++)
				matrixInverse[i][j] = matrixT[i][j+n];

#endif // VIS_USE_IMSL

		return matrixInverse;
	}
}

//
//  Matrix Determinant
//

VisMatrixExport double CVisDMatrix::Determinant(void) const
{
	assert ((NRows() == NCols()) && (NRows() > 0));

	CVisXCLAPACK *pxclapack = VisPxclapack();
	if (pxclapack != 0)
	{
		double det;
		CVisDMatrix matrixInverse(*this);

		int info = pxclapack->Dgetrf(NRows(), matrixInverse[0], &det);
		return det;
	}
	else
	{
		throw CVisError("Not implemented except for CLAPACK",
                    eviserrorOpFailed, "CVisDMatrix::Determinant",
                    __FILE__, __LINE__);
		return 0.0;
	}
}

VisMatrixExport double CVisDMatrix::Determinant(CVisDMatrix &matrixInverse) const
{
	assert ((NRows() == NCols()) && (NRows() > 0));

	CVisXCLAPACK *pxclapack = VisPxclapack();
	if (pxclapack != 0)
	{
		double det;
		matrixInverse = *this;

		int info = pxclapack->Dgetrf(NRows(), matrixInverse[0], &det);
		return det;
	}
	else
	{
		throw CVisError("Not implemented except for CLAPACK",
                    eviserrorOpFailed, "CVisDMatrix::Determinant",
                    __FILE__, __LINE__);
		return 0.0;
	}
}


VisMatrixExport CVisDMatrix CVisDMatrix::Transposed(void) const
{
    int i, j, n = NRows(), m = NCols();
    CVisDMatrix matrixTranspose(m, n);

    assert(m > 0 && n > 0);

    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            matrixTranspose[i][j] = (*this)[j][i];

	return matrixTranspose;
}

VisMatrixExport bool CVisDMatrix::IsSymmetric(void) const
{
    if (NRows() != NCols())
        return false;

    for (int i = 0; i < NRows(); i++)
        for (int j = i+1; j < NCols(); j++)
            if ((*this)[i][j] != (*this)[j][i])
                return false;

    return true;
}


//
// Global functions
//


VisMatrixExport void VISAPI VisSetToZero(CVisDVector& refvector)
{
#ifdef SAVE
	int iLim = refvector.Length();
	for (int i = 0; i < iLim; ++i)
		refvector[i] = 0;
#endif // SAVE

	if (refvector.Length() > 0)
		ZeroMemory(&(refvector[0]), refvector.Length() * sizeof(double));
}

VisMatrixExport void VISAPI VisSetToZero(CVisDMatrix& refmatrix)
{
	int nLength = refmatrix.NRows() * refmatrix.NCols();
	if (nLength > 0)
		ZeroMemory(refmatrix[0], nLength * sizeof(double));
}

VisMatrixExport void VISAPI VisSetToIdentity(CVisDMatrix& refmatrix)
{
	int iLim = min(refmatrix.NRows(), refmatrix.NCols());
	if (iLim > 0)
	{
		VisSetToZero(refmatrix);
		for (int i = 0; i < iLim; ++i)
			refmatrix[i][i] = 1.0;
	}
}



////////////////////////////////////////////////////////////////////////////
//  
//  FUNCTION:        VisDMatrixSqrtInverse
//  
//  DECLARATION:
//          CVisDMatrix VisDMatrixSqrtInverse(const CVisDMatrix& A);
//  
//  RETURN VALUE:
//      result matrix
//  INPUT:
//      A (const CVisDMatrix&) - input matrix
//  
//  DISCRIPTION:
//      Matrix square root inverse
//  
////////////////////////////////////////////////////////////////////////////
VisMatrixExport CVisDMatrix VISAPI VisDMatrixSqrtInverse(const CVisDMatrix& A)
{
    CVisDMatrix result(A.NRows(), A.NCols());
    VisDMatrixSqrtInverse(A, result);
    return result;
}

////////////////////////////////////////////////////////////////////////////
//  
//  FUNCTION:        VisDMatrixSqrtInverse
//  
//  DECLARATION:
//     void VisDMatrixSqrtInverse(const CVisDMatrix& A, CVisDMatrix &AsqrtInv);
//  
//  INPUT:
//      A (const CVisDMatrix&) - input matrix
//  
//      &AsqrtInv (CVisDMatrix) - result matrix
//  
//  DISCRIPTION:
//      Matrix t
//  
////////////////////////////////////////////////////////////////////////////
VisMatrixExport void VISAPI VisDMatrixSqrtInverse(const CVisDMatrix& A,
		CVisDMatrix &AsqrtInv)
{
    assert(A.NRows() == A.NCols() && A.NRows() > 0);
    assert(A.NRows() == AsqrtInv.NRows());
    assert(A.NCols() == AsqrtInv.NCols());

    int i, j, n = A.NRows();
    CVisDMatrix B(n, n);

	B = A.Transposed() * A;

    CVisDVector Eval(n);
    CVisDMatrix Evec(n,n);

	CVisXCLAPACK *pxclapack = VisPxclapack();
	if (pxclapack != 0)
	{
		VisDMatrixSymmetricEigen(B, Eval, Evec);

		for (i=0; i<n; i++){
			assert(Eval[i] > 0.0);
			Eval[i] = 1.0/sqrt(Eval[i]); // can be easily modified for sqrt
		}

		for (i = 0; i < n; i++){
			for (j = 0; j < n; j++){ 
				AsqrtInv[i][j] = 0.0; 
				for (int k=0; k<n; k++)
					AsqrtInv[i][j] += Eval[k] * Evec[i][k] * Evec[j][k];
			}
		}
	}
	else 
	{
#ifdef VIS_USE_IMSL

		VisIMSL_devcsf(n, B[0], n, &Eval[0], Evec[0], n);

#else // VIS_USE_IMSL

		// Not implemented!
		assert(0);
		throw CVisError("Inverse sqrt only works with IMSL for now",
						eviserrorOpFailed, "VisDMatrixSqrtInverse",
						"VisDMatrix.cpp", __LINE__);

#endif // VIS_USE_IMSL

		for (i=0; i<n; i++){
			assert(Eval[i] > 0.0);
			Eval[i] = 1.0/sqrt(Eval[i]); // can be easily modified for sqrt
		}

		for (i = 0; i < n; i++){
			for (j = 0; j < n; j++){
				AsqrtInv[i][j] = 0.0; 
				for (int k=0; k<n; k++)
					AsqrtInv[i][j] += Eval[k] * Evec[k][i] * Evec[k][j];
			}
		}
	}
}


//
//  Linear systems solution
//

////////////////////////////////////////////////////////////////////////////
//  
//  FUNCTION:        VisDMatrixSolve
//  
//  DECLARATION:
//          CVisDVector VisDMatrixSolve(const CVisDMatrix& A,
//                        const CVisDVector& b);
//  
//  RETURN VALUE:
//      result vector
//  INPUT:
//      A (const CVisDMatrix&) - input matrix (l.h.s. of linear system)
//                  
//  
//      b (const CVisDVector&) - input vector (r.h.s. of linear system)
//                  
//  
//  DISCRIPTION:
//      Linear system solution
//  
////////////////////////////////////////////////////////////////////////////
VisMatrixExport CVisDVector VISAPI VisDMatrixSolve(const CVisDMatrix& A,
		const CVisDVector& b)
{
    CVisDVector result(b.Length());
    VisDMatrixSolve(A, b, result);
    return result;
}

////////////////////////////////////////////////////////////////////////////
//  
//  FUNCTION:        VisDMatrixSolve
//  
//  DECLARATION:
//          void VisDMatrixSolve(const CVisDMatrix& A, const CVisDVector& b,
//                        CVisDVector& x);
//  
//  INPUT:
//      A (const CVisDMatrix&) - input matrix (l.h.s. of linear system)
//                  
//  
//      b (const CVisDVector&) - input vector (r.h.s. of linear system)
//                  
//  
//      x (CVisDVector&) - output vector (solution to linear system)
//                  
//  
//  DISCRIPTION:
//      Linear system solution
//  
////////////////////////////////////////////////////////////////////////////
VisMatrixExport void VISAPI VisDMatrixSolve(const CVisDMatrix& A,
		const CVisDVector& b, CVisDVector& x)
{
    assert(A.NRows() == A.NCols() && A.NRows() > 0);
    assert(A.NRows() == b.Length());

	CVisXCLAPACK *pxclapack = VisPxclapack();
	if (pxclapack != 0)
	{
		CVisDMatrix AT = A.Transposed();
		int info = pxclapack->Dgesv(A.NRows(), &AT[0][0], 1, &b[0], &x[0]);
		if (info) 
			throw CVisError("Cannot solve A x = b",
                    eviserrorOpFailed, "VisDMatrixSolve",
                    __FILE__, __LINE__);
	}
	else
	{
#ifdef VIS_USE_IMSL

		VisIMSL_dlslrg(A.NRows(), A[0], A.NRows(), &b[0], 2, &x[0]);

#else // VIS_USE_IMSL

		// Use Gauss-Jordan elimination
		int i, j, n = A.NRows();
		CVisDMatrix B(n, n+1);
		for (i = 0; i < n; i++) {       // copy into a single system
			for (j = 0; j < n; j++)
				B[i][j] = A[i][j];
			B[i][n] = b[i];
		}
		VisGaussJordanSolveDMatrix(B);
		for (i = 0; i < n; i++)
			x[i] = B[i][n];

#endif // VIS_USE_IMSL
	}
}

// Symmetric Positive Definite matrix A:

////////////////////////////////////////////////////////////////////////////
//  
//  FUNCTION:        VisDMatrixSolveSPD
//  
//  DECLARATION:
//          CVisDVector VisDMatrixSolveSPD(const CVisDMatrix& A,
//                        const CVisDVector& b);
//  
//  RETURN VALUE:
//      result of linear system solution
//  INPUT:
//      A (const CVisDMatrix&) - input matrix
//  
//      b (const CVisDVector&) - input vector (r.h.s. of linear system)
//                  
//  
//  DISCRIPTION:
//      Linear system solution of a symmetric positive definite matrix
//      
//  
////////////////////////////////////////////////////////////////////////////
VisMatrixExport CVisDVector VISAPI VisDMatrixSolveSPD(const CVisDMatrix& A,
		const CVisDVector& b)
{
    CVisDVector result(b.Length());
    VisDMatrixSolveSPD(A, b, result);
    return result;
}

////////////////////////////////////////////////////////////////////////////
//  
//  FUNCTION:        VisDMatrixSolveSPD
//  
//  DECLARATION:
//          void VisDMatrixSolveSPD(const CVisDMatrix& A, const CVisDVector& b,
//                      CVisDVector& x, int n);
//  
//  INPUT:
//      A (const CVisDMatrix&) - input matrix (l.h.s. of linear system)
//                  
//  
//      b (const CVisDVector&) - input vector (r.h.s. of linear system)
//                  
//  
//      x (CVisDVector&) - output vector (solution to linear system)
//                  
//  
//      n (int) - size of linear system (may be smaller than size of A)
//                  
//  
//  DISCRIPTION:
//      Linear system solution of a symmetric positive definite matrix
//      
//  
////////////////////////////////////////////////////////////////////////////
VisMatrixExport void VISAPI VisDMatrixSolveSPD(const CVisDMatrix& A,
		const CVisDVector& b, CVisDVector& x, int n)
{
    if (n < 1)      // optionally solve a sub-system (faster)
        n = A.NRows();
    assert(A.NRows() == A.NCols() && A.NRows() > 0);
    assert(A.NRows() == b.Length());

#if defined(_DEBUG) // && defined(VIS_USE_IMSL)

    // Before solving, check for all positive eigenvalues
    assert(A.IsSymmetric());
    CVisDVector eigenvalues(n);
    // VisIMSL_devlsf(n, A[0], A.NCols(), &eigenvalues[0]);
	if (n != A.NRows()) {
		CVisDMatrix tmp(n,n);
		A.Submatrix(n, n, tmp);
		VisDMatrixSymmetricEigen(tmp, eigenvalues);
	}
	else 
		VisDMatrixSymmetricEigen(A, eigenvalues);
    for (int i = 0; i < n; i++)
        assert(eigenvalues[i] > 0.0);

#endif // defined(_DEBUG) // && defined(VIS_USE_IMSL)

    // Now solve it
	CVisXCLAPACK *pxclapack = VisPxclapack();
	if (pxclapack != 0)
	{
		CVisDMatrix tmp(n,n);
		if (n != A.NRows())
			A.Submatrix(n, n, tmp);
		else 
			tmp = A;
		int info = pxclapack->Dposv(n, &tmp[0][0], 1, &b[0], &x[0]);
		if (info) 
			throw CVisError("Cannot solve A x = b with symmetric A",
                    eviserrorOpFailed, "VisDMatrixSolveSPD",
                    __FILE__, __LINE__);
	}
	else 
	{
#ifdef VIS_USE_IMSL

		x = 0.0;
		VisIMSL_dlslds(n, A[0], A.NCols(), &b[0], &x[0]);

#else // VIS_USE_IMSL

		// Use Gauss-Jordan elimination
		int i, j;
		CVisDMatrix B(n, n+1);
		for (i = 0; i < n; i++) {       // copy into a single system
			for (j = 0; j < n; j++)
				B[i][j] = A[i][j];
			B[i][n] = b[i];
		}
		VisGaussJordanSolveDMatrix(B);
		for (i = 0; i < n; i++)
			x[i] = B[i][n];

#endif // VIS_USE_IMSL
	}
}



////////////////////////////////////////////////////////////////////////////
//  
//  FUNCTION:        VisDMatrixLeastSquares
//  
//  DECLARATION:
//      CVisDMatrix VisDMatrixLeastSquares(const CVisDMatrix& A,
//                        const CVisDMatrix& B);
//      void VisDMatrixLeastSquares(const CVisDMatrix& A,
//                        const CVisDMatrix& B, CVisDMatrix& X);
//      CVisDVector VisDMatrixLeastSquares(const CVisDMatrix& A,
//                        const CVisDVectir& b);
//      void VisDMatrixLeastSquares(const CVisDMatrix& A,
//                        const CVisDVector& b, CVisDVector& x);
//  
//  INPUT:
//      A (const CVisDMatrix&) - input matrix (l.h.s. of linear system)
//                  
//      B (const CVisDMatrix&) - input matrix (r.h.s. of linear system)
//  
//      X (CVisDMatrix&) - output matrix (solution to linear system)
//
//      b (const CVisDVector&) - input vector (r.h.s. of linear system)
//                  
//      x (CVisDVector&) - output vector (solution to linear system)
//  
//  DISCRIPTION:
//      Least squares system solution using Householder transforms
//      (with IMSL), or using normal matrix solution (without)
//      
//  
////////////////////////////////////////////////////////////////////////////
VisMatrixExport CVisDMatrix VISAPI VisDMatrixLeastSquares(const CVisDMatrix& A,
		const CVisDMatrix& B)
{
    CVisDMatrix result(B.NRows(), B.NCols());
    VisDMatrixLeastSquares(A, B, result);
    return result;
}

VisMatrixExport void VISAPI VisDMatrixLeastSquares(const CVisDMatrix& A,
		const CVisDMatrix& B, CVisDMatrix& X)
{
    assert(A.NRows() == B.NRows() && A.NRows() > 0);

	CVisXCLAPACK *pxclapack = VisPxclapack();
	if (pxclapack != 0)
	{
		VisDMatrixLLS(A, B, X);
	}
	else
	{
#ifdef VIS_USE_IMSL

		// Transpose the matrices and form into one
		CVisDMatrix M(A.NCols() + B.NCols(), A.NRows());
		for (int i = 0; i < A.NRows(); i++) {
			for (int j = 0; j < A.NCols(); j++)
				M[j][i] = A[i][j];
			for (j = 0; j < B.NCols(); j++)
				M[j+A.NCols()][i] = B[i][j];
		}

		// Use the IMSL least squares solver (Householder QR decomposition)
		CVisDMatrix N(X.NCols(), X.NRows());
		VisIMSL_dlqrrv(A.NRows(), A.NCols(), B.NCols(), &M[0][0], A.NRows(),
					   &N[0][0], X.NRows());

		// Transpose the result
		for (i = 0; i < X.NRows(); i++) {
			for (int j = 0; j < X.NCols(); j++)
				X[i][j] = N[j][i];
		}

#else // VIS_USE_IMSL

		// Form the normal equations
		CVisDMatrix An(A.NCols(), A.NCols()), Bn(B.NCols(), A.NCols());
		An = 0.0, Bn = 0.0;
		for (int i = 0; i < A.NRows(); i++) {
			for (int j = 0; j < A.NCols(); j++) {
				for (int k = 0; k < A.NCols(); k++)
					An[j][k] += A[i][j] * A[i][k];
				for (k = 0; k < B.NCols(); k++)
					Bn[k][j] += A[i][j] * B[i][k];
			}
		}

		// Solve for each column of X independently
		CVisDVector b(X.NRows());
		for (int j = 0; j < X.NCols(); j++) {
			// Copy the input (rhs)
			for (int i = 0; i < X.NRows(); i++)
				b[i] = Bn[i][j];

			// Solve the system
			CVisDVector x = VisDMatrixSolveSPD(An, b);
        
			// Copy to the output (solution)
			for (i = 0; i < X.NRows(); i++)
				X[i][j] = x[i];
		}

#endif // VIS_USE_IMSL
	}
}

// Computes the minimum norm least squares solution to an over-
//    or under-determined system of linear equations A X=B,  using
//   the singular value decomposition of A.
// returns the effective rank of A
// added by zhang on May 15, 1998
VisMatrixExport int VISAPI VisDMatrixLLS(
	const CVisDMatrix& A,
	const CVisDMatrix& B,
	CVisDMatrix& X
	)
{
	CVisXCLAPACK *pxclapack = VisPxclapack();
	if (pxclapack != 0)
	{
		int m = A.NRows();
		int n = A.NCols();
		int nrhs = B.NCols();
		CVisDVector sVals((m < n) ? m : n);
		CVisDMatrix AT = A.Transposed();
		CVisDMatrix BT = B.Transposed();
		int rank;
		int info = pxclapack->Dgelss(m, n, &AT[0][0], nrhs, &BT[0][0], &sVals[0], 
			&rank, &X[0][0]);
		if (info) 
			throw CVisError("Cannot compute LLS A X = B",
                    eviserrorOpFailed, "VisDMatrixLLS",
                    __FILE__, __LINE__);
		return rank;
	}
	else 
	{
		throw CVisError("Only work with CLPACK",
						eviserrorOpFailed, "VisDMatrixLLS",
						__FILE__, __LINE__);
		return 0;
	}
}

VisMatrixExport CVisDVector VISAPI VisDMatrixLeastSquares(const CVisDMatrix& A,
		const CVisDVector& b)
{
    CVisDVector result(A.NCols());
    VisDMatrixLeastSquares(A, b, result);
    return result;
}

VisMatrixExport void VISAPI VisDMatrixLeastSquares(const CVisDMatrix& A,
		const CVisDVector& b, CVisDVector& x)
{
    CVisDMatrix B(b.Length(), 1, (double *) &b[0]);
    CVisDMatrix X(x.Length(), 1, &x[0]);
    VisDMatrixLeastSquares(A, B, X);
}

////////////////////////////////////////////////////////////////////////////
//  
//  FUNCTION:     VisGaussJordanSolveDMatrix   
//  
//  DECLARATION:
//          static void VisGaussJordanSolveDMatrix(CVisDMatrix& A)
//  
//  INPUT:
//    CVisDMatrix& A - a matrix of coefficients.
//  
//  DISCRIPTION:
//
// Gauss-Jordan elimination, no pivoting:  not very stable or accurate
//
//  
////////////////////////////////////////////////////////////////////////////


static void VisGaussJordanSolveDMatrix(CVisDMatrix& A)
{
    int n = A.NRows(), m = A.NCols(), i, j, k;
    
    // Reduce to triangular form
    for (i = 0; i < n; i++) {
        
        // Make sure diagonal entry is non-zero
        if (A[i][i] == 0.0) {
            for (j = i+1; j < n; j++)
                if (A[j][i] != 0.0)   // find non-zero entry
                    break;
            if (j >= n)      // if failed, matrix is singular
                throw CVisError("matrix is singular", eviserrorOpFailed,
                         "VisGaussJordanSolveDMatrix()", __FILE__, __LINE__);
            for (k = i; k < m; k++)
                A[i][k] += A[j][k];
        }

        // VisDMatrixScale this row to unity
        double aii_inv = 1.0 / A[i][i];
        A[i][i] = 1.0;
        for (k = i+1; k < m; k++)
            A[i][k] *= aii_inv;

        // Subtract from other rows
        for (j = i+1; j < n; j++) {
            double aij = A[j][i];
            A[j][i] = 0.0;
            for (k = i+1; k < m; k++)
                A[j][k] -= aij * A[i][k];
        }
    }

    // Back substitute
    for (i = n-1; i > 0; i--) {
        for (j = 0; j < i; j++)  {
            double aji = A[j][i];
            A[j][i] = 0.0;
            for (k = n; k < m; k++)
                A[j][k] -= aji * A[i][k];
        }
    }
}



// QR factorization related functions
VisMatrixExport CVisDVector VISAPI VisDMatrixSolveQR(const CVisDMatrix& A,
		const CVisDVector& b)
{
    int nr = A.NRows(); 
    int nc = A.NCols(); 
    assert(nr >= nc && nc > 0);
    assert(nr == b.Length());

    CVisDVector result(nc);
    VisDMatrixSolveQR(A, b, result);
    return result;
}

// IMSL (Fortune) takes column input for a matrix (unlike C which uses row
// indexing
// This is why transpose of A is used 
// unlike SPD solver, QR solver doesn't require A to be square
VisMatrixExport void VISAPI VisDMatrixSolveQR(const CVisDMatrix& A,
		const CVisDVector& b, CVisDVector& x)
{
    int nr = A.NRows(); 
    int nc = A.NCols(); 
    assert(nr >= nc && nc > 0);
    assert(nr == b.Length());
	assert(nc == x.Length());

    // Transpose the matrice
    CVisDMatrix M = A.Transposed(); 

	CVisXCLAPACK *pxclapack = VisPxclapack();
	if (pxclapack != 0)
	{
		CVisDVector tmp = b;
		int info = pxclapack->Dgels(nr, nc, &M[0][0], 1, &tmp[0], &x[0]);
		if (info) 
			throw CVisError("Cannot solve A x = b",
                    eviserrorOpFailed, "VisDMatrixSolveQR",
                    __FILE__, __LINE__);
	}
	else 
	{
		x = 0.0; 
		//VisIMSL_SolveQR(nr, nc, A[0], &b[0], &x[0]);
#ifdef VIS_USE_IMSL
	   VisIMSL_SolveQR(nr, nc, M[0], &b[0], &x[0]);
#else // VIS_USE_IMSL
	   throw CVisError("QR solver only works with IMSL for now",
			eviserrorOpFailed, "VisDMatrixSolveQR",
			"VisDMatrix.cpp", __LINE__);
#endif // VIS_USE_IMSL
	}
}

////////////////////////////////////////////////////////////////////////////
//  
//  FUNCTION:        VisDMatrixSVD
//  
//  DECLARATION:
//      void VisDMatrixSVD(const CVisDMatrix& A, CVisDVector& s,
//                        CVisDMatrix& U, CVisDMatrix& V,
//                        int compute_left = 1, compute_right = 1);
//  
//  INPUT:
//      A (const CVisDMatrix&) - input matrix
//                  
//      s (CVisDVector&) - singular values (output vector)
//
//      U (CVisDMatrix&) - left singular vectors
//  
//      V (CVisDMatrix&) - right singular vectors
//
//      compute_left (int) - compute the left singular vectors
//  
//      compute_right (int) - compute the right singular vectors
//  
//  DISCRIPTION:
//      Singular value decomposition (SVD), using IMSL routine.
//      The matrices returned are of the form A = U diag(s) V^T
//      with the columns of U and V being orthonormal.
//
//      If A in M x N, then s must be of length N,
//      U must be M x N (if computed), and V must be N x N (if computed).
//  
////////////////////////////////////////////////////////////////////////////

VisMatrixExport void VISAPI VisDMatrixSVD(const CVisDMatrix& A, CVisDVector& s, 
                  CVisDMatrix& U, CVisDMatrix& V,
                  int compute_left, int compute_right)
{
	CVisXCLAPACK *pxclapack = VisPxclapack();
	if (pxclapack != 0)
	{
		int nr = A.NRows(); 
		int nc = A.NCols();
		char jobu = compute_left ? 'A' : 'N';
		char jobv = compute_right ? 'A' : 'N';
		CVisDMatrix tmp = A;
		CVisDMatrix tmpU(compute_left ? nr : 1, compute_left ? nr : 1);
		int info = pxclapack->Dgesvd(nr, nc, &tmp[0][0], jobu, jobv, 
			&tmpU[0][0], &s[0], &V[0][0]);
		if (info)
			throw CVisError(
				"Singular value decomposition failed",
						eviserrorOpFailed, "VisDMatrixSVD",
						__FILE__, __LINE__);
		// fill the remaining s if necessary
		int i, j;
		for (i = nr; i < nc; i++)
			s[i] = 0.0;
		// copy matrix U if necessary
		if (compute_left) {
			int nc2 = __min(nr, nc);
			for (i = 0; i < nr; i++) 
				for (j = 0; j < nc2; j++)
					U[i][j] = tmpU[i][j];
			// if necessary, arbitrarily fill the matrix
			for (j = nr; j < nc; j++) 
				for (i = 0; i < nr; i++)
					U[i][j] = (nc - j == i); 
		}
	}
	else
	{
#ifdef VIS_USE_IMSL
		long nr = A.NRows(); 
		long nc = A.NCols();
		CVisDVector s2(__min(nr+1, nc));    // required by LSVRR
		CVisDMatrix AT(nc, nr);             // A^T
		CVisDMatrix UT(compute_left ? nc : 1, compute_left ? nr : 1);
		CVisDMatrix VT(compute_right ? nc : 1, compute_right ? nc : 1);
		double eps = 0.0;       // ignored
		long rank_a;             // ignored
		long iPath = compute_left * 20 + compute_right;

		// Perform the transpose
		for (int r = 0; r < nr; r++)
			for (int c = 0; c < nc; c++)
				AT[c][r] = A[r][c];

		VisIMSL_dlsvrr(nr, nc, &AT[0][0], nr, iPath, eps, rank_a,
					   &s2[0], &UT[0][0], nr, &VT[0][0], nc);

		// Copy the singular values
		memcpy(&s[0], &s2[0], __min(s2.Length(), s.Length()) * sizeof(double));
		// cout << "U^T = " << UT << "s = " << s << "V^T = " << VT;

		// Perform the transpose
		if (compute_left) {
			U = 0.0;    // in case nr < nc
			int nc2 = __min(nr, nc);
			for (int r = 0; r < nr; r++)
				for (int c = 0; c < nc2; c++)
					U[r][c] = UT[c][r];
		}
		if (compute_right) {
			for (int r = 0; r < nc; r++)
				for (int c = 0; c < nc; c++)
					V[r][c] = VT[c][r];
		}
#else // VIS_USE_IMSL
		throw CVisError(
				"Singular value decomposition only works with IMSL for now",
						eviserrorOpFailed, "VisDMatrixSVD",
						"VisDMatrix.cpp", __LINE__);
#endif // VIS_USE_IMSL
	}
}


VisMatrixExport void VISAPI VisMinEigenValue(CVisDMatrix& A, CVisDVector& x)
{
#ifdef OLDANDSLOW
	int N = A.NRows();

	CVisDVector Eval(N);
	CVisDMatrix Evec(N, N);

	VisIMSL_devcsf(N, A[0], N, &Eval[0], Evec[0], N);
	for (int i = 0; i < N; i++)
		x[i] = Evec[N-1][i]; 
#else // OLDANDSLOW
    double emin = VisMinMaxEigenValue(A, x, true); 
#endif // OLDANDSLOW
}

VisMatrixExport void VISAPI VisMaxEigenValue(CVisDMatrix& A, CVisDVector& x)
{
#ifdef OLDANDSLOW
	int N = A.NRows();

	CVisDVector Eval(N);
	CVisDMatrix Evec(N, N);

	VisIMSL_devcsf(N, A[0], N, &Eval[0], Evec[0], N);
	for (int i = 0; i < N; i++)
		x[i] = Evec[0][i]; 
#else // OLDANDSLOW
    double emax = VisMinMaxEigenValue(A, x, false); 
#endif // OLDANDSLOW
}

VisMatrixExport double VISAPI VisMinMaxEigenValue(CVisDMatrix& A, CVisDVector& x,
		bool fMin)
{
	int n = A.NRows();
	double e = 0.0; 

	assert(x.Length() == n); 

	CVisXCLAPACK *pxclapack = VisPxclapack();
	if (pxclapack != 0)
	{
		CVisDVector vals(1);
		CVisDMatrix vects(n,1);
		int indLow = fMin ? 1 : n;
		int indUp = indLow;
		VisDMatrixSymmetricEigen(A, vals, vects, indLow, indUp);
		e = vals[0];
		for (int i = 0; i < n; i++)
			x[i] = vects[i][0];
	}
	else 
	{
#ifdef VIS_USE_IMSL
		VisIMSL_devesf(n, 1, A[0], n, (fMin ? 1 : 0), &e, &x[0], n);
#else // VIS_USE_IMSL
		throw CVisError("Eigenvalues only work with IMSL for now",
						eviserrorOpFailed, "VisMinMaxEigenValue",
						"VisDMatrix.cpp", __LINE__);
#endif // VIS_USE_IMSL
	}

	return e; 
}

// compute all eigenvalues and eigenvectors of a real symmetric matrix
// the eigenvalues are in ascending order
// added by zhang on 13 may 1998
VisMatrixExport void VISAPI VisDMatrixSymmetricEigen(const CVisDMatrix &A,
		CVisDVector &vals, CVisDMatrix &vects)
{
	CVisXCLAPACK *pxclapack = VisPxclapack();
	if (pxclapack != 0)
	{
		CVisDMatrix tmp = A;
		int M = A.NRows();
		assert(M == A.NCols());
		assert(vals.Length() == M);
		assert(vects.NRows() == M);
		assert(vects.NCols() == M);
		int info = pxclapack->Dsyev(M, &tmp[0][0], &vals[0], &vects[0][0]);
		if (info) 
			throw CVisError("Cannot compute eigenvalues and eigenvectors",
                    eviserrorOpFailed, "VisDMatrixSymmetricEigen",
                    __FILE__, __LINE__);
	}
	else
		throw CVisError("Eigenvalues only work with CLAPACK for now",
                    eviserrorOpFailed, "VisDMatrixSymmetricEigen",
                    __FILE__, __LINE__);
}

// compute only the eigenvalues of a real symmetric matrix
// the eigenvalues are in ascending order
// added by zhang on 13 may 1998
VisMatrixExport void VISAPI VisDMatrixSymmetricEigen(const CVisDMatrix &A,
		CVisDVector &vals)
{
	CVisXCLAPACK *pxclapack = VisPxclapack();
	if (pxclapack != 0)
	{
		CVisDMatrix tmp = A;
		int M = A.NRows();
		assert(M == A.NCols());
		assert(vals.Length() == M);
		int info = pxclapack->Dsyev(M, &tmp[0][0], &vals[0], 0);
		if (info) 
			throw CVisError("Cannot compute eigenvalues and eigenvectors",
                    eviserrorOpFailed, "VisDMatrixSymmetricEigen",
                    __FILE__, __LINE__);
	}
	else
		throw CVisError("Eigenvalues only work with CLAPACK for now",
                    eviserrorOpFailed, "VisDMatrixSymmetricEigen",
                    __FILE__, __LINE__);
}

// compute the selected eigenvalues, and optionally (if 'vects' != 0), eigenvectors, 
//     of a real symmetric matrix
// they are selected by specifying the range of indices: 1 <= indLow <= indUp <= m
// the eigenvalues are in ascending order
// 'vals' must be of: indUp - indLow + 1
// 'vects' must be of: m x (indUp - indLow + 1)
// added by zhang on 13 may 1998
VisMatrixExport void VISAPI VisDMatrixSymmetricEigen(const CVisDMatrix &A,
	CVisDVector &vals, CVisDMatrix &vects, const int indLow, const indUp)
{
	CVisXCLAPACK *pxclapack = VisPxclapack();
	if (pxclapack != 0)
	{
		CVisDMatrix tmp = A;
		int M = A.NRows();
		assert(M == A.NCols());
		int num = indUp - indLow + 1;
		assert(vals.Length() == num);
		assert(vects.NRows() == M);
		assert(vects.NCols() == num);
		int info = pxclapack->Dsyevx(M, &tmp[0][0], &vals[0], &vects[0][0], 
			indLow, indUp);
		if (info) 
			throw CVisError("Cannot compute eigenvalues and eigenvectors",
                    eviserrorOpFailed, "VisDMatrixSymmetricEigen",
                    __FILE__, __LINE__);
	}
	else
		throw CVisError("Eigenvalues only work with CLAPACK for now",
                    eviserrorOpFailed, "VisDMatrixSymmetricEigen",
                    __FILE__, __LINE__);
}

VisMatrixExport void VISAPI VisDMatrixSymmetricEigen(const CVisDMatrix &A,
	CVisDVector &vals, const int indLow, const indUp)
{
	CVisXCLAPACK *pxclapack = VisPxclapack();
	if (pxclapack != 0)
	{
		CVisDMatrix tmp = A;
		int M = A.NRows();
		assert(M == A.NCols());
		int num = indUp - indLow + 1;
		assert(vals.Length() == num);
		int info = pxclapack->Dsyevx(M, &tmp[0][0], &vals[0], 0, indLow, indUp);
		if (info) 
			throw CVisError("Cannot compute eigenvalues and eigenvectors",
                    eviserrorOpFailed, "VisDMatrixSymmetricEigen",
                    __FILE__, __LINE__);
	}
	else
		throw CVisError("Eigenvalues only work with CLAPACK for now",
                    eviserrorOpFailed, "VisDMatrixSymmetricEigen",
                    __FILE__, __LINE__);
}

// compute for an N-by-N real  nonsymmetric  matrix  A,
//     the  eigenvalues  and,  optionally,  the  left  and/or right
//     eigenvectors
//   The right eigenvector v(j) of A satisfies
//                      A * v(j) = lambda(j) * v(j)
//     where lambda(j) is its eigenvalue.
//     The left eigenvector u(j) of A satisfies
//                   u(j)**H * A = lambda(j) * u(j)**H
//     where u(j)**H denotes the conjugate transpose of u(j).
//
//     The computed eigenvectors are normalized to  have  Euclidean
//     norm equal to 1 and largest component real.
//  If the j-th eigenvalue is real, then u(j) = vectsL(:,j),   
//     the j-th column of vectsL.   
//  If the j-th and (j+1)-st eigenvalues form a complex   
//     conjugate pair, then u(j) = vectsL(:,j) + i*vectsL(:,j+1) and   
//      u(j+1) = vectsL(:,j) - i*vectsL(:,j+1). 
//  Similarly for the relationship between v(j) and vectsR.  
//
// compute the eigenvalues and eigenvectors of a generic square matrix
// both left and right eigenvectors
// added by zhang on Jan 11, 1999
VisMatrixExport void VISAPI VisDMatrixEigen(
	const CVisDMatrix &A, // original matrix
	CVisDMatrix &vectsL,  // matrix of the left eigenvectors
	CVisDVector &valsReal, // real part of the eigenvalues
	CVisDVector &valsImaginary, // imaginary part of the eigenvalues
	CVisDMatrix &vectsR // matrix of the right eigenvectors
)
{
	CVisXCLAPACK *pxclapack = VisPxclapack();
	if (pxclapack != 0)
	{
		CVisDMatrix tmp = A.Transposed();
		int M = A.NRows();
		assert(M == A.NCols());
		valsReal.Resize(M);
		valsImaginary.Resize(M);
		vectsL.Resize(M, M);
		vectsR.Resize(M, M);
		CVisDMatrix vectsLT(M,M);
		CVisDMatrix vectsRT(M,M);
		int info = pxclapack->Dgeev(M, &tmp[0][0], &valsReal[0], &valsImaginary[0], 
			&vectsLT[0][0], &vectsRT[0][0]);
		vectsL = vectsLT.Transposed();
		vectsR = vectsRT.Transposed();
		if (info) 
			throw CVisError("Cannot compute eigenvalues and eigenvectors",
                    eviserrorOpFailed, "VisDMatrixEigen",
                    __FILE__, __LINE__);
	}
	else
		throw CVisError("Eigenvalues only work with CLAPACK for now",
                    eviserrorOpFailed, "VisDMatrixEigen",
                    __FILE__, __LINE__);
}

// compute the eigenvalues and right eigenvectors of a generic square matrix
// no left eigenvectors
// added by zhang on Jan 11, 1999
VisMatrixExport void VISAPI VisDMatrixEigen(
	const CVisDMatrix &A, // original matrix
	CVisDVector &valsReal, // real part of the eigenvalues
	CVisDVector &valsImaginary, // imaginary part of the eigenvalues
	CVisDMatrix &vectsR // matrix of the right eigenvectors
)
{
	CVisXCLAPACK *pxclapack = VisPxclapack();
	if (pxclapack != 0)
	{
		CVisDMatrix tmp = A.Transposed();
		int M = A.NRows();
		assert(M == A.NCols());
		valsReal.Resize(M);
		valsImaginary.Resize(M);
		vectsR.Resize(M, M);
		CVisDMatrix vectsRT(M,M);
		int info = pxclapack->Dgeev(M, &tmp[0][0], &valsReal[0], &valsImaginary[0], 
			0, &vectsRT[0][0]);
		vectsR = vectsRT.Transposed();
		if (info) 
			throw CVisError("Cannot compute eigenvalues and eigenvectors",
                    eviserrorOpFailed, "VisDMatrixEigen",
                    __FILE__, __LINE__);
	}
	else
		throw CVisError("Eigenvalues only work with CLAPACK for now",
                    eviserrorOpFailed, "VisDMatrixEigen",
                    __FILE__, __LINE__);
}

// compute the eigenvalues and left eigenvectors of a generic square matrix
// no right eigenvectors
// added by zhang on Jan 11, 1999
VisMatrixExport void VISAPI VisDMatrixEigen(
	const CVisDMatrix &A, // original matrix
	CVisDMatrix &vectsL,  // matrix of the left eigenvectors
	CVisDVector &valsReal, // real part of the eigenvalues
	CVisDVector &valsImaginary // imaginary part of the eigenvalues
)
{
	CVisXCLAPACK *pxclapack = VisPxclapack();
	if (pxclapack != 0)
	{
		CVisDMatrix tmp = A.Transposed();
		int M = A.NRows();
		assert(M == A.NCols());
		valsReal.Resize(M);
		valsImaginary.Resize(M);
		vectsL.Resize(M, M);
		CVisDMatrix vectsLT(M,M);
		int info = pxclapack->Dgeev(M, &tmp[0][0], &valsReal[0], &valsImaginary[0], 
			&vectsLT[0][0], 0);
		vectsL = vectsLT.Transposed();
		if (info) 
			throw CVisError("Cannot compute eigenvalues and eigenvectors",
                    eviserrorOpFailed, "VisDMatrixEigen",
                    __FILE__, __LINE__);
	}
	else
		throw CVisError("Eigenvalues only work with CLAPACK for now",
                    eviserrorOpFailed, "VisDMatrixEigen",
                    __FILE__, __LINE__);
}

// compute the eigenvalues of a generic square matrix
// only eigenvalues
// added by zhang on Jan 11, 1999
VisMatrixExport void VISAPI VisDMatrixEigen(
	const CVisDMatrix &A, // original matrix
	CVisDVector &valsReal, // real part of the eigenvalues
	CVisDVector &valsImaginary // imaginary part of the eigenvalues
)
{
	CVisXCLAPACK *pxclapack = VisPxclapack();
	if (pxclapack != 0)
	{
		CVisDMatrix tmp = A.Transposed();
		int M = A.NRows();
		assert(M == A.NCols());
		valsReal.Resize(M);
		valsImaginary.Resize(M);
		int info = pxclapack->Dgeev(M, &tmp[0][0], &valsReal[0], &valsImaginary[0], 
			0, 0);
		if (info) 
			throw CVisError("Cannot compute eigenvalues and eigenvectors",
                    eviserrorOpFailed, "VisDMatrixEigen",
                    __FILE__, __LINE__);
	}
	else
		throw CVisError("Eigenvalues only work with CLAPACK for now",
                    eviserrorOpFailed, "VisDMatrixEigen",
                    __FILE__, __LINE__);
}




// implementation of Equality constrained Least-Squares on p.585 Golub and
// Van Loan also see MSR-TR-97-23
// current implementation assumes rank(A) = n and rank(B) = p -> user's
// responsibility
VisMatrixExport void VISAPI VisDMatrixEQConstrainedLS(CVisDMatrix& A, CVisDVector& b,
		CVisDMatrix& C, CVisDVector& d, CVisDVector& x)
{
    int m = A.NRows(); 
    int n = A.NCols(); 
    int p = C.NRows(); 

    assert(b.Length() == m);
    assert(x.Length() == n); 
    assert(d.Length() == p); 

	CVisXCLAPACK *pxclapack = VisPxclapack();
	if (pxclapack != 0)
	{
		CVisDMatrix AT = A.Transposed();
		CVisDMatrix CT = C.Transposed();
		CVisDVector bTmp = b;
		CVisDVector dTmp = d;
		int info = pxclapack->Dgglse(m, n, p, &AT[0][0], &CT[0][0], &b[0], &d[0], 
			&x[0]);
		if (info) 
			throw CVisError("Cannot solve equality-constrained least squares",
                    eviserrorOpFailed, "VisDMatrixEQConstrainedLS",
                    __FILE__, __LINE__);
	}
	else
	{
#ifdef VIS_USE_IMSL
		CVisDVector x1(p); 
		CVisDVector x2(n-p); 

		x1 = 0.0; x2 = 0.0; 

		// QR factorization of C^T, i.e., C^T = Q [R | 0]^T
		// CVisDMatrix QR(n, p); 
		CVisDMatrix Q(n, n); 
		CVisDMatrix RT(p, p); 
		long* ipvt = new long[p]; 
		long pivot = 1; 

		CVisDMatrix QRTrans(p, n); 
		CVisDMatrix QTrans(n, n); 

		VisIMSL_GetQR(n, p, C[0], QTrans[0], QRTrans[0], ipvt, pivot); 

		// copy RT from QRTrans
		// Not necessary to create RT, but for clarity
		int i, j; 
		for (i = 0; i < p; i++) 
		{
			for (j = 0; j < p; j++)
			{
				RT[i][j] = QRTrans[i][j];
			}
		}

		Q = QTrans.Transposed();

		// permute d by P^{-1} d = dPermu
		// this step is necessary: see RoomBuilder paper
		// double* dPermu = new double[p]; 
		CVisDVector dPermu(p); 
		VisIMSL_DPERMU(p, &d[0], ipvt, 1, &dPermu[0]);

		// AQ = [A1, A2]
		CVisDMatrix AQ = A * Q; 
		CVisDMatrix A1(m, p); 
		CVisDMatrix A2(m, n-p); 

		for (i = 0; i < m; i++)
		{
			for (j = 0; j < p; j++)
			{
				A1[i][j] = AQ[i][j]; 
			}
			for (j = p; j < n; j++)
			{
				A2[i][j-p] = AQ[i][j]; 
			}
		}

		// solve x1; RT is a lower triangular matrix
		// or we can call IMSL to solve it
		for (i = 0; i < p; i++)
		{
			double sum = 0; 
			for (j = 0; j < i; j++)
			{
				sum += RT[i][j] * x1[j]; 
			}
			x1[i] = (dPermu[i] - sum)/RT[i][i]; 
		}

		// solve x2; unconstrained LS
		CVisDVector b1 = A1 * x1; 
		CVisDVector b2 = b - b1; 
		VisDMatrixLeastSquares(A2, b2, x2); 

		// get x
		CVisDVector z(n); 
		for (j = 0; j < p; j++)
			z[j] = x1[j];
		for (j = p; j < n; j++)
			z[j] = x2[j-p]; 

		x = Q * z; 
    
		delete ipvt; 
		//delete dPermu; 
#else // VIS_USE_IMSL
		throw CVisError("Constrained least squares only works with IMSL for now",
						eviserrorOpFailed, "VisDMatrixEQConstrainedLS",
						"VisDMatrix.cpp", __LINE__);
#endif // VIS_USE_IMSL
	}
}


//
//  Standard library Input/output (for debugging, mostly)
//

VisMatrixExport bool CVisDVector::s_fVerboseOutput = true;
VisMatrixExport bool CVisDMatrix::s_fVerboseOutput = true;


////////////////////////////////////////////////////////////////////////////
//  
//  FUNCTION:        operator<<
//  
//  DECLARATION:
//          ostream& operator<<(ostream& os, const CVisDVector& v);
//  
//  RETURN VALUE:
//      reference to output stream
//  INPUT:
//      os (ostream&) - output stream
//  
//      v (CVisDVector&) - vector being printed
//  
//  DISCRIPTION:
//      Print vector on output stream
//  
////////////////////////////////////////////////////////////////////////////
VisMatrixExport std::ostream& VISAPI operator<<(std::ostream& os, const CVisDVector& v)
{
    if (CVisDVector::s_fVerboseOutput)
        os << "# CVisDVector<" << v.Length() << "> = {";
    else
        os << "{";
    for (int i = 0; i < v.Length(); i++)
        os << v[i] << ((i == v.Length()-1) ? "}" : ", ");
    if (CVisDVector::s_fVerboseOutput)
        os << std::endl;

    return os;
}

////////////////////////////////////////////////////////////////////////////
//  
//  FUNCTION:        operator<<
//  
//  DECLARATION:
//          ostream& operator<<(ostream& os, const CVisDMatrix& mat );
//  
//  RETURN VALUE:
//      reference to output stream
//  INPUT:
//      os (ostream&) - output stream
//  
//      v (CVisDVector&) - matrix being printed
//  
//  DISCRIPTION:
//      Print matrix on output stream
//  
////////////////////////////////////////////////////////////////////////////

VisMatrixExport std::ostream& VISAPI operator<<(std::ostream& os, const CVisDMatrix& mat)
{
    if (CVisDMatrix::s_fVerboseOutput)
        os << "# CVisDMatrix<" << mat.NRows() << "," << 
                              mat.NCols() << "> =" << std::endl;
    for (int i = 0; i < mat.NRows(); i++) {
        os << ((i == 0) ? "{{" :  " {");
        for (int j = 0; j < mat.NCols(); j++)
            os << mat[i][j] << ((j == mat.NCols()-1) ? "}" : ", ");
        os << ((i == mat.NRows()-1) ? "}" : ",\n");
    }
    if (CVisDMatrix::s_fVerboseOutput)
        os << std::endl;

    return os;
}



//
//  Self-describing stream Input/output
//
// LATER:  We might want to add newlines and indents to format this better.
//


// Global variables used with self-describing streams.
// LATER:  Find a way to avoid using these global variables.
/*
#ifdef VIS_INCLUDE_SDSTREAM
VisMatrixExport CVisSDObject<CVisDVector> g_visdvectorExemplar;
VisMatrixExport CVisSDObject<CVisDMatrix> g_visdmatrixExemplar;
#endif
*/
/*
const char *CVisDVector::ReadWriteField(CVisSDStream& s, int field_id)
{
#ifdef VIS_INCLUDE_SDSTREAM
    switch (FieldType(field_id))
	{
    case eftName:
        return "class CVisDVector {";

    case eftLength:
		if (s.Status() == CVisSDStream::Read)
		{
			int lengthNew = m_length;
			s << lengthNew;
			assert(lengthNew >= 0);
			if (lengthNew != m_length)
				Resize(lengthNew);
		}
		else
		{
			s << m_length;
		}
		return "int m_length;             // number of elements";

    case eftData:
		s.OpenParen();
		for (int i = 0; i < m_length; i++)
		{
			s << m_data[i];
			if (i < m_length - 1)
				s.Comma();
		}
		s.CloseParen();
        return "double m_data[m_length];  // elements";
    }

    return 0;
#else
	assert(false);  // SDStreams are not included
	return 0;
#endif
}
*/
/*
const char *CVisDMatrix::ReadWriteField(CVisSDStream& s, int field_id)
{
#ifdef VIS_INCLUDE_SDSTREAM
    switch (FieldType(field_id))
	{
    case eftName:
        return "class CVisDMatrix {";

    case eftDims:
		s.OpenParen();
		if (s.Status() == CVisSDStream::Read)
		{
			int nRowsNew = m_nRows;
			int nColsNew = m_nCols;
			s << nRowsNew;
			s.Comma();
			s << nColsNew;
			assert((nRowsNew >= 0) && (nColsNew >= 0));
			if ((nRowsNew != m_nRows) || (nColsNew != m_nCols))
				Resize(nRowsNew, nColsNew);
		}
		else
		{
			s << m_nRows;
			s.Comma();
			s << m_nCols;
		}
		s.CloseParen();
		return "int m_nRows, m_nCols;     // dimensions";

    case eftData:
		s.NewLine();
		s.OpenParen();
		for (int r = 0; r < m_nRows; r++) {
			s.OpenParen();
			for (int c = 0; c < m_nCols; c++) {
				s << (*this)[r][c];
				if (c < m_nCols - 1)
					s.Comma();
			}
			s.CloseParen();
			if (r < m_nRows - 1)
				s.Comma(), s.NewLine();
		}
		s.CloseParen();
		s.NewLine();
        return "double m_data[m_nRows][m_nCols];  // elements";
    }

    return 0;
#else
	assert(false);  // SDStreams are not included
	return 0;
#endif
}
*/

// extract the submatrix of `newRows'x`newCols' from (i0,j0), 
//     and put it to `submat' whose reference is also returned
// added by zhang on June 9, 1998
CVisDMatrix& CVisDMatrix::Submatrix(
	const int newRows, 
	const int newCols, 
	CVisDMatrix& submat,
	const int i0, 
	const int j0
	) const
{
	assert(newRows > 0);
    assert(newCols > 0);
	assert(i0 >= 0);
	assert(j0 >= 0);
    assert(i0 + newRows <= NRows());
    assert(j0 + newCols <= NCols());
    submat.Resize(newRows, newCols);
    // this procedure assumes that a matrix is stored in rows 
    for (int i = 0; i < newRows; i++)
		memcpy(&submat[i][0], &(*this)[i0+i][j0], newCols*sizeof(double));
    return submat;
}

// return the r-th row
// added by zhang on Aug. 14, 1998
CVisDVector CVisDMatrix::Row(int r) const
{
	assert(r < NRows());
	assert(r >= 0);
	CVisDVector row(NCols());
	// this procedure assumes that a matrix is stored in rows
	memcpy(&row[0], &(*this)[r][0], NCols()*sizeof(double));
	return row;
}

// return the c-th column
// added by zhang on Aug. 14, 1998
CVisDVector CVisDMatrix::Column(int c) const
{
	assert(c < NCols());
	assert(c >= 0);
	CVisDVector col(NRows());
	for (int i = 0; i < NRows(); i++)
		col[i] = (*this)[i][c];
	return col;
}

// set the r-th row
// added by zhang on Feb. 2, 1999
void CVisDMatrix::SetRow(int r, const CVisDVector &v)
{
	int N = NCols();
	if (N > v.Length())
		N = v.Length();
    // this procedure assumes that a matrix is stored in rows
	memcpy(&(*this)[r][0], &v[0], N*sizeof(double));
}

// set the c-th column
// added by zhang on Feb. 2, 1999
void CVisDMatrix::SetColumn(int c, const CVisDVector &v)
{
	int N = NRows();
	if (N > v.Length())
		N = v.Length();
	for (int i = 0; i < N; i++)
		(*this)[i][c] = v[i];
}


// outer product of two vectors
// added by zhang on Sept. 4, 1998
VisMatrixExport CVisDMatrix VISAPI VisOuterProduct(const CVisDVector &v1, const CVisDVector &v2)
{
	int M = v1.Length();
    int N = v2.Length();
    CVisDMatrix A(M, N);
	for (int j = 0; j < N; j++)
	    for (int i = 0; i < M; i++)
			A[i][j] = v1[i] * v2[j];
    return A;
}

// cross product of two vectors
// added by zhang on 30 sept. 1998
VisMatrixExport void VISAPI VisCrossProduct(
	const CVisDVector &x,
	const CVisDVector &y,
	CVisDVector &z)
{
    int N = x.Length();
    if (N < 2 || N != y.Length())
		CVisError("CrossProduct only works for two vectors of same dimension (>= 2)",
                    eviserrorOpFailed, "VisCrossProduct",
                    __FILE__, __LINE__);
  
    int size = N * (N - 1) / 2 - 1;

    z.Resize(size+1);

    int inc = 0;
    double cross;
    for (int i = size; i > 0; i--)
        for (int j = (i - 1); j >= 0; j--) {
		    cross = x[i] * y[j] - y[i] * x[j];
            z[inc] = (((i-j)%2)==0) ? cross : -cross;
            inc++;
        }
}


VisMatrixExport void CVisDVector::ReadValue(CVisSDIStream& refsdistream)
{
	refsdistream.OpenGroup();

	if (refsdistream.AtEndOfGroup())
	{
		Resize(0);
	}
	else
	{
		CVisPropTypeInfoBase *pproptypeinfoDbl
				= VisLookupPropTypeInfo(typeid(double));

		std::string strType;
		std::string strName;
		CVisDim dim;
		refsdistream.ReadTypeNameAndDim(strType, strName, dim);
		if ((strType != pproptypeinfoDbl->Name())
				|| (strName != "m_data")
				|| (dim.CDim() != 1) || (dim[0] <= 0))
		{
			throw CVisSDIError("Bad CVisDVector class information",
					"CVisDVector::ReadValue", refsdistream.ILine());
		}

		Resize(dim[0]);

		pproptypeinfoDbl->ReadObjArray(refsdistream, dim,
				(void *) m_data);

		refsdistream.EndObj();
	}

	refsdistream.CloseGroup();
}

VisMatrixExport void CVisDVector::WriteValue(CVisSDOStream& refsdostream) const
{
	bool fEmpty = (m_data == 0);

	refsdostream.OpenGroup(!fEmpty);

	if (!fEmpty)
	{
		CVisPropTypeInfoBase *pproptypeinfoDbl
				= VisLookupPropTypeInfo(typeid(double));
		CVisDim dim(Length());
		pproptypeinfoDbl->WriteObjArray(refsdostream, dim,
				(const void *) m_data, "m_data");
	}

	refsdostream.CloseGroup(!fEmpty);
}


VisMatrixExport void CVisDMatrix::ReadValue(CVisSDIStream& refsdistream)
{
	refsdistream.OpenGroup();

	if (refsdistream.AtEndOfGroup())
	{
		Resize(0, 0);
	}
	else
	{
		CVisPropTypeInfoBase *pproptypeinfoDbl
				= VisLookupPropTypeInfo(typeid(double));

		std::string strType;
		std::string strName;
		CVisDim dim;
		refsdistream.ReadTypeNameAndDim(strType, strName, dim);
		if ((strType != pproptypeinfoDbl->Name())
				|| (strName != "m_data")
				|| (dim.CDim() != 2) || (dim[0] <= 0) || (dim[1] <= 0))
		{
			throw CVisSDIError("Bad CVisDMatrix class information",
					"CVisDMatrix::ReadValue", refsdistream.ILine());
		}

		Resize(dim[0], dim[1]);

		pproptypeinfoDbl->ReadObjArray(refsdistream, dim,
				(void *) m_data[0]);

		refsdistream.EndObj();
	}

	refsdistream.CloseGroup();
}

VisMatrixExport void CVisDMatrix::WriteValue(CVisSDOStream& refsdostream) const
{
	bool fEmpty = (m_data == 0);

	refsdostream.OpenGroup(!fEmpty);

	if (!fEmpty)
	{
		CVisPropTypeInfoBase *pproptypeinfoDbl
				= VisLookupPropTypeInfo(typeid(double));
		CVisDim dim(NRows(), NCols());
		pproptypeinfoDbl->WriteObjArray(refsdostream, dim,
				(const void *) m_data[0], "m_data");
	}

	refsdostream.CloseGroup(!fEmpty);
}


VIS_DEFINE_SD_CLASS_WITH_IO(VisMatrixExportDeclspec, CVisDVector)
VIS_DEFINE_SD_CLASS_WITH_IO(VisMatrixExportDeclspec, CVisDMatrix)

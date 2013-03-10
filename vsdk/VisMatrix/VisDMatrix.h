///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisDMatrix.h -- double precision matrix/vector operations
//
// DESCRIPTION
//  The CVisDMatrix class provides some basic matrix operations, using calls
//  to external software (IMSL for the moment) to perform the more
//  complicated operations.
//
//  To take advantage of the IMSL numerical analysis routines,
//  define VIS_USE_IMSL in the Build Settings C/C++ property page.
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////


#ifndef __VIS_MATRIX_DMATRIX_H__
#define __VIS_MATRIX_DMATRIX_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <iostream>

#ifndef VisMatrixExport
#define VisMatrixExport __declspec(dllimport)
#endif // VisMatrixExport


// UNDONE: Move inline functions to an INL file.


// LATER:  Could we use std::vector internally or derive from it?  (Would
// the data be adjacent in memory and the operators be as efficient?)


// LATER:  Should there be options to initialize elements when creating or
// resizing?


// LATER:  We could create derived classes to use to return values from
// functions like operator*.  Our code could know that the buffers in these
// classes were part of a temporary variable, so it could steal the buffers
// instead of copying the data in some cases.  (There was an article about
// this in CUJ or Dr. Dobbs around late 1997 or early 1998.)


// Forward declaration
class CVisDMatrix;


// Preprocessor definition to use a buffer to avoid allocating memory with
// small vectors.
// UNDONE:  What value (if any) do we want to use here?
#define VIS_DVECTOR_CDBL_BUF		3


////////////////////////////////////////////////////////////////////////////
//  
//  CLASS:
//      CVisDVector
//  
////////////////////////////////////////////////////////////////////////////
class CVisDVector
{
public:
    // Constructors (default destructor and copy constructor)
    CVisDVector(void);
    CVisDVector(int length, double *storage = 0);
    CVisDVector(const CVisDVector &vec);
    ~CVisDVector(void);

#ifndef VIS_NO_LUC_FILES
	// Conversion from Luciform types
	template<class TCoord>
	CVisDVector(const GXPoint2TC<TCoord>& refpoint2)
	  : m_length(0), m_data(0), m_fExternalStorage(false)
		{ Resize(2); m_data[0] = refpoint2.u; m_data[1] = refpoint2.v; }
	template<class TCoord>
	CVisDVector& operator=(const GXPoint2TC<TCoord>& refpoint2)
	  : m_length(0), m_data(0), m_fExternalStorage(false)
		{ Resize(2); m_data[0] = refpoint2.u; m_data[1] = refpoint2.v;
				return *this; }
	template<class TCoord>
	CVisDVector(const GXVector2TC<TCoord>& refvector2)
	  : m_length(0), m_data(0), m_fExternalStorage(false)
		{ Resize(2); m_data[0] = refvector2.du; m_data[1] = refvector2.dv; }
	template<class TCoord>
	CVisDVector& operator=(const GXVector2TC<TCoord>& refvector2)
	  : m_length(0), m_data(0), m_fExternalStorage(false)
		{ Resize(2); m_data[0] = refvector2.du; m_data[1] = refvector2.dv;
				return *this; }
	template<class TCoord>
	CVisDVector(const GXCoVector2TC<TCoord>& refcovector2)
	  : m_length(0), m_data(0), m_fExternalStorage(false)
		{ Resize(2); m_data[0] = refcovector2.nu; m_data[1] = refcovector2.nv; }
	template<class TCoord>
	CVisDVector& operator=(const GXCoVector2TC<TCoord>& refcovector2)
	  : m_length(0), m_data(0), m_fExternalStorage(false)
		{ Resize(2); m_data[0] = refcovector2.nu; m_data[1] = refcovector2.nv;
				return *this; }
	template<class TCoord>
	CVisDVector(const GXPoint3TC<TCoord>& refpoint3)
	  : m_length(0), m_data(0), m_fExternalStorage(false)
		{ Resize(3); m_data[0] = refpoint3.x; m_data[1] = refpoint3.y;
				m_data[2] = refpoint3.z; }
	template<class TCoord>
	CVisDVector& operator=(const GXPoint3TC<TCoord>& refpoint3)
	  : m_length(0), m_data(0), m_fExternalStorage(false)
		{ Resize(3); m_data[0] = refpoint3.x; m_data[1] = refpoint3.y;
				m_data[2] = refpoint3.z; return *this; }
	template<class TCoord>
	CVisDVector(const GXVector3TC<TCoord>& refvector3)
	  : m_length(0), m_data(0), m_fExternalStorage(false)
		{ Resize(3); m_data[0] = refvector3.dx; m_data[1] = refvector3.dy;
				m_data[2] = refvector3.dz; }
	template<class TCoord>
	CVisDVector& operator=(const GXVector3TC<TCoord>& refvector3)
	  : m_length(0), m_data(0), m_fExternalStorage(false)
		{ Resize(3); m_data[0] = refvector3.dx; m_data[1] = refvector3.dy;
				m_data[2] = refvector3.dz; return *this; }
	template<class TCoord>
	CVisDVector(const GXCoVector3TC<TCoord>& refcovector3)
	  : m_length(0), m_data(0), m_fExternalStorage(false)
		{ Resize(3); m_data[0] = refcovector3.nx; m_data[1] = refcovector3.ny;
				m_data[2] = refcovector3.nz; }
	template<class TCoord>
	CVisDVector& operator=(const GXCoVector3TC<TCoord>& refcovector3)
	  : m_length(0), m_data(0), m_fExternalStorage(false)
		{ Resize(3); m_data[0] = refcovector3.nx; m_data[1] = refcovector3.ny;
				m_data[2] = refcovector3.nz; return *this; }
	template<class TCoord>
	CVisDVector(const GXPoint4TC<TCoord>& refpoint4)
	  : m_length(0), m_data(0), m_fExternalStorage(false)
		{ Resize(4); m_data[0] = refpoint4.x; m_data[1] = refpoint4.y;
				m_data[2] = refpoint4.z; m_data[3] = refpoint4.w; }
	template<class TCoord>
	CVisDVector& operator=(const GXPoint4TC<TCoord>& refpoint4)
	  : m_length(0), m_data(0), m_fExternalStorage(false)
		{ Resize(4); m_data[0] = refpoint4.x; m_data[1] = refpoint4.y;
				m_data[2] = refpoint4.z; m_data[3] = refpoint4.w;
				return *this; }
	template<class TCoord>
	CVisDVector(const GXVector4TC<TCoord>& refvector4)
	  : m_length(0), m_data(0), m_fExternalStorage(false)
		{ Resize(4); m_data[0] = refvector4.dx; m_data[1] = refvector4.dy;
				m_data[2] = refvector4.dz; m_data[3] = refvector4.dw; }
	template<class TCoord>
	CVisDVector& operator=(const GXVector4TC<TCoord>& refvector4)
	  : m_length(0), m_data(0), m_fExternalStorage(false)
		{ Resize(4); m_data[0] = refvector4.dx; m_data[1] = refvector4.dy;
				m_data[2] = refvector4.dz; m_data[3] = refvector4.dw;
				return *this; }
	template<class TCoord>
	CVisDVector(const GXCoVector4TC<TCoord>& refcovector4)
	  : m_length(0), m_data(0), m_fExternalStorage(false)
		{ Resize(4); m_data[0] = refcovector4.nx; m_data[1] = refcovector4.ny;
				m_data[2] = refcovector4.nz; m_data[3] = refcovector4.nw; }
	template<class TCoord>
	CVisDVector& operator=(const GXCoVector4TC<TCoord>& refcovector4)
	  : m_length(0), m_data(0), m_fExternalStorage(false)
		{ Resize(4); m_data[0] = refcovector4.nx; m_data[1] = refcovector4.ny;
				m_data[2] = refcovector4.nz; m_data[3] = refcovector4.nw;
				return *this; }
	template<class TCoord>
	CVisDVector(const GXHPoint2TC<TCoord>& refhpoint2)
	  : m_length(0), m_data(0), m_fExternalStorage(false)
		{ Resize(3); m_data[0] = refhpoint2.x; m_data[1] = refhpoint2.y;
				m_data[2] = refhpoint2.w; }
	template<class TCoord>
	CVisDVector& operator=(const GXHPoint2TC<TCoord>& refhpoint2)
	  : m_length(0), m_data(0), m_fExternalStorage(false)
		{ Resize(3); m_data[0] = refhpoint2.x; m_data[1] = refhpoint2.y;
				m_data[2] = refhpoint2.w; return *this; }
	template<class TCoord>
	CVisDVector(const GXHPoint3TC<TCoord>& refhpoint3)
	  : m_length(0), m_data(0), m_fExternalStorage(false)
		{ Resize(4); m_data[0] = refhpoint3.x; m_data[1] = refhpoint3.y;
				m_data[2] = refhpoint3.z; m_data[3] = refhpoint3.w; }
	template<class TCoord>
	CVisDVector& operator=(const GXHPoint3TC<TCoord>& refhpoint3)
	  : m_length(0), m_data(0), m_fExternalStorage(false)
		{ Resize(4); m_data[0] = refhpoint3.x; m_data[1] = refhpoint3.y;
				m_data[2] = refhpoint3.z; m_data[3] = refhpoint3.w; return *this; }
	template<class TCoord>
	CVisDVector(const GXHPlane3TC<TCoord>& refhplane3)
	  : m_length(0), m_data(0), m_fExternalStorage(false)
		{ Resize(4); m_data[0] = refhplane3.x; m_data[1] = refhplane3.y;
				m_data[2] = refhplane3.z; m_data[3] = refhplane3.w; }
	template<class TCoord>
	CVisDVector& operator=(const GXHPlane3TC<TCoord>& refhplane3)
	  : m_length(0), m_data(0), m_fExternalStorage(false)
		{ Resize(4); m_data[0] = refhplane3.x; m_data[1] = refhplane3.y;
				m_data[2] = refhplane3.z; m_data[3] = refhplane3.w; return *this; }
#endif // VIS_NO_LUC_FILES


    // Vector length
    int Length(void) const { return m_length; }
    VisMatrixExport void Resize(int length, double *storage = 0);

    // Element access
    double& operator[](int i) { return m_data[i]; }
    const double& operator[](int i) const { return m_data[i]; }

    // Assignment
    VisMatrixExport CVisDVector& operator=(const CVisDVector &vec);
    VisMatrixExport CVisDVector& operator=(double value);

	// We define operator < so that this class can be used in STL containers.
	VisMatrixExport bool operator==(const CVisDVector& refvector) const;
	bool operator!=(const CVisDVector& refvector) const;
	VisMatrixExport bool operator<(const CVisDVector& refvector) const;
	
	CVisDVector operator+(const CVisDVector &refvectorB) const;

	VisMatrixExport CVisDVector operator-(void) const;
	CVisDVector operator-(const CVisDVector &refvectorB) const;

	CVisDVector operator*(double dbl) const;
	VisMatrixExport double operator*(const CVisDVector &refvectorB) const;   // dot product

	CVisDVector operator/(double dbl) const;

	CVisDVector& operator+=(const CVisDVector &refvectorB);
	CVisDVector& operator-=(const CVisDVector &refvectorB);
	VisMatrixExport CVisDVector& operator*=(double dbl);
	VisMatrixExport CVisDVector& operator/=(double dbl);

	VisMatrixExport CVisDVector& EqSum(const CVisDVector &refvectorA, const CVisDVector &refvectorB);
	VisMatrixExport CVisDVector& EqDiff(const CVisDVector &refvectorA, const CVisDVector &refvectorB);
	VisMatrixExport CVisDVector& EqProd(const CVisDMatrix &refmatrixA, const CVisDVector &refvectorB);

	// LATER:  Do we want to add (vector) * (matrix) multiplication (that implicitly
	// transposes the vector before multiplying)?

	VisMatrixExport void ReadValue(CVisSDIStream& refsdistream);
	VisMatrixExport void WriteValue(CVisSDOStream& refsdostream) const;

    // Old self-describing input/output format
    enum FieldType { eftName, eftLength, eftData, eftEnd};
    const char *ReadWriteField(CVisSDStream& s, int field_id);

	// Flag used with the std::ostream file I/O methods.
	// This may not be supported in future releases.
    VisMatrixExport static bool s_fVerboseOutput; // print out dimensions on output

protected:
	VisMatrixExport void FreeBuffer(void);
	VisMatrixExport void SetSize(int length, double *storage = 0);  // Called by ctors and Resize.

public:
	double *data() const { return m_data; }

private:
    double *m_data;     // pointer to data
    int m_length;       // number of elements
#ifdef VIS_DVECTOR_CDBL_BUF
    double m_rgdblBuf[VIS_DVECTOR_CDBL_BUF];  // Buffer to avoid allocating memory for small vectors
#endif // VIS_DVECTOR_CDBL_BUF
	bool m_fExternalStorage;
};


////////////////////////////////////////////////////////////////////////////
//  
//  CLASS:
//      CVisDMatrix
//  
////////////////////////////////////////////////////////////////////////////
class CVisDMatrix
{
public:
    // Constructors (default destructor and copy constructor)
    CVisDMatrix(void);
    CVisDMatrix(int rows, int cols, double *storage = 0);
    CVisDMatrix(const CVisDMatrix &mat);
    ~CVisDMatrix(void);

#ifndef VIS_NO_LUC_FILES

	// Conversion from Luciform types
	template<class TCoord>
	CVisDMatrix(const GXMatrix2TC<TCoord>& refmatrix)
	  : m_nRows(0), m_nCols(0), m_data(0), m_fExternalStorage(false)
		{ VisCopyMatrixToMatrix(refmatrix, *this); }
	template<class TCoord>
	CVisDMatrix& operator=(const GXMatrix2TC<TCoord>& refmatrix)
	  : m_nRows(0), m_nCols(0), m_data(0), m_fExternalStorage(false)
		{ VisCopyMatrixToMatrix(refmatrix, *this); }
	template<class TCoord>
	CVisDMatrix(const GXMatrix3TC<TCoord>& refmatrix)
	  : m_nRows(0), m_nCols(0), m_data(0), m_fExternalStorage(false)
		{ VisCopyMatrixToMatrix(refmatrix, *this); }
	template<class TCoord>
	CVisDMatrix& operator=(const GXMatrix3TC<TCoord>& refmatrix)
	  : m_nRows(0), m_nCols(0), m_data(0), m_fExternalStorage(false)
		{ VisCopyMatrixToMatrix(refmatrix, *this); }
	template<class TCoord>
	CVisDMatrix(const GXMatrix4TC<TCoord>& refmatrix)
	  : m_nRows(0), m_nCols(0), m_data(0), m_fExternalStorage(false)
		{ VisCopyMatrixToMatrix(refmatrix, *this); }
	template<class TCoord>
	CVisDMatrix& operator=(const GXMatrix4TC<TCoord>& refmatrix)
	  : m_nRows(0), m_nCols(0), m_data(0), m_fExternalStorage(false)
		{ VisCopyMatrixToMatrix(refmatrix, *this); }

#endif // VIS_NO_LUC_FILES

    // Matrix shape
    int NRows(void) const { return m_nRows; }
    int NCols(void) const { return m_nCols; }
    VisMatrixExport void Resize(int rows, int cols, double *storage = 0);

    // Element access
    double *operator[](int i) { return m_data[i]; }
    const double *operator[](int i) const { return m_data[i]; }

    // Assignment
    VisMatrixExport CVisDMatrix& operator=(const CVisDMatrix &mat);
    VisMatrixExport CVisDMatrix& operator=(double value);

	// We define operator < so that this class can be used in STL containers.
	VisMatrixExport bool operator==(const CVisDMatrix& refmatrix) const;
	bool operator!=(const CVisDMatrix& refmatrix) const;
	VisMatrixExport bool operator<(const CVisDMatrix& refmatrix) const;

	CVisDMatrix operator+(const CVisDMatrix& A) const;

	VisMatrixExport CVisDMatrix operator-(void) const;
	CVisDMatrix operator-(const CVisDMatrix& A) const;

	CVisDMatrix operator*(double dbl) const;
	CVisDMatrix operator*(const CVisDMatrix& refmatrix) const;
	CVisDVector operator*(const CVisDVector& refvector) const;

	CVisDMatrix operator/(double dbl) const;

	VisMatrixExport CVisDMatrix& operator+=(const CVisDMatrix& A);
	VisMatrixExport CVisDMatrix& operator-=(const CVisDMatrix& A);
	VisMatrixExport CVisDMatrix& operator*=(double dbl);
	VisMatrixExport CVisDMatrix& operator*=(const CVisDMatrix& A);
	VisMatrixExport CVisDMatrix& operator/=(double dbl);
	
	VisMatrixExport CVisDMatrix& EqSum(const CVisDMatrix &refmatrixA, const CVisDMatrix &refmatrixB);
	VisMatrixExport CVisDMatrix& EqDiff(const CVisDMatrix &refmatrixA, const CVisDMatrix &refmatrixB);
	VisMatrixExport CVisDMatrix& EqProd(const CVisDMatrix &refmatrixA, const CVisDMatrix &refmatrixB);

	VisMatrixExport bool IsSymmetric(void) const;

	VisMatrixExport CVisDMatrix Inverted(void) const;
	VisMatrixExport CVisDMatrix& Invert(void);

	VisMatrixExport CVisDMatrix Transposed(void) const;
	CVisDMatrix& Transpose(void);

	// compute the determinant and also the inverse
	VisMatrixExport double Determinant(CVisDMatrix &inverse) const;
	// compute only the determinant
	VisMatrixExport double Determinant(void) const;
	// extract the submatrix of `newRows'x`newCols' from (i0,j0), 
	// and put it to `submat' whose reference is also returned
	VisMatrixExport CVisDMatrix& Submatrix(const int newRows, const int newCols, CVisDMatrix& submat,
		const int i0 = 0, const int j0 = 0) const;
	// return the submatrix of `newRows'x`newCols' from (i0,j0)
	VisMatrixExport CVisDMatrix Submatrix(const int newRows, const int newCols,
		const int i0 = 0, const int j0 = 0) const;
	// return the r-th row
	VisMatrixExport CVisDVector Row(int r) const;
	// return the c-th column
	VisMatrixExport CVisDVector Column(int c) const;
	// set the r-th row
	VisMatrixExport void SetRow(int r, const CVisDVector &v);
	// set the c-th column
	VisMatrixExport void SetColumn(int c, const CVisDVector &v);

	VisMatrixExport void ReadValue(CVisSDIStream& refsdistream);
	VisMatrixExport void WriteValue(CVisSDOStream& refsdostream) const;

    // Old self-describing input/output format
    enum FieldType { eftName, eftDims, eftData, eftEnd};
    const char *ReadWriteField(CVisSDStream& s, int field_id);
    
	// Flag used with the std::ostream file I/O methods.
	// This may not be supported in future releases.
    VisMatrixExport static bool s_fVerboseOutput; // print out dimensions on output

protected:
	VisMatrixExport void FreeBuffer(void);
	VisMatrixExport void SetSize(int rows, int cols, double *storage = 0);  // Called by ctors and Resize.

private:
    int m_nRows;        // number of rows in matrix
    int m_nCols;        // number of columns in matrix
    double **m_data;    // Iliffe vector (array of pointers to data)
	bool m_fExternalStorage;
//    CVisMemBlockOf<double> m_memblockStorage;  // reference counted storage
//    CVisMemBlockOf<double *> m_memblockIliffe;   // Iliffe vector r. c. storage
};



//  Supported functions


VisMatrixExport void VISAPI VisSetToZero(CVisDVector& refvector);
VisMatrixExport void VISAPI VisSetToZero(CVisDMatrix& refmatrix);
VisMatrixExport void VISAPI VisSetToIdentity(CVisDMatrix& refmatrix);


// LATER:  Add these and fns to work with other exponents.
// VisMatrixExport CVisDMatrix VISAPI VisMatrixSqrt(const CVisDMatrix& A);
// VisMatrixExport void VISAPI VisMatrixSqrt(const CVisDMatrix& A, CVisDMatrix &Asqrt);

VisMatrixExport CVisDMatrix VISAPI VisDMatrixSqrtInverse(const CVisDMatrix& A);
VisMatrixExport void VISAPI VisDMatrixSqrtInverse(const CVisDMatrix& A,
		CVisDMatrix &AsqrtInv);

VisMatrixExport CVisDVector VISAPI VisDMatrixSolve(const CVisDMatrix& A,
		const CVisDVector& refvectorB);
VisMatrixExport void VISAPI VisDMatrixSolve(const CVisDMatrix& A,
		const CVisDVector& refvectorB, CVisDVector& x);



// A is symmetric positive definite
VisMatrixExport CVisDVector VISAPI VisDMatrixSolveSPD(const CVisDMatrix& A,
		const CVisDVector& refvectorB);
VisMatrixExport void VISAPI VisDMatrixSolveSPD(const CVisDMatrix& A,
		const CVisDVector& refvectorB, CVisDVector& x, int n = -1);

VisMatrixExport CVisDMatrix VISAPI VisDMatrixLeastSquares(const CVisDMatrix& A,
		const CVisDMatrix& refmatrixB);
VisMatrixExport void VISAPI VisDMatrixLeastSquares(const CVisDMatrix& A,
		const CVisDMatrix& refmatrixB,
                           CVisDMatrix& X);
VisMatrixExport CVisDVector VISAPI VisDMatrixLeastSquares(const CVisDMatrix& A,
		const CVisDVector& refvectorB);
VisMatrixExport void VISAPI VisDMatrixLeastSquares(const CVisDMatrix& A,
		const CVisDVector& refvectorB,
                           CVisDVector& x);

// Computes the minimum norm least squares solution to an over-
//    or under-determined system of linear equations A X=refmatrixB,  using
//   the singular value decomposition of A.
// returns the effective rank of A
// added by zhang on May 15, 1998
VisMatrixExport int VISAPI VisDMatrixLLS(
	const CVisDMatrix& A,
	const CVisDMatrix& refmatrixB,
	CVisDMatrix& X
	);

// QR factorization related functions
VisMatrixExport CVisDVector VISAPI VisDMatrixSolveQR(const CVisDMatrix& A,
		const CVisDVector& refvectorB);
VisMatrixExport void VISAPI VisDMatrixSolveQR(const CVisDMatrix& A,
		const CVisDVector& refvectorB, CVisDVector& x);
VisMatrixExport void VISAPI VisDMatrixEQConstrainedLS(CVisDMatrix& A,
		CVisDVector& refvectorB, CVisDMatrix& C, CVisDVector& d, CVisDVector& x); 


// Singular Value Decomposition (SVD)
VisMatrixExport void VISAPI VisDMatrixSVD(const CVisDMatrix& A, CVisDVector& s, 
		CVisDMatrix& U, CVisDMatrix& V, int compute_left = 1,
		int compute_right = 1);


// LATER:  Variations that don't find the eigenvector.
VisMatrixExport void VISAPI VisMinEigenValue(CVisDMatrix& A, CVisDVector& x);
VisMatrixExport void VISAPI VisMaxEigenValue(CVisDMatrix& A, CVisDVector& x);

// min = 1 -> minimum eigenvalue problem; min = 0 -> maximum eigenvalue
VisMatrixExport double VISAPI VisMinMaxEigenValue(CVisDMatrix& A, CVisDVector& x,
		bool fMin); 

// compute all eigenvalues and eigenvectors of a real symmetric matrix
// the eigenvalues are in ascending order
// added by zhang on 13 may 1998
VisMatrixExport void VISAPI VisDMatrixSymmetricEigen(const CVisDMatrix &A,
		CVisDVector &vals, CVisDMatrix &vects);
// compute only the eigenvalues of a real symmetric matrix
// the eigenvalues are in ascending order
// added by zhang on 13 may 1998
VisMatrixExport void VISAPI VisDMatrixSymmetricEigen(const CVisDMatrix &A,
		CVisDVector &vals);
// compute the selected eigenvalues, and optionally (if 'vects' != 0), eigenvectors, 
//     of a real symmetric matrix
// they are selected by specifying the range of indices: 1 <= indLow <= indUp <= m
// the eigenvalues are in ascending order
// 'vals' must be of: indUp - indLow + 1
// 'vects' must be of: m x (indUp - indLow + 1)
// added by zhang on 13 may 1998
VisMatrixExport void VISAPI VisDMatrixSymmetricEigen(const CVisDMatrix &A,
	CVisDVector &vals, CVisDMatrix &vects, const int indLow, const indUp);
VisMatrixExport void VISAPI VisDMatrixSymmetricEigen(const CVisDMatrix &A,
	CVisDVector &vals, const int indLow, const indUp);

// outer product of two vectors
// added by zhang on 4 sept. 1998
VisMatrixExport CVisDMatrix VISAPI VisOuterProduct(const CVisDVector &v1,
												   const CVisDVector &v2);

// cross product of two vectors
// added by zhang on 30 sept. 1998
inline CVisDVector VisCrossProduct(const CVisDVector &v1,
												   const CVisDVector &v2);
VisMatrixExport void VISAPI VisCrossProduct(const CVisDVector &v1,
										   const CVisDVector &v2,
										   CVisDVector &vOut);


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
);
// compute the eigenvalues and right eigenvectors of a generic square matrix
// no left eigenvectors
// added by zhang on Jan 11, 1999
VisMatrixExport void VISAPI VisDMatrixEigen(
	const CVisDMatrix &A, // original matrix
	CVisDVector &valsReal, // real part of the eigenvalues
	CVisDVector &valsImaginary, // imaginary part of the eigenvalues
	CVisDMatrix &vectsR // matrix of the right eigenvectors
);
// compute the eigenvalues and left eigenvectors of a generic square matrix
// no right eigenvectors
// added by zhang on Jan 11, 1999
VisMatrixExport void VISAPI VisDMatrixEigen(
	const CVisDMatrix &A, // original matrix
	CVisDMatrix &vectsL,  // matrix of the left eigenvectors
	CVisDVector &valsReal, // real part of the eigenvalues
	CVisDVector &valsImaginary // imaginary part of the eigenvalues
);
// compute the eigenvalues of a generic square matrix
// only eigenvalues
// added by zhang on Jan 11, 1999
VisMatrixExport void VISAPI VisDMatrixEigen(
	const CVisDMatrix &A, // original matrix
	CVisDVector &valsReal, // real part of the eigenvalues
	CVisDVector &valsImaginary // imaginary part of the eigenvalues
);


#ifndef VIS_ONLY_DECLARE_PROPLIST

VIS_DECLARE_SD_CLASS_WITH_IO(VisMatrixExportDeclspec, CVisDVector)
VIS_DECLARE_SD_CLASS_WITH_IO(VisMatrixExportDeclspec, CVisDMatrix)

#endif // VIS_ONLY_DECLARE_PROPLIST


//  Rudimentary output for debugging
// This may not be supported in future releases.
VisMatrixExport std::ostream& VISAPI operator<<(std::ostream& os, const CVisDVector& v);
// VisMatrixExport istream& operator>>(istream& os, CVisDVector& v);
// This may not be supported in future releases.
VisMatrixExport std::ostream& VISAPI operator<<(std::ostream& os, const CVisDMatrix& mat);
// VisMatrixExport istream& operator>>(istream& os, CVisDVector& v);


// Old self-describing stream I/O
inline CVisSDStream& operator<<(CVisSDStream& s, CVisDVector& o);
inline CVisSDStream& operator<<(CVisSDStream& s, CVisDMatrix& o);


#include "VisDMatrix.inl"


#endif __VIS_MATRIX_DMATRIX_H__

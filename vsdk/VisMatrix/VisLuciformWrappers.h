// VisLuciformWrappers.h
//
// Wrappers for the Luciform classes.  (These allow us to add our
// own methods.)
//
// Copyright © 2000 Microsoft Corporation, All Rights Reserved

#pragma once

#include "VisLuciform.h"


// Forward declarations
class CVisDVector;
class CVisDMatrix;
class CVisVector4;			// old 4-vector class
class CVisTransform4x4;		// old 4x4-matrix class


// UNDONE:  Property list and SDStream I/O support.
// UNDONE:  Transform chain classes that work with these types.
// UNDONE:  We may need to add overload operator methods to these
// classes to make sure that the results returned are of these
// types (instead of the base types).
// LATER:  We may also want some methods to copy submatrices to
// or from larger matrices.



template<class TCoord>
class CVisHPoint2TC : public GXHPoint2TC<TCoord>
{
public:
	// LATER:  Helpful typedefs
	// typedef CVisMatrix3TC<TCoord> T_Matrix;
	// typedef CVisPoint2TC<TCoord> T_Point;

	// Luciform class constructors
    CVisHPoint2TC( void );
    CVisHPoint2TC( const GXPoint2TC<TCoord>& p );
    CVisHPoint2TC( const TCoord& x, const TCoord& y, const TCoord& w );

	// Constructor and assignment operator using the base class
    CVisHPoint2TC( const GXHPoint2TC<TCoord>& refobj );
    CVisHPoint2TC<TCoord>& operator=( const GXHPoint2TC<TCoord>& refobj );

	// Constructor and assignment operator using the (general) CVisDVector class
    CVisHPoint2TC( const CVisDVector& refvector );
    CVisHPoint2TC<TCoord>& operator=( const CVisDVector& refvector );

	// Constructor and assignment operator using the old CVisVector4 class
    CVisHPoint2TC( const CVisVector4& refvector );
    CVisHPoint2TC<TCoord>& operator=( const CVisVector4& refvector );
};


template<class TCoord>
class CVisHPoint3TC : public GXHPoint3TC<TCoord>
{
public:
	// LATER:  Helpful typedefs
	// typedef CVisMatrix4TC<TCoord> T_Matrix;
	// typedef CVisPoint3TC<TCoord> T_Point;

	// Luciform class constructors
    CVisHPoint3TC( void );
    CVisHPoint3TC( const GXPoint3TC<TCoord>& p );
    CVisHPoint3TC( const TCoord& x, const TCoord& y, const TCoord& z,
			const TCoord& w );

	// Constructor and assignment operator using the base class
    CVisHPoint3TC( const GXHPoint3TC<TCoord>& refobj );
    CVisHPoint3TC<TCoord>& operator=( const GXHPoint3TC<TCoord>& refobj );

	// Constructor and assignment operator using the (general) CVisDVector class
    CVisHPoint3TC( const CVisDVector& refvector );
    CVisHPoint3TC<TCoord>& operator=( const CVisDVector& refvector );

	// Constructor and assignment operator using the old CVisVector4 class
    CVisHPoint3TC( const CVisVector4& refvector );
    CVisHPoint3TC<TCoord>& operator=( const CVisVector4& refvector );
};


template<class TCoord>
class CVisHPlane3TC : public GXHPlane3TC<TCoord>
{
public:
	// Luciform class constructors
    CVisHPlane3TC( void );
    CVisHPlane3TC( const GXPoint3TC<TCoord>& p1,
			const GXPoint3TC<TCoord>& p2,
			const GXPoint3TC<TCoord>& p3 );
    CVisHPlane3TC( const TCoord& x, const TCoord& y, const TCoord& z,
			const TCoord& w );

	// Constructor and assignment operator using the base class
    CVisHPlane3TC( const GXHPlane3TC<TCoord>& refobj );
    CVisHPlane3TC<TCoord>& operator=( const GXHPlane3TC<TCoord>& refobj );

	// Constructor and assignment operator using the (general) CVisDVector class
    CVisHPlane3TC( const CVisDVector& refvector );
    CVisHPlane3TC<TCoord>& operator=( const CVisDVector& refvector );

	// Constructor and assignment operator using the old CVisVector4 class
    CVisHPlane3TC( const CVisVector4& refvector );
    CVisHPlane3TC<TCoord>& operator=( const CVisVector4& refvector );
};


template<class TCoord>
class CVisVector2TC : public GXVector2TC<TCoord>
{
public:
	enum { dim = 2 };

	// Luciform class constructors
    CVisVector2TC( void );
    CVisVector2TC( const TCoord& d );
    CVisVector2TC( const TCoord& du, const TCoord& dv );

	// Constructor and assignment operator using the base class
    CVisVector2TC( const GXVector2TC<TCoord>& refobj );
    CVisVector2TC<TCoord>& operator=( const GXVector2TC<TCoord>& refobj );

	// Constructor and assignment operator using the (general) CVisDVector class
    CVisVector2TC( const CVisDVector& refvector );
    CVisVector2TC<TCoord>& operator=( const CVisDVector& refvector );

	// Constructor and assignment operator using the old CVisVector4 class
    CVisVector2TC( const CVisVector4& refvector );
    CVisVector2TC<TCoord>& operator=( const CVisVector4& refvector );
};


template<class TCoord>
class CVisCoVector2TC : public GXCoVector2TC<TCoord>
{
public:
	enum { dim = 2 };

	// Luciform class constructors
    CVisCoVector2TC( void );
    CVisCoVector2TC( const TCoord& n );
    CVisCoVector2TC( const TCoord& nu, const TCoord& nv );

	// Constructor and assignment operator using the base class
    CVisCoVector2TC( const GXCoVector2TC<TCoord>& refobj );
    CVisCoVector2TC<TCoord>& operator=( const GXCoVector2TC<TCoord>& refobj );

	// Constructor and assignment operator using the (general) CVisDVector class
    CVisCoVector2TC( const CVisDVector& refvector );
    CVisCoVector2TC<TCoord>& operator=( const CVisDVector& refvector );

	// Constructor and assignment operator using the old CVisVector4 class
    CVisCoVector2TC( const CVisVector4& refvector );
    CVisCoVector2TC<TCoord>& operator=( const CVisVector4& refvector );


	// Multiplication with vectors and matrices
	TCoord operator*( const GXVector2TC<TCoord>& refvector ) const;
	CVisCoVector2TC<TCoord> operator*( const GXMatrix2TC<TCoord>& refmatrix ) const;
	CVisCoVector2TC<TCoord>& operator*=( const GXMatrix2TC<TCoord>& refmatrix );
};


template<class TCoord>
class CVisMatrix2TC : public GXMatrix2TC<TCoord>
{
public:
	enum { dim = 2 };

	// Luciform class constructors
    CVisMatrix2TC( void );
    CVisMatrix2TC( const TCoord& s );
    CVisMatrix2TC( const GXVector2TC<TCoord>& vCol1,
			const GXVector2TC<TCoord>& vCol2 );
    CVisMatrix2TC( const GXCoVector2TC<TCoord>& covRow1,
			const GXCoVector2TC<TCoord>& covRow2 );

	// Constructor and assignment operator using the base class
    CVisMatrix2TC( const GXMatrix2TC<TCoord>& refobj );
    CVisMatrix2TC<TCoord>& operator=( const GXMatrix2TC<TCoord>& refobj );

	// Constructor and assignment operator using the (general) CVisDMatrix class
    CVisMatrix2TC( const CVisDMatrix& refmatrix );
    CVisMatrix2TC<TCoord>& operator=( const CVisDMatrix& refmatrix );

	// Constructor and assignment operator using the old CVisTransform4x4 class
    CVisMatrix2TC( const CVisTransform4x4& refmatrix );
    CVisMatrix2TC<TCoord>& operator=( const CVisTransform4x4& refmatrix );


	// Multiplication with vectors
	CVisVector2TC<TCoord> operator*( const GXVector2TC<TCoord>& refvector ) const;


	// Return the transpose of this matrix
	CVisMatrix2TC<TCoord> Transposed(void) const;

	// Change this matrix to its transpose.  Return a reference to this matrix.
	CVisMatrix2TC<TCoord>& Transpose(void);

	// Change this matrix to the transpose of the specified matrix.
	// Return a reference to this matrix.
	CVisMatrix2TC<TCoord>& Transpose(const CVisMatrix2TC<TCoord>& refmatrix);


	// Return the inverse of this matrix
	CVisMatrix2TC<TCoord> Inverted(void) const;

	// Change this matrix to its inverse.  Return a reference to this matrix.
	CVisMatrix2TC<TCoord>& Inverse(void);

	// Change this matrix to the inverse of the specified matrix.
	// Return a reference to this matrix.
	CVisMatrix2TC<TCoord>& Inverse(const CVisMatrix2TC<TCoord>& refmatrix);


protected:
	// Helper functions
	void CopyMatrix( const CVisDMatrix& refmatrix );
	void CopyMatrix( const CVisTransform4x4& refmatrix );
};


template<class TCoord>
class CVisPoint2TC : public GXPoint2TC<TCoord>
{
public:
	enum { dim = 2 };

	// Helpful typedefs
	typedef CVisVector2TC<TCoord> T_Vector;
	typedef CVisCoVector2TC<TCoord> T_CoVector;
	typedef CVisMatrix2TC<TCoord> T_Matrix;
	typedef CVisPoint2TC<TCoord> T_Point;

	// Luciform class constructors
    CVisPoint2TC( void );
//    CVisPoint2TC( const TCoord& x );
    CVisPoint2TC( const TCoord& u, const TCoord& v );

	// Constructor and assignment operator using the base class
    CVisPoint2TC( const GXPoint2TC<TCoord>& refobj );
    CVisPoint2TC<TCoord>& operator=( const GXPoint2TC<TCoord>& refobj );

	// Constructor and assignment operator using the (general) CVisDVector class
    CVisPoint2TC( const CVisDVector& refvector );
    CVisPoint2TC<TCoord>& operator=( const CVisDVector& refvector );

	// Constructor and assignment operator using the old CVisVector4 class
    CVisPoint2TC( const CVisVector4& refvector );
    CVisPoint2TC<TCoord>& operator=( const CVisVector4& refvector );
};



template<class TCoord>
class CVisVector3TC : public GXVector3TC<TCoord>
{
public:
	enum { dim = 3 };

	// Luciform class constructors
    CVisVector3TC( void );
    CVisVector3TC( const TCoord& d );
    CVisVector3TC( const TCoord& dx, const TCoord& dy, const TCoord& dz );

	// Constructor and assignment operator using the base class
    CVisVector3TC( const GXVector3TC<TCoord>& refobj );
    CVisVector3TC<TCoord>& operator=( const GXVector3TC<TCoord>& refobj );

	// Constructor and assignment operator using the (general) CVisDVector class
    CVisVector3TC( const CVisDVector& refvector );
    CVisVector3TC<TCoord>& operator=( const CVisDVector& refvector );

	// Constructor and assignment operator using the old CVisVector4 class
    CVisVector3TC( const CVisVector4& refvector );
    CVisVector3TC<TCoord>& operator=( const CVisVector4& refvector );
};


template<class TCoord>
class CVisCoVector3TC : public GXCoVector3TC<TCoord>
{
public:
	enum { dim = 3 };

	// Luciform class constructors
    CVisCoVector3TC( void );
    CVisCoVector3TC( const TCoord& n );
    CVisCoVector3TC( const TCoord& nx, const TCoord& ny, const TCoord& nz );

	// Constructor and assignment operator using the base class
    CVisCoVector3TC( const GXCoVector3TC<TCoord>& refobj );
    CVisCoVector3TC<TCoord>& operator=( const GXCoVector3TC<TCoord>& refobj );

	// Constructor and assignment operator using the (general) CVisDVector class
    CVisCoVector3TC( const CVisDVector& refvector );
    CVisCoVector3TC<TCoord>& operator=( const CVisDVector& refvector );

	// Constructor and assignment operator using the old CVisVector4 class
    CVisCoVector3TC( const CVisVector4& refvector );
    CVisCoVector3TC<TCoord>& operator=( const CVisVector4& refvector );


	// Multiplication with vectors and matrices
	TCoord operator*( const GXVector3TC<TCoord>& refvector ) const;
	CVisCoVector3TC<TCoord> operator*( const GXMatrix3TC<TCoord>& refmatrix ) const;
	CVisCoVector3TC<TCoord>& operator*=( const GXMatrix3TC<TCoord>& refmatrix );
};


template<class TCoord>
class CVisMatrix3TC : public GXMatrix3TC<TCoord>
{
public:
	enum { dim = 3 };

	// Luciform class constructors
    CVisMatrix3TC( void );
    CVisMatrix3TC( const TCoord& s );
    CVisMatrix3TC( const GXVector3TC<TCoord>& vCol1,
			const GXVector3TC<TCoord>& vCol2,
			const GXVector3TC<TCoord>& vCol3 );
    CVisMatrix3TC( const GXCoVector3TC<TCoord>& covRow1,
			const GXCoVector3TC<TCoord>& covRow2,
			const GXCoVector3TC<TCoord>& covRow3 );

	// Constructor and assignment operator using the base class
    CVisMatrix3TC( const GXMatrix3TC<TCoord>& refobj );
    CVisMatrix3TC<TCoord>& operator=( const GXMatrix3TC<TCoord>& refobj );

	// Constructor and assignment operator using the (general) CVisDMatrix class
    CVisMatrix3TC( const CVisDMatrix& refmatrix );
    CVisMatrix3TC<TCoord>& operator=( const CVisDMatrix& refmatrix );

	// Constructor and assignment operator using the old CVisTransform4x4 class
    CVisMatrix3TC( const CVisTransform4x4& refmatrix );
    CVisMatrix3TC<TCoord>& operator=( const CVisTransform4x4& refmatrix );


	// Multiplication with vectors
	CVisVector3TC<TCoord> operator*( const GXVector3TC<TCoord>& refvector ) const;


	// Return the transpose of this matrix
	CVisMatrix3TC<TCoord> Transposed(void) const;

	// Change this matrix to its transpose.  Return a reference to this matrix.
	CVisMatrix3TC<TCoord>& Transpose(void);

	// Change this matrix to the transpose of the specified matrix.
	// Return a reference to this matrix.
	CVisMatrix3TC<TCoord>& Transpose(const CVisMatrix3TC<TCoord>& refmatrix);


	// LATER:  Add an AppRot method.  (And other CVisTransform4x4 methods?)


	// Return the inverse of this matrix
	CVisMatrix3TC<TCoord> Inverted(void) const;

	// Change this matrix to its inverse.  Return a reference to this matrix.
	CVisMatrix3TC<TCoord>& Inverse(void);

	// Change this matrix to the inverse of the specified matrix.
	// Return a reference to this matrix.
	CVisMatrix3TC<TCoord>& Inverse(const CVisMatrix3TC<TCoord>& refmatrix);


protected:
	// Helper functions
	void CopyMatrix( const CVisDMatrix& refmatrix );
	void CopyMatrix( const CVisTransform4x4& refmatrix );
};


template<class TCoord>
class CVisPoint3TC : public GXPoint3TC<TCoord>
{
public:
	enum { dim = 3 };

	// Helpful typedefs
	typedef CVisVector3TC<TCoord> T_Vector;
	typedef CVisCoVector3TC<TCoord> T_CoVector;
	typedef CVisMatrix3TC<TCoord> T_Matrix;
	typedef CVisPoint3TC<TCoord> T_Point;

	// Luciform class constructors
    CVisPoint3TC( void );
    CVisPoint3TC( const TCoord& x );
    CVisPoint3TC( const TCoord& x, const TCoord& y, const TCoord& z );

	// Constructor and assignment operator using the base class
    CVisPoint3TC( const GXPoint3TC<TCoord>& refobj );
    CVisPoint3TC<TCoord>& operator=( const GXPoint3TC<TCoord>& refobj );

	// Constructor and assignment operator using the (general) CVisDVector class
    CVisPoint3TC( const CVisDVector& refvector );
    CVisPoint3TC<TCoord>& operator=( const CVisDVector& refvector );

	// Constructor and assignment operator using the old CVisVector4 class
    CVisPoint3TC( const CVisVector4& refvector );
    CVisPoint3TC<TCoord>& operator=( const CVisVector4& refvector );
};



template<class TCoord>
class CVisVector4TC : public GXVector4TC<TCoord>
{
public:
	enum { dim = 4 };

	// Luciform class constructors
    CVisVector4TC( void );
    CVisVector4TC( const TCoord& d );
    CVisVector4TC( const TCoord& dx, const TCoord& dy, const TCoord& dz,
			const TCoord& dw );

	// Constructor and assignment operator using the base class
    CVisVector4TC( const GXVector4TC<TCoord>& refobj );
    CVisVector4TC<TCoord>& operator=( const GXVector4TC<TCoord>& refobj );

	// Constructor and assignment operator using the (general) CVisDVector class
    CVisVector4TC( const CVisDVector& refvector );
    CVisVector4TC<TCoord>& operator=( const CVisDVector& refvector );

	// Constructor and assignment operator using the old CVisVector4 class
    CVisVector4TC( const CVisVector4& refvector );
    CVisVector4TC<TCoord>& operator=( const CVisVector4& refvector );
};


template<class TCoord>
class CVisCoVector4TC : public GXCoVector4TC<TCoord>
{
public:
	enum { dim = 4 };

	// Luciform class constructors
    CVisCoVector4TC( void );
    CVisCoVector4TC( const TCoord& n );
    CVisCoVector4TC( const TCoord& nx, const TCoord& ny, const TCoord& nz,
			const TCoord& nw );

	// Constructor and assignment operator using the base class
    CVisCoVector4TC( const GXCoVector4TC<TCoord>& refobj );
    CVisCoVector4TC<TCoord>& operator=( const GXCoVector4TC<TCoord>& refobj );

	// Constructor and assignment operator using the (general) CVisDVector class
    CVisCoVector4TC( const CVisDVector& refvector );
    CVisCoVector4TC<TCoord>& operator=( const CVisDVector& refvector );

	// Constructor and assignment operator using the old CVisVector4 class
    CVisCoVector4TC( const CVisVector4& refvector );
    CVisCoVector4TC<TCoord>& operator=( const CVisVector4& refvector );


	// Multiplication with vectors and matrices
	TCoord operator*( const GXVector4TC<TCoord>& refvector ) const;
	CVisCoVector4TC<TCoord> operator*( const GXMatrix4TC<TCoord>& refmatrix ) const;
	CVisCoVector4TC<TCoord>& operator*=( const GXMatrix4TC<TCoord>& refmatrix );
};


template<class TCoord>
class CVisMatrix4TC : public GXMatrix4TC<TCoord>
{
public:
	enum { dim = 4 };

	// Luciform class constructors
    CVisMatrix4TC( void );
    CVisMatrix4TC( const TCoord& s );
    CVisMatrix4TC( const GXVector4TC<TCoord>& vCol1,
			const GXVector4TC<TCoord>& vCol2,
			const GXVector4TC<TCoord>& vCol3,
			const GXVector4TC<TCoord>& vCol4 );
    CVisMatrix4TC( const GXCoVector4TC<TCoord>& covRow1,
			const GXCoVector4TC<TCoord>& covRow2,
			const GXCoVector4TC<TCoord>& covRow3,
			const GXCoVector4TC<TCoord>& covRow4 );

	// Constructor and assignment operator using the base class
    CVisMatrix4TC( const GXMatrix4TC<TCoord>& refobj );
    CVisMatrix4TC<TCoord>& operator=( const GXMatrix4TC<TCoord>& refobj );

	// Constructor and assignment operator using the (general) CVisDMatrix class
    CVisMatrix4TC( const CVisDMatrix& refmatrix );
    CVisMatrix4TC<TCoord>& operator=( const CVisDMatrix& refmatrix );

	// Constructor and assignment operator using the old CVisTransform4x4 class
    CVisMatrix4TC( const CVisTransform4x4& refmatrix );
    CVisMatrix4TC<TCoord>& operator=( const CVisTransform4x4& refmatrix );


	// Multiplication with vectors
	CVisVector4TC<TCoord> operator*( const GXVector4TC<TCoord>& refvector ) const;


	// Return the transpose of this matrix
	CVisMatrix4TC<TCoord> Transposed(void) const;

	// Change this matrix to its transpose.  Return a reference to this matrix.
	CVisMatrix4TC<TCoord>& Transpose(void);

	// Change this matrix to the transpose of the specified matrix.
	// Return a reference to this matrix.
	CVisMatrix4TC<TCoord>& Transpose(const CVisMatrix4TC<TCoord>& refmatrix);


	// Return the inverse of this matrix
	CVisMatrix4TC<TCoord> Inverted(void) const;

	// Change this matrix to its inverse.  Return a reference to this matrix.
	CVisMatrix4TC<TCoord>& Inverse(void);

	// Change this matrix to the inverse of the specified matrix.
	// Return a reference to this matrix.
	CVisMatrix4TC<TCoord>& Inverse(const CVisMatrix4TC<TCoord>& refmatrix);


	// transpose 3rd & 4th row and column
	CVisMatrix4TC<TCoord> TransposedZW(void) const;
	CVisMatrix4TC<TCoord>& TransposeZW(void);

	// transpose 3rd & 4th row
	CVisMatrix4TC<TCoord> TransposedZWRow(void) const;
	CVisMatrix4TC<TCoord>& TransposeZWRow(void);

	// Approximate rotation matrix
	CVisMatrix4TC<TCoord> AppRot(void) const;


	// Compute a projection of a vector defined by a this transform.
	CVisHPoint3TC<TCoord> Project(const GXHPoint3TC<TCoord>& v) const;

	// Project both endpoints of a line, and then clip
	void ProjectLine(const GXHPoint3TC<TCoord>& v0,
			const GXHPoint3TC<TCoord>& v1, GXHPoint3TC<TCoord>& e0,
			GXHPoint3TC<TCoord>& e1,
			TCoord max_size = TCoord(1 << 13)) const;


protected:
	// Helper functions
	void CopyMatrix( const CVisDMatrix& refmatrix );
	void CopyMatrix( const CVisTransform4x4& refmatrix );
};


template<class TCoord>
class CVisPoint4TC : public GXPoint4TC<TCoord>
{
public:
	enum { dim = 4 };

	// Helpful typedefs
	typedef CVisVector4TC<TCoord> T_Vector;
	typedef CVisCoVector4TC<TCoord> T_CoVector;
	typedef CVisMatrix4TC<TCoord> T_Matrix;
	typedef CVisPoint4TC<TCoord> T_Point;

	// Luciform class constructors
    CVisPoint4TC( void );
    CVisPoint4TC( const TCoord& x );
    CVisPoint4TC( const TCoord& x, const TCoord& y, const TCoord& z,
			const TCoord& w );

	// Constructor and assignment operator using the base class
    CVisPoint4TC( const GXPoint4TC<TCoord>& refobj );
    CVisPoint4TC<TCoord>& operator=( const GXPoint4TC<TCoord>& refobj );

	// Constructor and assignment operator using the (general) CVisDVector class
    CVisPoint4TC( const CVisDVector& refvector );
    CVisPoint4TC<TCoord>& operator=( const CVisDVector& refvector );

	// Constructor and assignment operator using the old CVisVector4 class
    CVisPoint4TC( const CVisVector4& refvector );
    CVisPoint4TC<TCoord>& operator=( const CVisVector4& refvector );
};




class CVisQuaternion : public GXQuaternion
{
public:
	// Luciform class constructors
    CVisQuaternion( void );
    CVisQuaternion( const GXVector3& v, float f );

	// Constructor and assignment operator using the base class
    CVisQuaternion( const GXQuaternion& refobj );
    CVisQuaternion& operator=( const GXQuaternion& refobj );
};


template<class TMatrix, class _A = std::allocator<TMatrix> >
class CVisMatrixChain
{
public:
    //------------------------------------------------------------------
	// Type definition

	typedef std::deque<TMatrix, _A> T_Deque;
	typedef CVisMatrixChain<TMatrix, _A> T_This;


    //------------------------------------------------------------------
	// Constructors, assignement operators, and destructors

	CVisMatrixChain(void);
	CVisMatrixChain(const TMatrix& refmatrix);
	CVisMatrixChain<TMatrix, _A>& operator=(const TMatrix& refmatrix);

	
    //------------------------------------------------------------------
	// Memory management methods

	int Capacity(void) const;
	void Reserve(int n);

	void Resize(int n);
	int Size(void) const;
	int MaxSize(void) const;
	bool IsEmpty(void) const;

	
    //------------------------------------------------------------------
	// Access methods

	// (To allow this class to keep a current copy of the matrices that it
	// contains, it does not provide methods that return non-const references
	// to matrices in the chain.)

	const TMatrix& At(int i) const;
	const TMatrix& operator[](int i) const;

	const TMatrix& Front(void) const;
	const TMatrix& Back(void) const;

	const TMatrix *Begin(void) const;
	const TMatrix *End(void) const;

	
    //------------------------------------------------------------------
	// Modification methods

	// (To allow this class to keep a current copy of the matrices that it
	// contains, it does not provide methods that return non-const references
	// to matrices in the chain.)

	// If no matrix is specified, the identity matrix is used.
	T_This& SetFront(void);
	T_This& SetFront(const TMatrix& refmatrix);

	// If no matrix is specified, the identity matrix is used.
	T_This& SetI(int i);
	T_This& SetI(int i, const TMatrix& refmatrix);

	// If no matrix is specified, the identity matrix is used.
	T_This& SetBack(void);
	T_This& SetBack(const TMatrix& refmatrix);

	// If no matrix is specified, the identity matrix is used.
	T_This& PushFront(void);
	T_This& PushFront(const TMatrix& refmatrix);

	// If no matrix is specified, the identity matrix is used.
	T_This& PushBack(void);
	T_This& PushBack(const TMatrix& refmatrix);
	T_This& PushBack(const T_This& refmatrixchain);

	void PopFront(void);
	void PopBack(void);

	// If no matrix is specified, the identity matrix is used.
	T_This& Insert(int i);
	T_This& Insert(int i, int n);
	T_This& Insert(int i, const TMatrix& refmatrix);
	T_This& Insert(int i, int n, const TMatrix& refmatrix);

	// If no matrix is specified, the identity matrix is used.
	T_This& Assign(void);
	T_This& Assign(int n);
	T_This& Assign(const TMatrix& refmatrix);
	T_This& Assign(int n, const TMatrix& refmatrix);
	T_This& Assign(const T_This& refmatrixchain);
	T_This& Assign(const T_This& refmatrixchain, int iFirst, int iLim);

	void Clear(void);

	void Erase(int i);
	void Erase(int iFirst, int iLim);

	void Swap(T_This& refmatrixchain);


    //------------------------------------------------------------------
	// Matrix chain methods

	// Return a matrix equal to the product of the matrices in the chain.
	TMatrix ProductMatrix(void) const;

	// Invert this matrix chain.
	T_This& Invert(void);

	// Change the matrix chain to contain a single identity matrix.
	void SetToIdentity(void);


#ifdef LATER
	//------------------------------------------------------------------
	// Multiplication of the front matrix by standard matrices

	// UNDONE:  Should these be global functions, since they depend on
	// the type of matrices in the chain?

	T_This& TranslateFront(double tx, double ty, double tz = 0.0);

	T_This& RotateFront(double angle,
			double ax = 0.0, double ay = 0.0, double az = 1.0);

	T_This& ScaleFront(double sx, double sy, double sz = 1.0);
#endif // LATER


#ifdef LATER
    //------------------------------------------------------------------
	// Comparison operators (needed for use in STL containers)

	bool operator==(const T_This& reftransformchain) const;
	bool operator!=(const T_This& reftransformchain) const;
	bool operator<(const T_This& reftransformchain) const;
#endif // LATER

private:
	T_Deque m_deque;
	TMatrix m_matrixProduct;
	bool m_fValidProduct;
};



// Helper functions for converting Luciform matrices to CVisDMatrix
template<class TCoord>
void VisCopyMatrixToMatrix( const GXMatrix2TC<TCoord>& refmatrixSrc,
		CVisDMatrix& refmatrixDest );

template<class TCoord>
void VisCopyMatrixToMatrix( const GXMatrix3TC<TCoord>& refmatrixSrc,
		CVisDMatrix& refmatrixDest );

template<class TCoord>
void VisCopyMatrixToMatrix( const GXMatrix4TC<TCoord>& refmatrixSrc,
		CVisDMatrix& refmatrixDest );


// Helper functions to initialize vectors and matrices

template<class TCoord>
void VisSetToZero(GXPoint2TC<TCoord>& refpoint);
template<class TCoord>
void VisSetToZero(GXVector2TC<TCoord>& refvector);
template<class TCoord>
void VisSetToZero(GXCoVector2TC<TCoord>& refcovector);
template<class TCoord>
void VisSetToZero(GXMatrix2TC<TCoord>& refmatrix);
template<class TCoord>
void VisSetToIdentity(GXMatrix2TC<TCoord>& refmatrix);

template<class TCoord>
void VisSetToZero(GXPoint3TC<TCoord>& refpoint);
template<class TCoord>
void VisSetToZero(GXVector3TC<TCoord>& refvector);
template<class TCoord>
void VisSetToZero(GXCoVector3TC<TCoord>& refcovector);
template<class TCoord>
void VisSetToZero(GXMatrix3TC<TCoord>& refmatrix);
template<class TCoord>
void VisSetToIdentity(GXMatrix3TC<TCoord>& refmatrix);

template<class TCoord>
void VisSetToZero(GXPoint4TC<TCoord>& refpoint);
template<class TCoord>
void VisSetToZero(GXVector4TC<TCoord>& refvector);
template<class TCoord>
void VisSetToZero(GXCoVector4TC<TCoord>& refcovector);
template<class TCoord>
void VisSetToZero(GXMatrix4TC<TCoord>& refmatrix);
template<class TCoord>
void VisSetToIdentity(GXMatrix4TC<TCoord>& refmatrix);

// With homogeneous coordinates, these functions set the last coordinate
// to one and all other coordinates to zero.
template<class TCoord>
void VisSetToZero(GXHPoint2TC<TCoord>& refhpoint);
template<class TCoord>
void VisSetToZero(GXHPoint3TC<TCoord>& refhpoint);
template<class TCoord>
void VisSetToZero(GXHPlane3TC<TCoord>& refhplane);


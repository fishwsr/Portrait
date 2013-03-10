///////////////////////////////////////////////////////////////////////////
// @doc INTERNAL EXTERNAL VISTRANSFORM4X4
// @module VisTransform4x4.h |
//
//  The CVisTransform4x4 class represents geometric transformation
//  in 4-D homogeneous coordinates (projective space), in a manner
//  similar to that used in 3D graphics packages such as
//  OpenGL.  A 4x4 homogeneous coordinate matrix is used to represent
//  the mapping between one or more points in the world to each
//  screen pixel.
//
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////

#ifndef __VIS_MATRIX_TRANSFORM4X4_H__
#define __VIS_MATRIX_TRANSFORM4X4_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include <stdio.h>


#ifndef VisMatrixExport
#define VisMatrixExport __declspec(dllimport)
#endif // VisMatrixExport

// Forward declarations
class CVisTransform4x4;
class CVisPropList;


// UNDONE:  Add EqSum, EqDiff, and EqProd methods like those in VisDMatrix.*.


////////////////////////////////////////////////////////////////////////////
//  
// @class
//
// This class is used to represent vectors in 3 dimensions.  These
// Vectors contain 4 homogeneous coordinates so that they can be used
// with 4x4 transform matrices (common in computer graphics systems like
// Open GL).  Only the first three coordinates of these vectors are
// used in the methods that perform arithmetic computations.
//  
////////////////////////////////////////////////////////////////////////////
class CVisVector4 
{
public:
	//------------------------------------------------------------------
	// @group Constructors

    CVisVector4(bool fInitialize = true);  // Init to 0 vector
    CVisVector4(double x, double y, double z = 0.0, double w = 1.0);


	//------------------------------------------------------------------
	// @group Indexing operators

    double operator[](int i) const;
    double& operator[](int i);


	//------------------------------------------------------------------
	// @group Arithmetic operators

	CVisVector4 operator+(const CVisVector4& refvector) const;
	CVisVector4 operator-(const CVisVector4& refvector) const;
	double operator*(const CVisVector4& refvector) const;
	CVisVector4 operator*(double dbl) const;
	CVisVector4 operator/(double dbl) const;

	CVisVector4& operator-(void);
	CVisVector4& operator+=(const CVisVector4& refvector);
	CVisVector4& operator-=(const CVisVector4& refvector);
	CVisVector4& operator*=(double dbl);
	CVisVector4& operator/=(double dbl);

	CVisVector4 Crossed(const CVisVector4& refvector) const;
	CVisVector4& Cross(const CVisVector4& refvector);

	VisMatrixExport CVisVector4& EqSum(const CVisVector4 &refvectorA,
			const CVisVector4 &refvectorB);
	VisMatrixExport CVisVector4& EqDiff(const CVisVector4 &refvectorA,
			const CVisVector4 &refvectorB);
	VisMatrixExport CVisVector4& EqCross(const CVisVector4 &refvectorA,
			const CVisVector4 &refvectorB);
	VisMatrixExport CVisVector4& EqProd(const CVisTransform4x4 &reftransformA,
			const CVisVector4 &refvectorB);


	//------------------------------------------------------------------
	// @group Standard vector operations

	double Magnitude(void) const;
	double MagnitudeSquared(void) const;

	VisMatrixExport CVisVector4 Normalized(void) const;
	VisMatrixExport CVisVector4& Normalize(void);


	//------------------------------------------------------------------
	// @group File I/O

    // Self-describing input/output format
    enum FieldType { eftName, eftData, eftEnd};
    const char *ReadWriteField(CVisSDStream& s, int field_id);

	// Old file I/O methods.  These may not be supported in future releases.
	VisMatrixExport void Read (FILE *stream);
    VisMatrixExport void Write(FILE *stream, int indent = 0);

	VisMatrixExport void BuildPropList(CVisPropList& refproplist);


    //------------------------------------------------------------------
	// @group Comparison operators (needed for use in STL containers)

	VisMatrixExport bool operator==(const CVisVector4& refvector) const;
	VisMatrixExport bool operator!=(const CVisVector4& refvector) const;
	VisMatrixExport bool operator<(const CVisVector4& refvector) const;

protected:
	double m_rgdbl[4];
};


////////////////////////////////////////////////////////////////////////////
//  
// @class
//
//  The CVisTransform4x4 class represents geometric transformation
//  in 4-D homogeneous coordinates (projective space), in a manner
//  similar to that used in 3D graphics packages such as
//  OpenGL.  A 4x4 homogeneous coordinate matrix is used to represent
//  the mapping between one or more points in the world to each
//  screen pixel.
//  
////////////////////////////////////////////////////////////////////////////
class CVisTransform4x4 
{
public:
	//------------------------------------------------------------------
	// @group Constructors

	VisMatrixExport CVisTransform4x4(bool fInitialize = true);  // Init to identity matrix
	// This ctor constructs a diagonal matrix.  Is it needed?
	VisMatrixExport CVisTransform4x4(double dbl00, double dbl11, double dbl22, double dbl33 = 0.0);


	//------------------------------------------------------------------
	// @group Indexing operators

    const double *operator[](int i) const;
    double *operator[](int i);
 

	//------------------------------------------------------------------
	// @group Multiplicative operators

	CVisTransform4x4 operator*(const CVisTransform4x4& reftransform4x4) const;
	CVisVector4 operator*(const CVisVector4& refvector) const;

	CVisTransform4x4& operator*=(const CVisTransform4x4& reftransform4x4);
 
	VisMatrixExport CVisTransform4x4& EqProd(const CVisTransform4x4 &reftransformA,
			const CVisTransform4x4 &reftransformB);


	//------------------------------------------------------------------
	// @group Multiplication by standard transforms

	CVisTransform4x4 Translated(double tx, double ty, double tz = 0.0) const;
	VisMatrixExport CVisTransform4x4& Translate(double tx, double ty, double tz = 0.0);

	VisMatrixExport CVisTransform4x4 Rotated(double angle,
			double ax = 0.0, double ay = 0.0, double az = 1.0) const;
	CVisTransform4x4& Rotate(double angle,
			double ax = 0.0, double ay = 0.0, double az = 1.0);

	CVisTransform4x4 Scaled(double sx, double sy, double sz = 1.0) const;
	VisMatrixExport CVisTransform4x4& Scale(double sx, double sy, double sz = 1.0);

	// LATER:  Do we want EqTranslated, EqRotated, EqScaled methods?


	//------------------------------------------------------------------
	// @group Standard matrix operations

	CVisTransform4x4 Normalized(void) const; // make t[3][3] == 1
	VisMatrixExport CVisTransform4x4& Normalize(void);

	VisMatrixExport CVisTransform4x4 Inverted(void) const; // matrix inverse
	CVisTransform4x4& Invert(void);

	CVisTransform4x4 Transposed(void) const;
	VisMatrixExport CVisTransform4x4& Transpose(void);

	// transpose 3rd & 4th row and column
	CVisTransform4x4 TransposedZW(void) const;
	VisMatrixExport CVisTransform4x4& TransposeZW(void);

	// transpose 3rd & 4th row
	CVisTransform4x4 TransposedZWRow(void) const;
	VisMatrixExport CVisTransform4x4& TransposeZWRow(void);

	// Approximate rotation matrix
	VisMatrixExport CVisTransform4x4 AppRot(void) const;

	// LATER:  Do we want EqNormalized, EqInverted,... methods?


	//------------------------------------------------------------------
	// @group Vector projections

	VisMatrixExport CVisVector4 ProjectVector(const CVisVector4& v) const;

	VisMatrixExport void ProjectLine(const CVisVector4& v0, const CVisVector4& v1,
			CVisVector4& e0, CVisVector4& e1,
			double max_size = double(1 << 13)) const;


	//------------------------------------------------------------------
	// @group File I/O

    // Self-describing input/output format
    enum FieldType { eftName, eftData, eftEnd};
    const char *ReadWriteField(CVisSDStream& s, int field_id);

	// Old file I/O methods.  These may not be supported in future releases.
	VisMatrixExport void Read (FILE *stream);
    VisMatrixExport void Write(FILE *stream, int indent = 0);

	VisMatrixExport void BuildPropList(CVisPropList& refproplist);


    //------------------------------------------------------------------
	// @group Comparison operators (needed for use in STL containers)

	VisMatrixExport bool operator==(const CVisTransform4x4& reftransform4x4) const;
	VisMatrixExport bool operator!=(const CVisTransform4x4& reftransform4x4) const;
	VisMatrixExport bool operator<(const CVisTransform4x4& reftransform4x4) const;

protected:
    double m_rgrgdbl[4][4];
};


typedef std::deque<CVisTransform4x4, std::allocator<CVisTransform4x4> > CVisT4L;


////////////////////////////////////////////////////////////////////////////
//  
// @class
//
// The CVisTransformChain class represents a chain of geometric
// transformations in 4-D homogeneous coordinates (projective space).
// It is derived from std::deque.  The front of the deque is the
// leftmost transform in the chain.  The back of the deque is the
// rightmost transform in the chain.  The copy constructors and
// assignment operators copy the transfroms in the chain.  (The
// memory used to store the transfroms is not shared between
// transform chains.)
//  
////////////////////////////////////////////////////////////////////////////
class CVisTransformChain : public CVisT4L
{
public:
    //------------------------------------------------------------------
	// @group Type definition

	typedef std::deque<CVisTransform4x4> T_Deque;


    //------------------------------------------------------------------
	// @group Constructors, assignement operators, and destructors

	CVisTransformChain(void);
	CVisTransformChain(const CVisTransform4x4& reftransform);
	CVisTransformChain(const CVisTransformChain& reftransformchain);
	CVisTransformChain& operator=(const CVisTransform4x4& reftransform);
	CVisTransformChain& operator=(const CVisTransformChain& reftransformchain);


    //------------------------------------------------------------------
	// @group Overloaded std::dqeue methods

	// This is like the std::deque push_front method, but it has a default
	// parameter and returns a reference to the modified transform chain.
	CVisTransformChain& push_front(
			const CVisTransform4x4& reftransform = CVisTransform4x4());

	// These are like the std::deque push_back method, but one overload
	// allows us to append a transform chain to this transform chain, and
	// both overloads return a reference to the modified transform chain.
	CVisTransformChain& push_back(const CVisTransform4x4& reftransform);
	CVisTransformChain& push_back(const CVisTransformChain& reftransformchain);


	//------------------------------------------------------------------
	// @group Multiplication of the front transform by standard transforms

	CVisTransformChain& Translate(double tx, double ty, double tz = 0.0);

	CVisTransformChain& Rotate(double angle,
			double ax = 0.0, double ay = 0.0, double az = 1.0);

	CVisTransformChain& Scale(double sx, double sy, double sz = 1.0);


    //------------------------------------------------------------------
	// @group Transform chain methods

	// Return a transformaion matrix equal to the product of the
	// transformations in the chain.
	VisMatrixExport CVisTransform4x4 CompleteTransform(void) const;

	// Return the transformation chain that is the inverse of this
	// transformation chain.
	VisMatrixExport CVisTransformChain Inverted(void) const;

	// Invert this transformation chain.
	CVisTransformChain& Invert(void);

	// Change the transform chain to contain a single identity transform.
	VisMatrixExport void Reset(void);


    //------------------------------------------------------------------
	// @group File I/O

	// LATER:  These methods may be removed in future releases.
	VisMatrixExport void Read (FILE *stream);
    VisMatrixExport void Write(FILE *stream, int indent = 0);

	VisMatrixExport void BuildPropList(CVisPropList& refproplist);

    // Self-describing input/output format
//    enum FieldType { eftName, eftData, eftEnd};
//    VisMatrixExport const char *ReadWriteField(CVisSDStream& s, int field_id);


    //------------------------------------------------------------------
	// @group Comparison operators (needed for use in STL containers)

	VisMatrixExport bool operator==(
			const CVisTransformChain& reftransformchain) const;
	bool operator!=(const CVisTransformChain& reftransformchain) const;
	bool operator<(const CVisTransformChain& reftransformchain) const;


	// LATER:  We might also want methods (PreMultiplyFront, TranslateFront,
	// RotateFront, and ScaleFront) that would modify the front transform in
	// the chain.  These methods might cause confusion with the const
	// CVisTransform4x4 methods that have similar names, so they are not
	// included in this release.  If we decide to add such methods in the
	// future, we need to make it clear that they modify the transform
	// chain while the CVisTransform4x4 methods do not modify the transform.
};


VisMatrixExport std::ostream& VISAPI operator<<(std::ostream& os, const CVisVector4& p);
//VisMatrixExport std::istream& VISAPI operator>>(std::istream& is, CVisVector4& p);

VisMatrixExport std::ostream& VISAPI operator<<(std::ostream& os, const CVisTransform4x4& m);
VisMatrixExport std::istream& VISAPI operator>>(std::istream& is, CVisTransform4x4& m);


// Self-describing stream output.
inline CVisSDStream& operator<<(CVisSDStream& s, CVisVector4& v);
inline CVisSDStream& operator<<(CVisSDStream& s, CVisTransform4x4& v);
inline CVisSDStream& operator<<(CVisSDStream& s, CVisTransformChain& v);


VIS_DECLARE_SD_CLASS_WITH_IO_PL_TYPEDEF(VisMatrixExportDeclspec, CVisVector4)
VIS_DECLARE_SD_CLASS_WITH_IO_PL_TYPEDEF(VisMatrixExportDeclspec, CVisTransform4x4)
VIS_DECLARE_PROP_LIST_CLASS_DEQUE(VisMatrixExportDeclspec, CVisTransform4x4)
VIS_DECLARE_SD_CLASS_WITH_IO_PL_TYPEDEF(VisMatrixExportDeclspec, CVisTransformChain)


#include "VisTransform4x4.inl"


#endif // __VIS_MATRIX_TRANSFORM4X4_H__

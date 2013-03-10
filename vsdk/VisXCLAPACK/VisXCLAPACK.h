// VisXCLAPACK.h
//
// DESCRIPTION
//  The functions in this file provide C++ calling convention
//  covers for the CLAPACK routines we want to use.  They also
//  reside in a DLL, which removes the need to link
//  in the Fortran libraries with the other modules,
//  and avoids link-time errors (clashes between Fortran and MFC).
//
// HISTORY
//  11-May-98  Zhengyou Zhang (zhang) at Microsoft
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////


#ifndef __VIS_XCLAPACK_H__
#define __VIS_XCLAPACK_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#ifdef VIS_SDK_LIBS
#define VIS_XCLA_LIB
#else // VIS_SDK_LIBS
	#ifndef VIS_XCLA_DLL
		#define VIS_XCLA_DLL
	#endif // !VIS_XCLA_DLL
#endif // VIS_SDK_LIBS

#ifdef VIS_XCLA_DLL
	#define VIS_XCLA_BUILD
#endif // VIS_XCLA_DLL

// Version information
#include "VisXCLAPACKVersion.h"

#ifdef VIS_XCLA_LIB
#define VisXCLAPACKExport			
#elif defined(VIS_XCLA_BUILD)
#define VisXCLAPACKExport			_declspec(dllexport)
#else // VIS_XCLA_LIB
#define VisXCLAPACKExport			_declspec(dllimport)
#endif // VIS_XCLA_LIB

// Accessor function.
inline class CVisXCLAPACK *VisFindPXCLAPACK(void);

// Class containing exported functions
// (All methods except the constructor should be virtual.)
class CVisXCLAPACK
{
public:
	CVisXCLAPACK(void) {};

	// compute the inverse of 'a', and its determinant if p_det != NULL
	// if 'a' is not inversible, then *p_det will get 0, or an exception will
	//    be thrown if p_det == NULL
	// note that 'a' will be overwritten
	virtual int Dgetrf(const int m, double *a, double *p_det);
	// compute the SVD of a mxn matrix 'a', which will be destroyed 
	// jobu = 'N' if U is not required; otherwise, set jobu = 'A'. Same for jobv
	// U should be mxm; V should be nxn; S should be min(m,n) 
	virtual int Dgesvd(const int m, const int n, double *a, const char jobu,
		const char jobv, double *U, double *S, double *V);
	// compute all eigenvalues, and optionally (if 'vects' != 0), eigenvectors, 
	//     of a real symmetric matrix
	// the eigenvalues are in ascending order
	virtual int Dsyev(const int m, double *a, double *vals, double *vects);
	// compute the selected eigenvalues, and optionally (if 'vects' != 0), eigenvectors, 
	//     of a real symmetric matrix
	// they are selected by specifying the range of indices: 1 <= indLow <= indUp <= m
	// the eigenvalues are in ascending order
	virtual int Dsyevx(
		const int m, 
		double *a, 
		double *vals, // must be of: indUp - indLow + 1
		double *vects, // set to 0, if not required. m x (indUp - indLow + 1)
		const int indLow,
		const int indUp
		);
	// computes the solution to a real system of linear equations
	//     A * X = B,
	//  where A is an N-by-N matrix and X and B are N-by-NRHS matrices.
	virtual int Dgesv(
		const int n,
		double *AT,  // input the transpose
		const int nrhs,
		const double *BT,  // input the transpose
		double *XT   // return the result in transpose
		);
	// computes the solution to a real system of linear equations
	//     A * X = B,
	//  where A is an N-by-N symmetric positive definite matrix and X and B
	//  are N-by-NRHS matrices.
	virtual int Dposv(
		const int n,
		double *A,  
		const int nrhs,
		const double *BT,  // input the transpose
		double *XT   // return the result in transpose
		);
	// solves overdetermined or underdetermined real linear systems
	//  involving an M-by-N matrix A, or its transpose, using a QR or LQ
	//  factorization of A.  It is assumed that A has full rank.
	virtual int Dgels(
		const int m,
		const int n,
		double *AT,  // input transpose
		const int nrhs,
		double *BT, // input transpose
		double *X   // return the result (each column is a solution)
		);
	// Computes the minimum norm least squares solution to an over-
	//    or under-determined system of linear equations A X=B,  using
	//   the singular value decomposition of A.
	virtual int Dgelss(
		const int m,
		const int n,
		double *AT,  // input transpose
		const int nrhs,
		const double *BT, // input transpose
		double *sVals, // dimension (min(M,N))
		//         The singular values of A in decreasing order.
		//         The condition number of A in the 2-norm = S(1)/S(min(m,n)).
		int *pRank, // the effective rank of A
		double *X   // return the result (each column is a solution)
		);
	// solves the linear equality-constrained least squares (LSE) problem:
	//
	//          minimize || c - A*x ||_2   subject to   B*x = d
	//
	//  where A is an M-by-N matrix, B is a P-by-N matrix, c is a given
	//  M-vector, and d is a given P-vector. It is assumed that
	//  P <= N <= M+P, and
	//
	//           rank(B) = P and  rank( ( A ) ) = N.
	//                                ( ( B ) )
	//
	//  These conditions ensure that the LSE problem has a unique solution,
	//  which is obtained using a GRQ factorization of the matrices B and A.
	virtual int Dgglse(
		const int m,
		const int n,
		const int p,
		double *AT, // input transpose
		double *BT, // input transpose
		double *c, // dim m
		double *d, // dim p
		double *x  // dim n
		);
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
	virtual	int // on exit, return 'info'
	Dgeev(
		const int N,
		double *aT, // because of Fortran
		double *wr,
		double *wi,
		double *vlT, // because of Fortran
		double *vrT // because of Fortran
	);
};

#ifdef VIS_XCLA_LIB

class CVisXCLAPACKInit
{
public:
    CVisXCLAPACKInit();
};

#endif // VIS_XCLA_LIB

#include "VisXCLAPACK.inl"


#endif // __VIS_XCLAPACK_H__

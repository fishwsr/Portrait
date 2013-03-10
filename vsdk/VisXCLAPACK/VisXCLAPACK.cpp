// VisXCLAPACK.cpp
//
// DESCRIPTION
//  The functions in this file provide C++ calling convention
//  covers for the CLAPACK routines we want to use.  They also
//  reside in a DLL, which removes the need to link
//  in the Fortran libraries with the other modules,
//  and avoids link-time errors (clashes between Fortran and MFC).
//
// UNDONE:  Right now, we're using the same CLAPACK LIBs in both
// the debug and release builds.  We should build two versions of
// the CLAPACK LIBs, one for release builds and another for debug
// builds.  (erikhan)
//
// HISTORY
//  11-May-98  Zhengyou Zhang (zhang) at Microsoft
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////

// Define these to skip parts of the Windows headers that we don't need.
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN


// Use MFC in the debug build for its debug memory packag.
// UNDONE:  We might need to add DLLMain code when we're an MFC extension DLL.
#if defined(_DEBUG) && !defined(VIS_NO_MFC)
#include <afxwin.h>         // MFC core and standard components
#else // _DEBUG && !VIS_NO_MFC
#include <windows.h>
#endif // _DEBUG && !VIS_NO_MFC


#include <assert.h>

#include "VisXCLAPACK.h"

#ifdef VIS_XCLA_LIB
#include "VisCore.h"
#endif // VIS_XCLA_LIB

#if defined(_DEBUG) && !defined(VIS_NO_MFC)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG && !VIS_NO_MFC


// the CLAPACK functions to be used in CVisXCLAPACK
extern "C" {
	extern void dgetrf_(const int *m, const int *n, double *a, const int *lda,
		    int *ipiv, int *info);
	extern void dgetri_(const int *n, double *a, const int *lda, int *ipiv,
		    double *work, int *lwork, int *info);
	extern int dgesvd_(const char *jobu, const char *jobvt, const int *m,
		   const int *n, double *a, const int *lda, double *s,
		   double *u, const int *ldu, double *vt, const int *ldvt,
		   double *work, const int *lwork, int *info);
	extern void dsyev_(const char *, const char *, const int *, double *,
                   const int *, double *, double *, const int *,
                   int *);
	extern void dgelss_(const int *, const int *, const int *, 
                    double *, const int *, double *, const int *, 
                    double *, double *, int *, double *, 
                    const int *, int *);
	extern void dsyevx_(const char * JOBZ, const char * RANGE, const char * UPLO, 
		const int *N, double *A, const int *LDA, const double *VL, const double *VU,
		const int *IL, const int *IU, const double *ABSTOL, int *M, double *W, 
		double *Z, const int *LDZ, double *WORK, const int *LWORK, int *IWORK,
		int *IFAIL, int *INFO);
	extern void dgesv_(const int *N, const int *NRHS, double *A, const int *LDA, 
		int *IPIV, double *B, const int *LDB, int *INFO);
	extern void dposv_(const char *UPLO, const int *N, const int *NRHS, double *A, 
		const int *LDA, double *B, const int *LDB, int *INFO);
	extern void dgels_(const char *TRANS, const int *M, const int *N, const int *NRHS,
		double *A, const int *LDA, double *B, const int *LDB, double *WORK, 
		const int *LWORK, int *INFO);
	extern void dgglse_(const int *M, const int *N, const int *P, 
		double *A, const int *LDA, double *B, const int *LDB, double *C, double *D, 
		double *X, double *WORK, const int *LWORK, int *INFO);
	extern void dgeev_(const char *, const char *, const int *, double *,
                   const int *, double *, double *, double *, 
                   const int *, double *, const int *, 
                   double *, const int *, int *);
	extern double dlamch_(const char *ch);
}

// Global CVisXCLAPACK variable exported by the DLL.
VisXCLAPACKExport CVisXCLAPACK g_xclapack;


// compute the inverse of a square matrix 'a', and its determinant if p_det != 0
// if 'a' is not inversible, then *p_det will get 0, or returns a nonzero value
//    if p_det == 0
// note that 'a' will be overwritten
int CVisXCLAPACK::Dgetrf(
	const int m, 
	double *a, 
	double *p_det
	)
{
	int info;
	int *ipiv = new int[m];
	// compute the LU factorization
	dgetrf_ (&m, &m, a, &m, ipiv, &info);
	if (info > 0) { // not inversible
		if (p_det != 0)
			*p_det = 0;
		delete [] ipiv;
		return info;
	}
	// compute the determinant
	if (p_det != 0) {
		*p_det = 1.0;
		int i;
		for (i = 0; i < m; i++)
			*p_det *= a[i+i*m]; // product of diagonal elements
		// we have to multiply by the signature of the permutation in ipiv
		//    to get the correct sign
		for (i = 0; i < m; i++)
			if (ipiv[i] != (i + 1))
				*p_det *= -1.0;
	}
	// compute the inverse
	int lwork = m * 5; // must be >= max(1, m)
	double *work = new double[lwork];
	dgetri_ (&m, a, &m, ipiv, work, &lwork, &info);
	delete [] ipiv;
	delete [] work;
	return info;
}

// compute the SVD of a MxN matrix 'a'
// jobu = 'N' if U is not required; otherwise, set jobu = 'A'. Same for jobv
// U should be MxM; V should be NxN; S should be min(M,N) 
int CVisXCLAPACK::Dgesvd(
	 const int M, 
	 const int N, 
	 double *a, 
	 const char jobu,
	 const char jobv, 
	 double *U, 
	 double *S, 
	 double *V
	 )
{
	double *u, *vt;
    int ldu, ldvt;

	// note Fortran matrix is the transpose of C matrix
	// So, U = v, V = u
    if (jobv == 'N') {
		ldu = 1;
		u = 0;
    }
    else {
		ldu = N;
		u = new double[N*N];
    }
    if (jobu == 'N') {
		ldvt = 1;
		vt = 0;
	}
    else {
		ldvt = M;
		vt = U;
    }

    int MN = (M < N) ? M : N;
    double *s = S;

    int lwork = 6*MN + M + N; // larger than necessary
	double *work = new double[lwork];

    int lda = N;
    
    int info = 0;
    dgesvd_ (&jobv, &jobu, &N, &M, a, &lda, s, u, &ldu, vt, &ldvt,
             work, &lwork, &info);

	delete [] work;
    if (!info) {
		// LAPACK outputs the transpose of U
		if (jobv != 'N') {
			for (int i = 0; i < N; i++)
				for (int j = 0; j < N; j++)
					V[i + j*N] = u[j + i*N];
			delete [] u;
		}
		return 0;
    }
    return 1;
}

// compute all eigenvalues, and optionally (if 'vects' != 0), eigenvectors, 
//     of a real symmetric matrix
// the eigenvalues are in ascending order
int CVisXCLAPACK::Dsyev(
	const int m, 
	double *a, 
	double *vals, 
	double *vects
	)
{
	if (m < 1) return 1;
	char jobz;
	char uplo = 'U';
	int info, lda = m;
	int lwork = 10*m; // should be >= max(1,3*m-1)
	double *work = new double[lwork];
	double *V;
	jobz = vects ? 'V' : 'N';
	V = new double[m*m];
	memcpy(V, a, m*m*sizeof(double));
	dsyev_(&jobz, &uplo, &m, V, &lda, vals, work, &lwork, &info);
	if (vects) {
		for (int i = 0; i < m; i++)
			for (int j = 0; j < m; j++)
				vects[i + j*m] = V[j + i*m];
	}

	delete [] work;
	delete [] V;
	return info;
}

// compute the selected eigenvalues, and optionally (if 'vects' != 0), eigenvectors, 
//     of a real symmetric matrix
// they are selected by specifying the range of indices: 1 <= indLow <= indUp <= m
// the eigenvalues are in ascending order
int CVisXCLAPACK::Dsyevx(
	const int m, 
	double *a, 
	double *vals, // must be of: indUp - indLow + 1
	double *vects, // set to 0, if not required. m x (indUp - indLow + 1)
	const int indLow,
	const int indUp
	)
{
	if (m < 1) return 1;
	char jobz;
	char range = 'I'; // the IL-th through IU-th eigenvalues will be found
	char uplo = 'U';
	int info, lda = m;
	int lwork = 16*m; // should be >= max(1,8*m)
	double *work = new double[lwork];
	jobz = vects ? 'V' : 'N';
	char charS = 'S';
	double abstol = 2.0 * dlamch_(&charS);
	int numbReturned;
	double *W = new double[m];
	int ldz = m;
	double *z = new double[ldz * (indUp - indLow + 1)];
	int *iwork = new int[5*m];
	int *ifail = new int[m];
	dsyevx_(&jobz, &range, &uplo, &m, a, &lda, 0, 0,
		&indLow, &indUp, &abstol, &numbReturned, W, 
		z, &ldz, work, &lwork, iwork, ifail, &info);
	for (int i = 0; i < numbReturned; i++)
		vals[i] = W[i];
	if (vects) {
		for (int i = 0; i < numbReturned; i++)
			for (int j = 0; j < m; j++)
				vects[i + j*numbReturned] = z[j + i*ldz];
	}
	delete [] ifail;
	delete [] iwork;
	delete [] z;
	delete [] W;
	delete [] work;
	return info;
}

// computes the solution to a real system of linear equations
//     A * X = B,
//  where A is an N-by-N matrix and X and B are N-by-NRHS matrices.
int CVisXCLAPACK::Dgesv(
	const int n,
	double *AT,  // input the transpose
	const int nrhs,
	const double *BT,  // input the transpose
	double *XT   // return the result in transpose
	)
{
	int *ipiv = new int[n];
	memcpy(XT, BT, n*nrhs*sizeof(double));
	int info;
	dgesv_(&n, &nrhs, AT, &n, ipiv, XT, &n, &info);
	delete [] ipiv;
	return info;
}

// computes the solution to a real system of linear equations
//     A * X = B,
//  where A is an N-by-N symmetric positive definite matrix and X and B
//  are N-by-NRHS matrices.
int CVisXCLAPACK::Dposv(
	const int n,
	double *A,  
	const int nrhs,
	const double *BT,  // input the transpose
	double *XT   // return the result in transpose
	)
{
	memcpy(XT, BT, n*nrhs*sizeof(double));
	int info;
	char uplo = 'U';
	dposv_(&uplo, &n, &nrhs, A, &n, XT, &n, &info);
	return info;
}

// solves overdetermined or underdetermined real linear systems
//  involving an M-by-N matrix A, or its transpose, using a QR or LQ
//  factorization of A.  It is assumed that A has full rank.
//   NOTE: X is column-row ordering (Fortran style)
//         To make row-column, uncomment for loops (which way??) --cmg
int CVisXCLAPACK::Dgels(
	const int m,
	const int n,
	double *AT,  // input transpose
	const int nrhs,
	double *BT, // input transpose
	double *X   // return the result (each column is a solution)
	)
{
	char trans = 'N';
	int lwork = 10 * (m + n + nrhs); // larger than necessary
	double *work = new double[lwork];
	int info;
	dgels_(&trans, &m, &n, &nrhs, AT, &m, BT, &m, work, &lwork, &info);

	for (int i = 0; i < nrhs; i++)
		for (int j = 0; j < n; j++)
			X[j*nrhs + i] = BT[i*m + j];

	delete [] work;
	return info;
}

// Computes the minimum norm least squares solution to an over-
//    or under-determined system of linear equations A X=B,  using
//   the singular value decomposition of A.
int CVisXCLAPACK::Dgelss(
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
	)
{
	double rcond = -1.0; // using machine precision
	int info = 0;
	int lwork = 5*m*n + nrhs; // larger than necessary
	double *work = new double[lwork];
	int ldb = (m > n) ? m : n;
	double *mat = new double[ldb * nrhs];
	for (int j = 0; j < nrhs; j++)
		for (int i = 0; i < m; i++)
			mat[i + j*ldb] = BT[i + j*m];
	dgelss_(&m, &n, &nrhs, AT, &m, mat, &ldb, sVals, &rcond, pRank,
		work, &lwork, &info);
	for (int k = 0; k < nrhs; k++)
		for (int l = 0; l < n; l++)
			X[l*nrhs+k] = mat[k*m + l];				
	delete [] mat;
	delete [] work;
	return info;
}

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
int CVisXCLAPACK::Dgglse(
	const int m,
	const int n,
	const int p,
	double *AT, // input transpose
	double *BT, // input transpose
	double *c, // dim m
	double *d, // dim p
	double *x  // dim n
	)
{
	int info;
	int lwork = p + (m + n) * 10;
	double *work = new double[lwork];
	dgglse_(&m, &n, &p, AT, &m, BT, &p, c, d, x, work, &lwork, &info);
	delete [] work;
	return info;
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
//  If the j-th eigenvalue is real, then u(j) = vectsL(:,j),   
//     the j-th column of vectsL.   
//  If the j-th and (j+1)-st eigenvalues form a complex   
//     conjugate pair, then u(j) = vectsL(:,j) + i*vectsL(:,j+1) and   
//      u(j+1) = vectsL(:,j) - i*vectsL(:,j+1). 
//  Similarly for the relationship between v(j) and vectsR.  
//
//     The computed eigenvectors are normalized to  have  Euclidean
//     norm equal to 1 and largest component real.
int // on exit, return 'info'
CVisXCLAPACK::Dgeev(
	const int N,
	double *aT, // because of Fortran
	double *wr,
	double *wi,
	double *vlT, // because of Fortran
	double *vrT // because of Fortran
)
{
    char jobvl;
    char jobvr;
    int ldvl, ldvr;
    int worksize = 5*N;
    int info = 0;
    double *pvl;
    double *pvr;
    double * work = new double[worksize];

	if (vlT == 0) {
		jobvl = 'N';
		ldvl = 1;
		pvl = 0;
    }
    else {
		jobvl = 'V';
		ldvl = N;
		pvl = vlT;
    }
	if (vrT == 0) {
		jobvr = 'N';
		ldvr = 1;
		pvr = 0;
    }
    else {
		jobvr = 'V';
		ldvr = N;
		pvr = vrT;
    }

	dgeev_(&jobvl, &jobvr, &N, aT, &N, wr, wi, pvl, &ldvl, pvr, &ldvr,
        work, &worksize, &info);

	delete [] work;

	return info;
}

#ifdef VIS_XCLA_LIB

// @mfunc Initializes the file handler.
CVisXCLAPACKInit::CVisXCLAPACKInit()
{
	PGlobals()->SetPXCLAPACK(&g_xclapack);
}

#endif // VIS_XCLA_LIB

// VisCovariance.cpp
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved

#include "VisMatrixPch.h"
#include "VisMatrixProj.h"


#ifndef M_PI
const double M_PI = 3.1415926535;
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#ifndef VIS_NO_LUC_FILES

// normalize a vector and 
// compute the corresponding covariance matrix if 'covV' is not a dumb matrix
void VisNormalizeVectorAndCovariance(
	const CVisVector2F & v,  // original vector
	const CVisMatrix2F &covV,
	CVisVector2F & u, // normalized vector (unit vector)
	CVisMatrix2F & covU
	)
{
	float norm = GXLength(v);
	u = v / norm;

	// Jacobian = (I - u u^T) / |v| 
	CVisMatrix2F Jacobian = GXOuterProduct(u, u);
	for (int i = 0; i < 2; i++)
		Jacobian[i][i] -= 1.0;
	Jacobian /= -norm;
	covU = Jacobian * covV * Jacobian.Transposed();
}

void VisNormalizeVectorAndCovariance(
	const CVisVector3F & v,  // original vector
	const CVisMatrix3F &covV,
	CVisVector3F & u, // normalized vector (unit vector)
	CVisMatrix3F & covU
	)
{
	float norm = GXLength(v);
	u = v / norm;

	// Jacobian = (I - u u^T) / |v| 
	CVisMatrix3F Jacobian = GXOuterProduct(u, u);
	for (int i = 0; i < 3; i++)
		Jacobian[i][i] -= 1.0;
	Jacobian /= -norm;
	covU = Jacobian * covV * Jacobian.Transposed();
}

void VisNormalizeVectorAndCovariance(
	const CVisVector4F & v,  // original vector
	const CVisMatrix4F &covV,
	CVisVector4F & u, // normalized vector (unit vector)
	CVisMatrix4F & covU
	)
{
	float norm = GXLength(v);
	u = v / norm;

	// Jacobian = (I - u u^T) / |v| 
	CVisMatrix4F Jacobian = GXOuterProduct(u, u);
	for (int i = 0; i < 4; i++)
		Jacobian[i][i] -= 1.0;
	Jacobian /= -norm;
	covU = Jacobian * covV * Jacobian.Transposed();
}

// compute the spherical coordinates of a 3-vector and 
// their covariance matrix if 'covV' is not a dumb matrix
void Vis3VectorToSphericalCoordinates(
	const CVisVector3F & v,  // original vector
	const CVisMatrix3F & covV,
	CVisVector2F & spherical, // 2D vector (phi, theta)
	CVisMatrix2F & covSpherical
	)
{
    double rr01 = v[0]*v[0] + v[1]*v[1] ;
    if (rr01 == 0.0) 
		rr01 = 1.0e-12 ;
	double r01 = sqrt(rr01);
	double rr = rr01 + v[2] * v[2];
	double r = sqrt(rr);

    spherical[0] = acos (v[0] / r01) ;
    if (v[1] < 0.0) 
		spherical[0] = 2.0 * M_PI - spherical[0] ;
    spherical[1] = acos (v[2] / r) ;

	double aux = r01 * rr ;
	CVisMatrix3F Jacobian;
	Jacobian[0][0] = - v[1]/rr01;
	Jacobian[0][1] = - v[0]/rr01;
	Jacobian[0][2] = 0.0;
	Jacobian[1][0] = v[0] * v[2] / aux;
	Jacobian[1][1] = v[1] * v[2] / aux;
	Jacobian[1][2] = - r01 / rr;
	Jacobian[2][0] = 0.0;
	Jacobian[2][1] = 0.0;
	Jacobian[2][2] = 0.0;

	CVisMatrix3F covT = Jacobian * covV * Jacobian.Transposed();
	covSpherical[0][0] = covT[0][0];
	covSpherical[0][1] = covT[0][1];
	covSpherical[1][0] = covT[1][0];
	covSpherical[1][1] = covT[1][1];
}

// compute the spherical coordinates of a 3-vector and 
// their covariance matrix if 'covSpherical' is not a dumb matrix
void VisSphericalCoordinatesTo3Vector(
	const CVisDVector & spherical, // 2D vector (phi, theta)
	const CVisMatrix2F & covSpherical,
	CVisDVector & u,  // unit 3-vector
	CVisDMatrix & covU
	)
{
	double st, ct, sf, cf ;
    sf = sin (spherical[0]);
    cf = cos (spherical[0]);
    st = sin (spherical[1]);
    ct = cos (spherical[1]);
    u[0] = st * cf ;      
	u[1] = st * sf ;      
	u[2] = ct ;

	CVisMatrix3F Jacobian;
	Jacobian[0][0] = - sf * st;
	Jacobian[0][1] =  cf * ct;
	Jacobian[0][2] = 0.0;
	Jacobian[1][0] = cf * st;
	Jacobian[1][1] = sf * ct;
	Jacobian[1][2] = 0.0;
	Jacobian[2][0] = 0.0;
	Jacobian[2][1] = - st;
	Jacobian[2][2] = 0.0;

	// We use this tempoarary matrix because we need a 3x3 instead of a 2x2 in
	// the code below.
	CVisMatrix3F covST;
	covST[0][0] = covSpherical[0][0];
	covST[0][1] = covSpherical[0][1];
	covST[0][2] = 0.0;
	covST[1][0] = covSpherical[1][0];
	covST[1][1] = covSpherical[1][1];
	covST[1][2] = 0.0;
	covST[2][0] = 0.0;
	covST[2][1] = 0.0;
	covST[2][2] = 0.0;

	CVisMatrix3F covT = Jacobian * covST * Jacobian.Transposed();
	covU[0][0] = covT[0][0];
	covU[0][1] = covT[0][1];
	covU[1][0] = covT[1][0];
	covU[1][1] = covT[1][1];
}


// merge 2 sets of spherical coordinates
void VisMergeSphericalCoordinates(
	const CVisVector2F & sph1, // 2D vector (phi, theta)
	const CVisMatrix2F & covSph1,
	const CVisVector2F & sph2, // 2D vector (phi, theta)
	const CVisMatrix2F & covSph2,
	CVisVector2F & sphMerged, // 2D vector (phi, theta)
	CVisMatrix2F & covSphMerged,
	const bool fHemisphere // set to 1 if they represent UNDIRECTED vectors
	)
{
	CVisVector2F ft1 = sph1;
	CVisVector2F ft2 = sph2;

	if (fHemisphere) {
		// vectors represented by (phi,theta) and ((phi+PI)%2PI, PI-theta) are identical.
		// we choose the one which is closer to the other vector before merging
		if (ft1[1] > M_PI/4 && ft2[1] > M_PI/4) { // otherwise, unlikely they represent the same
			double difPhi = fabs(ft1[0] - ft2[0]);
			if (difPhi > M_PI/2 && difPhi < 3*M_PI/2) { // otherwise, unlikely
				ft2[0] += M_PI;
				while (ft2[0] > 2*M_PI)
					ft2[0] -= 2*M_PI;
				ft2[1] = M_PI - ft2[1];
			}
		}
	}

	if (ft1[0] < M_PI/2 && ft2[0] > 3*M_PI/2)
		ft2[0] -= 2.0 * M_PI;
	if (ft1[0] > 3*M_PI/2 && ft2[0] < M_PI/2)
		ft1[0] -= 2.0 * M_PI;
	CVisMatrix2F Inf1 = covSph1.Inverted();
	CVisMatrix2F Inf2 = covSph2.Inverted();

	CVisMatrix2F matT = Inf1 + Inf2;
	covSphMerged = matT.Inverted();
	sphMerged = covSphMerged * (Inf1 * ft1 + Inf2 * ft2);

	if (sphMerged[0] < 0.0)
		sphMerged[0] += 2.0 * M_PI;
}









// normalize a vector and 
// compute the corresponding covariance matrix if 'covV' is not a dumb matrix
void VisNormalizeVectorAndCovariance(
	const CVisDVector & v,  // original vector
	const CVisDMatrix &covV,
	CVisDVector & u, // normalized vector (unit vector)
	CVisDMatrix & covU
	)
{
	int m = v.Length();

	u.Resize(m);
	double norm = sqrt(v * v);
	u = v / norm;

	if (covV.NRows() == 0 || covV.NCols() == 0)
		return;
	assert(m == covV.NRows() && m == covV.NCols());
	// Jacobian = (I - u u^T) / |v| 
	covU.Resize(m, m);
	CVisDMatrix Jacobian = VisOuterProduct(u, u);
	for (int i = 0; i < m; i++)
		Jacobian[i][i] -= 1.0;
	Jacobian /= -norm;
	covU = Jacobian * covV * Jacobian.Transposed();
}

// compute the spherical coordinates of a 3-vector and 
// their covariance matrix if 'covV' is not a dumb matrix
void Vis3VectorToSphericalCoordinates(
	const CVisDVector & v,  // original vector
	const CVisDMatrix & covV,
	CVisDVector & spherical, // 2D vector (phi, theta)
	CVisDMatrix & covSpherical
	)
{
	assert (v.Length() == 3);


    double rr01 = v[0]*v[0] + v[1]*v[1] ;
    if (rr01 == 0.0) 
		rr01 = 1.0e-12 ;
	double r01 = sqrt(rr01);
	double rr = rr01 + v[2] * v[2];
	double r = sqrt(rr);

	spherical.Resize(2);
    spherical[0] = acos (v[0] / r01) ;
    if (v[1] < 0.0) 
		spherical[0] = 2.0 * M_PI - spherical[0] ;
    spherical[1] = acos (v[2] / r) ;

	if (covV.NRows() == 0 || covV.NCols() == 0)
		return;
	assert (covV.NRows() == 3 && covV.NCols() == 3);
	double aux = r01 * rr ;
	CVisDMatrix Jacobian(2,3);
	Jacobian[0][0] = - v[1]/rr01;
	Jacobian[0][1] = - v[0]/rr01;
	Jacobian[0][2] = 0.0;
	Jacobian[1][0] = v[0] * v[2] / aux;
	Jacobian[1][1] = v[1] * v[2] / aux;
	Jacobian[1][2] = - r01 / rr;

	covSpherical.Resize(2,2);
	covSpherical = Jacobian * covV * Jacobian.Transposed();
}

// compute the spherical coordinates of a 3-vector and 
// their covariance matrix if 'covSpherical' is not a dumb matrix
void VisSphericalCoordinatesTo3Vector(
	const CVisDVector & spherical, // 2D vector (phi, theta)
	const CVisDMatrix & covSpherical,
	CVisDVector & u,  // unit 3-vector
	CVisDMatrix & covU
	)
{
	double st, ct, sf, cf ;
    sf = sin (spherical[0]);
    cf = cos (spherical[0]);
    st = sin (spherical[1]);
    ct = cos (spherical[1]);
	u.Resize(3);
    u[0] = st * cf ;      
	u[1] = st * sf ;      
	u[2] = ct ;

	if (covSpherical.NRows() == 0 || covSpherical.NCols() == 0)
		return;
	assert (covSpherical.NRows() == 2 && covSpherical.NCols() == 2);
	CVisDMatrix Jacobian(3,2);
	Jacobian[0][0] = - sf * st;
	Jacobian[0][1] =  cf * ct;
	Jacobian[1][0] = cf * st;
	Jacobian[1][1] = sf * ct;
	Jacobian[2][0] = 0.0;
	Jacobian[2][1] = - st;

	covU.Resize(3,3);
	covU = Jacobian * covSpherical * Jacobian.Transposed();
}


// merge 2 sets of spherical coordinates
void VisMergeSphericalCoordinates(
	const CVisDVector & sph1, // 2D vector (phi, theta)
	const CVisDMatrix & covSph1,
	const CVisDVector & sph2, // 2D vector (phi, theta)
	const CVisDMatrix & covSph2,
	CVisDVector & sphMerged, // 2D vector (phi, theta)
	CVisDMatrix & covSphMerged,
	const int hemisphere  // set to 1 if they represent UNDIRECTED vectors
	)
{
	CVisDVector ft1 = sph1;
	CVisDVector ft2 = sph2;

	if (hemisphere) {
		// vectors represented by (phi,theta) and ((phi+PI)%2PI, PI-theta) are identical.
		// we choose the one which is closer to the other vector before merging
		if (ft1[1] > M_PI/4 && ft2[1] > M_PI/4) { // otherwise, unlikely they represent the same
			double difPhi = fabs(ft1[0] - ft2[0]);
			if (difPhi > M_PI/2 && difPhi < 3*M_PI/2) { // otherwise, unlikely
				ft2[0] += M_PI;
				while (ft2[0] > 2*M_PI)
					ft2[0] -= 2*M_PI;
				ft2[1] = M_PI - ft2[1];
			}
		}
	}


	if (ft1[0] < M_PI/2 && ft2[0] > 3*M_PI/2)
		ft2[0] -= 2.0 * M_PI;
	if (ft1[0] > 3*M_PI/2 && ft2[0] < M_PI/2)
		ft1[0] -= 2.0 * M_PI;
	CVisDMatrix Inf1 = covSph1.Inverted();
	CVisDMatrix Inf2 = covSph2.Inverted();

	sphMerged.Resize(2);
	covSphMerged.Resize(2,2);
	covSphMerged = (Inf1 + Inf2).Inverted();
	sphMerged = covSphMerged * (Inf1 * ft1 + Inf2 * ft2);

	if (sphMerged[0] < 0.0)
		sphMerged[0] += 2.0 * M_PI;
}

#endif // VIS_NO_LUC_FILES

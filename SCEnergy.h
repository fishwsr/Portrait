#ifndef __SCENERGY_H__
#define __SCENERGY_H__

#define mPI 3.1415926535898
#define EPS 2.2204e-016

#include "cv.h"

class mySCEnergy
{
public:
	mySCEnergy();
	~mySCEnergy();
	
	CvMat*  myDist2(CvMat* Mat1, CvMat* Mat2);
	CvMat*  LogSpace(float lBound, float uBound, const int nBins = 50);
	CvMat*	SC_Compute(const CvMat* Bsamp,  CvMat* Tsamp, double& mean_dist, int nbins_theta, 
					int nbins_r, float r_inner, float r_outer, const CvMat* out_vec);
	CvMat*  HistCost2(CvMat* BH1, CvMat* BH2);

	double SCEnergy(CvMat* pScrSet, CvMat* pTarSet);
};




#endif
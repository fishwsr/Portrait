#include "stdafx.h"
#include "SCEnergy.h"

mySCEnergy::mySCEnergy()
{
}

mySCEnergy::~mySCEnergy()
{
}

CvMat* mySCEnergy::myDist2(CvMat* Mat1, CvMat* Mat11)
{

	CvMat* Mat2;
	bool bIsClone = false;
	if( Mat1 == Mat11 )
	{
		Mat2 = cvCloneMat(Mat11);
		bIsClone = true;
	}
	else
		Mat2 = Mat11;


	int ndata, ncentres, ncols;
	ndata = Mat1->rows;
	ncentres = Mat2->rows;
	ncols = Mat1->cols; // same as Mat2->cols

	// To perform the following Matlab code .... so sad
	// ones(ncentres, 1) * sum((x.^2)', 1))' + ...
	//	ones(ndata, 1) * sum((c.^2)',1) - ...
	//	2.*(x*(c'));

	CvMat* Mat1Clone = cvCloneMat(Mat1);
	CvMat* Mat2Clone = cvCloneMat(Mat2);
	CvMat* ones1     = cvCreateMat(ncentres,1, CV_64FC1);
	CvMat* ones2	 = cvCreateMat(ndata, 1, CV_64FC1);
	CvMat* Mat1Trans = cvCreateMat(ncols, Mat1->rows, CV_64FC1);
	CvMat* Mat2Trans = cvCreateMat(ncols, Mat2->rows, CV_64FC1);
	CvMat* Prod1     = cvCreateMat(ncentres, ndata, CV_64FC1);
	CvMat* Prod2     = cvCreateMat(ndata, ncentres, CV_64FC1);
	CvMat* Sum1      = cvCreateMat(1, ndata, CV_64FC1);
	CvMat* Sum2      = cvCreateMat(1, ncentres, CV_64FC1);
	CvMat* tarMat    = cvCreateMat(ndata, ncentres, CV_64FC1);

	cvSet(ones1, cvScalar(1.0));
	cvSet(ones2, cvScalar(1.0));

	//Mat1 = cvCloneMat(Mat2);

	cvMul(Mat1, Mat1, Mat1); //Mat1 =  Mat1.*Mat1;
	cvTranspose(Mat1, Mat1Trans);

	//SaveMatToFile(Mat1, "Mat1");
	//SaveMatToFile(Mat1Trans, "Mat1Trans");

	//cvPow(Mat2, Mat2, 2);
	cvMul(Mat2, Mat2, Mat2);
	cvTranspose(Mat2, Mat2Trans);

	//SaveMatToFile(Mat2, "Mat2");
	//SaveMatToFile(Mat2Trans, "Mat2Trans");

	CvMat Row1, Row2;

	cvGetRow(Mat1Trans, &Row1, 0);
	cvGetRow(Mat1Trans, &Row2, 1);
	cvAdd(&Row1, &Row2, Sum1);
	//SaveMatToFile(ones1, "ones1");
	//SaveMatToFile(Sum1, "Sum1");
	cvMatMulAdd(ones1, Sum1, 0,  Prod1);
	cvTranspose(Prod1, tarMat);

	//SaveMatToFile(Prod1, "Prod1");

	cvGetRow(Mat2Trans, &Row1, 0);
	cvGetRow(Mat2Trans, &Row2, 1);
	cvAdd(&Row1, &Row2, Sum2);

	//SaveMatToFile(ones2, "ones2");
	//SaveMatToFile(Sum2, "Sum2");

	cvMatMul(ones2, Sum2, Prod2);

	//SaveMatToFile(Prod2, "Prod2");

	//SaveMatToFile(tarMat, "tarMat");
	cvAdd(tarMat, Prod2, tarMat);
	//SaveMatToFile(tarMat, "tarMat");

	cvGEMM(Mat1Clone, Mat2Clone, -2.0, tarMat, 1.0, tarMat, CV_GEMM_B_T);

	//SaveMatToFile(tarMat, "tarMat");

	cvReleaseMat(&ones1);
	cvReleaseMat(&ones2);
	cvReleaseMat(&Mat1Trans);
	cvReleaseMat(&Mat2Trans);
	cvReleaseMat(&Prod1);
	cvReleaseMat(&Prod2);
	cvReleaseMat(&Sum1);
	cvReleaseMat(&Sum2);
	cvReleaseMat(&Mat1Clone);
	cvReleaseMat(&Mat2Clone);

	if(bIsClone)
		cvReleaseMat(&Mat2);

	return tarMat;
}

CvMat* mySCEnergy::LogSpace(float lBound, float uBound, const int nBins)
{
	//   LOGSPACE(X1, X2) generates a row vector of 50 logarithmically
	//   equally spaced points between decades 10^X1 and 10^X2.  If X2
	//   is pi, then the points are between 10^X1 and pi.
	if(uBound == mPI)
		uBound = log10f(uBound);

	CvMat* LogArr = cvCreateMat(1, nBins, CV_64FC1);

	double value;
	for(int i = 0; i < nBins; i++)
	{
		value = lBound + i*(uBound - lBound)/(nBins - 1);
		value = powf(10., value);
		cvSet1D(LogArr, i, cvScalar(value));
	}
	//y = (10).^ [d1+(0:n-2)*(d2-d1)/(floor(n)-1), d2];
	return LogArr;
}

CvMat* mySCEnergy::SC_Compute(const CvMat* Bsamp,  CvMat* Tsamp, double& mean_dist, int nbins_theta, 
									 int nbins_r, float r_inner, float r_outer, const CvMat* out_vec)
{
	int nsamp = Bsamp->cols;
	CvMat* bInvec = cvCreateMat(1, nsamp, CV_8UC1);
	cvCmpS(out_vec, 0.0, bInvec, CV_CMP_EQ);

	CvMat* BsampT = cvCreateMat(Bsamp->cols, Bsamp->rows, CV_64FC1);
	cvTranspose(Bsamp, BsampT);

	//compute r,theta arrays
	/*CvMat* BsampTCopy = cvCloneMat(BsampT);*/
	CvMat* r_arry = myDist2(BsampT, BsampT);
	//SaveMatToFile(r_arry, "r_arry");

	cvPow(r_arry, r_arry, 1.0/2);
	//SaveMatToFile(r_arry, "r_arry");

	CvMat Row1, Row2;
	//cvGetCol(BsampT, &Row2T, 1);
	cvGetRow(Bsamp, &Row1, 0);
	cvGetRow(Bsamp, &Row2, 1);
	CvMat* onesRow = cvCreateMat(1, nsamp, CV_64FC1);
	//CvMat* onesCol = cvCreateMat(nsamp, 1, CV_64FC1);
	CvMat* tar1    = cvCreateMat(nsamp, nsamp, CV_64FC1);
	CvMat* tar2    = cvCloneMat(tar1);
	cvSet(onesRow, cvScalar(1.0));
	//cvSet(onesCol, cvScalar(1.0));
	cvGEMM(&Row2, onesRow, 1.0, tar1, 0.0, tar1, CV_GEMM_A_T);
	cvGEMM(onesRow, &Row2, -1.0, tar1, 1.0, tar1, CV_GEMM_A_T);
	cvGEMM(&Row1, onesRow, 1.0, tar2, 0.0, tar2, CV_GEMM_A_T);
	cvGEMM(onesRow, &Row1, -1.0, tar2, 1.0, tar2, CV_GEMM_A_T);

	//SaveMatToFile(tar1, "tar1");
	//SaveMatToFile(tar2, "tar2");

	int i, j;
	CvScalar X, Y;
	double   degree;
	CvMat*  theta_arry = cvCreateMat(nsamp, nsamp, CV_64FC1);
	for (i = 0; i < nsamp; i++)
		for(j = 0; j < nsamp; j++)
		{
			X = cvGet2D(tar2, j, i);
			Y = cvGet2D(tar1, j, i);
			degree = atan2(Y.val[0], X.val[0]);
			//degree = cvFastArctan(Y.val[0], X.val[0]);
			//degree *= 2.0*PI/360.0;
			//if (degree > PI)
			//	degree -= 2*PI;
			cvSet2D(theta_arry, i, j, cvScalar(degree));
		}
		//SaveMatToFile(Tsamp, "Tsamp");
	//	SaveMatToFile(onesRow, "onesRow");
		//SaveMatToFile(theta_arry, "theta_arry");
		cvGEMM(Tsamp, onesRow, -1.0, theta_arry, 1.0, theta_arry, CV_GEMM_A_T);

		//SaveMatToFile(theta_arry, "theta_arry");

		//create joint (r,theta) histogram by binning r_array and
		//theta_array

		//normalize distance by mean, ignoring outliers
		CvMat* in_vecMask = cvCreateMat(nsamp,nsamp, CV_8UC1);
		if(mean_dist == 0.0)
		{
			cvSet(in_vecMask, cvScalar(1));
			for(i = 0; i < nsamp; i++)
				for(j = 0; j < nsamp; j++)
				{
					if(cvGet1D(bInvec,i).val[0] == 0 || cvGet1D(bInvec,j).val[0] == 0)
						cvSet2D(in_vecMask, i, j, cvScalar(0));
				}
				//SaveMatToFile(bInvec, "bInvec");
				//SaveMatToFile(in_vecMask, "in_vecMask");
				mean_dist = cvAvg(r_arry, in_vecMask).val[0];
		}
		CvMat* r_arry_n = cvCloneMat(r_arry);
		cvSet(r_arry_n, cvScalar(mean_dist));
		cvDiv(r_arry, r_arry_n, r_arry_n);

		//SaveMatToFile(r_arry_n, "r_arry_n");

		//use a log. scale for binning the distances
		CvMat* r_bin_edges = LogSpace(log10f(r_inner), log10f(r_outer), 5);

		//SaveMatToFile(r_bin_edges, "r_bin_edges");

		CvMat* r_arry_q = cvCreateMat(nsamp, nsamp, CV_64FC1);
		cvSet(r_arry_q, cvScalar(0));
		CvMat* arry_mask = cvCreateMat(nsamp, nsamp, CV_8UC1);
		for(i = 0; i < r_bin_edges->cols; i++)
		{
			cvCmpS(r_arry_n, cvGet1D(r_bin_edges, i).val[0], arry_mask, CV_CMP_LT); // r_arry_n < r_bin_edge[i]
			//SaveMatToFile(arry_mask, "arry_mask");
			
			cvAddS(r_arry_q, cvScalar(1), r_arry_q, arry_mask);
			//SaveMatToFile(r_arry_q, "r_arry_q");
		}	
		cvCmpS(r_arry_q, 0.0, arry_mask, CV_CMP_GT);

		//SaveMatToFile(r_arry_q, "r_arry_q");
		//SaveMatToFile(arry_mask, "arry_mask");

		//put all angles in [0, 2PI) range
		double theta_value;
		double PI2 = 2*mPI;
		for(i = 0; i < theta_arry->rows; i++)
		{
			for(j = 0; j < theta_arry->cols; j++)
			{
				theta_value = cvmGet(theta_arry, i, j);
				if(theta_value >= 0.)
				{
					theta_value = theta_value - cvFloor(theta_value/PI2)*PI2 + PI2 ;
				}
				else
				{
					theta_value = theta_value - cvCeil(theta_value/PI2)*PI2 + PI2 ;
				}
				theta_value = theta_value - cvFloor(theta_value/PI2)*PI2;
				//theta_array_2 = rem(rem(theta_array,2*pi)+2*pi,2*pi);
				cvmSet(theta_arry, i, j, theta_value);
			}
		}	
		//SaveMatToFile(theta_arry, "theta_arry");

		//quantize to a fixed set of angles (bin edges lie on 0,(2*pi)/k,...2*pi
		CvMat*  theta_arry_q = cvCreateMat(theta_arry->rows, theta_arry->cols, CV_8UC1);
		for(i = 0; i < theta_arry->rows; i++)
		{
			for(j = 0; j < theta_arry->cols; j++)
			{
				theta_value = cvmGet(theta_arry, i, j);
				int tempValue = 1 + floor(theta_value/(2.*mPI/((double)nbins_theta)));
				cvSet2D(theta_arry_q, i, j, cvScalar(tempValue));
			}
		}
		//SaveMatToFile(theta_arry_q, "theta_arry_q");

		int nbins = nbins_theta*nbins_r;
		CvMat* BH = cvCreateMat(nsamp, nbins, CV_8UC1);
		CvMat* SN = cvCreateMat(nbins_r,nbins_theta, CV_8UC1);

		CvMat thetaRow, rRow, maskRow, SNRowHeader, BHRow;
		int bin_theta, bin_r;
		CvMat* SNRow = cvCreateMat(1, nbins, CV_8UC1);
		for(int n = 0; n < nsamp; n++)
		{
			cvSet(SN, cvScalar(0));
			cvGetRow(arry_mask, &maskRow, n);
			cvAnd(&maskRow, bInvec, &maskRow);
			cvGetRow(theta_arry_q, &thetaRow, n);
			cvGetRow(r_arry_q, &rRow, n);
			//SaveMatToFile(&thetaRow, "thetaRow");

			for(i = 0; i < theta_arry_q->cols; i++)
			{
				if(cvGet1D(&maskRow,i).val[0] == 0 )
					continue;
				bin_theta = cvFloor(cvGet1D(&thetaRow, i).val[0]) - 1;
				bin_r = cvFloor(cvGet1D(&rRow, i).val[0]) - 1;
				cvSet2D(SN, bin_r,bin_theta, cvScalar(cvGet2D(SN, bin_r,bin_theta).val[0] + 1));
			}
			//SaveMatToFile(SN, "SN");
			SNRow = cvReshape(SN, &SNRowHeader, 0, 1);
			cvGetRow(BH, &BHRow, n);
			cvCopy(SNRow, &BHRow);
		}
		//SaveMatToFile(BH, "BH");

		cvReleaseMat(&bInvec);
		cvReleaseMat(&BsampT);
		cvReleaseMat(&r_arry);
		cvReleaseMat(&onesRow);
		cvReleaseMat(&tar1);
		cvReleaseMat(&tar2);
		cvReleaseMat(&theta_arry);
		cvReleaseMat(&in_vecMask);
		cvReleaseMat(&r_arry_n);
		cvReleaseMat(&arry_mask);
		cvReleaseMat(&r_bin_edges);
		cvReleaseMat(&theta_arry_q);
		cvReleaseMat(&r_arry_q);
		//cvReleaseMat(&BH);
		cvReleaseMat(&SN);
		//cvReleaseMat(&SNRow);
		return BH;
}

CvMat* mySCEnergy::HistCost2(CvMat* BH1, CvMat* BH2)
{
	ASSERT(BH1->cols == BH2->cols);
	int nsamp1 = BH1->rows;
	int nsamp2 = BH2->rows;
	int nbins  = BH1->cols;

	CvMat* HC = cvCreateMat(nsamp1, nsamp2, CV_64FC1);
	double cellvalue, sum1, sum2;
	CvMat* BH1Row   =  cvCreateMat(1, nbins, CV_64FC1);
	CvMat* BH2Row   =  cvCreateMat(1, nbins, CV_64FC1);
	CvMat* tempRow1 =  cvCreateMat(1, nbins, CV_64FC1);
	CvMat* tempRow2 =  cvCreateMat(1, nbins, CV_64FC1);

	for(int i = 0; i < nsamp1; i++)
	{
		//cvGetRow(BH1, &BH1Row, i);
		for(int k = 0; k < nbins; k++)
		{
			cvmSet(BH1Row, 0, k, cvGet2D(BH1, i, k).val[0]);
		}
		sum1 = cvSum(BH1Row).val[0]; //+EPS?
		if(sum1 == 0.0)
			sum1 += EPS;
		cvSet(tempRow1, cvScalar(sum1));
		//SaveMatToFile(BH1Row,"BH1Row");
		//SaveMatToFile(tempRow1,"tempRow1");
		cvDiv(BH1Row, tempRow1, BH1Row);
		for(int j = 0; j < nsamp2; j++)
		{
			//cvGetRow(BH2, &BH2Row, j);
			for(int k = 0; k < nbins; k++)
			{
				cvmSet(BH2Row, 0, k, cvGet2D(BH2, j, k).val[0]);
			}
			sum2 = cvSum(BH2Row).val[0]; //+EPS?
			if(sum2 == 0.0)
				sum2 += EPS;
			cvSet(tempRow2, cvScalar(sum2));
			//SaveMatToFile(tempRow2,"tempRow2");
			cvDiv(BH2Row, tempRow2, BH2Row);

			cvSub(BH1Row, BH2Row, tempRow1);
			cvMul(tempRow1, tempRow1, tempRow1);
			cvAdd(BH1Row, BH2Row, tempRow2);
			cvAddS(tempRow2, cvScalar(EPS), tempRow2);
			cvDiv(tempRow1, tempRow2, tempRow1, 0.5);
			cellvalue = cvSum(tempRow1).val[0];
			cvmSet(HC, i, j, cellvalue);
		}
	}
	//SaveMatToFile(HC, "HC");

	cvReleaseMat(&tempRow1);
	cvReleaseMat(&tempRow2);
	cvReleaseMat(&BH1Row);
	cvReleaseMat(&BH2Row);
	return HC;
}

double mySCEnergy::SCEnergy(CvMat* pScrSet, CvMat* pTarSet)
{
	int i, j;
	int  nsamp1 = pScrSet->rows;
	int  nsamp2 = pTarSet->rows;

	//initialize transformed version of model pointset
	CvMat* Xk = cvCloneMat(pScrSet);
	CvMat* XkT = cvCreateMat(Xk->cols, Xk->rows, CV_64FC1);
	CvMat* YT  = cvCreateMat(pTarSet->cols, pTarSet->rows, CV_64FC1);

	// out_vec_{1,2} are indicator vectors for keeping track of estimated
	// outliers on each iteration
	CvMat* out_vec_1 = cvCreateMat(1, nsamp1, CV_8UC1);
	CvMat* out_vec_2 = cvCreateMat(1, nsamp2, CV_8UC1);
	CvMat* Tsamp1    = cvCreateMat(1, nsamp1, CV_64FC1);
	CvMat* Tsamp2    = cvCreateMat(1, nsamp2, CV_64FC1);

	cvSet(out_vec_1, cvScalar(0));
	cvSet(out_vec_2, cvScalar(0));
	cvSet(  Tsamp1 , cvScalar(0));
	cvSet(  Tsamp2 , cvScalar(0));
	
	cvTranspose(Xk, XkT);
	cvTranspose(pTarSet, YT);
	//compute shape contexts for (transformed) model
	double mean_dist_1 = 0.;
	CvMat* BH1 = SC_Compute(XkT, Tsamp1, mean_dist_1, 12, 5,
			0.125, 2.0, out_vec_1);

	double mean_dist_2 = mean_dist_1;
	CvMat* BH2 = SC_Compute(YT, Tsamp2, mean_dist_2, 12, 5,
			0.125, 2.0, out_vec_2);

	CvMat* costmat = HistCost2(BH1, BH2);

	double dShapeCEnergy;
	double colSum = 0.0;
	double rowSum = 0.0;
	double curCost, tempCost;
	for(i=0; i < nsamp1; i++)
	{
		tempCost = 999.0;   // just a very large number
		for(j=0; j <nsamp2; j++)
		{
			curCost = cvmGet(costmat, i, j);
			if(curCost < tempCost)
				tempCost = curCost;
		}
		colSum += tempCost; 
	}
	for(i=0; i < nsamp2; i++)
	{
		tempCost = 999.0;   // just a very large number
		for(j=0; j < nsamp1; j++)
		{
			curCost = cvmGet(costmat, j, i);
			if(curCost < tempCost)
				tempCost = curCost;
		}
		rowSum += tempCost; 
	}

	colSum /= nsamp1;
	rowSum /= nsamp2;

	if( colSum < rowSum)
		dShapeCEnergy = rowSum;
	else
		dShapeCEnergy = colSum;

	return dShapeCEnergy;
}

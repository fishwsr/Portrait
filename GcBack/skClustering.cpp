//GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG
// skClustering.cpp	implementation file
//
// Author:			Cheng-en Guo
//
// Created Time:	01-26-2002
// Revised Time:	09-10-2003
//
// Descriptions:	This file is to define clustering methods
//					
//
// Member variables:
//					
// Functions: 	
//
//GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG
#include "stdafx.h"

#include <math.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include "skClustering.h"

//#include ".\skOutput.h"

#ifndef mySqrt
#define mySqrt( x )  sqrt((float)(x))
#endif

///////////////////
// skMeanShiftClustering1D()
//	Author: Cheng-en Guo, 01-26-2002
//	the standard mean shift clustering on 1-D
//	input:	pdData - pointer to the sample data, dim=nNum
//			nNum - data num
//			dWindowSize - the Parzen window size, which determine how big the cluster will covering
//	output:	pnClusterIndex - the cluster index for each sample, should have the dimension of nNum
//			pdClusterCenter - the mean value for each cluster, it won't be computed if not provide valid pointer
//	return: nClusterNum: succeeded0: failed, 

int skKMeanCluster(float *pdData, int nDim, int nNum, float *pdWeight, float *pdCC, int &nClusterNum, int nMaxClusterNum, int *pnCluster)
{
	if (nDim <= 0 || nNum <= 0)
	{ 
		nClusterNum = 0;
		return 0;
	}

	if (nClusterNum <= 0)
		nClusterNum = nMaxClusterNum;

	if (nNum < 5)
		nClusterNum = 1;
	
	if (nNum < nClusterNum)
		nClusterNum = 1;

	int i;//index of sample
	int d;//index of dimension
	int n;//index of data
	int c;//index of cluster

	// if only one cluster desired
	if (nClusterNum ==  1)
	{
		for (d = 0; d < nDim; d++)
			pdCC[d] = 0;
		n  = 0;
		for (i = 0; i < nNum; i++)
			for (d = 0; d < nDim; d++)
				pdCC[d] += pdData[n++];
		for (d = 0; d < nDim; d++)
			pdCC[d] /= nNum;
		for (i = 0; i < nNum; i++)
			pnCluster[i] = 0;//only one cluster
		return 1;
	}

	// initilize the the cluster center
	// This is a greedy method. Different init may lead to different results
	for (c = 0; c < nClusterNum; c++)
	{
		i = (int)(rand()/(RAND_MAX+1.0) * nNum);
		for (d = 0; d < nDim; d++)
			pdCC[c*nDim + d] = pdData[i*nDim + d];
	}

//	memset(pnCluster, -1, sizeof(int)*nNum);
	// assign each sample to a cluster
	while (true)
	{
		if (skAssignSample2Cluster(pdData, nDim, nNum, pdWeight, pdCC, nClusterNum, pnCluster) <= 0)
			break;
		skUpdateClusterCenter(pdData, nDim, nNum, pdCC, nClusterNum, pnCluster);
	}
	return nClusterNum;
}

float skEuclidianDistance(float *pData1, float *pData2, int nDim, float *pdWeight)
{
	float dDis = 0;
	for (int d = 0; d < nDim; d++)
		dDis += (pData1[d]-pData2[d])*(pData1[d]-pData2[d])*pdWeight[d]*pdWeight[d];
	return mySqrt(dDis);
}

int skUpdateClusterCenter(float *pdData, int nDim, int nNum, float *pdCC, int nClusterNum, int *pnCluster)
{
	if (nClusterNum <= 0)
		return 1;
	int c, d, i, n;

	memset(pdCC, 0, sizeof(float)*nDim*nClusterNum);

	int pnNum[1000];
	memset(pnNum, 0, sizeof(int)*1000);

	// add up center value
	n = 0;
	for (i = 0; i < nNum; i++)
	{ 
		c = pnCluster[i];
		pnNum[c]++;
		assert(c >= 0 && c < nClusterNum);
		for (d = 0; d < nDim; d++)
			pdCC[c*nDim+d] += pdData[n++];
	}
	// divided by sum
	for (c = 0; c < nClusterNum; c++)
	{
		if (pnNum[c] > 0)
			for (d = 0; d < nDim; d++)
				pdCC[c*nDim+d] /= pnNum[c];
	}
	return 0;
}
// return the number of samples whose label are changed
int skAssignSample2Cluster(float *pdData, int nDim, int nNum, float *pdWeight, float *pdCC, int nClusterNum, int *pnCluster)
{
	if (nClusterNum <= 0)
		return 0;
	int c, i;
	
	float dDis;
	float dMinDis = 0;
	int nMin;
	int sum = 0;
	for (i = 0; i < nNum; i++)
	{
		dMinDis = 100000;
		nMin = -1;
		for (c = 0; c < nClusterNum; c++)
		{
			dDis = skEuclidianDistance(pdData+i*nDim, pdCC+c*nDim, nDim, pdWeight);
			if (dMinDis > dDis)
			{
				dMinDis = dDis;
				nMin = c;
			}
		}
		assert(nMin >= 0);
		if (pnCluster[i] != nMin) 
		{
			sum++;
			pnCluster[i] = nMin;
		}
	}	
	return sum;
}

// return the number of samples whose label are changed

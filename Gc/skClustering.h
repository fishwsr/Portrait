//GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG
// skClustering.h	header file
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
#pragma once
#ifndef _SK_CLUSTERING_H_
#define _SK_CLUSTERING_H_

int    skKMeanCluster(float *pdData, int nDim, int nNum, float *pdWeight, float *pdCC, int &nClusterNum, int nMaxClusterNum, int *pnCluster);
float skEuclidianDistance(float *pData1, float *pData2, int nDim, float *pdWeight);
int    skUpdateClusterCenter(float *pdData, int nDim, int nNum, float *pdCC, int nClusterNum, int *pnCluster);
int    skAssignSample2Cluster(float *pdData, int nDim, int nNum, float *pdWeight, float *pdCC, int nClusterNum, int *pnCluster);

#endif
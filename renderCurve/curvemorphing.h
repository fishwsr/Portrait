//KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK
// TriangleMesh.h   header file
// Author:		    Kun Zeng
// Created Time:    2006-08-31 09:38:57
// Revised Time:
//
// Descriptions:    implementation the deformation between dictionary height map and
//                  Brush height map
// Member variables:
// Functions: 	
//
// email:           zengkun@gmail.com
//
// Copyright (C) 2006, Kun Zeng, all rights reserved.
//KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK

#ifndef CURVEMORPHING_H
#define CURVEMORPHING_H

#include <iomanip>
#include <cmath>
#include "Mypoint.h"
#include "NatCubic.h"

#include "../Common.h"
#include "../CImg.h"
using namespace cimg_library;

using namespace std;

class CCurveMorphing
{
public:

double inline GetDistance1(double x1, double y1, double x2, double y2);

double inline GetDistance2(double x1, double y1, double x2, double y2);

void NearestPoint(int i, int j, vector<DoublePoint> &Points, int &K, double &dist);

void NearestPoint2(int i, int j, vector<DoublePoint> &Points, const int K, double &dist);

// color image
void FindMapPoint(double ratio1, double ratio2, bool isPlus, CImg<unsigned char> &SrcImg, int &i0, int &j0);

void GetBoundary(vector<DoublePoint> &midLine, const int radius,
				 vector<DoublePoint> &plusLine, vector<DoublePoint> &minusLine);

void GetEndPoint(vector<DoublePoint> &Points, double &sx, double &sy, double &ex, double &ey);

void GetRange(vector<DoublePoint> &RetMidPoints,
			  vector<DoublePoint> &RetPlusPoints,
			  vector<DoublePoint> &RetMinusPoints,
			  double &minx, double &miny, double &maxx, double &maxy);

// Morphing a color image
void MorphingToCurve(vector<DoublePoint> &midLine,
					 int r, int g, int b,
					 CImg<unsigned char> &AlphaImg,					 
					 int radius, 
					 CImg<unsigned char> &RetImg, 
					 CImg<unsigned char> &flag,
					 CImg<unsigned char> &flagImage);


// spline a curve
void MorphingToCurve2(vector<DoublePoint> &midLine,
					 int r, int g, int b,
					 CImg<unsigned char> &RetImg);

};

#endif //#define CURVEMORPHING_H

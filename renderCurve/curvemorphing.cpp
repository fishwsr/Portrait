#include "stdafx.h"
#include "curvemorphing.h"

const double PI = 3.1415926;

inline double CCurveMorphing::GetDistance1(const double x1, const double y1, const double x2, const double y2)
{
	double dx = x2 - x1, dy = y2 - y1;
	return dx * dx + dy * dy;
}

inline double CCurveMorphing::GetDistance2(const double x1, const double y1, const double x2, const double y2)
{
	return sqrt(GetDistance1(x1, y1, x2, y2));
}

void CCurveMorphing::NearestPoint(int i, int j, vector<DoublePoint> &Points, int &K, double &dist)
{
	double x1, y1;

	x1 = (double) j;
	y1 = (double) i;

	double tempDist;

	dist = 1000000000.0;
	for(int n = 0; n < int(Points.size()); n++)
	{
		tempDist = GetDistance1(x1, y1, Points[n].x, Points[n].y);

		if(tempDist < dist)
		{
			dist = tempDist;
			K = n;
		}
	}
	dist = sqrt(dist);
}

void CCurveMorphing::NearestPoint2(int i, int j, vector<DoublePoint> &Points, const int K, double &dist)
{
	double x1, y1;

	x1 = (double) j;
	y1 = (double) i;

	double tempDist;

	int NN = int(Points.size());

	dist = 1000000000.0;
	for(int n = Max(0, K - NN / 8); n < Min(NN, K + NN / 8); n++)
	{
		tempDist = GetDistance1(x1, y1, Points[n].x, Points[n].y);

		if(tempDist < dist)
		{
			dist = tempDist;
		}
	}
	dist = sqrt(dist);
}

void CCurveMorphing::FindMapPoint(double ratio1, double ratio2, bool isPlus, CImg<unsigned char> &SrcImg, int &i0, int &j0)
{
	int height = SrcImg.height;
	int width  = SrcImg.width;

	i0 = (int) ((isPlus ? (1 + ratio2) : (1 - ratio2)) * height / 2);
	j0 = (int) (ratio1 * width + 0.5);
}

void CCurveMorphing::GetBoundary(vector<DoublePoint> &midLine, const int radius,
								 vector<DoublePoint> &plusLine, vector<DoublePoint> &minusLine)
{
	int i;

	int nLength = int(midLine.size());

	vector<DoublePoint> mPlusLine(nLength*2-2);
	vector<DoublePoint> mMinusLine(nLength*2-2);

	//get all boundary point
	int n1, n2;

	for (i = 0, n1 = 0, n2 =1; i < nLength - 1; i++, n1 += 2, n2 += 2)
	{
		double dTheta;

		if(midLine[i+1].x-midLine[i].x == 0.0)
			dTheta = PI/2.0;
		else
			dTheta = atan((midLine[i+1].y-midLine[i].y)/(midLine[i+1].x-midLine[i].x));

		double r_sin_dtheta = radius * sin(dTheta);
		double r_cos_dtheta = radius * cos(dTheta);

		mPlusLine[n1].x  = midLine[i].x - r_sin_dtheta;
		mPlusLine[n1].y  = midLine[i].y + r_cos_dtheta;
		mMinusLine[n1].x = midLine[i].x + r_sin_dtheta;
		mMinusLine[n1].y = midLine[i].y - r_cos_dtheta;

		mPlusLine[n2].x  = midLine[i+1].x - r_sin_dtheta;
		mPlusLine[n2].y  = midLine[i+1].y + r_cos_dtheta;
		mMinusLine[n2].x = midLine[i+1].x + r_sin_dtheta;
		mMinusLine[n2].y = midLine[i+1].y - r_cos_dtheta;		
	}

	//swap if needed
	for (i=1; i<nLength*2-2; i++)    
	{
		double d1 = (mPlusLine[i].x-mPlusLine[i-1].x)  * (mPlusLine[i].x-mPlusLine[i-1].x) + 
			(mPlusLine[i].y-mPlusLine[i-1].y)  * (mPlusLine[i].y-mPlusLine[i-1].y);

		double d2 = (mMinusLine[i].x-mMinusLine[i-1].x)* (mMinusLine[i].x-mMinusLine[i-1].x) + 
			(mMinusLine[i].y-mMinusLine[i-1].y)* (mMinusLine[i].y-mMinusLine[i-1].y);

		double d3 = (mPlusLine[i].x-mMinusLine[i-1].x) * (mPlusLine[i].x-mMinusLine[i-1].x) + 
			(mPlusLine[i].y-mMinusLine[i-1].y) * (mPlusLine[i].y-mMinusLine[i-1].y);

		double d4 = (mMinusLine[i].x-mPlusLine[i-1].x) * (mMinusLine[i].x-mPlusLine[i-1].x) + 
			(mMinusLine[i].y-mPlusLine[i-1].y) * (mMinusLine[i].y-mPlusLine[i-1].y);

		if ( (sqrt(d1) + sqrt(d2)) > (sqrt(d3) + sqrt(d4)) )
		{
			DoublePoint temp = mMinusLine[i];
			mMinusLine[i] = mPlusLine[i];
			mPlusLine[i] = temp;
		}
	}

	// First Point
	minusLine[0] = mMinusLine[0];			
	plusLine[0]  = mPlusLine[0];

	// Last Point
	minusLine[nLength-1] = mMinusLine[nLength*2-3];
	plusLine[nLength-1]  = mPlusLine[nLength*2-3];

	// Middle Point
	for (i = 1; i < nLength-1; i++)
	{
		double px,py,p1,p2,t;

		px = (mPlusLine[i*2-1].x + mPlusLine[i*2].x)/2;
		py = (mPlusLine[i*2-1].y + mPlusLine[i*2].y)/2;

		p1 = (px-midLine[i].x)*(px-midLine[i].x)+
			(py-midLine[i].y)*(py-midLine[i].y);

		p1 = sqrt(p1);   

		t = (px-mPlusLine[i*2].x)*(px-mPlusLine[i*2].x)+
			(py-mPlusLine[i*2].y)*(py-mPlusLine[i*2].y);

		p2 = t/p1;  

		plusLine[i].x = px+p2/p1*(px-midLine[i].x);
		plusLine[i].y = py+p2/p1*(py-midLine[i].y);


		px = (mMinusLine[i*2-1].x + mMinusLine[i*2].x)/2;
		py = (mMinusLine[i*2-1].y + mMinusLine[i*2].y)/2;

		p1 = (px-midLine[i].x)*(px-midLine[i].x)+
			(py-midLine[i].y)*(py-midLine[i].y);

		p1 = sqrt(p1);

		t = (px-mMinusLine[i*2].x)*(px-mMinusLine[i*2].x)+
			(py-mMinusLine[i*2].y)*(py-mMinusLine[i*2].y);

		p2 = t/p1; 

		minusLine[i].x = px+p2/p1*(px-midLine[i].x);
		minusLine[i].y = py+p2/p1*(py-midLine[i].y);		

	}	
}

void CCurveMorphing::GetRange(vector<DoublePoint> &RetMidPoints,
							  vector<DoublePoint> &RetPlusPoints,
							  vector<DoublePoint> &RetMinusPoints,
							  double &minx, double &miny, double &maxx, double &maxy)
{
	double minx0, miny0, maxx0, maxy0;
	int NN;

	// mid
	NN = int(RetMidPoints.size());
	minx0 = RetMidPoints[0].x;
	miny0 = RetMidPoints[0].y;
	maxx0 = RetMidPoints[0].x;
	maxy0 = RetMidPoints[0].y;

	for(int i=0; i<NN; i++)
	{
		minx0 = minx0 > RetMidPoints[i].x ? RetMidPoints[i].x : minx0;
		miny0 = miny0 > RetMidPoints[i].y ? RetMidPoints[i].y : miny0;
		maxx0 = maxx0 < RetMidPoints[i].x ? RetMidPoints[i].x : maxx0;
		maxy0 = maxy0 < RetMidPoints[i].y ? RetMidPoints[i].y : maxy0;
	}

	minx = minx0;
	miny = miny0;
	maxx = maxx0;
	maxy = maxy0;

	// plus	
	NN = int(RetPlusPoints.size());
	minx0 = RetPlusPoints[0].x;
	miny0 = RetPlusPoints[0].y;
	maxx0 = RetPlusPoints[0].x;
	maxy0 = RetPlusPoints[0].y;

	for(i=0; i<NN; i++)
	{
		minx0 = minx0 > RetPlusPoints[i].x ? RetPlusPoints[i].x : minx0;
		miny0 = miny0 > RetPlusPoints[i].y ? RetPlusPoints[i].y : miny0;
		maxx0 = maxx0 < RetPlusPoints[i].x ? RetPlusPoints[i].x : maxx0;
		maxy0 = maxy0 < RetPlusPoints[i].y ? RetPlusPoints[i].y : maxy0;
	}

	if(minx>minx0)
		minx = minx0;
	if(miny>miny0)
		miny = miny0;
	if(maxx<maxx0)
		maxx = maxx0;
	if(maxy<maxy0)
		maxy = maxy0;

	// minus
	NN = int(RetMinusPoints.size());
	minx0 = RetMinusPoints[0].x;
	miny0 = RetMinusPoints[0].y;
	maxx0 = RetMinusPoints[0].x;
	maxy0 = RetMinusPoints[0].y;

	for(i=0; i<NN; i++)
	{
		minx0 = minx0 > RetMinusPoints[i].x ? RetMinusPoints[i].x : minx0;
		miny0 = miny0 > RetMinusPoints[i].y ? RetMinusPoints[i].y : miny0;
		maxx0 = maxx0 < RetMinusPoints[i].x ? RetMinusPoints[i].x : maxx0;
		maxy0 = maxy0 < RetMinusPoints[i].y ? RetMinusPoints[i].y : maxy0;
	}

	if(minx>minx0)
		minx = minx0;
	if(miny>miny0)
		miny = miny0;
	if(maxx<maxx0)
		maxx = maxx0;
	if(maxy<maxy0)
		maxy = maxy0;
}

// Morphing a color image
void CCurveMorphing::MorphingToCurve(vector<DoublePoint> &midLine,
									 int r, int g, int b,
									 CImg<unsigned char> &AlphaImg,								
									 int radius, 
									 CImg<unsigned char> &RetImg, 
									 CImg<unsigned char> &flag,
									 CImg<unsigned char> &flagImage)
{
	int height = RetImg.height;
	int width  = RetImg.width;

	int h = AlphaImg.height;
	int w = AlphaImg.width;

	// 1. Get the control point of MidLine, PlusLine, MinusLine
	int NN;
	NN = int(midLine.size());

	vector<DoublePoint> plusLine(NN), minusLine(NN);
	GetBoundary(midLine, radius, plusLine, minusLine);


	// 2. Get the Interpolate point of the Three line
	vector<DoublePoint> RetMidPoints,RetPlusPoints,RetMinusPoints;

	NatCubic CSpline;
	CSpline.Spline(midLine, RetMidPoints);
	CSpline.Spline(plusLine, RetPlusPoints);
	CSpline.Spline(minusLine, RetMinusPoints);

	int MM = int(RetMidPoints.size());

	// 3. Get the RetImg range
	int sx, sy, ex, ey;

	double x1, y1, x2, y2;

	//Get Range of RetImg
	GetRange( RetMidPoints, RetPlusPoints, RetMinusPoints, x1, y1, x2, y2);

	x1 = x1 > 0 ? x1 : 0;
	y1 = y1 > 0 ? y1 : 0;

	x2 = x2 < width-1 ? x2 : width-1;
	y2 = y2 < height-1 ? y2 : height-1;

	sx = int(x1);
	sy = int(y1);
	ex = int(x2);
	ey = int(y2);

	NN = int(RetMidPoints.size());

	// RetImg = 255;
	// 4. Morphing	
	for(int i=sy; i<=ey; i++){
		for(int j=sx; j<=ex; j++){
			if(i<0 || j<0 || i>=height || j>=width)
				continue;

			int K;
			double dist_mid, dist_plus, dist_minus;
			NearestPoint(i, j, RetPlusPoints, K, dist_plus);
			NearestPoint(i, j, RetMinusPoints, K, dist_minus);
			NearestPoint(i, j, RetMidPoints, K, dist_mid);


			if(dist_mid>(double)radius+0.01)
				continue;			

			bool isPlus=false;

			if(dist_plus < dist_minus)
				isPlus = true;

			if(K==0 || K==NN-1)
				if(dist_plus+dist_minus>2*radius+0.5)
					continue;


			int i0,j0;
			double ratio_W = K*1.0/NN;
			double ratio_H = dist_mid/radius;


			FindMapPoint(ratio_W, ratio_H, isPlus, AlphaImg, i0, j0);

			if(i0<0 || i0>=h || j0<0 || j0>=w || AlphaImg(j0, i0)< 0.01)
				continue;

			double alpha = pow(AlphaImg(j0, i0)/255.0, 0.6);
			RetImg(j, i, 0) = unsigned char(alpha*r + (1-alpha)*RetImg(j, i, 0));
			RetImg(j, i, 1) = unsigned char(alpha*g + (1-alpha)*RetImg(j, i, 1));
			RetImg(j, i, 2) = unsigned char(alpha*b + (1-alpha)*RetImg(j, i, 2));


			if(AlphaImg(j0, i0) >= 0.1){
				flag(j, i)++;
				flagImage(j, i)++;
			}
		}
	}
}

// Morphing a color image
void CCurveMorphing::MorphingToCurve2(vector<DoublePoint> &midLine,
									 int r, int g, int b,
									 CImg<unsigned char> &RetImg)
{
	vector<DoublePoint> RetMidPoints;

	NatCubic CSpline;
	CSpline.Spline(midLine, RetMidPoints);

	int MM = int(RetMidPoints.size());

	for (int k=0; k<MM; k++)
	{
		int i = (int) (RetMidPoints[k].y + 0.5);
		int j = (int) (RetMidPoints[k].x + 0.5);
		RetImg(j, i, 0) = r;
		RetImg(j, i, 1) = g;
	 	RetImg(j, i, 2) = b;
	}
}
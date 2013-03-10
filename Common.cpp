#include "stdafx.h"
#include "Common.h"
#include <cfloat>
#include <cmath>


/*! This function returns the maximum of two given doubles.
    \param x first parameter
    \param y second parameter
    \return the maximum of these two parameters */
double Max(double x, double y)
{
	return x > y ? x : y;
}

int Max(int x, int y)
{
	return x > y ? x : y;
}

/*! This function returns the minimum of two given doubles.
    \param x first parameter
    \param y second parameter
    \return the minimum of these two parameters */
double Min(double x, double y)
{
	return x < y ? x : y;
}

int Min(int x, int y)
{
	return x < y ? x : y;
}

/*! This function returns the sign of a give double.
    1 is positive, -1 is negative
    \param d1 first parameter
    \return the sign of this double */
int Sign(double x)
{
	return (x >= 0) ? 1: -1;
}

// the distance from point(x1,y1) to point(x2,y2)
double GetDistance(double x1, double y1, double x2, double y2)
{
	return sqrt(Sqr(x2 - x1) + Sqr(y2 - y1));
}

double GetCPointDistance(CPoint &point1, CPoint &point2)
{
	return sqrt((point1.x - point2.x) * (point1.x - point2.x) * 1.0
			  + (point1.y - point2.y) * (point1.y - point2.y) * 1.0);
}

double GetDPointDistance(dPoint &point1, dPoint &point2)
{
	return GetDistance(point1.x, point1.y, point2.x, point2.y);
}

bool IsOnLine(double x1, double y1, double x2, double y2, double x, double y)
{
	double dist1 = GetDistance(x1, y1, x2, y2);
	double dist2 = GetDistance(x1, y1, x, y);
	double dist3 = GetDistance(x, y, x2, y2);
	if(fabs(dist1 - dist2 - dist3) < 0.5)
		return true;
	else
		return false;
}

void PointOnLine(CPoint point1, CPoint point2, vector<CPoint> &pointsVector)
{
	int num1 = abs(point1.x - point2.x);
	int num2 = abs(point1.y - point2.y);
	int num;
	bool Xdirection;

	if(num1 == 0 && num2 == 0)
	{
		pointsVector.push_back(point1);
		return;
	}

	if(num1 >= num2)
	{
		num = num1;
		Xdirection = true;
	}
	else
	{
		num = num2;
		Xdirection = false;
	}

	int x,y;
	if(point1.x >= point2.x)
		x = point2.x;
	else
		x = point1.x;

	if(point1.y >= point2.y)
		y = point2.y;
	else
		y = point1.y;

	CPoint temPoint;
	for(int i = 0; i <= num; i++)
	{
		if(Xdirection)
		{
			temPoint.x = x + i;	
			temPoint.y = (temPoint.x - point1.x) * (point2.y - point1.y) / (point2.x - point1.x) + point1.y;
			pointsVector.push_back(temPoint);
		}
		else
		{
			temPoint.y = y + i;
			temPoint.x = (temPoint.y - point1.y) * (point2.x - point1.x) / (point2.y - point1.y) + point1.x; 
			pointsVector.push_back(temPoint);
		}	
	}
}


int CrossPoint(double x1, double y1, double x2, double y2, double x, double y, double &cx, double &cy)
{
	if(x1 == x2)
	{
		return 0;
	}
	else
	{
		cx = x;
		cy = (cx - x1) * (y2 - y1) / (x2 - x1) + y1;

		if(IsOnLine(x1, y1, x2, y2, cx, cy))
			return 1;
		else
			return 0;
	}
}

void RatioPoint(double x1, double y1, double x2, double y2, double x, double y, double x3, double y3, double x4, double y4, double &rx, double &ry)
{
	double dist1 = GetDistance(x1, y1, x, y);
	double dist2 = GetDistance(x, y, x2, y2);
	
	if(dist2 == 0)
	{
		rx = x4;
		ry = y4;
	}
	else
	{
		double ratio = dist1 / dist2;
		rx = (x4 - x3) * ratio /(ratio + 1) + x3;
		ry = (y4 - y3) * ratio /(ratio + 1) + y3;
	}
}

int TransformPoint(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double x5, double y5, double x6, double y6, double x, double y, double &tx, double &ty)
{
		double cx1,cy1,cx2,cy2,cx3,cy3;
		int r1 = CrossPoint(x1, y1, x2, y2, x, y, cx1, cy1);
		int r2 = CrossPoint(x1, y1, x3, y3, x, y, cx2, cy2);
		int r3 = CrossPoint(x2, y2, x3, y3, x, y, cx3, cy3);

		if(r1 == 1 && r2 == 1 && IsOnLine(cx1, cy1, cx2, cy2, x , y))
		{
			double rx1,ry1,rx2,ry2;
			RatioPoint(x1, y1, x2, y2, cx1, cy1, x4, y4, x5, y5, rx1, ry1);
			RatioPoint(x1, y1, x3, y3, cx2, cy2, x4, y4, x6, y6, rx2, ry2);
			RatioPoint(cx1, cy1, cx2, cy2, x , y, rx1, ry1, rx2, ry2, tx, ty);

			return 1;
		}
		else if(r1 == 1 && r3 == 1 && IsOnLine(cx1, cy1, cx3, cy3, x , y))
		{
			double rx1,ry1,rx2,ry2;
			RatioPoint(x1, y1, x2, y2, cx1, cy1, x4, y4, x5, y5, rx1, ry1);
			RatioPoint(x2, y2, x3, y3, cx3, cy3, x5, y5, x6, y6, rx2, ry2);
			RatioPoint(cx1, cy1, cx3, cy3, x , y, rx1, ry1, rx2, ry2, tx, ty);

			return 1;
		}
		else if(r2 == 1 && r3 == 1 && IsOnLine(cx2, cy2, cx3, cy3, x , y))
		{
			double rx1,ry1,rx2,ry2;
			RatioPoint(x1, y1, x3, y3, cx2, cy2, x4, y4, x6, y6, rx1, ry1);
			RatioPoint(x2, y2, x3, y3, cx3, cy3, x5, y5, x6, y6, rx2, ry2);
			RatioPoint(cx2, cy2, cx3, cy3, x , y, rx1, ry1, rx2, ry2, tx, ty);

			return 1;
		}
		else
		{
			return 0;
		}

//		double t1 = x2 * y3 - x3 * y2;
//		double t2 = x3 * y1 - x1 * y3;
//		double t3 = x1 * y2 - x2 * y1;
//		double det = t1 + t2 + t3;
//
//		double a = ((y2 - y3) * x + (x3 - x2) * y + t1) / det;
//		double b = ((y3 - y1) * x + (x1 - x3) * y + t2) / det;
//		double c = ((y1 - y2) * x + (x2 - x1) * y + t3) / det;
//
//		tx = a * x4 + b * x5 + c * x6;
//		ty = a * y4 + b * y5 + c * y6;
}

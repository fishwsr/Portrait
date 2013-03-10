#ifndef __COMMON_H__
#define __COMMON_H__

#include <vector>
using namespace std;

#define PI					3.14159265358979323846
#define Sqr(x)				((x) * (x))

struct dPoint
{
	double x;
	double y;
};

int			Sign				(double x);
double		Max					(double x, double y);
double		Min					(double x, double y);
int         Max					(int x, int y);
int         Min					(int x, int y);
double		GetDistance			(double x1, double y1, double x2, double y2);
double		GetCPointDistance	(CPoint &point1, CPoint &point2);
double		GetDPointDistance	(dPoint &point1, dPoint &point2);

void		PointOnLine(CPoint point1, CPoint point2, vector<CPoint> &pointsVector);

bool		IsOnLine(double x1, double y1, double x2, double y2, double x, double y);
int			CrossPoint(double x1, double y1, double x2, double y2, double x, double y, double &cx, double &cy);
void		RatioPoint(double x1, double y1, double x2, double y2, double x, double y, double x3, double y3, double x4, double y4, double &rx, double &ry);
int			TransformPoint(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double x5, double y5, double x6, double y6, double x, double y, double &tx, double &ty);


#endif
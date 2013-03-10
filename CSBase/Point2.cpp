#include "stdafx.h"
#include "point2.h"

void CPoint2::Rotate(double theta, double cx, double cy)
{
	double tempx = X-cx;
	double tempy = Y-cy;

	X = tempx*cos(theta) - tempy*sin(theta) + cx;
	Y = tempx*sin(theta) + tempy*cos(theta) + cy;
}

void CPoint2::Translate(double tx, double ty)
{
	X += tx;
	Y += ty;
}

void CPoint2::Scale(double sx, double sy)
{
	sx = (sx<=0 ? NEARLY_ZERO : sx);
	sy = (sy<=0 ? NEARLY_ZERO : sy);

	X *= sx;
	Y *= sy;
}

CPoint2 CPoint2::MiddlePoint(const CPoint2 &pt) const
{
	CPoint2 MiddlePt;

	MiddlePt.X = ((*this).X + pt.X) / 2;
	MiddlePt.Y = ((*this).Y + pt.Y) / 2;

	return MiddlePt;
}

double CPoint2::DistantTo(const CPoint2 &p2) const
{
	CPoint2 tempp;
	tempp = (*this) - p2;

	return sqrt(tempp.X * tempp.X + tempp.Y * tempp.Y);
}

double CPoint2::Inclination(const CPoint2 &p2) const
{
	CPoint2 p1;
	p1 = (*this);

	double dCos = (p1 * p2) / (sqrt(p1 * p1) * sqrt(p2 * p2));

	return acos(dCos);
}

double CPoint2::Normalize(void) const
{
	return sqrt((*this) * (*this));	
}

bool CPoint2::operator==(const CPoint2& pt) const
{
	if ( &pt==this ) return true;

	return X==pt.X && Y==pt.Y;
}

CPoint2 CPoint2::operator*(const double s) const
{
	CPoint2 p;
	p.X = X * (float) s;
	p.Y = Y * (float) s;
	return p;
}

double CPoint2::operator*(const CPoint2 &pt) const
{
	return X * pt.X + Y * pt.Y;
}

CPoint2 CPoint2::operator/(const double s) const
{
	CPoint2 p;
	if (s > 0)
	{
		p.X = X / (float) s;
		p.Y = Y / (float) s;
	}
	else
	{
		p.X = X;
		p.Y = Y;
	}
	return p;
}

CPoint2 CPoint2::operator+(const CPoint2 &pt) const
{
	CPoint2 resPt;

	resPt.X = (*this).X + pt.X;
	resPt.Y = (*this).Y + pt.Y;

	return resPt;
}

CPoint2 CPoint2::operator-(const CPoint2 &pt) const
{
	CPoint2 resPt;
	
	resPt.X = (*this).X - pt.X;
	resPt.Y = (*this).Y - pt.Y;

	return resPt;
}

COutputDataFile& operator<<(COutputDataFile& ofs, const CPoint2& pt)
{
	return ofs << pt.X << pt.Y ; 
}

CInputDataFile& operator>>(CInputDataFile& ifs, CPoint2& pt)
{
	return ifs >> pt.X >> pt.Y ; 
}

void Shape2vPoint2(const CShape& shape, std::vector<CPoint2>& vPoints)
{
	vPoints.resize(shape.PointNum());
	for (int i = 0; i< shape.PointNum(); i++)
	{
		vPoints[i] = CPoint2(shape.PointX(i), shape.PointY(i));
	}
}

void ShapeSet2vPoint2(const CShapeSet& ss, std::vector<CPoint2>& vPoints)
{
	vPoints.clear();
	for (int i=0; i<ss.size(); i++)
	{
		for (int j=0; j<ss[i].PointNum(); j++)
		{
			vPoints.push_back(const_cast<CShapeSet&>(ss)[i].GetPoint(j));
		}
	}
}

void vPoint22Shape(const std::vector<CPoint2>& vPoints, CShape& shape)
{
	shape.Resize(vPoints.size());
	for (int i = 0; i< shape.PointNum(); i++)
	{
		shape.PointX(i) = vPoints[i].X;
		shape.PointY(i) = vPoints[i].Y;
	}
}
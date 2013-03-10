
#pragma once
#include "DataFile.h"
#include "Shape.h"

class CSBASE_API CPoint2 : public Gdiplus::PointF
{
public:
	CPoint2(void) { X = Y = 0; }
	CPoint2(const float x, const float y) { X = x; Y = y; }
	CPoint2(const CPoint2 &ref) { X = ref.X; Y = ref.Y; }

	CPoint2 operator*(const double s) const;
	CPoint2 operator-(const CPoint2 &pt) const;
	CPoint2 operator+(const CPoint2 &pt) const;
	double operator*(const CPoint2 &pt) const;
	CPoint2 operator/(const double s) const;
	bool operator==(const CPoint2 &pt) const;

	CPoint2 MiddlePoint(const CPoint2 &pt) const;
	double DistantTo(const CPoint2 &p2) const;
	double Inclination(const CPoint2 &p2) const;
	double Normalize(void) const;
	
	void	Translate(double tx, double ty);
	void	Rotate(double theta, double cx=0., double cy=0.);
	void	Scale(double sx, double sy);

	bool	InQuad1() const { return X>0 && Y>0; };
	bool	InQuad2() const { return X<0 && Y>0; };
	bool	InQuad3() const { return X<0 && Y<0; };
	bool	InQuad4() const { return X>0 && Y<0; };

	//int SSize(void) const;

	//float X;
	//float Y;
};

#define CVector2 CPoint2

CSBASE_API COutputDataFile& operator<<(COutputDataFile& ofs, const CPoint2& pt);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, CPoint2& pt);

CSBASE_API void Shape2vPoint2(const CShape& shape, std::vector<CPoint2>& vPoints);
CSBASE_API void vPoint22Shape(const std::vector<CPoint2>& vPoints, CShape& shape);
CSBASE_API void ShapeSet2vPoint2(const CShapeSet& ss, std::vector<CPoint2>& vPoints);

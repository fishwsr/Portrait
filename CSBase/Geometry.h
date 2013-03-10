// Geometry.h: interface for the Geometry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEOMETRY_H__5F0E6E45_847E_4940_88EF_A1A39B00F26C__INCLUDED_)
#define AFX_GEOMETRY_H__5F0E6E45_847E_4940_88EF_A1A39B00F26C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Defs.h"

class CSBASE_API Geometry_2D
{
public:
	Geometry_2D(){}
	virtual ~Geometry_2D(){}

	virtual void Translate(double tx, double ty) = 0;
	virtual void Scale(double sx, double sy) = 0;
	virtual void Rotate(double theta, double cx=0., double cy=0.) = 0;
};

// for every point (x,y) on the line: x*cos(theta)+y*sin(theta)=r
class CSBASE_API Line_Eq : public Geometry_2D
{
public:
	Line_Eq(double r=0., double theta=0.);
	virtual ~Line_Eq();
	const Line_Eq& operator=(const Line_Eq& refLine);

	const	double	Angle() const;						// angle between line and positive x-axis, [0, 2*PI)
	const	double	Dist2Org() const { return m_fR;}		// distance from origin to line
	const	double	Theta() const { return m_fTheta;}		// angle between X-axis and the perpendicular line

	CPoint2	Normal();
	CPoint2 Tangent();

	void	SetR(double r);					// set nearest distance from origin to line
	void	SetTheta(double theta) { m_fTheta=theta; }	

//	double		AngleWith(Line_Eq& refline);		 // (m_fTheta - refline.m_fTheta), in [0, 2*MY_PI)
	double		SmallestAngleWith(Line_Eq& refline); // smallest angle with another line, in [0, MY_PI/2.)
	CPoint2		NearestPt2Org();				   	 // nearest point to origin
	bool		ParallelWith(Line_Eq& refLine, double tolerance=NEARLY_ZERO);	 // if parallel to reference line
	bool		PtOnLine(double x, double y, double tolerance=NEARLY_ZERO);		 // true if p is on the line
	CPoint2		IntersectionWith(Line_Eq& line);		 // intersection point with another line


	double		DistantTo(CPoint2& p2);			// perpendicular distance from point to line
	double		DistantTo(Line_Eq& line);		// distance to parrallel line
	
	virtual	void Translate(double tx, double ty);
	virtual	void Scale(double sx, double sy);
	virtual	void Rotate(double theta, double cx=0., double cy=0.);

protected:
	double m_fR;			
	double m_fTheta;
};

CSBASE_API COutputDataFile& operator<<(COutputDataFile& ofs, const Line_Eq& line);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, Line_Eq& line);

class CSBASE_API LineSeg_Eq : public Line_Eq
{
public:
	LineSeg_Eq();
	LineSeg_Eq(CPoint2 p1, CPoint2 p2);
	LineSeg_Eq(double x1, double y1, double x2, double y2);
	virtual ~LineSeg_Eq();
	const LineSeg_Eq& operator=(const LineSeg_Eq& refLineSeg);
	
	const CPoint2	P1() const { return m_EndPt1;}
	const CPoint2	P2() const { return m_EndPt2;}

	CPoint2 Normal();
	CPoint2 Tangent();

	double	Length() { return m_EndPt1.DistantTo(m_EndPt2); }				// length of the line segment
	bool	PtOnLineSeg(double x, double y, double tolerance=NEARLY_ZERO);	// point on the segment
	bool	SegOnLine(double tolerance=NEARLY_ZERO);						// true if the segment is on line
	void	Update();														// update the line parameter r and theta by end points
	void	SetEndPts(double x1, double y1, double x2, double y2);

	CPoint2	MiddlePoint();	// middle point of the line segment
	
	virtual	void Translate(double tx, double ty);	
	virtual void Scale(double sx, double sy);		
	virtual void Rotate(double theta, double cx=0., double cy=0.);

protected:
	CPoint2	m_EndPt1;
	CPoint2 m_EndPt2;
};

CSBASE_API COutputDataFile& operator<<(COutputDataFile& ofs, const LineSeg_Eq& lineSeg);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, LineSeg_Eq& lineSeg);

class CSBASE_API Rectangle_Eq : public Geometry_2D
{
public:
	Rectangle_Eq();
	Rectangle_Eq(CPoint2 c1, CPoint2 c2, CPoint2 c3, CPoint2 c4);
	virtual ~Rectangle_Eq();
	const Rectangle_Eq& operator=(const Rectangle_Eq& refRect);
	
	const CPoint2 Corner(int i) const { if (i>=0&&i<4) return m_vCorners[i]; CPoint2 tempP2; return tempP2; }
	
	void SetCorner(int i, double x, double y) { if (i>=0&&i<4) { m_vCorners[i].X=x; m_vCorners[i].Y=y; } }
	void Set4Corners(CPoint2 c1, CPoint2 c2, CPoint2 c3, CPoint2 c4);

	double Height();
	double Width();
	CPoint2 Center();
	
	virtual void Translate(double tx, double ty);
	virtual void Scale(double sx, double sy);		
	virtual void Rotate(double theta, double cx=0., double cy=0.);

protected:
	CPoint2 m_vCorners[4];
};

CSBASE_API COutputDataFile& operator<<(COutputDataFile& ofs, const Rectangle_Eq& rect);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, Rectangle_Eq& rect);

class CSBASE_API Circle_Eq : public Geometry_2D
{
public:
	Circle_Eq(double r=1., double cx=0., double cy=0.);
	virtual ~Circle_Eq();
	const Circle_Eq& operator=(const Circle_Eq& refCircle);

	bool	InitByPts(std::vector<CPoint2>& vPoint2);
	void	SetCenter(double x, double y) { m_Center.X=x; m_Center.Y=y;}
	void	SetR(double r)	 { m_fR = fabs(r); }
	bool	PtOnCircle(double x, double y, double tolerance=NEARLY_ZERO);	

	const CPoint2	Center() const { return m_Center;}
	const double	Radius() const { return m_fR; }
	const double	Perimeter() const { return 2*MY_PI*m_fR; }
	const double	Diameter() const { return 2*m_fR; }
	const double	Area() const { return MY_PI*m_fR*m_fR; }

	CPoint2	Tangent(double x, double y);
	CPoint2 Normal(double x, double y);

	virtual	void Translate(double tx, double ty);	
	virtual void Scale(double sx, double sy);		
	virtual void Rotate(double theta, double cx=0., double cy=0.);

protected:
	double	m_fR;
	CPoint2	m_Center;
};

CSBASE_API COutputDataFile& operator<<(COutputDataFile& ofs, const Circle_Eq& circle);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, Circle_Eq& circle);

class CSBASE_API Ellipse_Eq : public Geometry_2D
{
public:
	Ellipse_Eq(double fA=1.414, double fx1=-1., double fy1=0., double fx2=1., double fy2=0.);
	virtual ~Ellipse_Eq();
	const Ellipse_Eq& operator=(const Ellipse_Eq& refEllipse);
	
	const CPoint2 F1() const {return m_Foci1;}
	const CPoint2 F2() const {return m_Foci2;}
	const double A() const { return m_fSemiMajorAxis;}
	const double C() const { return 0.5*m_Foci1.DistantTo(m_Foci2); }
	const CPoint2 Center() const { return m_Foci1.MiddlePoint(m_Foci2); }

	const double B() const;
	const double B_Sq() const;
	const double R(double theta) const;		// theta is the angle between line from ellipse center to (p.x, p.y)
	const double R_Sq(double theta) const;	// with the line passing (f1.x, f1.y) and (f2.x, f2.y)

	const double Perimeter() const;
	const double Area() const;

	CPoint2 Tangent(double x, double y);
	CPoint2 Normal(double x, double y);

	void	SetF(double fx1, double fy1, double fx2, double fy2);
	void	SetA(double fA);
	bool	PtOnEllipse(double x, double y, double tolerance=NEARLY_ZERO);

	virtual	void Translate(double tx, double ty);	
	virtual void Scale(double sx, double sy);		
	virtual void Rotate(double theta, double cx=0., double cy=0.);

protected:
	CPoint2 m_Foci1;
	CPoint2 m_Foci2;
	double  m_fSemiMajorAxis;
};

//CSBASE_API double ShapeDiff_Mean(Circle_Eq &c1, Circle_Eq &c2);
//CSBASE_API double ShapeDiff_Var(Circle_Eq &c1, Circle_Eq &c2);
//CSBASE_API double ShapeDiff_Mean(Ellipse_Eq &e1, Ellipse_Eq &e2);
//CSBASE_API double ShapeDiff_Var(Ellipse_Eq &e1, Ellipse_Eq &e2);

CSBASE_API void Circle2Ellipse(Circle_Eq &circle, Ellipse_Eq &ellipse);
CSBASE_API bool Fit_Ellipse(std::vector<CPoint2> &vPoint2, Ellipse_Eq &ellipse, double theta_tolerance=0.001);
CSBASE_API bool Fit_Ellipse(std::vector<CPoint2> & vPoint2, CPoint2& f1, CPoint2& f2, double& fa, double theta_tolerance=0.001);

CSBASE_API COutputDataFile& operator<<(COutputDataFile& ofs, const Ellipse_Eq& ellipse);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, Ellipse_Eq& ellipse);

template<class _GeomType>
class CSBASE_API Geometry_2D_Set : public std::vector<_GeomType>
{
public:
	Geometry_2D_Set() {}
	virtual ~Geometry_2D_Set() {}

	void Translate(double tx, double ty)
	{
		for (int i=0; i<size(); i++)
			(*this)[i].Translate(tx, ty);
	}
	void Scale(double sx, double sy)
	{
		for (int i=0; i<size(); i++)
			(*this)[i].Scale(sx, sy);
	}
	void Rotate(double theta, double cx=0., double cy=0.)
	{
		for (int i=0; i<size(); i++)
			(*this)[i].Rotate(theta, cx, cy);
	}
};

#define Line_Eq_Set		 Geometry_2D_Set<Line_Eq>
#define LineSeg_Eq_Set	 Geometry_2D_Set<LineSeg_Eq>
#define Circle_Eq_Set	 Geometry_2D_Set<Circle_Eq>
#define Ellipse_Eq_Set	 Geometry_2D_Set<Ellipse_Eq>
#define Rectangle_Eq_Set Geometry_2D_Set<Rectangle_Eq> 

CSBASE_API COutputDataFile& operator<<(COutputDataFile& ofs, const Circle_Eq_Set& vGeom);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, Circle_Eq_Set& vGeom);

CSBASE_API COutputDataFile& operator<<(COutputDataFile& ofs, const Ellipse_Eq_Set& vGeom);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, Ellipse_Eq_Set& vGeom);

CSBASE_API COutputDataFile& operator<<(COutputDataFile& ofs, const LineSeg_Eq_Set& vGeom);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, LineSeg_Eq_Set& vGeom);

CSBASE_API COutputDataFile& operator<<(COutputDataFile& ofs, const Rectangle_Eq_Set& vGeom);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, Rectangle_Eq_Set& vGeom);

CSBASE_API void RankBySize(Circle_Eq_Set &vCircle);
CSBASE_API void RankBySize(Ellipse_Eq_Set &vEllipse);
CSBASE_API void RankBySize(Rectangle_Eq_Set &vRect);
/*
template<class _GeomType>
CSBASE_API COutputDataFile& operator<<(COutputDataFile& ofs, const Geometry_2D_Set<_GeomType>& vGeom)
{
	ofs << int(vGeom.size()) << sep_endl;
	for (int i=0; i<vGeom.size(); i++)
	{
		ofs << vGeom[i] << sep_endl;
	}

	return ofs;
}

template<class _GeomType>
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, Geometry_2D_Set<_GeomType>& vGeom)
{
	int nSize;
	ifs >> nSize >> sep_endl;
	vGeom.resize(nSize);
	for (int i=0; i<vGeom.size(); i++)
	{
		ifs >> vGeom[i] >> sep_endl;
	}

	return ifs;
}

template<class _GeomType>
CSBASE_API CInputDataFile& Input(CInputDataFile& fs, Geometry_2D_Set<_GeomType>& vGeom)
{
	fs >> vGeom;
	return fs;	
}

template<class _GeomType>
CSBASE_API COutputDataFile& Output(COutputDataFile& fs, const Geometry_2D_Set<_GeomType>& vGeom)
{
	fs << vGeom;
	return fs;
}
*/
CSBASE_API COutputDataFile& operator<<(COutputDataFile& ofs, const Ellipse_Eq_Set& vEllipse);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, Ellipse_Eq_Set& vEllipse);

CSBASE_API COutputDataFile& operator<<(COutputDataFile& ofs, const Circle_Eq_Set& vCircle);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, Circle_Eq_Set& vCircle);

CSBASE_API COutputDataFile& operator<<(COutputDataFile& ofs, const Rectangle_Eq_Set& vRect);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, Rectangle_Eq_Set& vRect);

CSBASE_API COutputDataFile& operator<<(COutputDataFile& ofs, const LineSeg_Eq_Set& vLineSeg);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, LineSeg_Eq_Set& vLineSeg);

CSBASE_API bool ReadHanfengRects(const char* strFileName, Rectangle_Eq_Set &vRect);

CSBASE_API bool GetLineEndPointsWithImageFrame(int width, int height, double r, double theta, CPoint2& p0, CPoint2& p1);

CSBASE_API void SampleEllipse(std::vector<CPoint2>& vPoint2, CPoint2 f1, CPoint2 f2, double fa, int nTheta);
CSBASE_API void SampleLineSeg(std::vector<CPoint2>& vPoint2, CPoint2 p0, CPoint2 p1, int nPts);
CSBASE_API void SampleCircle(std::vector<CPoint2>& vPoint2, double cx, double cy, double r, int nTheta);
CSBASE_API void SampleRing(std::vector<CPoint2>& vPoint2, double cx, double cy, int minR, int maxR);
CSBASE_API void SampleRound(std::vector<CPoint2>& vPoint2, double cx, double cy, int r);

CSBASE_API void Combine2vLineSeg(LineSeg_Eq_Set& vLineSeg1, LineSeg_Eq_Set& vLineSeg2);
CSBASE_API void Shape2vLineSeg(CShape& shape, LineSeg_Eq_Set& vSeg);
CSBASE_API void ShapeSet2vLineSeg(CShapeSet& ss, LineSeg_Eq_Set& vSeg);

#endif // !defined(AFX_GEOMETRY_H__5F0E6E45_847E_4940_88EF_A1A39B00F26C__INCLUDED_)

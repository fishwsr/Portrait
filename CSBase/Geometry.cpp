// Geometry.cpp: implementation of the Geometry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Geometry.h"

///////////// Line_Eq //////////////
Line_Eq::Line_Eq(double r, double theta)
{
	m_fR = fabs(r);
	m_fTheta = theta;
}

Line_Eq::~Line_Eq()
{
}

const Line_Eq& Line_Eq::operator=(const Line_Eq& refLine)
{
	if ( &refLine != this )
	{
		m_fR = refLine.m_fR;
		m_fTheta = refLine.m_fTheta;
	}

	return *this;
}

const double Line_Eq::Angle() const
{
	double tempf = 0.5*MY_PI + m_fTheta;

	return tempf>=2.*MY_PI ? tempf-2.*MY_PI : tempf;
}

/*
double Line_Eq::LineOri()
{
	double tempf = MY_PI - m_fTheta;
	return tempf<0 ? (MY_PI+tempf) : tempf; 
}
*/

CPoint2 Line_Eq::NearestPt2Org()
{
	CPoint2 p;
	p.X = m_fR * cos(m_fTheta);
	p.Y = m_fR * sin(m_fTheta);
	return p;
}

CPoint2 Line_Eq::Normal()
{
	CPoint2 p2;

	p2.X = cos(m_fTheta);
	p2.Y = sin(m_fTheta);

	return p2;
}

CPoint2 Line_Eq::Tangent()
{
	CPoint2 p2 = Normal();
	
	double tempf = p2.X;
	p2.X = -p2.Y;
	p2.Y = tempf;
	
	return p2;
}

bool Line_Eq::PtOnLine(double x, double y, double tolerance)
{
	return fabs(fabs(x*cos(m_fTheta)+y*sin(m_fTheta))-m_fR) <= tolerance;
}

bool Line_Eq::ParallelWith(Line_Eq& refLine, double tolerance)
{
	return SmallestAngleWith(refLine) <= tolerance; 
}

void Line_Eq::SetR(double r)
{
	m_fR = fabs(r);
}

//double Line_Eq::AngleWith(Line_Eq& refline)
//{
//	return m_fTheta - refline.m_fTheta;
//}

double Line_Eq::SmallestAngleWith(Line_Eq& refline)
{
	double tempf = fabs(m_fTheta - refline.m_fTheta);
	
	if ( tempf>0.5*MY_PI && tempf<=1.5*MY_PI)
		tempf = fabs(MY_PI - tempf);
	else if ( tempf>1.5*MY_PI && tempf<2*MY_PI )
		tempf = 2*MY_PI - tempf;

	return tempf;
}

CPoint2 Line_Eq::IntersectionWith(Line_Eq& line)
{
	double tempf = cos(line.m_fTheta)*sin(m_fTheta)-sin(line.m_fTheta)*cos(m_fTheta);

	tempf = (fabs(tempf)<=NEARLY_ZERO) ? SIGN(tempf)*NEARLY_ZERO : tempf;
	
	CPoint2 p2;
	p2.X = (m_fR*cos(line.m_fTheta)-line.m_fR*cos(m_fTheta)) / tempf;
	p2.Y = (m_fR*sin(line.m_fTheta)-line.m_fR*sin(m_fTheta)) / (-tempf);

	return p2;
}

double Line_Eq::DistantTo(Line_Eq& line)
{
	double tempf = SmallestAngleWith(line);

	if ( tempf>NEARLY_ZERO ) // intersect 
	{
		return 0.;
	}
	else					 // parallel
	{
		tempf = fabs(m_fTheta - line.m_fTheta);
		if ( tempf<=NEARLY_ZERO )
			return fabs(m_fR - line.m_fR);
		else
			return m_fR + line.m_fR;
	}
}

double Line_Eq::DistantTo(CPoint2& p2)
{
	return fabs( p2.X*cos(m_fTheta)+p2.Y*sin(m_fTheta)-m_fR );
}

void Line_Eq::Translate(double tx, double ty)
{
	LineSeg_Eq lineSeg;

	if ( NEARLY_EQUAL(m_fTheta, 0) )
	{
		lineSeg.SetEndPts(m_fR, -m_fR, m_fR, m_fR);
	}
	else if ( NEARLY_EQUAL(m_fTheta, 0.5*MY_PI) )
	{
		lineSeg.SetEndPts(-m_fR, m_fR, m_fR, m_fR);
	}
	else if ( NEARLY_EQUAL(m_fTheta, MY_PI) )
	{
		lineSeg.SetEndPts(-m_fR, -m_fR, -m_fR, m_fR);
	}
	else if ( NEARLY_EQUAL(m_fTheta, 1.5*MY_PI) )
	{
		lineSeg.SetEndPts(-m_fR, -m_fR, m_fR, -m_fR);
	}
	else 
	{
		lineSeg.SetEndPts(0., m_fR/sin(m_fTheta), m_fR/cos(m_fTheta), 0.);
	}

	lineSeg.Translate(tx, ty);
	m_fR = lineSeg.m_fR;
	m_fTheta = lineSeg.m_fTheta;
}

void Line_Eq::Scale(double sx, double sy)
{
	LineSeg_Eq lineSeg;

	if ( NEARLY_EQUAL(m_fTheta, 0) )
	{
		lineSeg.SetEndPts(m_fR, -m_fR, m_fR, m_fR);
	}
	else if ( NEARLY_EQUAL(m_fTheta, 0.5*MY_PI) )
	{
		lineSeg.SetEndPts(-m_fR, m_fR, m_fR, m_fR);
	}
	else if ( NEARLY_EQUAL(m_fTheta, MY_PI) )
	{
		lineSeg.SetEndPts(-m_fR, -m_fR, -m_fR, m_fR);
	}
	else if ( NEARLY_EQUAL(m_fTheta, 1.5*MY_PI) )
	{
		lineSeg.SetEndPts(-m_fR, -m_fR, m_fR, -m_fR);
	}
	else 
	{
		lineSeg.SetEndPts(0., m_fR/sin(m_fTheta), m_fR/cos(m_fTheta), 0.);
	}
	
	lineSeg.Scale(sx, sy);
	m_fR = lineSeg.m_fR;
	m_fTheta = lineSeg.m_fTheta;
}

void Line_Eq::Rotate(double theta, double cx, double cy)
{
	LineSeg_Eq lineSeg;

	if ( NEARLY_EQUAL(m_fTheta, 0) )
	{
		lineSeg.SetEndPts(m_fR, -m_fR, m_fR, m_fR);
	}
	else if ( NEARLY_EQUAL(m_fTheta, 0.5*MY_PI) )
	{
		lineSeg.SetEndPts(-m_fR, m_fR, m_fR, m_fR);
	}
	else if ( NEARLY_EQUAL(m_fTheta, MY_PI) )
	{
		lineSeg.SetEndPts(-m_fR, -m_fR, -m_fR, m_fR);
	}
	else if ( NEARLY_EQUAL(m_fTheta, 1.5*MY_PI) )
	{
		lineSeg.SetEndPts(-m_fR, -m_fR, m_fR, -m_fR);
	}
	else 
	{
		lineSeg.SetEndPts(0., m_fR/sin(m_fTheta), m_fR/cos(m_fTheta), 0.);
	}

	lineSeg.Rotate(theta, cx, cy);
	m_fR = lineSeg.m_fR;
	m_fTheta = lineSeg.m_fTheta;
}

COutputDataFile& operator<<(COutputDataFile& ofs, const Line_Eq& line)
{
	return ofs << line.Dist2Org() << line.Theta() ; 
}

CInputDataFile& operator>>(CInputDataFile& ifs, Line_Eq& line)
{
	double r, theta;
	ifs >> r >> theta;

	line.SetR(r);
	line.SetTheta(theta);

	return ifs;
}

///////////// LineSeg_Eq ////////////////
LineSeg_Eq::LineSeg_Eq()
{
	m_EndPt1.X = m_EndPt1.Y = m_EndPt2.X = m_EndPt2.Y = 0.;
	Update();
}

LineSeg_Eq::LineSeg_Eq(CPoint2 p1, CPoint2 p2)
{
	m_EndPt1 = p1;
	m_EndPt2 = p2;
	Update();
}

LineSeg_Eq::LineSeg_Eq(double x1, double y1, double x2, double y2)
{
	m_EndPt1.X = x1;
	m_EndPt1.Y = y1;
	m_EndPt2.X = x2;
	m_EndPt2.Y = y2;
	Update();
}

LineSeg_Eq::~LineSeg_Eq()
{
}

const LineSeg_Eq& LineSeg_Eq::operator=(const LineSeg_Eq& refLineSeg)
{
	if ( &refLineSeg != this )
	{
		m_EndPt1 = refLineSeg.m_EndPt1;
		m_EndPt2 = refLineSeg.m_EndPt2;

		m_fR = refLineSeg.m_fR;
		m_fTheta = refLineSeg.m_fTheta;
	}

	return *this;
}

CPoint2 LineSeg_Eq::Tangent()
{
	CPoint2 p2;
	double tempf = m_EndPt1.DistantTo(m_EndPt2);

	if ( tempf>NEARLY_ZERO )
	{
		p2.X = (m_EndPt1.X-m_EndPt2.X)/tempf;
		p2.Y = (m_EndPt1.Y-m_EndPt2.Y)/tempf;
	}

	return p2;
}

CPoint2 LineSeg_Eq::Normal()
{
	CPoint2 p2 = Tangent();
	
	double tempf = p2.X;
	p2.X = -p2.Y;
	p2.Y = tempf;

	return p2;
}

void LineSeg_Eq::SetEndPts(double x1, double y1, double x2, double y2)
{
	m_EndPt1.X = x1;
	m_EndPt1.Y = y1;
	m_EndPt2.X = x2;
	m_EndPt2.Y = y2;
	Update();
}

bool LineSeg_Eq::SegOnLine(double tolerance)
{
	return PtOnLine(m_EndPt1.X, m_EndPt1.Y, tolerance) && PtOnLine(m_EndPt2.X, m_EndPt2.Y, tolerance);
}

void LineSeg_Eq::Update()
{
	if ( SegOnLine() ) return;

	double theta = atan2(m_EndPt1.X-m_EndPt2.X, m_EndPt2.Y-m_EndPt1.Y);
	if (theta<0) theta += 2.*MY_PI;

	double r1 = m_EndPt1.X*cos(theta)+m_EndPt1.Y*sin(theta);
	double r2 = m_EndPt2.X*cos(theta)+m_EndPt2.Y*sin(theta);
	if ( r1>=0 && r2>=0 && NEARLY_EQUAL(r1, r2) )
		m_fTheta = theta;
	else
	{
		m_fTheta = theta + MY_PI; 
		if (m_fTheta>=2.*MY_PI) m_fTheta -= 2.*MY_PI;
	}
	
	m_fR = m_EndPt1.X*cos(m_fTheta) + m_EndPt1.Y*sin(m_fTheta);
	
/*
	if ( NEARLY_EQUAL(m_EndPt1.X, m_EndPt2.X) )
	{
		m_fR = fabs(m_EndPt1.X);
		if ( m_EndPt1.X>=0 )
			m_fTheta = 0;
		else
			m_fTheta = MY_PI;
	}
	else if ( NEARLY_EQUAL(m_EndPt1.Y, m_EndPt2.Y) )
	{
		m_fR = fabs(m_EndPt1.X);
		if ( m_EndPt1.Y>=0 )
			m_fTheta = 0.5*MY_PI;
		else
			m_fTheta = 1.5*MY_PI;
	}
	else 
	{
		double theta = atan2(m_EndPt1.X-m_EndPt2.X, m_EndPt2.Y-m_EndPt1.Y);
		
		double r1 = m_EndPt1.X*cos(theta)+m_EndPt1.Y*sin(theta);
		double r2 = m_EndPt2.X*cos(theta)+m_EndPt2.Y*sin(theta);
		if ( NEARLY_EQUAL(r1, r2) )
			m_fTheta = theta;
		else
			m_fTheta += MY_PI; 

		m_fR = fabs(m_EndPt1.X*cos(theta) + m_EndPt1.Y*sin(theta));
	}	
*/
}

CPoint2 LineSeg_Eq::MiddlePoint()
{
	return m_EndPt1.MiddlePoint(m_EndPt2);
}

void LineSeg_Eq::Translate(double tx, double ty)
{
	m_EndPt1.Translate(tx, ty);
	m_EndPt2.Translate(tx, ty);
	Update();
}

void LineSeg_Eq::Scale(double sx, double sy)
{
	sx = (sx<=0 ? NEARLY_ZERO : sx);
	sy = (sy<=0 ? NEARLY_ZERO : sy);

	m_EndPt1.Scale(sx, sy);
	m_EndPt2.Scale(sx, sy);
	Update();
}

void LineSeg_Eq::Rotate(double theta, double cx, double cy)
{
	m_EndPt1.Rotate(theta, cx, cy);
	m_EndPt2.Rotate(theta, cx, cy);
	Update();
}

bool LineSeg_Eq::PtOnLineSeg(double x, double y, double tolerance)
{
	if ( !PtOnLine(x, y, tolerance) ) return false;

	if ( m_EndPt1.X < m_EndPt2.X )
	{
		if ( m_EndPt1.Y <m_EndPt2.Y )
			return x>=m_EndPt1.X && x<=m_EndPt2.X && y>=m_EndPt1.Y && y<=m_EndPt2.Y;
		else
			return x>=m_EndPt1.X && x<=m_EndPt2.X && y>=m_EndPt2.Y && y<=m_EndPt1.Y;
	}
	else
	{
		if ( m_EndPt1.Y <m_EndPt2.Y )
			return x>=m_EndPt2.X && x<=m_EndPt1.X && y>=m_EndPt1.Y && y<=m_EndPt2.Y;
		else
			return x>=m_EndPt2.X && x<=m_EndPt1.X && y>=m_EndPt2.Y && y<=m_EndPt1.Y;
	}
}

COutputDataFile& operator<<(COutputDataFile& ofs, const LineSeg_Eq& lineSeg)
{
	return ofs << lineSeg.Dist2Org() << lineSeg.Theta() << lineSeg.P1() << lineSeg.P2() ; 
}

CInputDataFile& operator>>(CInputDataFile& ifs, LineSeg_Eq& lineSeg)
{
	double r, theta;
	CPoint2 p1, p2;

	ifs >> r >> theta >> p1 >> p2;
	
	lineSeg.SetR(r);
	lineSeg.SetTheta(theta);
	lineSeg.SetEndPts(p1.X, p1.Y, p2.X, p2.Y);

	return ifs; 
}

///////////// Rectangle_Eq ////////////
Rectangle_Eq::Rectangle_Eq()
{
	for (int i=0; i<4; i++)
	{
		m_vCorners[i].X = 0.;
		m_vCorners[i].Y = 0.;
	}
}

Rectangle_Eq::Rectangle_Eq(CPoint2 c1, CPoint2 c2, CPoint2 c3, CPoint2 c4)
{
	Set4Corners(c1, c2, c3, c4);
}

Rectangle_Eq::~Rectangle_Eq()
{
}

const Rectangle_Eq& Rectangle_Eq::operator=(const Rectangle_Eq& refRect)
{
	if ( this != &refRect )
	{
		m_vCorners[0] = refRect.m_vCorners[0];
		m_vCorners[1] = refRect.m_vCorners[1];
		m_vCorners[2] = refRect.m_vCorners[2];
		m_vCorners[3] = refRect.m_vCorners[3];
	}
	return *this;
}

void Rectangle_Eq::Set4Corners(CPoint2 c1, CPoint2 c2, CPoint2 c3, CPoint2 c4)
{
	m_vCorners[0] = c1;
	m_vCorners[1] = c2;
	m_vCorners[2] = c3;
	m_vCorners[3] = c4;
}

CPoint2 Rectangle_Eq::Center()
{
	CPoint2 p2;
	p2.X = 0.25*(m_vCorners[0].X + m_vCorners[1].X + m_vCorners[2].X + m_vCorners[3].X);
	p2.Y = 0.25*(m_vCorners[0].Y + m_vCorners[1].Y + m_vCorners[2].Y + m_vCorners[3].Y);

	return p2;
}

double Rectangle_Eq::Height()
{
	CPoint2 p01, p23;
	p01 = m_vCorners[0].MiddlePoint(m_vCorners[1]);
	p23 = m_vCorners[2].MiddlePoint(m_vCorners[3]);

	return p01.DistantTo(p23);
}

double Rectangle_Eq::Width()
{
	CPoint2 p03, p12;
	p03 = m_vCorners[0].MiddlePoint(m_vCorners[3]);
	p12 = m_vCorners[1].MiddlePoint(m_vCorners[2]);

	return p03.DistantTo(p12);
}

void Rectangle_Eq::Translate(double tx, double ty)
{
	for (int i=0; i<4; i++)
	{
		m_vCorners[i].Translate(tx, ty);
	}
}

void Rectangle_Eq::Rotate(double theta, double cx, double cy)
{
	for (int i=0; i<4; i++)
	{
		m_vCorners[i].Rotate(theta, cx, cy);
	}
}

void Rectangle_Eq::Scale(double sx, double sy)
{
	for (int i=0; i<4; i++)
	{
		m_vCorners[i].Scale(sx, sy);
	}
}

COutputDataFile& operator<<(COutputDataFile& ofs, const Rectangle_Eq& rect)
{
	return ofs << rect.Corner(0) << rect.Corner(1) << rect.Corner(2) << rect.Corner(3) ; 
}

CInputDataFile& operator>>(CInputDataFile& ifs, Rectangle_Eq& rect)
{
	CPoint2 vCorner[4];

	ifs >> vCorner[0] >> vCorner[1] >> vCorner[2] >> vCorner[3] ;
	
	rect.Set4Corners(vCorner[0], vCorner[1], vCorner[2], vCorner[3]);

	return ifs;
}

///////////// Circle_Eq ///////////////
Circle_Eq::Circle_Eq(double r/* =1. */, double cx/* =0. */, double cy/* =0. */)
{
	m_fR = fabs(r);
	m_Center.X = cx;
	m_Center.Y = cy;
}

Circle_Eq::~Circle_Eq()
{
}

const Circle_Eq& Circle_Eq::operator=(const Circle_Eq& refCircle)
{
	if ( &refCircle != this )
	{
		m_fR = refCircle.m_fR;
		m_Center = refCircle.m_Center;
	}

	return *this;
}

CPoint2 Circle_Eq::Normal(double x, double y)
{
	CPoint2 p2;
	double tempf = sqrt(SQUARE(m_Center.X-x)+SQUARE(m_Center.Y-y));
	
	if ( tempf > NEARLY_ZERO ) 
	{
		p2.X = (x-m_Center.X)/tempf;
		p2.Y = (y-m_Center.Y)/tempf;
	}
	return p2;
}

CPoint2 Circle_Eq::Tangent(double x, double y)
{
	CPoint2 p2 = Normal(x, y);
	
	double tempf = p2.X;
	p2.X = -p2.Y;
	p2.Y = tempf;

	return p2;
}

bool Circle_Eq::InitByPts(std::vector<CPoint2>& vPoint2)
{
	if (vPoint2.size()!=3) return false;

	double a, d, e, f, tempf0, tempf1, tempf2;

	tempf0 = vPoint2[0].X*vPoint2[0].X + vPoint2[0].Y*vPoint2[0].Y;
	tempf1 = vPoint2[1].X*vPoint2[1].X + vPoint2[1].Y*vPoint2[1].Y;
	tempf2 = vPoint2[2].X*vPoint2[2].X + vPoint2[2].Y*vPoint2[2].Y;
	
	CVisDMatrix mat(3,3);

	mat[0][0]=vPoint2[0].X; mat[0][1]=vPoint2[0].Y; mat[0][2]=1;
	mat[1][0]=vPoint2[1].X; mat[1][1]=vPoint2[1].Y; mat[1][2]=1;
	mat[2][0]=vPoint2[2].X; mat[2][1]=vPoint2[2].Y; mat[2][2]=1;
	a = mat.Determinant();
	if ( a<=0 ) return false;

	mat[0][0]=tempf0; 
	mat[1][0]=tempf1; 
	mat[2][0]=tempf2;
	d = -mat.Determinant();

	mat[0][1]=vPoint2[0].X;
	mat[1][1]=vPoint2[1].X;
	mat[2][1]=vPoint2[2].X;
	e = mat.Determinant();

	mat[0][2]=vPoint2[0].Y;
	mat[1][2]=vPoint2[1].Y;
	mat[2][2]=vPoint2[2].Y;
	f = -mat.Determinant();

	m_Center.X = -d/(2.*a);
	m_Center.Y = -e/(2.*a);
	m_fR = sqrt((d*d+e*e)/(4.*a*a) - f/a);
	return true;
}

bool Circle_Eq::PtOnCircle(double x, double y, double tolerance)
{ 
	return fabs(sqrt(SQUARE(m_Center.X-x)+SQUARE(m_Center.Y-y))-m_fR) <= tolerance; 
}

void Circle_Eq::Translate(double tx, double ty)
{
	m_Center.Translate(tx, ty);
}

void Circle_Eq::Scale(double sx, double sy)
{
	sx = (sx<=0 ? NEARLY_ZERO : sx);
	sy = (sy<=0 ? NEARLY_ZERO : sy);

	m_fR *= sx;
}

void Circle_Eq::Rotate(double theta, double cx/* =0. */, double cy/* =0. */)
{
	m_Center.Rotate(theta, cx, cy);
}

COutputDataFile& operator<<(COutputDataFile& ofs, const Circle_Eq& circle)
{
	return ofs << circle.Center() << circle.Radius() ; 
}

CInputDataFile& operator>>(CInputDataFile& ifs, Circle_Eq& circle)
{
	double r;
	CPoint2 center;

	ifs >> center >> r ;

	circle.SetCenter(center.X, center.Y);
	circle.SetR(r);

	return ifs; 
}

//////////////// Ellipse_Eq //////////////////
Ellipse_Eq::Ellipse_Eq(double fA/* =1.414 */, double fx1/* =-1. */, double fy1/* =0. */, double fx2/* =1. */, double fy2/* =0. */)
{
	m_Foci1.X = fx1;
	m_Foci1.Y = fy1;

	m_Foci2.X = fx2;
	m_Foci2.Y = fy2;
	
	SetA(fA);
}

Ellipse_Eq::~Ellipse_Eq()
{
}

const Ellipse_Eq& Ellipse_Eq::operator=(const Ellipse_Eq& refEllipse)
{
	if (this != &refEllipse)
	{
		m_Foci1 = refEllipse.m_Foci1;
		m_Foci2 = refEllipse.m_Foci2;
		m_fSemiMajorAxis = refEllipse.m_fSemiMajorAxis;
	}
	
	return *this;
}

void Ellipse_Eq::SetF(double fx1, double fy1, double fx2, double fy2)
{
	m_Foci1.X = fx1;
	m_Foci1.Y = fy1;

	m_Foci2.X = fx2;
	m_Foci2.Y = fy2;

	SetA(m_fSemiMajorAxis);
}

void Ellipse_Eq::SetA(double fA)
{
	double tempf = C();
	m_fSemiMajorAxis = fA<tempf ? tempf : fA;
}

const double Ellipse_Eq::B() const
{
	return sqrt(B_Sq());
}

const double Ellipse_Eq::B_Sq() const
{
	double tempf = C();
	return SQUARE(m_fSemiMajorAxis)-SQUARE(tempf);
}

const double Ellipse_Eq::R(double theta) const
{
	return sqrt(R_Sq(theta));
}

const double Ellipse_Eq::R_Sq(double theta) const
{
	double a2 = SQUARE(m_fSemiMajorAxis);
	double b2 = B_Sq();
	double fcos = cos(theta);
	double fsin = sin(theta);

	return a2*b2/(b2*SQUARE(fcos)+a2*SQUARE(fsin));
}

CPoint2 Ellipse_Eq::Normal(double x, double y)
{	
	double theta1, theta2;

	theta1 = atan2(y-m_Foci1.Y, x-m_Foci1.X);
	theta2 = atan2(y-m_Foci2.Y, x-m_Foci2.X);
	
	CPoint2 p2;
	p2.X = cos( 0.5*(theta1+theta2) );
	p2.Y = sin( 0.5*(theta1+theta2) );

	return p2;
}

CPoint2 Ellipse_Eq::Tangent(double x, double y)
{
	CPoint2 p2 = Normal(x,y);

	double tempf = p2.X;
	p2.X = -p2.Y;
	p2.Y = tempf;

	return p2;
}

const double Ellipse_Eq::Perimeter() const
{
	double a = m_fSemiMajorAxis;
	double b = B();
//	double h = SQARE( (a-b)/(a+b) );

	double tempf = MY_PI * sqrt(2*(a*a+b*b));
//  double tempf = MY_PI * (3*(a+b) - sqrt((a+3*b)*(3*a+b)));
//	double tempf = MY_PI * (a+b)(1 + 3*h/(10+sqrt(4-3*h)));

	return tempf;
}

const double Ellipse_Eq::Area() const
{
	double tempf = MY_PI*A()*B();
	return tempf;
}

bool Ellipse_Eq::PtOnEllipse(double x, double y, double tolerance)
{
	double tempf1 = sqrt( SQUARE(m_Foci1.X-x) + SQUARE(m_Foci1.Y-y) );
	double tempf2 = sqrt( SQUARE(m_Foci2.X-x) + SQUARE(m_Foci2.Y-y) );

	return fabs(tempf1+tempf2 - 2.*m_fSemiMajorAxis) <= tolerance;
}

void Ellipse_Eq::Translate(double tx, double ty)
{
	m_Foci1.Translate(tx, ty);
	m_Foci2.Translate(tx, ty);
}

void Ellipse_Eq::Scale(double sx, double sy)
{
	std::vector<CPoint2> vPoint2;
	SampleEllipse(vPoint2, m_Foci1, m_Foci2, m_fSemiMajorAxis, 5);
	for (int i=0; i<vPoint2.size(); i++)
		vPoint2[i].Scale(sx, sy);
	Fit_Ellipse(vPoint2, *this);
}

void Ellipse_Eq::Rotate(double theta, double cx, double cy)
{
	m_Foci1.Rotate(theta, cx, cy);
	m_Foci2.Rotate(theta, cx, cy);
}

COutputDataFile& operator<<(COutputDataFile& ofs, const Ellipse_Eq& ellipse)
{
	return ofs << ellipse.F1() << ellipse.F2() << ellipse.A() ; 
}

CInputDataFile& operator>>(CInputDataFile& ifs, Ellipse_Eq& ellipse)
{
	CPoint2 f1, f2;
	double a;

	ifs >> f1 >> f2 >> a;

	ellipse.SetF(f1.X, f1.Y, f2.X, f2.Y);
	ellipse.SetA(a);

	return ifs; 
}

void Circle2Ellipse(Circle_Eq &circle, Ellipse_Eq &ellipse)
{
	ellipse.SetF(circle.Center().X, circle.Center().Y, circle.Center().X, circle.Center().Y);
	ellipse.SetA(circle.Radius());
}

bool Fit_Ellipse(std::vector<CPoint2> &vPoint2, CPoint2& f1, CPoint2& f2, double& fa, double theta_tolerance )
{
	int nSample = vPoint2.size();
	if ( nSample<5 ) return false;

	double theta, long_axis, short_axis, x0, y0;

	int i;
	double mean_x=0., mean_y=0.;
	double a, b, c, d, e, F;
	CVisDVector x(nSample), y(nSample);
	CVisDMatrix X_data(nSample, 5), A_param(1, 5);

	// calculate the mean of sample points
	// unbias the samples (to be added)
	for (i=0; i<nSample; i++)
	{	
		x[i] = vPoint2[i].X;
		y[i] = vPoint2[i].Y;
		mean_x += x[i];
		mean_y += y[i];
	}
	mean_x /= double(nSample);
	mean_y /= double(nSample);
	for (i=0; i<nSample; i++)
	{
		x[i] -= mean_x;
		y[i] -= mean_y;
	}

	// compute conic equations parameters by Least Square Estimation
	A_param = 0.;
	for (i=0; i<nSample; i++)
	{
		X_data[i][0] = x[i]*x[i];
		X_data[i][1] = x[i]*y[i];
		X_data[i][2] = y[i]*y[i];
		X_data[i][3] = x[i];
		X_data[i][4] = y[i];

		A_param[0][0] += X_data[i][0]; 
		A_param[0][1] += X_data[i][1]; 
		A_param[0][2] += X_data[i][2]; 
		A_param[0][3] += X_data[i][3]; 
		A_param[0][4] += X_data[i][4]; 
	}
	CVisDMatrix mat_temp(5, 5);
	mat_temp = X_data.Transposed() * X_data;
//	SaveToFile("test_matrix.txt", mat_temp);
	
	if (mat_temp.Determinant() == 0.) return false;

	A_param = A_param * mat_temp.Inverted();
	
	// assign the conic equation parameters
	a = A_param[0][0];
	b = A_param[0][1];
	c = A_param[0][2]; 
	d = A_param[0][3];
	e = A_param[0][4];

	// remove the orientation from the ellipse
	double cos_theta, sin_theta;
	if ( min(fabs(b/a), fabs(b/c)) > theta_tolerance )
	{
		theta = 0.5 * atan2(b, c-a);

		cos_theta = cos(theta);
		sin_theta = sin(theta);

		double a_old=a, b_old=b, c_old=c, d_old=d, e_old=e;

		a = a_old*cos_theta*cos_theta - b_old*cos_theta*sin_theta + c_old*sin_theta*sin_theta;
		b = 0;
		c = a_old*sin_theta*sin_theta + b_old*cos_theta*sin_theta + c_old*cos_theta*cos_theta;
		d = d_old*cos_theta - e_old*sin_theta;
		e = d_old*sin_theta + e_old*cos_theta;

		theta = MY_PI - theta;
		double mean_x_old=mean_x, mean_y_old=mean_y;
		mean_x = cos_theta*mean_x_old - sin_theta*mean_y_old;
		mean_y = sin_theta*mean_x_old + cos_theta*mean_y_old;
	}
	else if ( fabs(a) <= fabs(c) )
	{
		theta = 0.;
		cos_theta = 1.;
		sin_theta = 0.;
	}
	else if ( fabs(a) > fabs(c) )
	{
		theta = 0.5*MY_PI;
		cos_theta = 1.;
		sin_theta = 0.;
	}

	// test if ellipse found
	if ( a*c <= 0 ) 
	{
		return false;
	}

	// make sure the parameters are positive
	if ( a < 0 )
	{
		a = -a;
		c = -c;
		d = -d;
		e = -e;
	}

	// final parameters
	double x0_old = mean_x - d/2./a; 
	double y0_old = mean_y - e/2./c;
	x0 = cos_theta*x0_old + sin_theta*y0_old;
	y0 = -sin_theta*x0_old + cos_theta*y0_old;
    F  = 1. + (d*d)/(4.*a) + (e*e)/(4.*c);
    a = sqrt( F/a );
	b = sqrt( F/c );    
	long_axis = 2.*max(a, b);
	short_axis = 2.*min(a, b);

//	std::cout << "theta: " << theta << "; cos: " << cos(theta) << "; sin: " << sin(theta) << std::endl;
//	std::cout << "long_axis: " << long_axis << "; short_axis: " << short_axis
//		<< "; x0: " << x0 << "; y0: " << y0 << std::endl;

	f1.X = -0.5*sqrt(long_axis*long_axis - short_axis*short_axis);
	f2.X = -f1.X; 
	f1.Y = f2.Y = 0.;

	f1.Rotate(theta);
	f2.Rotate(theta);
	f1.Translate(x0, y0);
	f2.Translate(x0, y0);
	
	fa = 0.5 * long_axis;

	return true;
}

bool Fit_Ellipse(std::vector<CPoint2> &vPoint2, Ellipse_Eq &ellipse, double theta_tolerance /* =0.001 */)
{
	CPoint2 f1, f2;
	double fa;

	bool fRet = Fit_Ellipse(vPoint2, f1, f2, fa, theta_tolerance);

	ellipse.SetA(fa);
	ellipse.SetF(f1.X, f1.Y, f2.X, f2.Y);

	return fRet;
}

//////////////// functions ////////////////
// sample points on line segments, including the end points
void SampleLineSeg(std::vector<CPoint2>& vPoint2, CPoint2 p0, CPoint2 p1, int nPts /* =2 */)
{
	vPoint2.clear();
	if (nPts<=0) return;

	vPoint2.push_back(p0);
	if (nPts==1) return;

	if (nPts==2)
	{
		vPoint2.push_back(p1);
		return;
	}
	
	double fDist = p0.DistantTo(p1);
	if (fDist <= NEARLY_ZERO) return;
	
	double fStep = fDist/nPts;
	double tempf;
	CPoint2 p;
	int iSign;
	if (p0.X == p1.X)
	{
		p.X = p0.X;
		iSign = SIGN(p1.Y-p0.Y);
		tempf = p0.Y + iSign*fStep;
		while (fabs(tempf-p0.Y)<fDist)
		{
			p.Y = tempf;
			vPoint2.push_back(p);
			tempf += iSign*fStep;
		}
	}
	else if (p0.Y == p1.Y)
	{
		p.Y = p0.Y;
		iSign = SIGN(p1.X-p0.X);
		tempf = p0.X + iSign*fStep;
		while (fabs(tempf-p0.X)<fDist)
		{
			p.X = tempf;
			vPoint2.push_back(p);
			tempf += iSign*fStep;
		}
	}
	else 
	{
		double theta = atan((p1.Y-p0.Y)/(p1.X-p0.X));
		iSign = SIGN(p1.X-p0.X);
		tempf = fStep;
		
		while (tempf<fDist)
		{
			p.X = p0.X + iSign*tempf*cos(theta);
			p.Y = p0.Y + iSign*tempf*sin(theta);
			vPoint2.push_back(p);
			tempf += fStep;
		}
	}

	vPoint2.push_back(p1);
}

// sample points on a circle
void SampleCircle(std::vector<CPoint2>& vPoint2, double cx, double cy, double r, int nTheta/* =8 */)
{
	vPoint2.clear();

	double fThetaStep = 2*MY_PI/nTheta;

	for (int iTheta=0; iTheta<nTheta; iTheta++)
	{
		CPoint2 p2;
		p2.X = cx + r*cos(iTheta*fThetaStep);
		p2.Y = cy + r*sin(iTheta*fThetaStep);
		
		vPoint2.push_back(p2);
	}
}

// sample points on a ellipse
void SampleEllipse(std::vector<CPoint2>& vPoint2, CPoint2 f1, CPoint2 f2, double fa, int nTheta/* =8 */)
{
	vPoint2.clear();
	
	double fThetaStep = 2*MY_PI/nTheta;

	Ellipse_Eq ellipse(fa, f1.X, f1.Y, f2.X, f2.Y);

	LineSeg_Eq lineSeg(f1, f2);
	CPoint2 midPt = lineSeg.MiddlePoint();
	double angle = lineSeg.Angle();

	for (int iTheta=0; iTheta<nTheta; iTheta++)
	{
		double r = ellipse.R(iTheta*fThetaStep);
	
		CPoint2 p2;
		p2.X = r * cos(iTheta*fThetaStep);
		p2.Y = r * sin(iTheta*fThetaStep);

		p2.Rotate(angle);
		p2.Translate(midPt.X, midPt.Y);

		vPoint2.push_back(p2);
	}
}

// sample points on a ring, 
// when minR=0 it's to sample on a round;
// when minR=maxR it's to sample along a circle
void SampleRing(std::vector<CPoint2>& vPoint2, double cx, double cy, int minR, int maxR)
{
	assert(minR<maxR);
	assert(minR>=0);
	assert(maxR>=0);

	vPoint2.clear();
	if ( minR==0 )
	{
		CPoint2 p2;
		p2.X = cx;
		p2.Y = cy;
		vPoint2.push_back(p2);
	}

	for (int i=minR; i<maxR; i++)
	{
		int nTheta = 2*MY_PI*i; 
		std::vector<CPoint2> vTempPoint2;
		SampleCircle(vTempPoint2, cx, cy, i, nTheta);
		
		for (int j=0; j<vTempPoint2.size(); j++) 
			vPoint2.push_back(vTempPoint2[j]);
	}
}

// sample points on a round
void SampleRound(std::vector<CPoint2>& vPoint2, double cx, double cy, int r)
{
	assert (r>=1);

	vPoint2.clear();
	SampleRing(vPoint2, cx, cy, 0, r);
}

bool GetLineEndPointsWithImageFrame(int width, int height, double r, double theta, CPoint2& p0, CPoint2& p1)
{
	p0.X = 0.; p0.Y = 0.;
	p1.X = 0.; p1.Y = 0.;

	if ( width<=0 || height<=0 || r<1) return false;

	if ( theta==0 || theta==2*MY_PI )
	{
		if ( r>=width-1 ) return false;
		p0.X = r; 
		p0.Y = 0;
		p1.X = r; 
		p1.Y = height-1;
		return true;
	}
	else if ( theta==0.5*MY_PI || theta==1.5*MY_PI )
	{
		if ( r>=height-1 ) return false;
		p0.X = 0; 
		p0.Y = r;
		p1.X = width-1; 
		p1.Y = r;
		return true;
	}

	std::vector<CPoint2> vPoint2;
	vPoint2.clear();
	double x, y;
	// x = 0
	y = r/sin(theta);
	if (y>=1 && y<height-1)
	{
		CPoint2 p;
		p.X = 0;
		p.Y = y;
		vPoint2.push_back(p);
		if (vPoint2.size()==2) goto succeed;
	}
	// x = width-1
	y = (r-(width-1)*cos(theta) )/sin(theta);
	if (y>=1 && y<height-1)
	{
		CPoint2 p;
		p.X = width-1;
		p.Y = y;
		vPoint2.push_back(p);
		if (vPoint2.size()==2) goto succeed;
	}
	// y = 0
	x = r/cos(theta);
	if (x>=1 && x<width-1)
	{
		CPoint2 p;
		p.X = x;
		p.Y = 0;
		vPoint2.push_back(p);
		if (vPoint2.size()==2) goto succeed;
	}
	// y = height-1
	x = (r-(height-1)*sin(theta) )/cos(theta);
	if (x>=1 && x<width-1)
	{
		CPoint2 p;
		p.X = x;
		p.Y = height-1;
		vPoint2.push_back(p);
		if (vPoint2.size()==2) goto succeed;
	}

	return false;

succeed:
	p0 = vPoint2[0];
	p1 = vPoint2[1];
	return true;

}

void Shape2vLineSeg(CShape& shape, LineSeg_Eq_Set& vSeg)
{
	vSeg.clear();

	for (int i=0; i<shape.PointNum()-1; i++)
	{
		LineSeg_Eq seg(shape.GetPoint(i), shape.GetPoint(i+1));
		vSeg.push_back(seg);
	}
}

void ShapeSet2vLineSeg(CShapeSet& ss, LineSeg_Eq_Set& vSeg)
{
	vSeg.clear();

	for (int i=0; i<ss.size(); i++)
	{
		for (int j=0; j<ss[i].PointNum()-1; j++)
		{
//			if (ss[i].GetPoint(j).DistantTo(ss[i].GetPoint(j+1)) > 3 )
			{
				LineSeg_Eq seg(ss[i].GetPoint(j), ss[i].GetPoint(j+1));
				vSeg.push_back(seg);
			}
		}
	}
}

void Combine2vLineSeg(LineSeg_Eq_Set& vLineSeg1, LineSeg_Eq_Set& vLineSeg2)
{
	for (int i=0; i<vLineSeg2.size(); i++)
	{
		vLineSeg1.push_back(vLineSeg2[i]);
	}
}

bool ReadHanfengRects(const char* strFileName, Rectangle_Eq_Set &vRect)
{
	FILE* fp;

	vRect.clear();
	if ( (fp=fopen(strFileName, "r")) != NULL )
	{
		int nSize, vIndex[4];
		fscanf(fp, "%d \n", &nSize);
		if (nSize <= 0) 
		{
			fclose(fp);
			return false;
		}

		vRect.resize(nSize);
		for (int i=0; i<nSize; i++)
		{
			fscanf(fp, "%d %d %d %d\n", &vIndex[0], &vIndex[1], &vIndex[2], &vIndex[3]);

			for (int j=0; j<4; j++)
			{
				double x, y;
				fscanf(fp, "%lf %lf ", &x, &y);
				vRect[i].SetCorner(j, x, y);
			}
			fscanf(fp, "\n");
		}

		fclose(fp);
		return true;
	}

	return false;
}

void RankBySize(Circle_Eq_Set &vCircle)
{
	if ( vCircle.size()<=1 ) return;

	for (int i=0; i<vCircle.size()-1; i++)
	{
		for (int j=i+1; j<vCircle.size(); j++)
		{
			if ( vCircle[i].Radius() < vCircle[j].Radius() )
			{
				Circle_Eq circle_temp = vCircle[i];
				vCircle[i] = vCircle[j];
				vCircle[j] = circle_temp;
			}
		}
	}
}

void RankBySize(Ellipse_Eq_Set &vEllipse)
{
	if ( vEllipse.size()<=1 ) return;

	for (int i=0; i<vEllipse.size()-1; i++)
	{
		for (int j=i+1; j<vEllipse.size(); j++)
		{
			if ( vEllipse[i].A() < vEllipse[j].A() )
			{
				Ellipse_Eq ellipse_temp = vEllipse[i];
				vEllipse[i] = vEllipse[j];
				vEllipse[j] = ellipse_temp;
			}
		}
	}
}

void RankBySize(Rectangle_Eq_Set &vRect)
{
	if ( vRect.size()<=1 ) return;

	for (int i=0; i<vRect.size()-1; i++)
	{
		for (int j=i+1; j<vRect.size(); j++)
		{
			if ( vRect[i].Width()*vRect[i].Height() < vRect[j].Width()*vRect[j].Height() )
			{
				Rectangle_Eq rect_temp = vRect[i];
				vRect[i] = vRect[j];
				vRect[j] = rect_temp;
			}
		}
	}
}

COutputDataFile& operator<<(COutputDataFile& ofs, const Circle_Eq_Set& vGeom)
{
	ofs << int(vGeom.size()) << sep_endl;
	for (int i=0; i<vGeom.size(); i++)
	{
		ofs << vGeom[i] << sep_endl;
	}

	return ofs;
}

CInputDataFile& operator>>(CInputDataFile& ifs, Circle_Eq_Set& vGeom)
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

COutputDataFile& operator<<(COutputDataFile& ofs, const Ellipse_Eq_Set& vGeom)
{
	ofs << int(vGeom.size()) << sep_endl;
	for (int i=0; i<vGeom.size(); i++)
	{
		ofs << vGeom[i] << sep_endl;
	}

	return ofs;
}

CInputDataFile& operator>>(CInputDataFile& ifs, Ellipse_Eq_Set& vGeom)
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

COutputDataFile& operator<<(COutputDataFile& ofs, const LineSeg_Eq_Set& vGeom)
{
	ofs << int(vGeom.size()) << sep_endl;
	for (int i=0; i<vGeom.size(); i++)
	{
		ofs << vGeom[i] << sep_endl;
	}

	return ofs;
}

CInputDataFile& operator>>(CInputDataFile& ifs, LineSeg_Eq_Set& vGeom)
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

COutputDataFile& operator<<(COutputDataFile& ofs, const Rectangle_Eq_Set& vGeom)
{
	ofs << int(vGeom.size()) << sep_endl;
	for (int i=0; i<vGeom.size(); i++)
	{
		ofs << vGeom[i] << sep_endl;
	}

	return ofs;
}

CInputDataFile& operator>>(CInputDataFile& ifs, Rectangle_Eq_Set& vGeom)
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

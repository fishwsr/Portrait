
// CubicBezierSpline.h: interface for the CCubicBezierSpline class.
//
///////////////////////////////////////////////////////////////////////////

#ifndef __CSBASE_CUBICBEZIERSPLINE_H__
#define __CSBASE_CUBICBEZIERSPLINE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Point2.h"

//
//                       [-1  3  -3 1][P0] 
//                       [ 3 -6   3 0][P1]
//   P(u) = [u^3 u^2 u 1][-3  3   0 0][P2] 
//                       [ 1  0   0 0][P3]
//
//                        [-1  3  -3 1][P0] 
//                        [ 3 -6   3 0][P1]
//   DP(u) = [3u^2 2u 1 0][-3  3   0 0][P2]
//                        [ 1  0   0 0][P3]
//
//                         [-1  3  -3 1][P0] 
//                         [ 3 -6   3 0][P1]
//   DDP(u) = [6u  2  0  0][-3  3   0 0][P2]
//                         [ 1  0   0 0][P3]
//                         _______________
//                          precalc points

class CSBASE_API CCubicBezier
{
public:
    CCubicBezier(void) { }
    virtual ~CCubicBezier(void) { }

public:
	void Sample(int segnum, std::vector<double> &vu, std::vector<CPoint2> &vp) const;
	void Sample(std::vector<double> &vu, std::vector<CPoint2> &vp) const;

	void Fitting(std::vector<double> &vu, std::vector<CPoint2> &vp);
	void Fitting2(std::vector<double> &vu, std::vector<CPoint2> &vp);

	double Length(double interval = 0.2) const;
	double DistantToPoint(const CPoint2 point) const;
	double DistantYToPoint(const CPoint2 point) const;

	const CCubicBezier& operator=(const CCubicBezier &src);
	CCubicBezier &operator/=(double factor);

	void SetControlPoint(int n, const CPoint2 &point, bool precalc = true);
	CPoint2 P(double u) const;
	CVector2 DP(double u) const;
	CVector2 DDP(double u) const;
	double R(double u) const;
	CVector2 Normal(double u) const;
	double Tangent(double u) const; // added by zijian
	const CPoint2 &ControlPoint(int i) const { return m_ControlPoints[i]; }

	void Precalc(void);
//protected:
	CPoint2 m_PreCalcPoints[4];
	CPoint2 m_ControlPoints[4];	
};

class CSBASE_API CCubicBezierSpline
{
public:
    CCubicBezierSpline(void) { }
    virtual ~CCubicBezierSpline(void) { }

public:
	const CCubicBezierSpline &operator=(const CCubicBezierSpline &src);
	CCubicBezierSpline &operator/=(double factor);

public:
    void Clear(void);

    void Create_Catmull_Rom_Spline(const std::vector<CPoint2> &controlPoints, bool bOpen = true);
	void Create_Stroke_Simple_Spline(const std::vector<CPoint2> &controlPoints, bool bOpen = true);
	void Resample(std::vector<CPoint2> &points, int iSegments = 100);
	void Resample(int iSegments = 100);

	void GetPartCtrPts(int iStartSegment, int iEndSegment, std::vector<CPoint2> &CtrPts) const;
	void GetKeyPoints(std::vector<CPoint2> &keypoints) const;

    double DistantToPoint(const CPoint2 point, int * pIndex = NULL) const;
	double DistantToPoint2(const CPoint2 point, CPoint2 &NearPt, bool &bVertical) const;
	double DistantToPoint3(const CPoint2 point, int &iSegNum) const;
	double DistantYToPoint(const CPoint2 point, int &iSegNum) const;
	double DistantToPoint4(const CPoint2 point, CPoint2 &NearPt, int &iSegNum, double &dMin_u, float &tangent_angle) const;
	double Length(double interval = 0.2) const;
	double Length(int iStart, int iEnd) const;

	double DistantToCurve(const CCubicBezierSpline &Curve) const;
	int NbSegment(void) const { if (m_Segments.empty()) return 0; return (int)m_Segments.size(); }

//protected: 
	std::vector<CCubicBezier> m_Segments;
};

CSBASE_API COutputDataFile& operator<<(COutputDataFile& ofs, const CCubicBezier& cb);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, CCubicBezier& cb);

CSBASE_API COutputDataFile& operator<<(COutputDataFile& ofs, const CCubicBezierSpline& cbs);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, CCubicBezierSpline& cbs);

#endif // __CSBASE_CUBICBEZIERSPLINE_H__

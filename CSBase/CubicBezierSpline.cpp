// CubicBezierSpline.cpp: implementation of the CCubicBezierSpline class.
//
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CubicBezierSpline.h"

void CCubicBezier::Sample(int segnum, std::vector<double> &vu, std::vector<CPoint2> &vp) const
{
	double delta = 1.0 / segnum;
	vu.resize(segnum + 1);
	vp.resize(segnum + 1);
	double u = 0.0;

	for (int i = 0; i < segnum; i++)
	{
		vu[i] = u;
		vp[i] = P(u);
		u += delta;
	}
	vu[segnum] = 1.0;
	vp[segnum] = P(1.0);
}

void CCubicBezier::Sample(std::vector<double> &vu, std::vector<CPoint2> &vp) const
{
	vp.resize(vu.size());
	for (int i = 0; i < vu.size(); i++)
	{
		vp[i] = P(vu[i]);
	}
}

void CCubicBezier::Fitting(std::vector<double> &vu, std::vector<CPoint2> &vp)
{
	CVisDMatrix A; 
	CVisDMatrix B; // vp 
	CVisDMatrix X; // control points

	int samplenum = vu.size();
	assert(samplenum == vp.size());

	A.Resize(samplenum,4);
	B.Resize(samplenum,2);
	X.Resize(4, 2);

	for (int i = 0; i < samplenum; i++)
	{
		B[i][0] = vp[i].X;
		B[i][1] = vp[i].Y;

		double u,u2,u3;
		u = vu[i];
		u2 = u * u;
		u3 = u2 * u;

//                       [-1  3  -3 1][P0] 
//                       [ 3 -6   3 0][P1]
//   P(u) = [u^3 u^2 u 1][-3  3   0 0][P2] 
//                       [ 1  0   0 0][P3]
//

		A[i][0] = -u3 + 3 * u2 - 3 * u + 1;
		A[i][1] = 3 * u3 - 6 * u2 + 3 * u;
		A[i][2] = -3 * u3 + 3 * u2;
		A[i][3] = u3; 
	}
	VisDMatrixLeastSquares(A, B, X);

	for (i = 0; i < 4; i++)
	{
		m_ControlPoints[i].X = (float) X[i][0];
		m_ControlPoints[i].Y = (float) X[i][1];
	}	
	Precalc();
}

void CCubicBezier::Fitting2(std::vector<double> &vu, std::vector<CPoint2> &vp)
{
	CVisDMatrix A0, A; 
	CVisDMatrix B; // vp 
	CVisDMatrix X0, X; // control points

	int samplenum = vu.size();
	assert(samplenum == vp.size());

	A.Resize(samplenum, 2);
	A0.Resize(samplenum, 2);

	B.Resize(samplenum, 2);
	X.Resize(2, 2);
	X0.Resize(2, 2);

	for (int i = 0; i < samplenum; i++)
	{
		B[i][0] = vp[i].X;
		B[i][1] = vp[i].Y;

		double u, u2, u3;
		u = vu[i];
		u2 = u * u;
		u3 = u2 * u;

//                       [-1  3  -3 1][P0] 
//                       [ 3 -6   3 0][P1]
//   P(u) = [u^3 u^2 u 1][-3  3   0 0][P2] 
//                       [ 1  0   0 0][P3]
//

		A0[i][0] = -u3 + 3 * u2 - 3 * u + 1;
		A[i][0] = 3 * u3 - 6 * u2 + 3 * u;
		A[i][1] = -3 * u3 + 3 * u2;
		A0[i][1] = u3; 
	}
	X0[0][0] = m_ControlPoints[0].X;
	X0[0][1] = m_ControlPoints[0].Y;
	X0[1][0] = m_ControlPoints[3].X;
	X0[1][1] = m_ControlPoints[3].Y;
	B = B - A0 * X0;

	VisDMatrixLeastSquares(A, B, X);

	for (i = 0; i < 2; i++)
	{
		m_ControlPoints[i + 1].X = (float) X[i][0];
		m_ControlPoints[i + 1].Y = (float) X[i][1];
	}	
	Precalc();
}

CPoint2 CCubicBezier::P(double u) const
{
	CPoint2 p;
	double u2, u3;
	u2 = u * u;
	u3 = u2 * u;
	p = m_PreCalcPoints[0] * u3 + m_PreCalcPoints[1] * u2 + m_PreCalcPoints[2] * u + m_PreCalcPoints[3];
	return p;
}

CVector2 CCubicBezier::DP(double u) const
{
	CVector2 dp;
	double u2;
	u2 = u * u;
	dp = m_PreCalcPoints[0] * 3 * u2 + m_PreCalcPoints[1] * 2 * u + m_PreCalcPoints[2];
	return  dp;
}

CVector2 CCubicBezier::DDP(double u) const
{
	CVector2 ddp;
	ddp = m_PreCalcPoints[0] * 6 * u + m_PreCalcPoints[1] * 2;
	return ddp;
}

double CCubicBezier::R(double u) const
{
	CVector2 dp,ddp;
	dp = DP(u);
	ddp = DDP(u);

	double tempf, tempf2;
	tempf = sqrt(dp.X * dp.X + dp.Y * dp.Y);
	tempf2 = tempf * tempf * tempf/(dp.X * ddp.X - dp.Y * ddp.Y);
	
	return fabs(tempf2);
}

CVector2 CCubicBezier::Normal(double u) const
{
	CVector2 dp, nu;
	dp = DP(u);
	nu.X = -dp.Y;
	nu.Y = dp.X;
	
	double s = nu.X * nu.X + nu.Y * nu.Y;
	if (s < DBL_MIN)
		s = DBL_MIN;
	nu = nu / sqrt(s);

	return nu;
}

double CCubicBezier::Tangent(double u) const 
{
	CVector2 normal = Normal(u);

	double theta = atan2(normal.Y, normal.X) + MY_PI/2.;
	if(theta < 0.) 
		theta += 2.*MY_PI;
	else if(theta > 2.*MY_PI)
		theta -= 2.*MY_PI;

	return theta;
}

void CCubicBezier::SetControlPoint(int n, const CPoint2 &point, bool precalc)
{
	m_ControlPoints[n] = point;
	if (precalc) Precalc();
}

void CCubicBezier::Precalc(void)
{
	m_PreCalcPoints[0] = m_ControlPoints[1] * 3 - m_ControlPoints[0] - m_ControlPoints[2] * 3 + m_ControlPoints[3];
	m_PreCalcPoints[1] = m_ControlPoints[0] * 3 - m_ControlPoints[1] * 6 + m_ControlPoints[2] * 3;
	m_PreCalcPoints[2] = m_ControlPoints[1] * 3 - m_ControlPoints[0] * 3;
	m_PreCalcPoints[3] = m_ControlPoints[0];
}

const CCubicBezier& CCubicBezier::operator=(const CCubicBezier& src)
{
	if ( this!=&src )
	{
		for(int i = 0; i < 4; i++)
		{
			m_ControlPoints[i] = src.m_ControlPoints[i];
		}
		Precalc();
	}
	return *this;
}

const CCubicBezierSpline& CCubicBezierSpline::operator=(const CCubicBezierSpline &src)
{
	if ( this!=&src )
	{
		m_Segments.clear();
		m_Segments.resize(src.NbSegment());
		for (int i = 0; i < m_Segments.size(); i++)
		{
			m_Segments[i] = src.m_Segments[i];
		}
	}
	return *this;
}

CCubicBezier &CCubicBezier::operator/=(double factor)
{
    //assert(0);
	if (factor > 0)
	for(int i = 0; i < 4; i ++)
	{
		m_ControlPoints[i] = m_ControlPoints[i] / factor;
		m_PreCalcPoints[i] = m_PreCalcPoints[i] / factor;
	}
    return *this;
}

CCubicBezierSpline &CCubicBezierSpline::operator/=(double factor)
{
    //assert(0);
	if (factor > 0)
	for(int i = 0; i < m_Segments.size(); i ++)
	{
		m_Segments[i] /= factor;
	}
    return *this;
}


void CCubicBezierSpline::Clear()
{
    m_Segments.clear();
}

void CCubicBezierSpline::Create_Stroke_Simple_Spline(const std::vector<CPoint2> &controlPoints, bool bOpen /* = true */)
{
	int	iPointNum = controlPoints.size();
	assert(iPointNum >= 4);
	
	int iSegmentNum = (iPointNum-1)/3;

	if (iSegmentNum <= 0) 
	{	
		return;
	}

	m_Segments.resize(iSegmentNum);
	int iP = 0;
	for (int iS=0; iS<iSegmentNum; iS++)
	{
		m_Segments[iS].SetControlPoint(0, controlPoints[iP]);
		m_Segments[iS].SetControlPoint(1, controlPoints[iP+1]);
		m_Segments[iS].SetControlPoint(2, controlPoints[iP+2]);
		m_Segments[iS].SetControlPoint(3, controlPoints[iP+3]);

		iP += 3;
	}
	
}

void CCubicBezierSpline::Create_Catmull_Rom_Spline(const std::vector<CPoint2> &controlPoints, bool bOpen)
{
	int iPointNum = controlPoints.size();
	int iSegmentNum = iPointNum - 1;
	if (!bOpen)
		iSegmentNum = iPointNum;
	
	CPoint2 tempP;

	m_Segments.clear();

	if (iSegmentNum <= 0)
    {
        return;
    }

	m_Segments.resize(iSegmentNum);
	if (iSegmentNum == 1)
	{
		m_Segments[0].SetControlPoint(0, controlPoints[0]);
		tempP = controlPoints[0] + ((CPoint2)(controlPoints[1] - controlPoints[0])) / 3;
		m_Segments[0].SetControlPoint(1, tempP);
		tempP = controlPoints[0] + ((CPoint2)(controlPoints[1] - controlPoints[0])) * 2 / 3;
		m_Segments[0].SetControlPoint(2, tempP);
		m_Segments[0].SetControlPoint(3, controlPoints[1]);
		return;
	}

	for (int iP = 1; iP < iPointNum - 1 ; iP++)
	{
		m_Segments[iP - 1].SetControlPoint(3, controlPoints[iP]);
		tempP = controlPoints[iP] + ((CPoint2)(controlPoints[iP - 1] - controlPoints[iP + 1])) / 6;
		m_Segments[iP - 1].SetControlPoint(2, tempP);
		
		m_Segments[iP].SetControlPoint(0, controlPoints[iP]);
		tempP = controlPoints[iP] - ((CPoint2)(controlPoints[iP - 1] - controlPoints[iP + 1])) / 6;
		m_Segments[iP].SetControlPoint(1, tempP);
	}

	m_Segments[0].SetControlPoint(0, controlPoints[0]);
	m_Segments[iPointNum-2].SetControlPoint(3, controlPoints[iPointNum - 1]);

	if (bOpen)
	{
		tempP = controlPoints[0] + ((CPoint2)(controlPoints[1] - controlPoints[0])) / 3;
		m_Segments[0].SetControlPoint(1, tempP);
		tempP = controlPoints[iPointNum - 1] + ((CPoint2)(controlPoints[iPointNum - 2] - controlPoints[iPointNum - 1])) / 3;
		m_Segments[iPointNum - 2].SetControlPoint(2, tempP);
	}
	else
	{
		tempP = controlPoints[0] - ((CPoint2)(controlPoints[iPointNum - 1] - controlPoints[1])) / 6;
		m_Segments[0].SetControlPoint(1, tempP);

		tempP = controlPoints[iPointNum - 1] + ((CPoint2)(controlPoints[iPointNum - 2] - controlPoints[0])) / 6;
		m_Segments[iPointNum - 2].SetControlPoint(2, tempP);

		m_Segments[iPointNum - 1].SetControlPoint(0, controlPoints[iPointNum - 1]);
		m_Segments[iPointNum - 1].SetControlPoint(3, controlPoints[0]);
		tempP = controlPoints[iPointNum - 1] - ((CPoint2)(controlPoints[iPointNum - 2] - controlPoints[0])) / 6;
		m_Segments[iPointNum - 1].SetControlPoint(1, tempP);
		tempP = controlPoints[0] + ((CPoint2)(controlPoints[iPointNum - 1] - controlPoints[1])) / 6;
		m_Segments[iPointNum - 1].SetControlPoint(2, tempP);
	}
}

void CCubicBezierSpline::Resample(std::vector<CPoint2> &points, int iSegments)
{
	if (this->NbSegment() == 0) return;

	double dLength = this->Length(0.1);
	double dStep = dLength / (double)iSegments;		

	std::vector<CPoint2> &pNewContrPoints = points;
	pNewContrPoints.clear();
	pNewContrPoints.reserve(iSegments+1);

	int count = 0;
	CPoint2 oldP = this->m_Segments[0].P(0);
	CPoint2 curP;
	double dLen = 0;
	for (int i = 0; i < this->NbSegment(); i++)
	{
		int num = (int) (this->m_Segments[i].Length(0.1) / (dStep * 0.1));
		double delta = 1.0 / num;
		double u = 0.0;
		for (int j = 0; j <= num; j++, u += delta)
		{
			curP = this->m_Segments[i].P(u);
			dLen = dLen + curP.DistantTo(oldP);			

			if(dLen >= count * dStep)
			{
				pNewContrPoints.push_back(curP);
				count++;
			}		

			if(count >= iSegments + 1)
			{
				pNewContrPoints[count - 1] = this->m_Segments[this->NbSegment() - 1].P(1);
				break;
			}				

			oldP = this->m_Segments[i].P(u);
		}	

		if(count > iSegments + 1)
			break;
	}

	if(pNewContrPoints.size() < iSegments + 1)
		pNewContrPoints.push_back(this->m_Segments[this->NbSegment() - 1].P(1));
	
	int iTotalLen = pNewContrPoints.size();
//	assert(iTotalLen == (iSegments + 1));
}

void CCubicBezierSpline::Resample(int iSegments)
{
	if (this->NbSegment() == 0) return;

	double dLength=this->Length(0.1);
	double dStep = dLength/(double)iSegments;		

	std::vector<CPoint2> pNewContrPoints;	
	pNewContrPoints.reserve(iSegments+1);

	int count = 0;
	CPoint2 oldP = this->m_Segments[0].P(0);
	CPoint2 curP;
	double dLen = 0;
	for (int i = 0; i < this->NbSegment(); i++)
	{
		int num = (int) (this->m_Segments[i].Length(0.1) / (dStep * 0.1));
		double delta = 1.0 / num;
		double u = 0.0;
		for (int j =0 ; j <= num; j++, u += delta)
		{
			curP = this->m_Segments[i].P(u);
			dLen = dLen + curP.DistantTo(oldP);			

			if(dLen >= count * dStep)
			{
				pNewContrPoints.push_back(curP);
				count++;
			}		

			if(count >= iSegments + 1)
			{
				pNewContrPoints[count - 1] = this->m_Segments[this->NbSegment() - 1].P(1);
				break;
			}				

			oldP = this->m_Segments[i].P(u);
		}	

		if(count > iSegments + 1)
			break;
	}

	if(pNewContrPoints.size() < iSegments + 1)
	{
		for (int iRestPts=pNewContrPoints.size(); iRestPts<iSegments+1; iRestPts++)
		{
			pNewContrPoints.push_back(this->m_Segments[this->NbSegment() - 1].P(1));
		}
	}
	
	int iTotalLen = pNewContrPoints.size();
	assert(iTotalLen == (iSegments + 1));	

	Create_Catmull_Rom_Spline(pNewContrPoints);		
}

void CCubicBezierSpline::GetPartCtrPts(int iStartSegment, int iEndSegment, std::vector<CPoint2> &CtrPts) const
{
	if(iEndSegment<iStartSegment)
		return;

	if(iEndSegment>=this->NbSegment())
		iEndSegment = this->NbSegment()-1;
	if(iStartSegment<0)
		iStartSegment = 0;

	CtrPts.clear();
	CtrPts.reserve(iEndSegment-iStartSegment+1);
	for(int k=iStartSegment; k<=iEndSegment; k++)
	{
		CPoint2 pt = this->m_Segments[k].P(0);
		CtrPts.push_back(pt);
	}

	CPoint2 pt = this->m_Segments[iEndSegment].P(1);
	CtrPts.push_back(pt);
}

void CCubicBezierSpline::GetKeyPoints(std::vector<CPoint2> &keypoints) const
{
	keypoints.resize(NbSegment() + 1);
	for (int i = 0; i < NbSegment(); i++)
		keypoints[i] = m_Segments[i].m_ControlPoints[0];
	keypoints[NbSegment()] = m_Segments[NbSegment() - 1].m_ControlPoints[3];
}

double CCubicBezier::DistantToPoint(const CPoint2 point) const
{
	double mindist = m_ControlPoints[0].DistantTo(point);
	for (int i = 1; i < 4; i++)
	{
		double dist = m_ControlPoints[i].DistantTo(point);
		if (dist < mindist)
			mindist = dist;
	}
	return mindist;
}

double CCubicBezier::DistantYToPoint(const CPoint2 point) const
{
	double mindist = fabs(m_ControlPoints[0].Y - point.Y);
	for (int i = 1; i < 4; i++)
	{
		double dist = fabs(m_ControlPoints[i].Y - point.Y);
		if (dist < mindist)
			mindist = dist;
	}
	return mindist;
}

double CCubicBezierSpline::DistantToPoint(const CPoint2 point, int * pIndex) const
{
	double mindist = m_Segments[0].DistantToPoint(point);
	int minindex = 0;

	for (int i = 1; i < NbSegment(); i++)
	{
		double dist = m_Segments[i].DistantToPoint(point);
		if (mindist > dist)
		{
			mindist = dist;
			minindex = i;
		}
	}
	if (pIndex != NULL) *pIndex = minindex; 
	return mindist;
}

double CCubicBezierSpline::DistantToPoint3(const CPoint2 point, int& iSegNum) const
{
	iSegNum = 0;
	double mindist = m_Segments[0].DistantToPoint(point);
	for (int i = 1; i < NbSegment(); i++)
	{
		double dist = m_Segments[i].DistantToPoint(point);
		if (mindist > dist)
		{
			mindist = dist;
			iSegNum = i;
		}			
	}
	
	return mindist;
}

double CCubicBezierSpline::DistantYToPoint(const CPoint2 point, int& iSegNum) const
{
	iSegNum = 0;
	double mindist = m_Segments[0].DistantYToPoint(point);
	for (int i = 1; i < NbSegment(); i++)
	{
		double dist = m_Segments[i].DistantYToPoint(point);
		if (mindist > dist)
		{
			mindist = dist;
			iSegNum = i;
		}			
	}
	
	return mindist;
}

double CCubicBezierSpline::DistantToPoint2(const CPoint2 point, CPoint2 &NearPt, bool& bVertical) const
{
	double mindist = m_Segments[0].DistantToPoint(point);	
	int iMinSegNum = 0;
	for (int i = 1; i < NbSegment(); i++)
	{
		double dist = m_Segments[i].DistantToPoint(point);
		if (mindist > dist)
		{
			mindist = dist;
			iMinSegNum = i;			
			NearPt = m_Segments[i].P(0);
		}		
	}

	int iMinSeg = iMinSegNum;
	double dMin_u = 0;
	for(int k = -1; k < 2; k++)
	{
		int iSegNum = iMinSegNum + k;
		if(iSegNum < 0 || iSegNum >= NbSegment())	
			continue;

		CPoint2 curP;	
		int num = (int) m_Segments[iSegNum].Length(0.05);
		double delta = 1.0 / num;
		double u = 0.0;
		
		assert(num >= 1);
		for (int j = 0; j <= num; j++, u += delta)
		{
			curP = m_Segments[iSegNum].P(u);
			double dist = point.DistantTo(curP);		
			if(dist <= mindist)
			{
				mindist = dist;
				NearPt = curP;
				dMin_u = u;
				iMinSeg = iSegNum;
			}				
		}		
	}
	
	CVector2 n = m_Segments[iMinSeg].Normal(dMin_u);
	CVector2 l = NearPt - point;	

	double dCosAngle;
	if(sqrt(l * l)<1e-4)
		dCosAngle = 1;
	else
		dCosAngle = n * l / sqrt((n * n) * (l * l));
	assert(fabs(dCosAngle) <= 1);

	CPoint2 startPt = m_Segments[0].P(0);
	CVector2 endPt = m_Segments[NbSegment() - 1].P(1);

	if(fabs(dCosAngle) >= 0.8)
		bVertical = true;
	else
		bVertical = false;		

	return mindist;
}

double CCubicBezierSpline::DistantToPoint4(const CPoint2 point, CPoint2 &NearPt, int &iSegNum, double &dMin_u, float &tangent_angle) const
{
	double mindist = m_Segments[0].DistantToPoint(point);	
	int iMinSegNum = 0;
	NearPt = m_Segments[0].P(0);
	for (int i = 1; i < NbSegment(); i++)
	{
		double dist = m_Segments[i].DistantToPoint(point);
		if (mindist > dist)
		{
			mindist = dist;
			iMinSegNum = i;			
			NearPt = m_Segments[i].P(0);
		}		
	}

	int iMinSeg = iMinSegNum;
	dMin_u = 0;
	for(int k = -1; k < 2; k++)
	{
		int iSegNum = iMinSegNum + k;
		if(iSegNum < 0 || iSegNum >= NbSegment())	
			continue;

		CPoint2 curP;	
		int num = (int) m_Segments[iSegNum].Length(0.02);
		double delta = 1.0 / num;
		double u = 0.0;
		
		assert(num >= 1);
		for (int j = 0; j <= num; j++, u += delta)
		{
			curP = m_Segments[iSegNum].P(u);
			double dist = point.DistantTo(curP);		
			if(dist <= mindist)
			{
				mindist = dist;
				NearPt = curP;
				dMin_u = u;
				iMinSeg = iSegNum;
			}				
		}		
	}
	
	CVector2 n = m_Segments[iMinSeg].Normal(dMin_u);
	iSegNum = iMinSeg;
//	CVector2 l = NearPt - point;	

	// modified by zijian
	tangent_angle = atan2(n.Y, n.X) + MY_PI/2.;
	if(tangent_angle < 0.) 
		tangent_angle += 2.*MY_PI;
	else if(tangent_angle >= 2.*MY_PI) 
		tangent_angle -= 2.*MY_PI;
	
/*
	double dCosAngle;
	if(sqrt(l * l)<1e-4)
		dCosAngle = 1;
	else
		dCosAngle = n * l / sqrt((n * n) * (l * l));
	assert(fabs(dCosAngle) <= 1);

	CPoint2 startPt = m_Segments[0].P(0);
	CVector2 endPt = m_Segments[NbSegment() - 1].P(1);

	if(fabs(dCosAngle) >= 0.8)
		bVertical = true;
	else
		bVertical = false;		
*/
	return mindist;
}

double CCubicBezier::Length(double interval) const
{
	int segment = 2;
	bool converge = false;
	double length =0;
	while (!converge)
	{
		length = 0;
		double delta = 1.0 / segment;
		CPoint2 oldp = P(0);

		for (int i = 1; i < segment + 1; i++)
		{
			CPoint2 curP = P(delta * i);
			length += oldp.DistantTo(curP);
			oldp = curP;
		}
		if (length/segment < interval)
			converge = true;
		else
			segment *= 2;
	}
	return length;
}

double CCubicBezierSpline::Length(double interval) const
{
	double length = 0.0;
	for (int i = 0; i < NbSegment(); i++)
		length += m_Segments[i].Length(interval);
	return length;
}

double CCubicBezierSpline::DistantToCurve(const CCubicBezierSpline &Curve) const
{
	CCubicBezierSpline Curve2 = Curve;

	double dLen = Curve2.Length();	
	int iSegments = (int)dLen;
	Curve2.Resample(iSegments);

	double dWidth = -1.0;

	CPoint2 StartPt = Curve2.m_Segments[0].P(0);
	CPoint2 EndPt = Curve2.m_Segments[Curve2.NbSegment() - 1].P(1);

	{
		CPoint2 CurPt = Curve2.m_Segments[0].P(0);
		CPoint2 NearPt;
		bool bVertical;
		double dDist = this->DistantToPoint2(CurPt, NearPt, bVertical);
		if(!bVertical)
		{
			CurPt = this->m_Segments[0].P(0);
			dDist = Curve.DistantToPoint2(CurPt, StartPt, bVertical);
			assert(bVertical == true);
		}		
	}

	{
		CPoint2 CurPt = Curve2.m_Segments[Curve2.NbSegment() - 1].P(1);
		CPoint2 NearPt;
		bool bVertical;
		double dDist = this->DistantToPoint2(CurPt, NearPt, bVertical);	
		if(!bVertical)
		{
			CurPt = this->m_Segments[this->NbSegment() - 1].P(1);
			dDist = Curve.DistantToPoint2(CurPt, EndPt, bVertical);
			assert(bVertical == true);
		}
	}

	for(int i = 0; i < Curve2.NbSegment(); i++)
	{
		CPoint2 CurPt = Curve2.m_Segments[i].P(0);

		if(CurPt.DistantTo(EndPt) <= 1)
			break;

		if(CurPt.DistantTo(StartPt) <= 1)
		{
			CPoint2 NearPt;
			bool bVertical;
			double dDist = this->DistantToPoint2(CurPt, NearPt, bVertical);	
			if((dDist > dWidth) && bVertical)
				dWidth = dDist;
		}		
	}

	return dWidth;
}

double CCubicBezierSpline::Length(int iStart, int iEnd) const
{
	double dLength = 0.0;

	if(iStart == iEnd)
		return 0;

	for(int k = iStart; k <= iEnd; k++)
	{
		if(k < 0)
			continue;

		if(k > NbSegment())
			break;

		CPoint2 CurPt = m_Segments[k].P(0);
		CPoint2 NextPt;
		if(k < NbSegment())
			NextPt = m_Segments[k + 1].P(0);
		else
			NextPt = m_Segments[NbSegment() - 1].P(1);

		double Dist = CurPt.DistantTo(NextPt);
		dLength = dLength + Dist;
	}

	return dLength;
}

COutputDataFile& operator<<(COutputDataFile& ofs, const CCubicBezier& cb)
{
	return ofs << cb.m_ControlPoints[0] << cb.m_ControlPoints[1] << cb.m_ControlPoints[2] << cb.m_ControlPoints[3] ;
}

CInputDataFile& operator>>(CInputDataFile& ifs, CCubicBezier& cb)
{
	ifs >> cb.m_ControlPoints[0] >> cb.m_ControlPoints[1] >> cb.m_ControlPoints[2] >> cb.m_ControlPoints[3] ;
	cb.Precalc();
	return ifs;
}

COutputDataFile& operator<<(COutputDataFile& ofs, const CCubicBezierSpline& cbs)
{
	std::vector<CPoint2> vKeyPoint;
	
	cbs.GetKeyPoints(vKeyPoint);
	ofs << (int)vKeyPoint.size();

	for (int iPoint = 0; iPoint < vKeyPoint.size(); iPoint++ )
	{
		ofs << vKeyPoint[iPoint];
	}
	return ofs;
}

CInputDataFile& operator>>(CInputDataFile& ifs, CCubicBezierSpline& cbs)
{
	int nPoint;
	ifs >> nPoint;

	std::vector<CPoint2> vKeyPoint;
	vKeyPoint.resize(nPoint);
	for (int iPoint = 0; iPoint < vKeyPoint.size(); iPoint++ )
	{
		ifs >> vKeyPoint[iPoint];
	}
	cbs.Create_Catmull_Rom_Spline(vKeyPoint);
	return ifs;
}






// Shape.cpp: implementation of the CShape class.
//
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Shape.h"
#include "Point2.h"

CShape::CShape(void)
{
	Clear();
}

CShape::~CShape(void)
{
    Clear();
}

void CShape::Clear(void)
{
	m_PointNum = 0;
}

bool CShape::operator==(const CShape& shape) const
{
	if ( this==&shape ) return true;
	if ( shape.PointNum()!=PointNum() ) return false;

	for ( int i=0; i<m_Points.Length(); i++ )
	{
		if ( shape.m_Points[i]!=m_Points[i] )
		{
			return false;
		}
	}

	return true;
}

CShape& CShape::operator/=(double factor)
{
    assert(factor != 0);

    if (factor != 0)
    {
        m_Points /= factor;
    }

    return *this;
}

CShape& CShape::operator +=(const CShape &shape)
{
	if ( &shape==this )
	{
		Scale(2.);
	}
	else
	{
		int point_num = PointNum();
		assert(point_num == shape.PointNum());
		for(int i=0; i<point_num; i++) {
			PointX(i) += shape.PointX(i);
			PointY(i) += shape.PointY(i);
		}
	}

	return *this;
}

const CShape& CShape::operator=(const CShape& ref)
{
	if ( this!=&ref )
	{
		Resize(ref.PointNum());
		m_Points = ref.m_Points;
	}
	return (*this);
}

CPoint2 CShape::GetPoint(int index) const
{
	CPoint2 p2;
	if (index<0 || index>=PointNum()) return p2;

	p2.X = PointX(index);
	p2.Y = PointY(index);

	return p2;
}

void CShape::SetPoint(int index, const CPoint2& p2)
{
	if (index<0 || index>=PointNum()) return;
	
	PointX(index) = p2.X;
	PointY(index) = p2.Y;
}

void CShape::Scale(double s)
{
    double *dataPtr = m_Points.data();
    for (int i = 0; i < m_PointNum; i++)
    {
        *dataPtr++ *= s;
        *dataPtr++ *= s;
    }
}

void CShape::Scale(double sX, double sY)
{
    double *dataPtr = m_Points.data();
    for (int i = 0; i < m_PointNum; i++)
    {
        *dataPtr++ *= sX;
        *dataPtr++ *= sY;
    }
}

void CShape::ScaleAroundCenter(double s)
{
    double centerx, centery;
    CenterPoint(centerx, centery);
    Translate(-centerx, -centery);
    Scale(s);
    Translate(centerx, centery);
}

void CShape::RotateAroundCenter(double theta)
{
	double centerx, centery;
    CenterPoint(centerx, centery);
    Translate(-centerx, -centery);
    Rotation(theta);
    Translate(centerx, centery);
}

void CShape::ScaleAroundCenter(double cx, double cy, double s)
{
	Translate(-cx, -cy);
	Scale(s);
	Translate(cy, cy);
}

void CShape::Translate(double x, double y)
{
    double *dataPtr = m_Points.data();
    for (int i = 0; i < m_PointNum; i++)
    {
        *dataPtr++ += x;
        *dataPtr++ += y;
    }
}

void CShape::CenterPoint(double &x, double &y) const
{
    x = 0.0; y = 0.0;

    if (m_PointNum == 0)
    {
        return;
    }

    double *dataPtr = m_Points.data();
    for (int i = 0; i < m_PointNum; i++)
    {
        x += *dataPtr++;
        y += *dataPtr++;
    }

    x /= m_PointNum;
    y /= m_PointNum;
}

bool CShapeSet::MeanShape(CShape& shape) const
{
	shape.Clear();

	if ( size()<=0 ) 
	{
		return false;
	}
	else if ( size()==1 )
	{
		shape = (*this)[0];
		return true;
	}
	else 
	{
		int nPointNum = ((*this)[0]).PointNum();
		if ( nPointNum<=0 ) return false;

		shape = (*this)[0];
		for ( int i=1; i<size(); i++ )
		{
			if ( ((*this)[i]).PointNum()!=nPointNum ) return false;
			shape += (*this)[i];
		}

		shape /= double(size());
		return true;
	}
}

void CShapeSet::CenterPoint(double &x, double &y)
{
    x = 0.0; y = 0.0;

	int nPointNum = 0;
	for (int iShape=0; iShape<this->size(); iShape++)
	{
		for (int iPoint=0; iPoint<(*this)[iShape].PointNum(); iPoint++)
		{
			x += (*this)[iShape].PointX(iPoint);
			y += (*this)[iShape].PointY(iPoint);
		}

		nPointNum += (*this)[iShape].PointNum();
	}

    if (nPointNum == 0)
    {
        return;
    }
	
    x /= nPointNum;
    y /= nPointNum;
}

void CShape::CenterPoint(double &x, double &y, int start, int count) const
{
    x = 0.0; y = 0.0;

    if (m_PointNum < start + count)
    {
        return;
    }

    double *dataPtr = m_Points.data() + (start << 1);
    for (int i = 0; i < count; i++)
    {
        x += *dataPtr++;
        y += *dataPtr++;
    }

    x /= count;
    y /= count;
}

void CShape::AroundRect(double& left, double& top, double& right, double& bottom) const
{
    left = top = 10000.0;
	right = bottom = -10000.0;
    
    if (m_PointNum == 0)
    {
        return;
    }

    left = right = PointX(0);
    bottom = top = PointY(0);
    for (int i = 1; i < m_PointNum; i++)
    {
        if (left > PointX(i)) left = PointX(i);
        if (right < PointX(i)) right = PointX(i);
        if (bottom < PointY(i)) bottom = PointY(i);
        if (top > PointY(i)) top = PointY(i);
    }
}

double CShape::Distance(const CShape &refShape) const
{
    if ((m_PointNum == 0) || (refShape.PointNum() != m_PointNum))
    {
        assert(0);;
        return -1.0;
    }

    double x, y;
    double dist = 0.0;
    double *dataPtr = m_Points.data();
    double *refDataPtr = refShape.Points().data();
    for (int i = 0; i < m_PointNum; i++)
    {
        x = *dataPtr++ - *refDataPtr++;
        y = *dataPtr++ - *refDataPtr++;
        dist += x * x + y * y;
    }
    return sqrt(dist / m_PointNum);
}

double CShape::Length(double interval/* =0.2 */) const
{
	if ( m_PointNum<3 ) return 0.;

	CCubicBezierSpline spline;
	std::vector<CPoint2> vPoint2;
	vPoint2.resize(m_PointNum);
	for ( int i=0; i<m_PointNum; i++ )
	{
		vPoint2[i].X = m_Points[2*i];
		vPoint2[i].Y = m_Points[2*i+1];
	}

	spline.Create_Catmull_Rom_Spline(vPoint2);

	return spline.Length(interval);
}

double CShape::PointToPointAvgDistance(const CShape& refShape) const
{
    if ((m_PointNum == 0) || (refShape.PointNum() != m_PointNum))
    {
        assert(0);;
        return -1.0;
    }

    double x, y;
    double dist = 0.0;
    double *dataPtr = m_Points.data();
    double *refDataPtr = refShape.Points().data();
    for (int i = 0; i < m_PointNum; i++)
    {
        x = *dataPtr++ - *refDataPtr++;
        y = *dataPtr++ - *refDataPtr++;
        dist += sqrt(x * x + y * y);
    }
    return dist / m_PointNum;
}

int CShape::NearestPoint(double x, double y) const
{
    int index = -1;
    double detx, dety;
    double dist, minDist = DBL_MAX;
    double *dataPtr = m_Points.data();
    for (int i = 0; i < m_PointNum; i++)
    {
        detx = *dataPtr++ - x;
        dety = *dataPtr++ - y;
        dist = detx * detx + dety * dety;
        if (dist < minDist)
        {
            index = i;
            minDist = dist;
        }
    }
    return index;
}

CShapeSet::CShapeSet(void)
{

}

CShapeSet::~CShapeSet(void)
{

}

CShapeSet& CShapeSet::operator/=(double factor)
{
    assert(factor != 0);

    if (factor != 0)
    {
        for (int i = 0; i < size(); i++)
        {
            (*this)[i] /= factor;
        }
    }

    return *this;
}

void CShapeSet::Scale(double s)
{
	for (int i=0; i<size(); i++)
	{
		(*this)[i].Scale(s);
	}
}

void CShapeSet::Scale(double sX, double sY)
{
	for (int i=0; i<size(); i++)
	{
		(*this)[i].Scale(sX, sY);
	}
}

void CShapeSet::ScaleAroundCenter(double s)
{
	for (int i=0; i<size(); i++)
	{
		(*this)[i].ScaleAroundCenter(s);
	}
}
	
void CShapeSet::ScaleAroundCenter(double cx, double cy, double s)
{
	for (int i=0; i<size(); i++)
	{
		(*this)[i].ScaleAroundCenter(cx, cy, s);
	}
}

void CShapeSet::Translate(double dx, double dy)
{
	for (int i=0; i<size(); i++)
	{
		(*this)[i].Translate(dx, dy);
	}
}

void CShapeSet::Rotation(double theta)
{
	for (int i=0; i<size(); i++)
	{
		(*this)[i].Rotation(theta);
	}
}

void CShapeSet::RotateAroundCenter(double theta)
{
	for (int i=0; i<size(); i++)
	{
		(*this)[i].RotateAroundCenter(theta);
	}
}

void CShapeSet::Rank(bool descent/* =true */)
{
	if (size()<=1) return;

	int i, j;
	std::vector<int> vIndex;
	vIndex.resize(size());
	for (i=0; i<size(); i++) vIndex[i] = i;

	if (descent)
	{
		for (i=0; i<size()-1; i++)
		{
			for (j=i+1; j<size(); j++)
			{
				if ((*this)[vIndex[i]].PointNum() < (*this)[vIndex[j]].PointNum())
				{
					int tempi = vIndex[i];
					vIndex[i] = vIndex[j];
					vIndex[j] = tempi;
				}
			}
		}
	}
	else 
	{
		for (i=0; i<size()-1; i++)
		{
			for (j=i+1; j<size(); j++)
			{
				if ((*this)[vIndex[i]].PointNum() > (*this)[vIndex[j]].PointNum())
				{
					int tempi = vIndex[i];
					vIndex[i] = vIndex[j];
					vIndex[j] = tempi;
				}
			}
		}
	}

	CShapeSet tempSS = *this;
	for (i=0; i<tempSS.size(); i++)
	{
		(*this)[i] = tempSS[vIndex[i]];
	}
}

void ResampleShape(const CShape& srcShape, const CConnectedModel& srcConModel, CShape& dstShape, CConnectedModel& dstConModel, double fSampleStep)
{
	if ( fSampleStep<=1. ) return;
	if ( srcShape.PointNum()<=0 ) return;
	if ( srcConModel.ContourNum()<=0 ) return;
	if ( srcConModel.PointNum()<=0 ) return;

	CShapeSet ss;
	shape2shapeset(srcShape, ss, srcConModel);

	std::vector<CPoint2> vdstpoint2;
	vdstpoint2.clear();
	dstConModel.Clear();

	for ( int iSS=0; iSS<ss.size(); iSS++ )
	{
		std::vector<CPoint2> vpoint2;
		Shape2vPoint2(ss[iSS], vpoint2);

		if ( vpoint2.size()>2 )
		{
			CCubicBezierSpline spline;
			spline.Create_Catmull_Rom_Spline(vpoint2);
			
			if ( spline.Length()>=fSampleStep )
			{
				int nSegment = max(1, spline.Length()/fSampleStep);
				spline.Resample(vpoint2, nSegment);
			}
		}

		int iPoint = vdstpoint2.size();
		CIndexedContour contour(srcConModel.Contour(iSS).Type(), vpoint2.size());
		for ( int i=0; i<vpoint2.size(); i++ )
		{
			vdstpoint2.push_back(vpoint2[i]);
			contour.SetIndex(i, iPoint+i);
		}
		dstConModel.AddNewContour(contour);
	}

	vPoint22Shape(vdstpoint2, dstShape);
}

void ResampleCurve(const CShape& srcCurve, CShape& dstCurve, int nStepByPixels)
{
	dstCurve.Clear();
	CShape tempShape;

	if ( srcCurve.PointNum()<=1 )
	{
		return;
	}
	else if ( srcCurve.PointNum() == 2 )
	{
		if ( srcCurve.GetPoint(0).DistantTo(srcCurve.GetPoint(1)) <= nStepByPixels ) return;

		tempShape.Resize(3);
		tempShape.SetPoint(0, srcCurve.GetPoint(0));
		tempShape.SetPoint(1, srcCurve.GetPoint(0).MiddlePoint(srcCurve.GetPoint(1)));
		tempShape.SetPoint(2, srcCurve.GetPoint(1));
	}
	else
	{
		tempShape = srcCurve;
	}

	std::vector<CPoint2> vpoint2;
	Shape2vPoint2(tempShape, vpoint2);

	CCubicBezierSpline spline;
	spline.Create_Catmull_Rom_Spline(vpoint2);
	if ( spline.Length()>=nStepByPixels )
	{
		int nSegment = max(1, spline.Length()/nStepByPixels);
		spline.Resample(vpoint2, nSegment);
	}

	vPoint22Shape(vpoint2, dstCurve);
}

// re-sample a curve to different number of control points
// keep the first and last points as unchanged!
void ResampleShape(const CShape& orgshape, CShape& dstshape, int nPointNum)
{
	dstshape.Clear();
	if ( nPointNum<2 ) return;

	std::vector<CPoint2> vpoint2;
	if ( orgshape.PointNum()<2 )
	{
		return;
	}
	else if ( orgshape.PointNum()==2 )
	{
		if ( 2==nPointNum )
		{
			dstshape = orgshape;
			return;
		}
		else if ( 3==nPointNum )
		{
			dstshape.Resize(3);
			dstshape.SetPoint(0, orgshape.GetPoint(0));
			dstshape.SetPoint(2, orgshape.GetPoint(1));
			dstshape.SetPoint(1, orgshape.GetPoint(0).MiddlePoint(orgshape.GetPoint(1)));
			return;
		}
		else
		{
			vpoint2.resize(3);
			vpoint2[0].X = orgshape.PointX(0);
			vpoint2[0].Y = orgshape.PointY(0);
			vpoint2[2].X = orgshape.PointX(1);
			vpoint2[2].Y = orgshape.PointY(1);
			vpoint2[1] = orgshape.GetPoint(0).MiddlePoint(orgshape.GetPoint(1));
		}
	}
	else
	{
		if ( 2==nPointNum )
		{
			dstshape.Resize(2);
			dstshape.SetPoint(0, orgshape.GetPoint(0));
			dstshape.SetPoint(1, orgshape.GetPoint(orgshape.PointNum()-1));
			return;
		}
		else
		{
			Shape2vPoint2(orgshape, vpoint2);
		}
	}

	CCubicBezierSpline spline;
	spline.Create_Catmull_Rom_Spline(vpoint2);
	spline.Resample(vpoint2, nPointNum-1);
	vPoint22Shape(vpoint2, dstshape);

	dstshape.SetPoint(0, orgshape.GetPoint(0));
	dstshape.SetPoint(nPointNum-1, orgshape.GetPoint(orgshape.PointNum()-1));
}

void GetSubShape(const CShape& orgshape, int orgindex, CShape& subshape, int subindex, int number)
{
    for (int i = 0; i < number; i++)
    {
		if (i+subindex>=subshape.PointNum() || i+orgindex>=orgshape.PointNum()) break;
        subshape.PointX(i + subindex) = orgshape.PointX(i + orgindex);
        subshape.PointY(i + subindex) = orgshape.PointY(i + orgindex);
    }
}

void GetSubShape(const CShape& orgshape, CShape& subshape, int number, const int * index)
{
    for (int i = 0; i < number; i++)
    {
 		if (i>=subshape.PointNum() || index[i]>=orgshape.PointNum()) continue;
	    subshape.PointX(i) = orgshape.PointX(index[i]);
        subshape.PointY(i) = orgshape.PointY(index[i]);
    }
}

void PutSubShape(CShape& destShape, const CShape& subshape, int number, int startindex)
{
    for (int i = 0; i < number; i++)
    {
		if (i+startindex>=destShape.PointNum() || i>=subshape.PointNum()) break;
        destShape.PointX(i + startindex) = subshape.PointX(i);
        destShape.PointY(i + startindex) = subshape.PointY(i);
	}
}

void Combine2Shapes(const CShape& srcShape1, const CShape& srcShape2, CShape &desShape)
{
	desShape.Resize(srcShape1.PointNum() + srcShape2.PointNum());

	PutSubShape(desShape, srcShape1, srcShape1.PointNum(), 0);
	PutSubShape(desShape, srcShape2,  srcShape2.PointNum(), srcShape1.PointNum());
}

void POINTS2Shape(PPOINT lpPoints, int count, CShape& shape)
{
    if (!lpPoints || count < 0)
    {
        assert(0);
        return;
    }

    shape.Resize(count);
    for (int i = 0; i < count; i++)
    {
        shape.PointX(i) = lpPoints[i].x;
        shape.PointY(i) = lpPoints[i].y;
    }
}

void GetResample2DRTSMatrix(CVisTransform4x4 &destM, CShape &srcShape, CShape &destShape)
{
    CVisDMatrix A;
    CVisDVector B;
    CVisDVector T;
	
    int num = srcShape.PointNum();
    assert(destShape.PointNum() == num);

    A.Resize(num * 2, 4);
    B.Resize(2 * num);
    T.Resize(4);

    for (int i = 0; i < num; i++)
    {
        A[i * 2    ][0] = destShape.PointX(i);
        A[i * 2    ][1] = -destShape.PointY(i);
        A[i * 2    ][2] = 1;
        A[i * 2    ][3] = 0;
        A[i * 2 + 1][0] = destShape.PointY(i);
        A[i * 2 + 1][1] = destShape.PointX(i);
        A[i * 2 + 1][2] = 0;
        A[i * 2 + 1][3] = 1;
        B[i * 2    ]    = srcShape.PointX(i);
        B[i * 2 + 1]    = srcShape.PointY(i);
    }

    CVisDMatrix AT, K;
    AT.Resize(4, num * 2);
    AT = A.Transposed();
    K.Resize(4, 4);
    K = AT * A;
    K.Invert();
	
    CVisDVector ATB;
    ATB.Resize(4);
    ATB = AT * B;

    T = K * ATB;

    //  a -b 0 Tx
    //  b  a 0 Ty
    //  0  0 1 0
    //  0  0 0 1
    destM[0][0] = T[0];
    destM[0][1] = -T[1];
    destM[1][0] = T[1];
    destM[1][1] = T[0];
    destM[0][3] = T[2];
    destM[1][3] = T[3];
}

void GetTrans2DRTSMatrix(CVisTransform4x4 &destM, CVisDVector &srcPts, CVisDVector &dstPts)
{
    CVisDMatrix A;
    CVisDVector B;
    CVisDVector T;

	assert(srcPts.Length()>0 && srcPts.Length()%2==0);
    assert(dstPts.Length() == dstPts.Length());
    int num = srcPts.Length()*0.5;

    A.Resize(num * 2, 4);
    B.Resize(2 * num);
    T.Resize(4);

    for (int i = 0; i < num; i++)
    {
		int ix = 2*i;
		int iy = 2*i+1;

        A[i * 2    ][0] = srcPts[ix];
        A[i * 2    ][1] = -srcPts[iy];
        A[i * 2    ][2] = 1;
        A[i * 2    ][3] = 0;
        A[i * 2 + 1][0] = srcPts[iy];
        A[i * 2 + 1][1] = srcPts[ix];
        A[i * 2 + 1][2] = 0;
        A[i * 2 + 1][3] = 1;
        B[i * 2    ]    = dstPts[ix];
        B[i * 2 + 1]    = dstPts[iy];
    }

    CVisDMatrix AT, K;
    AT.Resize(4, num * 2);
    AT = A.Transposed();
    K.Resize(4, 4);
    K = AT * A;
    K.Invert();
	
    CVisDVector ATB;
    ATB.Resize(4);
    ATB = AT * B;

    T = K * ATB;

    //  a -b 0 Tx
    //  b  a 0 Ty
    //  0  0 1 0
    //  0  0 0 1
    destM[0][0] = T[0];
    destM[0][1] = -T[1];
    destM[1][0] = T[1];
    destM[1][1] = T[0];
	destM[0][3] = T[2];
	destM[1][3] = T[3];
}

void Apply2DRTSTransform(CVisTransform4x4 &transM, const CVisDVector& srcPts, CVisDVector& dstPts)
{
	assert(srcPts.Length()%2==0);

	dstPts.Resize(srcPts.Length());
	int num = srcPts.Length()*0.5;

    for (int i = 0; i < num; i++)
    {
        double srcx = srcPts[2*i];
        double srcy = srcPts[2*i+1];
        dstPts[2*i] = transM[0][0] * srcx + transM[0][1] * srcy + transM[0][3];
        dstPts[2*i+1] = transM[1][0] * srcx + transM[1][1] * srcy + transM[1][3];
    }
}

void GetTrans2DRTSMatrix(CVisTransform4x4 &destM, CShape &srcShape, CShape &destShape)
{
    CVisDMatrix A;
    CVisDVector B;
    CVisDVector T;
	
    int num = srcShape.PointNum();
    assert(destShape.PointNum() == num);

    A.Resize(num * 2, 4);
    B.Resize(2 * num);
    T.Resize(4);

    for (int i = 0; i < num; i++)
    {
        A[i * 2    ][0] = srcShape.PointX(i);
        A[i * 2    ][1] = -srcShape.PointY(i);
        A[i * 2    ][2] = 1;
        A[i * 2    ][3] = 0;
        A[i * 2 + 1][0] = srcShape.PointY(i);
        A[i * 2 + 1][1] = srcShape.PointX(i);
        A[i * 2 + 1][2] = 0;
        A[i * 2 + 1][3] = 1;
        B[i * 2    ]    = destShape.PointX(i);
        B[i * 2 + 1]    = destShape.PointY(i);
    }

    CVisDMatrix AT, K;
    AT.Resize(4, num * 2);
    AT = A.Transposed();
    K.Resize(4, 4);
    K = AT * A;
    K.Invert();
	
    CVisDVector ATB;
    ATB.Resize(4);
    ATB = AT * B;

    T = K * ATB;

    //  a -b 0 Tx
    //  b  a 0 Ty
    //  0  0 1 0
    //  0  0 0 1
    destM[0][0] = T[0];
    destM[0][1] = -T[1];
    destM[1][0] = T[1];
    destM[1][1] = T[0];
	destM[0][3] = T[2];
	destM[1][3] = T[3];
}

void Apply2DRTSTransform(CVisTransform4x4 &transM, const CShape &srcShape, CShape &destShape)
{
    destShape.Resize(srcShape.PointNum());
    for (int i = 0; i < srcShape.PointNum(); i++)
    {
        double srcx = srcShape.PointX(i);
        double srcy = srcShape.PointY(i);
        destShape.PointX(i) = transM[0][0] * srcx + transM[0][1] * srcy + transM[0][3];
        destShape.PointY(i) = transM[1][0] * srcx + transM[1][1] * srcy + transM[1][3];
    }
}

void Apply2DRTSTransform(CVisTransform4x4 &transM, CShape &shape)
{
    for (int i = 0; i < shape.PointNum(); i++)
    {
        double srcx = shape.PointX(i);
        double srcy = shape.PointY(i);
        shape.PointX(i) = transM[0][0] * srcx + transM[0][1] * srcy + transM[0][3];
        shape.PointY(i) = transM[1][0] * srcx + transM[1][1] * srcy + transM[1][3];
    }
}

void DrawShape(HDC hdc, CShape& path, bool fClosed)
{
    int size = path.PointNum();
    PPOINT lpPoints = new POINT[size + 1];
    for (int i = 0; i < size; i++)
    {
        lpPoints[i].x = (LONG) path.PointX(i);
        lpPoints[i].y = (LONG) path.PointY(i);
    }
    if (fClosed)
    {
        lpPoints[i].x = (LONG) path.PointX(0);
        lpPoints[i].y = (LONG) path.PointY(0);
    }
    Polyline(hdc, lpPoints, size + (fClosed ? 1 : 0));
    delete []lpPoints;
}


//modified by lin begin, 2003-3
void Apply2DRTSTransform(CVisTransform4x4& transM,double inx,double iny,double& outx,double& outy)
{
	outx = transM[0][0] * inx + transM[0][1] * iny + transM[0][3];
	outy = transM[1][0] * inx + transM[1][1] * iny + transM[1][3];

	//assert(outx>=0);
	//assert(outy>=0);
}
//modified by lin end


// Add By Hchen 2003-3-10 Begin 
void CShape::AlignTangent(const CShape &refshape)
{
	double Tx, Ty, s, theta;
	AlignTangent(refshape, Tx, Ty, s, theta);
}

void CShape::AlignTP(const CShape& refshape, std::vector<bool>& vbConstrained, double& cx, double& cy, double& Tx, double& Ty, double& s, double& theta)
{
	CShape tempShape = *this;
	AlignTP(refshape, vbConstrained);
	
	std::vector<bool> vbConstrained2;
	vbConstrained2.resize(vbConstrained.size());
	for (int i=0; i<vbConstrained2.size(); i++) vbConstrained2[i] = true;
	
	tempShape.AlignTangent(*this, vbConstrained, cx, cy, Tx, Ty, s, theta);	
}

void CShape::AlignTP(const CShape& refshape, std::vector<bool>& vbConstrained)
{
	if (PointNum() != refshape.PointNum()) return;
	if (vbConstrained.size() != refshape.PointNum()) return;

	std::vector<CPoint2> vInPoint2, vOutPoint2;
	vInPoint2.clear();
	vOutPoint2.clear();

	int i, iConstrained=0;
	for (i=0; i<vbConstrained.size(); i++)
	{
		if( vbConstrained[i] )
		{
			iConstrained++;
			CPoint2 p2;
			
			p2.X = refshape.PointX(i);
			p2.Y = refshape.PointY(i);
			vOutPoint2.push_back(p2);

			p2.X = PointX(i);
			p2.Y = PointY(i);
			vInPoint2.push_back(p2);
		}
	}

	if ( iConstrained == 0) 
	{
		return;
	}
	else if ( iConstrained == PointNum() )
	{
		*this = refshape;
		return;
	}

	CShape inShape, outShape;
	vPoint22Shape(vInPoint2, inShape);
	vPoint22Shape(vOutPoint2, outShape);

	CGeomThinSplineTrans tpTrans;
	tpTrans.Init(inShape, outShape);
	
	for (i=0; i<PointNum(); i++)
	{
		if ( !vbConstrained[i] )
		{
			CPoint2 inP2, outP2;
			
			inP2.X = PointX(i);
			inP2.Y = PointY(i);

			tpTrans.Transform(inP2, outP2);

			PointX(i) = outP2.X;
			PointY(i) = outP2.Y;
		}
		else 
		{
			PointX(i) = refshape.PointX(i);
			PointY(i) = refshape.PointY(i);
		}
	}
}

void CShape::AlignTangent(const CShape& refshape, std::vector<bool>& vbConstrained)
{
	double Tx, Ty, s, theta, cx, cy;
	AlignTangent(refshape, vbConstrained, cx, cy, Tx, Ty, s, theta);
}

void CShape::AlignTangent(const CShape& refshape, std::vector<bool>& vbConstrained, double& cx, double& cy, double& Tx, double& Ty, double& s, double& theta)
{
	assert(refshape.PointNum() == PointNum());
	assert(vbConstrained.size() == PointNum());

	Tx = 0.; Ty = 0.; s = 1.; theta = 0.;

	int i, nValidPt = 0;	
	for (i=0; i<vbConstrained.size(); i++)
	{
		if ( vbConstrained[i] )
			nValidPt++;
	}
	if ( nValidPt<=0 ) return;
	
	CShape s1, s2;
	s1.Resize(nValidPt);
	s2.Resize(nValidPt);
	nValidPt = 0;

	for (i=0; i<PointNum(); i++)
	{
		if ( vbConstrained[i] )
		{
			s1.PointX(nValidPt) = PointX(i);
			s1.PointY(nValidPt) = PointY(i);
		
			s2.PointX(nValidPt) = refshape.PointX(i);
			s2.PointY(nValidPt) = refshape.PointY(i);
			
			nValidPt++;
		}
	}

	s1.AlignTangent(s2, Tx, Ty, s, theta);
	s2.CenterPoint(cx, cy);

	Translate(-Tx, -Ty);
	Rotation(-theta);
	Scale(1/s);
	Translate(cx, cy);
}

void CShape::AlignTangent(const CShape& refshape, std::vector<bool>& vbConstrained, double& Tx, double& Ty, double& s, double& theta)
{
	assert(refshape.PointNum() == PointNum());
	assert(vbConstrained.size() == PointNum());
	
	Tx = 0.; Ty = 0.; s = 1.; theta = 0.;
	
	int i, nValidPt = 0;	
	for (i=0; i<vbConstrained.size(); i++)
	{
		if ( vbConstrained[i] )
			nValidPt++;
	}
	if ( nValidPt<=0 ) return;
	
	CShape s1, s2;
	s1.Resize(nValidPt);
	s2.Resize(nValidPt);
	nValidPt = 0;
	
	for (i=0; i<PointNum(); i++)
	{
		if ( vbConstrained[i] )
		{
			s1.PointX(nValidPt) = PointX(i);
			s1.PointY(nValidPt) = PointY(i);
			
			s2.PointX(nValidPt) = refshape.PointX(i);
			s2.PointY(nValidPt) = refshape.PointY(i);
			
			nValidPt++;
		}
	}
	
	s1.AlignTangent(s2, Tx, Ty, s, theta);
	
	Translate(-Tx, -Ty);
	Rotation(-theta);
	Scale(1/s);
}

void CShape::AlignTangent(const CShape &refshape,double& Tx,double& Ty,double& s,double& theta)
{
	CShape refcopy;
	refcopy = refshape;
	// Translate both shapecenter to (0,0) 
	double refcenterx,refcentery;
	refcopy.CenterPoint(refcenterx,refcentery);
	refcopy.Translate(-refcenterx,-refcentery);
	double centerx,centery;
	CenterPoint(centerx,centery);
	Translate(-centerx,-centery);
	
	// (x,y) is align to refshape (x',y')
	// SXX1 = E(x*x') SYY1 = E(y*y') SXY1 = E(x*y') SYX1 = E(y*x')
	// SXX = E(x*x) SYY = E(y*y)
	// x' = ax -by +tx
	// y' = bx +ay +ty
	// a = (SXX1+SYY1)/(SXX+SYY)
	// b = (SXY1-SYX1)/(SXX+SYY)
	if (0==PointNum()) return;
	double SXX,SYY,SXX1,SYY1,SXY1,SYX1;
	int i;
	SXX=SYY=SXX1=SYY1=SXY1=SYX1=0;
	for (i=0;i<PointNum();i++)
	{
		double x,y,x1,y1;
		x=PointX(i);y=PointY(i);
		x1=refcopy.PointX(i);y1=refcopy.PointY(i);
		SXX+=x*x;SYY+=y*y;
		SXX1+=x*x1;SYY1+=y*y1;
		SXY1+=x*y1;SYX1+=y*x1;
	}
	double a,b;
	if (SXX+SYY==0) return;
	a=(SXX1+SYY1)/(SXX+SYY);
	b=(SXY1-SYX1)/(SXX+SYY);
	
	// x' = ax -by +tx
	// y' = bx +ay +ty
	for (i=0;i<PointNum();i++)
	{
		double x,y;
		x=PointX(i);y=PointY(i);
		PointX(i)=a*x-b*y;
		PointY(i)=b*x+a*y;
	}

	// project into tangent space
	// scale x by ||x'||/<x,x'>
	double SX1X1;
	SX1X1=SXX1=0;	
	for (i=0;i<PointNum();i++)
	{
		double x,y,x1,y1;
		x=PointX(i);y=PointY(i);
		x1=refcopy.PointX(i);y1=refcopy.PointY(i);
			SX1X1+=x1*x1+y1*y1;
		SXX1+=x*x1+y*y1;
	}
	double ss = SX1X1/SXX1;
	Scale(ss);
	Translate(refcenterx,refcentery);	
	Tx=centerx;
	Ty=centery;
	s= sqrt(a*a+b*b) * ss; s=1/s;
	theta = - atan2(b,a);
}

void CShape::FlipHorizontal(double cx)
{
	for (int iP=0; iP<PointNum(); iP++)
	{
		PointX(iP) = 2.*cx - PointX(iP);
	}
}

void CShape::FlipVertical(double cy)
{
	for (int iP=0; iP<PointNum(); iP++)
	{
		PointY(iP) = 2.*cy - PointY(iP);
	}
}

double CShape::Norm2()
{
	int i;
	double norm=0;
	for (i=0;i<m_PointNum;i++)
		norm+=PointY(i)*PointY(i)+PointX(i)*PointX(i);
	return (norm);
}

void CShape::Normalize()
{
	m_Points /= sqrt(Norm2());	
}

void CShape::Rotation(double theta, double cx, double cy)
{
	double x,y;
	for (int i=0;i<m_PointNum;i++)
	{
		x = PointX(i) - cx;
		y = PointY(i) - cy;
		PointX(i) = cos(theta) * x - sin(theta) * y;
		PointY(i) = sin(theta) * x + cos(theta) * y;
	}
}
// Add By Hchen 2003-3-10 End 

//modified by lin begin, 2003-4-20
void Shape2POINTS(CShape& shape, PPOINT lpPoints, bool fClosed)
{
	if (!lpPoints)
	{
		assert(0);
		return;
	}

	for (int i = 0; i < shape.PointNum(); i++)
	{
		lpPoints[i].x = (LONG)shape.PointX(i);
		lpPoints[i].y = (LONG)shape.PointY(i);
	}

	if(fClosed)
	{
		lpPoints[shape.PointNum()].x = (LONG)shape.PointX(0);
		lpPoints[shape.PointNum()].y = (LONG)shape.PointY(0);
	}

}

void DrawMaskReg(CShape& contour, CVisGrayByteImage& MaskImg)
{
	//the mask reg is white
	CVisRGBAByteImage DesImg;
	DesImg.Allocate(MaskImg.Shape());
	DesImg.FillPixels(0);

	HDC hdc = DesImg.Hdc();	
	HPEN MaskPen       = ::CreatePen(0, 1, RGB(255, 255, 255));
	HBRUSH MaskBrush   = ::CreateSolidBrush(RGB(255, 255, 255));

	HPEN pOldPen = (HPEN)::SelectObject(hdc, MaskPen);
	HBRUSH pOldBrush = (HBRUSH)::SelectObject(hdc, MaskBrush);
	::SelectObject(hdc, MaskPen);
	::SelectObject(hdc, MaskBrush);	

	PPOINT lpPoints;
	lpPoints = new POINT[contour.PointNum()+1];
	Shape2POINTS(contour, lpPoints, true);
	::Polygon(hdc, lpPoints, contour.PointNum()+1);

	delete []lpPoints;
	::SelectObject(hdc, pOldPen);
	::SelectObject(hdc, pOldBrush);
	::DeleteObject(MaskPen);	
	::DeleteObject(MaskBrush);
	DesImg.DestroyHdc();
}
//modified by lin end, 2003-4-20

void GetConvexMaskImage(CShape& srcShape, int width, int height, CVisGrayByteImage& maskImg, int extraWidth)
{
    maskImg.Allocate(width, height);
    maskImg.FillPixels(0);
    
    CVisRGBAByteImage tempImg;
    tempImg.Allocate(width, height);
    tempImg.FillPixels(0);

    HDC hdc = tempImg.Hdc();

    HPEN hPen = ::CreatePen(PS_SOLID, extraWidth*2 +1 , RGB(255, 255, 255));
    HGDIOBJ hOldPen = ::SelectObject(hdc, hPen);

    int iNbPoints = srcShape.PointNum();
    for (int iStartPoint = 0; iStartPoint < iNbPoints; iStartPoint++)
    {
        for (int iEndPoint = iStartPoint + 1; iEndPoint < iNbPoints; iEndPoint++)
        {
            ::MoveToEx(hdc, int(srcShape.PointX(iStartPoint)), int(srcShape.PointY(iStartPoint)), NULL);
            ::LineTo(hdc, int(srcShape.PointX(iEndPoint)), int(srcShape.PointY(iEndPoint)));
        }
    }
    
    ::SelectObject(hdc, hOldPen);
    ::DeleteObject(hPen);

// added by sunlei 2002.10.2 begin: efficiency
	BYTE* pCurRowMask = NULL;
	DWORD* pCurRowTemp = NULL;
// added by sunlei 2002.10.2 end
    for (int iY = 0; iY < height; iY++)
    {
// added by sunlei 2002.10.2 begin: efficiency
		pCurRowMask = maskImg.RowPointer(iY);
		pCurRowTemp = (DWORD*)(tempImg.RowPointer(iY));
// added by sunlei 2002.10.2 end
        int iStartX, iEndX; // the first and last x, Point(x,y) = 1
        iStartX = -1; iEndX = -1;
        for (int iX = 0; iX < width; iX++)
        {
// added by sunlei 2002.10.2 begin: efficiency
/*
            if (tempImg.Pixel(iX, iY).R() != 0)
*/
            if (pCurRowTemp[iX] != 0)
// added by sunlei 2002.10.2 end
            {
                iEndX = iX;
                if (iStartX == -1)
                {
                    iStartX = iX;
                }
            }
        }
        if (iStartX != -1)
        {
            for (iX = iStartX; iX <= iEndX; iX++)
            {
// added by sunlei 2002.10.2 begin: efficiency
/*
                maskImg.Pixel(iX, iY) = 255;
*/
	            pCurRowMask[iX] = 255;
// added by sunlei 2002.10.2 end
            }
        }
    }
}


COutputDataFile& operator<<(COutputDataFile& ofs, const CShape& shape)
{
	if (!ofs.HasError())
	{
		ofs<< shape.PointNum() << sep_endl;
		for (int iPoint = 0; iPoint < shape.PointNum(); iPoint ++ )
		{
			ofs << shape.PointX(iPoint) << shape.PointY(iPoint) << sep_endl;
		}
	}
	return ofs;
}

CInputDataFile& operator>>(CInputDataFile& ifs, CShape& shape)
{
	if (!ifs.HasError())
	{
		int NPoint;
		ifs >> NPoint >> sep_endl;
		shape.Resize(NPoint);

		for (int iPoint = 0; iPoint < shape.PointNum(); iPoint ++ )
		{
			ifs >> shape.PointX(iPoint) >> shape.PointY(iPoint) >> sep_endl;
		}
	}
	return ifs;
}

CVisTransform4x4 GetTrans2DTMatrix(double transx, double transy)
{
	CVisTransform4x4 trans;
	trans[0][3] = transx;
	trans[1][3] = transy;
	return trans;
}

CVisTransform4x4 GetTrans2DRSMatrix(double scale, double theta)
{
	CVisTransform4x4 trans;
	trans[0][0] = trans[1][1] = cos(theta)*scale;
	trans[0][1] = -sin(theta)*scale;
	trans[1][0] = sin(theta)*scale;
	return trans;
}

CVisTransform4x4 GetTrans2DRSMatrix2(double sx, double sy, double theta)
{
	CVisTransform4x4 trans;
	trans[0][0] = cos(theta)*sx;
	trans[1][1] = cos(theta)*sy;
	trans[0][1] = -sin(theta)*sx;
	trans[1][0] = sin(theta)*sy;
	return trans;
}

COutputDataFile& operator<<(COutputDataFile& ofs, const std::vector<CShape>& shape)
{
	ofs << (int)shape.size() << sep_endl;
	for (int i = 0; i< shape.size(); i++)
	{
		ofs << shape[i] << sep_endl;
	}
	return ofs;
}

CInputDataFile& operator>>(CInputDataFile& ifs, std::vector<CShape>& shape)
{
	int nCurve;
	ifs >> nCurve;
	shape.resize(nCurve);

	for (int i = 0; i< nCurve; i++)
	{
		ifs >> shape[i] >> sep_endl;
	}
	return ifs;
}

void CShapeSet::Add(const CShapeSet& ss)
{
	if (ss.empty())
	{
		return;
	}
	for (int i = 0; i< ss.size(); i++)
	{
		push_back(ss[i]);
	}
}

int CShapeSet::PointNum() const
{
	int nPointNum = 0;
	for ( int i=0; i<size(); i++ )
	{
		nPointNum += ((*this)[i]).PointNum();
	}

	return nPointNum;
}

void CShapeSet::AroundRect(double &left, double& top, double& right, double& bottom)
{
    left = top = 10000.0;
	right = bottom = -10000.0;
    
	int nShapeNum = size();

    if (nShapeNum == 0 || (*this)[0].PointNum() == 0)
    {
        return;
    }

    left = right = (*this)[0].PointX(0);
    bottom = top = (*this)[0].PointY(0);

	for (int iShape = 0; iShape < nShapeNum; iShape++)
	{
		for (int iPoint = 0; iPoint < (*this)[iShape].PointNum(); iPoint++)
		{
			if ( left > (*this)[iShape].PointX(iPoint) ) left = (*this)[iShape].PointX(iPoint);
			if ( right < (*this)[iShape].PointX(iPoint) ) right = (*this)[iShape].PointX(iPoint);
			if ( bottom < (*this)[iShape].PointY(iPoint) )bottom = (*this)[iShape].PointY(iPoint);
			if ( top > (*this)[iShape].PointY(iPoint) ) top = (*this)[iShape].PointY(iPoint);
		}
	}
}

void Apply2DRTSTransform(CVisTransform4x4 &transM, const CShapeSet &srcShapeSet, CShapeSet &destShapeSet)
{
	if (srcShapeSet.empty())
	{
		destShapeSet.clear();
	}

	destShapeSet.resize(srcShapeSet.size());

	for (int i = 0; i< srcShapeSet.size(); i++)
	{
		Apply2DRTSTransform(transM, srcShapeSet[i], destShapeSet[i]);
	}
}

void shape2shapeset(const CShape& shape, CShapeSet& ss, const CConnectedModel& conModel)
{
	if ( shape.PointNum()<=0 ) return;

	if ( conModel.PointNum()!=shape.PointNum() )
	{
		ss.resize(1);
		ss[0] = shape;
	}
	else
	{
		ss.resize(conModel.ContourNum());
		for (int iContour=0; iContour<conModel.ContourNum(); iContour++)
		{
			if ( conModel.Contour(iContour).Type()==CONTOUR_CLOSED && conModel.Contour(iContour).PointNum()>=3 )
			{
				ss[iContour].Resize(conModel.Contour(iContour).PointNum()+1);
				for (int iPoint=0; iPoint<conModel.Contour(iContour).PointNum(); iPoint++)
				{
					ss[iContour].SetPoint(iPoint, shape.GetPoint(conModel.Contour(iContour).Index(iPoint)));
				}
				ss[iContour].SetPoint(iPoint, shape.GetPoint(conModel.Contour(iContour).Index(0)));
			}	
			else // CONTOUR_OPEN
			{
				ss[iContour].Resize(conModel.Contour(iContour).PointNum());
				for (int iPoint=0; iPoint<conModel.Contour(iContour).PointNum(); iPoint++)
				{
					ss[iContour].SetPoint(iPoint, shape.GetPoint(conModel.Contour(iContour).Index(iPoint)));
				}
			}
		}
	}
}

void shapeset2shape(const CShapeSet& ss, CShape& shape, CConnectedModel& conModel)
{
	shape.Clear();
	conModel.Clear();
	if ( ss.size()<=0 ) return;

	std::vector<CPoint2> vPoint2;
	
	int index = 0;
	for (int iShape=0; iShape<ss.size(); iShape++)
	{
		int nPointNum = ss[iShape].PointNum();

		if ( nPointNum>=3 && ss[iShape].GetPoint(0)==ss[iShape].GetPoint(nPointNum-1) ) 
		{
			CIndexedContour contour(CONTOUR_CLOSED, nPointNum-1);
			for (int iPoint=0; iPoint<nPointNum-1; iPoint++)
			{
				vPoint2.push_back(ss[iShape].GetPoint(iPoint));
				contour.SetIndex(iPoint, index++);
			}
			conModel.AddNewContour(contour);
		}
		else 
		{
			CIndexedContour contour(CONTOUR_OPEN, nPointNum);
			for (int iPoint=0; iPoint<nPointNum; iPoint++)
			{
				vPoint2.push_back(ss[iShape].GetPoint(iPoint));
				contour.SetIndex(iPoint, index++);
			}
			conModel.AddNewContour(contour);
		}
	}
	vPoint22Shape(vPoint2, shape);
}

void SmoothShape(CShape& shape, CConnectedModel& conModel, double m_dSmooth_Param1 /* = 0.5 */, double m_dSmooth_Param2 /* = 1.0 */)
{
	if ( conModel.PointNum()!=shape.PointNum() || shape.PointNum()<3 ) return;

	CShapeSet ss;
	shape2shapeset(shape, ss, conModel);
	SmoothCurveSet(ss, m_dSmooth_Param1, m_dSmooth_Param2);

	CConnectedModel tempConModel;
	shapeset2shape(ss, shape, tempConModel);

	assert(tempConModel==conModel);
}

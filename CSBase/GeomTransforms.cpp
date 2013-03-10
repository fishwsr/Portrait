#include "stdafx.h"
#include "geomtransforms.h"
//#include "..\BaseModel\BaseModel.h"

void CGeomAffineTrans::SetTranform(const CVisTransform4x4& mat) 
{ 
	m_Mat = mat; 
	m_InvMat = mat; 
	m_InvMat.Invert(); 
}

CGeomThinSplineTrans::CGeomThinSplineTrans()
	:CGeomTransform() , m_theta(100.0)
{
};

float CGeomThinSplineTrans::U(const float x1, const float y1, const float x2, const float y2) const
{
	float sqrdis = (x1 - x2) * (x1 - x2) +
		(y1 - y2) * (y1 - y2);
	float dis = (float)sqrt(sqrdis);

	if (dis < 0.0001) dis = 0.0001f;

	float retval = (float)(sqrdis/m_theta/m_theta*log(dis/m_theta));
	return retval;
}

void CGeomThinSplineTrans::	Init(const CShape& srcShape, const CShape& desShape, const bool fTrans, const bool fInvTrans)
{
	m_SrcShape = srcShape;
	m_DesShape = desShape;

	if (m_SrcShape.PointNum() != m_DesShape.PointNum())
	{
		printf("\n Error: Pointnum const srcshape and desshape must be same");
		return;
	}

	if (fInvTrans)
	{
		CShape& src=m_DesShape;
		CShape& des=m_SrcShape;
		float& theta = m_theta;
		CVisDMatrix& W=m_InvMat;

		// see Cootes' AAM document for details
		int d = 2;				// 2D
		int n = src.PointNum();		// number of points
		int m = n + d + 1;		// 

		CVisDMatrix L(m, m);
		for (int i = 0; i < n; i++)
		{
			L[i][i] = 0;
			for (int j = i + 1; j < n; j ++)
			{
				L[i][j] = L[j][i] = U((const float)src.PointX(i), (const float)src.PointY(i), (const float)src.PointX(j), (const float)src.PointY(j));
			}
			L[i][n] = L[n][i] = 1;
			L[i][n + 1] = L[n + 1][i] = src.PointX(i);
			L[i][n + 2] = L[n + 2][i] = src.PointY(i);
		}
		for (i = n; i < m; i++)
		{
			for (int j = n; j < m; j ++)
			{
				L[i][j] = 0;
			}
		}
		CVisDMatrix Xd(d, m);
		for (i = 0; i < n; i++)
		{
			Xd[0][i] = des.PointX(i);
			Xd[1][i] = des.PointY(i);
		}
		for (i = n; i < m; i++)
		{
			Xd[0][i] = Xd[1][i] = 0;
		}
		W.Resize(d,m);
		//W = Xd * L.Invert();	

		CVisDMatrix LT(m,m);
		LT = L.Transposed();
		CVisDMatrix WT(m,d);
		CVisDMatrix XdT(m,d);
		XdT= Xd.Transposed();

		VisDMatrixLeastSquares(LT,XdT,WT);

		W = WT.Transposed();
		m_fInvTransReady = fInvTrans;
	}
	if (fTrans)
	{
		CShape& des=m_DesShape;
		CShape& src=m_SrcShape;
		float& theta = m_theta;
		CVisDMatrix& W=m_Mat;

		// see Cootes' AAM document for details
		int d = 2;				// 2D
		int n = src.PointNum();		// number of points
		int m = n + d + 1;		// 

		CVisDMatrix L(m, m);
		for (int i = 0; i < n; i++)
		{
			L[i][i] = 0;
			for (int j = i + 1; j < n; j ++)
			{
				L[i][j] = L[j][i] = U((const float)src.PointX(i), (const float)src.PointY(i), (const float)src.PointX(j), (const float)src.PointY(j));
			}
			L[i][n] = L[n][i] = 1;
			L[i][n + 1] = L[n + 1][i] = src.PointX(i);
			L[i][n + 2] = L[n + 2][i] = src.PointY(i);
		}
		for (i = n; i < m; i++)
		{
			for (int j = n; j < m; j ++)
			{
				L[i][j] = 0;
			}
		}
		CVisDMatrix Xd(d, m);
		for (i = 0; i < n; i++)
		{
			Xd[0][i] = des.PointX(i);
			Xd[1][i] = des.PointY(i);
		}
		for (i = n; i < m; i++)
		{
			Xd[0][i] = Xd[1][i] = 0;
		}
		W.Resize(d,m);
		//W = Xd * L.Invert();	

		CVisDMatrix LT(m,m);
		LT = L.Transposed();
		CVisDMatrix WT(m,d);
		CVisDMatrix XdT(m,d);
		XdT= Xd.Transposed();

		VisDMatrixLeastSquares(LT,XdT,WT);

		W = WT.Transposed();
		m_fTransReady = fTrans;
	}	
}

bool CGeomThinSplineTrans::Transform(const CPoint2 inPt,  CPoint2& outPt) const
{
	const CShape& src= m_SrcShape;
	const CVisDMatrix& W = m_Mat;

	int d = 2;				// 2D
	int n = src.PointNum();		// number of points
	int m = n + d + 1;		// 

	CVisDVector ud(m);
	for (int i = 0; i < n; i++)
	{
		ud[i] = U((const float)src.PointX(i), (const float)src.PointY(i), inPt.X , inPt.Y);
	}
	ud[n] = 1;
	ud[n + 1] = inPt.X;
	ud[n + 2] = inPt.Y;

	CVisDVector vout = W * ud;

	outPt.X = (Gdiplus::REAL)vout[0];
	outPt.Y = (Gdiplus::REAL)vout[1];

	return true;
}


bool CGeomThinSplineTrans::InvTransform(const CPoint2 inPt,  CPoint2& outPt) const
{
	const CShape& src= m_DesShape;
	const CVisDMatrix& W = m_InvMat;

	int d = 2;				// 2D
	int n = src.PointNum();		// number of points
	int m = n + d + 1;		// 

	CVisDVector ud(m);
	for (int i = 0; i < n; i++)
	{
		ud[i] = U((const float)src.PointX(i), (const float)src.PointY(i), inPt.X , inPt.Y);
	}
	ud[n] = 1;
	ud[n + 1] = inPt.X ;
	ud[n + 2] = inPt.Y ;

	CVisDVector vout = W * ud;

	outPt.X = (Gdiplus::REAL)vout[0];
	outPt.Y = (Gdiplus::REAL)vout[1];

	return true;
}


CGeomTransform * CGeomThinSplineTrans::Clone() const 
{
	CGeomThinSplineTrans * pNewTrans = new CGeomThinSplineTrans();

	pNewTrans->m_theta = m_theta;
	pNewTrans->m_fTransReady = m_fTransReady;
	pNewTrans->m_fInvTransReady = m_fInvTransReady;

	pNewTrans->m_SrcShape = m_SrcShape;
	pNewTrans->m_DesShape = m_DesShape;

	pNewTrans->m_Mat = m_Mat;
	pNewTrans->m_InvMat = m_InvMat;
	return (CGeomTransform *)pNewTrans;
}

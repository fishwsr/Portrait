
#pragma once

#include "GeomTransform.h"

class CSBASE_API CGeomAffineTrans : public CGeomTransform
{
protected:
	CVisTransform4x4 m_Mat;
	CVisTransform4x4 m_InvMat;
public:
	CGeomAffineTrans();
	virtual ~CGeomAffineTrans();

	// Init method
	void SetTranform(const CVisTransform4x4& mat);
		
	const CVisTransform4x4& TransformMat() const;
	const CVisTransform4x4& InvTransformMat() const;

	virtual bool Transform(const CPoint2 inPt,  CPoint2& outPt) const ;
	virtual bool InvTransform(const CPoint2 inPt,  CPoint2& outPt) const;
	
	virtual CGeomTransform * Clone() const;
};

class CSBASE_API CGeomThinSplineTrans : public CGeomTransform
{
protected:
	float m_theta;
	CShape m_SrcShape;
	CShape m_DesShape;

	CVisDMatrix m_Mat;
	CVisDMatrix m_InvMat;

	float U(const float x1, const float y1, const float x2, const float y2) const;
public:
	CGeomThinSplineTrans();
	virtual ~CGeomThinSplineTrans();

	// Init method
	void Init(const CShape& srcShape, const CShape& desShape, const bool fTrans = true, const bool fInvTrans = true);

	virtual bool Transform(const CPoint2 inPt,  CPoint2& outPt) const ;
	virtual bool InvTransform(const CPoint2 inPt,  CPoint2& outPt) const;

	virtual CGeomTransform * Clone() const;
};


//************************************************************************
//*																		 *
//*                           inline functions							 *
//*																		 *
//************************************************************************
inline CGeomAffineTrans::CGeomAffineTrans()
	:CGeomTransform(true,true)
{
}

inline CGeomAffineTrans::~CGeomAffineTrans()
{
};

inline const CVisTransform4x4& CGeomAffineTrans::TransformMat()	const
{
	return m_Mat;
};

inline const CVisTransform4x4& CGeomAffineTrans::InvTransformMat() const
{
	return m_InvMat;
}

inline CGeomTransform * CGeomAffineTrans::Clone() const
{
	CGeomAffineTrans * pDes = new CGeomAffineTrans; 
	((CGeomAffineTrans *)pDes)->m_Mat = m_Mat; 
	((CGeomAffineTrans *)pDes)->m_InvMat = m_InvMat;
	return (CGeomTransform *) pDes;
};

inline bool CGeomAffineTrans::Transform(const CPoint2 inPt,  CPoint2& outPt) const
{ 
	CPoint2 tempPt;
	tempPt.X = (Gdiplus::REAL)(inPt.X * m_Mat[0][0]+ inPt.Y * m_Mat[0][1] + m_Mat[0][3]);
	tempPt.Y = (Gdiplus::REAL)(inPt.X * m_Mat[1][0]+ inPt.Y * m_Mat[1][1] + m_Mat[1][3]);	
	outPt = tempPt;
	return true;
};
	
inline bool CGeomAffineTrans::InvTransform(const CPoint2 inPt,  CPoint2& outPt) const
{
	CPoint2 tempPt;
	tempPt.X = (Gdiplus::REAL)(inPt.X * m_InvMat[0][0]+ inPt.Y * m_InvMat[0][1] + m_InvMat[0][3]);
	tempPt.Y = (Gdiplus::REAL)(inPt.X * m_InvMat[1][0]+ inPt.Y * m_InvMat[1][1] + m_InvMat[1][3]);	
	outPt = tempPt;
	return true;
};

inline CGeomThinSplineTrans::~CGeomThinSplineTrans()
{
};

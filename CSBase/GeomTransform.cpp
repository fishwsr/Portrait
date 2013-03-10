#pragma warning(disable : 4786)

#include "stdafx.h"
#include "geomtransform.h"

CGeomTransSequence::CGeomTransSequence(CGeomTransform * tran1, CGeomTransform * tran2) 
  : CGeomTransform( (tran1->TransReady())&&(tran2->TransReady()), (tran1->InvTransReady())&&(tran2->InvTransReady())) 
{
	m_pTrans1 = tran1; 
	m_pTrans2 = tran2;
};

void CGeomTransform::Testing() const
{
	CPoint2 pt1,pt2;

	for (int iStep = 0; iStep<4; iStep++)
	{
		switch(iStep)
		{
		case 0:
			pt1 = CPoint2(0.1f, 0.1f);
			break;
		case 1:
			pt1 = CPoint2(0.1f, 0.9f);
			break;	
		case 2:
			pt1 = CPoint2(0.9f, 0.1f);
			break;
		case 3:
			pt1 = CPoint2(0.9f, 0.9f);
			break;
		}
		Transform(pt1,pt2);
		InvTransform(pt2,pt2);
		printf("\n IN: %f %f OUT: %f %f", pt1.X, pt1.Y, pt2.X, pt2.Y);
		//std::cout<<std::endl<<"IN:"<<pt1.X<<' '<<pt1.Y<<' '<<"OUT:"<<pt2.X<<' '<<pt2.Y;
	}
}

void ApplyTransform(const CShape& orgShape, CShape& desShape, const CGeomTransform& geomTrans, const bool fNotInv)
{
	desShape.Resize(orgShape.PointNum());

	for (int iPoint = 0; iPoint < orgShape.PointNum() ; iPoint++ )
	{
		CPoint2 retP;
		if (fNotInv)
		{
			geomTrans.Transform(CPoint2(orgShape.PointX(iPoint), orgShape.PointY(iPoint)), retP);
		}
		else
		{
			geomTrans.InvTransform(CPoint2(orgShape.PointX(iPoint), orgShape.PointY(iPoint)), retP);
		}

		desShape.PointX(iPoint) = retP.X;
		desShape.PointY(iPoint) = retP.Y;
	}
}

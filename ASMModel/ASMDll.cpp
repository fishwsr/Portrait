#include "stdafx.h"
#include "ASMDll.h"
#include "../CSBase/CSBase.h"
#include "ASMSearchOption.h"
#include "ASMMLSearchModelBayesian.h"

int myadd(int x, int y)
{
	return x + y;
}

/*
ASMSearchOption::ASMSearchOption()
{
	m_fbUsedConstraint = false;
	m_ConstrainedShape.clear();
	m_vbConstrained.clear();
}

ASMSearchOption::~ASMSearchOption()
{
	m_ConstrainedShape.clear();
	m_vbConstrained.clear();
}

void ASMSearchOption::SetConstrainedShape(std::vector<CPoint2> &vPoint2)
{
	m_ConstrainedShape.resize(vPoint2.size());

	for ( int i=0; i<vPoint2.size(); i++ )
	{
		m_ConstrainedShape[i] = vPoint2[i];
	}
}

void ASMSearchOption::SetConstrainedPoints(std::vector<bool> &vbConstrained)
{
	m_vbConstrained.resize(vbConstrained.size());

	for ( int i=0; i<vbConstrained.size(); i++ )
	{
		m_vbConstrained[i] = vbConstrained[i];
	}
}
*/

int gAsmSearch1(const char* strImageFileName, const char* strModelFileName, std::vector<CPoint2> &vPoint2, bool fbUseConstraint, std::vector<CPoint2>* pConstrainedShape, std::vector<bool>* pvbConstrained)
{
	return 100;

	// load image
	CVisRGBAByteImage image;
	image.ReadFile(strImageFileName);
	if ( image.Width()<=0 || image.Height()<=0 ) return 1; // load image fail

	// load model
	CASMMLSearchModelBayesian asmmodel;
	LoadFromFile(strModelFileName, asmmodel);
	CShape avgShape;
	asmmodel.SetAvgShape();
	asmmodel.GetCurShape(avgShape);
	if ( avgShape.PointNum()<=0 ) return 2; // load model fail

	// load current shape
	if ( vPoint2.size() != avgShape.PointNum() ) return 3; // input not compatible with model
	CShape searchShape;
	vPoint22Shape(vPoint2, searchShape);

	// search
	CShape constrainShape;
	std::vector<bool> vbConstraint;
	asmmodel.m_SearchMode = 0;
//	if ( fbUseConstraint )
	{
		if ( NULL!=pConstrainedShape && pConstrainedShape->size()==avgShape.PointNum() )
		{
			vPoint22Shape(*pConstrainedShape, constrainShape);
		}
		else
		{
			constrainShape = avgShape;
		}

		vbConstraint.resize(avgShape.PointNum());
		if ( NULL!=pvbConstrained && pvbConstrained->size()==avgShape.PointNum() )
		{
			vbConstraint = *pvbConstrained;
		}
		else
		{
			for ( int i=0; i<vbConstraint.size(); i++ )
			{
				vbConstraint[i] = false;
			}
		}
		
		asmmodel.SetConstrain(4.0, vbConstraint, constrainShape);
	}
	CVisGrayByteImage searchImage = VisBrightnessFromRGBA(image);
	asmmodel.SetSearchImage(&searchImage);
	asmmodel.SearchIterative(3);

	searchShape = asmmodel.SearchResult();
	Shape2vPoint2(searchShape, vPoint2);

	return 0; // succeed
}

int gAsmSearch2(ASMSearchOption &option, std::vector<CPoint2> &vPoint2)
{
	return gAsmSearch1( option.m_strImageFileName, option.m_strModelFileName, 
					   vPoint2, option.m_fbUseConstraint, 
					   &(option.m_ConstrainedShape), &(option.m_vbConstrained) );

}

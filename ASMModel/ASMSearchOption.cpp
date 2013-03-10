#include "stdafx.h"
#include "ASMSearchOption.h"

ASMSearchOption::ASMSearchOption()
{
	m_fbUseConstraint = false;
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

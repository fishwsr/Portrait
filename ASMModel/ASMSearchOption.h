#ifndef _ASMSEARCHOPTION_H_
#define _ASMSEARCHOPTION_H_

//#include "ASMMLSearchModelBayesian.h"

class ASMMODEL_API ASMSearchOption
{
public:
	ASMSearchOption();
	~ASMSearchOption();

	void SetConstrainedShape(std::vector<CPoint2> &vPoint2);
	void SetConstrainedPoints(std::vector<bool> &vbConstrained);

	bool m_fbUseConstraint;
	char m_strImageFileName[256];
	char m_strModelFileName[256];

	std::vector<CPoint2> m_ConstrainedShape;
	std::vector<bool> m_vbConstrained; 
};

#endif

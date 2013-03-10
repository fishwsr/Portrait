
// ConnectedModel.h: interface for the CConnectedModel class.
//
///////////////////////////////////////////////////////////////////////////


#ifndef __CSBASE_CONNECTEDMODEL_H__
#define __CSBASE_CONNECTEDMODEL_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "IndexedContour.h"

class CSBASE_API CConnectedModel
{
public:
    CConnectedModel(void);
	CConnectedModel(const CConnectedModel& ref);
    virtual ~CConnectedModel(void);
	friend COutputDataFile& Output(COutputDataFile& ifs, const CConnectedModel& con);
	friend CInputDataFile& Input(CInputDataFile& ifs, CConnectedModel& con);

public:

    int PointNum(void) const { return m_PointNum; }
    int ContourNum(void) const { return (int)m_vContours.size(); }
    const CIndexedContour& Contour(int i) const{ return m_vContours[i]; }
	CIndexedContour& Contour(int i) { return m_vContours[i]; }

	void AddNewContour(const CIndexedContour& newContour){ m_PointNum += newContour.PointNum(); m_vContours.push_back(newContour);};

	const CConnectedModel& operator=(const CConnectedModel& ref)
	{
		if (this != &ref )
		{
			m_PointNum = ref.m_PointNum;
			m_vContours.clear();
			m_vContours = ref.m_vContours;
		}
		return *this;
	};

	bool operator==(const CConnectedModel& ref) const;

//protected:
    void Clear(void);

protected:
    int m_PointNum;
    std::vector<CIndexedContour> m_vContours;
};

CSBASE_API void GetShapeNormal(const CConnectedModel& conmodel, const CShape& orgShape, CShape& normalShape);
CSBASE_API void GetCurveNormal(const CShape& orgShape, CShape& normalShape, bool bClosed = false);


CSBASE_API COutputDataFile& operator<<(COutputDataFile& ifs, const CConnectedModel& con);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, CConnectedModel& con);

#endif // __ASMMODEL_CONNECTEDMODEL_H__

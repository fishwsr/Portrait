
// IndexedContour.h: interface for the CIndexedContour class.
//
///////////////////////////////////////////////////////////////////////////


#ifndef __CSBASE_INDEXEDCONTOUR_H__
#define __CSBASE_INDEXEDCONTOUR_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#define CONTOUR_OPEN        0
#define CONTOUR_CLOSED      1

class CSBASE_API CIndexedContour 
{
public:
    CIndexedContour(void);
	CIndexedContour(const CIndexedContour& ref);
	const CIndexedContour& operator =(const CIndexedContour& ref); 
	bool operator==(const CIndexedContour& ref) const;

	virtual ~CIndexedContour(void);
	CIndexedContour(const int type, const int PointNum);

	friend COutputDataFile& Output(COutputDataFile& ifs, const CIndexedContour& c);
	friend CInputDataFile& Input(CInputDataFile& ifs, CIndexedContour& c);

public:
    int Type(void) const { return m_Type; }
    int PointNum(void) const { return m_PointNum; }

	short& Index(int i) const { return m_Index[i]; }
	void SetIndex(int i, short index) {m_Index[i] = index;};
//protected:
    void Clear(void);

protected:
    int m_Type; // CONTOUR_OPEN or CONTOUR_CLOSED
    int m_PointNum;
    short* m_Index;
};

CSBASE_API COutputDataFile& operator<<(COutputDataFile& fs, const CIndexedContour& c);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& fs, CIndexedContour& c);


#endif // __ASMMODEL_INDEXEDCONTOUR_H__

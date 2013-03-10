
// PCAModel.h: interface for the CPCAModel class.
//
///////////////////////////////////////////////////////////////////////////

#ifndef __BASEMODEL_PCAMODEL_H__
#define __BASEMODEL_PCAMODEL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "Distribution.h"
#include "../CSBase/CSBase.h"

class CPCAModelBuilder;

class BASEMODEL_API CPCAModel
{
public:

    CPCAModel(void);
    virtual ~CPCAModel(void);
	void Clear();

    void Generate(CVisDVector& data, const CVisDVector& param) const;
    void GetEigenParam(const CVisDVector& data, CVisDVector& param) const;

	const int IsReady() const { return m_fbReady; }
    const int DataDimension(void) const { return m_iDataDimension; }
    const int NbEigen(void) const { return m_iNbEigen; }
    const double& EigenVal(int index) const { return m_EigenVal[index]; }
	const CVisDVector& AvgData() const {return m_AvgData;};
	const CVisDVector& EigenVal() const {return m_EigenVal;};
	const CVisDMatrix& EigenVec() const {return m_EigenVec;};
	
	double LogP(const CVisDVector& data) const;
	double LogP2(const CVisDVector& projectval) const;

	friend COutputDataFile& Output(COutputDataFile& ofs, const CPCAModel& model);
	friend CInputDataFile& Input(CInputDataFile& ifs, CPCAModel& model);
	friend class CPCAModelBuilder;
		
protected:
    // unchanged data
    int m_iDataDimension;

    // unchanged data
    int m_iNbEigen;
    CVisDVector m_AvgData;
    CVisDVector m_EigenVal, m_InvEigenVal;
    CVisDMatrix m_EigenVec;

	bool m_fbReady;
};

class BASEMODEL_API CPCAModelSampler
{
public:
	CPCAModelSampler(const CPCAModel& model);
	void GetOneSample(CVisDVector& data, int method = 0);
	void GetOneSampleAndParam(CVisDVector& data, CVisDVector& param, int method = 0);

protected:
	const CPCAModel& m_ModelRef;
};

BASEMODEL_API COutputDataFile& operator<<(COutputDataFile& ofs, const CPCAModel& model);
BASEMODEL_API CInputDataFile& operator>>(CInputDataFile& ifs, CPCAModel& model);

#endif // __BASEMODEL_PCAMODEL_H__

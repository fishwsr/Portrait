
// PCA2Model.h: interface for the CPCA2Model class.
//
///////////////////////////////////////////////////////////////////////////


#ifndef __BASEMODEL_PCA2MODEL_H__
#define __BASEMODEL_PCA2MODEL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "PCAModel.h"

class CPCA2ModelBuilder;

class BASEMODEL_API CPCA2Model : public CPCAModel
{
public:
	CPCA2Model(void);
	virtual ~CPCA2Model(void);
	friend class CPCA2ModelBuilder;
	friend COutputDataFile& Output(COutputDataFile& fs, const CPCA2Model& model);
	friend CInputDataFile& Input(CInputDataFile& fs, CPCA2Model& model);

protected:
    // unchanged data
	double m_ErrS, m_InvErrS;

public:
	double EnergyLessThan(CVisDVector& data, double MaxEnergy);

	//modified by lin begin, 2003-3
	double LogP(const CVisDVector& data, double MinLogP=-DBL_MAX) const;
	//modified by lin end, 2003-3
	
	//Add by Hchen 2003-3-10 Begin
	double ErrS()const {return m_ErrS;};
	//Add by Hchen 2003-3-10 End

};

class BASEMODEL_API CPCA2ModelSampler : CPCAModelSampler
{
public:
	CPCA2ModelSampler(const CPCA2Model& model);
	void GetOneSample(CVisDVector& data, int method = 0);
	void GetOneSampleAndParam(CVisDVector& data, CVisDVector& param, int method = 0);
};

BASEMODEL_API COutputDataFile& operator<<(COutputDataFile& ofs, const CPCA2Model& model);
BASEMODEL_API CInputDataFile& operator>>(CInputDataFile& ifs, CPCA2Model& model);

#endif // __BASEMODEL_PCA2MODEL_H__

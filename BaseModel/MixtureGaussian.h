// MixtureGaussian.h: interface for the CMixtureGaussian class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIXTUREGAUSSIAN_H__5247A926_752B_4CF4_915A_8210EA900CD2__INCLUDED_)
#define AFX_MIXTUREGAUSSIAN_H__5247A926_752B_4CF4_915A_8210EA900CD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GaussianModel.h"

class CMixtureGaussianBuilder;
class CMixtureGaussianSampler;
class BASEMODEL_API CMixtureGaussian : public CDistribution 
{
public:
	friend class CMixtureGaussianBuilder;
	friend class CMixtureGaussianSampler;

	friend COutputDataFile& Output(COutputDataFile& ofs, const CMixtureGaussian& model);
	friend CInputDataFile& Input(CInputDataFile& ifs, CMixtureGaussian& model);

	CMixtureGaussian();
	virtual ~CMixtureGaussian();

	int NGaussian() const { return m_vGaussian.size(); }
	const CVisDVector& AvgData(int iGaussian) const { assert(iGaussian>=0 && iGaussian<m_vGaussian.size()); return m_vGaussian[iGaussian].AvgData(); }
	void GetCovMatrix(int iGaussian, CVisDMatrix& CovMat);

	virtual double LogP(const CVisDVector& data) const;
	virtual double LogPWithBias(const CVisDVector& data) const {return LogP(data);};

	double JointLogP(const CVisDVector& data, int j) const;
	void CalcPj(const CVisDVector& data, CVisDVector& vPj) const;

	// Inference 
	// j* = argmax P(j|data) = argmax P(data|j)P(j)
	// return P(data,j*)
	double MAPInference(const CVisDVector& data, int j) const;
	const CVisDVector& MixingWeight() const { return m_vMixingWeight; } ;

protected:
	int m_nGaussian;
	CVisDVector m_vMixingWeight;
	std::vector<CGaussianModel> m_vGaussian;
};

BASEMODEL_API COutputDataFile& operator<<(COutputDataFile& ofs, const CMixtureGaussian& model);
BASEMODEL_API CInputDataFile& operator>>(CInputDataFile& ifs, CMixtureGaussian& model);

class BASEMODEL_API CMixtureGaussianSampler : public CSampler
{
public:
	CMixtureGaussianSampler(const CMixtureGaussian& model);
	virtual ~CMixtureGaussianSampler();
	virtual void GetOneSample(CVisDVector& data, int method = 0);
protected:
	std::vector<CGaussianSampler *> m_vpGaussianSampler; 
};

#endif // !defined(AFX_MIXTUREGAUSSIAN_H__5247A926_752B_4CF4_915A_8210EA900CD2__INCLUDED_)

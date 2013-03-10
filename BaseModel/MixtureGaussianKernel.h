// MixtureGaussianKernel.h: interface for the CMixtureGaussianKernel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIXTUREGAUSSIANKERNEL_H__F4B23226_0923_4AF7_8614_4168AD61FB0E__INCLUDED_)
#define AFX_MIXTUREGAUSSIANKERNEL_H__F4B23226_0923_4AF7_8614_4168AD61FB0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GaussianModel.h"
#include "MixtureModel.h"

class CMixtureGKernelSampler;

class BASEMODEL_API CMixtureGaussianKernel : public CMixtureModel<CGaussianKernel> 
{
public:
	friend class CMixtureGKernelSampler;

	CMixtureGaussianKernel();
	virtual ~CMixtureGaussianKernel();

	virtual double JointLogP(const CVisDVector& data, int j ) const;
	double MAPInference(const CVisDVector& data, int& j ) const;

	void Init(const CVisDMatrix& vCentralData, double Sigma);
	void InitAdaptive(const CVisDMatrix& vCentralData, int nWithin2Sigma);
	const CMixtureGaussianKernel& operator=(const CMixtureGaussianKernel& ref);

	const CVisDMatrix& vData() const{return m_vData;};

	friend COutputDataFile& Output(COutputDataFile& ofs, const CMixtureGaussianKernel& model);
	friend CInputDataFile& Input(CInputDataFile& ifs, CMixtureGaussianKernel& model);
protected:
	CVisDMatrix m_vData;
};

BASEMODEL_API COutputDataFile& operator<<(COutputDataFile& ofs, const CMixtureGaussianKernel& model);
BASEMODEL_API CInputDataFile& operator>>(CInputDataFile& ifs, CMixtureGaussianKernel& model);

class BASEMODEL_API CMixtureGKernelSampler : public CSampler
{
public:
	CMixtureGKernelSampler(const CMixtureGaussianKernel& model);
	virtual ~CMixtureGKernelSampler();

	virtual void GetOneSample(CVisDVector& data, int method = 0);
	void GetOneSampleAndParam(CVisDVector& data, int& j, int method = 0);

protected:
	std::vector<CGuassianKernelSampler *> m_vpClusterSampler; 
};

// the mixingweight of m_MixtureGKernelX and m_MixtureGKernelY will not be used 
class BASEMODEL_API CJointMixtureGKernel
{
public:
	void Init(const CVisDMatrix& vDataX, double sigmaX, const CVisDMatrix& vDataY, double sigmaY);
	void InitAdaptive(const CVisDMatrix& vDataX, const CVisDMatrix& vDataY, int nWithin2Sigma);

	void LogPKernelGivenY(const CVisDVector& dataY, CVisDVector& LogP) const;
	void LogPKernelGivenX(const CVisDVector& dataX, CVisDVector& LogP) const;
	void LogPKernelGivenXY(const CVisDVector& dataX, const CVisDVector& dataY, CVisDVector& LogP) const;

	double LogP(const CVisDVector& dataX, const CVisDVector& dataY) const;

	//int nKernel() const;
	//const CVisDMatrix& vDataX() const {return m_vDataX; };
	//const CVisDMatrix& vDataY() const {return m_vDataY; };
protected:
	//CVisDMatrix m_vDataX;
	//CVisDMatrix m_vDataY;
	//double m_SigmaX, m_SigmaY;
	CMixtureGaussianKernel m_MixtureGKernelX;
	CMixtureGaussianKernel m_MixtureGKernelY;
};

#endif // !defined(AFX_MIXTUREGAUSSIANKERNEL_H__F4B23226_0923_4AF7_8614_4168AD61FB0E__INCLUDED_)

// GaussianModel.h: interface for the CGaussianModel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAUSSIANMODEL_H__97E11EA7_6AAF_433C_9CAD_CEA0F36639C1__INCLUDED_)
#define AFX_GAUSSIANMODEL_H__97E11EA7_6AAF_433C_9CAD_CEA0F36639C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Distribution.h"

class CGaussianBuilder;
class CSGaussianSampler;
class CSGaussianBuilder;

class BASEMODEL_API CSGaussian : public CSDistribution
{
public:
	friend COutputDataFile& Output(COutputDataFile& ofs, const CSGaussian& model);
	friend CInputDataFile& Input(CInputDataFile& ifs, CSGaussian& model);

	CSGaussian(double mean=0., double sigma2=1.);
	void SetModel(double mean=0., double sigma2=1.);
	void SetModel(std::vector<double>& vdata);

	double Mean() const { return m_mean; }
	double Var() const { return m_sigma2; }

	static double LogGaussian(double mean, double sigma2, double data);
	virtual double LogP(double data) const ;
	virtual double LogPWithBias(double data) const ;
	
	friend class CSGaussianBuilder;
	friend class CSGaussianSampler;
protected:
	double m_mean;
	double m_sigma2;
};

BASEMODEL_API COutputDataFile& operator<<(COutputDataFile& ofs, const CSGaussian& model);
BASEMODEL_API CInputDataFile& operator>>(CInputDataFile& ifs, CSGaussian& model);

class BASEMODEL_API CSGaussianSampler : public CSSampler
{
public:
	CSGaussianSampler(const CSGaussian& model) : CSSampler(model) { m_sigma = sqrt(model.m_sigma2); };
	virtual double GetOneSample(int method = 0);
protected:
	double m_sigma;
};

class CGuassianKernelSampler;
class BASEMODEL_API CGaussianKernel : public CDistribution
{
public:
	friend class CGuassianKernelSampler;

	virtual double LogP(const CVisDVector& data) const;
	virtual double LogPWithBias(const CVisDVector& data) const;
	void SetModel(int nDimension, const CVisDVector& AvgData, double sigma);

	friend COutputDataFile& Output(COutputDataFile& ofs, const CGaussianKernel& model);
	friend CInputDataFile& Input(CInputDataFile& ifs, CGaussianKernel& model);

	const CVisDVector& MeanData() const {return m_AvgData;};
	double Sigma() const {return m_sigma;};

protected:
	CVisDVector m_AvgData;
	double m_sigma;
};

BASEMODEL_API COutputDataFile& operator<<(COutputDataFile& ofs, const CGaussianKernel& model);
BASEMODEL_API CInputDataFile& operator>>(CInputDataFile& ifs, CGaussianKernel& model);

class BASEMODEL_API CGuassianKernelSampler : public CSampler
{
public:
	CGuassianKernelSampler(const CGaussianKernel& model);
	virtual void GetOneSample(CVisDVector& data, int method = 0);
};

class CGaussianSampler;
class BASEMODEL_API CGaussianModel : public CDistribution 
{
public:
	friend class CGaussianBuilder;
	friend class CGaussianSampler;
	
	CGaussianModel();
	virtual ~CGaussianModel();

	virtual double LogP(const CVisDVector& data) const;
	virtual double LogPWithBias(const CVisDVector& data) const;
	void SetModel(int nDimension, const CVisDVector& AvgData, const CVisDMatrix& CovMat);

	friend COutputDataFile& Output(COutputDataFile& ofs, const CGaussianModel& model);
	friend CInputDataFile& Input(CInputDataFile& ifs, CGaussianModel& model);
	
	void GetCovMatrix(CVisDMatrix& CovMat);
	const CVisDVector& AvgData() const;

protected:
	void InitParametersWithCovMat(const CVisDMatrix& CovMat);

	// Basic parameters
	CVisDVector m_AvgData;
	CVisDMatrix m_RotateMatrix;
	CVisDVector m_RotateVar2;
	
	// other parameters
	//CVisDMatrix m_CovMat;
	//CVisDMatrix m_InvCovMat;
	//double m_CovMatDeterminant;
};

BASEMODEL_API COutputDataFile& operator<<(COutputDataFile& ofs, const CGaussianModel& model);
BASEMODEL_API CInputDataFile& operator>>(CInputDataFile& ifs, CGaussianModel& model);

class BASEMODEL_API CJointGaussianModel : public CJointDistribution
{
public:
	
	friend COutputDataFile& Output(COutputDataFile& ofs, const CJointGaussianModel& model);
	friend CInputDataFile& Input(CInputDataFile& ifs, CJointGaussianModel& model);

	CGaussianModel& GetMarginalX();
	CGaussianModel& GetMarginalY();

	void GetConditionalX(CGaussianModel& xconditionalmodel, const CVisDVector& y);
	void GetConditionalY(CGaussianModel& yconditionalmodel, const CVisDVector& x);

	virtual double LogP(const CVisDVector& data) const;
	
	void SetModel(const CVisDMatrix& dataMatX, const CVisDMatrix& dataMatY);
	void SetModel(int nDimensionX, int nDimensionY, const CVisDVector& mean, const CVisDMatrix& CovMat);

protected:
	
	void InitByJointModel();
	
	CGaussianModel m_JointModel;

	// for speeding up
	// only compute once after built or loaded
	CVisDVector m_MeanX;
	CVisDVector m_MeanY;
	CVisDMatrix m_CovXX;
	CVisDMatrix m_CovXY;
	CVisDMatrix m_CovYX;
	CVisDMatrix m_CovYY;

	CGaussianModel m_MarginalX;
	CGaussianModel m_MarginalY;
};

BASEMODEL_API COutputDataFile& operator<<(COutputDataFile& ofs, const CJointGaussianModel& model);
BASEMODEL_API CInputDataFile& operator>>(CInputDataFile& ifs, CJointGaussianModel& model);

class BASEMODEL_API CGaussianSampler : public CSampler
{
public:
	CGaussianSampler(const CGaussianModel& model);
	virtual void GetOneSample(CVisDVector& data, int method = 0);
protected:
	CVisDMatrix m_InvRotateMatrix;
	CVisDVector m_RotateVar;
};


#endif // !defined(AFX_GAUSSIANMODEL_H__97E11EA7_6AAF_433C_9CAD_CEA0F36639C1__INCLUDED_)

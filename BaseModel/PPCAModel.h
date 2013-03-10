// PPCAModel.h: interface for the CPPCAModel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PPCAMODEL_H__9C740869_061B_409B_96D2_93779E8BAB69__INCLUDED_)
#define AFX_PPCAMODEL_H__9C740869_061B_409B_96D2_93779E8BAB69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Distribution.h"
#include "GaussianModel.h"

class CPPCAModelBuilder;
class CPPCAModelSampler;

class BASEMODEL_API CPPCAModel : public CGaussianModel  
{
public:
	friend class CPPCAModelBuilder;
	friend class CPPCAModelSampler;

	CPPCAModel(int LogPCalcMode = 1 );
	virtual ~CPPCAModel();
	
	const CVisDVector& Mean() const {return m_AvgData;};
	int NEigen() const {return m_nEigen;}; 

	void GetParam(const CVisDVector& data, CVisDVector& param) const;
	void Reconstruct(const CVisDVector& param, CVisDVector& data) const;

	// zijian
	void Reconstruct(const CVisDVector& param, CVisDVector& data, double dPercent) const;
	void Reconstruct(const CVisDVector& param, CVisDVector& data, int nEigen) const;
	const CVisDVector& EigenVal() const {return m_EigenVal;};
	const CVisDMatrix& EigenVec() const {return m_EigenVec;};

	virtual double LogP(const CVisDVector& data) const;

	double PriorLogP(const CVisDVector& param) const;
	double LikelihoodLogP(const CVisDVector& data, const CVisDVector& param) const;
	double PostLogP(const CVisDVector& data, const CVisDVector& param) const;
	double JointLogP(const CVisDVector& data, const CVisDVector& param) const;
	
	// Inference 
	// param* = argmax P(param|data) = argmax P(data|param)P(param)
	// return P(data,param*)
	double MAPInference(const CVisDVector& data, CVisDVector& param) const;

	friend COutputDataFile& Output(COutputDataFile& ofs, const CPPCAModel& model);
	friend CInputDataFile& Input(CInputDataFile& ifs, CPPCAModel& model);

	double Sigma2() const {return m_Xigma2;};
	void GetCovMatrix(CVisDMatrix& CovMat);

	const CPPCAModel& operator=(const CPPCAModel& refModel);
	CPPCAModel(const CPPCAModel& refModel);

	double CodingLengthParameter(double const_quanti = 0);
	double DictionarySize();

	void GetSimpleModelByPercent(const CPPCAModel& srcModel, double p);


protected: 
	// 0: Gaussian Mode  1: Using MAP solution
	int m_LogPCalcMode; 
	CGaussianModel m_PosteriorModel;
	double m_PosteriorConstant;

	//void GetParam_PCA(const CVisDVector& data, CVisDVector& param);
	//void Reconstruct_PCA(const CVisDVector& param, CVisDVector& data);
	//void GetParam_PostMean(const CVisDVector& data, CVisDVector& param);
	//void Reconstruct_PostMean(const CVisDVector& param, CVisDVector& data);

	// After set the basic parameters, call this to get other parameters
	void InitParametersWithBasicParameters();

	// Build parameter
	int m_nEigen;
	// Basic parameters
	double m_Xigma2;
	CVisDVector m_EigenVal;
	CVisDMatrix m_EigenVec;

	// Other parameters 
	CVisDMatrix m_W;
	CVisDMatrix m_ProjectMatrix;
	CVisDMatrix m_ReconstructMatrix;
};

class BASEMODEL_API CPPCAModelSampler : public CSampler
{
public:
	CPPCAModelSampler(const CPPCAModel& model);
	virtual void GetOneSample(CVisDVector& data, int method = 0);
	void GetOneSampleAndParam(CVisDVector& data, CVisDVector& param, int method = 0);
};


BASEMODEL_API COutputDataFile& operator<<(COutputDataFile& ofs, const CPPCAModel& model);
BASEMODEL_API CInputDataFile& operator>>(CInputDataFile& ifs, CPPCAModel& model);

#endif // !defined(AFX_PPCAMODEL_H__9C740869_061B_409B_96D2_93779E8BAB69__INCLUDED_)

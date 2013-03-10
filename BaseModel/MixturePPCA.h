// MixturePPCA.h: interface for the CMixturePPCA class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable : 4275)

#if !defined(AFX_MIXTUREPPCA_H__6053863F_DB49_498B_8F83_DEA80DBF48E5__INCLUDED_)
#define AFX_MIXTUREPPCA_H__6053863F_DB49_498B_8F83_DEA80DBF48E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ppcamodel.h"
#include "MixtureModel.h"

class CMixturePPCABuilder;
class CMixturePPCASampler;

class BASEMODEL_API CMixturePPCAParam
{
public:
	CMixturePPCAParam();
	const CMixturePPCAParam& operator=(const CMixturePPCAParam& ref);

	int m_iCluster;
	CVisDVector m_EigenParam;
};

class BASEMODEL_API CMixturePPCA : public CMixtureModel<CPPCAModel> 
{
public:
	friend class CMixturePPCABuilder;
	friend class CMixturePPCASampler;

	double JointLogP(const CVisDVector& data, const CMixturePPCAParam& param) const;
	double MAPInference(const CVisDVector& data, CMixturePPCAParam& param) const;
	void Reconstruct(const CMixturePPCAParam& param, CVisDVector& data) const;
	

	double JointLogP(const CVisDVector& data, int j, const CVisDVector& param) const;
	double MAPInference(const CVisDVector& data, int& j, CVisDVector& param) const;
	void Reconstruct(const CVisDVector& param, int j, CVisDVector& data) const;

	// zijian 
	void Reconstruct(const CVisDVector& param, int j, CVisDVector& data, int nEigen) const;
	void Reconstruct(const CVisDVector& param, int j, CVisDVector& data, double dPercent) const;

	friend COutputDataFile& Output(COutputDataFile& ofs, const CMixturePPCA& model);
	friend CInputDataFile& Input(CInputDataFile& ifs, CMixturePPCA& model);

	void GetSimpleModelByPercent(const CMixturePPCA& srcModel, double p);
};

class BASEMODEL_API CMixturePPCASampler : public CSampler
{
public:
	CMixturePPCASampler(const CMixturePPCA& model);
	virtual ~CMixturePPCASampler();
	virtual void GetOneSample(CVisDVector& data, int method = 0);
	void GetOneSampleAndParam(CVisDVector& data, int& j, CVisDVector& param, int method = 0);

public:
	std::vector<CPPCAModelSampler *> m_vpClusterSampler; 
};

BASEMODEL_API COutputDataFile& operator<<(COutputDataFile& ofs, const CMixturePPCAParam& param);
BASEMODEL_API CInputDataFile& operator>>(CInputDataFile& ifs, CMixturePPCAParam& param);

BASEMODEL_API COutputDataFile& operator<<(COutputDataFile& ofs, const CMixturePPCA& model);
BASEMODEL_API CInputDataFile& operator>>(CInputDataFile& ifs, CMixturePPCA& model);

#endif // !defined(AFX_MIXTUREPPCA_H__6053863F_DB49_498B_8F83_DEA80DBF48E5__INCLUDED_)

// MixturePPCA.cpp: implementation of the CMixturePPCA class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseModel.h"
#include "MixturePPCA.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void CMixturePPCA::GetSimpleModelByPercent(const CMixturePPCA& srcModel, double p)
{
	m_nDataDimesion = srcModel.m_nDataDimesion;
	m_nCluster = srcModel.m_nCluster;
	
	m_vMixingWeight.Resize(m_nCluster);
	m_vClusterModel.resize(m_nCluster);
	for (int iCluster = 0; iCluster < m_nCluster; iCluster++ )
	{
		m_vMixingWeight[iCluster] = srcModel.m_vMixingWeight[iCluster];
		m_vClusterModel[iCluster].GetSimpleModelByPercent(srcModel.m_vClusterModel[iCluster], p);
	}
}

double CMixturePPCA::JointLogP(const CVisDVector& data, const CMixturePPCAParam& param) const
{
	return JointLogP(data, param.m_iCluster, param.m_EigenParam);
}

double CMixturePPCA::MAPInference(const CVisDVector& data, CMixturePPCAParam& param) const
{
	return MAPInference(data, param.m_iCluster, param.m_EigenParam);	
}

void CMixturePPCA::Reconstruct(const CMixturePPCAParam& param, CVisDVector& data) const
{
	Reconstruct(param.m_EigenParam, param.m_iCluster, data);
}

double CMixturePPCA::JointLogP(const CVisDVector& data, int j, const CVisDVector& param) const
{
	return log(m_vMixingWeight[j]) + m_vClusterModel[j].JointLogP(data, param);
}

double CMixturePPCA::MAPInference(const CVisDVector& data, int& j, CVisDVector& param) const
{
	CVisDVector vLogP(m_nCluster);
	for (int iCluster = 0; iCluster < m_nCluster; iCluster++ )
	{
		vLogP[iCluster] = m_vClusterModel[iCluster].MAPInference(data, param) + log(m_vMixingWeight[iCluster]);
	}
	j = VisDVectorMaxIndex(vLogP);
	m_vClusterModel[j].MAPInference(data, param);
	return vLogP[j];
}

void CMixturePPCA::Reconstruct(const CVisDVector& param, int j, CVisDVector& data) const
{
	m_vClusterModel[j].Reconstruct(param, data);
}

// zijian
// reconstruct by Energy-percent
void CMixturePPCA::Reconstruct(const CVisDVector& param, int j, CVisDVector& data, double dPercent) const
{
	m_vClusterModel[j].Reconstruct(param, data, dPercent);
}

// reconstruct by Eigen-number
void CMixturePPCA::Reconstruct(const CVisDVector& param, int j, CVisDVector& data, int nEigen) const 
{
	m_vClusterModel[j].Reconstruct(param, data, nEigen);
}

COutputDataFile& Output(COutputDataFile& ofs, const CMixturePPCA& model)
{
	ofs << ClassTag("CMixturePPCA") << sep_endl;
	ofs << model.m_nDataDimesion << model.m_nCluster << sep_endl;
	ofs << model.m_vMixingWeight << sep_endl;
	for (int iCluster = 0; iCluster < model.m_nCluster; iCluster ++ )
	{
		ofs << model.m_vClusterModel[iCluster] << sep_endl; 
	}
	return ofs;
}

CInputDataFile& Input(CInputDataFile& ifs, CMixturePPCA& model)
{
	ifs >> ClassTag("CMixturePPCA") >> sep_endl;
	ifs >> model.m_nDataDimesion >> model.m_nCluster >> sep_endl;
	ifs >> model.m_vMixingWeight >> sep_endl;
	assert(model.m_nCluster == model.m_vMixingWeight.Length() );

	model.m_vClusterModel.resize(model.m_nCluster);
	for (int iCluster = 0; iCluster < model.m_nCluster; iCluster ++ )
	{
		ifs >> model.m_vClusterModel[iCluster] >> sep_endl; 
		assert(model.m_vClusterModel[iCluster].NDataDimension() == model.m_nDataDimesion );
	}
	return ifs;
}

COutputDataFile& operator<<(COutputDataFile& ofs, const CMixturePPCA& model)
{
	return Output(ofs, model);
}

CInputDataFile& operator>>(CInputDataFile& ifs, CMixturePPCA& model)
{
	return Input(ifs, model);
}

CMixturePPCASampler::CMixturePPCASampler(const CMixturePPCA& model)
: CSampler(model)
{
	m_vpClusterSampler.resize(model.m_nCluster);
	for (int iCluster = 0; iCluster < model.m_nCluster; iCluster++ )
	{
		m_vpClusterSampler[iCluster] = new CPPCAModelSampler(model.m_vClusterModel[iCluster]);
	}
}


CMixturePPCASampler::~CMixturePPCASampler()
{
	for (int iCluster = 0; iCluster < m_vpClusterSampler.size(); iCluster++ )
	{
		delete m_vpClusterSampler[iCluster];
	}
	m_vpClusterSampler.clear();
}

void CMixturePPCASampler::GetOneSampleAndParam(CVisDVector& data, int& j, CVisDVector& param, int method)
{
	const CMixturePPCA& model = (const CMixturePPCA&)m_ModelRef;

	float f = MyRand();
	int iCluster = 0;
	while ( iCluster < model.m_nCluster )
	{
		f -= model.m_vMixingWeight[iCluster];	
		if ( f <= 0 )
		{
			break;	
		}
		else
		{
			iCluster++;
		}
	}

	j = iCluster;
	m_vpClusterSampler[j]->GetOneSampleAndParam(data, param);
}


void CMixturePPCASampler::GetOneSample(CVisDVector& data, int method)
{
	int j;
	CVisDVector param;
	GetOneSampleAndParam(data, j, param, method);
}

CMixturePPCAParam::CMixturePPCAParam()
: m_iCluster(0)
{
}

const CMixturePPCAParam& CMixturePPCAParam::operator=(const CMixturePPCAParam& ref)
{
	if ( this!=&ref )
	{
		m_iCluster = ref.m_iCluster;
		if ( ref.m_EigenParam.Length()>0 )
		{
			m_EigenParam.Resize(ref.m_EigenParam.Length());
			m_EigenParam = ref.m_EigenParam;
		}
	}
	return *this;
}

COutputDataFile& operator<<(COutputDataFile& ofs, const CMixturePPCAParam& param)
{
	ofs << param.m_iCluster << param.m_EigenParam;
	return ofs;
}

CInputDataFile& operator>>(CInputDataFile& ifs, CMixturePPCAParam& param)
{
	ifs >> param.m_iCluster >> param.m_EigenParam;
	return ifs;
}

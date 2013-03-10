// MixtureGaussian.cpp: implementation of the CMixtureGaussian class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MixtureGaussian.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMixtureGaussian::CMixtureGaussian()
{

}

CMixtureGaussian::~CMixtureGaussian()
{

}

void CMixtureGaussian::GetCovMatrix(int iGaussian, CVisDMatrix& CovMat)
{
	assert(iGaussian>=0 && iGaussian<m_vGaussian.size());

	m_vGaussian[iGaussian].GetCovMatrix(CovMat);
}

double CMixtureGaussian::LogP(const CVisDVector& data) const
{
	CVisDVector vGaussianLogP(m_nGaussian);

	for ( int iGaussian = 0; iGaussian < m_nGaussian; iGaussian ++ )
	{
		vGaussianLogP[iGaussian] = m_vGaussian[iGaussian].LogP(data) + log(m_vMixingWeight[iGaussian]);
	}
	
	return LogSum(vGaussianLogP);
}


double CMixtureGaussian::JointLogP(const CVisDVector& data, int j) const
{
	return m_vGaussian[j].LogP(data) + log(m_vMixingWeight[j]);
}

void CMixtureGaussian::CalcPj(const CVisDVector& data, CVisDVector& vPj) const
{
	vPj.Resize(m_nGaussian);

	for (int iGaussian =0; iGaussian< m_nGaussian; iGaussian++ )
	{
		vPj[iGaussian] = m_vGaussian[iGaussian].LogP(data) + log(m_vMixingWeight[iGaussian]);
	}
	
	double sumP = LogSum(vPj);
	for (iGaussian = 0; iGaussian < m_nGaussian; iGaussian++ )
	{
		vPj[iGaussian] = exp(vPj[iGaussian] - sumP);
	}
}

double CMixtureGaussian::MAPInference(const CVisDVector& data, int j) const
{
	CVisDVector vPj;
	vPj.Resize(m_nGaussian);
	for (int iGaussian =0; iGaussian< m_nGaussian; iGaussian++ )
	{
		vPj[iGaussian] = m_vGaussian[iGaussian].LogP(data) + log(m_vMixingWeight[iGaussian]);
	}
	
	j = VisDVectorMaxIndex(vPj);	
	return vPj[j];
}



CMixtureGaussianSampler::CMixtureGaussianSampler(const CMixtureGaussian& model)
: CSampler(model)
{
	m_vpGaussianSampler.resize(model.m_nGaussian);
	for (int iGaussian = 0; iGaussian < model.m_nGaussian; iGaussian++ )
	{
		m_vpGaussianSampler[iGaussian] = new CGaussianSampler(model.m_vGaussian[iGaussian]);
	}
}


CMixtureGaussianSampler::~CMixtureGaussianSampler()
{
	for (int iGaussian = 0; iGaussian < m_vpGaussianSampler.size(); iGaussian++ )
	{
		delete m_vpGaussianSampler[iGaussian];
	}
	m_vpGaussianSampler.clear();
}


void CMixtureGaussianSampler::GetOneSample(CVisDVector& data, int method)
{
	const CMixtureGaussian& model = (const CMixtureGaussian&)m_ModelRef;

	float f = MyRand();
	int iCluster = 0;
	while ( iCluster < model.m_nGaussian )
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

	m_vpGaussianSampler[iCluster]->GetOneSample(data);
}

COutputDataFile& Output(COutputDataFile& fs, const CMixtureGaussian& model)
{
	fs << ClassTag("MixtureGaussian") << sep_endl;
	fs << model.m_nGaussian << sep_endl;
	assert(model.m_vMixingWeight.Length()==model.m_nGaussian);
	fs << ClassTag("Weights") << model.m_vMixingWeight << sep_endl;
	assert(model.m_vGaussian.size()==model.m_nGaussian);
	for ( int i=0; i<model.m_nGaussian; i++ )
	{
		fs << model.m_vGaussian[i] << sep_endl;
	}
	return fs;
}

COutputDataFile& operator<<(COutputDataFile& fs, const CMixtureGaussian& model)
{
	return Output(fs, model);
}

CInputDataFile& Input(CInputDataFile& fs, CMixtureGaussian& model)
{
	fs >> ClassTag("MixtureGaussian") >> sep_endl;
	fs >> model.m_nGaussian >> sep_endl;
	fs >> ClassTag("Weights") >> model.m_vMixingWeight >> sep_endl;
	assert(model.m_vMixingWeight.Length()==model.m_nGaussian);
	model.m_vGaussian.resize(model.m_nGaussian);
	for ( int i=0; i<model.m_nGaussian; i++ )
	{
		fs >> model.m_vGaussian[i] >> sep_endl;
	}
	return fs;
}

CInputDataFile& operator>>(CInputDataFile& fs, CMixtureGaussian& model)
{
	return Input(fs, model);
}

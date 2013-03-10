// MixtureGaussianKernel.cpp: implementation of the CMixtureGaussianKernel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MixtureGaussianKernel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMixtureGaussianKernel::CMixtureGaussianKernel()
{

}

CMixtureGaussianKernel::~CMixtureGaussianKernel()
{

}

double CMixtureGaussianKernel::JointLogP(const CVisDVector& data, int j ) const
{
	return log(m_vMixingWeight[j]) + m_vClusterModel[j].LogP(data);
}

double CMixtureGaussianKernel::MAPInference(const CVisDVector& data, int& j ) const
{
	CVisDVector vLogP(m_nCluster);
	CalcPj(data, vLogP);

	j = VisDVectorMaxIndex(vLogP);
	return vLogP[j];
}

CMixtureGKernelSampler::CMixtureGKernelSampler(const CMixtureGaussianKernel& model)
: CSampler(model)
{
	m_vpClusterSampler.resize(model.m_nCluster);
	for (int iCluster = 0; iCluster < model.m_nCluster; iCluster++ )
	{
		m_vpClusterSampler[iCluster] = new CGuassianKernelSampler(model.m_vClusterModel[iCluster]);
	}
}


CMixtureGKernelSampler::~CMixtureGKernelSampler()
{
	for (int iCluster = 0; iCluster < m_vpClusterSampler.size(); iCluster++ )
	{
		delete m_vpClusterSampler[iCluster];
	}
	m_vpClusterSampler.clear();
}

void CMixtureGKernelSampler::GetOneSampleAndParam(CVisDVector& data, int& j, int method)
{
	const CMixtureGaussianKernel& model = (const CMixtureGaussianKernel&)m_ModelRef;

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
	m_vpClusterSampler[j]->GetOneSample(data);
}


void CMixtureGKernelSampler::GetOneSample(CVisDVector& data, int method)
{
	int j;
	GetOneSampleAndParam(data, j, method);
}

void CMixtureGaussianKernel::Init(const CVisDMatrix& vCentralData, double Sigma)
{
	m_nCluster = vCentralData.NRows();
	m_vData = vCentralData;
	
	m_nDataDimesion = vCentralData.NCols();
	m_vMixingWeight.Resize(m_nCluster);

	double weight = (double)1.0 / m_nCluster;
	for (int iCluster = 0; iCluster < m_nCluster ; iCluster++ )
	{
		m_vMixingWeight[iCluster] = weight;
	}
	
	m_vClusterModel.resize(m_nCluster);
	for (iCluster = 0; iCluster < m_nCluster; iCluster++ )
	{
		m_vClusterModel[iCluster].SetModel(m_nDataDimesion, vCentralData.Row(iCluster), Sigma);
	}
}

const CMixtureGaussianKernel& CMixtureGaussianKernel::operator=(const CMixtureGaussianKernel& ref)
{
	if ( this!=&ref )
	{
		m_nCluster = ref.m_nCluster;
		m_nDataDimesion = ref.m_nDataDimesion;
		m_vMixingWeight.Resize(m_nCluster);
		m_vMixingWeight = ref.m_vMixingWeight;
		
		m_vClusterModel.resize(m_nCluster);
		for (int iCluster = 0; iCluster < m_nCluster; iCluster++ )
		{
			m_vClusterModel[iCluster].SetModel(m_nDataDimesion, ref.m_vClusterModel[iCluster].MeanData(), ref.m_vClusterModel[iCluster].Sigma());
		}
	}
	return (*this);
}


void CJointMixtureGKernel::Init(const CVisDMatrix& vDataX, double sigmaX, const CVisDMatrix& vDataY, double sigmaY)
{
	assert( vDataY.NRows() == vDataX.NRows());
	//m_vDataX = vDataX;
	//m_vDataY = vDataY;
	//m_SigmaX = sigmaX;
	//m_SigmaY = sigmaY;
	m_MixtureGKernelX.Init(vDataX, sigmaX);
	m_MixtureGKernelY.Init(vDataY, sigmaY);
}
	
void CJointMixtureGKernel::LogPKernelGivenY(const CVisDVector& dataY, CVisDVector& LogP) const
{
	m_MixtureGKernelY.CalcLogPj(dataY, LogP);
}

void CJointMixtureGKernel::LogPKernelGivenX(const CVisDVector& dataX, CVisDVector& LogP) const
{
	m_MixtureGKernelX.CalcLogPj(dataX, LogP);
}

void CJointMixtureGKernel::LogPKernelGivenXY(const CVisDVector& dataX, const CVisDVector& dataY, CVisDVector& LogP) const
{
	CVisDVector LogPX, LogPY;
	LogPKernelGivenX(dataX, LogPX);
	LogPKernelGivenY(dataY, LogPY);

	LogP.Resize(LogPX.Length());
	LogP = LogPX + LogPY;
}

void CJointMixtureGKernel::InitAdaptive(const CVisDMatrix& vDataX, const CVisDMatrix& vDataY, int nWithin2Sigma)
{
	assert( vDataY.NRows() == vDataX.NRows());
	m_MixtureGKernelX.InitAdaptive(vDataX, nWithin2Sigma);
	m_MixtureGKernelY.InitAdaptive(vDataY, nWithin2Sigma);

}

void CMixtureGaussianKernel::InitAdaptive(const CVisDMatrix& vCentralData, int nWithin2Sigma)
{
	m_nCluster = vCentralData.NRows();
	m_vData = vCentralData;

	nWithin2Sigma++;
	assert((m_nCluster > nWithin2Sigma)&&(nWithin2Sigma>2));

	// Estimate sigma
	double Sigma;
	{
		double avgDist = 0;

		for (int iStep = 0; iStep < nWithin2Sigma; iStep++ )
		{
			CVisDVector distVec(m_nCluster);
			// Random choose center
			int iChoose = MyRand() * (m_nCluster - 1);
			for (int iData = 0; iData < m_nCluster; iData++ )
			{
				CVisDVector tempVec = vCentralData.Row(iChoose);
				tempVec = tempVec - vCentralData.Row(iData);
				double dist = sqrt(tempVec * tempVec);
				distVec[iData] = dist;
			}
	
			// Find the nWidthin2Sigma-th minvalue
			double fMax = VisDVectorMaxData(distVec);
			double fMin;
			for ( int iMin = 0; iMin < nWithin2Sigma; iMin++ )
			{
				int iTemp = VisDVectorMinIndex(distVec);
				fMin = distVec[iTemp];
				distVec[iTemp] = fMax;
			}	
			
			avgDist += fMin;
		}
		avgDist /= nWithin2Sigma;
		Sigma = avgDist / 2;
	}
	
	m_nDataDimesion = vCentralData.NCols();
	m_vMixingWeight.Resize(m_nCluster);

	double weight = (double)1.0 / m_nCluster;
	for (int iCluster = 0; iCluster < m_nCluster ; iCluster++ )
	{
		m_vMixingWeight[iCluster] = weight;
	}
	
	m_vClusterModel.resize(m_nCluster);
	for (iCluster = 0; iCluster < m_nCluster; iCluster++ )
	{
		m_vClusterModel[iCluster].SetModel(m_nDataDimesion, vCentralData.Row(iCluster), Sigma);
	}
}

COutputDataFile& Output(COutputDataFile& ofs, const CMixtureGaussianKernel& model)
{
	ofs << ClassTag("MixtureGaussianKernel") << sep_endl;
	ofs << model.m_nDataDimesion << model.m_nCluster <<sep_endl;
	ofs << model.m_vData << sep_endl;
	ofs << model.m_vMixingWeight << sep_endl;

	for (int i = 0; i < model.NCluster(); i++ )
	{
		ofs << model.m_vClusterModel[i] ;
		if (i < model.NCluster() - 1)
		{
			ofs << sep_endl;
		}
	}
	return ofs;
}

COutputDataFile& operator<<(COutputDataFile& ofs, const CMixtureGaussianKernel& model)
{
	return Output(ofs, model);
}

CInputDataFile& Input(CInputDataFile& ifs, CMixtureGaussianKernel& model)
{
	ifs >> ClassTag("MixtureGaussianKernel") >> sep_endl;
	ifs >> model.m_nDataDimesion >> model.m_nCluster >> sep_endl;
	ifs >> model.m_vData >> sep_endl;
	ifs >> model.m_vMixingWeight >> sep_endl;

	model.m_vClusterModel.resize(model.m_nCluster);
	for (int i = 0; i < model.NCluster(); i++ )
	{
		ifs >> model.m_vClusterModel[i] ;
		if (i < model.NCluster() - 1)
		{
			ifs >> sep_endl;
		}
	}
	return ifs;
}

CInputDataFile& operator>>(CInputDataFile& ifs, CMixtureGaussianKernel& model)
{
	return Input(ifs, model);
}


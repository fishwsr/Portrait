// Distribution.cpp: implementation of the CDistribution class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Distribution.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDistribution::CDistribution()
: m_nDataDimesion(1)
{

}

CDistribution::~CDistribution()
{

}

// return log( SUM( exp(vLogData) )
double LogSum(const CVisDVector& vLogData)
{
	int nDimension = vLogData.Length();

	double maxlog = vLogData[0];
	for (int iDimension = 1; iDimension < nDimension; iDimension++ )
	{
		maxlog = max(maxlog, vLogData[iDimension]);
	}

	double sumexp = 0;
	for (iDimension = 0; iDimension < nDimension; iDimension++ )
	{
		sumexp += exp(vLogData[iDimension] - maxlog);
	}
	
	return maxlog + log(sumexp);
}

double Sum(const CVisDVector& vData)
{
	int nDimension = vData.Length();
	double sum = 0;
	for (int iDimension = 0; iDimension < nDimension ; iDimension++ )
	{
		sum += vData[iDimension];
	}
	return sum;
}

void CSSampler::GetSamples(int nSample, CVisDVector& vSample, int method )
{
	vSample.Resize(nSample);
	for (int iSample = 0; iSample < nSample; iSample++ )
	{
		vSample[iSample] = GetOneSample(method);
	}
}


void CSampler::GetSamples(int nSample, CVisDMatrix& dataMat, int method )
{
	dataMat.Resize(nSample, m_ModelRef.NDataDimension());
	CVisDVector tempData;
	for (int iSample = 0; iSample < nSample; iSample++ )
	{
		GetOneSample(tempData, method);
		dataMat.SetRow(iSample, tempData);
	}
}

double CDDistribution::Probability(int iEvent) const
{
	return exp(LogP(iEvent));
}

double CDDistribution::LogP(int iEvent) const
{
	return m_vLogP[iEvent];
}

void CDDistribution::Init(const CVisDVector& vLogP)
{
	m_nEvent = vLogP.Length();
	m_vLogP.Resize(m_nEvent);

	double sum = LogSum(vLogP);
	for (int iEvent = 0; iEvent < m_nEvent; iEvent++ )
	{
		m_vLogP[iEvent] = vLogP[iEvent] - sum;
	}
}

void CDDistribution::NormalizeLogP(CVisDVector& vLogP) 
{
	int nEvent = vLogP.Length();
	double sum = LogSum(vLogP);
	for (int iEvent = 0; iEvent < nEvent; iEvent++ )
	{
		vLogP[iEvent] -= sum; 
	}
}

CDSampler::CDSampler(const CDDistribution& model)
{
	m_vSumP.Resize(model.m_nEvent);
	for (int iEvent = 0; iEvent < model.m_nEvent; iEvent++ )
	{
		m_vSumP[iEvent] = model.Probability(iEvent);
	}

	for (iEvent = 1; iEvent < model.m_nEvent; iEvent++ )
	{
		m_vSumP[iEvent] += m_vSumP[iEvent - 1];
	}
}


void CDSampler::GetOneSample(int& iEvent)
{
	double rand = MyRand();
	for (iEvent = 0; iEvent < m_vSumP.Length() - 1; iEvent++ )
	{
		if ( rand < m_vSumP[iEvent] )
		{
			break;
		}
	}
}

CJointDistribution::CJointDistribution()
: m_nDataDimensionX(0), m_nDataDimensionY(0)
{
}

CJointDistribution::~CJointDistribution()
{
}

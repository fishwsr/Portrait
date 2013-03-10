
// PCAModel.cpp: implementation of the CPCAModel class.
//
///////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
//#include "DataFile.h"
#include "PCAModel.h"


CPCAModel::CPCAModel(void)
{
    m_iDataDimension = 0;
    m_iNbEigen = 0;
	m_fbReady = false;
}

CPCAModel::~CPCAModel(void)
{
}

void CPCAModel::Clear()
{
	m_fbReady = false;
}

void CPCAModel::Generate(CVisDVector& data, const CVisDVector& param) const
{
    if (param.Length() != m_iNbEigen)
    {
        assert(0);;
        return;
    }

    data = m_AvgData;

//	for (int i=0; i<m_iNbEigen; i++)
//		data += m_EigenVec.Row(i) * param[i];

	
	const double * pDataSrc;
	double * pDataDes;
	double tempf;

	int iLength = data.Length();
	
    for (int i = 0; i < m_iNbEigen; i++)
    {
		pDataDes = data.data();
		pDataSrc = m_EigenVec[i];
		tempf = param[i];
		
		for (int j=0;j<iLength; j++)
			(*pDataDes++) += (*pDataSrc++) * tempf;
    }

}

void CPCAModel::GetEigenParam(const CVisDVector& data, CVisDVector& param) const
{
    if (data.Length() != m_AvgData.Length())
    {
        assert(0);;
        return;
    }

	param.Resize(m_iNbEigen);
    param = m_EigenVec * (data - m_AvgData);
}

double CPCAModel::LogP(const CVisDVector& data) const
{
	CVisDVector tocenter(m_iDataDimension);
	tocenter = data - m_AvgData;
	CVisDVector projectval(m_iNbEigen);
	projectval = m_EigenVec * tocenter;
	double energy=0;
	double logp=0;
	for (int iEigen = 0; iEigen < m_iNbEigen; iEigen++ )
	{
		energy = (projectval[iEigen] * projectval[iEigen]) / m_EigenVal[iEigen];
		logp += (-energy/2) - log(2*3.14*m_EigenVal[iEigen])/2 ;
	}
	return logp;
}

double CPCAModel::LogP2(const CVisDVector& projectval) const
{
	assert(projectval.Length() == m_iNbEigen);

	double energy=0;
	double logp=0;
	for (int iEigen = 0; iEigen < m_iNbEigen; iEigen++ )
	{
		energy = (projectval[iEigen] * projectval[iEigen]) / m_EigenVal[iEigen];
		logp += (-energy/2) - log(2*3.14*m_EigenVal[iEigen])/2 ;
	}
	return logp;
}

COutputDataFile& Output(COutputDataFile& ofs, const CPCAModel& model)
{
	ofs << ClassTag("CPCAModel") << model.m_fbReady << sep_endl;
	
	if ( model.m_fbReady ) 
	{
		ofs << model.m_iDataDimension << model.m_iNbEigen << sep_endl;
		ofs << model.m_AvgData << sep_endl;
		ofs << model.m_EigenVal << sep_endl;
		ofs << model.m_InvEigenVal << sep_endl;
		ofs << model.m_EigenVec;
	}

	return ofs;
}

COutputDataFile& operator<<(COutputDataFile& ofs, const CPCAModel& model)
{
	return Output(ofs, model);
}


CInputDataFile& Input(CInputDataFile& ifs, CPCAModel& model)
{
	ifs >> ClassTag("CPCAModel") >> model.m_fbReady >> sep_endl;

	if ( model.m_fbReady )
	{
		ifs >> model.m_iDataDimension >> model.m_iNbEigen >> sep_endl;
		ifs >> model.m_AvgData >> sep_endl;
		ifs >> model.m_EigenVal >> sep_endl;
		ifs >> model.m_InvEigenVal >> sep_endl;
		ifs >> model.m_EigenVec;
		
		// Check
		if ((model.m_AvgData.Length() != model.m_iDataDimension) ||
			(model.m_EigenVal.Length() != model.m_iNbEigen ) ||
			(model.m_InvEigenVal.Length() != model.m_iNbEigen ) ||
			(model.m_EigenVec.NRows() != model.m_iNbEigen ) ||
			(model.m_EigenVec.NCols() != model.m_iDataDimension))
		{
			ifs.SetError(1, "Read Model Error");	
		}
	}

	return ifs;	
}

CInputDataFile& operator>>(CInputDataFile& ifs, CPCAModel& model)
{
	return Input(ifs, model);
}

CPCAModelSampler::CPCAModelSampler(const CPCAModel& model)
: m_ModelRef(model)
{
}

void CPCAModelSampler::GetOneSample(CVisDVector& data, int method )
{
	CVisDVector param;
	GetOneSampleAndParam(data, param, method);
}

void CPCAModelSampler::GetOneSampleAndParam(CVisDVector& data, CVisDVector& param, int method)
{
	const CPCAModel& model = (const CPCAModel&)m_ModelRef;
	CSGaussian sg(0, 1);
	CSGaussianSampler sgsampler(sg);

	param.Resize(model.NbEigen());
	sgsampler.GetSamples(model.NbEigen(), param);

	for ( int i=0; i<model.NbEigen(); i++ )
		param[i] *= sqrt(model.EigenVal(i));	

	data.Resize(model.AvgData().Length());
	model.Generate(data, param);
}	


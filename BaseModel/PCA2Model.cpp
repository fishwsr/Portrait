
// PCA2Model.cpp: implementation of the CPCA2Model class.
//
///////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "BaseModel.h"
#include "PCA2Model.h"
#include "Distribution.h"


CPCA2Model::CPCA2Model(void)
{
    m_ErrS = 0.00001;
    m_InvErrS = 1.0 / m_ErrS;
}

CPCA2Model::~CPCA2Model(void)
{

}

double CPCA2Model::EnergyLessThan(CVisDVector& data, double MaxEnergy)
{
    if (data.Length() != m_iDataDimension)
    {
        assert(0);;
        return -1.0;
    }

    int iEigen, iCol;

    CVisDVector tocenter(m_iDataDimension);
    CVisDVector projectval(m_iNbEigen);

    // 1. Get the PCA Energy
    double *tocenterPtr = tocenter.data();
    double *dataPtr = data.data();
    double *avgdataPtr = m_AvgData.data();
    for (int i = 0; i < m_iDataDimension; i++)
    {
        *tocenterPtr++ = *dataPtr++ - *avgdataPtr++;
    }
    
    double *projectvalPtr = projectval.data();
    double *tocenterPtr0 = tocenter.data();
    //double *eigenvecPtr = m_EigenVec.data();
    double *inveigenvalPtr = m_InvEigenVal.data();
    double energy = 0.0;
    for (iEigen = 0; iEigen < m_iNbEigen; iEigen++)
    {
        tocenterPtr = tocenterPtr0;
        *projectvalPtr = 0.0;
		double * eigenvecPtr = &m_EigenVec[iEigen][0];
        for (iCol = 0; iCol < m_iDataDimension; iCol++)
        {
            *projectvalPtr += (*eigenvecPtr++) * (*tocenterPtr++);
        }
        energy += (*projectvalPtr) * (*projectvalPtr) * (*inveigenvalPtr++);
        if (energy > MaxEnergy)
        {
            return DBL_MAX;
        }
        projectvalPtr++;
    }

    // 2. Get the error energy
    double errdata;
    tocenterPtr = tocenter.data();
    double *projectvalPtr0 = projectval.data();
    for (iCol = 0; iCol < m_iDataDimension; iCol++)
    {
        projectvalPtr = projectvalPtr0;
        errdata = *tocenterPtr++;
        for (iEigen = 0; iEigen < m_iNbEigen; iEigen++)
        {
            errdata -= (m_EigenVec[iEigen][iCol]) * (*projectvalPtr++);
        }
        energy += errdata * errdata * m_InvErrS;
        if (energy > MaxEnergy)
        {
            return DBL_MAX;
        }
    }

    return energy;
}

//modified by lin begin, 2003-3
double CPCA2Model::LogP(const CVisDVector& data, double MinLogP) const
{
	assert(data.Length()==m_iDataDimension);
	CVisDVector tocenter(m_iDataDimension);

	// 1. Get the PCA Probability
	tocenter = data - m_AvgData;
	CVisDVector projectval(m_iNbEigen);
	projectval = m_EigenVec * tocenter;
	double energy=0;
	double logp=0;
	for (int iEigen=0;iEigen<m_iNbEigen;iEigen++)
	{
		energy=projectval[iEigen]*projectval[iEigen]/m_EigenVal[iEigen];
		logp += (-energy/2)-log(2*3.14*m_EigenVal[iEigen])/2 ;
	}
	
	if ( logp<MinLogP )
		return -DBL_MAX;

	// 2. Get the error energy
	CVisDVector recdata(m_iDataDimension);
	CVisDVector errdata(m_iDataDimension);
	recdata =0 ;
	for (iEigen=0;iEigen<m_iNbEigen;iEigen++)
		recdata += m_EigenVec.Row(iEigen) * projectval[iEigen];

	errdata = tocenter - recdata;
	for (int i=0;i<m_iDataDimension;i++)
	{
		energy = errdata[i]*errdata[i] / m_ErrS;
		logp += (-energy/2) - log(m_ErrS*2*3.14159)/2;
	}

	if ( logp<MinLogP )
		return -DBL_MAX;

	return logp;
}
//modified by lin end, 2003-3

COutputDataFile& Output(COutputDataFile& fs, const CPCA2Model& model)
{
	fs << ClassTag("CPCA2Model") << model.m_fbReady << sep_endl;
	
	if ( model.m_fbReady )
	{
		fs << model.m_ErrS << model.m_InvErrS << sep_endl;
		fs << *(CPCAModel * )(&model) << sep_endl;
	}
	
	return fs;
}

CInputDataFile& Input(CInputDataFile& fs, CPCA2Model& model)
{
	fs >> ClassTag("CPCA2Model") >> model.m_fbReady >> sep_endl;

	if ( model.m_fbReady )
	{
		fs >> model.m_ErrS >> model.m_InvErrS >> sep_endl;
		fs >> * (CPCAModel * )(&model) >> sep_endl;
	}
	
	return fs;

}

COutputDataFile& operator<<(COutputDataFile& fs, const CPCA2Model& model)
{
	return Output(fs, model);
}

CInputDataFile& operator>>(CInputDataFile& fs, CPCA2Model& model)
{
	return Input(fs, model);
}

CPCA2ModelSampler::CPCA2ModelSampler(const CPCA2Model& model)
: CPCAModelSampler(model)
{
}

void CPCA2ModelSampler::GetOneSample(CVisDVector& data, int method )
{
	CVisDVector param;
	GetOneSampleAndParam(data, param, method);
}

void CPCA2ModelSampler::GetOneSampleAndParam(CVisDVector& data, CVisDVector& param, int method)
{
	const CPCA2Model& model = (const CPCA2Model&)m_ModelRef;
	
	CPCAModelSampler sampler((const CPCAModel&)m_ModelRef);
	sampler.GetOneSampleAndParam(data, param, method);

	// sampler error
	CSGaussian sg(0, 1);
	CSGaussianSampler sgsampler(sg);

	CVisDVector error;
	error.Resize(model.DataDimension());
	sgsampler.GetSamples(model.DataDimension(), error);

	for ( int i=0; i<model.DataDimension(); i++ )
	{
		data[i] += error[i] * sqrt(model.ErrS());
	}
}	

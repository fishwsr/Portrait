// PPCAModel.cpp: implementation of the CPPCAModel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PPCAModel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CPPCAModel::GetSimpleModelByPercent(const CPPCAModel& srcModel, double p)
{
	assert(p <= 1.0f );

	m_nDataDimesion = srcModel.m_nDataDimesion;
	m_nEigen = max( 1, srcModel.m_nEigen * p);
	m_AvgData.Resize(m_nDataDimesion);
	m_AvgData = srcModel.m_AvgData;
	
	m_EigenVal.Resize(m_nEigen);
	m_EigenVec.Resize(m_nEigen, m_nDataDimesion);
	for (int iEigen = 0; iEigen < m_nEigen; iEigen++ )
	{
		m_EigenVal[iEigen] = srcModel.m_EigenVal[iEigen];
		m_EigenVec.SetRow(iEigen, srcModel.m_EigenVec.Row(iEigen));
	}

	double VT = srcModel.m_Xigma2 * (m_nDataDimesion - srcModel.m_nEigen);
	for (iEigen = m_nEigen; iEigen < srcModel.m_nEigen; iEigen++ )
	{
		VT += srcModel.m_EigenVal[iEigen];
	}

	m_Xigma2 = VT / (m_nDataDimesion - m_nEigen);
	
	InitParametersWithBasicParameters();
}

CPPCAModel::CPPCAModel(int LogPCalcMode)
: m_LogPCalcMode(LogPCalcMode) 
{
	m_nEigen = 1;
	m_EigenVal.Resize(1);
	m_EigenVal[0] = 1;
	m_EigenVec.Resize(1,1);
	m_EigenVec[0][0] = 1;
	m_Xigma2 = 0.5;
	
	InitParametersWithBasicParameters();
}

CPPCAModel::~CPPCAModel()
{

}

void CPPCAModel::InitParametersWithBasicParameters()
{
	if ( 0 )
	{
		// Standard 
		m_ProjectMatrix.Resize(m_nEigen, m_nDataDimesion);
		m_ReconstructMatrix.Resize(m_nDataDimesion, m_nEigen);

		m_ProjectMatrix = m_EigenVec;
		m_ReconstructMatrix = m_EigenVec.Transposed();
	}
	else {
		// PPCA
		const double EPSVAL = 0.0000000001;
		m_Xigma2 = max(m_Xigma2, EPSVAL);

		CVisDMatrix VarMat(m_nEigen, m_nEigen);
		VarMat = 0;
		{
			double tempf;
			for (int iEigen = 0; iEigen< m_nEigen; iEigen++)
			{
				tempf = m_EigenVal[iEigen] - m_Xigma2;
				tempf = max(tempf, EPSVAL);
				VarMat[iEigen][iEigen] = sqrt(tempf);
			}
		}

		CVisDMatrix& Wml = m_W;
		
		Wml.Resize(m_nDataDimesion, m_nEigen);
		Wml = m_EigenVec.Transposed() * VarMat;

		// Init project and reconstruct matrix
		CVisDMatrix M(m_nEigen, m_nEigen);
		M = Wml.Transposed() * Wml;
	
		{
			for (int iEigen = 0; iEigen< m_nEigen; iEigen++)
			{
				M[iEigen][iEigen] += m_Xigma2;
			}
		}

		CVisDMatrix PosteriorCov;
		PosteriorCov.Resize(m_nEigen, m_nEigen);
		PosteriorCov = M.Inverted() * m_Xigma2;
		CVisDVector AvgData;
		AvgData.Resize(m_nEigen);
		AvgData = 0;
		m_PosteriorModel.SetModel(m_nEigen, AvgData, PosteriorCov);
		m_PosteriorConstant = m_PosteriorModel.LogP(AvgData);

		m_ProjectMatrix.Resize(m_nEigen, m_nDataDimesion);
		m_ProjectMatrix = M.Inverted() * Wml.Transposed();
		m_ReconstructMatrix.Resize(m_nDataDimesion, m_nEigen);
		//m_ReconstructMatrix = Wml * (Wml.Transposed() * Wml).Inverted() * M;
		m_ReconstructMatrix = Wml;

		if ( m_LogPCalcMode == 0)
		{
			// Init parameters of Gaussian model
			CVisDMatrix CovMat(m_nDataDimesion, m_nDataDimesion);
			CovMat = Wml * Wml.Transposed();
			{
				for (int iData = 0; iData < m_nDataDimesion; iData++)
				{
					CovMat[iData][iData] += m_Xigma2;
				}
			}	
			InitParametersWithCovMat(CovMat);
		}
	}
}

void CPPCAModel::GetCovMatrix(CVisDMatrix& CovMat)
{
	CVisDMatrix& Wml = m_W;

	CovMat.Resize(m_nDataDimesion, m_nDataDimesion);
	CovMat = Wml * Wml.Transposed();
	{
		for (int iData = 0; iData < m_nDataDimesion; iData++)
		{
			CovMat[iData][iData] += m_Xigma2;
		}
	}	
	return;	
}

void CPPCAModel::GetParam(const CVisDVector& data, CVisDVector& param) const
{
	param.Resize(m_nEigen);
	param =  m_ProjectMatrix * (data-m_AvgData);
}

void CPPCAModel::Reconstruct(const CVisDVector& param, CVisDVector& data) const
{
	data.Resize(m_nDataDimesion);
	data = m_ReconstructMatrix * param + m_AvgData;
}

// zijian
// reconstruct by Energy-percent
void CPPCAModel::Reconstruct(const CVisDVector& param, CVisDVector& data, double dPercent) const
{
	data.Resize(m_nDataDimesion);

	int i, iNbEigen;
	double VT, VT2;
	
	VT = 0.;
	for(i=0; i<=m_EigenVal.Length(); i++)
		VT += m_EigenVal[i];

	VT2 = 0.;
	iNbEigen=0;
	do
	{
		VT2 += m_EigenVal[iNbEigen++];
	}while((iNbEigen<m_EigenVal.Length()) && (VT2<dPercent*VT));

	if(iNbEigen<=0) 
		data = m_AvgData;
	else if(iNbEigen>=m_EigenVal.Length())
		data = m_ReconstructMatrix * param + m_AvgData;
	else {
		CVisDMatrix reconstructMatrix;
		CVisDVector nbParam;
		reconstructMatrix.Resize(m_ReconstructMatrix.NRows(), iNbEigen);
		nbParam.Resize(iNbEigen);
		for(i=0; i<iNbEigen; i++) { 
			reconstructMatrix.SetColumn(i, m_ReconstructMatrix.Column(i));
			nbParam[i] = param[i];
		}
		data = reconstructMatrix * nbParam + m_AvgData;
	}
}
// reconstruct by Eigen-number
void CPPCAModel::Reconstruct(const CVisDVector& param, CVisDVector& data, int nEigen) const
{
	data.Resize(m_nDataDimesion);

	if(nEigen<=0)
		data = m_AvgData;
	else if(nEigen>=m_EigenVal.Length())
		data = m_ReconstructMatrix * param + m_AvgData;
	else {
		CVisDMatrix reconstructMatrix;
		CVisDVector nbParam;
		reconstructMatrix.Resize(m_ReconstructMatrix.NRows(), nEigen);
		nbParam.Resize(nEigen);
		for(int i=0; i<nEigen; i++) {
			reconstructMatrix.SetColumn(i, m_ReconstructMatrix.Column(i));
			nbParam[i] = param[i];
		}
		data = reconstructMatrix * nbParam + m_AvgData;
	}
}

double CPPCAModel::LogP(const CVisDVector& data) const 
{
	if (m_LogPCalcMode == 0)
	{
		// PPCA
		return CGaussianModel::LogP(data);
	}
	else if (m_LogPCalcMode == 2)
	{
		// Standard PCA
		double LogPrior = 0;
		CVisDVector param(m_nEigen);
		GetParam(data, param);
		for (int iEigen = 0; iEigen < m_nEigen; iEigen++ )
		{
			CSGaussian g(0, m_EigenVal[iEigen]);
			LogPrior += g.LogP(param[iEigen]);
		}

		CVisDVector err(m_nDataDimesion);
		Reconstruct(param, err);
		err -= data;
		double err2 = err * err / (m_nDataDimesion - m_nEigen);

		double LogErr = 0;
		{
			CSGaussian g(0, m_Xigma2);
			LogErr = g.LogP(err2) * (m_nDataDimesion - m_nEigen); 
		}
		return LogPrior + LogErr;
	}
	else if (m_LogPCalcMode == 1)
	{
		// Constrcut P(param | data)
		CVisDVector param(m_nEigen);
		GetParam(data, param);
		double tempf1 = PriorLogP(param);
		double tempf2 = LikelihoodLogP(data, param);
		return tempf1 + tempf2 - m_PosteriorConstant; //PostLogP(data, param);
	}

	return 0.;
}


double CPPCAModel::PriorLogP(const CVisDVector& param) const
{
	double LogPrior = 0;
	CSGaussian g(0, 1);
	for (int iEigen = 0; iEigen< m_nEigen; iEigen++)
	{
		LogPrior += g.LogP(param[iEigen]);
	}
	return LogPrior;
}

double CPPCAModel::LikelihoodLogP(const CVisDVector& data, const CVisDVector& param) const
{
	CVisDVector ResidualData(m_nDataDimesion);
	ResidualData = m_W * param + m_AvgData;
	ResidualData -= data;
	
	double LogLikelihood = 0;
	{
		CSGaussian g(0, m_Xigma2);
		for (int iData = 0; iData < m_nDataDimesion; iData++ )
		{
			LogLikelihood += g.LogP(ResidualData[iData]);
		}
	}
	return LogLikelihood;
}

double CPPCAModel::PostLogP(const CVisDVector& data, const CVisDVector& param) const
{
	CVisDVector bestparam(m_nEigen);
	GetParam(data, bestparam);
	return m_PosteriorModel.LogP(param - bestparam);
}

double CPPCAModel::JointLogP(const CVisDVector& data, const CVisDVector& param) const
{
	return PriorLogP(param) + LikelihoodLogP(data, param);	
}

// Inference 
// param* = argmax P(param|data) = argmax P(data|param)P(param)
// return P(data,param*)
double CPPCAModel::MAPInference(const CVisDVector& data, CVisDVector& param) const
{
	GetParam(data, param);
	return JointLogP(data, param);
}

const CPPCAModel& CPPCAModel::operator=(const CPPCAModel& refModel)
{
	if ( this!= &refModel )
	{
		m_nDataDimesion = refModel.m_nDataDimesion;
		m_nEigen = refModel.m_nEigen;
		m_LogPCalcMode = refModel.m_LogPCalcMode;
		
		m_AvgData.Resize(refModel.m_AvgData.Length());
		m_AvgData = refModel.m_AvgData;
		
		m_EigenVal.Resize(refModel.m_EigenVal.Length());
		m_EigenVal = refModel.m_EigenVal;
		
		m_EigenVec.Resize(refModel.m_EigenVec.NRows(), refModel.m_EigenVec.NCols());
		m_EigenVec = refModel.m_EigenVec;
		
		m_Xigma2 = refModel.m_Xigma2;
		InitParametersWithBasicParameters();
	}
	return *this;
}

CPPCAModel::CPPCAModel(const CPPCAModel& refModel)
{
	m_nDataDimesion = refModel.m_nDataDimesion;
	m_nEigen = refModel.m_nEigen;
	m_LogPCalcMode = refModel.m_LogPCalcMode;
	m_AvgData = refModel.m_AvgData;
	m_EigenVal = refModel.m_EigenVal;
	m_EigenVec = refModel.m_EigenVec;
	m_Xigma2 = refModel.m_Xigma2;
	InitParametersWithBasicParameters();
}

COutputDataFile& Output(COutputDataFile& ofs, const CPPCAModel& model)
{
	ofs << ClassTag("CPPCAModel") << sep_endl;
	ofs << model.m_nDataDimesion << model.m_nEigen << model.m_LogPCalcMode << sep_endl;
	ofs << model.m_AvgData << sep_endl;
	ofs << model.m_EigenVal << sep_endl;
	ofs << model.m_EigenVec << sep_endl;
	ofs << model.m_Xigma2;

	return ofs;
}

COutputDataFile& operator<<(COutputDataFile& ofs, const CPPCAModel& model)
{
	return Output(ofs, model);
}


CInputDataFile& Input(CInputDataFile& ifs, CPPCAModel& model)
{
	ifs >> ClassTag("CPPCAModel") >> sep_endl;
	ifs >> model.m_nDataDimesion >> model.m_nEigen >> model.m_LogPCalcMode >> sep_endl;
	ifs >> model.m_AvgData >> sep_endl;
	ifs >> model.m_EigenVal >> sep_endl;
	ifs >> model.m_EigenVec >> sep_endl;
	ifs >> model.m_Xigma2;

	// Check
	if ((model.m_AvgData.Length() != model.m_nDataDimesion) ||
		(model.m_EigenVal.Length() != model.m_nEigen ) ||
		(model.m_EigenVec.NRows() != model.m_nEigen ) ||
		(model.m_EigenVec.NCols() != model.m_nDataDimesion))
	{
		ifs.SetError(1, "Read Model Error");	
	}

	model.InitParametersWithBasicParameters();
	return ifs;	
}

CInputDataFile& operator>>(CInputDataFile& ifs, CPPCAModel& model)
{
	return Input(ifs, model);
}

CPPCAModelSampler::CPPCAModelSampler(const CPPCAModel& model)
: CSampler(model)
{
	
}

void CPPCAModelSampler::GetOneSample(CVisDVector& data, int method )
{
	CVisDVector param;
	GetOneSampleAndParam(data, param, method);
}

void CPPCAModelSampler::GetOneSampleAndParam(CVisDVector& data, CVisDVector& param, int method)
{
	const CPPCAModel& model = (const CPPCAModel&)m_ModelRef;
	CSGaussian sg(0, 1);
	CSGaussianSampler sgsampler(sg);

	param.Resize(model.NEigen());
	sgsampler.GetSamples(model.NEigen(), param);

	data.Resize(model.m_nDataDimesion);
	sgsampler.GetSamples(model.NDataDimension(), data);
	data *= sqrt(model.m_Xigma2);

	data += model.m_W * param + model.m_AvgData;
}	

double CPPCAModel::CodingLengthParameter(double const_quanti)
{
	double CL = 0;
	double tempf;

	for (int i = 0; i< m_nEigen; i++)
	{
		tempf = 0.5 + 0.5 * log(2*3.14) + 0.5 * log(m_EigenVal[i]) + const_quanti;
		tempf = max(tempf, 0);
		CL += tempf;
	}
	
	return CL;
}

double CPPCAModel::DictionarySize()
{
	double DS = 0;
	DS = m_nDataDimesion * m_nEigen + m_nEigen + 1;

	return DS;
}


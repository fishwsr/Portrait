#ifndef __MIXTURE_MODEL_H__
#define __MIXTURE_MODEL_H__

template<class CClusterModel>
class BASEMODEL_API CMixtureModel : public CDistribution
{
public:
	virtual double LogP(const CVisDVector& data) const;
	virtual double LogPWithBias(const CVisDVector& data) const {return LogP(data);};

	void CalcLogPj(const CVisDVector& data, CVisDVector& vLogPj) const;
	void CalcPj(const CVisDVector& data, CVisDVector& vPj) const;

	int NCluster() const { return m_nCluster; };
	const CClusterModel& ClusterModel(int index) const {return m_vClusterModel[index];};

	void ResignWeight(const CVisDVector& weight);
	const CVisDVector& MixingWeight() const {return m_vMixingWeight; };

	double CodingLengthParameter(double const_quanti = 0);
	double DictionarySize();
protected:
	int m_nCluster;

	CVisDVector m_vMixingWeight;
	std::vector<CClusterModel> m_vClusterModel;
};

template<class CClusterModel>
inline double CMixtureModel<CClusterModel>::LogP(const CVisDVector& data) const
{
	CVisDVector vLogP(m_nCluster);

	for ( int iCluster = 0; iCluster < m_nCluster; iCluster ++ )
	{
		vLogP[iCluster] = m_vClusterModel[iCluster].LogP(data) + log(m_vMixingWeight[iCluster]);
	}
	
	return LogSum(vLogP);
}

template<class CClusterModel>
inline void CMixtureModel<CClusterModel>::CalcPj(const CVisDVector& data, CVisDVector& vPj) const
{
	vPj.Resize(m_nCluster);

	for (int iCluster =0; iCluster< m_nCluster; iCluster++ )
	{
		vPj[iCluster] = m_vClusterModel[iCluster].LogP(data) + log(m_vMixingWeight[iCluster]);
	}
	
	double sumP = LogSum(vPj);
	for (iCluster = 0; iCluster < m_nCluster; iCluster++ )
	{
		vPj[iCluster] = exp(vPj[iCluster] - sumP);
	}
}

template<class CClusterModel>
inline void CMixtureModel<CClusterModel>::CalcLogPj(const CVisDVector& data, CVisDVector& vLogPj) const
{
	vLogPj.Resize(m_nCluster);

	for (int iCluster =0; iCluster< m_nCluster; iCluster++ )
	{
		vLogPj[iCluster] = m_vClusterModel[iCluster].LogP(data) + log(m_vMixingWeight[iCluster]);
	}
	
	double sumP = LogSum(vLogPj);
	for (iCluster = 0; iCluster < m_nCluster; iCluster++ )
	{
		vLogPj[iCluster] = (vLogPj[iCluster] - sumP);
	}
}

template<class CClusterModel>
inline void CMixtureModel<CClusterModel>::ResignWeight(const CVisDVector& weight)
{
	assert(weight.Length() == m_nCluster );
	m_vMixingWeight = weight;
}


template<class CClusterModel>
inline double CMixtureModel<CClusterModel>::CodingLengthParameter(double const_quanti)
{
	double CL = 0;
	for (int iCluster = 0; iCluster < NCluster(); iCluster++ )
	{
		CL = m_vMixingWeight[iCluster] * m_vClusterModel[iCluster].CodingLengthParameter(const_quanti);
	}
	return CL;
}

template<class CClusterModel>
inline double CMixtureModel<CClusterModel>::DictionarySize()
{
	double DS = 0;
	for (int iCluster = 0; iCluster < NCluster(); iCluster++ )
	{
		DS += m_vClusterModel[iCluster].DictionarySize();
	}
	return DS;
}

#endif
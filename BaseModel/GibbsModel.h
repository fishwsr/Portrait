// GibbsModel.h: interface for the GibbsModel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GIBBSMODEL_H__121F1B8E_C9DF_4106_B540_CB147555AA12__INCLUDED_)
#define AFX_GIBBSMODEL_H__121F1B8E_C9DF_4106_B540_CB147555AA12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class BASEMODEL_API GibbsModel_Abstract
{
public:
	GibbsModel_Abstract(double fLearningRate=0.5) : m_fLearningRate(fLearningRate) {}
	virtual ~GibbsModel_Abstract() {}

	// learn the model
	virtual void Learn_model() = 0;

	// sample from current model
	virtual void Sample_model(int nSampleNum, int nBurningTime=5000, int nDrawEveryFewSamples=0) = 0;

protected:
	
	virtual void Initialize() = 0;		// initialization of obs_hist, syn_hist, lambda and feature sets
	// lowerbound, upperbound and sigma
	
	// Update the variables of all dimension.
	// Traditionally it's updated by each dimension;
	// however with a prior model on some dimensions, 
	// we may be able to update them all together by sampling from the prior.
	virtual void Update_variable() = 0; 
	
	// depends on pursuit from a set of predefined features 
	// or to learn a set of features (as in the LinearFeature case)	
	virtual void Pursuit_feature() = 0;   
	
	// update the potentials/lambda 
	virtual void Update_model() = 0; 
	
	// compute the log-likelihood of a sample
	virtual double LogProb() = 0;	

protected:

	double m_fLearningRate;
};

/*
class BASEMODEL_API GibbsModel_Linear
{
public:
	GibbsModel_Linear(double lfLearningRate=0.5);
	virtual ~GibbsModel_Linear();

	void Learn_model();		
	void Sample_model(int nSampleNum, int nBurningTime=5000, int nDrawEveryFewSamples=0);
	
protected:
	virtual void Initialize();
	virtual void Update_variable(); 		
	virtual void Pursuit_feature();   
	
	virtual double LogLikelihood(std::vector<double*>& vSampleVariables) { return 0.;}

protected:
	int m_nFeature;  // current number of feature/histogram
	std::vector<CVisDVector*> m_vvpLambda;  // potential/lambda
	std::vector<CHistogram1D*> m_vpHistObs;  // observed histogram of feature response
	std::vector<CHistogram1D*> m_vpHistSyn;  // synthesized histogram of feature response

	int m_nObsSize; // number of observed samples
	int m_nSynSize; // number of synthesized samples
	std::vector< std::vector<double*> > m_vvpVariableObs; // pointed to the variables in observed sample set
	std::vector< std::vector<double*> > m_vvpVariableSyn; // pointed to the variables in synthesized sample set

	// upperbound, lowerbound and standard deviation of 
	std::vector<double*> m_vVariableLowBound; 
	std::vector<double*> m_vVariableUpBound;
	std::vector<double*> m_vVariableSigma;

	double m_lfLearningRate; // learning rate = how fast we want the Lambda updated
};
*/
/*
class BASEMODEL_API GibbsModelSampler
{
public:
	GibbsModelSampler();
	virtual ~GibbsModelSampler();

	virtual void Choose_var()=0;	// choose 	
};

class GibbsModel{
public:
	int			nfeatures;
	vector<Matrixd> beta;			// the potentials for each feature, features can be samples or histograms
	vector<histogram *> Hobs;		// the observed histograms of the features
	vector<histogram *> Hsyn;		// the synthesized histograms from current prob
	vector<bool> histfeature;		// true if feature i is histogram, else false
	double		learning_rate;

	GibbsModel();
	virtual void Sample_model()=0;	//Get samples from current p and update Hsyn
	void		Update_model();
	double		logprob(Matrixd &FI);//log prob of I with features FI without the Z constant
	void		Draw(CDC* dc, int dx, int dy);
};

class GibbsSampler{ // abstract Gibbs sampler class				
public:
	int nval;
	
	GibbsSampler();
	virtual void	Choose_var()=0;		// first choose the variable to be sampled 
	virtual double	Get_increment()=0;	// and the increment for that variable
	virtual double	logprob()=0;		// then the logprob() will give the marginal log prob 
										// for that var having the current value
	virtual double	Get_var_value()=0;	// the current value of var
	virtual bool	Set_var(double d)=0;// set the current value of var
	virtual double	Get_temp()=0;
	double			Sample();           // then the Gibbs sampler will look arround +-nval values and will resample
};
*/

#endif // !defined(AFX_GIBBSMODEL_H__121F1B8E_C9DF_4106_B540_CB147555AA12__INCLUDED_)

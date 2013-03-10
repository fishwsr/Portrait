///////////////////////////////////////////////////////////////////////////
// Authour: Yao Zhenyu 2006-08-29
// DLL header of shape context functions
// Input:  std::vector<double> vSrc
// Output: 
//         1. std::vector<int> m_vAssignment for corespondence indexing
//						
//         2. std::vector<double> Cx, Cy TPS transform parameters from source to target
// Note:  Target point set should have greater number than source
//////////////////////////////////////////////////////////////////////////
#pragma once

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the VECDLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SC_EXPORTS functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#ifdef VECDLL_EXPORTS
#define SC_EXPORTS __declspec(dllexport)
#define EXPIMP_TEMPLATE
#else
#define SC_EXPORTS __declspec(dllimport)
#define EXPIMP_TEMPLATE extern
#endif

#include <vector>
using namespace std;

enum ParameterType {type0 = 0, oneLoopAssign, type2};

class SC_EXPORTS CSCParam
{
public:
	CSCParam(){};
	CSCParam(ParameterType e_type);
	~CSCParam(void);

public:
	float	fMeanDist_g;
	int		nBinsTheta;
	int		nBinsR;
	int		nDumScr;
	int		nDumTar;
	float	fEps_dum; // episilong for dummy
	float	r_inner;
	float   r_outer;
	unsigned nIter;  // times of iteration
	float	fAnnelRate; // anneling rate
	float   beta;       // initial regularization parameter (normalized)

	float   fCuttOff;
};

//EXPIMP_TEMPLATE template class SC_EXPORTS std::vector<double>;
class SC_EXPORTS CMyTPS
{
	//friend class CShapeContextCore;
public:
	CMyTPS(){};
	~CMyTPS(){};

	//The transform of current TPS(input dSrcX, dSrcY, return transformed point)
	void TPSTransform( double dSrcX, double dSrcY, double & dTarX, double & dTarY ) const;
	double   Create( vector<double> vecSrcX, vector<double> vecSrcY, vector<double> vecTarX, 
		vector<double> vecTarY, double dRegularize, bool bCompEnergy = false );
private:
	double   UFun(double dSqrDist ) const;

private:
	vector<double>  m_vecCtrlX;			// source control points
	vector<double>  m_vecCtrlY;			// source control points
	vector<double>	m_vecWeightX;		// for x coordinate	
	vector<double>	m_vecWeightY;		// for y coordinate
	vector<double>	m_vecCoefX;			// for x coordinate
	vector<double>	m_vecCoefY;			// for y coordinate
};

//EXPIMP_TEMPLATE template class SC_EXPORTS std::vector<double>;
class SC_EXPORTS CDLL_ShapeContext
{
public:
	CDLL_ShapeContext(void);
	~CDLL_ShapeContext(void);

	//Initialize the source, target and parameters
	bool Initialization(vector<double>& vSrcX, vector<double>& vSrcY, vector<double>& vTarX, 
		vector<double>& vTarY, CSCParam& param);
	//Major matching routine, will run call_back function on each matching loop
	void DoMatching();
	void DoSingleAssign();

	//Call_back class for active display on each matching loop
	class SC_EXPORTS CSCCallBack
	{
	public:
		CSCCallBack(){};
		~CSCCallBack(){};
		virtual void OnStep(CDLL_ShapeContext * Puller)=0;
	};
	void SetCallBack(CSCCallBack* _callBack){callBack = _callBack;};
	//void GetUpdateData(int& srcSize, double*& pSrcX, double*& pSrcY, int& tarSize, double*& pTarX, double*& pTarY,
	//				   int& curSize, double*& vCurX, double*& vCurY, int*& pAssignment, int& assign_size, int& nLoop);
	void GetUpdateData(vector<double>& vSrcX, vector<double>& vSrcY, vector<double>& vTarX, vector<double>& vTarY, 
		               vector<double>& vCurX, vector<double>& vCurY, int& assign_size, int& nLoop);
	void GetAssignment(int* pAssignment);
	CMyTPS* GetTPS(){return &m_tps;};

	//Tool functions
	void ReadVecFromFile(vector<double>& vec, int index, const char* name);
	//void SaveVecToFile(vector<double>& vec, const char* name);
	//CvMat* ReadMatFromFile(const char* name);

	//Inner data exchangement with shapecontextcore
public:
	void OnCallBack();
	void SetUpdateData(const vector<double> vCurX, const vector<double> vCurY, const vector<int> vAssignment,
		               const int _nLoop, CMyTPS& tps);
	bool GetSwitchFlag(){return m_fSwitch;};

protected:
	CSCCallBack *   callBack; // call_back function
	// source and target point set
	vector<double>  m_vSrcX;
	vector<double>  m_vSrcY;
	vector<double>  m_vTarX;
	vector<double>  m_vTarY;
	vector<double>  m_vCurX;
	vector<double>  m_vCurY;
	// mapping from source to target
	vector<int>     m_vAssignment;
	// Current loop
	int    m_nLoop;
	//TPS parameters
	CMyTPS m_tps;

    bool m_fSwitch;   // a flag to indicate whether the source and targe are switched
private:
	CSCParam myParam;
	int		 m_nSamp1; // point number of source
	int		 m_nSamp2; // point number of target
};






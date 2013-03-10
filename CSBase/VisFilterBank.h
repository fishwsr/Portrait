// VisFilterBank.h: interface for the CVisFilterBank class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VISFILTERBANK_H__5E661EC6_704E_4339_9BBE_A8EFD4E9C16D__INCLUDED_)
#define AFX_VISFILTERBANK_H__5E661EC6_704E_4339_9BBE_A8EFD4E9C16D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

extern CSBASE_API float k_basicgradient[3];
extern CSBASE_API float k_basicgradient2[3];
extern CSBASE_API float k_1[1];
extern CSBASE_API float k_010[3];
extern CSBASE_API float k_121[3];

extern CSBASE_API CVisFloatImage VisConvolutionKernel_BasicGradientX;
extern CSBASE_API CVisFloatImage VisConvolutionKernel_BasicGradientX2;
extern CSBASE_API CVisFloatImage VisConvolutionKernel_BasicGradientY;
extern CSBASE_API CVisFloatImage VisConvolutionKernel_BasicGradientY2;
extern CSBASE_API CVisFloatImage VisConvolutionKernel_1;
extern CSBASE_API CVisFloatImage VisConvolutionKernel_010;
extern CSBASE_API CVisFloatImage VisConvolutionKernel_SobelX;
extern CSBASE_API CVisFloatImage VisConvolutionKernel_SobelY;

#define VisFilter_PREWITT_3X3_V		0
#define VisFilter_PREWITT_3X3_H		1
#define VisFilter_SOBEL_3X3_V		2
#define VisFilter_SOBEL_5X5_H		3
#define VisFilter_LAPLACIAN_3X3		4
#define VisFilter_LAPLACIAN_5x5		5
#define VisFilter_GAUSSIAN_3X3		6
#define VisFilter_GAUSSIAN_5X5		7
#define VisFilter_HIPASS_3X3		8
#define VisFilter_HIPASS_5X5		9
#define VisFilter_SHARPEN_3X3		10

const float visfilter_prewitt_3x3_v[9] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
const float visfilter_prewitt_3x3_h[9] = {1, 1, 1, 0, 0, 0, -1, -1, -1};
const float visfilter_sobel_3x3_v[9] = {-1, 0, -1, 2, 0, 2, -1, 0, -1};
const float visfilter_sobel_3x3_h[9] = {1, 2, 1, 0, 0, 0, -1, -2, -1};
const float visfilter_laplacian_3x3[9] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
const float visfilter_laplacian_5x5[25] = {-1, -3, -4, -3, -1, 
										   -3, 0, 6, 0, -3,
										   -4, 6, 20, 6, -4,
										   -3, 0, 6, 0, -3,
										   -1, -3, -4, -3, -1};
const float visfilter_gaussian_3x3[9] = {1, 2, 1, 2, 4, 2, 1, 2, 1};			// A*(1/16)
const float visfilter_gaussian_5x5[25] = {2, 7, 12, 7, 2, 
										  7, 31, 52, 31, 7,
										  12, 52, 127, 52, 12,
										  7, 31, 52, 31, 7,
										  2, 7, 12, 7, 2};						// A*(1/571)
const float visfilter_hipass_3x3[9] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
const float visfilter_hipass_5x5[25] = {-1, -1, -1, -1, -1,
										-1, -1, -1, -1, -1,
										-1, -1, 24, -1, -1,
										-1, -1, -1, -1, -1,
										-1, -1, -1, -1, -1};
const float visfilter_sharpen_3x3[9] = {-1, -1, -1, -1, 16, -1, -1, -1, -1};	// A*(1/8)

class CSBASE_API CVisFilterBank
{
public:
	CVisFilterBank(int nScale=1, int nScaleStep=0.5, int nOri=1);
	virtual ~CVisFilterBank();
	
	void	ConstructFilterBank(){ m_fbReady=false; return;}
	void	FilterImage(const CVisFloatImage& srcImage, std::vector<CVisFloatImage>& vResponseImg);
	void	GetEnergyFeature(const std::vector<CVisFloatImage>& vResponseImg, std::vector<CVisFloatImage>& vEnergy, std::vector<CVisFloatImage>& vTotEnergyOverScale, std::vector<CVisFloatImage>& vTotEnergyOverOri, bool fUseEven, bool fUseOdd);
	void	GradAndStrengthOverScale(const std::vector<CVisFloatImage>& vEnergy, std::vector<CVisFloatImage>& vDominantTheta, std::vector<CVisFloatImage>& vDominantStrength);
	float	Ori2Theta(int iOri);
	void	GetOriented(const std::vector<CVisFloatImage>& vResponseImg, const CVisFloatImage& thetaImg, std::vector<CVisFloatImage>& vOriented);

	void	ShowFilters();
	bool&	IsReady() { return m_fbReady;}

protected:
	int		m_nScale;
	float	m_fScaleStep;
	int		m_nOri;
	bool	m_fbReady;

	std::vector<CVisFloatImage> m_vFilterBank;
};

class CSBASE_API CVisGaborFilterBank : public CVisFilterBank
{
public:
	CVisGaborFilterBank(int nScale=2, float fScaleStep=1.414f, float fSmallestLamda=5.f, int nOri=8, float fGamma=0.75f, float fSigmaWeight=0.56f);
	virtual ~CVisGaborFilterBank();
	void ConstructFilterBank();

protected:
	float	m_fSmallestLamda;
	float	m_fSigmaWeight;
	float	m_fGamma;
};

class CSBASE_API CVisDoogFilterBank : public CVisFilterBank
{
public:
	CVisDoogFilterBank(int nScale=2, float fScaleStep=1.414f, int nOri=8, float fSigmma=1.f, float fAspect=3.f);
	virtual ~CVisDoogFilterBank();
	void ConstructFilterBank();

protected:
	float	m_fSigma;
	float	m_fAspect;	
};

class CSBASE_API CVisBlobFilterBank : public CVisFilterBank
{
	CVisBlobFilterBank(int nScale=5, float fScaleStep=1.414f, float fSigma=2.f);
	virtual ~CVisBlobFilterBank();
	void ConstructFilterBank();

protected:
	float	m_fSigma;
};

CSBASE_API void NomalizeFilter(float *pImgData, int nCol, int nRow);
CSBASE_API void VisCreateFilter(int halfsizex, int halfsizey, float* data, CVisFloatImage& ker);

CSBASE_API double G(double x, double s);
CSBASE_API void visdog(double sigma, CVisFloatImage& ker);
CSBASE_API void visdoog(double sigma,double orientation,double aspect, CVisFloatImage& ker);
CSBASE_API void visdoog1(double sigma,double orientation,double aspect, CVisFloatImage& ker);

CSBASE_API int GetVisFixedFilter(int visfilter, float** pkernel, int &hwd, int &hht, double &fweight);
CSBASE_API int VisFixedFilter(CVisFloatImage& srcImage, CVisFloatImage& desImage, int visfilter=VisFilter_GAUSSIAN_3X3, int nIterative=1);
CSBASE_API int VisFixedFilterMasked(CVisFloatImage& srcImage, CVisFloatImage& desImage, std::vector<CPoint2>& vPoint2, int visfilter=VisFilter_GAUSSIAN_3X3, int nIterative=1);

CSBASE_API void GaborPt(float x, float y, float lamda, float sigma, float fGamma, float theta, float& cosvalue, float& sinvalue);
CSBASE_API void GaborFilter(float lamda, float fGamma, float fSigmaWeight, float theta, CVisFloatImage& cosfilter, CVisFloatImage& sinfilter);

CSBASE_API void Get1DGaussian(float sigma, CVisFloatImage& GaussianKernel1D, bool fNormalize);
CSBASE_API void Get1DCuttedGaussian(float sigma, CVisFloatImage& GaussianKernel1D, bool fNormalize);
CSBASE_API void Get2DGaussian(float sigmaX, float sigmaY, float theta, CVisFloatImage& GaussianKernel2D, bool fNormalize);
CSBASE_API void Get2DCuttedGaussian(float sigmaX, float sigmaY, float theta, CVisFloatImage& GaussianKernel2D, bool fNormalize);
CSBASE_API void Get1DRectangle(float sigma, CVisFloatImage& ker, bool fNormalize);
CSBASE_API void Get2DRectangle(float sigmaX, float sigmaY, float theta, CVisFloatImage& ker, bool fNormalize);

#endif // !defined(AFX_VISFILTERBANK_H__5E661EC6_704E_4339_9BBE_A8EFD4E9C16D__INCLUDED_)

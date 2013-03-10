// VisFilterBank.cpp: implementation of the CVisFilterBank class.
//
//////////////////////////////////////////////////////////////////////

#include "Stdafx.h"
#include "VisFilterBank.h"
//#include "VisDisplay.h"

static float k_basicgradient[3] = {-1, 0, 1};
static float k_basicgradient2[3] = { -1, 0, 1};
static float k_1[1] = {1};
static float k_010[3] = {0, 1, 0};
static float k_121[3] = {0.25, 0.5, 0.25};


CVisFloatImage VisConvolutionKernel_BasicGradientX (CVisRect(-1, 0, 2, 1), 1,
										  evisimoptDefault, (BYTE *) &k_basicgradient);
CVisFloatImage VisConvolutionKernel_BasicGradientX2 (CVisRect(-1, 0, 2, 1), 1,
										  evisimoptDefault, (BYTE *) &k_basicgradient2);
CVisFloatImage VisConvolutionKernel_BasicGradientY (CVisRect(0, -1, 1, 2), 1,
										  evisimoptDefault, (BYTE *) &k_basicgradient);
CVisFloatImage VisConvolutionKernel_BasicGradientY2 (CVisRect(0, -1, 1, 2), 1,
										  evisimoptDefault, (BYTE *) &k_basicgradient2);
CVisFloatImage VisConvolutionKernel_1 (CVisRect( 0, 0, 1, 1), 1,
										  evisimoptDefault, (BYTE *) &k_1);
CVisFloatImage VisConvolutionKernel_010 (CVisRect( -1, 0, 2, 1), 1,
										  evisimoptDefault, (BYTE *) &k_010);
CVisFloatImage VisConvolutionKernel_SobelX (CVisRect(-1, 0, 2, 1), 1,
										  evisimoptDefault, (BYTE *) &k_basicgradient2);
CVisFloatImage VisConvolutionKernel_SobelY (CVisRect(-1, 0, 2, 1), 1,
										  evisimoptDefault, (BYTE *) &k_121);

int GetVisFixedFilter(int visfilter, float** pkernel, int &hwd, int &hht, double &fweight)
{
	hwd=1;
	hht=1;
	fweight = 1.;
	*pkernel = NULL;

	switch(visfilter)
	{
	case VisFilter_PREWITT_3X3_V:
		*pkernel = (float*)visfilter_prewitt_3x3_v;
		break;
	case VisFilter_PREWITT_3X3_H:
		*pkernel = (float*)visfilter_prewitt_3x3_h;
		break;
	case VisFilter_SOBEL_3X3_V:	
		*pkernel = (float*)visfilter_sobel_3x3_v;
		break;
	case VisFilter_SOBEL_5X5_H:	
		*pkernel = (float*)visfilter_sobel_3x3_h;
		break;
	case VisFilter_LAPLACIAN_3X3:
		*pkernel = (float*)visfilter_laplacian_3x3;
		break;
	case VisFilter_LAPLACIAN_5x5:
		hwd = 2; 
		hht = 2;
		*pkernel = (float*)visfilter_laplacian_5x5;
		break;
	case VisFilter_GAUSSIAN_3X3:	
		fweight = 1./16;
		*pkernel = (float*)visfilter_gaussian_3x3;
		break;
	case VisFilter_GAUSSIAN_5X5:	
		hwd = 2; 
		hht = 2;
		fweight = 1./571;
		*pkernel = (float*)visfilter_gaussian_5x5;
		break;
	case VisFilter_HIPASS_3X3:
		*pkernel = (float*)visfilter_hipass_3x3;
		break;
	case VisFilter_HIPASS_5X5:
		hwd = 2; 
		hht = 2;
		*pkernel = (float*)visfilter_hipass_5x5;
		break;
	case VisFilter_SHARPEN_3X3:	
		fweight = 1./8;
		*pkernel = (float*)visfilter_sharpen_3x3;
		break;
	default:
		return 0;
	}	

	return 1;
}

int VisFixedFilter(CVisFloatImage &srcImg, CVisFloatImage &desImg, int visfilter /* =VisFilter_GAUSSIAN_3X3 */, int nIterative /* =1 */)
{
	if ( srcImg.Width()<=0 || srcImg.Height()<=0 ) return 0;

	int hwd, hht;
	double fweight;
	float* kernel;

	if ( !GetVisFixedFilter(visfilter, &kernel, hwd, hht, fweight) ) return 0;
	int nKerCols = 2*hwd+1;
	
	VisImageCopy(srcImg, desImg);
	CVisFloatImage tempImg;
	nIterative = max(1, nIterative);
	for (int i=0; i<nIterative; i++)
	{
		VisImageCopy(desImg, tempImg);
		desImg.Allocate(srcImg.Shape());
		desImg.FillPixels(0.);
		for (int y=0; y<desImg.Height(); y++)
		{
			for (int x=0; x<desImg.Width(); x++)
			{
				for (int r=-hht; r<=hht; r++)
				{
					for (int c=-hwd; c<=hwd; c++)
					{
						int tempx = LIMIT_TO_INTERVAL(c+x, 0, desImg.Width()-1);
						int tempy = LIMIT_TO_INTERVAL(r+y, 0, desImg.Height()-1);
						
						desImg.Pixel(x,y) += kernel[(r+hht)*nKerCols+c+hwd] * tempImg.Pixel(tempx, tempy);
						
					} // c
				} // r
				desImg.Pixel(x,y) *= fweight;
			} // x
		} // y
	} // i

	return 1;
}

int VisFixedFilterMasked(CVisFloatImage &srcImg, CVisFloatImage &desImg, std::vector<CPoint2>& vPoint2, int visfilter /* =VisFilter_GAUSSIAN_3X3 */, int nIterative /* =1 */)
{
	if ( srcImg.Width()<=0 || srcImg.Height()<=0 ) return 0;
	if ( vPoint2.size()<=0 ) return 0;

	int hwd, hht;
	double fweight;
	float* kernel;

	if ( !GetVisFixedFilter(visfilter, &kernel, hwd, hht, fweight) ) return 0;
	int nKerCols = 2*hwd+1;
	
	VisImageCopy(srcImg, desImg);
	CVisFloatImage tempImg;
	nIterative = max(1, nIterative);
	for (int i=0; i<nIterative; i++)
	{
		VisImageCopy(desImg, tempImg);
		for (int j=0; j<vPoint2.size(); j++)
		{
			int x = ROUND(vPoint2[j].X);
			int y = ROUND(vPoint2[j].Y);

			if (x<0 || x>desImg.Width()-1 || y<0 || y>desImg.Height()-1) continue;

			double tempf = 0.;
			for (int r=-hht; r<=hht; r++)
			{
				for (int c=-hwd; c<=hwd; c++)
				{
					int tempx = LIMIT_TO_INTERVAL(c+x, 0, desImg.Width()-1);
					int tempy = LIMIT_TO_INTERVAL(r+y, 0, desImg.Height()-1);
					
					tempf += kernel[(r+hht)*nKerCols+c+hwd] * tempImg.Pixel(tempx, tempy);
				} // c
			} // r
			desImg.Pixel(x,y) = fweight*tempf;
		} // j
	} // i

	return 1;
}

double G(double x, double s)
{
	// g = G(x, s)
	// Gaussian density truncated to (-3, 3) with 0 ending 
	// x: the difference from the mean
	// s: the scale 
	double d,p,x1,g;
	d = 3; p = .9974; 
	x1 = x/s; 
	if (fabs(x1)<d){ 
	   g = (exp(-x1*x1/2) - exp(-d*d/2))/sqrt(2*3.14)/s; 
	   return g/(p-d*2/sqrt(2*3.14)*exp(-d*d/2));    
	}
	else return 0; 
}

void NomalizeFilter(float *pImgData, int nCol, int nRow)
{
	return;

	float * pData = pImgData;
	float sumd = 0;

	int iRow, iCol;
	for (iRow = 0; iRow < nRow; iRow++ )
	{
		for (iCol = 0; iCol < nCol; iCol++ )
		{
			sumd += *pData++;
		}
	}
	sumd /= (nRow*nCol);
	
	pData = pImgData;
	for (iRow = 0; iRow < nRow; iRow++ )
	{
		for (iCol = 0; iCol < nCol; iCol++ )
		{
			*pData++ -= sumd;
		}
	}
	
	float sumd2 = 0;
	pData = pImgData;
	for (iRow = 0; iRow < nRow; iRow++ )
	{
		for (iCol = 0; iCol < nCol; iCol++ )
		{
			sumd2 += (*pData) * (*pData);
			pData++;
		}
	}
	sumd2 /= (nRow * nCol);
	sumd2 = sqrt(sumd2);
	if (sumd2 < 0.000001)
	{
		sumd2 = 0;
	}

	pData = pImgData;
	for (iRow = 0; iRow < nRow; iRow++ )
	{
		for (iCol = 0; iCol < nCol; iCol++ )
		{
			*pData++ /= sumd2;
		}
	}
}

void VisCreateFilter(int halfsizex, int halfsizey, float* data, CVisFloatImage& ker)
{
	ker.Allocate(CVisRect(-halfsizex, -halfsizey, halfsizex + 1, halfsizey +1 ));
	int sizex = halfsizex *2 +1;
	for (int y = -halfsizey; y< halfsizey+1; y++)
	{
		for (int x = -halfsizex; x< halfsizex+1; x++)
		{
			ker.Pixel(x,y) = data[ (y+halfsizey) * sizex + x + halfsizex];
		}
	}
}

void visdog(double sigma, CVisFloatImage& ker)
{
//	function [B, C] = dog(sigma)
//	[B, C] = dog(sigma)
// Center-sorround DOG base
// sigma: scale
// B: the base matrix
// C: the symbolic representation
	long i,j,size;
	double c,r,s,x,y,d;
	float *B;

	size = (int) (sigma*6);     // the size of the base matrix 
	c = sigma*3;        // the half size
	
	if ((size%2)==0)         // the size of B should be odd 
		size++;
	B=new float[size*size];

	r = .9; s = 1;          // the sizes of center and sorround 
	for (i = 0;i<size;i++){
		for (j = 0;j<size;j++){
			x = (i-c)/sigma;
			y = (j-c)/sigma;    // scale x and y
			B[size*size-1-(i*size+j)] = (float)(G(x, r)*G(y, r) - G(x, s)*G(y, s));
							// difference between center and sorround
		}
	}
	d=0;
	for (i=0; i<size; i++)
		for (j=0; j<size; j++)
			d += B[i*size+j]*B[i*size+j];
	d=sqrt(d);
	for (i=0; i<size; i++)
		for (j=0; j<size; j++)
			B[i*size+j] = (float)(B[i*size+j]/d);    // normalized to norm 1

	//NomalizeFilter(B, size, size);
	VisCreateFilter(c,c, B, ker);

	delete []B;
}

void visdoog(double sigma, double orientation, double aspect, CVisFloatImage& ker)
{
	// [B,C] = doog(sigma, orientation, aspect)
	// Even DOOG base, i.e., second derivative 
	// sigma: scale of the Gaussian in x-direction 
	// orientation: in degree
	// aspect ratio: y:x
	// B: the base matrix
	// C: the symbolic representation (in symbdoog)
	
	long i,j,size;
	double c,d,theta,costheta,sintheta,r,x,y;
	float *B;
	
	size = (int)(sigma * 4 * aspect)+1;     // size of the base
	c = sigma * 2 * aspect ;        // half-size
	theta = (MY_PI * orientation)/180.;    // orientation in radians
	costheta=cos(theta);
	sintheta=sin(theta);
	
	if ((size%2)==0)         // the size of B should be odd 
		size++;
	B=new float[size*size];

	r = aspect;           // aspect ratio
	for (i = 0;i<size;i++){
		for (j = 0;j<size;j++){
			x = ((i-c) * costheta + (j-c) * sintheta)/sigma;
			y = ((c-i) * sintheta + (j-c) * costheta)/sigma;
							  // rotate and scale x and y
			B[size*size-1-(i*size+j)] = (float)((2*G(x, 1)-G(x-1, 1)-G(x+1, 1))*G(y, r)); 
							  // second derivative 
		}
	}
	
	d=0;
	for (i=0; i<size; i++)
		for (j=0; j<size; j++)
			d += B[i*size+j]*B[i*size+j];
	d=sqrt(d);
	for (i=0; i<size; i++)
		for (j=0; j<size; j++)
			B[i*size+j] = (float)(B[i*size+j]/d);    // normalized to norm 1

	VisCreateFilter(c, c, B, ker);
	delete []B;
}

void visdoog1(double sigma, double orientation, double aspect, CVisFloatImage& ker)
{
// [B,C] = doog1(sigma, orientation, aspect)
// Odd DOOG base, i.e., first derivative 
// sigma: scale of the Gaussian in x-direction 
// orientation: in degree
// aspect ratio: y:x
// B: the base matrix
// C: the symbolic representation 

	long i,j,size;
	double c,d,theta,costheta,sintheta,r,x,y;
	float *B;

	size = (int)(sigma * 4 * aspect)+1;     // size of the base
	c = sigma * 2 * aspect ;        // half-size
	theta = (MY_PI * orientation)/180;    // orientation in radians
	costheta=cos(theta);
	sintheta=sin(theta);
	
	if ((size%2)==0)         // the size of B should be odd 
		size++;
	B=new float[size*size];

	r = aspect;           // aspect ratio
	for (i = 0;i<size;i++){
		for (j = 0;j<size;j++){
			x = ((i-c) * costheta + (j-c) * sintheta)/sigma;
			y = ((c-i) * sintheta + (j-c) * costheta)/sigma;
								 // rotate and scale x and y
			B[size*size-1-(i*size+j)] = (float)((G(x-1, 1)-G(x+1, 1))*G(y, r));      
                         // first derivative 
		}
	}
	
	d=0;
	for (i=0; i<size; i++)
		for (j=0; j<size; j++)
			d += B[i*size+j]*B[i*size+j];
	d=sqrt(d);
	for (i=0; i<size; i++)
		for (j=0; j<size; j++)
			B[i*size+j] = (float)(B[i*size+j]/d);    // normalized to norm 1

	//NomalizeFilter(B, size, size);
	VisCreateFilter(c, c, B, ker);
	delete []B;
}

void GaborPt(float x, float y, float lamda, float sigma, float fGamma, float theta, float& cosvalue, float& sinvalue)
{
	float x2 = x * cos(theta) + y * sin(theta);
	float y2 = - x * sin(theta) + y * cos(theta);
	
	float tempf = exp(- ( (x2 * x2) + fGamma* fGamma * y2 * y2) / (2 * sigma * sigma));
	cosvalue = tempf * cos(2 * MY_PI * x2 / lamda);
	sinvalue = tempf * cos(2 * MY_PI * x2 / lamda - MY_PI/2);
}

void GaborFilter(float lamda, float fGamma, float fSigmaWeight, float theta, CVisFloatImage& cosfilter, CVisFloatImage& sinfilter)
{
	int halflen = int(lamda*2);
	float sigma = fSigmaWeight * lamda;

	cosfilter.Allocate(CVisShape(-halflen, -halflen, halflen + 1, halflen + 1));
	sinfilter.Allocate(CVisShape(-halflen, -halflen, halflen + 1, halflen + 1));
	
	for (int iy = -halflen; iy < halflen+ 1; iy++ )
	{
		for (int ix = -halflen; ix < halflen+ 1; ix++ )
		{
			GaborPt(ix, iy, lamda, sigma, fGamma, theta, cosfilter.Pixel(ix, iy), sinfilter.Pixel(ix, iy));
		}
	}
}

void Get1DGaussian(float sigma, CVisFloatImage& GaussianKernel1D, bool fNormalize)
{
	int len = sigma * 2 ;
	GaussianKernel1D.Allocate(CVisShape(-len, 0, len + 1, 1));
	int ix;
	for (ix = -len ; ix < len + 1; ix++)
	{
		GaussianKernel1D.Pixel(ix, 0) = exp(- ix* ix / (2 * sigma * sigma)) / ( sqrt(2 * MY_PI) * sigma); 
	}

	if (fNormalize)
	{
		float sum = 0.f; 
		for (ix = -len ; ix < len + 1; ix++)
		{
			sum += GaussianKernel1D.Pixel(ix, 0);
		}
		assert(sum >0.f);
		for (ix = -len ; ix < len + 1; ix++)
		{
			GaussianKernel1D.Pixel(ix, 0) /= sum;
		}
	}
}

void Get1DCuttedGaussian(float sigma, CVisFloatImage& GaussianKernel1D, bool fNormalize)
{
	int len = sigma * 2 ;
	GaussianKernel1D.Allocate(CVisShape(-len, 0, len + 1, 1));
	
	int ix;
	float fConst = sqrt(2 * MY_PI) * sigma;
	float t;

	for (ix = -len ; ix < len + 1; ix++)
	{
		t = ix* ix / (2 * sigma * sigma);
		if (t > 1.f)
		{
			GaussianKernel1D.Pixel(ix, 0) = 0.f;
		}
		else
		{
			GaussianKernel1D.Pixel(ix, 0) = exp(-t) / (fConst); 
		}
	}

	if (fNormalize)
	{
		float sum = 0.f; 
		for (ix = -len ; ix < len + 1; ix++)
		{
			sum += GaussianKernel1D.Pixel(ix, 0);
		}
		assert(sum >0.f);
		for (ix = -len ; ix < len + 1; ix++)
		{
			GaussianKernel1D.Pixel(ix, 0) /= sum;
		}
	}
}

void Get2DGaussian(float sigmaX, float sigmaY, float theta, CVisFloatImage& GaussianKernel2D, bool fNormalize)
{
	int lenx = sigmaX * 2;
	int leny = sigmaY * 2;
	int len = max(lenx , leny);

	GaussianKernel2D.Allocate(CVisShape(-len, -len, len+1, len+1));
	
	int ix, iy;
	float PI2SigmaXSigmaY = 2 * MY_PI * sigmaX * sigmaY;

	for (iy = -len; iy < len + 1; iy++ )
	{
		for (ix = -len; ix < len+ 1; ix++ )
		{
			float u = cos(theta) * ix + sin(theta) * iy;
			float v = -sin(theta) * ix + cos(theta) * iy;
			
			GaussianKernel2D.Pixel(ix, iy) = exp( - u * u/(2 * sigmaX * sigmaX) 
											 - v * v/(2 * sigmaY * sigmaY))
											/ PI2SigmaXSigmaY;			
		}
	}

	if (fNormalize)
	{
		float sum = 0.0;
		for (iy = -len; iy < len + 1; iy++ )
		{
			for (ix = -len; ix < len+ 1; ix++ )
			{
				sum += GaussianKernel2D.Pixel(ix, iy);
			}
		}
		assert(sum > 0.f);
		for (iy = -len; iy < len + 1; iy++ )
		{
			for (ix = -len; ix < len+ 1; ix++ )
			{
				GaussianKernel2D.Pixel(ix, iy) /= sum;
			}
		}
	}
}

void Get2DCuttedGaussian(float sigmaX, float sigmaY, float theta, CVisFloatImage& GaussianKernel2D, bool fNormalize)
{
	int lenx = sigmaX * 1.1;
	int leny = sigmaY * 1.1;
	int len = max(lenx , leny);

	GaussianKernel2D.Allocate(CVisShape(-len, -len, len+1, len+1));
	GaussianKernel2D.FillPixels(0.f);
	
	int ix, iy;
	float PI2SigmaXSigmaY = 2 * MY_PI * sigmaX * sigmaY;
	for (iy = -len; iy < len + 1; iy++ )
	{
		for (ix = -len; ix < len+ 1; ix++ )
		{
			float u = cos(theta) * ix + sin(theta) * iy;
			float v = -sin(theta) * ix + cos(theta) * iy;
			
			float t = u * u/(2 * sigmaX * sigmaX) + v * v/(2 * sigmaY * sigmaY);
			if (t <= 1.0f)
			{
				GaussianKernel2D.Pixel(ix, iy) = exp(-t)/(PI2SigmaXSigmaY);			
			}
		}
	}

	if (fNormalize)
	{
		float sum = 0.0;
		for (iy = -len; iy < len + 1; iy++ )
		{
			for (ix = -len; ix < len+ 1; ix++ )
			{
				sum += GaussianKernel2D.Pixel(ix, iy);
			}
		}
		assert(sum > 0.f);
		for (iy = -len; iy < len + 1; iy++ )
		{
			for (ix = -len; ix < len+ 1; ix++ )
			{
				GaussianKernel2D.Pixel(ix, iy) /= sum;
			}
		}
	}
}

void Get1DRectangle(float sigma, CVisFloatImage& ker, bool fNormalize)
{
	int len = floor(sigma) + 1;
	ker.Allocate(CVisShape(-len, 0, len + 1, 1));
	ker.FillPixels(1.0f/(2 * len + 1));	
}

void Get2DRectangle(float sigmaX, float sigmaY, float theta, CVisFloatImage& ker, bool fNormalize)
{
	int len = sqrt(sigmaX * sigmaX + sigmaY * sigmaY) + 1;
	ker.Allocate(CVisShape(-len, -len, len+ 1, len + 1));
	ker.FillPixels(0);

	int ix, iy;
	float sum = 0.f;
	for (iy = -len; iy < len + 1; iy++ )
	{
		for (ix = -len; ix < len+ 1; ix++ )
		{
			float u = cos(theta) * ix + sin(theta) * iy;
			float v = -sin(theta) * ix + cos(theta) * iy;
			
			if ((fabs(u) <= sigmaX)&&(fabs(v) <= sigmaY))
			{
				ker.Pixel(ix, iy) = 1.f;
				sum += 1.f;
			}
			else if ((fabs(u) < sigmaX + 1) && (fabs(v) < sigmaY + 1))
			{
				ker.Pixel(ix, iy) = 0.5f;
				sum += 0.5f;
			}
		}
	}
	for (iy = -len; iy < len + 1; iy++ )
	{
		for (ix = -len; ix < len+ 1; ix++ )
		{
			ker.Pixel(ix, iy) /= sum;
		}
	}
}

///////////////////////////////// CVisFilterBank //////////////////////////////
CVisFilterBank::CVisFilterBank(int nScale, int nScaleStep, int nOri)
: m_nScale(nScale), m_fScaleStep(nScaleStep), m_nOri(nOri), m_fbReady(false)
{	
}

CVisFilterBank::~CVisFilterBank()
{
}

void CVisFilterBank::FilterImage(const CVisFloatImage& srcImg, std::vector<CVisFloatImage>& vResponseImg)
{
	if (!m_fbReady)
		ConstructFilterBank();

	int nFilterBankSize = m_vFilterBank.size();
	vResponseImg.resize(nFilterBankSize);

	for (int i = 0; i<nFilterBankSize; i++)
	{
		std::cout << i << " ";
		vResponseImg[i].Allocate(srcImg.Shape());
		//OtherInstantiate();
		VisConvolve(const_cast<CVisFloatImage&>(srcImg), vResponseImg[i*2] , m_vFilterBank[i], 1);
	}
	std::cout << std::endl;
}

void CVisFilterBank::GetEnergyFeature(const std::vector<CVisFloatImage>& vResponseImg, std::vector<CVisFloatImage>& vEnergy, std::vector<CVisFloatImage>& vTotEnergyOverScale, std::vector<CVisFloatImage>& vTotEnergyOverOri, bool fUseEven, bool fUseOdd)
{
	int nHalfFilterSize = m_nScale * m_nOri;
	int nWidth = vResponseImg[0].Width();
	int nHeight = vResponseImg[0].Height();
	vEnergy.resize(nHalfFilterSize);

	vTotEnergyOverScale.resize(m_nScale);
	vTotEnergyOverOri.resize(m_nOri);

	float tempf, energy;;
	for (int i=0; i<m_nScale*m_nOri; i++)
	{
		vEnergy[i].Allocate(nWidth, nHeight);
		for (int iy=0; iy<nHeight; iy++)
		{
			for (int ix =0; ix < nWidth; ix++ )
			{
				energy = 0;
				if (fUseEven)
				{
					tempf = vResponseImg[i*2].Pixel(ix, iy);
					energy += tempf * tempf;
				}
				if (fUseOdd)
				{
					tempf = vResponseImg[i*2 + 1].Pixel(ix, iy);
					energy += tempf * tempf;
				}
				vEnergy[i].Pixel(ix, iy) = energy;
			}
		}
	}

	for (int iScale=0; iScale<m_nScale; iScale++)
	{
		vTotEnergyOverScale[iScale].Allocate(nWidth, nHeight);
		vTotEnergyOverScale[iScale].FillPixels(0);
		for (int iOri=0; iOri<m_nOri; iOri++)
		{
			VisMap2(CVisSum(), vTotEnergyOverScale[iScale], vEnergy[iScale * m_nOri + iOri]);
		}	
	}

	for (int iOri=0; iOri<m_nOri; iOri++)
	{
		vTotEnergyOverOri[iOri].Allocate(nWidth, nHeight);
		vTotEnergyOverOri[iOri].FillPixels(0);
		for (int iScale=0; iScale<m_nScale; iScale++)
		{
			VisMap2(CVisSum(), vTotEnergyOverOri[iOri], vEnergy[iScale * m_nOri + iOri]);
		}
	}
}

void CVisFilterBank::GradAndStrengthOverScale(const std::vector<CVisFloatImage>& vEnergy, std::vector<CVisFloatImage>& vDominantTheta, std::vector<CVisFloatImage>& vDominantStrength)
{
	int nWidth = vEnergy[0].Width();
	int nHeight = vEnergy[0].Height();

	vDominantTheta.resize(m_nScale);
	vDominantStrength.resize(m_nScale);
	int iDominantOri;
	float fDominantStrength;
		
	for (int iScale=0; iScale<m_nScale; iScale++)
	{
		vDominantTheta[iScale].Allocate(nWidth, nHeight);  
		vDominantStrength[iScale].Allocate(nWidth, nHeight);
		
		for (int iy=0; iy<nHeight; iy++)
		{
			for (int ix=0; ix<nWidth; ix++)
			{
				iDominantOri = 0;
				fDominantStrength = vEnergy[iScale * m_nOri].Pixel(ix, iy);

				for (int iOri=1; iOri<m_nOri; iOri++)
				{
					float tempf = vEnergy[iScale * m_nOri + iOri].Pixel(ix, iy);
					if (fDominantStrength < tempf)
					{
						iDominantOri = iOri;
						fDominantStrength = tempf;
					}
				}
				vDominantTheta[iScale].Pixel(ix, iy) = Ori2Theta(iDominantOri);
				vDominantStrength[iScale].Pixel(ix, iy) = fDominantStrength;
			}
		}
	}
}

void CVisFilterBank::GetOriented(const std::vector<CVisFloatImage>& vResponseImg, const CVisFloatImage& thetaImg, std::vector<CVisFloatImage>& vOriented)
{
	int nWidth = vResponseImg[0].Width();
	int nHeight = vResponseImg[0].Height();

	vOriented.resize(vResponseImg.size());
	for (int i = 0; i< vOriented.size(); i++)
	{
		vOriented[i].Allocate(nWidth, nHeight);  
	}

	float theta;
	int iThetaDet;
	for (int iy=0; iy<nHeight; iy++)
	{
		for (int ix=0; ix<nWidth; ix++)
		{
			theta = thetaImg.Pixel(ix, iy);	
			iThetaDet = ((int)(( theta * m_nOri ) / MY_PI + 0.5)) % m_nOri;
			while (iThetaDet < 0)
				iThetaDet += m_nOri;

			for (int iScale=0; iScale<m_nScale; iScale++)
			{
				for (int iOri=0; iOri<m_nOri; iOri++)
				{
					vOriented[(iScale * m_nOri + iOri) * 2].Pixel(ix, iy) =  vResponseImg[(iScale * m_nOri + ( iOri +iThetaDet) % m_nOri) * 2].Pixel(ix , iy);
					vOriented[(iScale * m_nOri + iOri) * 2 + 1].Pixel(ix, iy) =  vResponseImg[(iScale * m_nOri + ( iOri +iThetaDet) % m_nOri) * 2 + 1].Pixel(ix , iy);
				}
			}
		}
	}
}

float CVisFilterBank::Ori2Theta(int iOri)
{
	return MY_PI*iOri/m_nOri;
}

void CVisFilterBank::ShowFilters()
{
	if (m_fbReady)
	{
		for (int i = 0; i< m_vFilterBank.size(); i++ )
		{
//			VisDisplayImage(evisnormalizeRelative, m_vFilterBank[i]);
		}
	}
}

//////////////// CVisGaborFilterBank ///////////////
CVisGaborFilterBank::CVisGaborFilterBank(int nScale, float fScaleStep, float fSmallestLamda, int nOri, float fGamma, float fSigmaWeight)
: CVisFilterBank(nScale, fScaleStep, nOri), m_fSmallestLamda(fSmallestLamda), 
	m_fGamma(fGamma), m_fSigmaWeight(fSigmaWeight)
{
	ConstructFilterBank();
}

CVisGaborFilterBank::~CVisGaborFilterBank()
{
}

void CVisGaborFilterBank::ConstructFilterBank()
{
	m_vFilterBank.resize(2 * m_nScale * m_nOri);
	
	float lamda = m_fSmallestLamda;
	float theta;
	for (int iScale=0; iScale<m_nScale; iScale++)
	{
		for (int iOri=0; iOri<m_nOri; iOri++)
		{
			theta = Ori2Theta(iOri);	
			GaborFilter(lamda, m_fGamma, m_fSigmaWeight, theta, m_vFilterBank[2 * (iScale*m_nOri+iOri)], m_vFilterBank[2 * (iScale*m_nOri+iOri) + 1]);
		}
		lamda *= m_fScaleStep;
	}
	m_fbReady = true;
}

//////////////// CVisDoogFilterBank ///////////////
CVisDoogFilterBank::CVisDoogFilterBank(int nScale, float fScaleStep, int nOri, float fSigma, float fAspect)
: CVisFilterBank(nScale, fScaleStep, nOri), m_fSigma(fSigma), m_fAspect(fAspect)
{
	ConstructFilterBank();
}

CVisDoogFilterBank::~CVisDoogFilterBank()
{
}

void CVisDoogFilterBank::ConstructFilterBank()
{
	m_vFilterBank.resize(2 * m_nOri * m_nScale);
	
	float fCurSigma = m_fSigma;
	for (int iScale=0; iScale<m_nScale; iScale++)
	{
		for (int iOri=0; iOri<m_nOri; iOri++)
		{
			visdoog(fCurSigma, - iOri * 180. / m_nOri - 90.,  m_fAspect, m_vFilterBank[2 * (m_nOri*iScale+iOri)]);
			visdoog1(fCurSigma, - iOri * 180. / m_nOri - 90.,  m_fAspect, m_vFilterBank[2 * (m_nOri*iScale+iOri) + 1]);
		}
		fCurSigma *= m_fScaleStep;
	}
	m_fbReady = true;
}

//////////////// CVisBlobFilterBank ///////////////
CVisBlobFilterBank::CVisBlobFilterBank(int nScale, float fScaleStep, float fSigma)
: CVisFilterBank(nScale, fScaleStep, 1), m_fSigma(fSigma)
{
	ConstructFilterBank();
}

CVisBlobFilterBank::~CVisBlobFilterBank()
{
}

void CVisBlobFilterBank::ConstructFilterBank()
{
	m_vFilterBank.resize(2 * m_nScale);
	
	double sigma = m_fSigma;
	for (int iScale=0; iScale<m_nScale; iScale++)
	{
		visdog(sigma, m_vFilterBank[2*iScale]);

		m_vFilterBank[2*iScale+1].Allocate(m_vFilterBank[2*iScale].Shape());
		for(int h=m_vFilterBank[2*iScale].Top(); h<m_vFilterBank[2*iScale].Bottom(); h++)
		{
			for(int w=m_vFilterBank[2*iScale].Left(); w<=m_vFilterBank[2*iScale].Right(); w++)
			{
				m_vFilterBank[2*iScale+1].Pixel(w, h) = - m_vFilterBank[2*iScale].Pixel(w, h);
			}
		}

		sigma *= m_fScaleStep;		
	}

	m_fbReady = true;
}

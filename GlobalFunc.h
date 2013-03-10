// GlobalFunc.h: interface for the CGlobalFunc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBALFUNC_H__36EB379B_FAB8_46D9_B3E4_797C4CFE039B__INCLUDED_)
#define AFX_GLOBALFUNC_H__36EB379B_FAB8_46D9_B3E4_797C4CFE039B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define NORMALIZED_X 64
#define NORMALIZED_Y 64

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

//feature extraction method
#define FE_TEXTURE        1
#define FE_SHAPE          2
#define FE_TEXTURE_SHAPE  3
#define FE_MESH           4
#define FE_WAVELET        6
#define FE_ORIGINIMAGE    5
#define FE_AUTOCOV        7
#define FE_DEFAULT			FE_WAVELET	//default feature extraction method

#define TargetDIM		64	//default feature dimension

#define FR_LDA			1
#define FR_DEFAULT		FR_LDA

#define EPS				0.0000000001

#define	LARGER(x,y) (((x)>(y))?(x):(y))
#define	SMALLER(x,y) (((x)<(y))?(x):(y))

int		Convert2Stride(int	nWidth);
void	OrderValues(double* ev, long* order, long M);

BOOL	ResizeImage(BYTE* pSrcImg, int nSrcWidth, int nSrcHeight, BYTE* pDstImg, int nDstWidth, int nDstHeight);
BOOL	CropSubImage(BYTE* pbyteSrcImgData, int nSrcImgWidth, int nSrcImgHeight, BYTE* pDstImgData, RECT subRect);

void	DrawPlus(CDC* pDC, CPoint pnt, int size, COLORREF col);
BOOL	RGB2YIQ(BYTE R, BYTE G, BYTE B, BYTE & Y, double & I, double & Q);
BOOL	RGB2Fai(BYTE R, BYTE G, BYTE B, int& Fai);

double	NormalVector2UnitLength(double* vector, long N);
double	NormalVector2ZeroMeanUnitVar(double* vector, long N);
double	InnerProduct(double* pv1,double* pv2, long N);
double	PointDistance(CPoint p1, CPoint p2);
double	EuclidDistance(double* pv1, double* pv2, long N);
double	BlockDistance(double* pv1, double* pv2, long n);
double	MahalanobisDistance(double* pv1, double* pv2, double* sigma, int nDim);
double	VectorSimilarity(double* pv1, double* pv2, int nDim);

void	CalEgvalue(double *matrix,long n,double *eigenvalue,double *c);
long	CalEgvector(double *matrix,long n,double *eigenvalue,double *c,double eps);
void	ShrinkImage(BYTE *scrImage, BYTE* destImage, WORD w, WORD h, float cscale);
void	ShrinkImage(BYTE *scrImage, BYTE* destImage, WORD scrW, WORD scrH,WORD destW, WORD destH);
void	MagnifyImage(BYTE *scrImage, BYTE* destImage, WORD scrW, WORD scrH,WORD destW, WORD destH);

template <class T>
T**	New2DPointer(int n1D, int n2D)
{
	T**		pp;
	typedef		T*	T_P;
	int		i, j;
	if(n1D <= 0 || n2D <= 0)
		return FALSE;

	pp = new T_P[n1D];
	if(!pp)
		return NULL;
	for(i=0; i<n1D; i++)
	{
		pp[i] = new T[n2D];
		if(!pp[i])
		{
			for(j=0; j<i; j++)
			{
				delete	pp[j];
			}
			delete	pp;
			return NULL;
		}
	}
	return pp;
}

template <class T>
void	Delete2DPointer(T **pp, int n1D)
{
	int		i;
	if(pp == NULL)
		return;
	for(i=0; i<n1D; i++)
	{
		if(pp[i])
			delete pp[i];
	}
	delete	pp;
}

class CGlobalFunc  
{
public:

public:

	CGlobalFunc();
	virtual ~CGlobalFunc();

	static void	PseudoInverseMatrix(double *pA, double* pA_PT, long n, long m);
	static BOOL	ludcmp(double *a, int n, int *indx, double *d);
	static void	lubksb(double *a, int n, int *indx, double b[]);
	static BOOL	MatrixComputReverse(double *a, double *y,int n);
	static BOOL	MatrixMultiply(double* pScr1, double* pScr2, double* pDst, int m, int n, int l, int p);
	static void	ScaleVector(double * src,double * dest, int length, double value);
	static void	SubVector(double * src1,double * src2, double * dest, int length);

};

#endif // !defined(AFX_GLOBALFUNC_H__36EB379B_FAB8_46D9_B3E4_797C4CFE039B__INCLUDED_)

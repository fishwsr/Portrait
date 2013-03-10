// GlobalFunc.cpp: implementation of the CGlobalFunc class.
//
//////////////////////////////////////////////////////////////////////

/*
   This file is created by Shiguang Shan at 01.06.2002 to contain 
   Some common-used global functions
*/


#include "stdafx.h"
#include "GlobalFunc.h"
#include "math.h"

int		gnNormalFaceWidth = 32;
int		gnNormalFaceHeight= 32;
CString	gWorkDir;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGlobalFunc::CGlobalFunc()
{

}

CGlobalFunc::~CGlobalFunc()
{

}

/* 

  *******************************************************	
  The following functions are used for Matrix Compuation
  Induced in 2002.01.04 by Shiguang Shan
  *******************************************************
  
*/

////////////////////////////////////////////////////////////////
//对矩阵进行按行置换的LU分解，结果存在该矩阵中
BOOL CGlobalFunc::ludcmp(double *a, int n, int *indx, double *d)
{
	int      i,imax,j,k;
	double   big,dum,sum,temp;
	double   *vv;
	double   swap;
	
	vv = new double[n];
	*d = 1.0;
	for(i=0; i<n; i++)
	{
		big = 0.0;
		for(j=0; j<n; j++)
			if((temp = fabs( *(a+i*n+j))) > big)  big =temp;
			if(big == 0.0) 
			{
				AfxMessageBox("Singular matrix in routine ludcmp");
				delete [] vv;
				return false;
			}
			vv[i] = 1.0/big;
	}
	
	for(j=0; j<n; j++)
	{
		for(i=0; i<j; i++)
		{
			sum = *(a+i*n+j);
			for(k=0; k<i; k++)  sum -= (*(a+i*n+k))*(*(a+k*n+j));
			*(a+i*n+j) = sum;
		}
		
		big = 0.0;
		for(i=j; i<n; i++)
		{
			sum = *(a+i*n+j);
			for(k=0; k<j; k++)
				sum -= (*(a+i*n+k))*(*(a+k*n+j));
			*(a+i*n+j) = sum;
			if((dum=vv[i] * fabs(sum)) >= big)
			{
				big = dum;
				imax = i;
			}
		}
		
		if(j != imax)
		{
			for(k=0; k<n; k++)
			{
				dum = *(a+imax*n+k);
				*(a+imax*n+k) = *(a+j*n+k);
				*(a+j*n+k) = dum;
			}
			*d = -(*d);
			swap = vv[imax];
			vv[imax] = vv[j];
			vv[j] = swap;
		}
		indx[j] = imax;
		if(*(a+j*n+j) == 0.0)  *(a+j*n+j) = 1.0e-20;
		if(j != n -1 )
		{
			dum = 1.0/(*(a+j*n+j));
			for(i=j+1; i<n; i++)  *(a+i*n+j) *= dum;
		}
	}
	delete [] vv;
	
	return  true;
}

//////////////////////////////////////////////////////////////////
//求解n维线性方程A*x=B
void CGlobalFunc::lubksb(double *a, int n, int *indx, double b[])
{
	int      i, ii=-1, ip, j;
	double   sum;
	
	for(i=0; i<n; i++)
	{
		ip = indx[i];
		sum = b[ip];
		b[ip] = b[i];
		if(ii>=0)
			for(j=ii; j<=i-1; j++)  sum -= (*(a+i*n+j))*(b[j]);
			else if  (sum) ii = i;
			b[i] = sum;
	}
	
	for(i=n-1; i>=0; i--)
	{
		sum = b[i];
		for(j=i+1; j<n; j++)  sum -= (*(a+i*n+j))*(b[j]);
		b[i] = sum/(*(a+i*n+i));
	}
}

//////////////////////////////////////////////////////////////////////
// In MatrixComputReverse : a will be distroyed
BOOL CGlobalFunc::MatrixComputReverse(double *a, double *y,int n)
{
	
	double   d, *col;
	int      i, j, *indx;
	
	col = new double[n];
	indx = new int[n];
	
	if (!ludcmp(a, n, indx, &d)) 
	{
		delete [] col;
		delete [] indx;
		return false;
	}
	
	for(j=0; j<n; j++)
	{
		for(i=0; i<n; i++)  col[i] = 0.0;
		
		col[j] = 1.0;
		lubksb(a, n, indx,col);
		for(i=0; i<n; i++)  *(y+i*n+j) = col[i];
	}
	
	delete [] col;
	delete [] indx;
	
	return true;
	
}


/////////////////////////////////////////////////////////////////////////////////////
//comput MatrixMultiply
BOOL	CGlobalFunc::MatrixMultiply(double* pScr1, double* pScr2, double* pDst, int m, int n, int l, int p)
{
	int		i, j, k;
	
	ASSERT(n == l);
	
	double	sum = 0.0;
	double* pDstMatrix = pDst;
	
	for(i=0; i<m; i++)
	{
		for(j=0; j<p; j++)
		{
			sum = 0;
			for(k=0; k<n; k++)
			{
				sum += (*(pScr1+i*n+k)) * (*(pScr2+k*p+j));
			}
			*pDstMatrix++ = sum;
		}
	}
	
	return TRUE;
	
}

void CGlobalFunc::ScaleVector(double * src,double * dest, int length, double value)
{
	int i;
	double val = value;
	
	for( i = 0; i < length; i++ )
	{
		dest[i] = src[i] * val;
	}
	
}

void CGlobalFunc::SubVector(double * src1,double * src2, double * dest, int length) 
{
	int i;
	
	for( i = 0; i < length; i++ )
	{
		dest[i] = src1[i] - src2[i];
	}
	
}

/*

  Note: 
  pA: should be a n-row, m-col matrix, vectorized by row-first order
  
*/

void	CGlobalFunc::PseudoInverseMatrix(double *pA, double* pA_PT, long n, long m)
{
	int     i,j;
	
	double 	*p, *p2;
	p = pA;
	
	//compute the reverse matrix of A
	double  *pAT = new double[m*n];
	p = pAT; p2 = pA;
	for(i=0; i<m; i++)
		for(j=0; j<n; j++)
			*(p+i*n+j) = *(p2+j*m+i);
		
	//multiply A' and A
	double   *pATA = new double[m*m];
	MatrixMultiply(pAT, pA, pATA, m, n, n, m);
	
	//compute matrix D = (A'A)~1
	double*	pD = new double[m*m];
	MatrixComputReverse(pATA, pD, m);
	
	//compute matrix the pseudo-reverse of A pA_PT = D * A'
	MatrixMultiply(pD, pAT, pA_PT, m, m, m, n);
	
	delete[] pAT;
	delete[] pATA;
	delete[] pD;
}

int		Convert2Stride(int	nWidth)
{
	 return ((nWidth%4)?(nWidth+4-nWidth%4):(nWidth));
}

/* To normalize the vector to be length = 1 */
double NormalVector2UnitLength(double* vector, long N)
{
	long j;
	double sum = 0.0;
    double  *pv = vector;
	
	for(j=0; j <N; j++, pv++)
		sum += (*pv) * (*pv);
	sum = sqrt(sum); // it is ||vector|| 
	
	pv  = vector;
	for(j=0; j<N; j++)
		*pv++ /= sum;  

	return sum;
}

/* 
   First, To Normalize the vector to be 0-mean and 1-variance;
   Then,  Normalize the vector to length 1
*/
double NormalVector2ZeroMeanUnitVar(double* vector, long N)
{
	long j;
	double std=0.0, mean = 0.0;
    double  *pv;

	pv = vector;
	for(j=0; j<N; j++)
		mean += *pv++;
	mean /= N;
	
	pv  = vector;
	for(j=0; j<N; j++, pv++)
		std += (*pv-mean)*(*pv-mean);
	std = sqrt(std/(N-1));

	pv  = vector;
	for(j=0; j<N; j++, pv++)
		*pv = (*pv-mean)/std;

	pv  = vector;
	NormalVector2UnitLength(pv, N);//Still need to normalize to length '1'

	return mean;
}

double InnerProduct(double* pv1,double* pv2, long N) 
{
	long   i;
	double  sum=(double)0.0;

	double* q1=pv1;
	double* q2=pv2;
	
	for(i=0;i<N;i++)
		sum += (*q1++) * (*q2++);
	
	return sum;
}

double PointDistance(CPoint p1, CPoint p2)
{
	return (double) sqrt( (double)((p2.y-p1.y) * (p2.y-p1.y) + (p2.x-p1.x) * (p2.x-p1.x)) );
}

double EuclidDistance(double* pv1, double* pv2, long N)
{
	long	i;
	double	dis = 0.0;

	double *pf1 = pv1;
	double *pf2 = pv2;

	for(i=0; i<N; i++, pf1++, pf2++)
		dis += (*pf1 - *pf2) * (*pf1 - *pf2);

	dis = (double)sqrt(dis);
	return dis;
}

double BlockDistance(double* pv1, double* pv2, long n)
{
	long	i;
	double	dis = 0.0;
	
	double *pf1 = pv1;
	double *pf2 = pv2;

	for(i=0; i<n; i++, pf1++, pf2++)
		dis += fabs(*pf1 - *pf2);

	dis /= n;

	return dis;
}

double	MahalanobisDistance(double* pv1, double* pv2, double* sigma, int nDim)
{
	int		i;
	double	dis = 0.0;
	double *pf1 = pv1;
	double *pf2 = pv2;
	double *pf3 = sigma;

	for(i=0; i<nDim; i++, pf1++, pf2++)
		dis += (((*pf1 - *pf2)*(*pf1 - *pf2)) / (*pf3++));

	dis = (double)sqrt(dis);
	return dis;
}

double	VectorSimilarity(double* pv1, double* pv2, int nDim)
{
	double similarity = InnerProduct(pv1, pv2, nDim);
	similarity /= sqrt(InnerProduct(pv1, pv1, nDim));
	similarity /= sqrt(InnerProduct(pv2, pv2, nDim));
	return similarity;
}

/*
Function: Calculate the eigenvalues of the matrix matrix;
double* matrix:		a  n*n_D-real-symmetry matrix;
long n				the dimension of the matrix matrix;
doube* eigenvalue:	the n_D vector that contains the n eigenvalues of the matrix that the function calculated;
double* c:			The temporary n*n_D vector that used by the function;
Note:
When the function returns, the vector eigenvalue contains the eigenvalues!
*/
///////////////////////////////////////////////////////////////////
void CalEgvalue(double *matrix,long n,double *eigenvalue,double *c)
{
	long i,j,k,u;
	double h,f,g,h2;
	for(i=n-1;i>=1;i--){
        printf("\rloop1 times: %d   ",i);
		h=0.0;
		if(i>1)
			for(k=0;k<=i-1;k++){
				u=i*n+k;
				h=h+matrix[u]*matrix[u];
			}
			if(h+1.0==1.0){
				c[i-1]=0.0;
				if(i==1) c[i-1]=matrix[i*n+i-1];
				eigenvalue[i]=0.0;
			}
			else{
				c[i-1]=sqrt(h);
				u=i*n+i-1;
				if(matrix[u]>0.0) c[i-1]=-c[i-1];
				h=h-matrix[u]*c[i-1];
				matrix[u]=matrix[u]-c[i-1];
				f=0.0;
				for(j=0;j<=i-1;j++){//j=0;
					matrix[j*n+i]=matrix[i*n+j]/h;
					g=0.0;
					for(k=0;k<=j;k++) 
						g=g+matrix[j*n+k]*matrix[i*n+k];
					if(j+1<=i-1)
						for(k=j+1;k<=i-1;k++) 
							g=g+matrix[k*n+j]*matrix[i*n+k];
						c[j-1]=g/h;//////////////////////////////////////??????????????????????????
						f=f+g*matrix[j*n+i];
				}
				h2=f/(h+h);
				for(j=0;j<=i-1;j++){//j=0;
					f=matrix[i*n+j];
					g=c[j-1]-h2*f;//////////////////////////////////////??????????????????????????
					c[j-1]=g;
					for(k=0;k<=j;k++){//k=0;
						u=j*n+k;
						matrix[u]=matrix[u]-f*c[k-1]-g*matrix[i*n+k];//////////////////////////////////////??????????????????????????
					}
				}
				eigenvalue[i]=h;
			}
	}
	eigenvalue[0]=0.0;
	for(i=0;i<=n-1;i++){
        printf("\rloop2 times: %d   ",i);
		if((eigenvalue[i]!=0.0)&&(i-1>=0))
			for(j=0;j<=i-1;j++){
				g=0.0;
				for(k=0;k<=i-1;k++) g=g+matrix[i*n+k]*matrix[k*n+j];
				for(k=0;k<=i-1;k++){
					u=k*n+j;
					matrix[u]=matrix[u]-g*matrix[k*n+i];
				}
			}
			u=i*n+i;
			eigenvalue[i]=matrix[u];
			matrix[u]=1.0;
			if(i-1>=0)
				for(j=0;j<=i-1;j++){
					matrix[i*n+j]=0.0;
					matrix[j*n+i]=0.0;
				}
	}
}
///////////////////////////////////////////////////////////////////
/*
Function: Calculate the eigenvalues of the matrix matrix;
double* matrix   :	a  n*n_D-real-symmetry matrix;
long n			 :	the dimension of the matrix matrix;
doube* eigenvalue:	the n_D vector that contains the n eigenvalues of the matrix that the function calculated;
double* c        :	The temporary n*n_D vector that used by the function;
doube eps		 :	The approximate of 00000000
Note:
When the function returns, the vector eigenvalue contains the eigenvalues!
and the matrix matrix contains the n eigenvectors. 
*/
///////////////////////////////////////////////////////////////////
long CalEgvector(double *matrix,long n,double *eigenvalue,double *c,double eps)
{
	long   i,j,k,m,u,v;
	double d,f,h,g,p,r,e,s;
	
	c[n-1]=0.0;
	d=0.0;
	f=0.0;
	for(j=0;j<=n-1;j++){
        printf("\rloop3 times: %d   ",j);
		h=eps*(fabs(eigenvalue[j])+fabs(c[j]));
		if(h>d) d=h;
		m=j;
		while((m<=n-1)&&(fabs(c[m])>d)) m=m+1;
		
		if(m!=j) do{
			g=eigenvalue[j];
			p=(eigenvalue[j+1]-g)/(2.0*c[j]);
			r=sqrt(p*p+1.0);
			if(p>=0.0) eigenvalue[j]=c[j]/(p+r);
			else eigenvalue[j]=c[j]/(p-r);
			h=g-eigenvalue[j];
			for(i=j+1;i<=n-1;i++) eigenvalue[i]=eigenvalue[i]-h;
			f=f+h;
			p=eigenvalue[m];
			e=1.0;
			s=0.0;
			for(i=m-1;i>=j;i--){
				g=e*c[i];
				h=e*p;
				if(fabs(p)>=fabs(c[i])){
					e=c[i]/p;
					r=sqrt(e*e+1.0);
					c[i+1]=s*p*r;
					s=e/r;
					e=1.0/r;
				}
				else{
					e=p/c[i];
					r=sqrt(e*e+1.0);
					c[i+1]=s*c[i]*r;
					s=1.0/r;
					e=e/r;
				}
				p=e*eigenvalue[i]-s*g;
				eigenvalue[i+1]=h+s*(e*g+s*eigenvalue[i]);
				for(k=0;k<=n-1;k++){
					u=k*n+i+1;
					v=u-1;
					h=matrix[u];
					matrix[u]=s*matrix[v]+e*h;
					matrix[v]=e*matrix[v]-s*h;
				}
			}
			c[j]=s*p;
			eigenvalue[j]=e*p;
		}while(fabs(c[j])>d);
		
		eigenvalue[j]=eigenvalue[j]+f;
	}
	for(i=0;i<=n-1;i++){
		k=i;
		p=eigenvalue[i];
		if(i+1<=n-1){
			j=i+1;
			while((j<=n-1)&&(eigenvalue[j]<=p)){
				k=j;
				p=eigenvalue[j];
				j=j+1;
			}
		}
		if(k!=i){
			eigenvalue[k]=eigenvalue[i];
			eigenvalue[i]=p;
			for(j=0;j<=n-1;j++){
				u=j*n+i;
				v=j*n+k;
				p=matrix[u];
				matrix[u]=matrix[v];
				matrix[v]=p;
			}
		}
	}
	return(1);
}

void OrderValues(double* ev, long* order, long M)
{
    long	i,j,maxnum;
	double	mini,maxi;
	
	double* evs = new double[M];
	mini=*ev;
	for(i=0;i<M;i++)
	{
		evs[i]=*(ev+i);
		if(evs[i]<mini)  mini=evs[i];
		order[i]=i;
	}

	for(i=0;i<M;i++)
	{
		maxi=evs[i];
		maxnum=i;
		for(j=0;j<M;j++)
		{
			if(evs[j]>maxi)
			{
				maxi=evs[j];
				maxnum=j;
			} 
		}
		evs[maxnum]=mini-1.0;
		order[i]=maxnum;
	}

	delete evs;
}

BOOL	ResizeImage(BYTE* pSrcImg, int nSrcWidth, int nSrcHeight, BYTE* pDstImg, int nDstWidth, int nDstHeight)
{
	int		n_x_d, n_y_d;
	int		n_x_s, n_y_s;
	double	lfXscl, lfYScl, lf_x_s, lf_y_s, lfNewGray;
	double	lfWeight_x, lfWeight_y;

	if(nSrcWidth == nDstWidth && nSrcHeight == nDstHeight)
	{
		memcpy(pDstImg, pSrcImg, Convert2Stride(nSrcWidth)*nSrcHeight);
		return TRUE;
	}

	lfXscl = double(nSrcWidth+0.0)/nDstWidth;
	lfYScl = double(nSrcHeight+0.0)/nDstHeight;

	//if the image data is strided, open the following 2 lines
	//nSrcWidth = Convert2Stride(nSrcWidth);
	//nDstWidth = Convert2Stride(nDstWidth);

	for(n_y_d=0; n_y_d<nDstHeight; n_y_d++)
	{
		for(n_x_d=0; n_x_d<nDstWidth; n_x_d++)
		{
			lf_x_s = lfXscl * n_x_d;
			lf_y_s = lfYScl * n_y_d;
			n_x_s = int(lf_x_s);
			n_x_s = SMALLER(n_x_s, nSrcWidth-2);
			n_y_s = int(lf_y_s);
			n_y_s = SMALLER(n_y_s, nSrcHeight-2);
			lfWeight_x = lf_x_s - n_x_s;
			lfWeight_y = lf_y_s - n_y_s;
			lfNewGray = (1-lfWeight_y)*((1-lfWeight_x)*pSrcImg[n_y_s*nSrcWidth+n_x_s]+lfWeight_x*pSrcImg[n_y_s*nSrcWidth+n_x_s+1])+
						lfWeight_y*((1-lfWeight_x)*pSrcImg[(n_y_s+1)*nSrcWidth+n_x_s]+lfWeight_x*pSrcImg[(n_y_s+1)*nSrcWidth+n_x_s+1]);
			pDstImg[n_y_d*nDstWidth+n_x_d] = BYTE(lfNewGray);
		}
	}
	return TRUE;
}

BOOL RGB2YIQ(BYTE R, BYTE G, BYTE B, BYTE & Y, double & I, double & Q)
{
	Y = BYTE( 0.299 * R + 0.587 * G + 0.114 * B );
	I = 0.596 * R - 0.274 * G - 0.322 * B;
	Q = 0.211 * R - 0.523 * G + 0.312 * B;
	return TRUE;
}

BOOL RGB2Fai(BYTE R, BYTE G, BYTE B, int& Fai)
{
	double	t;
	double	u, v;

	u = -0.147 * R - 0.289 * G + 0.436 * B;
	v = 0.615  * R - 0.515 * G - 0.100 * B;
	if(u == 0.0)
	{
		t = 0;
	}
	else
	{
		if(u > 0)
		{
			if( v < 0 )
				t = atan( v / u ) + 6.2831853;
			else
				t = atan( v / u );
		}
		else
		{
			t = atan( v / u ) + 3.1415926;
		}
	}

	t *= ( 180 / 3.1415926 );
	Fai = int( t );
	if(Fai >= 0 && Fai <= 360 )
		return TRUE;
	else
		return FALSE;
}

void	DrawPlus(CDC* pDC, CPoint pnt, int size, COLORREF col)
{
	int x = pnt.x;
	int y = pnt.y;

	CPen	newPen(PS_SOLID, 1, col);
	CPen*	pOldPen = pDC->SelectObject(&newPen);
	
	pDC -> MoveTo(x, y-(size/2));
	pDC -> LineTo(x, y+(size/2)+1);
	pDC -> MoveTo(x-(size/2), y);
	pDC -> LineTo(x+(size/2)+1, y);

	pDC->SelectObject(pOldPen);
}



BOOL	CropSubImage(BYTE* pbyteSrcImgData, int nSrcImgWidth, int nSrcImgHeight, BYTE* pDstImgData, RECT subRect)
{
	/* */
	int		left	= subRect.left;
	int		top		= subRect.top;
	int		right	= subRect.right;
	int		bottom	= subRect.bottom;

	int width  = right - left + 1;
	int height = bottom - top + 1;
	
	//int stride = Convert2Stride(width);
	//int	nSrcStride = Convert2Stride(nSrcImgWidth);
	int stride = width;
	int	nSrcStride = nSrcImgWidth;
	
	ASSERT( width > 0 && height > 0 && left >= 0 && right >=0 && top >= 0 && bottom >= 0 && right < nSrcImgWidth && bottom <= nSrcImgHeight);
	
	int i;
	BYTE* pS = pbyteSrcImgData+top*nSrcStride+left;
	BYTE* pD = pDstImgData;
	for( i = 0; i < height; i++)
	{
		CopyMemory( (void*)pD, (void*)pS, width);
		pS += nSrcStride;
		pD += stride;
	}
	return true;
}
void ShrinkImage(BYTE *scrImage, BYTE* destImage, WORD w, WORD h, float cscale)
{
	DWORD		i, j, m, n, scrOff, destOff, destw, desth;
	DWORD		num,sum;
	double	step_w, step_h,tm, tn;
	
	destw = (WORD) (w * cscale);
	desth = (WORD) (h * cscale);
	
	step_w = (double)w / destw;
	step_h = (double)h / desth;
	
	for (i=0; i < desth; i++) 
	{
		tm=i*step_h;
		destOff = destw * i;
		for (j=0; j < destw; j++) 
		{
			sum=0;      
			num = 0;
			tn=j*step_w;
			for (m = (WORD)tm + 1; m <= (WORD)(tm + step_h); m++)  
			{
				scrOff=w*m;
				for (n = (WORD)tn + 1; n <= (WORD)(tn + step_w); n++)  
				{
					sum += scrImage[scrOff+n];
					if(scrOff >= (DWORD)(w * h)) 
						break;
					num++;
				}
			}
			if(num!=0) 
				destImage[destOff + j] = (BYTE)(sum/num);
			else  
				destImage[destOff + j]=(BYTE)sum;
		}
	}
}
////////////////////////////////////////////////////////////////////////
//author:   ssg					Time: 1998.4.18
//Para1:	BYTE* scrImage:		the source image data poiter
//Para2:	BYTE* destImage:	the resulting image data poiter
//Para3,4:	WORD scrW, scrH:	source image's width and height in pixel
//para5,6:	WORD destW,destH:	destinate image's width and height in pixel
//////////////////////////////////////////////////////////////////////////
void ShrinkImage(BYTE *scrImage, BYTE* destImage, WORD scrW, WORD scrH,WORD destW, WORD destH)
{
	DWORD		i, j, m, n,scrOff, destOff;
	DWORD		num,sum;
	double		step_w, step_h,tm,tn;
	
	step_w = (double)scrW / destW;
	step_h = (double)scrH / destH;
	
	for (i=0; i < destH; i++) 
	{
		tm  = i * step_h;
		destOff = i * destW;
		for (j=0; j < destW; j++) 
		{
			sum = 0; 
			num = 0;
			tn  = j*step_w;
			for (m = (DWORD)tm + 1; m <= (DWORD)(tm + step_h); m++)  
			{
				scrOff = m * scrW;
				for (n = (DWORD)tn + 1; n <= (DWORD)(tn + step_w); n++)  
				{
					if(scrOff + n >= (DWORD)(scrW * scrH))  break;
					sum += scrImage[scrOff + n];
					num++;
				}
			}
			if(num!=0) 
				destImage[destOff + j] = (BYTE)(sum / num);
			else  
				destImage[destOff + j] = (BYTE)sum;
		}
	}
}

////////////////////////////////////////////////////////////////////////
//author:   ssg					Time: 1998.4.18
//Para1:	BYTE* scrImage:		the source image data poiter
//Para2:	BYTE* destImage:	the resulting image data poiter
//Para3,4:	WORD scrW, scrH:	source image's width and height in pixel
//para5,6:	WORD destW,destH:	destinate image's width and height in pixel
//////////////////////////////////////////////////////////////////////////
void MagnifyImage(BYTE *scrImage, BYTE* destImage, WORD scrW, WORD scrH,WORD destW, WORD destH)
{
	DWORD i, j;
	double fGrey;
	double scrRow, scrVol;
	double coefX, coefY;
	DWORD scrOffset, destOffset;
	double xScale, yScale;	//the magnify scale of x,y direction
	
	ASSERT(destW >= scrW);
	ASSERT(destH >= scrH);
	
	xScale = (double)destW * 1.0 / scrW;
	yScale = (double)destH * 1.0 / scrH;
	
	for(i=0; i<destH; i++)
	{
		scrRow = i * 1.0 / yScale;
		if(((WORD)scrRow) >= scrH)
			scrRow = scrH - 1.0;
		//scrOffset = scrRow * scrW;
		destOffset = i * destW;
		for(j=0; j<destW; j++)
		{
			scrVol = j * 1.0 / xScale;
			if(((WORD)scrVol) >= scrW)
				scrVol = scrW - 1.0; //内插方法
			coefX = scrVol - int(scrVol);
			coefY = scrRow - int(scrRow);
			scrOffset = ((int)scrRow) * scrW + (int)scrVol;
			fGrey = (1-coefX) * (float)scrImage[scrOffset] + coefX * (float)scrImage[scrOffset + 1];
			fGrey = (1-coefY) * fGrey + coefY * ((1-coefX) * (float)scrImage[scrOffset + scrW] + coefX * (float)scrImage[scrOffset + scrW + 1]);
			destImage[destOffset + j] = int (fGrey + 0.5);
		}
	}
}

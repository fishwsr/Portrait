#include "stdafx.h"
#include "myrandom.h"
#include <math.h>
#include <time.h>

#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-7
const float RNMX= 1.0-EPS; 
#define IM1 2147483563
#define IMM1 (IM1-1)
#define IM2 2147483399
#define IA1 40014
#define IA2 40692
#define IQ1 53668
#define IQ2 52774
#define IR1 12211
#define IR2 3791
#define MBIG 1000000000
#define MSEED 161803398
#define MZ 0
const float FAC=1.0/MBIG;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//set seed of random number generator
void InitMyRand(long seed);

class CRandInit
{
public:
	CRandInit();
protected:
	static bool m_fInitialized;
	static int m_Method;
};

bool CRandInit::m_fInitialized = false;
int CRandInit::m_Method = 2;
CRandInit g_RandInit;

CRandInit::CRandInit()
{
	if (!m_fInitialized)
	{
		// Initialize
		m_fInitialized = true;
		time_t tempt;
		InitMyRand((long)time(&tempt));
		MyRand(m_Method);
	}
}

float mmran1(long *idum)
{
	int j;
	long k;
	static long iy=0;
	static long iv[NTAB];
	float temp;
	if (*idum <= 0 || !iy) 
	{ 
		if (-(*idum) < 1) 
			*idum=1; 
		else 
			*idum = -(*idum);
		for (j=NTAB+7;j>=0;j--) 
		{ 
			k=(*idum)/IQ;
			*idum=IA*(*idum-k*IQ)-IR*k;
			if (*idum < 0) *idum += IM;
			if (j < NTAB) iv[j] = *idum;
		}
		iy=iv[0];
	}
	k=(*idum)/IQ; 
	*idum=IA*(*idum-k*IQ)-IR*k; 
	 if (*idum < 0) *idum += IM;
	j=iy/NDIV; 
	iy=iv[j]; 
	iv[j] = *idum;
	if ((temp=AM*iy) > RNMX) return RNMX; 
	else return temp;
}

float mmran2(long *idum)
{
	int j;
	long k;
	static long idum2=123456789;
	static long iy=0;
	static long iv[NTAB];
	float temp;
	
	if (*idum <= 0) 
	{ 
		if (-(*idum) < 1) *idum=1; 
		else *idum = -(*idum);
		idum2=(*idum);
		for (j=NTAB+7;j>=0;j--) 
		{ 
			k=(*idum)/IQ1;
			*idum=IA1*(*idum-k*IQ1)-k*IR1;
			if (*idum < 0) *idum += IM1;
			if (j < NTAB) iv[j] = *idum;
		}
		iy=iv[0];
	}
	
	k=(*idum)/IQ1; //Start here when not initializing.
	*idum=IA1*(*idum-k*IQ1)-k*IR1; //Compute idum=(IA1*idum) % IM1 withoutover ows by Schrage's method. 
	if (*idum < 0) *idum += IM1;
	k=idum2/IQ2;
	idum2=IA2*(idum2-k*IQ2)-k*IR2; //Compute idum2=(IA2*idum) % IM2 likewise.
	if (idum2 < 0) idum2 += IM2;
	j=iy/NDIV; //Will be in the range 0..NTAB-1.
	iy=iv[j]-idum2; //Here idum is shu.ed, idum and idum2 arecombined to generate output. 
	iv[j] = *idum;
	if (iy < 1) iy += IMM1;
	if ((temp=AM*iy) > RNMX) return RNMX; //Because users don't expect endpoint values.
	else return temp;
}


float mmran3(long *idum)
{
	static int inext,inextp;
	static long ma[56]; 
	static int iff=0;
	long mj,mk;
	int i,ii,k;
	
	if (*idum < 0 || iff == 0) 
	{ //Initialization.
		iff=1;
		mj=labs(MSEED-labs(*idum)); 
		mj %= MBIG;
		ma[55]=mj;
		mk=1;
		for (i=1;i<=54;i++) 
		{ 
			ii=(21*i) % 55; 
			ma[ii]=mk; 
			mk=mj-mk;
			if (mk < MZ) mk += MBIG;
			mj=ma[ii];
		}
		
		for (k=1;k<=4;k++) 
			 for (i=1;i<=55;i++) 
			 {
				ma[i] -= ma[1+(i+30) % 55];
				if (ma[i] < MZ) ma[i] += MBIG;
			 }
		
		inext=0; 
		inextp=31; 
		*idum=1;
	}

	if (++inext == 56) inext=1; 
	if (++inextp == 56) inextp=1;
	mj=ma[inext]-ma[inextp]; 
	if (mj < MZ) mj += MBIG; 
	ma[inext]=mj; 
	return mj*FAC; 
}

static long my_idum_data=-1;
float MyRand(int method)
{
	switch(method)
	{
	case 1:
		return mmran1(&my_idum_data);
	case 2:
		return mmran2(&my_idum_data);
	case 3:
		return mmran3(&my_idum_data);
	default:
		return mmran1(&my_idum_data);
	}
}

void InitMyRand(long seed)
{
	my_idum_data=seed;
}

/* GAUSSIAN GENERATOR.  Done by using the Box-Muller method, but only one
   of the variates is retained (using both would require saving more state).
   See Devroye, p. 235. 

   As written, should never deliver exactly zero, which may sometimes be
   helpful. */

double rand_gaussian (void)
{
  double a, b;

  a = MyRand(2);
  b = MyRand(2);

  return cos(2.0*M_PI*a) * sqrt(-2.0*log(b));
}


/* EXPONENTIAL GENERATOR.  See Devroye, p. 29.  Written so as to never
   return exactly zero. */

double rand_exp (void)
{
  return -log(MyRand(2));
}


/* CAUCHY GENERATOR.  See Devroye, p. 29. */

double rand_cauchy (void)
{
  return tan (M_PI * (MyRand(2)-0.5));
}


/* GAMMA GENERATOR.  Generates a positive real number, r, with density
   proportional to r^(a-1) * exp(-r).  See Devroye, p. 410 and p. 420. 
   Things are fiddled to avoid ever returning a value that is very near 
   zero. */

double rand_gamma
( double a
)
{
  double b, c, X, Y, Z, U, V, W;

  if (a<0.00001)
  { X = a;
  }

  else if (a<=1) 
  { 
    U = MyRand(2);
    X = rand_gamma(1+a) * pow(U,1/a);
  }

  else if (a<1.00001)
  { X = rand_exp();
  }

  else
  {
    b = a-1;
    c = 3*a - 0.75;
  
    for (;;)
    {
      U = MyRand(2);
      V = MyRand(2);
    
      W = U*(1-U);
      Y = sqrt(c/W) * (U-0.5);
      X = b+Y;
  
      if (X>=0)
      { 
        Z = 64*W*W*W*V*V;
  
        if (Z <= 1 - 2*Y*Y/X || log(Z) <= 2 * (b*log(X/b) - Y)) break;
      }
    }
  }

  return X<1e-30 && X<a ? (a<1e-30 ? a : 1e-30) : X;
}


/* BETA GENERATOR. Generates a real number, r, in (0,1), with density
   proportional to r^(a-1) * (1-r)^(b-1).  Things are fiddled to avoid
   the end-points, and to make the procedure symmetric between a and b. */

double rand_beta 
( double a, 
  double b
)
{
  double x, y, r;

  do
  { x = rand_gamma(a);
    y = rand_gamma(b);
    r = 1.0 + x/(x+y);
    r = r - 1.0;
  } while (r<=0.0 || r>=1.0);

  return r;
}



#include "stdafx.h"
#include "NatCubic.h"

/* calculates the natural cubic spline that interpolates
   y[0], y[1], ... y[n]
   The first segment is returned as
   C[0].a + C[0].b*u + C[0].c*u^2 + C[0].d*u^3 0<=u <1
   the other segments are in C[1], C[2], ...  C[n-1] */

vector<Cubic> NatCubic::calcNaturalCubic(int n, vector<double> &x)
{
	double *gamma = new double[n+1];
	double *delta = new double[n+1];
	double *D     = new double[n+1];
	
	/* We solve the equation
	   [2 1       ] [D[0]]   [3(x[1] - x[0])  ]
	   |1 4 1     | |D[1]|   |3(x[2] - x[0])  |
	   |  1 4 1   | | .  | = |      .         |
	   |    ..... | | .  |   |      .         |
	   |     1 4 1| | .  |   |3(x[n] - x[n-2])|
	   [       1 2] [D[n]]   [3(x[n] - x[n-1])]
       
	   by using row operations to convert the matrix to upper triangular
	   and then back sustitution.  The D[i] are the derivatives at the knots.
	*/

	int i;
	gamma[0] = 1.0f/2.0f;
	for ( i = 1; i < n; i++)
	{
		gamma[i] = 1/(4-gamma[i-1]);
	}
	gamma[n] = 1/(2-gamma[n-1]);
    
	delta[0] = 3*(x[1]-x[0])*gamma[0];
		
	for ( i = 1; i < n; i++)
	{
		delta[i] = (3*(x[i+1]-x[i-1])-delta[i-1])*gamma[i];
	}
	delta[n] = (3*(x[n]-x[n-1])-delta[n-1])*gamma[n];
    
	D[n] = delta[n];
	for ( i = n-1; i >= 0; i--)
	{
		D[i] = delta[i] - gamma[i]*D[i+1];
	}

	/* now compute the coefficients of the cubics */
	vector<Cubic> C(n);
	for ( i = 0; i < n; i++)
	{
		C[i].Init((double)x[i], D[i], 3*(x[i+1] - x[i]) - 2*D[i] - D[i+1], 2*(x[i] - x[i+1]) + D[i] + D[i+1]);
	}
	return C;
}


	

/* get a cubic spline curve*/
void NatCubic::Spline(ControlPoints &pts, ControlPoints &p)
{
	int N = pts.npoints;
		
	if (pts.npoints >= 2)
	{			
		vector<Cubic> X = calcNaturalCubic(pts.npoints-1, pts.xpoints);
		vector<Cubic> Y = calcNaturalCubic(pts.npoints-1, pts.ypoints);    
			
		for (int i = 0; i < int(X.size()); i++)
		{			
			int STEPS = int(max(abs(pts.xpoints[i+1] - pts.xpoints[i]),
								abs(pts.ypoints[i+1] - pts.ypoints[i])) * 1.2);
					
			cout << STEPS << endl;
				
			for (int j = 0; j <= STEPS; j++)
			{		
				double u = j / (double) STEPS;
				p.AddPoints(X[i].eval(u), Y[i].eval(u));
			}
		}
			
	}
}

/* get a cubic spline curve*/
void NatCubic::Spline(ControlPoints &pts, vector<int> &pointsx, vector<int> &pointsy)
{

	ControlPoints p;
	int N = pts.npoints;
		
	if (pts.npoints >= 2)
	{			
		vector<Cubic> X = calcNaturalCubic(pts.npoints-1, pts.xpoints);
		vector<Cubic> Y = calcNaturalCubic(pts.npoints-1, pts.ypoints);
    
			
		for (int i = 0; i < int(X.size()); i++)
		{			
			int STEPS = int(max(abs(pts.xpoints[i+1] - pts.xpoints[i]),
								abs(pts.ypoints[i+1] - pts.ypoints[i])) * 1.2);

			for (int j = 0; j <= STEPS; j++)
			{
									
		
				double u = j / (double) STEPS;
				p.AddPoints(X[i].eval(u), Y[i].eval(u));

			}
		}

		N = p.npoints;

		pointsx = vector<int> (N);
		pointsy = vector<int> (N);

		for(i=0; i<N; i++)
		{
			pointsx[i] = int(p.xpoints[i]);
			pointsy[i] = int(p.ypoints[i]);				
		}			
	}		
}	

/* get a cubic spline curve for curve morphint*/
void NatCubic::Spline(vector<DoublePoint> &InPoints, vector<DoublePoint> &OutPoints)
{

	int N = int(InPoints.size());

	ControlPoints pts;
	
	for(int i=0; i<N; i++)
		pts.AddPoints(InPoints[i].x, InPoints[i].y);

	ControlPoints p;
		
	if (pts.npoints >= 2)
	{			
		vector<Cubic> X = calcNaturalCubic(pts.npoints-1, pts.xpoints);
		vector<Cubic> Y = calcNaturalCubic(pts.npoints-1, pts.ypoints);
    
			
		for (int i = 0; i < int(X.size()); i++)
		{			
			int STEPS = int(max(abs(pts.xpoints[i+1] - pts.xpoints[i]),
								abs(pts.ypoints[i+1] - pts.ypoints[i])) * 1.2);

			for (int j = 0; j <= STEPS; j++)
			{
									
		
				double u = j / (double) STEPS;
				p.AddPoints(X[i].eval(u), Y[i].eval(u));

			}
		}

		N = p.npoints;
		OutPoints = vector<DoublePoint> (N);

		for(i=0; i<N; i++)
		{
			OutPoints[i].x = p.xpoints[i];
			OutPoints[i].y = p.ypoints[i];				
		}			
	}		
}	

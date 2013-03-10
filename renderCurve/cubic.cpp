#include "stdafx.h"
#include "cubic.h"

ControlPoints::ControlPoints()
{		
	npoints = 0;
}

void ControlPoints::AddPoints(double x, double y)
{
	xpoints.push_back(x);
	ypoints.push_back(y);
	
	++npoints;	
}


void Cubic::Init(double a1, double b1, double c1, double d1)
{		
	a = a1;
	b = b1;
	c = c1;
	d = d1;
}

	
Cubic::Cubic()
{
	
}

/** evaluate cubic */
double Cubic::eval(double u)
{
	return (((d*u) + c)*u + b)*u + a;
}

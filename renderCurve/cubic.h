#ifndef CUBIC_H
#define CUBIC_H

#include <vector>
#include <cstdlib>

using namespace std;

class ControlPoints{

public:
	
	vector<double> xpoints;
	vector<double> ypoints;

	int npoints;

	
public:
	
	ControlPoints();
	
	void AddPoints(double x, double y);
};


class Cubic{
	
	double a,b,c,d;         /* a + b*u + c*u^2 +d*u^3 */

public:
	
	void Init(double a1, double b1, double c1, double d1);

	
	Cubic();
	
public:

	/** evaluate cubic */
	double eval(double u);	
};

#endif //CUBIC_H

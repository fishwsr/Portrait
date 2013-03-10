#ifndef NATCUBIC_H
#define NATCUBIC_H

#include "MyPoint.h"
#include "cubic.h"
#include <cmath>

class NatCubic{
  
public:
	vector<Cubic> calcNaturalCubic(int n, vector<double> &x);

	void Spline(ControlPoints &pts, ControlPoints &p);

	void Spline(ControlPoints &pts, vector<int> &pointsx, vector<int> &pointsy);
    
	void Spline(vector<DoublePoint> &InPoints, vector<DoublePoint> &OutPoints);
};

#endif //NATCUBIC_H
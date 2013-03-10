///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisMedian.h -- templated function for median computation
//
// DESCRIPTION
//  The templated VisMedian function computes the median
//  (or other order statisticelement) of a list of objects
//  using the expected linear time randomized select algorithm
//  [Introduction to Algorithms, Cormen, Leiserson & Rivest, 1992, p. 187]
//
//  The algorithm is re-written slightly, and all of the code for
//  Randomized-Partition and Partition is folded in.
//
//  To use this routine, you need to first define a less_than_equal
//  function object and then call it like this:
//
//  struct FloatCompare {  // compare floats
//      int operator()(float& x, float& y) { return x <= y; }
//  }
//  ...
//  float l[4] = {3, 4, 2, 1};
//  float med = VisMedian(FloatCompare(), l, 4, 4/2);
//
//  A more complex example shows how to pass arguments to the comparison
//  function:
//
//
//  struct ColorCompare {  // compare colors using red or green band
//      int r;
//      ColorCompare(int red) : r(red) {}
//      int operator()(CVisColorBytePixel& p1, CVisColorBytePixel& p2) {
//          return r ? p1.R() <= p2.R() : p1.G() <= p2.G();
//  }
//  ...
//  CVisColorBytePixel *l;
//  ...
//  CVisColorBytePixel med = VisMedian(ColorCompare(1), l, n, n/2);
//
// SEE ALSO
//  Standard Template Library (STL) for the concept of function objects.
//  VisComposite.cpp    code used to median blend images
//  VisAlign.cpp        code used to compute median block error
//
// BUGS
//
//
// DESIGN
//  In the future, I want to add code to do a "weighted median", so
//  that we can combined the benefits of feathering and median filtering.
//
// HISTORY
//  10-Sep-97  Richard Szeliski (szeliski) at Microsoft
//  Created.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////

#ifndef __VIS_MEDIAN_H__
#define __VIS_MEDIAN_H__

template <class TFnOpLE, class TElement>
static TElement VisMedian(TFnOpLE& less_than_equal, TElement *l, int n, int m)
{
    if (n <= 1)
        return l[0];

    // Use the expected linear time algorithm from Leiserson's book...
    int r = rand() * n / (RAND_MAX+1);      // RAND_MAX * n must be < 2^31!
    assert(0 <= r && r < n);
    TElement p = l[r];        // pivot
    l[r] = l[0];
    l[0] = p;

    // Re-arrange the array into lower and upper "halves"
    for (int i = -1, j = n; i < j; ) {
        do i++; while (! less_than_equal(p, l[i]));
        do j--; while (! less_than_equal(l[j], p));
        if (i < j) {
            TElement t = l[i];
            l[i] = l[j];
            l[j] = t;
        }
    }

    // Recurse on the proper half...
    j += 1;
    if (m < j)
        return VisMedian(less_than_equal, l, j, m);
    else
        return VisMedian(less_than_equal, &l[j], n-j, m-j);
}

#endif // __VIS_MEDIAN_H__

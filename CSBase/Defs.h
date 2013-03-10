
#ifndef MY_PI
#define MY_PI 3.1415926
#endif

#ifndef SQUARE
#define SQUARE(x) ((x)*(x))
#endif

#ifndef ROUND
#define ROUND(x) int(x+0.5)
#endif

#ifndef SIGN
#define SIGN(x) ((x>=0)?1:-1)
#endif

#ifndef NEARLY_EQUAL
#define NEARLY_EQUAL(x, y) (fabs(x-y)<=NEARLY_ZERO?1:0)
#endif

#ifndef NEARLY_ZERO
#define NEARLY_ZERO 0.00001
#endif

#ifndef RECT_CX
#define RECT_CX(rect) (0.5*(rect.left+rect.right))
#endif

#ifndef RECT_CY
#define RECT_CY(rect) (0.5*(rect.top+rect.bottom))
#endif

#ifndef RECT_WD
#define RECT_WD(rect) (rect.right-rect.left+1)
#endif

#ifndef RECT_HT
#define RECT_HT(rect) (rect.bottom-rect.top+1)
#endif

#ifndef LIMIT_TO_INTERVAL
#define LIMIT_TO_INTERVAL(a, min, max) ((a)>=(min)?((a)<=(max)?(a):((max)>(min)?(max):(min))):((a)>=(max)?(a):((max)>(min)?(min):(max))))  
#endif


///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisTransform4x4.cpp -- basic geometric transformations
//
// DESCRIPTION
//  The CVisTransform4x4 class represents geometric transformation
//  in 4-D homogeneous coordinates (projective space), in a manner
//  similar to that used in 3D graphics packages such as
//  OpenGL.  A 4x4 homogeneous coordinate matrix is used to represent
//  the mapping between one or more points in the world to each
//  screen pixel.
//
//
// SEE ALSO
//  VisResample.h     image resampling algorithm
//  VisGeometry.h     coordinate transformation chains (stacks)
//
//  Vision Library      Technical Note No. 1 (Vision Technology Group)
//  OpenGL Programming Guide
//
// BUGS
//  
//
// DESIGN
//  Jeremy deBonet suggested that CVisVector4 and CVisTransform4x4
//  be derived from CVisDVector and CVisDMatrix.  This is a reasonable
//  idea, as it cleans up the class structure, at the cost of a slight
//  loss in efficiency in accessing vector/matrix elements.
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////


// Precompiled header file
#include "VisMatrixPch.h"
#include "VisMatrixProj.h"


#include <assert.h>
#include <ctype.h>
#include <memory.h>
#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



VisMatrixExport CVisVector4 CVisVector4::Normalized(void) const
{
	CVisVector4 vector4Ret(false);
	double dblMagnitude = Magnitude();

	if (dblMagnitude > 0.0)
	{
		vector4Ret[0] = m_rgdbl[0] / dblMagnitude;
		vector4Ret[1] = m_rgdbl[1] / dblMagnitude;
		vector4Ret[2] = m_rgdbl[2] / dblMagnitude;
	}
	else
	{
		vector4Ret[0] = vector4Ret[1] = vector4Ret[2] = 0.0;
	}

	vector4Ret[3] = 1.0;

	return vector4Ret;
}

VisMatrixExport CVisVector4& CVisVector4::Normalize(void)
{
	double dblMagnitude = Magnitude();

	if (dblMagnitude > 0.0)
		*this /= dblMagnitude;

	m_rgdbl[3] = 1.0;

	return *this;
}

VisMatrixExport CVisVector4& CVisVector4::EqSum(const CVisVector4 &refvectorA,
		const CVisVector4 &refvectorB)
{
	m_rgdbl[0] = refvectorA[0] + refvectorB[0];
	m_rgdbl[1] = refvectorA[1] + refvectorB[1];
	m_rgdbl[2] = refvectorA[2] + refvectorB[2];
	m_rgdbl[3] = 1.0;
    return *this;
}

VisMatrixExport CVisVector4& CVisVector4::EqDiff(const CVisVector4 &refvectorA,
		const CVisVector4 &refvectorB)
{
	m_rgdbl[0] = refvectorA[0] - refvectorB[0];
	m_rgdbl[1] = refvectorA[1] - refvectorB[1];
	m_rgdbl[2] = refvectorA[2] - refvectorB[2];
	m_rgdbl[3] = 1.0;
    return *this;
}

VisMatrixExport CVisVector4& CVisVector4::EqCross(const CVisVector4 &refvectorA,
		const CVisVector4 &refvectorB)
{
	m_rgdbl[0] = (refvectorA[1] * refvectorB[2]
			- refvectorA[2] * refvectorB[1]);
	m_rgdbl[1] = (refvectorA[2] * refvectorB[0]
			- refvectorA[0] * refvectorB[2]);
	m_rgdbl[2] = (refvectorA[0] * refvectorB[1]
			- refvectorA[1] * refvectorB[0]);
	m_rgdbl[3] = 1.0;
    return *this;
}

VisMatrixExport CVisVector4& CVisVector4::EqProd(const CVisTransform4x4 &reftransformA,
		const CVisVector4 &refvectorB)
{
    for (int i = 0; i < 4; i++)
 	{
        m_rgdbl[i] = reftransformA[i][0] * refvectorB[0]
				+ reftransformA[i][1] * refvectorB[1]
				+ reftransformA[i][2] * refvectorB[2]
				+ reftransformA[i][3] * refvectorB[3];
    }
    return *this;
}


VisMatrixExport CVisTransform4x4& CVisTransform4x4::Translate(double tx, double ty, double tz)
{
    for (int i = 0; i < 4; i++)
	{
        m_rgrgdbl[0][i] += tx * m_rgrgdbl[3][i];
        m_rgrgdbl[1][i] += ty * m_rgrgdbl[3][i];
        m_rgrgdbl[2][i] += tz * m_rgrgdbl[3][i];
    }
    return *this;
}


VisMatrixExport CVisTransform4x4::CVisTransform4x4(bool fInitialize)
{
	if (fInitialize)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (i == j)
					m_rgrgdbl[i][j] = 1.0;
				else
					m_rgrgdbl[i][j] = 0.0;
			}
		}
	}
}

VisMatrixExport CVisTransform4x4::CVisTransform4x4(double dbl00, double dbl11,
		double dbl22, double dbl33)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m_rgrgdbl[i][j] = 0.0;
	
	m_rgrgdbl[0][0] = dbl00;
	m_rgrgdbl[1][1] = dbl11;
	m_rgrgdbl[2][2] = dbl22;
	m_rgrgdbl[3][3] = dbl33;
}

VisMatrixExport bool CVisTransform4x4::operator==(
		const CVisTransform4x4& reftransform4x4) const
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (m_rgrgdbl[i][j] != reftransform4x4[i][j])
				return false;

	return true;
}

VisMatrixExport CVisTransform4x4& CVisTransform4x4::EqProd(const CVisTransform4x4 &reftransformA,
		const CVisTransform4x4 &reftransformB)
{
    for (int i = 0; i < 4; i++)
 	{
        for (int j = 0; j < 4; j++)
		{
            double dblSum = 0.0;
            for (int k = 0; k < 4; k++)
                dblSum += reftransformA[i][k] * reftransformB[k][j];

            m_rgrgdbl[i][j] = dblSum;
		}
    }

    return *this;
}

// Helper function
// cos of the angle given in degrees
static double cosd(double angle)  
{
    double PI_OVER_180 = M_PI / 180.0;
    angle = 180.0 - fabs(180.0 - fmod(fabs(angle), 360.0));
    return (angle <  45.0) ? cos(PI_OVER_180 * angle) :
           (angle < 135.0) ? sin(PI_OVER_180 * (90.0 - angle)) :
                            -cos(PI_OVER_180 * (180.0 - angle)); 
}

// Helper function
// sin of the angle given in degrees
static double sind(double angle)  
{
    return cosd(angle + 90.0);
}


VisMatrixExport CVisTransform4x4 CVisTransform4x4::Rotated(double angle, double ax, double ay,
		double az) const
{
	// Create a rotation matrix defined by an angle and an axis.
    CVisTransform4x4 r;
    double mag = ax * ax + ay * ay + az * az;
    if (mag <= 0.0)
        return r;
    double imag = (mag == 1.0) ? 1.0 : 1.0 / sqrt(mag);

    // Formula from Faugeras' book (3-D Computer Vision), p. 146
    double s = cosd(0.5 * angle);
    double v = sind(0.5 * angle) * imag;
    double l = ax * v, m = ay * v, n = az * v;
    r[0][0] = s*s+l*l-m*m-n*n, r[0][1] = 2*(l*m-s*n), r[0][2] = 2*(l*n+s*m);
    r[1][0] = 2*(l*m+s*n), r[1][1] = s*s-l*l+m*m-n*n, r[1][2] = 2*(m*n-s*l);
    r[2][0] = 2*(l*n-s*m), r[2][1] = 2*(m*n+s*l), r[2][2] = s*s-l*l-m*m+n*n;

    // Round small numbers down to 0
    const double near_zero = 1.0 / (256.0 * 256.0); // 2^-16
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (fabs(r[i][j]) < near_zero)
                r[i][j] = 0.0;

	// Apply the rotation to this transform.
    return r * (*this);
}


VisMatrixExport CVisTransform4x4& CVisTransform4x4::Scale(double sx, double sy, double sz)
{
    for (int i = 0; i < 4; i++)
        m_rgrgdbl[0][i] *= sx, m_rgrgdbl[1][i] *= sy, m_rgrgdbl[2][i] *= sz;

    return *this;
}

VisMatrixExport CVisTransform4x4& CVisTransform4x4::Normalize(void)
{
    if (m_rgrgdbl[3][3] != 0.0)
	{
        double n = 1.0 / m_rgrgdbl[3][3];
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                m_rgrgdbl[i][j] = m_rgrgdbl[i][j] * n;
        m_rgrgdbl[3][3] = 1.0;
    }
    return *this;
}


VisMatrixExport CVisTransform4x4 CVisTransform4x4::Inverted(void) const
{
    CVisTransform4x4 result;

    CVisDMatrix A(4, 4, (double *) m_rgrgdbl[0]);
	CVisDMatrix Ainv(4, 4, result[0]);
	Ainv = A.Inverted();

    return result;
}

VisMatrixExport CVisTransform4x4& CVisTransform4x4::Transpose(void)
{
	// Idea is that Transposed[i][j] = Original[j][i].
	double dblTemp;
    for (int i = 1; i < 4; i++)
	{
        for (int j = 0; j < i; j++)
		{
            dblTemp = m_rgrgdbl[j][i];
            m_rgrgdbl[j][i] = m_rgrgdbl[i][j];
            m_rgrgdbl[i][j] = dblTemp;
		}
	}

    return *this;
}

VisMatrixExport CVisTransform4x4& CVisTransform4x4::TransposeZW(void)
{
	// Idea is that Transposed[i][j] = Original[i ^ (i > 1)][j ^ (j > 1)].
	double dblTemp;
    for (int i = 0; i < 4; i++)
	{
        dblTemp = m_rgrgdbl[2][i];
        m_rgrgdbl[2][i] = m_rgrgdbl[3][i];
        m_rgrgdbl[3][i] = dblTemp;
	}
    for (i = 0; i < 4; i++)
	{
        dblTemp = m_rgrgdbl[i][2];
        m_rgrgdbl[i][2] = m_rgrgdbl[i][3];
        m_rgrgdbl[i][3] = dblTemp;
	}

    return *this;
}

VisMatrixExport CVisTransform4x4& CVisTransform4x4::TransposeZWRow(void)
{
	// Idea is that Transposed[j][i] = Original[i ^ (i > 1)][j].
	double dblTemp;
    for (int i = 0; i < 4; i++)
	{
        dblTemp = m_rgrgdbl[2][i];
        m_rgrgdbl[2][i] = m_rgrgdbl[3][i];
        m_rgrgdbl[3][i] = dblTemp;
	}

    return *this;
}

// Compute a projection of a vector defined by a this transform.
VisMatrixExport CVisVector4 CVisTransform4x4::ProjectVector(const CVisVector4& v) const
{
    CVisVector4 result;

    for (int i = 0; i < 3; i++)
	{
        result[i] = m_rgrgdbl[i][0] * v[0] + m_rgrgdbl[i][1] * v[1]
				+ m_rgrgdbl[i][2] * v[2] + m_rgrgdbl[i][3] * v[3];
	}

    double s = m_rgrgdbl[3][0] * v[0] + m_rgrgdbl[3][1] * v[1]
			+ m_rgrgdbl[3][2] * v[2] + m_rgrgdbl[3][3] * v[3];
    double d = (s > 0.0) ? 1.0 / s : (s < 0.0) ? -1.0/ s : 1.0;
    for (i = 0; i < 3; i++)
        result[i] *= d;
    result[3] = (s > 0.0) ? 1.0 : (s < 0.0) ? -1.0 : 0.0;

    return result;
}

////////////////////////////////////////////////////////////////////////////
//  
//  FUNCTION:
//      VisProjectLine
//  
//  DECLARATION:
//      void VisProjectLine(const CVisVector4& v0, const CVisVector4& v1,
//                          CVisVector4& e0, CVisVector4& e1, double max_size)
//  
//  DESCRIPTION:
//      Project both endpoints of a line, and then clip so that
//       w > 0.0 and |x| and |y| <= max_size.
//      If both endpoints are invisible, e0 = e1 = (max_size, max_size);
//      See Rick's notes from Dec. 12, 1996 and Aug 25, 1997.
//
//      Note that after clipping, only the (x,y,z) components are valid.
//  
////////////////////////////////////////////////////////////////////////////

// Helper function
static inline void VisClipEnd(CVisVector4& e0, CVisVector4& e1, int i, 
                       double lambda, double s)
{
    for (int j = 0; j < 4; j++)
        e0[j] += lambda * (e1[j] - e0[j]);
    e0[i] = s;
    if (e0[3] != 1.0) {
        double d = 1.0 / s;
        for (j = 0; j < 3; j++)
            e0[j] *= d;
        e0[3] = 1.0;
    }
}

// Helper function
static int VisClipPlane(CVisVector4& e0, CVisVector4& e1, int i, bool gt, double s)
{
    double x0 = e0[i], x1= e1[i];
    bool ok0 = (gt) ? x0 > s : x0 < s;
    bool ok1 = (gt) ? x1 > s : x1 < s;
    if (ok0 == ok1)
        return ! ok0;       // both endpoints lie outside or inside
    else if (ok1) {
        double lambda = (s - x0) / (x1 - x0);
        VisClipEnd(e0, e1, i, lambda, s);
    } else if (ok0) {
        double lambda = (s - x1) / (x0 - x1);
        VisClipEnd(e1, e0, i, lambda, s);
    }
    return 0;               // success after clipping
}

// Helper function
void VisClipLine(CVisVector4& e0, CVisVector4& e1, double max_size)
{
    if (VisClipPlane(e0, e1, 3, true, 1.0/max_size) ||
        VisClipPlane(e0, e1, 0, false,  max_size) ||
        VisClipPlane(e0, e1, 0, true, -max_size) ||
        VisClipPlane(e0, e1, 1, false,  max_size) ||
        VisClipPlane(e0, e1, 1, true, -max_size))
        e0[0] = e0[1] = e1[0] = e1[1] = max_size;   // clipped out
}

VisMatrixExport void CVisTransform4x4::ProjectLine(const CVisVector4& v0,
		const CVisVector4& v1, CVisVector4& e0, CVisVector4& e1,
		double max_size) const
{
    e0 = ProjectVector(v0);
    e1 = ProjectVector(v1);
#if 1   // new code, Aug 25, 1997
    VisClipLine(e0, e1, max_size);
#else   // old code, Dec 12, 1996
    double x0 = e0[0], x1 = e1[0];
    double y0 = e0[1], y1 = e1[1];
    double z0 = e0[3], z1 = e1[3];
    double s0 = __max(fabs(x0), fabs(y0));
    double s1 = __max(fabs(x1), fabs(y1));
    double tiny = 1.0 / max_size;
    if (z0 <= s0 * tiny && z1 <= s1 * tiny) {
        // This test may clip out some lines that pass through visible volume
        e0[0] = e1[0] = e1[0] = e1[1] = max_size;
    } else if (z0 <= s0 * tiny || z1 <= s1 * tiny) {
		// if we are here, z0 = z1 = 1; 
		// then we use the mid point for clip

		if (z0 == z1) z1 = -z1;
        double clipx = (z0 * x1 - z1 * x0) / (z0 - z1);
        double clipy = (z0 * y1 - z1 * y0) / (z0 - z1);

        double s = __max(__max(fabs(clipx), fabs(clipy)), tiny);
        if (z0 <= s0 * tiny)
            e0[0] = clipx * max_size/s, e0[1] = clipy * max_size/s;
        else
            e1[0] = clipx * max_size/s, e1[1] = clipy * max_size/s;
    }
#endif
}

VisMatrixExport CVisTransform4x4 CVisTransform4x4::AppRot(void) const
{
    CVisTransform4x4 transformAppRot;

    CVisDMatrix matrixThis(4, 4, (double *) m_rgrgdbl[0]);
	CVisDMatrix matrixAppRot(4, 4, transformAppRot[0]);

	matrixAppRot = matrixThis * VisDMatrixSqrtInverse(matrixThis);

    return transformAppRot;
}



VisMatrixExport CVisTransform4x4 CVisTransformChain::CompleteTransform(void) const
{
	CVisTransform4x4 transformRet;

	if (!empty())
	{
		T_Deque::const_iterator iteratorCur = begin();
		T_Deque::const_iterator iteratorLim = end();
		for ( ; iteratorCur < iteratorLim; ++iteratorCur)
			transformRet *= (*iteratorCur);
	}
	
	return transformRet;
}

VisMatrixExport CVisTransformChain CVisTransformChain::Inverted(void) const
{
	CVisTransformChain transformchainRet;

	if (!empty())
	{
		transformchainRet.clear();
		T_Deque::const_iterator iteratorCur = begin();
		T_Deque::const_iterator iteratorLim = end();
		for ( ; iteratorCur < iteratorLim; ++iteratorCur)
			transformchainRet.push_front(iteratorCur->Inverted());
	}
	
	return transformchainRet;
}

// Change the transform chain to contain a single identity transform.
VisMatrixExport void CVisTransformChain::Reset(void)
{
	if (!empty())
		clear();
	
	push_front(CVisTransform4x4());
}


VisMatrixExport bool CVisTransformChain::operator==(
		const CVisTransformChain& reftransformchain) const
{
	if (size() != reftransformchain.size())
		return false;

	if (empty())
		return true;

	T_Deque::const_iterator iteratorThisCur = begin();
	T_Deque::const_iterator iteratorThisLim = end();
	T_Deque::const_iterator iteratorThatCur = reftransformchain.begin();
	for ( ; iteratorThisCur < iteratorThisLim;
			++iteratorThisCur, ++iteratorThatCur)
	{
		if (*iteratorThisCur != *iteratorThatCur)
			return false;
	}
	
	return true;
}


// Helper function
static inline void indent_stream(FILE *stream, int indent)
{
    while (indent-- > 0)
        fputc(' ', stream);
}


// Helper function
// Read the next non-whitespace character from the stream.
static inline int fgetcsw(FILE *stream)   // skip whitespace
{
    char c;
    while (isspace(c = fgetc(stream)));
    return c;
}

////////////////////////////////////////////////////////////////////////////
//  
//  FUNCTION:
//      CVisTransform4x4::Read
//  
//  DECLARATION:
//      void CVisTransform4x4::Read (FILE *stream);
//  
//  RETURN VALUE:
//      
//  
//  DESCRIPTION:
//      Read the transformation from the file.
//      For a description of the format, see CVisTransform4x4::Write.
//  
////////////////////////////////////////////////////////////////////////////
VisMatrixExport void CVisTransform4x4::Read (FILE *stream)
{
    char c = fgetcsw(stream);
    assert(c == '{');
    for (int i = 0; i < 4; i++) {
        c = fgetcsw(stream);
        assert(c == '{');
        for (int j = 0; j < 4; j++) {
            fscanf(stream, "%lf", &m_rgrgdbl[i][j]);
            c = fgetcsw(stream);
            assert(c == ((j < 3) ? ',' : '}'));
        }
        c = fgetcsw(stream);
        assert(c == ((i < 3) ? ',' : '}'));
    }
}

////////////////////////////////////////////////////////////////////////////
//  
//  FUNCTION:
//      CVisTransform4x4::Write
//  
//  DECLARATION:
//      void CVisTransform4x4::Write(FILE *stream, int indent);
//  
//  RETURN VALUE:
//      
//  
//  DESCRIPTION:
//      Write the transformation into the file.
//
//      The format for a matrix is a set of floating point numbers,
//      separated by commas (and spaces), and bracketed with braces {}, e.g.,
//       { {{0.174020, 0.002902, 0.000000, 249.810158},
//        {-0.452641, 0.848578, 0.000000, 8.571580},
//        {0.000000, 0.000000, 1.000000, 0.000000},
//        {-0.002346, -0.000033, 0.000000, 0.880452}}}
//  
////////////////////////////////////////////////////////////////////////////
VisMatrixExport void CVisTransform4x4::Write(FILE *stream, int indent)
{
    for (int i = 0; i < 4; i++) {
        indent_stream(stream, indent);
        fprintf(stream, (i == 0) ? "{{" : " {");
        for (int j = 0; j < 4; j++)
            fprintf(stream, "%lf%s", m_rgrgdbl[i][j], (j < 3) ? ", " : "}");
        fprintf(stream, (i < 3) ? ",\n" : "}");
    }
}



VisMatrixExport void CVisTransformChain::Read (FILE *stream)
{
    // Clear the transform chain
	*this = CVisTransformChain();

	char c = fgetcsw(stream);
    assert(c == '{');

    front().Read(stream);

    while ((c = fgetcsw(stream)) == ',')     // another transform
	{
		// LATER:  Make this more efficient by adding a method that
		// appends an uninitialized transform?
        push_back(CVisTransform4x4(false));

        back().Read(stream);
    }

	assert(c == '}');
}

VisMatrixExport void CVisTransformChain::Write(FILE *stream, int indent)
{
	for (int i = 0; i < size(); ++i)
	{
		if (i == 0)
		{
			indent_stream(stream, indent);
			fprintf(stream, "{\n");
		}
		else
		{
			fprintf(stream, ",\n");
		}
		(*this)[i].Write(stream, indent+1);
	}

	fprintf(stream, "\n");
	indent_stream(stream, indent);
	fprintf(stream, "}\n");
}



//
// Standard lib stream I/O
//

static std::ostream& write(std::ostream& os, const CVisVector4& p, int write_w = 0)
{
    os << "{" << p[0] << ", " << p[1] << ", " << p[2];
    if (write_w)
        os << ", " << p[3];
    os << "}";
    return os;
}

VisMatrixExport std::ostream& VISAPI operator<<(std::ostream& os, CVisVector4& p)
{
    return write(os, p, 0);
}

#ifdef TAKEOUT  // UNDONE:  Was this once the fn to read vectors?
std::istream& operator>>(std::istream& is, char c)
{
    is.ignore(1024, c);
    return is;
}
#endif // TAKEOUT


VisMatrixExport std::ostream& VISAPI operator<<(std::ostream& os, const CVisTransform4x4& m)
{
    for (int i = 0; i < 4; i++) {
        os << ((i == 0) ? "{{" : " {");
        for (int j = 0; j < 4; j++)
            os << m[i][j] << ((j < 3) ? ", " : "}");
        os << ((i < 3) ? ",\n" : "}");
    }
    return os;
}

VisMatrixExport std::istream& VISAPI operator>>(std::istream& is, CVisTransform4x4& m)
{
    while (is.get() != '{');    // skip leading whitespace
    for (int i = 0; i < 4; i++) {
        while (is.get() != '{');    // skip leading whitespace
        char c;
        for (int j = 0; j < 4; j++) {
            is >> m[i][j];
            while (isspace(c = is.get()));  // get trailing ',' or '}'
        }
        while (isspace(c = is.get()));  // get trailing ',' or '}'
    }
    return is;
}


//
// Self-describing stream output.
//
// LATER:  We should consider having both op<< and op>> with these streams.
//
// LATER:  We might want to add newlines and indents to format this better.
//


// Global variables used with self-describing streams.
// LATER:  Find a way to avoid using these global variables.
#ifdef VIS_INCLUDE_SDSTREAM
VisMatrixExport CVisSDObject<CVisVector4> g_visvector4Exemplar;
VisMatrixExport CVisSDObject<CVisTransform4x4> g_vistransform4x4Exemplar;
//VisMatrixExport CVisSDObject<CVisTransformChain> g_vistransformchainExemplar;
#endif


const char *CVisVector4::ReadWriteField(CVisSDStream& s, int field_id)
{
#ifdef VIS_INCLUDE_SDSTREAM
    switch (FieldType(field_id))
	{
    case eftName:
        return "class CVisVector4 {";

    case eftData:
		// Use the old code to read or write from the stream.
		// LATER:  Should we change this?
		{
			// Don't print the last two elements, if standard
			CVisVector4 def(0, 0);
			if (s.Status() == CVisSDStream::Read)
				(*this) = def;
			s << (*this)[0], s.Comma(), s << (*this)[1];
			if ((*this)[2] != def[2] || (*this)[3] != def[3] || s.PeekChar() == ',')
				s.Comma(), s << (*this)[2];
			if ((*this)[3] != def[3] || s.PeekChar() == ',')
				s.Comma(), s << (*this)[3];
		}
        return "double m_rgdbl[4];  // x, y, (and optionally) z, and w components";
    }

    return 0;
#else
	assert(false);  // SDStreams are not included
	return 0;
#endif
}


const char *CVisTransform4x4::ReadWriteField(CVisSDStream& s, int field_id)
{
#ifdef VIS_INCLUDE_SDSTREAM
    switch (FieldType(field_id))
	{
    case eftName:
        return "class CVisTransform4x4 {";

    case eftData:
		// Use the old code to read or write from the stream.
		// LATER:  Should we change this?
		{
			for (int i = 0; i < 4; i++) {
				s.OpenParen();
				for (int j = 0; j < 4; j++) {
					s << (*this)[i][j];
					if (j < 3)
						s.Comma();
				}
				s.CloseParen();
				if (i < 3)
					s.Comma(), s.NewLine();
			}
		}
        return "double m_rgrgdbl[4][4];";
    }

    return 0;
#else
	assert(false);  // SDStreams are not included
	return 0;
#endif
}


VisMatrixExport void CVisVector4::BuildPropList(CVisPropList& refproplist)
{
	assert(refproplist.IsEmpty());
	refproplist.AddPropReference("m_dbl4Vector", m_rgdbl[0],
			CVisDim(4), false);
}

VisMatrixExport void CVisTransform4x4::BuildPropList(CVisPropList& refproplist)
{
	assert(refproplist.IsEmpty());
	refproplist.AddPropReference("m_dbl4x4Matrix", m_rgrgdbl[0][0],
			CVisDim(4, 4), false);
}

VisMatrixExport void CVisTransformChain::BuildPropList(
		CVisPropList& refproplist)
{
	assert(refproplist.IsEmpty());
	refproplist.AddPropReference("m_deque", (T_Deque&) *this, false);
}


VIS_DEFINE_SD_CLASS_WITH_IO_PL_TYPEDEF(VisMatrixExportDeclspec, CVisVector4)
VIS_DEFINE_SD_CLASS_WITH_IO_PL_TYPEDEF(VisMatrixExportDeclspec, CVisTransform4x4)
VIS_DEFINE_PROP_LIST_CLASS_DEQUE(VisMatrixExportDeclspec, CVisTransform4x4)
VIS_DEFINE_SD_CLASS_WITH_IO_PL_TYPEDEF(VisMatrixExportDeclspec, CVisTransformChain)

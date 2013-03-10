///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisFreeRotation.cpp -- panoramic image alignment using free rotation
//
// DESCRIPTION
//  The routines in this file were originally in the file "VisBlockAdjust.cpp"
//
//
// SEE ALSO
//  VisFreeRotation.h        description of aligner class
//
//  Richard Szeliski's research notes, July 17, July 26, Aug 13, Sept 3.
//
// BUGS
//   
//
// DESIGN
//  
//
// HISTORY
//  03-Sep-96  Richard Szeliski (szeliski) at Microsoft
//  Created.
//  06-Mar-97  Harry Shum (hshum) changed the file name
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////


#include "VisImageProcPch.h"
#include "VisImageProc.h"


#ifndef M_PI 
#define M_PI    3.1415926536
#endif // M_PI
#define RadianToDegree  180.0/M_PI

//#include "VisXIMSL\VisXIMSL.h"
//#include <math.h>               //do i need that? 


static CVisTransform4x4 VisQuaternionToRotation(const CVisVector4 &q)
{
    CVisTransform4x4 R; 
    
    double s = q[0];
    double l = q[1]; 
    double m = q[2]; 
    double n = q[3]; 

    R[0][0] = s*s+l*l-m*m-n*n; 
    R[0][1] = 2*(l*m-s*n); 
    R[0][2] = 2*(l*n+s*m); // the way we represent rotation 
    R[1][0] = 2*(l*m+s*n); 
    R[1][1] = s*s-l*l+m*m-n*n; 
    R[1][2] = 2*(m*n-s*l);
    R[2][0] = 2*(l*n-s*m);
    R[2][1] = 2*(m*n+s*l); 
    R[2][2] = s*s-l*l-m*m+n*n;

    return R; 
}

static CVisVector4 VisRotationToQuaternion(const CVisTransform4x4 &R)
{
    // r00 + r11 + r22 = 4*s*s - 1
    // r21 - r12 = 4*s*l
    // r02 - r20 = 4*s*m
    // r10 - r01 = 4*s*n

    double s = 0.5 * sqrt(R[0][0] + R[1][1] + 1.0 + R[2][2]); 
    double s4 = 4 * s; 
    double l = (R[2][1] - R[1][2])/s4; 
    double m = (R[0][2] - R[2][0])/s4; 
    double n = (R[1][0] - R[0][1])/s4;
    CVisVector4 q(s, l, m, n);
    return q; 
}


//
//  Extract rotation and focal length from homography
//      see July 26, 1996 notes
//        [ r00     r01     f0*r02      ]
//  M = s [ r10     r11     f0*r12      ]
//        [ r20/f1  r21/f1  f0*r22/f1   ]
//  Strategy:
//      if (|m02^2 - m12^2| > |m02 * m12|)
//          f0^2 (m02^2 - m12^2) = m10^2 - m00^2 + m11^2 - m01^2
//      else
//          f0^2 (m02 * m12) = -(m00 * m10 + m01 * m11)
//    for f1, use M^{-1} (matrix inverse).
//
#if 0
static double VisGuessF(const CVisTransform4x4 &M)
{
    double den1 = M[0][3]*M[0][3] - M[1][3]*M[1][3];
    double den2 = M[0][3]*M[1][3];
    double d; 
    if (fabs(den1) > fabs(den2)) {
        d = den1 / (M[1][0]*M[1][0] - M[0][0]*M[0][0] +
                    M[1][1]*M[1][1] - M[0][1]*M[0][1]);
        if (d > 0.0)
            return sqrt(d);
    } else if (den2 != 0.0) {
        d = -den2 / (M[0][0]*M[1][0] + M[0][1]*M[1][1]);
        if (d > 0.0)
            return sqrt(d);
    }
    return -1.0;
}
#endif


static double VisGuessF(const CVisTransform4x4 &M)
{
    double num1 = M[1][3]*M[1][3] - M[0][3]*M[0][3]; 
    double den1 = M[0][0]*M[0][0] + M[0][1]*M[0][1] - M[1][0]*M[1][0] - M[1][1]*M[1][1]; 
    double num2 = -M[0][3]*M[1][3]; 
    double den2 = M[0][0]*M[1][0] + M[0][1]*M[1][1]; 

    double d; 
    if (fabs(den1) > fabs(den2)) {
        d = num1 / den1;
        if (d > 0.0)
            return sqrt(d);
    } else if (den2 != 0.0) {
        d = num2 / den2; 
        if (d > 0.0)
            return sqrt(d);
    }
    return -1.0;
}

// instead of using inverse(M) for f1, we can directly get f1 from M

static double VisGuessF1(const CVisTransform4x4 &M)
{
    double den1 = M[3][0]*M[3][0] - M[3][1]*M[3][1]; 
    double den2 = M[3][0]*M[3][1]; 
    double d; 
    if (fabs(den1) > fabs(den2)){
        d = (M[0][1]*M[0][1] - M[0][0]*M[0][0] +
                M[1][1]*M[1][1] - M[1][0]*M[1][0]) / den1; 
        if (d > 0.0)
            return sqrt(d); 
    } else if (den2 != 0.0) {
        d = -(M[0][0]*M[0][1] + M[1][0]*M[1][1]) / den2; 
        if (d > 0.0)
            return sqrt(d); 
    }
    return -1.0; 
}

//
// Convert 3x3 M to TVR
//

CVisTransformChain VisExtractTVRVT(const CVisTransform4x4 &M, 
                             const double cx, 
                             const double cy, 
                             const double f0, 
                             const double f1)
{
    CVisTransform4x4 t1;
	t1.Translate( cx,  cy);
    CVisTransform4x4 t2;
	t2.Translate(-cx, -cy);
    CVisTransform4x4 v1(f1, f1, 0.0, 0.0); 
    v1[2][3] = v1[3][2] = 1.0;    // transpose z and w (see CylindricalToPanoramic)
    CVisTransform4x4 v2(1.0/f0, 1.0/f0, 0.0, 0.0); 
    v2[2][3] = v2[3][2] = 1.0;    // transpose z and w (see CylindricalToPanoramic)

    // M = T1 V1 prevR invR V2 T2
    //   = T1 V1 R1 (T2 V2 R2)^-1
    //CVisTransform4x4 invR = VisMatrixInverse(prevR) *
    //                        v2 * t2 * M * t1 * v1; 
    // the above eq is only valid if f0 = f1; change later
    // CVisTransform4x4  R = v2 * t2 * M * t1 * v1; 

    CVisTransform4x4 VRV = t2 * M * t1; 
    CVisTransform4x4 rotation; 
    // if the transform is identity, rotation matrix is identity 
    // note the possible problem with f0 != f1
    if (! (M == CVisTransform4x4()))
        VisFNormalizedRotation(VRV, rotation, f0, f1); 

    CVisTransformChain g1 = t2; 
    g1.push_front(v2);
    g1.push_front(rotation); 
    g1.push_front(v1);
    g1.push_front(t1); 

    return (g1); 
}

CVisTransformChain VisExtractTVRVT(const CVisTransformChain &geometry, 
                           const double cx, 
                           const double cy, 
                           const double f0, 
                           const double f1
                           )
{
    CVisTransform4x4  M = geometry.CompleteTransform(); 
    return VisExtractTVRVT(M, cx, cy, f0, f1); 
}
                                 
CVisTransformChain ExtractTVRRVT(const CVisTransform4x4 &M, 
                    const double cx, 
                    const double cy, 
                    const double f0, 
                    const double f1, 
                    const CVisTransformChain &view)
{
    assert(view.size() == 3); 
    CVisTransformChain g = VisExtractTVRVT(M, cx, cy, f0, f1); 

    CVisTransform4x4 Rview = view[0]; 
    CVisTransform4x4 Rnew = g[2]; 
    CVisTransform4x4 Rinv = Rview * Rnew;

    CVisTransformChain g3 = view; 
    g3 = g3.Inverted();
    g3.push_back(Rinv);

//    CVisTransformChain g5 = g.Nth(3).Copy();
//    g3.Append(g5); 
	assert(g.size() == 5);  // From TVRVT factorization
	for (int i = 3; i < g.size(); ++i)
		g3.push_back(g[i]);

    return (g3); 
}

void VisExtractTVRVT(CVisColorComposite &composite)
{
    int nFrames = composite.NFrames(); 
    CVisShape s = composite[0].Shape(); // if all images have same size
    double cx = 0.5*(s.Left() + s.Right()-1);
    double cy = 0.5*(s.Top() + s.Bottom()-1); 
    double f = composite.FocalLength();

    for (int i=0; i<nFrames; i++)
    {
        CVisTransformChain &g = composite[i].TransformChain(); 
        int d = g.size(); 
        if (d != 3)
        {
            assert(d == 1 || d == 5); // d = 5 for previous results
            CVisTransform4x4 M = g.CompleteTransform(); 
            g = ExtractTVRRVT(M, cx, cy, f, f, composite.ViewGeometry()); 
            g.pop_front(); 
            g.pop_front(); 
            g.pop_front(); 
        }
    }
    
}

void VisCollapseTVRVT(CVisColorComposite &composite)
{
    CVisTransformChain& view = composite.ViewGeometry(); 

    for (int i=0; i< composite.NFrames(); i++) {
        CVisTransformChain& g = composite[i].TransformChain();
        int d = g.size();
        if (d > 1)
        {
            assert(d == 3 || d == 5); // d = 5 for previous results
            if (d == 5)
                g = CVisTransformChain(g.CompleteTransform());
            else // d = 3
            {
                assert(view.size() == 3); // make sure view is still 3-chain
                g = CVisTransformChain(view.CompleteTransform().Inverted() 
                    * g.CompleteTransform());
            }
        }
    }
}

//
// Convert 3x3 M to TVRVT and modify R according to newTheta
//

static void VisModifyRVT(CVisTransformChain& g, const CVisTransform4x4 &dR,
                           double f0)
{
    assert(g.size() == 3); 

    CVisTransform4x4& V1 = g[1];
    CVisTransform4x4& R  = g[0];
   
    V1 = CVisTransform4x4(1.0/f0, 1.0/f0, 0.0, 0.0); 
    V1[2][3] = V1[3][2] = 1.0;    // transpose z and w (see CylindricalToPanoramic)

    R = R * dR;
}
                                 
void VisCloseRGap(CVisColorComposite &composite)
{
    // Get the second frame's rotation
    int nFrames = composite.NFrames(); 

    CVisTransformChain &secondG = composite[1].TransformChain();
    CVisTransform4x4 R1 = secondG[0]; 
    CVisTransform4x4 R1inv = R1.Inverted(); 
    CVisVector4 q1 = VisRotationToQuaternion(R1inv); 

    // Get the last frame's rotation
    CVisTransformChain &lastG = composite[nFrames-1].TransformChain();
    CVisTransform4x4 R = lastG[0];
    CVisTransform4x4 Rinv = R.Inverted(); 
    CVisVector4 q = VisRotationToQuaternion(Rinv); 
    double dTheta = 2 * atan2 (sqrt(q[1]*q[1]+q[2]*q[2]+q[3]*q[3]), q[0]); 

    // Compute the gap and fix up f
    double qdot = q[1]*q1[1] + q[2]*q1[2] + q[3]*q1[3];
    if (qdot > 0.0)     // overshot 360 degrees,  => f should be bigger
        dTheta = -dTheta;
    double f = composite.FocalLength();
    // double mf = f; 
    double mf = f * (2*M_PI - dTheta) / (2*M_PI); 
    composite.SetFocalLength(mf);

    for (int i=0; i<nFrames; i++){
        double ratio = (double) i / (double) (nFrames -1); 
        // how to divide a rotation? equal q (that is, sin(theta/2) ) or equal theta? 
        double l = q[1] * ratio; 
        double m = q[2] * ratio; 
        double n = q[3] * ratio; 
        double s = sqrt(1.0 - l*l - m*m - n*n); 
        CVisVector4 qi(s, l, m, n); 
        CVisTransform4x4 Ri = VisQuaternionToRotation(qi); 

        CVisTransformChain &g = composite[i].TransformChain(); 
        VisModifyRVT(g, Ri, mf); 
    }

}

#if 0
void VisExtractTVR(CVisColorComposite &composite)
{
    int nFrames = composite.NFrames(); 
    for (int i = 0; i < nFrames; i++){
        CVisTransformChain &g = composite[i].TransformChain(); 
        CVisShape s = composite[i].Shape(); 
        double cx = 0.5*(s.Left() + s.Right()-1);
        double cy = 0.5*(s.Top() + s.Bottom()-1); 
        double f  = composite.FocalLength(); // 275.0; 
        g = VisExtractTVR(g, cx, cy, f, f); 

        // old code for doing minor block adjustment
        // double dTheta = 24.36; 
        // double dTheta = 0.6; // should be computed as the rotating angle 
        // double newTheta = 360 - dTheta; 
        // CVisTransform4x4 dR = VisRotateMatrix(-dTheta * i / (composite.NFrames()-1), 0, 1, 0);
        // CVisTransform4x4 dRT = VisTransposeZW(dR); 
        // g = VisModifyVRV(g, cx, cy, f, f, newTheta, dRT);
    }

}
#endif


void VisExtractRVT(CVisColorComposite &composite)
{
    VisExtractTVRVT(composite);
    int nFrames = composite.NFrames(); 
    for (int i = 0; i < nFrames; i++){
        CVisTransformChain &g = composite[i].TransformChain();
        // pop off first two
		g.pop_front();
		g.pop_front();
    }
}

void VisShiftReferenceToLastFrame(CVisColorComposite &composite)
/*
void VisExtractRVT(CVisColorComposite &composite)
*/
{
    VisExtractTVRVT(composite); 
    int nFrames = composite.NFrames();
    CVisTransform4x4 refR = composite[nFrames-1].TransformChain()[2]; 
    CVisTransform4x4 invRefR = refR.Inverted(); 
    for (int i = 0; i < nFrames; i++){
        CVisTransformChain &g = composite[i].TransformChain(); 
        CVisTransform4x4 &Ri = g[2]; 
        Ri = invRefR * Ri; 
    }
    VisCollapseTVRVT(composite);

}

//
//  Convert 3x3 homographies into separated intrinsic/extrinsic form
//

CVisTransformChain VisExtractPictureCenter(const CVisTransformChain &geometry, 
                                     const CVisRect &r)
{
    double cx = 0.5*(r.left + r.right-1);
    double cy = 0.5*(r.top + r.bottom-1);
    CVisTransform4x4 t1;
	t1.Translate( cx,  cy);
    CVisTransform4x4 t2;
	t2.Translate(-cx, -cy);

	CVisTransformChain g1 = t2 * geometry.CompleteTransform();
    g1.push_front(t1);

    return g1;
}

void VisExtractPictureCenter(CVisColorComposite &composite)
{
    // Break each geometry into intrinsic (centering) and extrinsic
    for (int i = 0; i < composite.NFrames(); i++) {
        CVisTransformChain &g = composite[i].TransformChain();
        CVisShape s = composite[i].Shape();
        g = VisExtractPictureCenter(g, s);
    }

    // PostMultiply all transforms by first to restore identity
    CVisTransform4x4 t0 = composite[0].TransformChain()[0];
    CVisTransform4x4 t1;
	t1.Translate(-t0[0][3], -t0[1][3]);
    for (i = 0; i < composite.NFrames(); i++) {
        CVisTransformChain g = composite[i].TransformChain().back();
        g[0] *= t1;
    }
    CVisTransform4x4 vt = composite.ViewGeometry().CompleteTransform();
    CVisTransformChain     vg = vt * t1;
    composite.SetViewGeometry(vg);
}

CVisVector4 VisQuaternionFromRotation(const CVisTransform4x4 &R)
{
    // Formula from Faugeras' book (3-D Computer Vision), p. 146    
    // r[0][0] = s*s+l*l-m*m-n*n, r[0][1] = 2*(l*m-s*n), r[0][2] = 2*(l*n+s*m);
    // r[1][0] = 2*(l*m+s*n), r[1][1] = s*s-l*l+m*m-n*n, r[1][2] = 2*(m*n-s*l);
    // r[2][0] = 2*(l*n-s*m), r[2][1] = 2*(m*n+s*l), r[2][2] = s*s-l*l-m*m+n*n;
    CVisTransform4x4 E;     // outer product matrix
    E[0][0] = R[0][0] - R[1][1] - R[2][2];  // l*l
    E[0][1] = R[0][1] + R[1][0];            // l*m
    E[0][2] = R[0][2] + R[2][0];            // l*n
    E[1][0] = R[0][1] + R[1][0];            // l*m
    E[1][1] = R[1][1] - R[2][2] - R[0][0];  // m*m
    E[1][2] = R[1][2] + R[2][1];            // m*n
    E[2][0] = R[0][2] + R[2][0];            // l*n
    E[2][1] = R[1][2] + R[2][1];            // m*n
    E[2][2] = R[2][2] - R[0][0] - R[1][1];  // m*m
    E[0][3] = R[2][1] - R[1][2];            // s*l
    E[3][0] = R[2][1] - R[1][2];            // s*l
    E[1][3] = R[0][2] - R[2][0];            // s*m
    E[3][1] = R[0][2] - R[2][0];            // s*m
    E[2][3] = R[1][0] - R[0][1];            // s*n
    E[3][2] = R[1][0] - R[0][1];            // s*n
    E[3][3] = R[0][0] + R[1][1] + R[2][2];  // s*s
    CVisDMatrix Em(4, 4, &E[0][0]);
    CVisDVector qv(4);
    // call the eigenvalue code
    CVisVector4 q(qv[0], qv[1], qv[2], qv[3]);
    return q;
}

// equation from Horn's book p.
CVisTransform4x4 VisRotationFromQuaternion(double r0, double r1, double r2, double r3)
{
	CVisTransform4x4 M;
	M[0][0]=r0*r0+r1*r1-r2*r2-r3*r3; 
	M[0][1]=2*(r1*r2-r0*r3); 
	M[0][2]=2*(r1*r3+r0*r2);

	M[1][0]=2*(r1*r2+r0*r3); 
	M[1][1]=r0*r0-r1*r1+r2*r2-r3*r3; 
	M[1][2]=2*(r2*r3-r0*r1);

	M[2][0]=2*(r1*r3-r0*r2); 
	M[2][1]=2*(r2*r3+r0*r1); 
	M[2][2]=r0*r0-r1*r1-r2*r2+r3*r3;

    return M; 
}

CVisTransform4x4 VisRotationFromQuaternion(const CVisVector4 &r)
{
    return VisRotationFromQuaternion(r[0], r[1], r[2], r[3]); 
}

// two problems remain:
// 1. add code to handle special case when atan goes to singularity
// 2. add other euler angle cases, Z-Y-X and Z-Y-Z
void VisRotationAngle(const CVisTransform4x4 &rotation, int mode, 
                      double &alpha, double &beta, double &gamma)
{
    // see eqs on p.41 of J.J. Craig
    // note that the order of rotation is roll,pitch,yaw
    // move this part of code elsewhere later 

    switch (mode){
    case 0:

        // roll-pitch-yaw
        alpha = atan2(rotation[2][1], rotation[2][2]); 
        beta  = atan2(-rotation[2][0], 
                sqrt(rotation[0][0]*rotation[0][0]+rotation[1][0]*rotation[1][0])); 
        gamma = atan2(rotation[1][0], rotation[0][0]); 

    case 1:
    // x-y-x euler angle :-) see harry's notes 12/17/96
    // all convention rotation angle definitions (3 in Craig's book)
    // are not what we want. We need to estimate the tilting angle 
    // which tilt the camera up/down for alpha, then panning for beta
    // and then tilting again down/up for gamma;
    // therefore gamma = -alpha which is true in normalized RRR

    // I use atan instead of atan2 to eliminate the confusion in quads
        alpha = atan(-rotation[1][0]/rotation[2][0]); // x-axis
        beta  = atan2(sqrt(rotation[1][0]*rotation[1][0]+rotation[2][0]*rotation[2][0]),
                rotation[0][0]); // y
        gamma = atan(rotation[0][1]/rotation[0][2]); // x-axis
    }

    alpha *= RadianToDegree;    
    beta  *= RadianToDegree;
    gamma *= RadianToDegree;      

}

// without translation effect
void VisProjFromRotAndF(const CVisTransform4x4 &rotation, 
                        CVisTransform4x4 &transform, 
                        double f0, double f1, 
                        double cx, double cy)
{
    // Input has R in the upper-right 3x3 corner
    int i, j; 
    for (i=0; i<4; i++)
        for (j=0; j<4; j++)
        transform[i][j] = rotation[i^(i>1)][j^(j>1)]; 
    transform[0][3] *= f0; 
    transform[1][3] *= f0; 
    transform[3][0] /= f1; 
    transform[3][1] /= f1; 
 
    CVisTransform4x4 t1;
	t1.Translate( cx,  cy);
    CVisTransform4x4 t2;
	t2.Translate(-cx, -cy);
    CVisTransform4x4 temp = t1 * transform; 
    transform = temp * t2;          // T^-1*M*T
}

int VisGuessRotationAndF(const CVisTransform4x4 &transform,
                          CVisTransform4x4 &rotation, 
                          double &f0, double &f1, 
                          const double cx, const double cy)
{
    CVisTransform4x4 t1;
	t1.Translate( cx,  cy);
    CVisTransform4x4 t2;
	t2.Translate(-cx, -cy);
    CVisTransform4x4 temp = t2 * transform * t1; // T*M*T^-1

	/*
    transform = temp * t1;          // T*M*T^-1

    f0 = VisGuessF(transform);      // on the right of R
    f1 = VisGuessF1(transform);     // on the left  of R
*/
    f0 = VisGuessF (temp);      // on the right of R
    f1 = VisGuessF1(temp);      // on the left  of R

    if ((f0 == -1) || (f1 == -1))
        return (0);

#define SAME_FOCAL_LENGTH

#ifdef SAME_FOCAL_LENGTH
    double f = sqrt(f0*f1); 
    f0 = f; 
    f1 = f;
#endif //SAME_FOCAL_LENGTH

    VisFNormalizedRotation(transform, rotation, f0, f1);

    return (1);
}

// enough transformation to get rotation
// this is the wrap from 4x4 to 3x3
void VisFNormalizedRotation(const CVisTransform4x4 &transform, 
                            CVisTransform4x4 &rotation, 
                            double f0, double f1)
{
    // Input is a 4x4 matrix in V R V^-1 form
    CVisTransform4x4 r;
    int i, j;
    for (i=0; i<2; i++)
        for (j=0; j<2; j++)
            r[i][j] = transform[i][j]; 
    r[0][2] = transform[0][3]; 
    r[1][2] = transform[1][3]; 
    r[2][0] = transform[3][0]; 
    r[2][1] = transform[3][1]; 
    r[2][2] = transform[3][3]; 

    r[2][0] *= f1; 
    r[2][1] *= f1; 
    r[0][2] /= f0; 
    r[1][2] /= f0;

    r[2][2] *= f1; 
    r[2][2] /= f0; 

    // UNDONE:  Is r setup correctly for the AppRot method in the new SDK?
	rotation = r.AppRot(); 

    // Output is an R matrix in upper 3x3 corner (see CylindricalToPanoramic)
    for (i=0; i<4; i++)
        rotation[i][3] = rotation[3][i] = (i == 3);
}

//
// Remove the trends in the rotation matrix by high-pass filtering the
// incremental rotations between frames, i.e., subtracting out a low-pass
// filtered version of the instantaneous rotations.
//
// The current implementation uses a box filter of size blur_width for low-pass filtering.
//
// This code is patterned after VisCloseRGap();
//
// See Rick's research notes, 15-Oct-99
//

void VisDeTrend3DRotation(CVisRGBAComposite &composite, int blur_width)
{
    // Compute the incremental rotations
    int nFrames = composite.NFrames();
    CVisFloatImage incr_rot(nFrames-1, 1, 3);   // N x 3 vector of incr. rot.'s
    for (int i = 0; i < nFrames-1; i++)
    {
        CVisTransform4x4& R0 = composite[i+0].TransformChain()[0];
        CVisTransform4x4& R1 = composite[i+1].TransformChain()[0];
        CVisTransform4x4  dR = R1 * R0.Inverted();
        CVisVector4 q01      = VisRotationToQuaternion(dR);
        for (int j = 0; j < 3; j++)
            incr_rot.Pixel(i, 0, j) = q01[j+1];
    }

	// Build box filter kernel (from Chris Buehler's mods to VisStereo.cpp)
	CVisFloatImage kernelImage(CVisRect(-blur_width/2, 0, blur_width/2 + 1, 1), 1, evisimoptDefault);
	float *k = kernelImage.PtrToFirstPixelInRow(0);
	for (i = 0; i < blur_width; i++)
		k[i] = 1.0f/blur_width;
    CVisFloatImage vkernel(1, 1);   // no vertical convolution
    vkernel.Pixel(0, 0) = 1.0f;

    // Compute a smooth version of the incremental rotations
    CVisFloatImage smooth_rot(nFrames-1, 1, 3);
    incr_rot.CopyPixelsTo(smooth_rot);
    EVisPad g_evispadConvolveEnlarge = evispadZero;
    VisConvolveSeparable(incr_rot, smooth_rot, kernelImage, vkernel, 1);

    // Update the rotations rotations
    //  if I had quaternion mult. code handy, I would just keep a running product
    //  of dq's... (Rick)
    for (i = 0; i < nFrames-1; i++)
    {
        CVisTransform4x4& R0 = composite[i+0].TransformChain()[0];
        CVisTransform4x4& R1 = composite[i+1].TransformChain()[0];
        CVisVector4 dq;
        for (int j = 0; j < 3; j++)
            dq[j+1] = incr_rot.Pixel(i, 0, j) - smooth_rot.Pixel(i, 0, j);
        dq[0] = sqrt(1.0 - dq[1]*dq[1] - dq[2]*dq[2] - dq[3]*dq[3]); 
        CVisTransform4x4 dR = VisQuaternionToRotation(dq);
        R1 = dR * R0;

        // Normalize R1 (icky hack)
        CVisVector4 q = VisRotationToQuaternion(R1);
        R1 = VisQuaternionToRotation(q);
    }
}

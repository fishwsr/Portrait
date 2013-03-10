///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisFreeRotation.h -- panoramic image alignment using free rotation
//
// DESCRIPTION
//  The routines in this file were originally in the file "VisBlockAdjust.cpp"
//
//
// SEE ALSO
//  VisFreeRotation.cpp        
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

#ifndef _VIS_FREE_ROTATION_H_
#define _VIS_FREE_ROTATION_H_
                         

/*
#define TRUE 1
#define FALSE 0

typedef unsigned char BOOL;
*/

CVisTransformChain VisExtractTVRVT(const CVisTransformChain &geometry, 
                           const double cx, 
                           const double cy, 
                           const double f0, 
                           const double f1//, 
                           //const CVisTransform4x4 &prevR
                           //CVisTransform4x4 &R
                           ); 

CVisTransformChain VisExtractTVRVT(const CVisTransform4x4 &M, 
                           const double cx, 
                           const double cy, 
                           const double f0, 
                           const double f1 
                           ); 

// convert geometry at each frame between 3-chain (RVT) and 1-chain (8-parameter)
void VisExtractTVRVT(CVisRGBAComposite &composite); 
void VisCollapseTVRVT(CVisRGBAComposite &composite);

CVisTransformChain VisExtractPictureCenter(const CVisTransformChain &geometry, 
                                     const CVisRect &r);

void VisExtractPictureCenter(CVisRGBAComposite &composite);

int VisGuessRotationAndF(const CVisTransform4x4 &transform,
                            CVisTransform4x4 &rotation, 
                            double &f0, double &f1, 
                            const double cx, const double cy);
void VisProjFromRotAndF(const CVisTransform4x4 &rotation, 
                        CVisTransform4x4 &transform, 
                        double f0, double f1, 
                        double cx, double cy); 
void VisFNormalizedRotation(const CVisTransform4x4 &transform, 
                            CVisTransform4x4 &rotation, 
                            double f0, double f1); 

void VisRotationAngle(const CVisTransform4x4 &rotation, int mode, 
                      double &alpha, double &beta, double &gamma);
void VisCloseRGap(CVisRGBAComposite &composite);
void VisExtractRVT(CVisRGBAComposite &composite);

// 6 transform chain
void __cdecl VisExtractTVRRVT(CVisRGBAComposite &composite);

CVisTransformChain ExtractTVRRVT(const CVisTransform4x4 &M, 
                    const double cx, 
                    const double cy, 
                    const double f0, 
                    const double f1, 
                    const CVisTransformChain &view);

// quarternion
CVisTransform4x4 VisRotationFromQuaternion(double r0, double r1, double r2, double r3); 
CVisTransform4x4 VisRotationFromQuaternion(const CVisVector4 &r); 
CVisVector4 VisQuaternionFromRotation(const CVisTransform4x4 &R); 

void VisDeTrend3DRotation(CVisRGBAComposite &composite, int blur_width);

#endif // _VIS_FREE_ROTATION_H_

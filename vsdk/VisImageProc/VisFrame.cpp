// VisFrame.cpp
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved


#include "VisImageProcPch.h"
#include "VisImageProc.h"


#if defined(_DEBUG) && defined(_AFXDLL)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG


//////////////////////////////////////////////////////////////////////////\/
//  
//  FUNCTION:
//      VisMakePixelsInvisible
//  
//  DECLARATION:
//      static void VisMakePixelsInvisible(CVisRGBAByteImage &img, int v);
//  
//  PARAMETERS:
//      img - 
//          Image
//  
//      v - 
//			A value of the pixel to be set invisible.
//          
//  
//  DESCRIPTION:
//      
//		Sets all the pixels equal to (v,v,v,255) to be invisible (0,0,0,0).
//			
//  
//////////////////////////////////////////////////////////////////////////\/
void VisMakePixelsInvisible(CVisRGBAByteImage &img, int v)
{
    int rows = img.Height(), cols = img.Width()*img.NBands();
    for (int r = 0; r < rows; r++) {
        CVisRGBABytePixel input(v, v, v, 255);
        int iv = *(int *) &input;
        int *p = (int *) img.PtrToFirstPixelInRow(r + img.Top());
        for (int c = 0; c < cols; c++)
            if (p[c] == iv)
                p[c] = 0;
    }
}

//////////////////////////////////////////////////////////////////////////\/
//  
//  FUNCTION:
//      VisBackInvisiblePixels
//  
//  DECLARATION:
//      static void VisBackInvisiblePixels(CVisRGBAByteImage &img, int v);
//  
//  PARAMETERS:
//      img - 
//          Image to which backing color should be added
//  
//      v - 
//			Gray-level value of backing pixel.
//          
//  
//  DESCRIPTION:
//      
//		Sets out to in OVER (v,v,v,255).
//      Also, sets any (v,v,v,255) value to (v+e,v+e,v+e,255), e = +/- 1
//			
//  
//////////////////////////////////////////////////////////////////////////\/
void VisBackInvisiblePixels(CVisRGBAByteImage &img, int v)
{
    int rows = img.Height(), cols = img.Width()*img.NBands();
    int ic, e = (v < 128) ? 1 : -1;
    *(CVisRGBABytePixel *) &ic = CVisRGBABytePixel(v, v, v, 255);
    int i0 = ic;
    *(CVisRGBABytePixel *) &ic = CVisRGBABytePixel(v+e, v+e, v+e, 255);
    int i1 = ic;
    CVisRGBABytePixel a_mask(0, 0, 0, 255);
    int ia_mask = *(int *) &a_mask;
    for (int r = 0; r < rows; r++) {
        CVisRGBABytePixel *p = img.PtrToFirstPixelInRow(r + img.Top());
        int *ip = (int *) p;
        for (int c = 0; c < cols; c++) {
#ifdef FASTER
            // Test 8 pixels at a time (speedup)
            if ((c & 7) == 0 && (c+7) < cols) {
                int *f = &ip[c];
                int f255 = f[0] & f[1] & f[2] & f[3] & 
                           f[4] & f[5] & f[6] & f[7];
                if ((f255 & ia_mask) == ia_mask) {
                    c += 7;
                    continue;   // all 8 pixels are opaque
                }
                int f000 = f[0] | f[1] | f[2] | f[3] | 
                           f[4] | f[5] | f[6] | f[7];
                if ((f000  & ia_mask) == 0) {
                    // all 8 pixels are transparent
                    f[0] = f[1] = f[2] = f[3] =
                    f[4] = f[5] = f[6] = f[7] = i0;
                    c += 7;
                    continue;
                }
            }
#endif
            if (ip[c] == 0)
                ip[c] = i0;
            else if (ip[c] == i0)
                ip[c] = i1;
            else if (p[c].A() != 255) {
                int w = ((255 - p[c].A())*v + 127)/255;
                p[c].SetR(p[c].R() + w);
				p[c].SetG(p[c].G() + w);
				p[c].SetB(p[c].B() + w);
				p[c].SetA(255);
            }
        }
    }
}



// Specialized PropList helper functions for common types
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrame<BYTE> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisFrame<BYTE> >(0, fAlwaysTypedef); }

CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrame<int> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisFrame<int> >(0, fAlwaysTypedef); }

CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrame<float> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisFrame<float> >(0, fAlwaysTypedef); }

CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrame<double> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisFrame<double> >(0, fAlwaysTypedef); }

CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrame<CVisRGBA<BYTE> > const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisFrame<CVisRGBA<BYTE> > >(0, fAlwaysTypedef); }

CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrame<CVisRGBA<int> > const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisFrame<CVisRGBA<int> > >(0, fAlwaysTypedef); }

CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrame<CVisRGBA<float> > const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisFrame<CVisRGBA<float> > >(0, fAlwaysTypedef); }

CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrame<CVisRGBA<double> > const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisFrame<CVisRGBA<double> > >(0, fAlwaysTypedef); }


CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrameSequence<BYTE> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisFrameSequence<BYTE> >(0, fAlwaysTypedef); }

CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrameSequence<int> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisFrameSequence<int> >(0, fAlwaysTypedef); }

CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrameSequence<float> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisFrameSequence<float> >(0, fAlwaysTypedef); }

CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrameSequence<double> const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisFrameSequence<double> >(0, fAlwaysTypedef); }

CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrameSequence<CVisRGBA<BYTE> > const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisFrameSequence<CVisRGBA<BYTE> > >(0, fAlwaysTypedef); }

CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrameSequence<CVisRGBA<int> > const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisFrameSequence<CVisRGBA<int> > >(0, fAlwaysTypedef); }

CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrameSequence<CVisRGBA<float> > const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisFrameSequence<CVisRGBA<float> > >(0, fAlwaysTypedef); }

CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisFrameSequence<CVisRGBA<double> > const &refobj, bool fAlwaysTypedef)
	{ return new CVisPropTypeInfoWithIOPropList<CVisFrameSequence<CVisRGBA<double> > >(0, fAlwaysTypedef); }


///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisColorAdjust.h -- panoramic image color adjustment
//
// DESCRIPTION
//  The routines in this file perform color adjustment between 
//  overlapping images in a panorama.
//
// DESIGN
//  
//
// HISTORY
//  08-Aug-99  Matt Uyttendaele (mattu) at Microsoft
//  Created.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////
#ifndef _VIS_COLOR_ADJUST_H_
#define _VIS_COLOR_ADJUST_H_

// This class could potentially be sub-classed from CVisBlockAdjuster since
// it share some common code.  For now it is implemented as a separate class.
 
template <class TFrame, class TImage>
class CVisColorAdjuster
{
public:
    // Constructor and destructor
    CVisColorAdjuster();
    ~CVisColorAdjuster();

    // Initialize blockadjuster from a composite
    void Initialize(CVisWarpedComposite<TFrame::T_Pixel> *composite);

    // Frame info from sequence
    int NFrames()
        { return (m_composite) ? m_composite->NFrames() : 0; }
    int CurrentFrame()
        { return (m_composite) ? m_composite->CurrentFrame() : 0; }

    void ComputeMatch(int j, int k, TVisColorDir dir);
    void ComputeMatchAll();
                   
    // number of matching pairs: +-2, or 0 (all pairs)
    int m_matchpairs;         // number of neighbors to be matched
    int m_match_only_0;       // match to frame 0 only (for stereo)

private:
    // Internal members
    CVisWarpedComposite<TFrame::T_Pixel> *m_composite;
};

template <class TImage>
void VisAdjustColor(TImage &dst,
                    SColorAdjustParam& adjacent0,
                    SColorAdjustParam& adjacent1,
					double fall_off = 0.0);

typedef CVisColorAdjuster<CVisRGBAByteCompositeFrame,CVisRGBAByteImage>   CVisRGBAColorAdjuster;
typedef CVisColorAdjuster<CVisByteCompositeFrame,CVisByteImage>           CVisGrayColorAdjuster;


#endif

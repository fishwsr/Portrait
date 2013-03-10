///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisBlendedSequence.h -- generate a blended sequence of frames given an
//                          input stream, a blending window and frame count.
//
// DESCRIPTION
//  
//  
//
// DESIGN
//  
//
// HISTORY
//  1-Mar-00  Matt Uyttendaele (mattu) at Microsoft
//  Created.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////
#ifndef _VIS_BLENDED_SEQUENCE_H_
#define _VIS_BLENDED_SEQUENCE_H_

#if 0
class CVisFrameCountGenerator
{
public:
    CVisFrameCountGenerator() {};

    void  Reset();
    int   Next();
    int   TotalFrameCount();

private:

    CVisTransitionProbability m_TransitionProbability;
    CVisLoopTable             m_LoopTable;
};
#endif

template<class TPixel>
class CVisBlendedSequence : public CVisSequence<TPixel>
{
public:
    CVisBlendedSequence( int WindowSize );
    ~CVisBlendedSequence();

    void     Reset();
    void     SetWindowSize( int size );
    void     ShiftFrame( int i );
    T_Image& Blended();

private:
	typedef struct _SBlendWeight
    {
        int   frame;
        float weight;
        _SBlendWeight( )
        {
            frame  = 0;
            weight = 1;
        }
        _SBlendWeight( int f, float w )
        {
            frame  = f;
            weight = w;
        }
    } SBlendWeight;
    typedef std::vector<SBlendWeight> T_BLENDVEC;
    typedef std::vector<int>	      T_INTVEC;


    T_INTVEC   m_FrameNumHistory;
    T_BLENDVEC m_BlendFactors;
    int        m_iState;
    int        m_iShiftIn;
    int        m_iActiveSize;

    CVisRGBAByteImage   m_BlendImage;
    CVisRGBAFloatPixel* m_pBlendRow;
};

typedef CVisBlendedSequence<CVisRGBABytePixel> CVisBlendedRGBAByteSequence;

#endif // _VIS_BLENDED_SEQUENCE_H_


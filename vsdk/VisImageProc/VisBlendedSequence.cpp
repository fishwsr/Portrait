///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisBlendedSequence.cpp -- 
//
// DESCRIPTION
//  generate a blended sequence of frames given an input stream,
//  a blending window size and frame count.
//
//
//
// SEE ALSO
//  VisBlendedSequence.h    description
//
// DESIGN
//  
//
// HISTORY
//  01-Mar-00  Matt Uyttendaele (mattu) at Microsoft
//  Created.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////

#include "VisImageProcPch.h"
#include "VisImageProc.h"

#define BLNDSEQ_STATE_RESET   0
#define BLNDSEQ_STATE_PRIME   1
#define BLNDSEQ_STATE_RUN     2

template<class TPixel>
CVisBlendedSequence<TPixel>::CVisBlendedSequence( int WindowSize )
    : CVisSequence<TPixel>(0, evissequenceLimitMemoryUsage|
                              evissequenceDelayReadUntilNeeded)
{
    SetWindowSize(WindowSize);
    m_pBlendRow = NULL;
    Reset();
}

template<class TPixel>
CVisBlendedSequence<TPixel>::~CVisBlendedSequence( )
{
    if( m_pBlendRow )
    {
        delete [] m_pBlendRow;
    }
}

template<class TPixel> void
CVisBlendedSequence<TPixel>::Reset()
{
    m_iState = BLNDSEQ_STATE_RESET;
}

template<class TPixel> void
CVisBlendedSequence<TPixel>::SetWindowSize( int size )
{
    if( size==0 )
    {
        size = 1;
    }

    if( size < m_FrameNumHistory.size() )
    {
        m_iActiveSize = size;
    }
    else if( size > m_FrameNumHistory.size() )
    {
        m_iActiveSize = m_FrameNumHistory.size();
    }

    m_BlendFactors.resize(size);
    m_FrameNumHistory.resize(size);
}

template<class TPixel> void
CVisBlendedSequence<TPixel>::ShiftFrame( int i )
{
    // example:
    // WindowSize of 5
    // 1) After a reset the first call to this is with i = 10
    //    the m_FrameNumHistory will be initialized to 12 11 10 9 8.
    //    the m_iState var will now be        BLNDSEQ_STATE_PRIME
    //    and the m_iShiftIn var will be      1
    // 2) The next call is with i = 20,
    //    the m_FrameNumHistory will now be   21 20 10 9 8   
    //    the m_iState var will stay          BLNDSEQ_STATE_PRIME
    //    and the m_iShiftIn var will be      0
    // 3) The next call is with i = 30,
    //    the m_FrameNumHistory will now be   30 20 10 9 8   
    //    the m_iState var will change to     BLNDSEQ_STATE_RUN
    //    and the m_iShiftIn var will be      0
    //    **** Shift is primed, from now on Blended can be called ***
    // 4) The next call is with i = 31,
    //    the m_FrameNumHistory will now be   31 30 20 10 9   
    //    the m_iState var will stay          BLNDSEQ_STATE_RUN
    //    and the m_iShiftIn var stay         0

    // ShiftFrame should be called (w + 1) / 2 times before getting the
    // first blended frame, but because of the initialization described
    // above Blended will return a reasonable image if it is called 
    // pre-maturely.

    switch( m_iState )
    {
    case BLNDSEQ_STATE_RESET:
        {
        m_iActiveSize = m_FrameNumHistory.size();
        int mididx    = ((m_iActiveSize - 1) >> 1);
        int fcnt      = i + mididx;

        int last = Length() - 1;
        for( int j = 0; j < m_iActiveSize; j++, fcnt-- )
        {
            m_FrameNumHistory[j] = (fcnt > last)? last: fcnt;
        }

        if( mididx > 0 )
        {
            m_iShiftIn = mididx - 1;
            m_iState = BLNDSEQ_STATE_PRIME;
        }
        else
        {
            m_iState = BLNDSEQ_STATE_RUN;
        }
        }
        break;
    
    case BLNDSEQ_STATE_PRIME:
        {
        int fcnt = i + m_iShiftIn;
        int last = Length() - 1;
        for( int j = 0; j <= m_iShiftIn; j++, fcnt-- )
        {
            m_FrameNumHistory[j] = (fcnt > last)? last: fcnt;
        }
        
        if( m_iShiftIn == 0 )
        {
            m_iState = BLNDSEQ_STATE_RUN;
        }
        else
        {
            m_iShiftIn--;
        }
        }
        break;

    case BLNDSEQ_STATE_RUN:
        {
        if( m_iActiveSize < m_FrameNumHistory.size() )
        {
            m_iActiveSize++;
        }

        for( int j = m_iActiveSize - 1; j >= 1; j-- )
        {
            m_FrameNumHistory[j] = m_FrameNumHistory[j-1];
        }

        m_FrameNumHistory[0] = i;
        }
        break;

    default:
        break;
    }
}

template<class TPixel> CVisSequence<TPixel>::T_Image&
CVisBlendedSequence<TPixel>::Blended()
{
    // first figure out contributions of the frames
    int mididx    = ((m_iActiveSize - 1) >> 1);
    int midframe  = m_FrameNumHistory[mididx];
    int numblends = 0;

    for( int i = 0; i < m_iActiveSize; i++ )
    {
        int frame = m_FrameNumHistory[i] + i - mididx;
        
        if( frame < 0 )
        {
            frame = 0;
        }
        else if( frame >= Length() )
        {
            frame = Length() - 1;
        }

        bool fFound = false;
        for( int j = 0 ; j < numblends; j++ )
        {
            if( m_BlendFactors[j].frame == frame )
            {
                m_BlendFactors[j].weight++;
                fFound = true;
                break;
            }
        }
        if( !fFound )
        {
            m_BlendFactors[numblends] = SBlendWeight( frame, 1 );
            numblends++;
        }
    }

    assert( numblends );
    
    T_Image& frm0 = CVisSequence<T_Pixel>::At(m_BlendFactors[0].frame);
    
    // special case no blending
    if( numblends == 1 )
    {
        return frm0;
    }

    // otherwise go through with the blend 
    if( m_BlendImage.Shape()  != frm0.Shape() )
    {
        m_BlendImage.Allocate( frm0.Shape() );
        if( m_pBlendRow )
        {
            delete [] m_pBlendRow;
            m_pBlendRow = NULL;
        }
    }

    if( m_pBlendRow == NULL )
    {
        // UNDONE: check the return value
        m_pBlendRow = new CVisRGBAFloatPixel[frm0.Width()];
    }

    // compute the floating point blend weight for each image
    for( i = 0; i < numblends; i++ )
    {
        // do a debug check that all image are same size
        T_Image& img = CVisSequence<T_Pixel>::At(m_BlendFactors[i].frame);
        assert( img.Shape()  == m_BlendImage.Shape() );

        m_BlendFactors[i].weight /= m_iActiveSize;
    }

    // blend one row at a time
    // UNDONE: blend alpha
    for ( int y = 0; y < m_BlendImage.Height(); y++ )
    {
        // mult
        const T_Pixel* pSrc = 
            CVisSequence<T_Pixel>::At(m_BlendFactors[0].frame).PtrToFirstPixelInRow(y);
        float weight = m_BlendFactors[0].weight;

        for ( int x = 0; x < m_BlendImage.Width(); x++ )
        {
            CVisRGBAFloatPixel& fpel = m_pBlendRow[x];

            fpel.SetR( float(pSrc->R()) * weight );
            fpel.SetG( float(pSrc->G()) * weight );
            fpel.SetB( float(pSrc->B()) * weight );
            pSrc++;
        }

        // mult-accumulate
        for ( int i = 1; i < numblends; i++ )
        {
            const T_Pixel* pSrc = 
                CVisSequence<T_Pixel>::At(m_BlendFactors[i].frame).PtrToFirstPixelInRow(y);
            float weight = m_BlendFactors[i].weight;

            for ( int x = 0; x < m_BlendImage.Width(); x++ )
            {
                CVisRGBAFloatPixel& fpel = m_pBlendRow[x];

                fpel.SetR( fpel.R() + float(pSrc->R()) * weight );
                fpel.SetG( fpel.G() + float(pSrc->G()) * weight );
                fpel.SetB( fpel.B() + float(pSrc->B()) * weight );
                pSrc++;
            }
        }

        // back to 8 bit components
        BYTE* pDst = m_BlendImage.PbFirstPixelInRow(y);
        for ( x = 0; x < m_BlendImage.Width(); x++ )
        {
            CVisRGBAFloatPixel& fpel = m_pBlendRow[x];

            pDst[3] = 0xff;      // note: force alpha to 0xff   
            pDst[2] = fpel.R();  // note: no check for > 255, should never be
            pDst[1] = fpel.G();  //      < 0
            pDst[0] = fpel.B();
            pDst += 4;
        }
    }

    return m_BlendImage;
}


template CVisBlendedRGBAByteSequence;

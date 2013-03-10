
//
// Cylindrically warping video composition class
//
// VisWarpedVideoComposite.cpp
//
// Copyright © 1999 - 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////

#include "VisImageProcPch.h"
#include "VisImageProc.h"

#if defined(_DEBUG) && defined(_AFXDLL)
#include <afxwin.h>         // MFC core and standard components
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG


template<class TPixel>
CVisWarpedVideoComposite<TPixel>::CVisWarpedVideoComposite(EVisSequence evissequence)
   : CVisWarpedComposite<TPixel>(evissequence)
{
    m_uCur        = 0;
    m_uFrameCount = 0;
}

template<class TPixel>
BOOL CVisWarpedVideoComposite<TPixel>::Open( const char* strmname,
                                             int seqidx = -1 )
{
    CVisStreamHandler* pStream = OpenStreamHandler( strmname, OF_READWRITE );

    if( pStream->FrameCount() > m_uFrameCount )
    {
        m_uFrameCount = pStream->FrameCount();
    }
    
    if( seqidx == -1 || seqidx >= Length() )
    {
        T_FRAME frm;
        
        frm.SetStreamHandler( pStream, m_uCur );
        PushBack( frm );
    }
    else
    {
        T_FRAME& frm = At(seqidx);
        
        // deallocate image currently in this slot then
        // set the new stream handler
        frm.Deallocate();
        frm.SetStreamHandler( pStream, m_uCur );
    }
}
    
template<class TPixel>
BOOL CVisWarpedVideoComposite<TPixel>::FrameAdvance( int step = 1 )
{
    m_uCur += step;
    
    if( m_uCur >= m_uFrameCount )
    {
        m_uCur -= step;
    }

    // loop over sequence and advance each frame
    for( UINT i = 0; i < Length(); i++ )
    {
        At(i).SetStreamHandler( frm.StreamHandler(), m_uCur );
    }
}

template<class TPixel>
BOOL CVisWarpedVideoComposite<TPixel>::FrameSet( UINT frame_num )
{
    if( frame_num == -1 )
    {
        m_uCur = m_uFrameCount - 1;
    }
    else if( frame_num < m_uFrameCount )
    {
        m_uCur = frame_num;
    }
    
    // loop over stream infos and set each frame number to frame_num
    for( UINT i = 0; i < Length(); i++ )
    {
        At(i).SetStreamHandler( frm.StreamHandler(), m_uCur );
    }
}
                                                        
template<class TPixel>
UINT CVisWarpedVideoComposite<TPixel>::FrameCur()
{
    return m_uCur;
}

template<class TPixel>
UINT CVisWarpedVideoComposite<TPixel>::FrameCount()
{
    return m_uFrameCount;
}


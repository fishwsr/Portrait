// VisAudioSamples.inl
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#ifndef __VIS_CORE_AUDIOSAMPLES_INL__
#define __VIS_CORE_AUDIOSAMPLES_INL__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG


/////////////////////////////////////////////////////////////////////////////////////
//
//  Class CVisAudioChannel
//
/////////////////////////////////////////////////////////////////////////////////////

inline UINT
CVisAudioChannel::SampleRate(void) const
{
    return m_uSampleRate;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  Class CVisAudioData
//
/////////////////////////////////////////////////////////////////////////////////////

inline BOOL
CVisAudioData::WriteToStream( CVisStreamHandler* pStream, INT64 StartTimeInStream )
{
    return pStream->WriteAudio( *this, StartTimeInStream );
}

inline INT64
CVisAudioData::Duration(void) const
{
    return m_ilDuration;
}
    
inline UINT
CVisAudioData::NumChannels(void) const
{    
    if( m_Channels.size() == 0 && m_pVisStreamHandler )
    {
        return m_pVisStreamHandler->AudioChannelCount();
    }
    else
    {
        return m_Channels.size();
    }
}

inline UINT
CVisAudioData::SampleRate(void) const
{    
    if( m_pVisStreamHandler )
    {
        return m_pVisStreamHandler->AudioSampleRate();
    }
    else if( m_Channels.size() == 0 || 
             m_Channels[0].m_pAudioChan == NULL )
    {
        return 0;
    }

    return m_Channels[0].m_pAudioChan->SampleRate();
}

inline CVisAudioChannel*
CVisAudioData::AudioChannel(UINT i) const
{    
    if( i >= m_Channels.size() )
	{
		return NULL;
	}

    return m_Channels[i].m_pAudioChan;
}

#ifdef _DEBUG
#undef new
#endif

#endif // __VIS_CORE_AUDIOSAMPLES_INL__


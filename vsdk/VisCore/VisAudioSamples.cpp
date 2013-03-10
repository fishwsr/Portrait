// VisAudioSamples.cpp
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved

#include "VisCorePch.h"
#include "VisCoreProj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define HUNDRED_NS_TICKS_PER_SEC  10000000 

// any fraction of a sample counts as a full sample
VisCoreExport long
AudioSampleNumber( INT64 StartTime, long SampleRate )
{
    INT64 num = (StartTime * SampleRate) / HUNDRED_NS_TICKS_PER_SEC;
    
    return (long)num;
}

// also here, any fraction of a sample counts as a full sample
VisCoreExport long
AudioSampleCount( INT64 Duration, long SampleRate )
{
    INT64 num = (Duration * SampleRate + HUNDRED_NS_TICKS_PER_SEC-1) / HUNDRED_NS_TICKS_PER_SEC;
    
    return (long)num;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  Class CVisAudioChannel
//
/////////////////////////////////////////////////////////////////////////////////////
VisCoreExport
CVisAudioChannel::CVisAudioChannel( UINT uSampleRate, bool fThrow) : 
    CVisStreamErrorBase( fThrow )
{
    m_iRefCount   = 1;
    m_uSampleRate = uSampleRate;
    m_ilDuration  = 0;

    m_uTotalSamples    = 0;
    m_puSamplesWritten = NULL;
    m_pfSamples        = NULL;

    m_uMemoryState       = AUDIO_CHANNEL_READ_ACCESS;
    m_uWaitingForWrite   = 0;
    m_uWaitingForRead    = 0;
    m_uMemoryAccessCount = 0;

    // XXX mattu these functions can fail, should be moved out of constructor
    m_MemoryReadEvent = CreateEvent(NULL, TRUE, TRUE, NULL );
    m_MemoryWriteSema = CreateSemaphore(NULL, 0, INT_MAX, NULL);
    InitializeCriticalSection(&m_MemoryStateMutex);
}

VisCoreExport 
CVisAudioChannel::~CVisAudioChannel()
{
    assert(m_uMemoryState==AUDIO_CHANNEL_READ_ACCESS);
    assert(m_uWaitingForWrite==0);
    assert(m_uWaitingForRead==0);
    assert(m_uMemoryAccessCount==0);

    if( m_puSamplesWritten )
        free(m_puSamplesWritten);
    if( m_pfSamples )
        free(m_pfSamples);

    CloseHandle(m_MemoryWriteSema);
    CloseHandle(m_MemoryReadEvent);
    DeleteCriticalSection(&m_MemoryStateMutex);
}

VisCoreExport int
CVisAudioChannel::AddRef()
{
    return InterlockedIncrement( &m_iRefCount );
}

VisCoreExport int
CVisAudioChannel::Release()
{
    int iNewCount = InterlockedDecrement( &m_iRefCount );
    if ( iNewCount == 0 )
    {
        delete this;
    }
    return iNewCount;
}

BOOL
CVisAudioChannel::MemoryAccessEvent( MEMORY_ACCESS_EVENT event )
{
    BOOL bWaitForRead  = FALSE;
    BOOL bWaitForWrite = FALSE;

    // XXX mattu can throw an exception
    EnterCriticalSection(&m_MemoryStateMutex);

    switch( event )
    {
    case MEMORY_ACCESS_START_READ:
        if( m_uWaitingForWrite || m_uMemoryState == AUDIO_CHANNEL_WRITE_ACCESS )
        {
            ResetEvent(m_MemoryReadEvent);
            m_uWaitingForRead++;
            bWaitForRead = TRUE;
        }
        else
        {
            m_uMemoryAccessCount++;
            m_uMemoryState = AUDIO_CHANNEL_READ_ACCESS;
        }
        break;

    case MEMORY_ACCESS_START_WRITE:
        if( m_uMemoryAccessCount )
        {
            m_uWaitingForWrite++;
            bWaitForWrite = TRUE;
        }
        else
        {
            m_uMemoryAccessCount++;
            m_uMemoryState = AUDIO_CHANNEL_WRITE_ACCESS;
        }
        break;

        
    case MEMORY_ACCESS_END_READ:
    case MEMORY_ACCESS_END_WRITE:
        if( m_uMemoryAccessCount == 0 )
        {
            VIS_STRM_FAIL_EXIT(E_FAIL,
                               "mismatched Start{Read/Write}, End{Read/Write} calls");
        }

        m_uMemoryAccessCount--;
        if( m_uMemoryAccessCount == 0 )
        {
            if( m_uWaitingForWrite )
            {
                m_uMemoryAccessCount++;
                m_uMemoryState = AUDIO_CHANNEL_WRITE_ACCESS;
                // free one waiting for write
                m_uWaitingForWrite--;
                ReleaseSemaphore(m_MemoryWriteSema, 1, NULL);
            }
            else
            {
                m_uMemoryState = AUDIO_CHANNEL_READ_ACCESS;
                // free all waiting for read
                if( m_uWaitingForRead )
                {
                    m_uMemoryAccessCount = m_uWaitingForRead;
                    m_uWaitingForRead = 0;
                    SetEvent(m_MemoryReadEvent);
                }
            }
        }
        break;
    
    default:
        assert(0);
        break;
    }

    // XXX mattu can throw an exception
    LeaveCriticalSection(&m_MemoryStateMutex);
    
    if( bWaitForRead )
    {
        WaitForSingleObject(m_MemoryReadEvent, INFINITE);
    }
    
    if( bWaitForWrite )
    {
        WaitForSingleObject(m_MemoryWriteSema, INFINITE);
    }

	return TRUE;
    
Exit:
    // XXX mattu can throw an exception
    LeaveCriticalSection(&m_MemoryStateMutex);

    return HandleError();
}

VisCoreExport const float*
CVisAudioChannel::StartRead( INT64 ilStartTime, INT64 ilDuration,
                             UINT* pNumSamples )
{    
    INT64 timeAvailable = m_ilDuration - ilStartTime;

    if( timeAvailable <= 0  )
    {
        return NULL;
    }
    
    MemoryAccessEvent(MEMORY_ACCESS_START_READ);

    if( timeAvailable < ilDuration )
    {
        ilDuration = timeAvailable;
    }

    *pNumSamples = AudioSampleCount ( ilDuration, m_uSampleRate );
    return m_pfSamples + AudioSampleNumber( ilStartTime, m_uSampleRate );
}

VisCoreExport BOOL
CVisAudioChannel::EndRead( INT64 ilStartTime, INT64 ilDuration )
{
    return MemoryAccessEvent(MEMORY_ACCESS_END_READ);
}

VisCoreExport float*
CVisAudioChannel::StartWrite( INT64 ilStartTime, INT64 ilDuration,
                              UINT* pNumSamples )
{
    MemoryAccessEvent(MEMORY_ACCESS_START_WRITE);

    INT64 reqDuration = ilStartTime + ilDuration;
    if( reqDuration > m_ilDuration )
    {
        m_ilDuration = reqDuration;

        UINT uPrevSampleCount = m_uTotalSamples;
        m_uTotalSamples = AudioSampleCount( m_ilDuration, m_uSampleRate );
        m_uTotalSamples = (m_uTotalSamples + 0x1f) & ~0x1f;

        // XXX mattu failure
        m_puSamplesWritten = (UINT*)realloc( m_puSamplesWritten, (m_uTotalSamples/32)*sizeof(UINT) );
        m_pfSamples = (float*)realloc( m_pfSamples, m_uTotalSamples*sizeof(float) );

        // intialize the written flags to 0
        for( UINT i=(uPrevSampleCount+31)/32; i<m_uTotalSamples/32; i++ )
        {
            m_puSamplesWritten[i] = 0;
        }
    }

    *pNumSamples = AudioSampleCount ( ilDuration, m_uSampleRate );
    return m_pfSamples + AudioSampleNumber( ilStartTime, m_uSampleRate );
}

VisCoreExport BOOL 
CVisAudioChannel::EndWrite( INT64 ilStartTime, INT64 ilDuration )
{
    // set the appropriate sample written flags to 1
    long iFrst = AudioSampleNumber( ilStartTime, m_uSampleRate );
    long iLast = iFrst + AudioSampleCount( ilDuration, m_uSampleRate ) - 1;

    if( iFrst & 0x1f )
    {
        UINT mask = 0xFFFFFFFF << (iFrst & 0x1f);
        m_puSamplesWritten[iFrst>>5] |= mask;
    }
    if( (iLast & 0x1f) != 0x1f )
    {
        UINT mask = (1 << (iLast & 0x1f)) - 1;
        m_puSamplesWritten[iLast>>5] |= mask;
    }

    for( UINT i = (iFrst + 31) >> 5; i <= ((iLast - 31) >> 5); i++ )
    {
        m_puSamplesWritten[i] = 0xFFFFFFFF;
    }

    return MemoryAccessEvent(MEMORY_ACCESS_END_WRITE);
}

VisCoreExport BOOL 
CVisAudioChannel::BeenWritten( INT64 ilStartTime, INT64 ilDuration )
{
	if( ilStartTime + ilDuration > m_ilDuration )
	{
		return FALSE;
	}

    // set the appropriate sample written flags to 1
    long iFrst = AudioSampleNumber( ilStartTime, m_uSampleRate );
    long iLast = iFrst + AudioSampleCount( ilDuration, m_uSampleRate ) - 1;

    if( iFrst & 0x1f )
    {
        UINT mask = 0xFFFFFFFF << (iFrst & 0x1f);
        if( (m_puSamplesWritten[iFrst>>5] & mask) != mask )
        {
            return FALSE;
        }
    }
    if( (iLast & 0x1f) != 0x1f )
    {
        UINT mask = (1 << (iLast & 0x1f)) - 1;
        if( (m_puSamplesWritten[iLast>>5] & mask) != mask )
        {
            return FALSE;
        }
    }

    for( UINT i = (iFrst + 31) >> 5; i <= ((iLast - 31) >> 5); i++ )
    {
        if( m_puSamplesWritten[i] != 0xFFFFFFFF )
        {
            return FALSE;
        }
    }

    return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////
//
//  Class CVisAudioData
//
/////////////////////////////////////////////////////////////////////////////////////

VisCoreExport 
CVisAudioData::CVisAudioData( bool fThrow ) :
    CVisStreamErrorBase( fThrow )
{
    m_ilDuration = 0;
	m_pVisStreamHandler = NULL;
}
    
VisCoreExport 
CVisAudioData::CVisAudioData( CVisAudioData& ref, INT64 ilStartOffset, INT64 ilDuration,
                              bool fThrow ) :
    CVisStreamErrorBase( fThrow )
{
    *this = ref;

    m_ilDuration   = ilDuration;
    m_ilStartTime += ilStartOffset;
}

VisCoreExport 
CVisAudioData::~CVisAudioData()
{
    if( m_pVisStreamHandler )
        m_pVisStreamHandler->Release();

    ClearChannels();
}

void
CVisAudioData::ClearChannels()
{
    if( m_Channels.size() )
    {
        T_AudioChanVec::iterator iteratorV    = m_Channels.begin();
        T_AudioChanVec::iterator iteratorVLim = m_Channels.end();
        for ( ; iteratorV != iteratorVLim; ++iteratorV )
        {
            if ( iteratorV->m_pAudioChan )
            {
                iteratorV->m_pAudioChan->Release();
            }
        }

        m_Channels.clear();
    }
    m_ilDuration = 0;
}

VisCoreExport BOOL
CVisAudioData::InitializeFromStream( CVisStreamHandler* pStream,
                                     INT64 StartTime,  INT64 Duration )
{
    if( pStream==NULL )
    {
        VIS_STRM_FAIL_RET(E_INVALIDARG,
                          "CVisStreamHandler cannot be NULL");
    }

	if( pStream->AudioDuration() == 0 )
	{
		return FALSE;
	}

    ClearChannels();

    m_ilStartTime = StartTime;
    m_ilDuration  = Duration;
    if( m_pVisStreamHandler )
        m_pVisStreamHandler->Release();
    m_pVisStreamHandler = pStream;
    pStream->AddRef();

    return TRUE;
}

VisCoreExport BOOL
CVisAudioData::InitializeChannel( UINT uChannelNum, CVisAudioChannel* pChannel, 
                                  INT64 StartTime,  INT64 Duration )
{
    if( pChannel==NULL )
    {
        VIS_STRM_FAIL_RET(E_INVALIDARG,
                          "CVisAudioChannel cannot be NULL");
    }
    
    if( m_ilDuration==0 )
    {
        m_ilDuration = Duration;
    }
    else 
    {
        if( m_ilDuration != Duration )
        {
            VIS_STRM_FAIL_RET(E_INVALIDARG,
                              "All channels must have the same duration");
        }
        if( pChannel->SampleRate() != SampleRate() )
        {
            VIS_STRM_FAIL_RET(E_INVALIDARG,
                              "All channels must have the same sample-rate");
        }
    }

    ChannelInfo info;
    info.m_ilStartTime = StartTime;
    info.m_pAudioChan  = pChannel;
    pChannel->AddRef();

    if( uChannelNum >= m_Channels.size() )
    {
        ChannelInfo info2;
        info2.m_ilStartTime = 0;
        info2.m_pAudioChan  = NULL;
        
        while( uChannelNum != m_Channels.size() )
        {
            m_Channels.push_back(info2);
        }
        m_Channels.push_back(info);
    }
    else
    {
        ChannelInfo& cur = m_Channels[uChannelNum];
        if( cur.m_pAudioChan )
        {
            cur.m_pAudioChan->Release();
        }
        cur = info;
    }

    return TRUE;
}

VisCoreExport CVisAudioData&
CVisAudioData::operator=(const CVisAudioData& src)
{
    m_ilDuration  = src.m_ilDuration;
    m_ilStartTime = src.m_ilStartTime;
    
    if( src.m_Channels.size() )
    {
        T_AudioChanVec::const_iterator iteratorV    = src.m_Channels.begin();
        T_AudioChanVec::const_iterator iteratorVLim = src.m_Channels.end();
        for ( ; iteratorV != iteratorVLim; ++iteratorV )
        {
            if( iteratorV->m_pAudioChan )
            {
                iteratorV->m_pAudioChan->AddRef();
            }
            m_Channels.push_back(*iteratorV);
        }
    }

    m_pVisStreamHandler = src.m_pVisStreamHandler;
    if( m_pVisStreamHandler )
    {
        m_pVisStreamHandler->AddRef();
    }

	return *this;
}

#define CHK_AND_GET_INFO_FLOAT( n ) \
    if( (n) >= m_Channels.size() ) \
    {\
        SetError( "Invalid channel number\n", __FILE__, __LINE__, E_INVALIDARG);\
        HandleError();\
        return NULL;\
    }\
    ChannelInfo& info = m_Channels[(n)];\
    if( info.m_pAudioChan == NULL )\
    {\
        SetError( "Accessing un-initialized channel\n", __FILE__, __LINE__, E_INVALIDARG);\
        HandleError();\
        return NULL;\
    }

#define CHK_AND_GET_INFO( n ) \
    if( (n) >= m_Channels.size() ) \
    {\
        VIS_STRM_FAIL_RET(E_INVALIDARG, "Invalid channel number\n");\
    }\
    ChannelInfo& info = m_Channels[(n)];
    
#define CHK_INFO_CHAN() \
    if( info.m_pAudioChan == NULL )\
    {\
        VIS_STRM_FAIL_RET(E_INVALIDARG, "Accessing un-initialized channel");\
    }


VisCoreExport const float*
CVisAudioData::StartReadChannel( UINT uChannelNum, INT64 ilOffset, INT64 ilDuration, UINT* pNumSamples )
{
    if( m_Channels.size() == 0 && m_pVisStreamHandler )
    {
        if ( !m_pVisStreamHandler->ReadAudio( *this, m_ilStartTime, m_ilDuration ) )
        {
            return FALSE;
        }
    }

    CHK_AND_GET_INFO_FLOAT(uChannelNum);
    return info.m_pAudioChan->StartRead(info.m_ilStartTime + ilOffset, ilDuration,
                                        pNumSamples);
}

VisCoreExport BOOL
CVisAudioData::EndReadChannel( UINT uChannelNum, INT64 ilOffset, INT64 ilDuration )
{
    CHK_AND_GET_INFO(uChannelNum);
    CHK_INFO_CHAN();
    return info.m_pAudioChan->EndRead(info.m_ilStartTime + ilOffset, ilDuration);
}
    
VisCoreExport float*
CVisAudioData::StartWriteChannel( UINT uChannelNum, INT64 ilOffset, INT64 ilDuration, UINT* pNumSamples )
{
    CHK_AND_GET_INFO_FLOAT(uChannelNum);
    return info.m_pAudioChan->StartWrite(info.m_ilStartTime + ilOffset, ilDuration,
                                         pNumSamples);
}

VisCoreExport BOOL
CVisAudioData::EndWriteChannel( UINT uChannelNum, INT64 ilOffset, INT64 ilDuration )
{
    CHK_AND_GET_INFO(uChannelNum);
    CHK_INFO_CHAN();
    return info.m_pAudioChan->EndWrite(info.m_ilStartTime + ilOffset, ilDuration);
}

VisCoreExport INT64
CVisAudioData::StartTime(UINT uChannelNum)
{
    CHK_AND_GET_INFO(uChannelNum);
    return info.m_ilStartTime;
}

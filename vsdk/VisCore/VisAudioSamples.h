// VisAudioSamples.h
//
// Copyright  2000 Microsoft Corporation, All Rights Reserved


#ifndef __VIS_CORE_AUDIO_SAMPLES_H__
#define __VIS_CORE_AUDIO_SAMPLES_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <vector>

//
// function used to keep a consistent relationship between time and
// sample number through out
// Time is in 100ns ticks
// Rate is in samples/sec
//
VisCoreExport long  AudioSampleNumber( INT64 StartTime, long SampleRate );
VisCoreExport long  AudioSampleCount ( INT64 Duration, long SampleRate );

enum AUDIO_CHANNEL_STATE
{
    AUDIO_CHANNEL_READ_ACCESS  = 0,
    AUDIO_CHANNEL_WRITE_ACCESS = 1
};

enum MEMORY_ACCESS_EVENT
{
    MEMORY_ACCESS_START_READ   = 0,
    MEMORY_ACCESS_END_READ     = 1,
    MEMORY_ACCESS_START_WRITE  = 2,
    MEMORY_ACCESS_END_WRITE    = 3
};

class CVisAudioChannel : public CVisStreamErrorBase
{
public:
    VisCoreExport CVisAudioChannel( UINT uSampleRate, bool fThrow = true );
    VisCoreExport ~CVisAudioChannel();

    VisCoreExport int AddRef();
    VisCoreExport int Release();

    inline UINT   SampleRate(void) const;
    
    // used to read the channel data
    VisCoreExport const float* StartRead( INT64 ilStartTime,
                                          INT64 ilDuration,
                                          UINT* pNumSamples );
    VisCoreExport BOOL   EndRead( INT64 ilStartTime, INT64 ilDuration );

    // used to write channel data
    VisCoreExport float* StartWrite( INT64 ilStartTime,
                                     INT64 ilDuration,
                                     UINT* pNumSamples );
    VisCoreExport  BOOL  EndWrite( INT64 ilStartTime, INT64 ilDuration );

    // verify that data in a certain time range has been written
    VisCoreExport BOOL   BeenWritten( INT64 ilStartTime, INT64 ilDuration );

private:
    BOOL   MemoryAccessEvent( MEMORY_ACCESS_EVENT event );

    long         m_iRefCount;

    UINT         m_uSampleRate;
    INT64        m_ilDuration;

    AUDIO_CHANNEL_STATE m_uMemoryState;
    CRITICAL_SECTION    m_MemoryStateMutex;
    UINT         m_uMemoryAccessCount;
    UINT         m_uWaitingForWrite;
    HANDLE       m_MemoryWriteSema;
    UINT         m_uWaitingForRead;
    HANDLE       m_MemoryReadEvent;

    UINT         m_uTotalSamples;
    UINT*        m_puSamplesWritten;
    float*       m_pfSamples;
};

class CVisAudioData : public CVisStreamErrorBase
{
public:
    VisCoreExport CVisAudioData( bool fThrow = true );
    VisCoreExport CVisAudioData( CVisAudioData& ref, INT64 ilStartOffset, INT64 ilDuration,
                                 bool fThrow = true );
    VisCoreExport ~CVisAudioData( );

    // method to use when reading from a stream
    VisCoreExport BOOL   InitializeFromStream( CVisStreamHandler*,
                                               INT64 StartTime,
                                               INT64 Duration );
    
    // methods used when writing to a stream
    inline BOOL   WriteToStream( CVisStreamHandler* pStream,
                                 INT64 StartTimeInStream );


    // methods used to intialize and access the audio data
    VisCoreExport BOOL   InitializeChannel( UINT uChannelNum, CVisAudioChannel*, 
                                            INT64 StartTime,  INT64 Duration );

    VisCoreExport const float* StartReadChannel ( UINT uChannelNum, INT64 ilOffset, 
                                                  INT64 ilDuration, UINT* pNumSamples ); 
    VisCoreExport BOOL   EndReadChannel    ( UINT uChannelNum, INT64 ilOffset, INT64 ilDuration ); 
    
    VisCoreExport float* StartWriteChannel ( UINT uChannelNum, INT64 ilOffset, 
                                             INT64 ilDuration, UINT* pNumSamples ); 
    VisCoreExport BOOL   EndWriteChannel   ( UINT uChannelNum, INT64 ilOffset, INT64 ilDuration ); 
    
    VisCoreExport CVisAudioData& operator=(const CVisAudioData& Src);

    // accessor methods for private data
    VisCoreExport INT64  StartTime(UINT uChannelNum);
    inline INT64  Duration(void)    const;
    inline UINT   NumChannels(void) const;
    inline UINT   SampleRate(void)  const;
    inline CVisAudioChannel* AudioChannel(UINT i) const;

private:
    void   ClearChannels();

    typedef struct {
        INT64             m_ilStartTime;
        CVisAudioChannel* m_pAudioChan;
    } ChannelInfo;
    typedef std::vector<ChannelInfo> T_AudioChanVec;
        
    INT64              m_ilDuration;
    INT64              m_ilStartTime;
    T_AudioChanVec     m_Channels;

    CVisStreamHandler* m_pVisStreamHandler;
};

#include "VisAudioSamples.inl"

#endif // __VIS_CORE_AUDIO_SAMPLES_H__

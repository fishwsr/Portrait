// VisFileStreamIO.h
//
// Copyright © 1999-2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////

#ifndef __VIS_CORE_FILESTREAMIO_H__
#define __VIS_CORE_FILESTREAMIO_H__

#if _MSC_VER >= 1000
    #pragma once
#endif // _MSC_VER >= 1000

#include <vfw.h>
#include "VisImageBase.h"
#include "ddraw.h"        // DirectDraw interfaces
#include "mmstream.h"    // Multimedia stream interfaces
#include "amstream.h"    // DirectShow multimedia stream interfaces
#include "ddstream.h"    // DirectDraw multimedia stream interfaces

#include <atlbase.h>

// forward declarations
class CVisAudioData;
class CVisAudioChannel;

////////////////////////////////////////////////////////////////////////////
//  
// @class
//
// The base stream handler class.  All stream handlers inherit from this class.
// The pure virtual functions in this class must be defined in the
// derived classes.
//  
//
////////////////////////////////////////////////////////////////////////////
class VisCoreExport CVisStreamHandler 
{
public:
    virtual int  AddRef()  = 0;
    virtual int  Release() = 0;

    virtual BOOL Open( const char* filename, UINT open_flags,
                       UINT uBitCount = 32, bool fCompressorDlg = FALSE ) = 0;
    virtual BOOL ReadFrame(   CVisImageBase& img,  INT frame_num = -1 ) = 0;
    virtual BOOL WriteFrame(  CVisImageBase& img,  INT frame_num = -1 ) = 0;

    virtual BOOL ReadAudio(  CVisAudioData& aud,  INT64 StartTime, INT64 Duration ) = 0;
    virtual BOOL WriteAudio( CVisAudioData& aud,  INT64 StartTime ) = 0;
    
    virtual void Close( ) = 0;

    virtual const char* FileName(void)   const = 0;
    virtual UINT        Mode()           const = 0;
    
    // video specific parameters
    virtual int         FrameCount(void)    const = 0;
    virtual INT64       FrameDuration(void) const = 0;
    virtual INT64       TimeFromFrameCount( int count ) const = 0;
    virtual UINT        Width(void)      const = 0;
    virtual int         Height(void)     const = 0;
    virtual EVisPixFmt  PixFmt(void)     const = 0;

    // audio specific parameters
    virtual UINT       AudioSampleRate(void) const = 0;
    virtual UINT       AudioChannelCount(void) const = 0;
    virtual UINT       AudioDuration(void) const = 0;

    virtual HRESULT    GetLastError() = 0;
};

VisCoreExport CVisStreamHandler* OpenStreamHandler( const char* filename, 
                                                    UINT open_flags,
                            UINT uBitCount = 32,
                                                    bool fCompressorDlg = FALSE,
                                                    bool fThrow=TRUE );
// error handling for stream i/o classess
class VisCoreExport CVisStreamErrorBase
{
protected:
    CVisStreamErrorBase( bool fThrow )
    {
        m_fThrow    = fThrow;
        m_latchedhr = S_OK;
    }

    BOOL HandleError()
    {
        if ( m_fThrow )
        {
            throw CVisError(m_pszErrorMessage, m_latchedhr, "",
                            m_pszFileName, m_uErrorLine);
        }
        return FALSE;
    }


    void SetError(const char* szErrorMessage, const char* szFileName, 
                  UINT uErrorLine, HRESULT hr)
    {
        if ( m_latchedhr == S_OK )
        {
            m_pszErrorMessage = szErrorMessage;
            m_pszFileName     = szFileName;
            m_uErrorLine      = uErrorLine;
            m_latchedhr       = hr;
        }
    }

    HRESULT GetError()
    {
        HRESULT hr = m_latchedhr;
        m_latchedhr = S_OK;
        return m_latchedhr;
    }

protected:
    bool        m_fThrow;

private:
    const char* m_pszErrorMessage;
    const char* m_pszFileName;
    UINT        m_uErrorLine;
    HRESULT     m_latchedhr;
};


#define VIS_STRM_FAIL_RET( hr, msg ) { \
                                        SetError( msg, __FILE__, __LINE__, hr);\
                                        return HandleError();\
                                     }

#define VIS_STRM_FAIL_EXIT( hr, msg ) { \
                                        SetError( msg, __FILE__, __LINE__, hr);\
                                        goto Exit;\
                                    }

#define VIS_STRM_GTHR(h, msg)   {  HRESULT hr = h;\
                                    if( FAILED(hr) ){\
                                        SetError( msg, __FILE__, __LINE__, hr);\
                                        goto Exit;\
                                    }\
                                 }

//
// specific stream handler classes
//
class VisCoreExport CVisAviStreamHandler : public CVisStreamHandler,
                                           public CVisStreamErrorBase
{
public:
    CVisAviStreamHandler(bool fThrow = TRUE);
    virtual ~CVisAviStreamHandler();

    virtual int  AddRef();
    virtual int  Release();

    virtual BOOL Open( const char* filename, UINT open_flags,
                       UINT uBitCount = 32,
                       bool fCompressorDlg = FALSE );
    virtual BOOL ReadFrame(  CVisImageBase& img, INT frame_num = -1 );
    virtual BOOL WriteFrame( CVisImageBase& img, INT frame_num = -1 );

    virtual BOOL ReadAudio(  CVisAudioData& aud,  INT64 StartTime, INT64 Duration );
    virtual BOOL WriteAudio( CVisAudioData& aud,  INT64 StartTime );

    virtual void Close( );

    virtual const char* FileName(void)   const;
    virtual UINT        Mode()           const;

    // video specific parameters
    virtual int         FrameCount(void) const;
    virtual INT64       FrameDuration(void) const;
    virtual INT64       TimeFromFrameCount( int count ) const;
    virtual UINT        Width(void)      const;
    virtual int         Height(void)     const;
    virtual EVisPixFmt  PixFmt(void)     const;

    // audio specific parameters
    virtual UINT       AudioSampleRate(void) const;
    virtual UINT       AudioChannelCount(void) const;
    virtual UINT       AudioDuration(void) const;

    virtual HRESULT    GetLastError();

protected:
    BOOL           OpenVideoStream( BITMAPINFO *pbmi, bool fOneIter );
    BOOL           OpenAudioStream( );

    long           m_iRefCount;

    PAVIFILE       m_paviFile;         // The AVI file
    PAVISTREAM     m_paviVideoStream;  // The AVI Video stream
    PAVISTREAM     m_paviCVideoStream; // The compressed AVI Video stream
    PGETFRAME      m_pgfFrame;         // Ptr in the stream to get a frame
    PAVISTREAM     m_paviAudioStream;  // The AVI Audio stream

    // avi description members
    char*          m_pzFileName;
    UINT           m_uMode;
    BOOL           m_bCompressorDlg;

    // video description
    UINT           m_uFrameCount;
    AVISTREAMINFO  m_strhdrVideo;
    INT            m_iHeight;
    UINT           m_uWidth;
    EVisPixFmt     m_ePixFmt;
    UINT           m_uPixelByteCount;

    // audio description
    AVISTREAMINFO  m_strhdrAudio;
    WAVEFORMATEX   m_AudioFormat;
    INT64          m_ilAudioDuration;

    // video data tmp storage
    BYTE          *m_pData;
    UINT           m_uDataSize;

    // audio data storage
    typedef std::vector<CVisAudioChannel*> T_AudioChanVec;
        
    T_AudioChanVec m_pAudioChannels;


    BOOL           m_bFirstVideoWrite;
    BOOL           m_bFirstAudioWrite;

    UINT           m_uCurFrame;

	long           m_cbHeader;
};

class VisCoreExport CVisDShowStreamHandler : public CVisStreamHandler,
                                             public CVisStreamErrorBase
{
public:
    CVisDShowStreamHandler(bool fThrow = TRUE);
    virtual ~CVisDShowStreamHandler();

    virtual int  AddRef();
    virtual int  Release();

    virtual BOOL Open( const char* filename, UINT open_flags,
                       UINT uBitCount = 32,
                       bool fCompressorDlg = FALSE );
    virtual BOOL ReadFrame(  CVisImageBase& img, INT frame_num = -1 );
    virtual BOOL WriteFrame( CVisImageBase& img, INT frame_num = -1 );

    virtual BOOL ReadAudio(  CVisAudioData& aud,  INT64 StartTime, INT64 Duration )
    { assert(0); return FALSE; };
    virtual BOOL WriteAudio( CVisAudioData& aud,  INT64 StartTime )
    { assert(0); return FALSE; }

    virtual void Close( );

    virtual const char* FileName(void)   const;
    virtual UINT        Mode()           const;

    // video specific parameters
    virtual int         FrameCount(void) const;
    virtual INT64       FrameDuration(void) const;
    virtual INT64       TimeFromFrameCount( int count ) const;
    virtual UINT        Width(void)      const;
    virtual int         Height(void)     const;
    virtual EVisPixFmt  PixFmt(void)     const;

    // audio specific parameters
    virtual UINT       AudioSampleRate(void) const 
    { assert(0); return 0; };
    virtual UINT       AudioChannelCount(void) const
    { assert(0); return 0; };
    virtual UINT       AudioDuration(void) const
    { assert(0); return 0; };

    virtual HRESULT     GetLastError();

protected:
    BOOL    InitDShowComponents( const char* szFileName, UINT open_flags );
    BOOL    CreateRenderSurface();
    BOOL    BltToImage( IDirectDrawStreamSample* pSample, CVisImageBase& img );
    
    long    m_iRefCount;

    // DX pointers
    IDirectDraw                         *m_pDD;
    
    CComPtr<IMultiMediaStream>       m_pMMStream;
    CComPtr<IDirectDrawMediaStream>  m_pDDStream;
    CComPtr<IMediaSeeking>           m_pMediaSeeking;
    CComPtr<IDirectDrawStreamSample> m_pSample;
    CComPtr<IDirectDrawSurface3>     m_pRenderSurface;
    
    DDSURFACEDESC            m_ddsd;

    // stream description members
    char*          m_pzFileName;
    UINT           m_uMode;
    UINT           m_uFrameCount;
    EVisPixFmt     m_ePixFmt;
    STREAM_TIME    m_stTimePerFrame;

    BOOL           m_bOpenForWrite;
    BOOL           m_bOpenForRead;
    UINT           m_uCurFrame;
};

#endif
// VisDShowStreamIO.cpp
//
// Copyright © 1999-2000 Microsoft Corporation, All Rights Reserved

#include "VisCorePch.h"
#include "VisCoreProj.h"
#include "VisFileStreamIO.h"

#include <io.h>
#include <uuids.h>


#if (DIRECTDRAW_VERSION < 0x0700)
// special header info for controlling the DV decoder
// got this from the DSHOW guys it didn't make it into DShow6 docs

enum _DVDECODERRESOLUTION {
    DVDECODERRESOLUTION_720x480     =   1000,
    DVDECODERRESOLUTION_360x240     =   1001,
    DVDECODERRESOLUTION_180x120     =   1002,
    DVDECODERRESOLUTION_88x60       =   1003
};

MIDL_INTERFACE("b8e8bd60-0bfe-11d0-af91-00aa00b67a42")
IIPDVDec : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE get_IPDisplay (
            int *displayPix        // The display pixels arrage
                 ) = 0;

    virtual HRESULT STDMETHODCALLTYPE put_IPDisplay (
             int displayPix            // Change to this display pixel arrage
                 ) = 0;
};

#endif // (DIRECTDRAW_VERSION < 0x0700)

//extern const IID IID_IIPDVDec;
const IID IID_IIPDVDec = { 0xb8e8bd60, 0x0bfe, 0x11d0, 0xaf, 0x91, 0x00, 0xaa, 0x00, 0xb6, 0x7a, 0x42 };


CVisDShowStreamHandler::CVisDShowStreamHandler( bool fThrow ) :
    CVisStreamErrorBase( fThrow )
{
    m_iRefCount   = 1;

    m_pMediaSeeking  = NULL;
    m_pSample        = NULL;
    m_pRenderSurface = NULL;
    m_pDDStream      = NULL;
    m_pMMStream      = NULL;
    m_pDD            = NULL;

    m_pzFileName  = NULL;
    m_uMode       = 0;
    m_uFrameCount = 0;
    m_ePixFmt     = evispixfmtNil;

    ZeroMemory( &m_ddsd, sizeof(m_ddsd) );
    m_ddsd.dwSize = sizeof(m_ddsd);
}

CVisDShowStreamHandler::~CVisDShowStreamHandler()
{
    Close();
}

int
CVisDShowStreamHandler::AddRef()
{
    return InterlockedIncrement( &m_iRefCount );
}

int
CVisDShowStreamHandler::Release()
{
    int iNewCount = InterlockedDecrement( &m_iRefCount );
    if ( iNewCount == 0 )
    {
        delete this;
    }
    return iNewCount;
}

BOOL 
CVisDShowStreamHandler::InitDShowComponents( const char* szFileName, UINT open_flags )
{ 
    CComPtr<IAMMultiMediaStream> pAMStream;
    CComPtr<IMediaStream>        pPrimaryVidStream;
    CComPtr<IGraphBuilder>       pGraphBuilder;

    CComPtr<IBaseFilter>         pDVVidDecoder; 

    VIS_STRM_GTHR( CoCreateInstance(CLSID_AMMultiMediaStream, NULL, CLSCTX_INPROC_SERVER,
                                    IID_IAMMultiMediaStream, (void **)&pAMStream),
                   "Can't CreateInstance" );

     VIS_STRM_GTHR( pAMStream->Initialize(STREAMTYPE_READ, AMMSF_NOGRAPHTHREAD, NULL),
                   "Initialize failed" );
     
    VIS_STRM_GTHR( pAMStream->AddMediaStream(m_pDD, &MSPID_PrimaryVideo, 0, NULL),
                   "Can't add primary video stream" );

    // Create a widechar version of the file name
    WCHAR    wPath[MAX_PATH];  
    MultiByteToWideChar(CP_ACP, 0, szFileName, -1, wPath, sizeof(wPath)/sizeof(wPath[0]));    
     VIS_STRM_GTHR( pAMStream->OpenFile(wPath, AMMSF_NOCLOCK), "Can't OpenFile" );
    
    VIS_STRM_GTHR( pAMStream->QueryInterface(IID_IMultiMediaStream, (void**)&m_pMMStream),
                   "QI for IID_IMultiMediaStream failed" );
    
    STREAM_TIME duration;
    VIS_STRM_GTHR( m_pMMStream->GetDuration(&duration),
                   "GetDuration failed" );

     VIS_STRM_GTHR( m_pMMStream->GetMediaStream(MSPID_PrimaryVideo, &pPrimaryVidStream),
                   "Get PrimaryVideo media stream failed" );
     
    VIS_STRM_GTHR( pPrimaryVidStream->QueryInterface(IID_IDirectDrawMediaStream, 
                   (void **)&m_pDDStream), "QI for IDirectDrawMediaStream failed" );

    VIS_STRM_GTHR( pAMStream->GetFilterGraph( &pGraphBuilder ),
                   "GetFilterGraph failed" );

    // if this is a DV file specify to playback at full resolution
    if( S_OK == pGraphBuilder->FindFilterByName(L"DV Video Decoder", &pDVVidDecoder ) )
    {        
        CComPtr<IIPDVDec> pIIPDVDec;
        
        VIS_STRM_GTHR( pDVVidDecoder->QueryInterface( IID_IIPDVDec, (void**)&pIIPDVDec ),
                       "QI for IIPDVDec failed" );

        pIIPDVDec->put_IPDisplay(DVDECODERRESOLUTION_720x480);
    }
    
    VIS_STRM_GTHR( pGraphBuilder->QueryInterface( IID_IMediaSeeking,
                   (void**)&m_pMediaSeeking),
                   "QI for IID_IMediaSeeking failed" );

    VIS_STRM_GTHR( m_pDDStream->GetTimePerFrame(&m_stTimePerFrame),
                   "GetTimePerFrame failed" );

    m_uFrameCount = duration / m_stTimePerFrame;
        
    return TRUE;

    // error handling cleanup
Exit:
    return HandleError();
}

BOOL
CVisDShowStreamHandler::CreateRenderSurface()
{
    CComPtr<IDirectDrawSurface> pRenderSurface1;

     VIS_STRM_GTHR( m_pDDStream->GetFormat(&m_ddsd, NULL, NULL, NULL),
                    "GetFormat failed" );

    m_ddsd.dwFlags         = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
     m_ddsd.ddsCaps.dwCaps  = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;

    // required pixel format
    m_ddsd.ddpfPixelFormat.dwSize            = sizeof(DDPIXELFORMAT);
    m_ddsd.ddpfPixelFormat.dwFlags           = DDPF_RGB;
    m_ddsd.ddpfPixelFormat.dwRGBBitCount     = 32;
    m_ddsd.ddpfPixelFormat.dwRBitMask        = 0xFF0000;
    m_ddsd.ddpfPixelFormat.dwGBitMask        = 0x00FF00;
    m_ddsd.ddpfPixelFormat.dwBBitMask        = 0x0000FF;
 
    VIS_STRM_GTHR( m_pDD->CreateSurface(&m_ddsd, &pRenderSurface1, NULL),
                    "Create rendering surface failed" );
     
    VIS_STRM_GTHR( m_pDDStream->CreateSample(pRenderSurface1, NULL, 0, &m_pSample),
                    "CreateSample failed" );

    VIS_STRM_GTHR( m_pMMStream->SetState(STREAMSTATE_RUN),
                    "Can't set stream state to run" );
    
    VIS_STRM_GTHR( m_pMediaSeeking->SetTimeFormat(&TIME_FORMAT_FRAME),
                    "Can't set media seeking time format" );

    // This isn't a fatal error. The only Surface3 functionality used
    // is setsurfacedesc, to wrap a ddraw surface around a cvisimage.
    // If version 3 isn't supported the subsequent code forgoes using 
    // setsurfacedesc and simply does a blit.  See ReadFrame().
    pRenderSurface1->QueryInterface( IID_IDirectDrawSurface3,
                                     (void**)&m_pRenderSurface );

    m_ePixFmt = evispixfmtRGBAByte;
    
    return TRUE;

Exit:
    return HandleError();
}

BOOL
CVisDShowStreamHandler::Open( const char* szFileName, UINT open_flags,
                  UINT uBitCount, bool fCompressorDlg )
{
    BOOL rval = TRUE;

    if( open_flags & (OF_WRITE|OF_READWRITE ) )
    {
        VIS_STRM_FAIL_RET(E_INVALIDARG, "Write is not implemented in DShowStreamHandler");
    }

    Close();

    m_uCurFrame = 0;
    m_uMode     = open_flags;
    m_pzFileName = strdup(szFileName);
    if ( m_pzFileName == NULL )
    {
        VIS_STRM_FAIL_RET(E_OUTOFMEMORY, "Out of memeory");
    }

    m_pDD = PGlobals()->IDDraw();

    if( !InitDShowComponents( szFileName, open_flags ) )
    {
        return FALSE;
    }

    if( !(open_flags & OF_WRITE) || (open_flags & OF_READWRITE) )
    {
        rval = CreateRenderSurface();
    }

    return rval;

}

static void
FixAlphaChannel( UINT32* pBits, UINT uWidth, UINT uHeight )
{
    for( UINT i=0; i<uWidth*uHeight; i++ )
    {
        *pBits++ |= 0xFF000000;
    }
}

#define RECTHEIGHT(r) ((r).bottom - (r).top)
#define RECTWIDTH(r)  ((r).right - (r).left)

BOOL
CVisDShowStreamHandler::BltToImage( IDirectDrawStreamSample* pSample, CVisImageBase& img )
{
    BOOL rval = FALSE;

    CComPtr <IDirectDrawSurface> pSrcSurface;
    RECT rectSrc;
    
    VIS_STRM_GTHR( pSample->GetSurface(&pSrcSurface, &rectSrc),
                   "GetSurface call failed" );

    HDC hdcDest, hdcSrc;
    
    VIS_STRM_GTHR( pSrcSurface->GetDC(&hdcSrc),
                   "GetDC failed on ddraw surface" );
    
    if( hdcDest = img.Hdc() )
    {
        SetStretchBltMode( hdcDest, COLORONCOLOR );

        rval = StretchBlt(hdcDest, 0, 0, img.Width(), img.Height(),
                          hdcSrc,  rectSrc.left, rectSrc.top, 
                          RECTWIDTH(rectSrc), RECTHEIGHT(rectSrc), SRCCOPY);
        
        img.DestroyHdc();
    }
    
    pSrcSurface->ReleaseDC(hdcSrc);

    return rval;

Exit:
    return HandleError();
}

BOOL
CVisDShowStreamHandler::ReadFrame( CVisImageBase& img, INT frame_num )
{
    // UNDONE: failure cases
    if ( frame_num == -1 )
    {
        frame_num = m_uCurFrame;
    }

    if ( frame_num >= m_uFrameCount )
    {
        frame_num = m_uFrameCount - 1;
    }
    INT64 lframe_num = frame_num;

    // UNDONE mattu: should this handle resizing to incoming image size
    if ( img.IsValid() )
    { 
        if ( img.Height() != m_ddsd.dwHeight || img.Width() != m_ddsd.dwWidth )
        {
            VIS_STRM_FAIL_RET(E_INVALIDARG, "Invalid input image");
        }
    }
    else if( img.Width() != 0 && img.Height() != 0 )
    {
        if ( img.Height() != m_ddsd.dwHeight || img.Width() != m_ddsd.dwWidth )
        {
            VIS_STRM_FAIL_RET(E_INVALIDARG, "Invalid input image");
        }
                
        CVisShape memshape( 0, 0, (m_ddsd.dwWidth+1)&~1, m_ddsd.dwHeight );
        img.Allocate( img.Shape(), memshape );
    }
    else
    {
        CVisShape imgshape( 0, 0, m_ddsd.dwWidth,        m_ddsd.dwHeight );
        CVisShape memshape( 0, 0, (m_ddsd.dwWidth+1)&~1, m_ddsd.dwHeight );

        img.Allocate( imgshape, memshape );
    }

    // seek to appropriate frame
    if( frame_num != m_uCurFrame )
    {
        VIS_STRM_GTHR( m_pMMStream->Seek( frame_num ),
                       "SetPosition failed" );
    }
    m_ddsd.dwFlags = DDSD_PIXELFORMAT | DDSD_LPSURFACE;
    if ( img.PixFmt() == evispixfmtRGBAByte &&
         img.NBands() == 1 )
    {
        // wrap the ddraw surface around current image
        if( m_pRenderSurface )
        {
            // this code path (ddraw) requires the image pitch to be 8 byte aligned
            assert( (img.MemoryShape().Width() & 1)==0 );
            m_ddsd.lpSurface = (PVOID)((CVisRGBAByteImage&)img).PtrToFirstPixelInRow(img.Top());
            VIS_STRM_GTHR( m_pRenderSurface->SetSurfaceDesc( &m_ddsd, 0 ),
                            "SetSurfaceDesc failed" );
        }
        
        // decode the frame
        HRESULT hr1 = m_pSample->Update(0, NULL, NULL, NULL);
        VIS_STRM_GTHR( hr1, "Sample Update failed" );
        if( hr1 == MS_S_ENDOFSTREAM  )
        {
            m_pMMStream->Seek( 0 );
            m_uCurFrame = 0;
        }

        // if ddraw surface3 isn't supported do a blit
        if( !m_pRenderSurface )
        {
            if( !BltToImage( m_pSample, img ) )
                goto Exit;
        }

        FixAlphaChannel( (UINT32*)((CVisRGBAByteImage&)img).PtrToFirstPixelInRow(img.Top()), 
                          m_ddsd.dwWidth, m_ddsd.dwHeight );

    }
    else
    {
        // create a tmp image
        CVisRGBAByteImage tmpImage(m_ddsd.dwWidth, m_ddsd.dwHeight, 1, evisimoptDefault);
        tmpImage.Allocate(m_ddsd.dwWidth, m_ddsd.dwHeight);

        // wrap the ddraw surface around tmp image
        if( m_pRenderSurface )
        {
            m_ddsd.lpSurface = tmpImage.PbPixel(0, 0);
            VIS_STRM_GTHR( m_pRenderSurface->SetSurfaceDesc( (DDSURFACEDESC*)&m_ddsd, 0 ),
                            "SetSurfaceDesc failed" );
        }
        
        // decode the frame
        VIS_STRM_GTHR( m_pSample->Update(0, NULL, NULL, NULL),
                        "Sample Update failed" );
        
        // if ddraw surface3 isn't supported to a blit
        if( !m_pRenderSurface )
        {
            if( !BltToImage( m_pSample, tmpImage ) )
                goto Exit;
        }

        FixAlphaChannel( (UINT32*)tmpImage.PbPixel(0, 0),
                         m_ddsd.dwWidth, m_ddsd.dwHeight );

        // use CopyPixelsTo to reformat
        if ( !tmpImage.CopyPixelsTo(img) )
        {
            VIS_STRM_FAIL_RET(E_FAIL, "CopyPixelsTo failed");
        }
    }

    m_uCurFrame = frame_num + 1;

    return TRUE;

Exit:
    return HandleError();
}

BOOL
CVisDShowStreamHandler::WriteFrame( CVisImageBase& img, INT frame_num )
{
    VIS_STRM_FAIL_RET(E_INVALIDARG, "Write is not implemented in DShowStreamHandler");
}

void
CVisDShowStreamHandler::Close()
{   
    if ( m_pzFileName )
    {
        free( (void*)m_pzFileName );
        m_pzFileName = NULL;
    }

    if( m_pMMStream )
    {
        m_pMMStream->SetState(STREAMSTATE_STOP);
    }

    m_pMediaSeeking  = NULL;
    m_pSample        = NULL;
    m_pRenderSurface = NULL;
    m_pDDStream      = NULL;
    m_pMMStream      = NULL;
    m_pDD = NULL;

    m_uMode       = 0;
    m_uFrameCount = 0;
    m_ePixFmt     = evispixfmtNil;

    ZeroMemory( &m_ddsd, sizeof(m_ddsd) );
    m_ddsd.dwSize = sizeof(m_ddsd);
}

HRESULT
CVisDShowStreamHandler::GetLastError()
{
    return GetError();
}

const char* CVisDShowStreamHandler::FileName(void) const
{
    return m_pzFileName;
}

UINT CVisDShowStreamHandler::Mode(void) const
{
    return m_uMode;
}

int CVisDShowStreamHandler::FrameCount(void) const
{
    return m_uFrameCount;
}

INT64 CVisDShowStreamHandler::FrameDuration(void) const
{
    return m_stTimePerFrame;
}

INT64 CVisDShowStreamHandler::TimeFromFrameCount( int count ) const
{
    // this isn't as accurate as the AviStreamHandler because the multiple of any fractional
    // frame times is lost here.
    return m_stTimePerFrame * count; 
}

UINT CVisDShowStreamHandler::Width(void) const
{
    return m_ddsd.dwWidth;
}

int CVisDShowStreamHandler::Height(void) const
{
    return m_ddsd.dwHeight;
}

EVisPixFmt CVisDShowStreamHandler::PixFmt(void) const
{
    return m_ePixFmt;
}

VisCoreExport CVisStreamHandler*
OpenStreamHandler( const char* filename, UINT open_flags,
           UINT uBitCount, bool fCompressorDlg, bool fThrow )
{
    // XXX mattu determine which type of loader to instance
    //     based on filename extension
    CVisStreamHandler* pSH;
    
    if( ((open_flags & (OF_WRITE|OF_READWRITE)) != 0)
            || (CVisSequenceBase::AlwaysUseAVIStreamHandler()) )
    {
        pSH = new CVisAviStreamHandler(fThrow);
    }
    else
    {
        pSH = new CVisDShowStreamHandler(fThrow);
    }

    if ( pSH )
    {
        pSH->Open( filename, open_flags, uBitCount, fCompressorDlg );
    }

    return pSH;
}


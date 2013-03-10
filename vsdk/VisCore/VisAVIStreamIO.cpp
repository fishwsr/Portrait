// VisAVIStreamIO.cpp
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved

#include "VisCorePch.h"
#include "VisCoreProj.h"
#include "VisFileStreamIO.h"

#include <io.h>
#include <vfw.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CVisAviStreamHandler::CVisAviStreamHandler(bool fThrow) :
    CVisStreamErrorBase(fThrow)
{
    m_iRefCount   = 1;

    m_paviFile    = NULL;
    m_paviVideoStream  = NULL;
    m_paviCVideoStream = NULL;
    m_paviAudioStream  = NULL;
    m_pgfFrame    = NULL;

    m_pData          = NULL;
    m_uDataSize      = 0;
    m_bFirstVideoWrite = FALSE;
    m_bFirstAudioWrite = FALSE;

    m_pzFileName  = NULL;
    m_uMode       = 0;
    m_uFrameCount = 0;
    m_iHeight     = 0;
    m_uWidth      = 0;
    m_ePixFmt     = evispixfmtNil;
    m_uPixelByteCount = 0;

    m_ilAudioDuration = 0;

	m_cbHeader = 0;

    AVIFileInit();
}

CVisAviStreamHandler::~CVisAviStreamHandler()
{
    Close();

    AVIFileExit();
}

int
CVisAviStreamHandler::AddRef()
{
    return InterlockedIncrement( &m_iRefCount );
}

int
CVisAviStreamHandler::Release()
{
    int iNewCount = InterlockedDecrement( &m_iRefCount );
    if ( iNewCount == 0 )
    {
        delete this;
    }
    return iNewCount;
}

BOOL
CVisAviStreamHandler::OpenVideoStream( BITMAPINFO *pbmi, bool fOneIter )
{
    assert(m_paviVideoStream);
    
    // at a maximum request in following order, the initial conditions sent to 
    // this function may be such that this sequence is started at an intermediate step.
    // 1) 32bpp first row first
    // 2) 32bpp last row first
    // 3) 24bpp first row first
    // 4) 24bpp last row first
    // 5) fail
    while( 1 )
    {
        pbmi->bmiHeader.biBitCount  = m_uPixelByteCount * 8;
        pbmi->bmiHeader.biSizeImage = m_uWidth * m_iHeight * m_uPixelByteCount;
        
        m_pgfFrame = AVIStreamGetFrameOpen( m_paviVideoStream,  &(pbmi->bmiHeader) );
        
        if ( m_pgfFrame )
        {
            break;
        }
        
        if ( fOneIter || (m_uPixelByteCount == 3 && pbmi->bmiHeader.biHeight > 0) )
        {
            VIS_STRM_FAIL_RET(E_FAIL, "Can't open avi frames");
        }
        
        if( pbmi->bmiHeader.biHeight > 0 )
        {
            m_uPixelByteCount--;
        }
        pbmi->bmiHeader.biHeight = -pbmi->bmiHeader.biHeight;
    }
    return TRUE;
}

BOOL
CVisAviStreamHandler::OpenAudioStream( )
{
    for( UINT i=0; i<m_AudioFormat.nChannels; i++ )
    {
        CVisAudioChannel* pChan = new CVisAudioChannel(m_AudioFormat.nSamplesPerSec,
                                                       m_fThrow);
        if( pChan==NULL )
        {
            VIS_STRM_FAIL_RET(E_OUTOFMEMORY, "out of memory");
        }
        m_pAudioChannels.push_back(pChan);
    }

    return TRUE;
}

BOOL
CVisAviStreamHandler::Open( const char* filename, UINT open_flags,
                            UINT   uBitCount,
                            bool   fCompressorDlg )
{
    bool fFileExists = _access(filename,0) != -1;

    Close();

    // Open the AVI file
    m_bCompressorDlg = fCompressorDlg;

    m_uMode   = open_flags;
    VIS_STRM_GTHR( AVIFileOpen( &m_paviFile, filename, open_flags, NULL ), 
                    "AviFileOpen failed");

    m_pzFileName = strdup(filename);
    if ( m_pzFileName == NULL )
    {
        VIS_STRM_FAIL_RET(E_OUTOFMEMORY, "Out of memeory");
    }

    m_ePixFmt = evispixfmtRGBAByte;

    if ( fFileExists && !( open_flags & OF_CREATE ) )
    {
        // Open the first video stream in the file
        VIS_STRM_GTHR( AVIFileGetStream( m_paviFile, &m_paviVideoStream, streamtypeVIDEO, 0 ),
                        "AviFileGetStream failed");

        m_uFrameCount = AVIStreamLength(m_paviVideoStream);

        AVIStreamInfo(m_paviVideoStream, &m_strhdrVideo, sizeof(m_strhdrVideo));

		long cbFormat = 0;
        VIS_STRM_GTHR( AVIStreamFormatSize( m_paviVideoStream, 0, &cbFormat ),
                        "AVIStreamFormatSize failed");
		assert(cbFormat > 0);

		long cbRead = cbFormat;
		void *pvFormat = new char[cbFormat];
        AVIStreamReadFormat(m_paviVideoStream, 0, pvFormat, &cbRead);
		assert(cbRead == cbFormat);

		m_cbHeader = 0;  // Set to zero to use biSize in packed DIB as offset.
		if (cbRead >= sizeof(BITMAPINFOHEADER))
		{
			const BITMAPINFOHEADER *pbmih = (const BITMAPINFOHEADER *) pvFormat;
			m_iHeight = abs(pbmih->biHeight);
			m_uWidth = pbmih->biWidth;
			m_uPixelByteCount = ((pbmih->biBitCount == 32) ? 4: 3);
			if ((cbFormat > pbmih->biSize)
					&& (pbmih->biCompression == BI_RGB)
					&& ((pbmih->biBitCount == 24) || (pbmih->biBitCount == 32)))
			{
				// Special case:  Remember the size of the header information to get
				// the right offset to use with packed DIBs containing frame information.
				m_cbHeader = cbFormat;
			}
		}
		else if (cbRead >= sizeof(BITMAPCOREHEADER))
		{
			const BITMAPCOREHEADER *pbmch = (const BITMAPCOREHEADER *) pvFormat;
			m_iHeight = abs(pbmch->bcHeight);
			m_uWidth = pbmch->bcWidth;
			m_uPixelByteCount = 24;
		}
		else
		{
			// Fail
			m_iHeight = 0;
			m_uWidth = 0;
			m_uPixelByteCount = 0;
		}
		delete[] (char *) pvFormat;

		if ((m_iHeight == 0) || (m_uWidth == 0))
		{
			return false;
		}

        BITMAPINFO bmi;
		ZeroMemory(&bmi, sizeof(bmi));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
 		bmi.bmiHeader.biWidth = m_uWidth;
 		bmi.bmiHeader.biHeight = - m_iHeight;
 		bmi.bmiHeader.biPlanes = 1;
 		bmi.bmiHeader.biBitCount = m_uPixelByteCount * 8;
 		bmi.bmiHeader.biCompression = BI_RGB;

        if( !OpenVideoStream( &bmi, FALSE ) )
        {
            return FALSE;
        }

        m_bFirstVideoWrite = FALSE;
        
        // Open the first audio stream in the file
        AVIFileGetStream( m_paviFile, &m_paviAudioStream, streamtypeAUDIO, 0 );
        if( m_paviAudioStream )
        {
            AVIStreamInfo(m_paviAudioStream, &m_strhdrAudio, sizeof(m_strhdrAudio));

            m_ilAudioDuration  = ((INT64(m_strhdrAudio.dwLength * m_strhdrAudio.dwScale) * INT64(10000000)) / 
                                   INT64(m_strhdrAudio.dwRate));

            long frmt_size = sizeof(m_AudioFormat);
            AVIStreamReadFormat(m_paviAudioStream, 0, &m_AudioFormat, &frmt_size);

            if( !OpenAudioStream() )
            {
                return FALSE;
            }
        }

        m_bFirstAudioWrite = FALSE;
    }
    else
    {    
        m_uPixelByteCount = uBitCount / 8;

        m_bFirstVideoWrite = TRUE;
        m_bFirstAudioWrite = TRUE;
    }

    m_uCurFrame = 0;

    return TRUE;

Exit:
    return HandleError();
}

BOOL
CVisAviStreamHandler::ReadFrame( CVisImageBase& img, INT frame_num )
{
    assert(m_pgfFrame);

    // UNDONE mattu: should this handle resizing to incoming image size
    if ( img.IsValid() &&
         (img.Height() != m_iHeight ||
          img.Width()  != m_uWidth) )
    {
        VIS_STRM_FAIL_RET(E_INVALIDARG, "Invalid input image");
    }

    if ( frame_num == -1 )
    {
        frame_num = m_uCurFrame;
    }

    if ( frame_num >= m_uFrameCount )
    {
        frame_num = m_uFrameCount - 1;
    }

    const BITMAPINFO *pBMI
            = (const BITMAPINFO *) AVIStreamGetFrame( m_pgfFrame, frame_num );

	// Note:  If the format of frames in the AVI file matches the format that we're
	// requesting, we should use AVIStreamFormatSize instead of pBMI->bmiHeader.biSize
	// to find the start of the data.  In practice, we should be able to use
	// pBMI->bmiHeader.biSize when the format matches the format we're requesting.
//	long cbFormat = 0;
//	AVIStreamFormatSize(m_paviVideoStream, 0, &cbFormat);

    const BYTE *pBMIData;
	if (m_cbHeader != 0)
		pBMIData = (m_cbHeader + (const BYTE *) pBMI);
	else
        pBMIData = (pBMI->bmiHeader.biSize + (const BYTE *) pBMI);

    // Variables used to remember whether the image shape needs to be
    // cropped after reading.  (This is a special case used with delayed
    // reading in sequences).
    bool fCropAfterReading = false;
    RECT rectCrop;

    img.AllocateUsingPrevoiusShapeIfPossible(m_uWidth, m_iHeight);
    if ((img.Width() != m_uWidth) || (img.Height() != m_iHeight))
    {
        // Remember the desired RECT and temporarily change the image
        // rect to match the dimensions of the image that we're reading.
        fCropAfterReading = true;
        rectCrop = img.Rect();
        img.SetRect(img.MemoryRect().left, img.MemoryRect().top,
                img.MemoryRect().left + m_uWidth,
                img.MemoryRect().top + m_iHeight);
    }

    // If we have a 32-bit RGBA image, we'll copy pixel values into it.
    // Otherwise, we'll need to allocate a temporary 32-bit RGBA image.
    CVisRGBAByteImage *pimageRGBA;
    if (img.PixFmt() == evispixfmtRGBAByte)
        pimageRGBA = (CVisRGBAByteImage *) &img;
    else
        pimageRGBA = new CVisRGBAByteImage(img.Shape());

    assert(pimageRGBA->Width() == m_uWidth);
    assert(pimageRGBA->Height() == m_iHeight);
    assert(pimageRGBA->NBands() == 1);

    // Copy the pixels line-by-line.
    assert(sizeof(unsigned long) == 4);
    unsigned long *pulRowDest = (unsigned long *)
            (pimageRGBA->PixelAddress(pimageRGBA->StartPoint()));
    int dpulRowDest = pimageRGBA->CbRow() / 4;

    const BYTE *pbRowSrc = pBMIData;
    assert(m_uPixelByteCount * 8 == pBMI->bmiHeader.biBitCount);
    int dpbRowSrc = m_uPixelByteCount * pBMI->bmiHeader.biWidth;
    if (pBMI->bmiHeader.biHeight > 0)
    {
        pbRowSrc += (dpbRowSrc * (m_iHeight - 1));
        dpbRowSrc = - dpbRowSrc;
    }
    if (m_uPixelByteCount == 3)
    {
        for (int y = 0; y < m_iHeight; ++y)
        {
            const BYTE *pbCur = pbRowSrc;
            for (int x = 0; x < m_uWidth; ++x)
            {
                pulRowDest[x] = (0xff000000 | (pbCur[0])
                        | (pbCur[1] << 8) | (pbCur[2] << 16));
                pbCur += 3;
            }

            pulRowDest += dpulRowDest;
            pbRowSrc += dpbRowSrc;
        }
    }
    else
    {
        for (int y = 0; y < m_iHeight; ++y)
        {
            const BYTE *pbCur = pbRowSrc;
            memcpy( pulRowDest, pbRowSrc, m_uWidth*sizeof(UINT32) );
            pulRowDest += dpulRowDest;
            pbRowSrc += dpbRowSrc;
        }
    }

    if (pimageRGBA != (CVisRGBAByteImage *) &img)
    {
        // Need to copy image pixels.
        if (img.PixFmt() == evispixfmtGrayByte)
        {
            // Special case.
            img = VisBrightnessFromRGBA(*pimageRGBA);
        }
        else
        {
            if( !pimageRGBA->CopyPixelsTo(img) )
            {
                SetError("CopyPixelsTo failed", __FILE__, __LINE__, E_FAIL);
                delete pimageRGBA;
                return HandleError();
            }
        }

        delete pimageRGBA;
    }

    // Special cropping that may be needed with delayed reading.
    if (fCropAfterReading)
        img.SetRect(rectCrop);

    m_uCurFrame = frame_num + 1;

    return TRUE;
}

BOOL
CVisAviStreamHandler::WriteFrame( CVisImageBase& img, INT frame_num )
{
    if ( img.PixFmt() != evispixfmtRGBAByte )
    {
        VIS_STRM_FAIL_RET(E_INVALIDARG, "Incorrect pixel type");
    }

    if ( frame_num == -1 )
    {
        frame_num = m_uCurFrame;
    }

    if( frame_num > m_uFrameCount )
    {
        m_uFrameCount = frame_num;
    }

    if ( m_bFirstVideoWrite )
    {
        m_iHeight = img.Height();
        m_uWidth  = img.Width();
        m_ePixFmt = evispixfmtRGBAByte;

        AVICOMPRESSOPTIONS opts[1];
        ZeroMemory( opts, sizeof(AVICOMPRESSOPTIONS) );
        AVICOMPRESSOPTIONS *aopts = &opts[0];

        ZeroMemory(&m_strhdrVideo,sizeof(m_strhdrVideo));
        m_strhdrVideo.fccType    = streamtypeVIDEO;
        m_strhdrVideo.dwScale    = 1;
        m_strhdrVideo.dwRate     = 30;
        m_strhdrVideo.dwQuality  = (unsigned long)-1;
        m_strhdrVideo.rcFrame.right  = m_uWidth;
        m_strhdrVideo.rcFrame.bottom = m_iHeight;
        
        BITMAPINFO    bmi;
        ZeroMemory( &bmi, sizeof(bmi) );
        bmi.bmiHeader.biSize         = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biHeight       = m_iHeight;
        bmi.bmiHeader.biWidth        = m_uWidth;
        bmi.bmiHeader.biBitCount     = m_uPixelByteCount * 8;
        bmi.bmiHeader.biPlanes       = 1;
        bmi.bmiHeader.biCompression  = BI_RGB;
        
        VIS_STRM_GTHR( AVIFileCreateStream( m_paviFile, &m_paviVideoStream, &m_strhdrVideo ),
                        "Can't create avi stream" );

        //VIS_STRM_GTHR( AVIStreamSetFormat( m_paviVideoStream, 0, &bmi, sizeof(bmi) ),
        //                "Can't set avi stream format" );
        
        // optionally pop-up a dialog box
        if( m_bCompressorDlg )
        {
            AVISaveOptions( NULL, 0, 1, &m_paviVideoStream, &aopts);
        }
        else
        {
            aopts->fccHandler = 0x20424944; // Uncompressed
            aopts->dwFlags    = 8;
        }
        
        VIS_STRM_GTHR( AVIMakeCompressedStream( &m_paviCVideoStream, m_paviVideoStream, opts, NULL ),
                        "Can't make compressed stream" );

        VIS_STRM_GTHR( AVIStreamSetFormat( m_paviCVideoStream, 0, &bmi, sizeof(BITMAPINFOHEADER) ),
                        "Can't set compressed avi stream format" );

        if( (m_uMode & OF_READWRITE) && !m_pgfFrame)
        {
            if( !OpenVideoStream( &bmi, TRUE) )
            {
                return FALSE;
            }
        }
        m_bFirstVideoWrite = FALSE;
    }
    else
    {
        if ( img.Height() != m_iHeight ||
             img.Width()  != m_uWidth )
        {
            VIS_STRM_FAIL_RET(E_INVALIDARG, "All images must be the same size");
        }
    }

    {
    UINT uRowSize = (m_uWidth * m_uPixelByteCount + 3) & ~3;

            
    UINT uDataSize = uRowSize * m_iHeight;
    if( m_uDataSize < uDataSize )
    {
        // create a buffer in the format that AVIStreamWrite likes
        m_uDataSize = uDataSize;
        if( m_pData )
            delete [] m_pData;
        m_pData = new BYTE[m_uDataSize];
        if ( m_pData == NULL )
        {
            VIS_STRM_FAIL_RET(E_OUTOFMEMORY, "Out of memory");   
        }
    }
    
    // write last row first
    BYTE* pData = m_pData + uRowSize * (m_iHeight - 1);
    BYTE* pDataNext;

    if( m_uPixelByteCount == 4 )
    {
        for ( int y = img.Top(); y < img.Bottom(); y++ )
        {
            pDataNext = pData - uRowSize;
            UINT32* pDataSrc = (UINT32*)((CVisRGBAByteImage&)img).PtrToFirstPixelInRow(y);
            for ( UINT x = 0; x < m_uWidth; x++ )
            {
                *(UINT32*)pData = *pDataSrc;
                pData += 4;
                pDataSrc++;
            }
            pData = pDataNext;
        }
    }
    else // bytecount must be 3
    {
        assert( m_uPixelByteCount == 3 );
        for ( int y = img.Top(); y < img.Bottom(); y++ )
        {
            pDataNext = pData - uRowSize;
            BYTE* pDataSrc = (BYTE*)((CVisRGBAByteImage&)img).PtrToFirstPixelInRow(y);
            for ( UINT x = 0; x < m_uWidth; x++ )
            {
                pData[2] = pDataSrc[2];
                pData[1] = pDataSrc[1];
                pData[0] = pDataSrc[0];
                pData += 3;
                pDataSrc += 4;
            }
            pData = pDataNext;
        }
    }
    
    VIS_STRM_GTHR( AVIStreamWrite( m_paviCVideoStream, frame_num, 1, m_pData,
                                   uDataSize, AVIIF_KEYFRAME, NULL, NULL ),
                    "Write to AVI stream failed" );
    }

    m_uCurFrame = frame_num + 1;

    return TRUE;

Exit:
    return HandleError();
}

BOOL
CVisAviStreamHandler::ReadAudio(  CVisAudioData& aud,  INT64 StartTime, INT64 Duration )
{
    UINT i = 0;
    BYTE *pSamples = NULL;
    UINT uNumSamplesInDuration = 0;

    if( (StartTime + Duration) > m_ilAudioDuration )
    {
        VIS_STRM_FAIL_EXIT(E_INVALIDARG, "Invalid time parameters");
    }

    for ( ; i < m_AudioFormat.nChannels; i++ )
    {
        CVisAudioChannel* pAudioChan = m_pAudioChannels[i];

        // write data into the audio channel if necessary
        if( !pAudioChan->BeenWritten(StartTime, Duration) )
        {
            float* pDst = pAudioChan->StartWrite(StartTime, Duration, &uNumSamplesInDuration);

            if( !pSamples )
            {
                long iStartSample     = AudioSampleNumber( StartTime, m_AudioFormat.nSamplesPerSec );

                UINT cbBufferSize     = uNumSamplesInDuration * (m_AudioFormat.wBitsPerSample>>3) *
                                        m_AudioFormat.nChannels;
                // XXX mattu failure
                pSamples = new BYTE[cbBufferSize];
                memset( pSamples, 0xf0, cbBufferSize ); 

                long SamplesRead, BytesRead;

                VIS_STRM_GTHR( AVIStreamRead( m_paviAudioStream, iStartSample, uNumSamplesInDuration,
                                              pSamples, cbBufferSize, &BytesRead, &SamplesRead ),
                               "Read of audio stream failed" ); 
            }

            BYTE*  pSrc = pSamples;

            if( m_AudioFormat.wBitsPerSample == 8*sizeof(BYTE) )
            {
                for( UINT j=0; j<uNumSamplesInDuration; j++ )
                {
                    *pDst++ = float( ((BYTE*)pSrc)[i] << 8 );
                    pSrc   += m_AudioFormat.nChannels * sizeof(BYTE);
                }
            }
            else
            {
                assert( m_AudioFormat.wBitsPerSample == 8*sizeof(SHORT) );

                for( UINT j=0; j<uNumSamplesInDuration; j++ )
                {
                    *pDst++ = float( ((SHORT*)pSrc)[i] );
                    pSrc   += m_AudioFormat.nChannels * sizeof(SHORT);
                }
            }

            pAudioChan->EndWrite(StartTime, Duration);
        }
        else
        {
            // either all or none of the audio channels should have been written
            assert( pSamples==NULL );
        }

        // set the audio channel in the CVisAudioData class
        if( !aud.InitializeChannel( i, pAudioChan, StartTime, Duration ) )
        {
            VIS_STRM_FAIL_EXIT(E_FAIL, "InitializeChannel failed");
        }
    }

    if( pSamples )
        delete [] pSamples;

    return TRUE;

Exit:
    if( pSamples )
        delete [] pSamples;

    return HandleError();
}

BOOL
CVisAviStreamHandler::WriteAudio( CVisAudioData& aud,  INT64 StartTime )
{
#define DEF_AUDIO_SIZE 16
    UINT cbBufferSize = 0;
    BYTE *pSamples = NULL;
    UINT uNumSamplesInDuration = 0;
    UINT i = 0;

    if ( m_bFirstAudioWrite )
    {
        AVISTREAMINFO      m_strhdrAudio;
        ZeroMemory(&m_strhdrAudio,sizeof(m_strhdrAudio));
        m_strhdrAudio.fccType      = streamtypeAUDIO;
        m_strhdrAudio.dwScale      = 1;
        m_strhdrAudio.dwRate       = aud.SampleRate();
        m_strhdrAudio.dwSampleSize = aud.NumChannels() * (DEF_AUDIO_SIZE/8);
        m_strhdrAudio.dwQuality  = (unsigned long)-1;
        VIS_STRM_GTHR( AVIFileCreateStream( m_paviFile, &m_paviAudioStream, &m_strhdrAudio ),
                        "Can't create avi audio stream" );

        m_AudioFormat.wFormatTag      = WAVE_FORMAT_PCM;
        m_AudioFormat.nChannels       = aud.NumChannels();
        m_AudioFormat.nBlockAlign     = aud.NumChannels() * (DEF_AUDIO_SIZE/8);
        m_AudioFormat.nAvgBytesPerSec = aud.SampleRate() *  m_AudioFormat.nBlockAlign;
        m_AudioFormat.nSamplesPerSec  = aud.SampleRate();
        m_AudioFormat.wBitsPerSample  = DEF_AUDIO_SIZE;
        m_AudioFormat.cbSize = 0;

        VIS_STRM_GTHR( AVIStreamSetFormat( m_paviAudioStream, 0, &m_AudioFormat, sizeof(m_AudioFormat) ),
                        "Can't set avi stream format" );

        if( !OpenAudioStream( ) )
        {
            return FALSE;
        }

        m_bFirstAudioWrite = FALSE;
    }
    else
    {
        // check that all calls are consistent
        if( m_AudioFormat.nChannels != aud.NumChannels() )
        {
            VIS_STRM_FAIL_EXIT(E_INVALIDARG,
                               "Number of audio channels must match for all calls to WriteAudio\n");
        }
        
        if( m_AudioFormat.nSamplesPerSec != aud.SampleRate() )
        {
            VIS_STRM_FAIL_EXIT(E_INVALIDARG,
                               "SampleRate must match for all calls to WriteAudio\n");
        }
    }

    if( StartTime + aud.Duration() > m_ilAudioDuration )
    {
        m_ilAudioDuration = StartTime + aud.Duration(); 
    }
    
    for ( ; i < m_AudioFormat.nChannels; i++ )
    {
        CVisAudioChannel* pAudioChan = aud.AudioChannel(i);

        const float* pSrc = pAudioChan->StartRead(StartTime, aud.Duration(), &uNumSamplesInDuration);

        if( pSamples == NULL )
        {
            cbBufferSize = uNumSamplesInDuration * (m_AudioFormat.wBitsPerSample>>3) *
                           m_AudioFormat.nChannels;

            pSamples = new BYTE[cbBufferSize];
            if ( pSamples == NULL )
            {
                VIS_STRM_FAIL_EXIT(E_OUTOFMEMORY, "Out of memeory");
            }
             memset( pSamples, 0xf0, cbBufferSize );
        }

        BYTE*  pDst = pSamples;

        if( m_AudioFormat.wBitsPerSample == 8*sizeof(BYTE) )
        {
            for( UINT j=0; j<uNumSamplesInDuration; j++ )
            {
                // convert from float to 8bit and saturate if necessary
                int res = int(*pSrc+0.5);
                res = (res + (1<<7) ) >> 8;
                if( res < -128 )     res = -128;
                else if( res > 127 ) res = 127;

                ((BYTE*)pDst)[i] = (BYTE)res;
                pDst   += m_AudioFormat.nChannels * sizeof(BYTE);
                pSrc++;
            }
        }
        else
        {
            assert( m_AudioFormat.wBitsPerSample == 8*sizeof(SHORT) );

            for( UINT j=0; j<uNumSamplesInDuration; j++ )
            {
                // convert from float to 16bit and saturate if necessary
                int res = int(*pSrc+0.5);
                if( res < -32768 )     res = -32768;
                else if( res > 32767 ) res = 32767;

                ((SHORT*)pDst)[i] = (SHORT)res;
                pDst   += m_AudioFormat.nChannels * sizeof(SHORT);
                pSrc++;
            }
        }

        pAudioChan->EndRead(StartTime, aud.Duration());
    }

    if( pSamples )
    {
        long iStartSample = AudioSampleNumber( StartTime, m_AudioFormat.nSamplesPerSec );

        long SamplesWritten, BytesWritten;

        VIS_STRM_GTHR( AVIStreamWrite(m_paviAudioStream, iStartSample, uNumSamplesInDuration,
                                      pSamples, cbBufferSize, AVIIF_KEYFRAME, &SamplesWritten, &BytesWritten),
                       "AviStreamWrite failed" );

        delete [] pSamples;
    }

    return TRUE;

Exit:
    if( pSamples )
        delete [] pSamples;

    return HandleError();
}

void
CVisAviStreamHandler::Close()
{   
    if ( m_pzFileName )
    {
        free( (void*)m_pzFileName );
        m_pzFileName = NULL;
    }

    m_uMode       = 0;
    m_uFrameCount = 0;
    m_iHeight     = 0;
    m_uWidth      = 0;
    m_ePixFmt     = evispixfmtNil;
    m_uPixelByteCount = 0;

    m_ilAudioDuration = 0;

    if ( m_pData )
    {
        delete [] m_pData;
        m_pData = NULL;
    }
    m_uDataSize = 0;

    if( m_pAudioChannels.size() )
    {
        T_AudioChanVec::iterator iteratorV    = m_pAudioChannels.begin();
        T_AudioChanVec::iterator iteratorVLim = m_pAudioChannels.end();
        for ( ; iteratorV != iteratorVLim; ++iteratorV )
        {
            (*iteratorV)->Release();
        }

        m_pAudioChannels.clear();
    }


    if ( m_pgfFrame )
    {
        AVIStreamGetFrameClose( m_pgfFrame );
        m_pgfFrame = NULL;
    }

    if ( m_paviAudioStream )
    {
        AVIStreamRelease( m_paviAudioStream );
        m_paviAudioStream = NULL;
    }

    if ( m_paviCVideoStream )
    {
        AVIStreamRelease( m_paviCVideoStream );
        m_paviCVideoStream = NULL;
    }

    if ( m_paviVideoStream )
    {
        AVIStreamRelease( m_paviVideoStream );
        m_paviVideoStream = NULL;
    }

    if ( m_paviFile )
    {
        AVIFileClose(m_paviFile);
        m_paviFile = NULL;
    }
}

HRESULT
CVisAviStreamHandler::GetLastError()
{
    return GetError();
}

const char* CVisAviStreamHandler::FileName(void) const
{
    return m_pzFileName;
}

UINT CVisAviStreamHandler::Mode(void) const
{
    return m_uMode;
}

int CVisAviStreamHandler::FrameCount(void) const
{
    return m_uFrameCount;
}

INT64 CVisAviStreamHandler::FrameDuration(void) const
{
    return (10000000i64 * (INT64)m_strhdrVideo.dwScale + ((INT64)m_strhdrVideo.dwRate/2)) / 
        (INT64)m_strhdrVideo.dwRate;

}

INT64 CVisAviStreamHandler::TimeFromFrameCount( int count ) const
{
    return ((INT64)count * 10000000i64 * (INT64)m_strhdrVideo.dwScale + ((INT64)m_strhdrVideo.dwRate/2)) / 
        (INT64)m_strhdrVideo.dwRate;

}

UINT CVisAviStreamHandler::Width(void) const
{
    return m_uWidth;
}

int CVisAviStreamHandler::Height(void) const
{
    return m_iHeight;
}

EVisPixFmt CVisAviStreamHandler::PixFmt(void) const
{
    return m_ePixFmt;
}

UINT
CVisAviStreamHandler::AudioSampleRate(void) const
{
    return m_AudioFormat.nSamplesPerSec;
}

UINT
CVisAviStreamHandler::AudioChannelCount(void) const
{
    return m_AudioFormat.nChannels;
}

UINT
CVisAviStreamHandler::AudioDuration(void) const
{
    return m_ilAudioDuration;
}

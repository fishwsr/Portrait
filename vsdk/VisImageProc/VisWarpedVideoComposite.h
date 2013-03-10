// VisWarpedVideoComposite.h
//
// Copyright © 1999 - 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////

template <class TPixel>
class CVisWarpedVideoComposite : public CVisWarpedComposite<TPixel>
{
public:
	// Constructor and destructor
    CVisWarpedVideoComposite(EVisSequence evissequence = 
                                      evissequenceDelayReadUntilNeeded|
                                      evissequenceLimitMemoryUsage);
    virtual ~CVisWarpedVideoComposite();

    // opens stream and associates it with composite at pos idx
    BOOL Open( const char* strmname, int idx = -1 );
    
    // advances all open streams by step
    BOOL FrameAdvance( int step = 1 );

    // set frame number to idx use -1 for last
    BOOL FrameSet( UINT idx );
                                                        
    // returns the current frame number
    UINT FrameCur();

    // returns the number of frames in the longest
    // contained stream
    UINT FrameCount();

protected:
    
    UINT m_uCur;
    UINT m_uFrameCount;
};

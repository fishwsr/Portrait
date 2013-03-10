// VisAudioOperation.cpp
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved

#include "VisCorePch.h"
#include "VisCoreProj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////////////
//
//  Class CVisAudioOperation
//
/////////////////////////////////////////////////////////////////////////////////////

CVisAudioOperation::CVisAudioOperation()
{
}

CVisAudioOperation::~CVisAudioOperation()
{
    if( m_AudioOperands.size() )
    {
        T_AudioOpVec::iterator iteratorV    = m_AudioOperands.begin();
        T_AudioOpVec::iterator iteratorVLim = m_AudioOperands.end();
        for ( ; iteratorV != iteratorVLim; ++iteratorV )
        {
            if( iteratorV->m_pAudioSamples )
                delete iteratorV->m_pAudioSamples;
        }

		m_AudioOperands.clear();
    }
}

VisCoreExport UINT
CVisAudioOperation::NumChannels() const
{
    if( m_AudioOperands.size() == 0 )
        return 0;

    return m_AudioOperands[0].m_pAudioSamples->NumChannels();
}

VisCoreExport INT64
CVisAudioOperation::Duration() const
{
    if( m_AudioOperands.size() == 0 )
        return 0;

    return m_AudioOperands[0].m_pAudioSamples->Duration();
}
    
VisCoreExport UINT
CVisAudioOperation::SampleRate() const
{    
    if( m_AudioOperands.size() == 0 )
        return 0;

    return m_AudioOperands[0].m_pAudioSamples->SampleRate();
}
    
VisCoreExport UINT
CVisAudioOperation::NumOperands() const
{
    return m_AudioOperands.size();
}

VisCoreExport BOOL
CVisAudioOperation::AddOperand(const CVisAudioData& data, AUDIO_OPERATION op, float fWeight)
{
    // this operand must have same duration, same number of channels and
    // same sample rate as all others
    if( m_AudioOperands.size() )
    {
        if( Duration() != data.Duration() )
        {
            throw CVisError("All audio durations must be identical",
                    eviserrorUnexpected, "CVisAudioOperation::AddOperand",
                    __FILE__, __LINE__);
        }
        
        if( NumChannels() != data.NumChannels() )
        {
            throw CVisError("All audio channel counts must be identical",
                    eviserrorUnexpected, "CVisAudioOperation::AddOperand",
                    __FILE__, __LINE__);
        }
        
        if( SampleRate() != data.SampleRate() )
        {
            throw CVisError("All audio sample rates must be identical",
                            eviserrorUnexpected, "CVisAudioOperation::AddOperand",
                            __FILE__, __LINE__);
        }
    }
    
    Operand Op;
    Op.m_eOp              = op;
    Op.m_fWeight          = fWeight;
    Op.m_pAudioSamples    = new CVisAudioData();
    *(Op.m_pAudioSamples) = data;

    m_AudioOperands.push_back(Op);

    return TRUE;
}

VisCoreExport BOOL
CVisAudioOperation::Process(CVisAudioData& result, INT64 ilResultOffset, 
                            INT64 ilOpOffset,  INT64 ilDuration )
{
    if( m_AudioOperands.size() == 0 )
    {
        return FALSE;
    }

    UINT  uNumChannels = NumChannels();
    UINT  uSampleRate  = SampleRate();

    for( UINT i=0; i<uNumChannels; i++ )
    {
        // initialize the result channel if one doesn't exist already
        CVisAudioChannel* pAudioChan;
        if( pAudioChan = result.AudioChannel(i) )
        {
            if( uSampleRate != pAudioChan->SampleRate() )
            {
                throw CVisError("result sample rate must me the same as source",
                        eviserrorUnexpected, "CVisAudioOperation::Process",
                        __FILE__, __LINE__);
            }
        }
        else
        {
            pAudioChan = new CVisAudioChannel(uSampleRate);
            result.InitializeChannel( i, pAudioChan, 0, ilDuration);
            pAudioChan->Release();
        }

        // start editing the result channel
        UINT   uNumResSamples;
        float* pRes = result.StartWriteChannel(i, ilResultOffset, ilDuration, &uNumResSamples);

        // clear out the channel
        memset(pRes, 0, uNumResSamples*sizeof(float));

        T_AudioOpVec::iterator iteratorV    = m_AudioOperands.begin();
        T_AudioOpVec::iterator iteratorVLim = m_AudioOperands.end();
        for ( ; iteratorV != iteratorVLim; ++iteratorV )
        {
            // start editing the read channel
            UINT   uNumSrcSamples;
            const float* pSrc = iteratorV->m_pAudioSamples->StartReadChannel(i, ilOpOffset, ilDuration, 
				                                                             &uNumSrcSamples);

            assert( uNumResSamples==uNumSrcSamples );

            // perform the operation
			UINT j = 0;
            float fRampVal;
            float fRampInc;

            switch( iteratorV->m_eOp )
            {
            case AUDIO_CONST:
                for( ; j<uNumResSamples; j++ )
                {
                    pRes[j] += pSrc[j] * iteratorV->m_fWeight;
                }
                break;

            case AUDIO_RAMP_UP:
                if( uNumResSamples > 1 )
                {
                    fRampVal = 0.0;
                    fRampInc = (iteratorV->m_fWeight - 0.0) / (uNumResSamples-1);
                }
                else
                {
                    fRampVal = iteratorV->m_fWeight;
                }

                for( ; j<uNumResSamples; j++ )
                {
                    pRes[j]  += pSrc[j] * fRampVal;
                    fRampVal += fRampInc; 
                } 
  
                break;

            case AUDIO_RAMP_DOWN:
                fRampVal = iteratorV->m_fWeight;
                if( uNumResSamples > 1 )
                {
                    fRampInc = (0.0 - iteratorV->m_fWeight) / (uNumResSamples-1);
                }

                for( ; j<uNumResSamples; j++ )
                {
                    pRes[j]  += pSrc[j] * fRampVal;
                    fRampVal += fRampInc; 
                } 
                break;

            default:
                assert(0);
                break;

            }
        
            // done reading this channel
            iteratorV->m_pAudioSamples->EndReadChannel(i, ilOpOffset, ilDuration);
        }

        // done writing this channel
        result.EndWriteChannel(i, ilResultOffset, ilDuration);
    }

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  Class CVisAudioOperationSequence
//
/////////////////////////////////////////////////////////////////////////////////////

CVisAudioOperationSequence::CVisAudioOperationSequence()
{
    m_ilDuration = 0;
}

CVisAudioOperationSequence::~CVisAudioOperationSequence()
{
	if( m_AudioOperations.size() )
	{
        T_AudioOpVec::iterator iteratorV    = m_AudioOperations.begin();
	    T_AudioOpVec::iterator iteratorVLim = m_AudioOperations.end();
	    for ( ; iteratorV != iteratorVLim; ++iteratorV )
		{
			if( *iteratorV )
            {
				delete *iteratorV;
			}
		}
	}
}

VisCoreExport void
CVisAudioOperationSequence::Clear()
{
    if( m_AudioOperations.size() )
    {
        m_AudioOperations.clear();
    }
    m_ilDuration = 0;
}

VisCoreExport INT64
CVisAudioOperationSequence::Duration() const
{
    return m_ilDuration;
}

VisCoreExport void
CVisAudioOperationSequence::PushBack()
{
    CVisAudioOperation *pOp = new CVisAudioOperation();

    m_AudioOperations.push_back(pOp);
}

VisCoreExport BOOL
CVisAudioOperationSequence::AddOperandBack(const CVisAudioData& data, AUDIO_OPERATION op, float weight)
{
    if( m_AudioOperations.size() == 0 )
    {
        return FALSE;
    }
    else if( m_AudioOperations.size() > 1 )
    {
        if( m_AudioOperations[0]->NumChannels() != data.NumChannels() )
        {
            throw CVisError("All audio channel counts must be identical",
                    eviserrorUnexpected, "CVisAudioOperationSequence::AddOperandBack",
                    __FILE__, __LINE__);
        }
        
        if( m_AudioOperations[0]->SampleRate() != data.SampleRate() )
        {
            throw CVisError("All audio sample rates must be identical",
                            eviserrorUnexpected, "CVisAudioOperationSequence::AddOperandBack",
                            __FILE__, __LINE__);
        }
    }

    CVisAudioOperation *pOp = m_AudioOperations.back();
    if( pOp->NumOperands() == 0 )
    {
        m_ilDuration += data.Duration();
    }

    return pOp->AddOperand(data, op, weight);
}

VisCoreExport const CVisAudioOperation&
CVisAudioOperationSequence::operator[](int i) const
{
	assert(i >= 0);
	assert(i < m_AudioOperations.size());
    return *m_AudioOperations[i];
}

VisCoreExport CVisAudioOperation&
CVisAudioOperationSequence::operator[](int i)
{
	assert(i >= 0);
	assert(i < m_AudioOperations.size());
    return *m_AudioOperations[i];
}

VisCoreExport BOOL
CVisAudioOperationSequence::Process(CVisAudioData& result, INT64 ilStartTime, INT64 ilDuration)
{
    if( m_AudioOperations.size() == 0 )
    {
        return FALSE;
    }

    // verify that sufficient data exists for the request
    if( ilStartTime + ilDuration > m_ilDuration )
    {
        return FALSE;
    }

    UINT uNumChannels = m_AudioOperations[0]->NumChannels();
    UINT uSampleRate  = m_AudioOperations[0]->SampleRate();
    for( UINT i=0; i<uNumChannels; i++ )
    {
        // initialize the result channels
        CVisAudioChannel* pAudioChan = new CVisAudioChannel(uSampleRate);
        result.InitializeChannel( i, pAudioChan, 0, ilDuration);
        pAudioChan->Release();
	}

	// prcess the sequence
	INT64 ilTimeInVector = 0;
	INT64 ilOffsetResult = 0;
	T_AudioOpVec::iterator iteratorV    = m_AudioOperations.begin();
	T_AudioOpVec::iterator iteratorVLim = m_AudioOperations.end();
	for ( ; iteratorV != iteratorVLim; ++iteratorV )
	{
		INT64 ilDurationOp = (*iteratorV)->Duration(); 
		if( ilTimeInVector + ilDurationOp < ilStartTime )
		{
			ilTimeInVector += ilDurationOp;
			continue;
		}
		
		UINT ilStartOffsetOp = 0;
		UINT ilDurationRes   = ilDurationOp;
		if( ilTimeInVector < ilStartTime )
		{
			INT64 offset = ilStartTime - ilTimeInVector;
			ilStartOffsetOp += offset;
			ilDurationRes   -= offset;
		}
		
		(*iteratorV)->Process(result, ilOffsetResult, ilStartOffsetOp, ilDurationRes);
		
		ilTimeInVector += ilDurationOp;
		ilOffsetResult += ilDurationRes;
	}
	
    return TRUE;
}

VisCoreExport BOOL
CVisAudioOperationSequence::ProcessAndWrite(CVisStreamHandler* pStream, INT64 ilStartTime, INT64 ilDuration,
                                            INT64 ilStartInStream)
{
    CVisAudioData audio;

    if( !Process( audio, ilStartTime, ilDuration ) )
        return FALSE;

    return audio.WriteToStream(pStream, ilStartInStream );
}

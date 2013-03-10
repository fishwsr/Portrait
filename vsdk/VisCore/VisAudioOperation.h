// VisAudioSequence.h
//
// Copyright  2000 Microsoft Corporation, All Rights Reserved


#ifndef __VIS_CORE_AUDIO_SEQUENCE_H__
#define __VIS_CORE_AUDIO_SEQUENCE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include <vector>

enum AUDIO_OPERATION 
{
   AUDIO_CONST     = 0,
   AUDIO_RAMP_UP   = 1,
   AUDIO_RAMP_DOWN = 2
};

class CVisAudioOperation
{
public:
    VisCoreExport CVisAudioOperation();
    VisCoreExport ~CVisAudioOperation();

    VisCoreExport UINT  NumChannels() const;
    VisCoreExport INT64 Duration()    const;
    VisCoreExport UINT  SampleRate()  const;
    VisCoreExport UINT  NumOperands() const;

    VisCoreExport BOOL  AddOperand(const CVisAudioData& data, AUDIO_OPERATION op, float fWeight);
    VisCoreExport BOOL  Process(CVisAudioData& result, INT64 ilResultOffset, 
                                INT64 ilOpOffset,  INT64 ilDuration );

private:
    typedef struct _Operand {
        AUDIO_OPERATION  m_eOp;
        float m_fWeight;
        CVisAudioData* m_pAudioSamples;
    } Operand;
    typedef std::vector<Operand> T_AudioOpVec;

    T_AudioOpVec m_AudioOperands;
};


class CVisAudioOperationSequence
{
public:
    VisCoreExport CVisAudioOperationSequence();
    VisCoreExport ~CVisAudioOperationSequence();
    
    VisCoreExport void  Clear(void);
    VisCoreExport INT64 Duration()  const;

	VisCoreExport void PushBack();
	//VisCoreExport void PushBack(const CVisAudioOperation& );
	VisCoreExport BOOL AddOperandBack(const CVisAudioData&, AUDIO_OPERATION op, float weight );

	VisCoreExport const CVisAudioOperation& operator[](int i) const;
	VisCoreExport CVisAudioOperation& operator[](int i);

    //VisCoreExport BOOL Compact(void);
    VisCoreExport BOOL Process(CVisAudioData& result, INT64 ilStartTime, INT64 ilDuration);
    VisCoreExport BOOL ProcessAndWrite(CVisStreamHandler* pStream, INT64 ilStartTime, INT64 ilDuration,
                                       INT64 ilStartInStream);

private:
    typedef std::vector<CVisAudioOperation*> T_AudioOpVec;
   
    T_AudioOpVec m_AudioOperations;
    INT64        m_ilDuration;
};

#endif // __VIS_CORE_AUDIO_SEQUENCE_H__


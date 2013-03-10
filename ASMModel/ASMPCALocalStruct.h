
// ASMPCALocalStruct.h: interface for the CASMPCALocalStruct class.
//
//////////////////////////////////////////////////////////////////////


#ifndef __ASMMODEL_PCALOCALSTRUCT_H__
#define __ASMMODEL_PCALOCALSTRUCT_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "ASMLocalProfile.h"

class CASMPCALocalStructBuilder;
class ASMMODEL_API CASMPCALocalStruct
{
public:
    CASMPCALocalStruct(void);
    virtual ~CASMPCALocalStruct(void);
	friend class CASMPCALocalStructBuilder;
	friend COutputDataFile& Output(COutputDataFile& ifs, const CASMPCALocalStruct& ls);
	friend CInputDataFile& Input(CInputDataFile& ifs, CASMPCALocalStruct& ls);

public:

    double EvaluateLessThan(CASMLocalProfile& srcprofile, double MinEnergy);
	double LogP(CASMLocalProfile& srcprofile, double MaxLogP);

protected:
    // unchanged data
    CPCA2Model m_PCA2Model;
    int m_TotLength;
};

ASMMODEL_API COutputDataFile& operator<<(COutputDataFile& fs, const CASMPCALocalStruct& ls);
ASMMODEL_API CInputDataFile& operator>>(CInputDataFile& fs, CASMPCALocalStruct& ls);

#endif // __ASMMODEL_PCALOCALSTRUCT_H__

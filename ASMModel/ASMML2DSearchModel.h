
// ASMML2DSearchModel.h: interface for the CASMML2DSearchModel class.
//
//////////////////////////////////////////////////////////////////////


#ifndef __ASMMODEL_ML2DSEARCHMODEL_H__
#define __ASMMODEL_ML2DSEARCHMODEL_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "ASMLocalProfile.h"
#include "ASMMLSearchModel.h"

class CASMModelBuilder;

class ASMMODEL_API CASMML2DSearchModel : public CASMMLSearchModel
{
public:
    CASMML2DSearchModel(void);
    virtual ~CASMML2DSearchModel(void);
	friend class CASMModelBuilder;
	friend COutputDataFile& Output(COutputDataFile& ifs, const CASMML2DSearchModel& model);
	friend CInputDataFile& Input(CInputDataFile& ifs, CASMML2DSearchModel& model);

    virtual bool SearchOnePoint(int iPoint, double& desx, double& desy);

protected:
    void Get2DDetProfile(int iPoint, CASMLocalProfile& longprofile, CASMLocalProfile& profile, int detx, int dety);
    void Get2DLongProfile(int iPoint, CASMLocalProfile &profile);

protected:
    std::vector<int> m_vUse2DLocalStruct;
    std::vector<int> m_vUse2DSearch;
    int m_iLocalStructWidth;
    int m_iSearchWidth;
    int m_iUse2DStructNum;
    int m_iUse2DSearchNum;
};

ASMMODEL_API COutputDataFile& operator<<(COutputDataFile& fs, const CASMML2DSearchModel& model);
ASMMODEL_API CInputDataFile& operator>>(CInputDataFile& fs, CASMML2DSearchModel& model);

#endif // __ASMMODEL_ML2DSEARCHMODEL_H__

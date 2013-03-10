
// ASMLocalProfile.h: interface for the CASMLocalProfile class.
//
//////////////////////////////////////////////////////////////////////


#ifndef __ASMMODEL_LOCALPROFILE_H__
#define __ASMMODEL_LOCALPROFILE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class ASMMODEL_API CASMLocalProfile : public CVisDVector
{
public:
    CASMLocalProfile(void);
    virtual ~CASMLocalProfile(void);

public:
    void Normalize(void);
};


#endif // __ASMMODEL_LOCALPROFILE_H__


#ifndef __CSBASE_INITCSBASE_H__
#define __CSBASE_INITCSBASE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CSBASE_API CInitCSBase
{
public:
    CInitCSBase(void);
	~CInitCSBase(void);

    bool Init(void);
    void DeInit(void);
};


CSBASE_API CInitCSBase *PInitCSBase(void);


#endif __CSBASE_INITCSBASE_H__

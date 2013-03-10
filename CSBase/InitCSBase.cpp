#include "stdafx.h"
#include "InitCSBase.h"


CInitCSBase::CInitCSBase(void)
{
}

CInitCSBase::~CInitCSBase(void)
{
}

bool CInitCSBase::Init(void)
{
 //   if (!CSketchVIR::Init(L"..\\data\\strokectrl.txt", _O_TEXT))
 //   {
 //       return( false );
 //   }

	return( true );
}

void CInitCSBase::DeInit(void)
{
//    CSketchVIR::DeInit();
}


static CInitCSBase    s_InitCSBase;


CInitCSBase *PInitCSBase(void)
{
    return &s_InitCSBase;
}

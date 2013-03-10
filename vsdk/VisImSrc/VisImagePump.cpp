// VisImagePump.cpp
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#include "VisImSrcProj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/*
VisImSrcExport CVisImagePumpBase::CVisImagePumpBase(void)
  : m_critsect()
{
	m_pThread = AfxBeginThread(StaticThreadMain, this, THREAD_PRIORITY_NORMAL,
			0, CREATE_SUSPENDED);
}


VisImSrcExport UINT __cdecl CVisImagePumpBase::StaticThreadMain(void *pvImagePump)
{
	assert(pvImagePump != 0);
	return ((CVisImagePumpBase *) pvImagePump)->ThreadMain();
}
*/
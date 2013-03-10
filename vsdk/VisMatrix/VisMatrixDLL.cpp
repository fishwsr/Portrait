///////////////////////////////////////////////////////////////////////////
// VisMatrixDLL.cpp
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved


#include "VisMatrixPch.h"
#include "VisMatrixProj.h"

#ifdef __AFX_H__  // (__AFX_H__ is defined when we're using MFC.)
	#include <afxdllx.h>
#endif // __AFX_H__  // (__AFX_H__ is defined when we're using MFC.)


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef __AFX_H__  // (__AFX_H__ is defined when we're using MFC.)
	static AFX_EXTENSION_MODULE VisMatrixDLL = { NULL, NULL };
#endif // __AFX_H__  // (__AFX_H__ is defined when we're using MFC.)


extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	VisUnreferenced(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		#ifdef _DEBUG
			VisTrace(1, "VisMatrix.dll Initializing!\n");
		#endif
#ifdef __AFX_H__  // (__AFX_H__ is defined when we're using MFC.)
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(VisMatrixDLL, hInstance))
			return 0;


		new CDynLinkLibrary(VisMatrixDLL);
#endif // __AFX_H__  // (__AFX_H__ is defined when we're using MFC.)

		
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		#ifdef _DEBUG
			VisTrace(1, "VisMatrix.dll Terminating!\n");
		#endif
#ifdef __AFX_H__  // (__AFX_H__ is defined when we're using MFC.)
		// Terminate the library before destructors are called
		AfxTermExtensionModule(VisMatrixDLL);
#endif // __AFX_H__  // (__AFX_H__ is defined when we're using MFC.)

	}
	return 1;   // ok
}


////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISOLEHELP
//
// @module VisOLEHelp.h |
//
// This file defines functions that can be used to implement parts of
// some common OLE methods.
//
// <nl>
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
////////////////////////////////////////////////////////////////////////////


#ifndef __VIS_IMSRC_VISOLEHELP_H__
#define __VIS_IMSRC_VISOLEHELP_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// LATER:  Do we want an INL file for this?
inline HRESULT VisGetSzPcbFromSz(THIS_ char *szOut, LONG *pcbOut,
		const char *szIn)
{
	HRESULT hrRet;

	assert(szIn != 0);
	int cb = lstrlen(szIn) + 1;

	if ((szOut == 0) && (pcbOut == 0))
	{
		hrRet = ResultFromScode(E_POINTER);
	}
	else if (szOut == 0)
	{
		hrRet = ResultFromScode(S_FALSE);
		*pcbOut = cb;
	}
	else if (pcbOut == 0)
	{
		hrRet = ResultFromScode(S_OK);
		lstrcpy(szOut, szIn);
	}
	else if (*pcbOut >= cb)
	{
		hrRet = ResultFromScode(S_OK);
		lstrcpy(szOut, szIn);
		*pcbOut = cb;
	}
	else
	{
		hrRet = ResultFromScode(E_FAIL);
		*pcbOut = 0;
	}

	return hrRet;
}


#endif // __VIS_IMSRC_VISOLEHELP_H__

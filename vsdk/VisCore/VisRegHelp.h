////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISREGHELP
//
// @module VisRegHelp.h |
//
// This file defines functions that can be used to work with the Windows
// registry.
//
// <nl>
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisRegHelp\.inl>
//
////////////////////////////////////////////////////////////////////////////


#ifndef __VIS_IMSRC_VISREGHELP_H__
#define __VIS_IMSRC_VISREGHELP_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// @const const char * | VIS_SZ_REG_ROOT |
// This string is the root of the VisSDK registry key
// (under HKEY_LOCAL_MACHINE) for this version of the SDK.
#ifndef VIS_SZ_REG_ROOT
	#ifdef _DEBUG
		#define VIS_SZ_REG_ROOT		"Software\\Microsoft\\VisSDK\\Debug\\1.0"
	#else // _DEBUG
		#define VIS_SZ_REG_ROOT		"Software\\Microsoft\\VisSDK\\1.0"
	#endif // _DEBUG
#endif // !VIS_SZ_REG_ROOT


inline bool VisFCreateRegKey(HKEY *phkey, const char *szKey,
		HKEY hkeyBase = HKEY_LOCAL_MACHINE);

inline bool VisFOpenRegKey(HKEY *phkey, const char *szKey,
		HKEY hkeyBase = HKEY_LOCAL_MACHINE);

inline bool VisFCloseRegKey(HKEY hkey);


inline bool VisFReadDwordFromReg(DWORD *pdw, HKEY hkey,
		const char *szValue = 0);

inline bool VisFReadIntFromReg(int *pi, HKEY hkey, const char *szValue = 0);

inline bool VisFReadFloatFromReg(float *pflt, HKEY hkey,
		const char *szValue = 0);

inline bool VisFReadBoolFromReg(bool *pf, HKEY hkey, const char *szValue = 0);

inline bool VisFReadSzFromReg(char *szBuf, unsigned long cbBuf, HKEY hkey,
		const char *szValue = 0);


inline bool VisFWriteDwordToReg(DWORD dw, HKEY hkey, const char *szValue = 0);

inline bool VisFWriteIntToReg(int i, HKEY hkey, const char *szValue = 0);

inline bool VisFWriteFloatToReg(float flt, HKEY hkey, const char *szValue = 0);

inline bool VisFWriteBoolToReg(bool f, HKEY hkey, const char *szValue = 0);

inline bool VisFWriteSzToReg(const char *sz, HKEY hkey,
		const char *szValue = 0);


#include "VisRegHelp.inl"


#endif // __VIS_IMSRC_VISREGHELP_H__

////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISDLGLISTSZ
//
// @module VisDlgListSz.h |
//
// This file defines a dialog that can be used to select a string from a
// list of strings.
//
// <nl>
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisDlgListSz\.cpp>
//
////////////////////////////////////////////////////////////////////////////


#ifndef __VIS_IMSRC_VISDLGLISTSZ_H__
#define __VIS_IMSRC_VISDLGLISTSZ_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


/////////////////////////////////////////////////////////////////////////////
// CVisDlgListSz dialog

class CVisDlgListSz
{
// Construction
public:
	CVisDlgListSz(LPCSTR szTitle, IVisListSz *pIVisListSz,
			HWND hWndParent = NULL, int iSelectionInitial = - 1);
	~CVisDlgListSz();

	int DoModal();

// Dialog Data

	HANDLE m_hDlgTemplate;
	LPDLGTEMPLATE m_lpDlgTemp;


	HWND m_hWnd;
	BOOL	m_fAlwaysAsk;
	BOOL	m_fSetDefault;

	inline int ISelection(void) const
	{
		return m_iSelection;
	}

	inline void SetISelection(int i)
	{
		m_iSelection = i;
	}

	inline void ShowCheckboxes(bool fAlwaysAsk = false,
			bool fSetDefault = false)
	{
		m_fShowCheckboxes = true;
		m_fAlwaysAsk = fAlwaysAsk;
		m_fSetDefault = fSetDefault;
	}

	void LoadDialogTemplate();

    virtual LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	HWND m_hWndParent;
    HWND m_hWndLB;

	std::string m_szTitle;
	IVisListSz *m_pIVisListSz;
	int m_iSelection;
	bool m_fShowCheckboxes;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.


#endif // __VIS_IMSRC_VISDLGLISTSZ_H__

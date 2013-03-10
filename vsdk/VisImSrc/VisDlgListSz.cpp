// VisDlgListSz.cpp : implementation file
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved

#include "VisImSrcPch.h"
#include "VisImSrcProj.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The actual callback proc for this dialog...

static CVisDlgListSz *pBindNext = NULL;

BOOL CALLBACK VisDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// get the class pointer from the EXTRA words in the HWND structure...
    CVisDlgListSz *pT = (CVisDlgListSz *) GetWindowLong(hDlg, GWL_USERDATA);

    if(!pT && pBindNext) {  
        SetWindowLong(hDlg, GWL_USERDATA, (long)pBindNext);
        pBindNext->m_hWnd = hDlg;
        pT = pBindNext;
        pBindNext = NULL;
    }
    
    // do a secondary check to make sure that the pwndT->m_hWnd is
    // the same as the hWnd passed in!

    BOOL fSetLong;
    if(pT) {
		fSetLong = pT && uMsg != WM_NCDESTROY;
	}
	
	LRESULT lRes = FALSE;
    if(pT && pT->m_hWnd == hDlg)
        lRes = pT->WindowProc(uMsg, wParam, lParam);
    else // if all else fails, go to DefWindowProc!
        lRes = DefWindowProc(hDlg, uMsg, wParam, lParam);

    if(fSetLong)
	    SetWindowLong(hDlg, DWL_MSGRESULT, lRes);
	return lRes;
}

/////////////////////////////////////////////////////////////////////////////
// CVisDlgListSz dialog

CVisDlgListSz::CVisDlgListSz(LPCSTR szTitle, IVisListSz *pIVisListSz,
		HWND hWndParent, int iSelectionInitial)
	: m_pIVisListSz(pIVisListSz),
	m_szTitle(),
	m_iSelection(- 1),
	m_fShowCheckboxes(false)
{
	m_hWndParent = hWndParent;
	m_szTitle = szTitle;
	if (m_pIVisListSz != 0)
		m_pIVisListSz->AddRef();

	// LATER:  We might want a sorted listbox and a mapping from selected
	// strings to IVisListSz indices.

	m_fAlwaysAsk = FALSE;
	m_fSetDefault = FALSE;
	
	LoadDialogTemplate();

}


void CVisDlgListSz::LoadDialogTemplate()
{
	std::string szOK("OK");
	std::string szCancel("Cancel");
	std::string szDefault("&Set as\ndefault");
	std::string szAlways("&Always\nask");

	LONG lUnits = GetDialogBaseUnits();

	DWORD dwTotalSize = (m_szTitle.size() + 1) * 2 + 5 * (sizeof(DLGITEMTEMPLATE) + 6) +
						(szOK.size() + 1) * 2 +
						(szCancel.size() + 1) * 2  +
						(szDefault.size() + 1) * 2 +
						(szAlways.size() + 1) * 2;
	m_hDlgTemplate = HeapCreate(HEAP_GENERATE_EXCEPTIONS, 
									sizeof(DLGTEMPLATE) + dwTotalSize, 0L);
	LPDLGTEMPLATE lpDlgTemp = (LPDLGTEMPLATE)HeapAlloc(m_hDlgTemplate, 
								HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY, 
								sizeof(DLGTEMPLATE) + dwTotalSize);
	m_lpDlgTemp = lpDlgTemp;
	BYTE* pbPos = (BYTE *) lpDlgTemp;
	lpDlgTemp->style = DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU;
	lpDlgTemp->cdit = 5;
	lpDlgTemp->x = 0;
	lpDlgTemp->y = 0;
	lpDlgTemp->cx = 520 * 3/4;
	lpDlgTemp->cy = 220 * 3/4;
	pbPos += sizeof(DLGTEMPLATE);
	WORD* wPos = (WORD *)pbPos;
	*wPos++ = 0;
	*wPos++ = 0;
	MultiByteToWideChar(CP_ACP,
						MB_PRECOMPOSED,
						(LPCTSTR)(LPCSTR)m_szTitle.c_str(),
						-1,
						wPos,
						m_szTitle.size() + 1);

	
	wPos += (m_szTitle.size() + 2);
	
	LPDLGITEMTEMPLATE lpDlgItem = (LPDLGITEMTEMPLATE)(wPos);
	lpDlgItem->id = IDC_LIST_SZ_LIST;
	lpDlgItem->style = LBS_NOINTEGRALHEIGHT | WS_BORDER | WS_VISIBLE | 
						LBS_NOTIFY | WS_VSCROLL | WS_HSCROLL | WS_TABSTOP | WS_CHILD;
	lpDlgItem->x = 7 * 3/4;
	lpDlgItem->y = 7 * 3/4;
	lpDlgItem->cx = 449 * 3/4;
	lpDlgItem->cy = 206 * 3/4;
	lpDlgItem++;

	wPos = (WORD *)lpDlgItem;
	*wPos++ = 0xFFFF;
	*wPos++ = 0x0083;

	pbPos = ((BYTE *)wPos) + 6;
	lpDlgItem = (LPDLGITEMTEMPLATE)(pbPos);
	lpDlgItem->id = IDOK;
	lpDlgItem->style = BS_DEFPUSHBUTTON | WS_TABSTOP | WS_VISIBLE | WS_CHILD;
	lpDlgItem->x = 463 * 3/4;
	lpDlgItem->y = 7 * 3/4;
	lpDlgItem->cx = 50 * 3/4;
	lpDlgItem->cy = 14 * 3/4;
	lpDlgItem++;

	wPos = (WORD *)lpDlgItem;
	*wPos++ = 0xFFFF;
	*wPos++ = 0x0080;
	MultiByteToWideChar(CP_ACP,
						MB_PRECOMPOSED,
						(LPCTSTR)(LPCSTR)szOK.c_str(),
						-1,
						wPos,
						szOK.size() + 1);
	
	wPos+= (szOK.size() + 2);

	pbPos = ((BYTE *)wPos) + 2;

	lpDlgItem = (LPDLGITEMTEMPLATE)(pbPos);
	lpDlgItem->id = IDCANCEL;
	lpDlgItem->style = BS_PUSHBUTTON | WS_TABSTOP | WS_VISIBLE | WS_CHILD;
	lpDlgItem->x = 463 * 3/4;
	lpDlgItem->y = 27 * 3/4;
	lpDlgItem->cx = 50 * 3/4;
	lpDlgItem->cy = 14 * 3/4;
	lpDlgItem++;

	wPos = (WORD *)lpDlgItem;
	*wPos++ = 0xFFFF;
	*wPos++ = 0x0080;
	MultiByteToWideChar(CP_ACP,
						MB_PRECOMPOSED,
						(LPCTSTR)(LPCSTR)szCancel.c_str(),
						-1,
						wPos,
						szCancel.size() + 1);
	
	wPos+= (szCancel.size() + 2);

	pbPos = ((BYTE *)wPos) + 2;

	lpDlgItem = (LPDLGITEMTEMPLATE)(pbPos);
	lpDlgItem->id = IDC_CHECK_SET_DEFAULT;
	lpDlgItem->style = BS_AUTOCHECKBOX | BS_MULTILINE | WS_TABSTOP | WS_CHILD;
	lpDlgItem->x = 463 * 3/4;
	lpDlgItem->y = 47 * 3/4;
	lpDlgItem->cx = 50 * 3/4;
	lpDlgItem->cy = 18 * 3/4;
	lpDlgItem++;

	wPos = (WORD *)lpDlgItem;
	*wPos++ = 0xFFFF;
	*wPos++ = 0x0080;
	MultiByteToWideChar(CP_ACP,
						MB_PRECOMPOSED,
						(LPCTSTR)(LPCSTR)szDefault.c_str(),
						-1,
						wPos,
						szDefault.size() + 1);
	
	wPos+= (szDefault.size() + 2);

	lpDlgItem = (LPDLGITEMTEMPLATE)(wPos);
	lpDlgItem->id = IDC_CHECK_ALWAYS_ASK;
	lpDlgItem->style = BS_AUTOCHECKBOX | BS_MULTILINE | WS_TABSTOP | WS_CHILD;
	lpDlgItem->x = 463 * 3/4;
	lpDlgItem->y = 71 * 3/4;
	lpDlgItem->cx = 50 * 3/4;
	lpDlgItem->cy = 18 * 3/4;
	lpDlgItem++;

	wPos = (WORD *)lpDlgItem;
	*wPos++ = 0xFFFF;
	*wPos++ = 0x0080;
	MultiByteToWideChar(CP_ACP,
						MB_PRECOMPOSED,
						(LPCTSTR)(LPCSTR)szAlways.c_str(),
						-1,
						wPos,
						szAlways.size() + 1);

}

CVisDlgListSz::~CVisDlgListSz()
{
	if (m_pIVisListSz != 0)
		m_pIVisListSz->Release();
		HeapDestroy(m_hDlgTemplate);
}

int CVisDlgListSz::DoModal()
{
	// well, this does everything!!!

    pBindNext = this;
	return DialogBoxIndirect(imsrcCurrentInstanceHandle, m_lpDlgTemp, m_hWndParent, 
			(DLGPROC) VisDlgProc);
}

BOOL ModifyStyle(HWND hWnd, DWORD dwRemove, DWORD dwAdd, UINT nFlags)
{
	assert(hWnd != NULL);
	DWORD dwStyle = ::GetWindowLong(hWnd, GWL_STYLE);
	DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
	if (dwStyle == dwNewStyle)
		return FALSE;

	::SetWindowLong(hWnd, GWL_STYLE, dwNewStyle);
	if (nFlags != 0)
	{
		::SetWindowPos(hWnd, NULL, 0, 0, 0, 0,
			SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
	}
	return TRUE;
}

LRESULT CVisDlgListSz::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	HFONT hFont;
/*
	int cSelect;
	int nColonIndex = 0;
*/
	switch( uMsg ) {

    case WM_COMMAND:
        switch( LOWORD(wParam) ) {

        case IDOK: 
            m_iSelection = (UINT) SendMessage( m_hWndLB, LB_GETCURSEL,
                    (WPARAM) NULL, (LPARAM) NULL );

			assert((m_iSelection >= -1)
					&& (m_iSelection < m_pIVisListSz->CszGet()));

/*
			cSelect = SendMessage( m_hWndLB, LB_GETTEXTLEN, (WPARAM)m_iSelection, 0);

			m_szSelection.resize(cSelect + 1);
			SendMessage( m_hWndLB, LB_GETTEXT, 
						(WPARAM)m_iSelection, (LPARAM)m_szSelection.c_str());
			nColonIndex = m_szSelection.find(':');
            if(nColonIndex >= 0)
				m_szSelection.erase(nColonIndex, m_szSelection.size() - nColonIndex);
*/
			EndDialog( m_hWnd, TRUE );
            return( TRUE );

        case IDCANCEL:
            EndDialog( m_hWnd, FALSE );
            return( TRUE );
        }
        
		// specirfic controls area...
		switch( HIWORD( wParam ) ) {
        case LBN_DBLCLK:
            SendMessage( m_hWnd, WM_COMMAND, (WPARAM) IDOK, (LPARAM) 0L );
            return( TRUE );
        }
        break;

	case WM_INITDIALOG:
		hFont = CreateFont(8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, VARIABLE_PITCH | FF_SWISS, "MS Sans Serif");


		if (m_fShowCheckboxes)
		{
			// Enable the checkboxes.
			ModifyStyle(GetDlgItem(m_hWnd, IDC_CHECK_SET_DEFAULT), 0, WS_VISIBLE, 0);
			SendMessage( GetDlgItem(m_hWnd, IDC_CHECK_SET_DEFAULT), WM_SETFONT, (UINT)hFont, MAKELONG (TRUE, 0));
			ModifyStyle(GetDlgItem(m_hWnd, IDC_CHECK_ALWAYS_ASK), 0, WS_VISIBLE, 0);
			SendMessage( GetDlgItem(m_hWnd, IDC_CHECK_ALWAYS_ASK), WM_SETFONT, (UINT)hFont, MAKELONG (TRUE, 0));
		}

		// Set the correct title.
		SetWindowText(m_hWnd, m_szTitle.c_str());

		// Set listbox strings.
		assert(m_pIVisListSz != 0);
		int csz = m_pIVisListSz->CszGet();

        m_hWndLB = GetDlgItem(m_hWnd, IDC_LIST_SZ_LIST);
		if(m_hWndLB) {
			for (int isz = 0; isz < csz; ++isz)
			{
				LPCSTR sz = m_pIVisListSz->SzGetI(isz);
				assert(sz != 0);
				SendMessage(m_hWndLB, LB_ADDSTRING, 0, (LPARAM) (LPCTSTR) sz);
			}

			assert(csz > 0);
			if ((m_iSelection < 0) || (m_iSelection >= csz))
				m_iSelection = 0;
			SendMessage(m_hWndLB, LB_SETCURSEL, (WPARAM)m_iSelection, 0L);

			// Unless the dialog is dismissed with the OK button, we don't want
			// to report that any item was selected.
			m_iSelection = - 1;
		}
		SendMessage( GetDlgItem(m_hWnd, IDC_LIST_SZ_LIST), WM_SETFONT, (UINT)hFont, MAKELONG (TRUE, 0));
		SendMessage( GetDlgItem(m_hWnd, IDOK), WM_SETFONT, (UINT)hFont, MAKELONG (TRUE, 0));
		SendMessage( GetDlgItem(m_hWnd, IDCANCEL), WM_SETFONT, (UINT)hFont, MAKELONG (TRUE, 0));

		return TRUE;  // return TRUE unless you set the focus to a control
					  // EXCEPTION: OCX Property Pages should return FALSE
    }

    return( FALSE );
    UNREFERENCED_PARAMETER( lParam );
}


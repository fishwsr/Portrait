#if !defined(AFX_CROSSSHOW_H__6416C528_4B56_404C_8D48_63A476C2295C__INCLUDED_)
#define AFX_CROSSSHOW_H__6416C528_4B56_404C_8D48_63A476C2295C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// crossShow.h : header file
//
#include "Portrait EditingDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CcrossShow dialog

class CcrossShow : public CDialog
{
// Construction
public:
	CcrossShow(CWnd* pParent = NULL);   // standard constructor

	CvvImage crossImg;
	int offx;
	int offy;

	int SCROLL_MIN;
	int SCROLL_MAX;
	int SCROLL_MAXH;
// Dialog Data
	//{{AFX_DATA(CcrossShow)
	enum { IDD = IDD_CROSSSHOW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CcrossShow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CcrossShow)
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CROSSSHOW_H__6416C528_4B56_404C_8D48_63A476C2295C__INCLUDED_)

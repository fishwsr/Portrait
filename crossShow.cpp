// crossShow.cpp : implementation file
//

#include "stdafx.h"
#include "Portrait Editing.h"
#include "crossShow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CcrossShow dialog


CcrossShow::CcrossShow(CWnd* pParent /*=NULL*/)
	: CDialog(CcrossShow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CcrossShow)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	offx = 0;
	offy = 0;
}


void CcrossShow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CcrossShow)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CcrossShow, CDialog)
	//{{AFX_MSG_MAP(CcrossShow)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CcrossShow message handlers

void CcrossShow::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CWnd *pWnd = GetDlgItem(IDC_CROSS_IMG);
	CDC *pCROSSDC = pWnd->GetDC();
	pCROSSDC->SetBkMode(TRANSPARENT);

	CRect rect;
	pWnd->GetClientRect(&rect);
	rect.top += 8;

	SCROLL_MIN = 0;
	SCROLL_MAX = crossImg.Width()  - rect.Width();
	SCROLL_MAXH = crossImg.Height() - rect.Height();
	SetScrollRange(SB_HORZ, SCROLL_MIN, SCROLL_MAX );
	SetScrollRange(SB_VERT, SCROLL_MIN, SCROLL_MAXH );

	CvvImage showImg;
	showImg.Create(rect.Width(), rect.Height(), 24);
	BYTE *buf1 = (BYTE *)showImg.GetImage()->imageData;
	int WidthPix1 = WIDTHBYTES(showImg.Width() * 24);
	BYTE *buf2 = (BYTE *)crossImg.GetImage()->imageData;
	int WidthPix2 = WIDTHBYTES(crossImg.Width() * 24);
	for(int i = 0; i < showImg.Height(); i++)
	{
		for(int j = 0; j < showImg.Width(); j++)
		{
			buf1[i * WidthPix1 + 3 * j] = buf2[(i + offy) * WidthPix2 + 3 * (j + offx)];
			buf1[i * WidthPix1 + 3 * j + 1] = buf2[(i + offy) * WidthPix2 + 3 * (j + offx) + 1];
			buf1[i * WidthPix1 + 3 * j + 2] = buf2[(i + offy) * WidthPix2 + 3 * (j + offx) + 2];
		}
	}

	showImg.DrawToHDC(pCROSSDC->m_hDC, rect);
	//CDialog::OnPaint();
	// Do not call CDialog::OnPaint() for painting messages
}

void CcrossShow::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nSBCode)     //滚动条的通知消息码
	{
		case SB_THUMBTRACK: //滚动框被拖动
		{
		  int curPos=GetScrollPos(SB_HORZ);
		  SetScrollPos(SB_HORZ,nPos);
		  offx = nPos;
		}
		break;

		case SB_LINERIGHT:   //向右滚动一行（列）
		offx=GetScrollPos(SB_HORZ)+1;
		SetScrollPos(SB_HORZ,offx);
		if(offx>SCROLL_MAX)
		  offx=SCROLL_MAX;
		break;

		case SB_LINELEFT: //向左滚动一行（列）．   
		offx=GetScrollPos(SB_HORZ)-1;
		SetScrollPos(SB_HORZ,offx);
		if(offx<SCROLL_MIN)
		  offx=SCROLL_MIN;
		break;

        default:
		break;
	}

	Invalidate(FALSE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CcrossShow::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nSBCode)     //滚动条的通知消息码
	{
		case SB_THUMBTRACK: //滚动框被拖动
		{
		  int curPos=GetScrollPos(SB_VERT);
		  SetScrollPos(SB_VERT,nPos);
		  offy = nPos;
		}
		break;

		case SB_LINEUP:   //向上滚动一行（列）
		offy=GetScrollPos(SB_VERT)-1;
		if(offy<SCROLL_MIN)
		  offy=SCROLL_MIN;
		SetScrollPos(SB_VERT,offy);
		break;

		case SB_LINEDOWN: //向下滚动一行（列）．   
		offy=GetScrollPos(SB_VERT)+1;
		if(offy>SCROLL_MAXH)
		  offy=SCROLL_MAXH;
		SetScrollPos(SB_VERT,offy);
		break;

        default:
		break;
	}

	Invalidate(FALSE);
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

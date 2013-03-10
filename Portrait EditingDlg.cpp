// Portrait EditingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Portrait Editing.h"
#include "Portrait EditingDlg.h"
#include "../XML/XmlFile.h"
#include "crossShow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortraitEditingDlg dialog

CPortraitEditingDlg::CPortraitEditingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPortraitEditingDlg::IDD, pParent), m_pModel(NULL), Gcs(NULL), m_fbInitShape(false)
{
	//{{AFX_DATA_INIT(CPortraitEditingDlg)
	m_nPart = -1;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_bStart = false;
	m_bColorAdjust = true;
	m_bRectSelected = false;
	m_bRectMove = false;
	m_bGCInit = false;
	m_bDrag = false;
	m_bDown = false;
	m_bImgOpened = false;
	m_bFeatureLocalized = false;
	m_bAuto = false;
	m_bLeftEar = false;
	m_bRightEar = false;
    m_bFeature = false;
    m_bBackground = false;
	m_bSkin = false;
    m_bHair = false;
    m_bCollar = false;
	m_bEar = false;
	m_bAutoColor = false;
	m_nSelectedFeature = -1;
	m_nRecordIndex = -1;
	m_nRecordMaxIndex = -1;
	m_nStatus = 0;
	m_nRender = 1;
	m_nR = m_nG = m_nB = 255;
	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	::GetCurrentDirectory(_MAX_PATH, m_sPath.GetBuffer(_MAX_PATH));
	::SetCurrentDirectory(m_sPath);
	m_sPath.ReleaseBuffer();
}

void CPortraitEditingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortraitEditingDlg)
	DDX_Control(pDX, IDC_LIST1, m_LstCtrComponent);
	DDX_Control(pDX, IDC_OPEN_IMAGE, m_btOpen);
	DDX_Control(pDX, IDC_SCISSORS, m_btScissors);
	DDX_Control(pDX, IDC_FEATURE_LOCAL, m_btFeature);
	DDX_Control(pDX, IDC_DETECT_BACKGROUND, m_btBackground);
	DDX_Control(pDX, IDC_DETECT_SKIN, m_btSkin);
	DDX_Control(pDX, IDC_DETECT_HAIR, m_btHair);
	DDX_Control(pDX, IDC_DETECT_COLLAR, m_btCollar);
	DDX_Control(pDX, IDC_GET_COLOR, m_btGetColor);
	DDX_Control(pDX, IDC_BRUSH_COLOR, m_btBrushColor);
	DDX_Control(pDX, IDC_Hair_COLOR, m_btHairColor);
	DDX_Control(pDX, IDC_SKIN_COLOR, m_btSkinColor);
	DDX_Control(pDX, IDC_CLOTH_COLOR, m_btClothColor);
	DDX_Control(pDX, IDC_BACKGROUND_COLOR, m_btBackgroundColor);
	DDX_Control(pDX, IDC_SHADOW_COLOR, m_btShadowColor);
	DDX_Control(pDX, IDC_Default_COLOR, m_btDefaultColor);
	DDX_Control(pDX, IDC_CROSS_STITCH, m_btCrossStitch);
	DDX_Control(pDX, IDC_CARTOON, m_btCartoon);
	DDX_Control(pDX, IDC_SAVE, m_btSave);
	DDX_Control(pDX, IDC_EXIT, m_btExit);
	DDX_Control(pDX, IDC_AUTORENDER, m_btRender);
	DDX_Control(pDX, IDC_DETECT_EAR, m_btEar);
	DDX_Control(pDX, IDC_CONSTRAIN_ASM, m_btAdjust);
	DDX_Radio(pDX, IDC_HAIR, m_nPart);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPortraitEditingDlg, CDialog)
	//{{AFX_MSG_MAP(CPortraitEditingDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_IMAGE, OnOpenImage)
	ON_BN_CLICKED(IDC_FEATURE_LOCAL, OnFeatureLocal)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_DETECT_BACKGROUND, OnDetectBackground)
	ON_BN_CLICKED(IDC_DETECT_SKIN, OnDetectSkin)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_Hair_COLOR, OnHairCOLOR)
	ON_BN_CLICKED(IDC_SKIN_COLOR, OnSkinColor)
	ON_BN_CLICKED(IDC_CLOTH_COLOR, OnClothColor)
	ON_BN_CLICKED(IDC_BACKGROUND_COLOR, OnBackgroundColor)
	ON_BN_CLICKED(IDC_SHADOW_COLOR, OnShadowColor)
	ON_BN_CLICKED(IDC_Default_COLOR, OnDefaultCOLOR)
	ON_BN_CLICKED(IDC_DETECT_HAIR, OnDetectHair)
	ON_BN_CLICKED(IDC_DETECT_COLLAR, OnDetectCollar)
	ON_BN_CLICKED(IDC_CROSS_STITCH, OnCrossStitch)
	ON_BN_CLICKED(IDC_GET_COLOR, OnGetColor)
	ON_BN_CLICKED(IDC_BRUSH_COLOR, OnBrushColor)
	ON_BN_CLICKED(IDC_CARTOON, OnPortrait)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	ON_BN_CLICKED(IDC_SCISSORS, OnScissors)
	ON_BN_CLICKED(IDC_HAIR, OnHair)
	ON_BN_CLICKED(IDC_EYEBROW, OnEyebrow)
	ON_BN_CLICKED(IDC_EYE, OnEye)
	ON_BN_CLICKED(IDC_NOSE, OnNose)
	ON_BN_CLICKED(IDC_MOUTH, OnMouth)
	ON_BN_CLICKED(IDC_COLLAR, OnCollar)
	ON_BN_CLICKED(IDC_BACKGROUND, OnBackground)
	ON_BN_CLICKED(IDC_AUTORENDER, OnAutorender)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_EFFECT1, OnEffect1)
	ON_BN_CLICKED(IDC_EFFECT2, OnEffect2)
	ON_BN_CLICKED(IDC_EFFECT3, OnEffect3)
	ON_BN_CLICKED(IDC_FORE, OnFore)
	ON_BN_CLICKED(IDC_BACKG, OnBackg)
	ON_BN_CLICKED(IDC_GCUT, OnGcut)
	ON_BN_CLICKED(IDC_CONSTRAIN_ASM, OnConstrainAsm)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_DETECT_EAR, OnDetectEar)
	ON_BN_CLICKED(IDC_EAR, OnEar)
	ON_BN_CLICKED(IDC_FACECONTOUR, OnFacecontour)
	ON_BN_CLICKED(IDC_REDO, OnRedo)
	ON_BN_CLICKED(IDC_UNDO, OnUndo)
	ON_BN_CLICKED(IDC_RESET, OnReset)
	//ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_ACTIVATE()
	ON_WM_ACTIVATEAPP()
	ON_BN_CLICKED(IDC_CROSS_STITCH2, OnEffect1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortraitEditingDlg message handlers

BOOL CPortraitEditingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	//m_btGetColor.SetBitmaps(IDB_STRAW, RGB(236,230,212));
	//m_btBrushColor.SetBitmaps(IDB_BRUSH, RGB(236,230,212));
	m_btOpen.DrawTransparent(TRUE);
	m_btScissors.DrawTransparent(TRUE);
	m_btFeature.DrawTransparent(TRUE);
	m_btBackground.DrawTransparent(TRUE);
	m_btSkin.DrawTransparent(TRUE);
	m_btHair.DrawTransparent(TRUE);
	m_btCollar.DrawTransparent(TRUE);
	m_btGetColor.DrawTransparent(TRUE);
	m_btBrushColor.DrawTransparent(TRUE);
	m_btHairColor.DrawTransparent(TRUE);
	m_btSkinColor.DrawTransparent(TRUE);
	m_btClothColor.DrawTransparent(TRUE);
	m_btBackgroundColor.DrawTransparent(TRUE);
	m_btShadowColor.DrawTransparent(TRUE);
	m_btDefaultColor.DrawTransparent(TRUE);
	m_btCrossStitch.DrawTransparent(TRUE);
	m_btCartoon.DrawTransparent(TRUE);
	m_btSave.DrawTransparent(TRUE);
	m_btExit.DrawTransparent(TRUE);
	m_btRender.DrawTransparent(TRUE);
	m_btEar.DrawTransparent(TRUE);
	m_btAdjust.DrawTransparent(TRUE);

	m_btScissors.EnableWindow(FALSE);
	m_btCartoon.EnableWindow(FALSE);
	m_btCartoon.ShowWindow(SW_HIDE);
	m_btFeature.EnableWindow(FALSE);
	m_btBackground.EnableWindow(FALSE);
	m_btSkin.EnableWindow(FALSE);
	m_btHair.EnableWindow(FALSE);
	m_btCollar.EnableWindow(FALSE);
	m_btDefaultColor.EnableWindow(FALSE);
	m_btHairColor.EnableWindow(FALSE);
	m_btSkinColor.EnableWindow(FALSE);
	m_btClothColor.EnableWindow(FALSE);
	m_btBackgroundColor.EnableWindow(FALSE);
	m_btShadowColor.EnableWindow(FALSE);
	m_btCrossStitch.EnableWindow(FALSE);
	m_btRender.EnableWindow(FALSE);
	m_btEar.EnableWindow(FALSE);
	m_btAdjust.EnableWindow(FALSE);

	
	// TODO: Add extra initialization here
	m_bmMainBg.LoadBitmap(IDB_BACKGROUND);
	BITMAP bm;
	m_bmMainBg.GetObject(sizeof(BITMAP), &bm);
	MoveWindow(0, 0, bm.bmWidth, bm.bmHeight + 30);

	LoadASMModel();
	//OnHair(); 

	CTime t_cur = CTime::GetCurrentTime();
	//CTime t_exp(2009, 12, 30, 23, 59, 59);   
	CTime t_exp(2012, 12, 31, 23, 59, 59);
	if (t_cur > t_exp)
	{
		m_btOpen.EnableWindow(FALSE);
		MessageBox("软件已过期");
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPortraitEditingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPortraitEditingDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
		if (!m_bImgOpened)
			return;

	
		CWnd *pWnd = GetDlgItem(IDC_SOURCE_IMG);
		CDC *pIMAGEDC = pWnd->GetDC();
		pIMAGEDC->SetBkMode(TRANSPARENT);

		CRect rect;
		pWnd->GetClientRect(&rect);
		rect.top += 2;
		
		int nRectWidth = rect.Width();
		int nRectHeight = rect.Height() - 12;
		double dRectScale = (double)(nRectHeight) / nRectWidth;

		int nImgWidth = m_pCurImg.Width();
		int nImgHeight = m_pCurImg.Height();
		double dImgScale = (double)(nImgHeight) / nImgWidth;

		int nDisHeight, nDisWidth;
		if(dImgScale > dRectScale)
		{
			nDisHeight = nRectHeight;
			nDisWidth = (int)(nDisHeight / dImgScale);
		}
		else
		{
			nDisWidth = nRectWidth;
			nDisHeight = (int) (nDisWidth * dImgScale);
		}
		m_ptLeftTop = rect.CenterPoint() - CPoint(nDisWidth / 2, nDisHeight / 2);
		m_dScale = (double)nDisWidth / nImgWidth;
		CRect showrect(m_ptLeftTop, CPoint(m_ptLeftTop.x + nDisWidth, m_ptLeftTop.y + nDisHeight));
		m_image.DrawToHDC(pIMAGEDC->m_hDC, showrect);
		
		CPen RedPen(PS_SOLID, 2, RGB(255, 0, 0) );
		CPen *pOldPen = pIMAGEDC->SelectObject(&RedPen);
		if (m_bFeatureLocalized)
		{

			if(m_nStatus == feature)
			{
				//105
//				for(int nlm = 0; nlm < NUM_OF_LANDMARKS - 5; nlm++)
//				{
//					int nX = (int) (m_ptArrLandMarks[nlm].x * m_dScale + m_ptLeftTop.x);
//					int nY = (int) (m_ptArrLandMarks[nlm].y * m_dScale + m_ptLeftTop.y);
//				
//					pIMAGEDC->Rectangle(nX-1, nY-1, nX+1, nY+1);
//				}

				CPoint showPoints[89];
				for(int i = 0; i < 89; i++)
				{
					int nXX = (int) (m_dpLandMarks[i].x * m_dScale + m_ptLeftTop.x);
					int nYY = (int) (m_dpLandMarks[i].y * m_dScale + m_ptLeftTop.y);

					showPoints[i].x = nXX;
					showPoints[i].y = nYY;

					 if(i == 70 || i == 73 || i == 76 || i == 79 || i == 82 || i == 85 || i == 88
					 || i == 16 || i == 20 || i == 21 || i == 25 || i == 26 || i == 30 || i == 31 || i == 35
					 || i == 0 || i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14
					 || i == 36 || i == 39 || i == 40 || i == 43 || i == 44 || i == 47
					 || i == 48 || i == 50 || i == 51 || i == 52 || i == 54 || i == 57)
					{
						pIMAGEDC->Rectangle(nXX-1, nYY-1, nXX+1, nYY+1);
					}
					
				}

				pIMAGEDC->SelectObject(pOldPen);
				CPen BluePen(PS_SOLID, 1, RGB(0, 0, 255));
				CPen *pOldPen = pIMAGEDC->SelectObject(&BluePen);
				for(i = 70; i < 88; i++)
				{
					pIMAGEDC->MoveTo(showPoints[i]);
					pIMAGEDC->LineTo(showPoints[i+1]);
				}

				for(i = 16; i < 26; i++)
				{
					if(i < 25)
					{
						pIMAGEDC->MoveTo(showPoints[i]);
						pIMAGEDC->LineTo(showPoints[i+1]);
					}
					else
					{
						pIMAGEDC->MoveTo(showPoints[i]);
						pIMAGEDC->LineTo(showPoints[16]);
					}
				}
				for(i = 26; i < 36; i++)
				{
					if(i < 35)
					{
						pIMAGEDC->MoveTo(showPoints[i]);
						pIMAGEDC->LineTo(showPoints[i+1]);
					}
					else
					{
						pIMAGEDC->MoveTo(showPoints[i]);
						pIMAGEDC->LineTo(showPoints[26]);
					}
				}

				for(i = 0; i < 8; i++)
				{
					if(i < 7)
					{
						pIMAGEDC->MoveTo(showPoints[i]);
						pIMAGEDC->LineTo(showPoints[i+1]);
					}
					else
					{
						pIMAGEDC->MoveTo(showPoints[i]);
						pIMAGEDC->LineTo(showPoints[0]);
					}
				}
				for(i = 8; i < 16; i++)
				{
					if(i < 15)
					{
						pIMAGEDC->MoveTo(showPoints[i]);
						pIMAGEDC->LineTo(showPoints[i+1]);
					}
					else
					{
						pIMAGEDC->MoveTo(showPoints[i]);
						pIMAGEDC->LineTo(showPoints[8]);
					}
				}

				for(i = 36; i < 47; i++)
				{
					pIMAGEDC->MoveTo(showPoints[i]);
					pIMAGEDC->LineTo(showPoints[i+1]);
				}

				for(i = 48; i < 60; i++)
				{
					if(i < 59)
					{
						pIMAGEDC->MoveTo(showPoints[i]);
						pIMAGEDC->LineTo(showPoints[i+1]);
					}
					else
					{
						pIMAGEDC->MoveTo(showPoints[i]);
						pIMAGEDC->LineTo(showPoints[48]);
					}
				}
			}
		
            if(m_nStatus == background)
			{
				for(int nlm = 0; nlm < m_backgroundVector.size(); nlm++)
				{
					int nX = (int) (m_backgroundVector[nlm].x * m_dScale + m_ptLeftTop.x);
					int nY = (int) (m_backgroundVector[nlm].y * m_dScale + m_ptLeftTop.y);
				
					pIMAGEDC->Rectangle(nX-1, nY-1, nX+1, nY+1);
				}

				pIMAGEDC->SelectObject(pOldPen);
				CPen BluePen(PS_SOLID, 1, RGB(0, 0, 255));
				CPen *pOldPen = pIMAGEDC->SelectObject(&BluePen);
				for(int i = 0; i < m_backgroundVector.size(); i++)
				{
					CPoint backPoint, nextPoint;

					backPoint.x= (int) (m_backgroundVector[i].x * m_dScale + m_ptLeftTop.x);
					backPoint.y = (int) (m_backgroundVector[i].y * m_dScale + m_ptLeftTop.y);
					if(i == m_backgroundVector.size() - 1)
					{
						nextPoint.x = (int) (m_backgroundVector[0].x * m_dScale + m_ptLeftTop.x);
						nextPoint.y = (int) (m_backgroundVector[0].y * m_dScale + m_ptLeftTop.y);
					}
					else
					{
						nextPoint.x= (int) (m_backgroundVector[i + 1].x * m_dScale + m_ptLeftTop.x);
						nextPoint.y = (int) (m_backgroundVector[i + 1].y * m_dScale + m_ptLeftTop.y);
					}
				
					if(backPoint.y > 10 && nextPoint.y > 10)
					{
						pIMAGEDC->MoveTo(backPoint);
						pIMAGEDC->LineTo(nextPoint);
					}
				}
			}
			
		    if(m_nStatus == skin)
			{
				for(int nlm = 0; nlm < m_skinVector.size(); nlm++)
				{
					int nX = (int) (m_skinVector[nlm].x * m_dScale + m_ptLeftTop.x);
					int nY = (int) (m_skinVector[nlm].y * m_dScale + m_ptLeftTop.y);
				
					pIMAGEDC->Rectangle(nX-1, nY-1, nX+1, nY+1);
				}

				pIMAGEDC->SelectObject(pOldPen);
				CPen BluePen(PS_SOLID, 1, RGB(0, 0, 255));
				CPen *pOldPen = pIMAGEDC->SelectObject(&BluePen);
				for(int i = 0; i < m_skinVector.size(); i++)
				{
					CPoint backPoint, nextPoint;

					backPoint.x= (int) (m_skinVector[i].x * m_dScale + m_ptLeftTop.x);
					backPoint.y = (int) (m_skinVector[i].y * m_dScale + m_ptLeftTop.y);
					if(i == m_skinVector.size() - 1)
					{
						nextPoint.x = (int) (m_skinVector[0].x * m_dScale + m_ptLeftTop.x);
						nextPoint.y = (int) (m_skinVector[0].y * m_dScale + m_ptLeftTop.y);
					}
					else
					{
						nextPoint.x= (int) (m_skinVector[i + 1].x * m_dScale + m_ptLeftTop.x);
						nextPoint.y = (int) (m_skinVector[i + 1].y * m_dScale + m_ptLeftTop.y);
					}
				
					pIMAGEDC->MoveTo(backPoint);
					pIMAGEDC->LineTo(nextPoint);
				}
			}

			if(m_nStatus == ear)
			{
				for(int nlm = 0; nlm < leftEarVector.size(); nlm++)
				{
					int nX = (int) (leftEarVector[nlm].x * m_dScale + m_ptLeftTop.x);
					int nY = (int) (leftEarVector[nlm].y * m_dScale + m_ptLeftTop.y);
				
					pIMAGEDC->Rectangle(nX-1, nY-1, nX+1, nY+1);
				}

				for(nlm = 0; nlm < rightEarVector.size(); nlm++)
				{
					int nX = (int) (rightEarVector[nlm].x * m_dScale + m_ptLeftTop.x);
					int nY = (int) (rightEarVector[nlm].y * m_dScale + m_ptLeftTop.y);
				
					pIMAGEDC->Rectangle(nX-1, nY-1, nX+1, nY+1);
				}
			}
			
			if(m_nStatus == hair)
			{
				for(int nlm = 0; nlm < hairVectorX.size(); nlm++)
				{
					int nX = (int) (hairVectorX[nlm] * m_dScale + m_ptLeftTop.x);
					int nY = (int) (hairVectorY[nlm] * m_dScale + m_ptLeftTop.y);
				
					pIMAGEDC->Rectangle(nX-1, nY-1, nX+1, nY+1);
				}

				pIMAGEDC->SelectObject(pOldPen);
				CPen BluePen(PS_SOLID, 1, RGB(0, 0, 255));
				CPen *pOldPen = pIMAGEDC->SelectObject(&BluePen);
				for(int i = 0; i < hairVectorX.size(); i++)
				{
					CPoint backPoint, nextPoint;
					
					backPoint.x = (int) (hairVectorX[i] * m_dScale + m_ptLeftTop.x);
					backPoint.y = (int) (hairVectorY[i] * m_dScale + m_ptLeftTop.y);
					if(i == hairVectorX.size() - 1)
					{
						nextPoint.x = (int) (hairVectorX[0] * m_dScale + m_ptLeftTop.x);
						nextPoint.y = (int) (hairVectorY[0] * m_dScale + m_ptLeftTop.y);
					}
					else
					{
						nextPoint.x = (int) (hairVectorX[i + 1] * m_dScale + m_ptLeftTop.x);
						nextPoint.y = (int) (hairVectorY[i + 1] * m_dScale + m_ptLeftTop.y);
					}
				
					pIMAGEDC->MoveTo(backPoint);
					pIMAGEDC->LineTo(nextPoint);
				}
			}	
			
			if(m_nStatus == collar)
			{
				for(int nlm = 0; nlm < clothVectorX.size(); nlm++)
				{
					int nX = (int) (clothVectorX[nlm] * m_dScale + m_ptLeftTop.x);
					int nY = (int) (clothVectorY[nlm] * m_dScale + m_ptLeftTop.y);
				
					pIMAGEDC->Rectangle(nX-1, nY-1, nX+1, nY+1);
				}

				pIMAGEDC->SelectObject(pOldPen);
				CPen BluePen(PS_SOLID, 1, RGB(0, 0, 255));
				CPen *pOldPen = pIMAGEDC->SelectObject(&BluePen);
				for(int i = 0; i < clothVectorX.size(); i++)
				{
					CPoint backPoint, nextPoint;

					backPoint.x = (int) (clothVectorX[i] * m_dScale + m_ptLeftTop.x);
					backPoint.y = (int) (clothVectorY[i] * m_dScale + m_ptLeftTop.y);
					if(i == clothVectorX.size() - 1)
					{
						nextPoint.x = (int) (clothVectorX[0] * m_dScale + m_ptLeftTop.x);
						nextPoint.y = (int) (clothVectorY[0] * m_dScale + m_ptLeftTop.y);
					}
					else
					{
						nextPoint.x = (int) (clothVectorX[i + 1] * m_dScale + m_ptLeftTop.x);
						nextPoint.y = (int) (clothVectorY[i + 1] * m_dScale + m_ptLeftTop.y);
					}
				
					pIMAGEDC->MoveTo(backPoint);
					pIMAGEDC->LineTo(nextPoint);
				}
			}		
		}
		pIMAGEDC->SelectObject(pOldPen);

		if (m_nSelectedFeature >= 0 || m_nSelectedVector.size() > 0)
		{
			CPen GreenPen(PS_SOLID, 2, RGB(0, 255, 0) );
			CPen *pOldPen = pIMAGEDC->SelectObject(&GreenPen);

			if(m_nStatus == feature)
			{
//				int nX = (int) (m_ptArrLandMarks[m_nSelectedFeature].x * m_dScale + m_ptLeftTop.x);
//				int nY = (int) (m_ptArrLandMarks[m_nSelectedFeature].y * m_dScale + m_ptLeftTop.y);
//				pIMAGEDC->Rectangle(nX-1, nY-1, nX+1, nY+1);

				int nX = (int) (m_dpLandMarks[m_nSelectedFeature].x * m_dScale + m_ptLeftTop.x);
				int nY = (int) (m_dpLandMarks[m_nSelectedFeature].y * m_dScale + m_ptLeftTop.y);
				pIMAGEDC->Rectangle(nX-1, nY-1, nX+1, nY+1);

				for(int i = 0; i < m_nSelectedVector.size(); i++)
				{	
					nX = (int) (m_dpLandMarks[m_nSelectedVector[i]].x * m_dScale + m_ptLeftTop.x);
					nY = (int) (m_dpLandMarks[m_nSelectedVector[i]].y * m_dScale + m_ptLeftTop.y);
					pIMAGEDC->Rectangle(nX-1, nY-1, nX+1, nY+1);
				}
			}

			 if(m_nStatus == background)
			 {
				int nX = (int) (m_backgroundVector[m_nSelectedFeature].x * m_dScale + m_ptLeftTop.x);
				int nY = (int) (m_backgroundVector[m_nSelectedFeature].y * m_dScale + m_ptLeftTop.y);
				pIMAGEDC->Rectangle(nX-1, nY-1, nX+1, nY+1); 

				for(int i = 0; i < m_nSelectedVector.size(); i++)
				{	
					nX = (int) (m_backgroundVector[m_nSelectedVector[i]].x * m_dScale + m_ptLeftTop.x);
					nY = (int) (m_backgroundVector[m_nSelectedVector[i]].y * m_dScale + m_ptLeftTop.y);
					pIMAGEDC->Rectangle(nX-1, nY-1, nX+1, nY+1);
				}
			 }

			 if(m_nStatus == ear)
			 {
				 if(m_bLeftEar == true)
				 {
					int nX = (int) (leftEarVector[m_nSelectedFeature].x * m_dScale + m_ptLeftTop.x);
					int nY = (int) (leftEarVector[m_nSelectedFeature].y * m_dScale + m_ptLeftTop.y);
					pIMAGEDC->Rectangle(nX-1, nY-1, nX+1, nY+1); 
				 }

				 if(m_bRightEar == true)
				 {
					int nX = (int) (rightEarVector[m_nSelectedFeature].x * m_dScale + m_ptLeftTop.x);
					int nY = (int) (rightEarVector[m_nSelectedFeature].y * m_dScale + m_ptLeftTop.y);
					pIMAGEDC->Rectangle(nX-1, nY-1, nX+1, nY+1); 
				 }
			 }

			 if(m_nStatus == skin)
			 {
				int nX = (int) (m_skinVector[m_nSelectedFeature].x * m_dScale + m_ptLeftTop.x);
				int nY = (int) (m_skinVector[m_nSelectedFeature].y * m_dScale + m_ptLeftTop.y);
				pIMAGEDC->Rectangle(nX-1, nY-1, nX+1, nY+1); 

				for(int i = 0; i < m_nSelectedVector.size(); i++)
				{	
					nX = (int) (m_skinVector[m_nSelectedVector[i]].x * m_dScale + m_ptLeftTop.x);
					nY = (int) (m_skinVector[m_nSelectedVector[i]].y * m_dScale + m_ptLeftTop.y);
					pIMAGEDC->Rectangle(nX-1, nY-1, nX+1, nY+1);
				}
			 }

			 if(m_nStatus == hair)
			 {
				int nX = (int) (hairVectorX[m_nSelectedFeature] * m_dScale + m_ptLeftTop.x);
				int nY = (int) (hairVectorY[m_nSelectedFeature] * m_dScale + m_ptLeftTop.y);
				pIMAGEDC->Rectangle(nX-1, nY-1, nX+1, nY+1); 

				for(int i = 0; i < m_nSelectedVector.size(); i++)
				{	
					nX = (int) (hairVectorX[m_nSelectedVector[i]] * m_dScale + m_ptLeftTop.x);
					nY = (int) (hairVectorY[m_nSelectedVector[i]] * m_dScale + m_ptLeftTop.y);
					pIMAGEDC->Rectangle(nX-1, nY-1, nX+1, nY+1);
				}
			 }

			 if(m_nStatus == collar)
			 {
				int nX = (int) (clothVectorX[m_nSelectedFeature] * m_dScale + m_ptLeftTop.x);
				int nY = (int) (clothVectorY[m_nSelectedFeature] * m_dScale + m_ptLeftTop.y);
				pIMAGEDC->Rectangle(nX-1, nY-1, nX+1, nY+1); 

				for(int i = 0; i < m_nSelectedVector.size(); i++)
				{	
					nX = (int) (clothVectorX[m_nSelectedVector[i]] * m_dScale + m_ptLeftTop.x);
					nY = (int) (clothVectorY[m_nSelectedVector[i]] * m_dScale + m_ptLeftTop.y);
					pIMAGEDC->Rectangle(nX-1, nY-1, nX+1, nY+1);
				}
			 }
		   	 pIMAGEDC->SelectObject(pOldPen);
		}

		CWnd *pTargetWnd = GetDlgItem(IDC_TARGET_IMG);
		CDC *pTIMAGEDC = pTargetWnd->GetDC();
		pTIMAGEDC->SetBkMode(TRANSPARENT);
		m_TargetImg.DrawToHDC(pTIMAGEDC->m_hDC, showrect);
	
		pWnd->ReleaseDC(pIMAGEDC);
		pTargetWnd->ReleaseDC(pTIMAGEDC);
		
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPortraitEditingDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPortraitEditingDlg::ImageChanged(IplImage* grayImage, int gray)
{
//	if(grayImage->nChannels == 1)
//	{
//		BYTE *buf = (BYTE *)grayImage->imageData;
//		int WidthPix = WIDTHBYTES(grayImage->width * 8);
//		for(int i = 0; i < grayImage->height; i++)
//		{
//			for(int j = 0; j < grayImage->width; j++)
//			{
//				buf[i * WidthPix + j] = gray;
//			}
//		}
//	}
//
//	if(grayImage->nChannels == 3)
//	{
//		BYTE *buf = (BYTE *)grayImage->imageData;
//		int WidthPix = WIDTHBYTES(grayImage->width * 24);
//		for(int i = 0; i < grayImage->height; i++)
//		{
//			for(int j = 0; j < grayImage->width; j++)
//			{
//				buf[i * WidthPix + 3 * j] = gray;
//				buf[i * WidthPix + 3 * j + 1] = gray;
//				buf[i * WidthPix + 3 * j + 2] = gray;
//			}
//		}
//	}

	memset((void *) grayImage->imageData, gray,
	WIDTHBYTES(grayImage->width * grayImage->nChannels * 8) * grayImage->height * sizeof(BYTE));
}

double CPortraitEditingDlg::ImageZoom(CvvImage &Image)
{	
		int nDisWidth = 450;
	    double ratio = (double) (nDisWidth) / Image.Width();
		int nDisHeight = (int)(Image.Height() * ratio);
		
		CvvImage tempImg;
		tempImg.Create(Image.Width(), Image.Height(), 24);
		cvCopy(Image.GetImage(), tempImg.GetImage());

		Image.Destroy();
		Image.Create(nDisWidth, nDisHeight, 24);

    	BYTE *buf1 = (BYTE *)tempImg.GetImage()->imageData;
		int WidthPix1 = WIDTHBYTES(tempImg.Width() * 24);
		BYTE *buf = (BYTE *)Image.GetImage()->imageData;
		int WidthPix = WIDTHBYTES(Image.Width() * 24);

		for(int i = 0; i < Image.Height(); i++)
		{
			for(int j = 0; j < Image.Width(); j++)
			{
				int x = (int)(j / ratio + 0.5);
		    	int y = (int)(i / ratio + 0.5);

				if(x >= 0 && x < tempImg.Width() && y >= 0 && y < tempImg.Height())
				{
//					buf[i * WidthPix + 3 * j] = buf1[y * WidthPix1 + 3 * x];
//					buf[i * WidthPix + 3 * j + 1] = buf1[y * WidthPix1 + 3 * x + 1];
//					buf[i * WidthPix + 3 * j + 2] = buf1[y * WidthPix1 + 3 * x + 2];
					memcpy((void *) (buf + i * WidthPix + 3 * j), (const void *) (buf1 + y * WidthPix1 + 3 * x), 3 * sizeof(BYTE));
				}
				else
				{
//					buf[i * WidthPix + 3 * j] = 255;
//					buf[i * WidthPix + 3 * j + 1] = 255;
//					buf[i * WidthPix + 3 * j + 2] = 255;
					memset((void *) (buf + i * WidthPix + 3 * j), 255, 3 * sizeof(BYTE));
				}
			}
		}	

		return ratio;
}

void CPortraitEditingDlg::ImageZoom(CvvImage &Image, double ratioX, double ratioY)
{
	IplImage* ImageCopy = 0;
	ImageCopy = cvCreateImage(cvGetSize(Image.GetImage()), IPL_DEPTH_8U, 3);
	cvCopy(Image.GetImage(), ImageCopy);

	int width = (int) (Image.Width() * ratioX);
	int height = (int) (Image.Height() * ratioY);
	Image.Destroy();
	Image.Create(width, height, 24);

	cvResize(ImageCopy, Image.GetImage());

//	BYTE *buf1 = (BYTE *)ImageCopy->imageData;
//	int WidthPix1 = WIDTHBYTES(ImageCopy->width * 24);
//	BYTE *buf = (BYTE *)Image.GetImage()->imageData;
//	int WidthPix = WIDTHBYTES(Image.Width() * 24);
//
//	double f1,f2,f3,f4,f12,f34;
//	for(int i = 0; i < Image.Height(); i++)
//	{
//		for(int j = 0; j < Image.Width(); j++)
//		{
//			int x = (int)(j / ratioX + 0.5);
//			int y = (int)(i / ratioY + 0.5);
//			int index = i * WidthPix + 3 * j;
//
//			if(x >= 0 && x < ImageCopy->width && y >= 0 && y < ImageCopy->height)
//			{
//				if(x == ImageCopy->width - 1 && y == ImageCopy->height - 1)
//				{
////					buf[i * WidthPix + 3 * j] = buf1[y * WidthPix1 + 3 * x];
////					buf[i * WidthPix + 3 * j + 1] = buf1[y * WidthPix1 + 3 * x + 1];
////					buf[i * WidthPix + 3 * j + 2] = buf1[y * WidthPix1 + 3 * x + 2];
//
//					memcpy((void *) (buf + i * WidthPix + 3 * j), (const void *) (buf1 + y * WidthPix1 + 3 * x), 3 * sizeof(BYTE));
//				}
//				else if(x == ImageCopy->width - 1)
//				{
//					f1 = buf1[y * WidthPix1 + 3 * x];
//					f3 = buf1[(y + 1) * WidthPix1 + 3 * x];
//					buf[index] = BYTE(f1 + (i / ratioY + 0.5 - y) * (f3 - f1));
//
//					f1 = buf1[y * WidthPix1 + 3 * x + 1];
//					f3 = buf1[(y + 1) * WidthPix1 + 3 * x + 1];
//					buf[index + 1] = BYTE(f1 + (i / ratioY + 0.5 - y) * (f3 - f1));
//
//					f1 = buf1[y * WidthPix1 + 3 * x + 2];
//					f3 = buf1[(y + 1) * WidthPix1 + 3 * x + 2];
//					buf[index + 2] = BYTE(f1 + (i / ratioY + 0.5 - y) * (f3 - f1));
//				}
//				else if(y == ImageCopy->height - 1)
//				{
//					f1 = buf1[y * WidthPix1 + 3 * x];
//					f2 = buf1[y * WidthPix1 + 3 * (x + 1)];
//					buf[index] = BYTE(f1 + (j / ratioX + 0.5 - x) * (f2 - f1));
//
//					f1 = buf1[y * WidthPix1 + 3 * x + 1];
//					f2 = buf1[y * WidthPix1 + 3 * (x + 1) + 1];
//					buf[index + 1] = BYTE(f1 + (j / ratioX + 0.5 - x) * (f2 - f1));
//
//					f1 = buf1[y * WidthPix1 + 3 * x + 2];
//					f2 = buf1[y * WidthPix1 + 3 * (x + 1) + 2];
//					buf[index + 2] = BYTE(f1 + (j / ratioX + 0.5 - x) * (f2 - f1));
//				}
//				else
//				{
//					f1 = buf1[y * WidthPix1 + 3 * x];
//					f2 = buf1[y * WidthPix1 + 3 * (x + 1)];
//					f3 = buf1[(y + 1) * WidthPix1 + 3 * x];
//					f4 = buf1[(y + 1) * WidthPix1 + 3 * (x + 1)];
//
//					f12 = f1 + (j / ratioX + 0.5 - x) * (f2 - f1);
//					f34 = f3 + (j / ratioX + 0.5 - x) * (f4 - f3);
//					buf[index] = BYTE(f12 + (i / ratioY + 0.5 - y) * (f34 - f12));
//
//					f1 = buf1[y * WidthPix1 + 3 * x + 1];
//					f2 = buf1[y * WidthPix1 + 3 * (x + 1) + 1];
//					f3 = buf1[(y + 1) * WidthPix1 + 3 * x + 1];
//					f4 = buf1[(y + 1) * WidthPix1 + 3 * (x + 1) + 1];
//
//					f12 = f1 + (j / ratioX + 0.5 - x) * (f2 - f1);
//					f34 = f3 + (j / ratioX + 0.5 - x) * (f4 - f3);
//					buf[index + 1] = BYTE(f12 + (i / ratioY + 0.5 - y) * (f34 - f12));
//
//					f1 = buf1[y * WidthPix1 + 3 * x + 2];
//					f2 = buf1[y * WidthPix1 + 3 * (x + 1) + 2];
//					f3 = buf1[(y + 1) * WidthPix1 + 3 * x + 2];
//					f4 = buf1[(y + 1) * WidthPix1 + 3 * (x + 1) + 2];
//
//					f12 = f1 + (j / ratioX + 0.5 - x) * (f2 - f1);
//					f34 = f3 + (j / ratioX + 0.5 - x) * (f4 - f3);
//					buf[index + 2] = BYTE(f12 + (i / ratioY + 0.5 - y) * (f34 - f12));
//				}
//			}
//			else
//			{
////				buf[i * WidthPix + 3 * j] = 255;
////				buf[i * WidthPix + 3 * j + 1] = 255;
////				buf[i * WidthPix + 3 * j + 2] = 255;
//
//				memset((void *) (buf + i * WidthPix + 3 * j), 255, 3 * sizeof(BYTE));
//			}
//		}
//	}

	cvReleaseImage(&ImageCopy);
}


void CPortraitEditingDlg::Mean(IplImage* YCrCbImage, CRect rect, double &meanY, double &meanCr, double &meanCb, double &max_diffY, double &max_diffCr, double &max_diffCb)
{
	int nWidth = YCrCbImage->width;
	int nHeight = YCrCbImage->height;
	BYTE *buf = (BYTE *) YCrCbImage->imageData;
	int WidthPix = WIDTHBYTES(nWidth * 24);

	double sumY = 0;
	double sumCr = 0;
	double sumCb = 0;
	max_diffY = 0;
	max_diffCr = 0;
	max_diffCb = 0;


	int row, col;
	for(row = rect.top; row < rect.top + rect.Height(); row++) 
	{
		for(col = rect.left; col < rect.left + rect.Width(); col++) 
		{
			 sumY = sumY + *(buf + row * WidthPix + 3 * col); 
			 sumCr = sumCr + *(buf + row * WidthPix + 3 * col + 1);
			 sumCb = sumCb + *(buf + row * WidthPix + 3 * col + 2);
		}
	}

	meanY = sumY / (1.0 * rect.Height() * rect.Width());
	meanCr = sumCr /(1.0 * rect.Height() * rect.Width());
	meanCb = sumCb /(1.0 * rect.Height() * rect.Width());

	for(row = rect.top; row < rect.top + rect.Height(); row++) 
	{
		for(col = rect.left; col < rect.left + rect.Width(); col++) 
		{
			double tempY = fabs(*(buf + row * WidthPix + 3 * col) - meanY);
			double tempCr = fabs(*(buf + row * WidthPix + 3 * col + 1) - meanCr);
			double tempCb = fabs(*(buf + row * WidthPix + 3 * col + 2) - meanCb);
			if(max_diffY < tempY)
				max_diffY = tempY;
			if(max_diffCr < tempCr)
				max_diffCr = tempCr;
			if(max_diffCb < tempCb)
				max_diffCb = tempCb;
		}
	}
}

void CPortraitEditingDlg::LoadTemplate(CString sFileName, vector<dPoint> &pointVector)
{
		FILE *fp = fopen(sFileName, "r");

		if(fp != NULL)
		{
			int nPoint;
			fscanf(fp, "%d\n", &nPoint);

			for(int i = 0; i < nPoint; i++)
			{
				double dx, dy;
				fscanf(fp, "%lf %lf\n", &dx, &dy);

				dPoint temPoint;
				temPoint.x = dx;
				temPoint.y = dy;
				pointVector.push_back(temPoint);
			}
			
			fclose(fp);
		}		
}

void CPortraitEditingDlg::LoadTemplate(CString sFileName, vector<double> &pointVectorX, vector<double> &pointVectorY)
{
		FILE *fp = fopen(sFileName, "r");

		if(fp != NULL)
		{
			int nPoint;
			fscanf(fp, "%d\n", &nPoint);

			for(int i = 0; i < nPoint; i++)
			{
				double dx, dy;
				fscanf(fp, "%lf %lf\n", &dx, &dy);

				pointVectorX.push_back(dx);
				pointVectorY.push_back(dy);
			}
			
			fclose(fp);
		}		
}

void CPortraitEditingDlg::LoadTemplate(CString sFileName, vector< vector<CPoint> > &pointVVector)
{
		FILE *fp = fopen(sFileName, "r");

		vector<CPoint> tempVector;
		if(fp != NULL)
		{
			int nLine;
			fscanf(fp, "%d\n", &nLine);

			for(int i = 0; i < nLine; i++)
			{
				int nPoint;
				fscanf(fp, "%d\n", &nPoint);

				tempVector.clear();
				for(int j = 0; j < nPoint; j++)
				{
					int dx, dy;
					fscanf(fp, "%d %d\n", &dx, &dy);

					CPoint temPoint;
					temPoint.x = dx;
					temPoint.y = dy;
					tempVector.push_back(temPoint);
				}
			
				pointVVector.push_back(tempVector);
			}
			
			fclose(fp);
		}		
}

void CPortraitEditingDlg::ReadTriangle(CString sFileName,  vector<int> &nV1,  vector<int> &nV2, vector<int> &nV3)
{
	FILE *fh = fopen(sFileName, "r");

	if(fh != NULL)
	{
		int nTriNum;
		fscanf(fh, "%d\n", &nTriNum);

		for (int nTri = 0; nTri < nTriNum; nTri++)
		{
			int n1, n2, n3;
			fscanf(fh, "%d %d %d\n", &n1, &n2, &n3);
			
			nV1.push_back(n1);
			nV2.push_back(n2);
			nV3.push_back(n3);
		}

		fclose(fh);
	}
}


void  CPortraitEditingDlg::SampleLine(dPoint p1, dPoint p2, double length, vector<dPoint> &pointVector)
{
	double dist = GetDPointDistance(p1, p2);
	if(dist <= length)
	{
		pointVector.push_back(p1);
	}
	else
	{
		pointVector.push_back(p1);

		int i;
		dPoint tempPoint;
		if(fabs(p2.x - p1.x) < 0.00001)
		{
			for(i = 1; i < 10000; i++)
			{
				tempPoint.x = p1.x;
				tempPoint.y = p1.y + Sign(p2.y - p1.y) * i * length;

				if((tempPoint.y - p2.y) * (p2.y - p1.y) < 0)
					pointVector.push_back(tempPoint);
				else
					break;	
			}
		}
		else
		{
			double k = (p2.y - p1.y) / (p2.x - p1.x);
			for(i = 1; i < 10000; i++)
			{
				tempPoint.x = p1.x + Sign(p2.x - p1.x) * i * length / sqrt(1 + k * k);
				tempPoint.y = k * (tempPoint.x - p1.x) + p1.y;

				if((tempPoint.x - p2.x) * (p2.x - p1.x) < 0)
					pointVector.push_back(tempPoint);
				else
					break;	
			}
		}
	}
}

void CPortraitEditingDlg::ImageWarp(IplImage *sourceImage, IplImage *targetImage, vector<dPoint> &sourceVector, vector<dPoint> &targetVector, bool isErase)
{
	if(sourceVector.size() == targetVector.size())
	{
		CRgn	 targetRgn;
		CRect    rect;
		int      i, j, k, t;
		int      size = sourceVector.size();
		double   tx = 0;
		double   ty = 0;
		
		CPoint *tempPoint = new CPoint[size - 1];
		for(i = 0; i < size - 1; i++)
		{
			tempPoint[i].x = targetVector[i].x;
			tempPoint[i].y = targetVector[i].y;
		}
		targetRgn.CreatePolygonRgn(tempPoint, size - 1, WINDING);
		targetRgn.GetRgnBox(rect);

		BYTE *buf1 = (BYTE *)sourceImage->imageData;
		BYTE *buf2 = (BYTE *)targetImage->imageData;
		int WidthPix1 = WIDTHBYTES(sourceImage->width * 24);
		int WidthPix2 = WIDTHBYTES(targetImage->width * 24);

		for(i = rect.top - 2; i < rect.bottom + 2; i++)
		{
			for(j = rect.left - 2; j < rect.right + 2; j++)
			{
				if(isErase == true)
				{
					buf2[i * WidthPix2 + 3 * j] = 255;
					buf2[i * WidthPix2 + 3 * j + 1] = 255;
					buf2[i * WidthPix2 + 3 * j + 2] = 255;
				}
				
				if(targetRgn.PtInRegion(CPoint(j,i)))
				{
					for(k = 0; k < size - 1; k++)
					{
						t = k + 1;
						if(t == size - 1)
						{
							t = 0;
						}

						int num = TransformPoint(targetVector[k].x, targetVector[k].y, targetVector[t].x, targetVector[t].y, targetVector[size-1].x, targetVector[size-1].y,
												 sourceVector[k].x, sourceVector[k].y, sourceVector[t].x, sourceVector[t].y, sourceVector[size-1].x, sourceVector[size-1].y,
									             j, i, tx, ty);
						if(num == 0)
							continue;
						else
						{
							buf2[i * WidthPix2 + j * 3] = buf1[(int)(ty) * WidthPix1 + (int)(tx) * 3];
							buf2[i * WidthPix2 + j * 3 + 1] = buf1[(int)(ty) * WidthPix1 + (int)(tx) * 3 + 1];
							buf2[i * WidthPix2 + j * 3 + 2] = buf1[(int)(ty) * WidthPix1 + (int)(tx) * 3 + 2];

							break;
						}	
					}
				}
			}
		}

		delete [] tempPoint;
	}
}

void CPortraitEditingDlg::WarpImg2Img(IplImage *imgDes, IplImage *imgSrc, vector<CPoint> &vecDes, vector<CPoint> &vecSrc, vector<int> &vMeshV1, vector<int> &vMeshV2, vector<int> &vMeshV3)
{
	int nBits = imgDes->nChannels * 8;

	int nHeightDes = imgDes->height;
	int nWidthDes = imgDes->width;
	long lbyteperlimgDes = WIDTHBYTES(imgDes->width * nBits);
	BYTE *pDataDes = (BYTE *)imgDes->imageData;

	int nHeightSrc = imgSrc->height;
	int nWidthSrc = imgSrc->width;
	long lbyteperlingSrc = WIDTHBYTES(imgSrc->width * nBits);
	BYTE *pDataSrc = (BYTE *)imgSrc->imageData;

	int nTriangles = vMeshV1.size();
	for (int nTri = 0; nTri < nTriangles; nTri++)
	{

		int nV1 = vMeshV1[nTri];
		int nV2 = vMeshV2[nTri];
		int nV3 = vMeshV3[nTri];

		double dx1_des = vecDes[nV1].x;		double dy1_des = vecDes[nV1].y;
		double dx2_des = vecDes[nV2].x;		double dy2_des = vecDes[nV2].y;
		double dx3_des = vecDes[nV3].x;		double dy3_des = vecDes[nV3].y;

		double dx1_src = vecSrc[nV1].x;		double dy1_src = vecSrc[nV1].y;
		double dx2_src = vecSrc[nV2].x;		double dy2_src = vecSrc[nV2].y;
		double dx3_src = vecSrc[nV3].x;		double dy3_src = vecSrc[nV3].y;

		//get rect
		double tx, ty;
		int nMinX = Min(Min(dx1_des, dx2_des), dx3_des);	nMinX = Max(nMinX, 0);
		int nMinY = Min(Min(dy1_des, dy2_des), dy3_des);	nMinY = Max(nMinY, 0);
		int nMaxX = Max(Max(dx1_des, dx2_des), dx3_des);	nMaxX = Min(nMaxX, nWidthDes-1);
		int nMaxY = Max(Max(dy1_des, dy2_des), dy3_des);	nMaxY = Min(nMaxY, nHeightDes-1);

		for (int h = nMinY; h < nMaxY; h++)
		{
			for (int w = nMinX; w < nMaxX; w++)
			{

				int nFind = TransformPoint(dx1_des, dy1_des, dx2_des, dy2_des, dx3_des, dy3_des,
					dx1_src, dy1_src, dx2_src, dy2_src, dx3_src, dy3_src, w, h, tx, ty);

				if (nFind == 0)
					continue;
				
				int nFromX = w;
				int nFromY = h;
				int nToX = tx;
				int nToY = ty;


				if (nBits == 24)
				{
					int nPosDes = nFromY  * lbyteperlimgDes + nFromX * 3;
					int nPosSrc = nToY * lbyteperlingSrc + nToX * 3;
					pDataDes[nPosDes++] = pDataSrc[nPosSrc++];
					pDataDes[nPosDes++] = pDataSrc[nPosSrc++];
					pDataDes[nPosDes]	= pDataSrc[nPosSrc];
				}
				else
				{
					int nPosDes = nFromY  * lbyteperlimgDes + nFromX;
					int nPosSrc = nToY  * lbyteperlingSrc + nToX;
					pDataDes[nPosDes]	= pDataSrc[nPosSrc];
				}
			}
		}
	}
}

void CPortraitEditingDlg::ConncetContour(vector<dPoint> &pointVector, IplImage *img, CvScalar &color, int lineWidth, bool isSkin)
{
	BYTE *buf1 = (BYTE *)img->imageData;
	int WidthPix1 = WIDTHBYTES(img->width * 24);

	CvPoint pt1;
	CvPoint pt2;
	for(int i = 0; i < pointVector.size(); i++)
	{
		int next = i + 1;
		if(next == pointVector.size())
		{
          next = 0;   
		}

		pt1.x = pointVector[i].x;
		pt1.y = pointVector[i].y;
		pt2.x = pointVector[next].x;
		pt2.y = pointVector[next].y;

		CRgn tempRgn;
		CPoint points[23];
		for(int k = 0; k < 23; k++)
		{
			points[k].x = m_dpLandMarks[k + 68].x;
			points[k].y = m_dpLandMarks[k + 68].y;
		}
		tempRgn.CreatePolygonRgn(points, 23, WINDING);

		int left = Min(pt1.x, pt2.x);
		int top = Min(pt1.y, pt2.y);
		int right = Max(pt1.x, pt2.x);
		int bottom = Max(pt1.y, pt2.y);
		int sum = 0;
		int len = sqrt(Sqr(right - left) + Sqr(bottom - top));
		for(k = top; k < bottom; k++)
		{
			for(int j = left; j < right; j++)
			{
				if(k >= 0 && k < img->height && j >= 0 && j < img->width)
				{
					if(buf1[k * WidthPix1 + 3 * j] < 220)
					{
						sum++;
					}
				}
			}
		}
        
		//if(sum < 1.6*len || color.val[0] != 0)
		{
			if(isSkin == false)
				cvLine(img, pt1, pt2, color, lineWidth, CV_AA);
			else
			{	
				if(tempRgn.PtInRegion(CPoint(pt1.x, pt1.y)) && tempRgn.PtInRegion(CPoint(pt2.x, pt2.y)))
					continue;

				if(clothRect.top == m_TargetImg.Height())
				{
					if(pt1.y > m_dpLandMarks[69].y && pt2.y > m_dpLandMarks[69].y)
						cvLine(img, pt1, pt2, color, lineWidth, CV_AA);
				}
				else
				{
					if(clothRgn.PtInRegion(CPoint(pt1.x, pt1.y)) && clothRgn.PtInRegion(CPoint(pt2.x, pt2.y)))
						continue;

					 if(pt1.y > m_dpLandMarks[75].y && pt2.y > m_dpLandMarks[75].y && pt1.y < clothRect.top + 30 && pt2.y < clothRect.top + 30)
						 cvLine(img, pt1, pt2, color, lineWidth, CV_AA);
				}
			}
		}
	}
}

void CPortraitEditingDlg::ConncetContour(vector<double> &pointVectorX, vector<double> &pointVectorY, IplImage *img, CvScalar &color, int lineWidth, bool isOpen)
{
	BYTE *buf1 = (BYTE *)img->imageData;
	int WidthPix1 = WIDTHBYTES(img->width * 24);

	CvPoint pt1;
	CvPoint pt2;
	for(int i = 0; i < pointVectorX.size(); i++)
	{
		int next = i + 1;
		if(next == pointVectorX.size())
		{
          next = 0;   
		}

		pt1.x = pointVectorX[i];
		pt1.y = pointVectorY[i];
		pt2.x = pointVectorX[next];
		pt2.y = pointVectorY[next];

		int left = Min(pt1.x, pt2.x);
		int top = Min(pt1.y, pt2.y);
		int right = Max(pt1.x, pt2.x);
		int bottom = Max(pt1.y, pt2.y);
		int sum = 0;
		int len = sqrt(Sqr(right - left) + Sqr(bottom - top));
		for(int k = top; k < bottom; k++)
		{
			for(int j = left; j < right; j++)
			{
				if(k >= 0 && k < img->height && j >= 0 && j < img->width)
				{
					if(buf1[k * WidthPix1 + 3 * j] < 220)
					{
						sum++;
					}
				}
			}
		}
        
		if(isOpen == false)
		{
			if(sum < 1.6*len || color.val[0] != 0)
				cvLine(img, pt1, pt2, color, lineWidth, CV_AA);
		}
		else
		{
			if((sum < 1.6*len || color.val[0] != 0) && next != 0)
				cvLine(img, pt1, pt2, color, lineWidth, CV_AA);
		}
	}
}

void CPortraitEditingDlg::ConnectLandmark(CvScalar &color, int lineWidth)
{
	int i = 0;
	CvPoint FacePoint[25];
	for(i = 0; i < 25; i++)
	{
		FacePoint[i].x = m_ptArrLandMarks[i].x;
		FacePoint[i].y = m_ptArrLandMarks[i].y;
	}
	for(i = 0; i < 24; i++)
	{
		cvLine(m_TargetImg.GetImage(), FacePoint[i], FacePoint[i + 1], color, lineWidth, CV_AA);
	}

	//connect right eyebrow
	CvPoint RightEyebrowPoint[8];
	for(i = 0; i < 8; i++)
	{
		RightEyebrowPoint[i].x = m_ptArrLandMarks[25 + i].x;
		RightEyebrowPoint[i].y = m_ptArrLandMarks[25 + i].y;
	}
	for(i = 0; i < 8; i++)
	{
		if(i < 7)
			cvLine(m_TargetImg.GetImage(), RightEyebrowPoint[i], RightEyebrowPoint[i + 1], color, lineWidth, CV_AA);
		else
			cvLine(m_TargetImg.GetImage(), RightEyebrowPoint[7], RightEyebrowPoint[0], color, lineWidth, CV_AA);
	}

	//connect left eyebrow
	CvPoint LeftEyebrowPoint[8];
	for(i = 0; i < 8; i++)
	{
		LeftEyebrowPoint[i].x = m_ptArrLandMarks[33 + i].x;
		LeftEyebrowPoint[i].y = m_ptArrLandMarks[33 + i].y;
	}
	for(i = 0; i < 8; i++)
	{
		if(i < 7)
			cvLine(m_TargetImg.GetImage(), LeftEyebrowPoint[i], LeftEyebrowPoint[i + 1], color, lineWidth, CV_AA);
		else
			cvLine(m_TargetImg.GetImage(), LeftEyebrowPoint[7], LeftEyebrowPoint[0], color, lineWidth, CV_AA);
	}

	//connect left eye
	CvPoint LeftEyePoint[8];
	for(i = 0; i < 8; i++)
	{
		LeftEyePoint[i].x = m_ptArrLandMarks[41 + i].x;
		LeftEyePoint[i].y = m_ptArrLandMarks[41 + i].y;
	}
	for(i = 0; i < 8; i++)
	{
		if(i < 7)
			cvLine(m_TargetImg.GetImage(), LeftEyePoint[i], LeftEyePoint[i + 1], color, lineWidth, CV_AA);
		else
			cvLine(m_TargetImg.GetImage(), LeftEyePoint[7], LeftEyePoint[0], color, lineWidth, CV_AA);
	}

	//connect right eye
	CvPoint RightEyePoint[8];
	for(i = 0; i < 8; i++)
	{
		RightEyePoint[i].x = m_ptArrLandMarks[49 + i].x;
		RightEyePoint[i].y = m_ptArrLandMarks[49 + i].y;
	}
	for(i = 0; i < 8; i++)
	{
		if(i < 7)
			cvLine(m_TargetImg.GetImage(), RightEyePoint[i], RightEyePoint[i + 1], color, lineWidth, CV_AA);
		else
			cvLine(m_TargetImg.GetImage(), RightEyePoint[7], RightEyePoint[0], color, lineWidth, CV_AA);
	}

	//connect nose
	CvPoint NosePoint[15];
	for(i = 0; i < 15; i++)
	{
		if(i < 6)
		{
			NosePoint[i].x = m_ptArrLandMarks[61 + i].x;
			NosePoint[i].y = m_ptArrLandMarks[61 + i].y;
		}
		if(i == 6)
		{
			NosePoint[i].x = m_ptArrLandMarks[98].x;
			NosePoint[i].y = m_ptArrLandMarks[98].y;
		}
		if(i == 7)
		{
			NosePoint[i].x = m_ptArrLandMarks[67].x;
			NosePoint[i].y = m_ptArrLandMarks[67].y;
		}
		if(i == 8)
		{
			NosePoint[i].x = m_ptArrLandMarks[99].x;
			NosePoint[i].y = m_ptArrLandMarks[99].y;
		}
		if(i > 8)
		{
			NosePoint[i].x = m_ptArrLandMarks[59 + i].x;
			NosePoint[i].y = m_ptArrLandMarks[59 + i].y;
		}
	}
	for(i = 0; i < 11; i++)
	{
		cvLine(m_TargetImg.GetImage(), NosePoint[i], NosePoint[i + 1], color, lineWidth, CV_AA);
	}

	//connect mouth
	CvPoint MouthPoint[22];
	for(i = 0; i < 22; i++)
	{
		MouthPoint[i].x = m_ptArrLandMarks[74 + i].x;
		MouthPoint[i].y = m_ptArrLandMarks[74 + i].y;
	}

	double dist = GetDistance(MouthPoint[12].x, MouthPoint[12].y, MouthPoint[21].x, MouthPoint[21].y);
	for(i = 0; i < 22; i++)
	{
		if(i < 11)
			cvLine(m_TargetImg.GetImage(), MouthPoint[i], MouthPoint[i + 1], color, lineWidth, CV_AA);
		else
			cvLine(m_TargetImg.GetImage(), MouthPoint[11], MouthPoint[0], color, lineWidth, CV_AA);

		if(i > 11 && i < 16)
		{
			cvLine(m_TargetImg.GetImage(), MouthPoint[0], MouthPoint[12], color, lineWidth, CV_AA);
			cvLine(m_TargetImg.GetImage(), MouthPoint[i], MouthPoint[i + 1], color, lineWidth, CV_AA);
			cvLine(m_TargetImg.GetImage(), MouthPoint[16], MouthPoint[6], color, lineWidth, CV_AA);
		}

		if(dist > 5 && i > 16 && i < 21)
		{
			cvLine(m_TargetImg.GetImage(), MouthPoint[0], MouthPoint[21], color, lineWidth, CV_AA);
			cvLine(m_TargetImg.GetImage(), MouthPoint[i], MouthPoint[i + 1], color, lineWidth, CV_AA);
			cvLine(m_TargetImg.GetImage(), MouthPoint[17], MouthPoint[6], color, lineWidth, CV_AA);
		}
	}
	
	Invalidate(FALSE);
}

void CPortraitEditingDlg::ExtractPnt(vector<CPoint> &vecDes)
{
	int i;
	for(i = 0; i < 57; i++)
	{
		vecDes.push_back(m_ptArrLandMarks[i]);
	}
	for(i = 61; i < 86; i++)
	{
		vecDes.push_back(m_ptArrLandMarks[i]);
	}
	vecDes.push_back(m_ptArrLandMarks[102]);

	int diff = m_ptArrLandMarks[1].y - m_ptArrLandMarks[0].y;
	CPoint pt84, pt85;
	pt84.x = m_ptArrLandMarks[24].x;
	pt84.y = m_ptArrLandMarks[25].y;
	pt85.x = m_ptArrLandMarks[25].x;
	pt85.y = m_ptArrLandMarks[104].y + diff;
	vecDes.push_back(pt84);
	vecDes.push_back(pt85);
	vecDes.push_back(m_ptArrLandMarks[104]);
	CPoint pt87;
	pt87.x = m_ptArrLandMarks[102].x;
	pt87.y = m_ptArrLandMarks[104].y - diff/3;
	vecDes.push_back(pt87);
	vecDes.push_back(m_ptArrLandMarks[103]);
	CPoint pt89, pt90;
	pt89.x = m_ptArrLandMarks[36].x;
	pt89.y = m_ptArrLandMarks[103].y + diff;
	pt90.x = m_ptArrLandMarks[0].x;
	pt90.y = m_ptArrLandMarks[37].y;
	vecDes.push_back(pt89);
	vecDes.push_back(pt90);
	
}

void CPortraitEditingDlg::DrawEyeball()
{
	CRgn	 LeftEyeRgn, RightEyeRgn;
	CRect    LeftRect, RightRect;
	int      i, j;
	
	CPoint LeftEyePoint[8];
	for(i = 0; i < 8; i++)
	{
		LeftEyePoint[i].x = m_dpLandMarks[i].x; //m_ptArrLandMarks[41 + i].x;
		LeftEyePoint[i].y = m_dpLandMarks[i].y; // m_ptArrLandMarks[41 + i].y;
	}
	LeftEyeRgn.CreatePolygonRgn(LeftEyePoint, 8, WINDING);
	LeftEyeRgn.GetRgnBox(LeftRect);

	CPoint RightEyePoint[8];
	for(i = 0; i < 8; i++)
	{
		RightEyePoint[i].x = m_dpLandMarks[8 + i].x; //m_ptArrLandMarks[49 + i].x;
		RightEyePoint[i].y = m_dpLandMarks[8 + i].y; //m_ptArrLandMarks[49 + i].y;
	}
	RightEyeRgn.CreatePolygonRgn(RightEyePoint, 8, WINDING);
	RightEyeRgn.GetRgnBox(RightRect);

	IplImage* grayImage = 0;
	grayImage = cvCreateImage(cvGetSize(m_image.GetImage()), IPL_DEPTH_8U, 1);
	cvCvtColor(m_image.GetImage(), grayImage, CV_BGR2GRAY);

	BYTE *buf = (BYTE *)grayImage->imageData;
	BYTE *buf1 = (BYTE *)m_TargetImg.GetImage()->imageData;

	int WidthPix = WIDTHBYTES(grayImage->width * 8);
	int WidthPix1 = WIDTHBYTES(m_TargetImg.Width() * 24);

	BYTE *buf6 = (BYTE *)m_RenderImg.GetImage()->imageData;
	int WidthPix6 = WIDTHBYTES(m_RenderImg.Width() * 24);

	for(i = LeftRect.top; i < LeftRect.bottom; i++)
	{
		for(j = LeftRect.left; j < LeftRect.right; j++)
		{
			if(LeftEyeRgn.PtInRegion(CPoint(j,i)) && buf[i * WidthPix + j] < 75)
			{
				memset((void *) (buf1 + i * WidthPix1 + 3 * j), 0, 3 * sizeof(BYTE));
				memset((void *) (buf6 + i * WidthPix6 + 3 * j), 0, 3 * sizeof(BYTE));
			}
		}
	}

	for(i = RightRect.top; i < RightRect.bottom; i++)
	{
		for(j = RightRect.left; j < RightRect.right; j++)
		{
			if(RightEyeRgn.PtInRegion(CPoint(j,i)) && buf[i * WidthPix + j] < 75)
			{
				memset((void *) (buf1 + i * WidthPix1 + 3 * j), 0, 3 * sizeof(BYTE));
				memset((void *) (buf6 + i * WidthPix6 + 3 * j), 0, 3 * sizeof(BYTE));
			}
		}
	}
    cvReleaseImage(&grayImage);

	Invalidate(FALSE);
}


void CPortraitEditingDlg::DetectHairCollar()
{
	int i, j;
	BYTE *buf = (BYTE *)m_binaryImg.GetImage()->imageData;
	int WidthPix = WIDTHBYTES(m_binaryImg.Width() * 8);

	for(i = bgRect.top; i < bgRect.bottom; i++)
	{
		for(j = bgRect.left; j < bgRect.right; j++)
		{
			if(bgRgn.PtInRegion(CPoint(j,i)))
			{
				buf[i * WidthPix + j] = 255;
			}
		}
	}

	for(i = skRect.top; i < skRect.bottom; i++)
	{
		for(j = skRect.left; j < skRect.right; j++)
		{
			if(skinRgn.PtInRegion(CPoint(j,i)))
			{
				buf[i * WidthPix + j] = 255;
			}
		}
	}

	for(i = 0; i < m_binaryImg.Height(); i++)
	{
		for(j = 0; j < m_binaryImg.Width(); j++)
		{
			if( i > m_ptArrLandMarks[12].y)
				buf[i * WidthPix + j] = 180;
			if(buf[i * WidthPix + j] == 0)
				buf[i * WidthPix + j] = 90;
			if(buf[i * WidthPix + j] == 255)
				buf[i * WidthPix + j] = 180;
		}
	}

	for(i = 0; i < m_binaryImg.Height(); i++)
	{
		for(j = 0; j < m_binaryImg.Width(); j++)
		{
			if(buf[i * WidthPix + j] == 90)
				buf[i * WidthPix + j] = 255;
			if(buf[i * WidthPix + j] == 180)
				buf[i * WidthPix + j] = 0;
		}
	}

	m_binaryImg.Save(m_sPath + "\\temp\\hair_mask.jpg");
	
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours = 0;
	cvFindContours( m_binaryImg.GetImage(), storage, &contours, sizeof(CvContour),
		CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0,0) );

	contours = cvApproxPoly( contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, 3, 1 );

	IplImage* hairImage = 0;
	hairImage = cvCreateImage(cvGetSize(m_image.GetImage()), IPL_DEPTH_8U, 1);
	ImageChanged(hairImage, 255);
	
	IplImage* targetImageCopy = 0;
	targetImageCopy = cvCreateImage(cvGetSize(m_TargetImg.GetImage()), IPL_DEPTH_8U, 3);
	cvCopy(m_TargetImg.GetImage(), targetImageCopy);

	cvDrawContours(hairImage, contours, CV_RGB(0,0,0), CV_RGB(0,255,0), 2, 1, CV_AA, cvPoint(0,0) );

	BYTE *buf1 = (BYTE *)m_TargetImg.GetImage()->imageData;
	int WidthPix1 = WIDTHBYTES(m_TargetImg.Width() * 24);
	BYTE *buf2 = (BYTE *)hairImage->imageData;
	int WidthPix2 = WIDTHBYTES(hairImage->width * 8);
	BYTE *buf3 = (BYTE *)targetImageCopy->imageData;
	int WidthPix3 = WIDTHBYTES(targetImageCopy->width * 24);

	for(i = 3; i < hairImage->height - 3; i++)
	{
		for(j = 3; j < hairImage->width - 3; j++)
		{
			if(buf3[i * WidthPix3 + 3 * j] == 255 && buf3[i * WidthPix3 + 3 * (j + 1)] == 255 && buf3[i * WidthPix3 + 3 * (j - 1)] == 255 
			&& buf3[i * WidthPix3 + 3 * (j + 2)] == 255 && buf3[i * WidthPix3 + 3 * (j - 2)] == 255 
			&& buf3[i * WidthPix3 + 3 * (j + 3)] == 255 && buf3[i * WidthPix3 + 3 * (j - 3)] == 255 
			&& buf3[(i - 1) * WidthPix3 + 3 * j] == 255	&& buf3[(i + 1) * WidthPix3 + 3 * j] == 255 
			&& buf3[(i - 2) * WidthPix3 + 3 * j] == 255	&& buf3[(i + 2) * WidthPix3 + 3 * j] == 255
			&& buf3[(i - 3) * WidthPix3 + 3 * j] == 255	&& buf3[(i + 3) * WidthPix3 + 3 * j] == 255 && buf2[i * WidthPix2 + j] < 250)
			{
				buf1[i * WidthPix1 + 3 * j] = buf2[i * WidthPix2 + j];
				buf1[i * WidthPix1 + 3 * j + 1] = buf2[i * WidthPix2 + j];
				buf1[i * WidthPix1 + 3 * j + 2] = buf2[i * WidthPix2 + j];
			}
		}
	}



	cvReleaseImage(&hairImage);
	cvReleaseImage(&targetImageCopy);

	Invalidate(FALSE);
	
	///////////////////////////////get hair contour and cloth contour
	// contours = cvApproxPoly( contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, 9, 1 );

	 bool isCloth = false;
	 CvTreeNodeIterator iteratorHair;
	 cvInitTreeNodeIterator( &iteratorHair, contours, 1);

	 while( (contours = (CvSeq*)cvNextTreeNode( &iteratorHair )) != 0 )
	 {
		 CvSeqReader reader;
		 int count = contours->total;
		 cvStartReadSeq( contours, &reader, 0 );
		 CvPoint pt1;
		 if(count >= 8)
		 {
			 CPoint *pHairPoints = new CPoint[count];
			 
			 if(isCloth == false)
			 {
				 hairVectorX.clear();
				 hairVectorY.clear();

				 for(i = 0; i < count; i++ )
				 {
					 CV_READ_SEQ_ELEM( pt1, reader );
					 
					 pHairPoints[i].x = pt1.x;
					 pHairPoints[i].y = pt1.y;
					 hairVectorX.push_back(pt1.x);
					 hairVectorY.push_back(pt1.y);

					 //cvCircle(m_TargetImg.GetImage(), pt1, 4, CV_RGB(255,0,0), 2, CV_AA, 0);
				 }
				 hairRgn.DeleteObject();
				 hairRgn.CreatePolygonRgn(pHairPoints, count, WINDING);
				 hairRgn.GetRgnBox(hairRect);
			 }
			 else
			 {
				 clothVectorX.clear();
				 clothVectorY.clear();

				 for(i = 0; i < count; i++ )
				 {
					 CV_READ_SEQ_ELEM( pt1, reader );
					 
					 pHairPoints[i].x = pt1.x;
					 pHairPoints[i].y = pt1.y;
					 clothVectorX.push_back(pt1.x);
					 clothVectorY.push_back(pt1.y);
				 }
				 clothRgn.DeleteObject();
				 clothRgn.CreatePolygonRgn(pHairPoints, count, WINDING);
				 clothRgn.GetRgnBox(clothRect);
			 }
		
		

			 if(hairRect.top < m_ptArrLandMarks[25].y)
			 {
				 isCloth = true;
			 }
			 else
			 {
				 clothVectorX = hairVectorX;
				 clothVectorY = hairVectorY;
				 clothRgn.DeleteObject();
				 clothRgn.CreatePolygonRgn(pHairPoints, count, WINDING);
				 clothRgn.GetRgnBox(clothRect);
			 }
				

			 delete [] pHairPoints;
		 }	
	 }
}

void CPortraitEditingDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_nStatus == scissors)
	{
		m_image.Load(strImgName);
		ImageZoom(m_image);
		m_image.Save(m_sPath + "\\temp" + "\\input.bmp");
		
		m_TargetImg.Destroy();
		m_TargetImg.Create(m_image.Width(), m_image.Height(), 24);
		ImageChanged(m_TargetImg.GetImage(), 255);
		m_pCurImg.ImageClear();
		m_pCurImg.ReadFromFile(m_sPath + "\\temp" + "\\input.bmp");

		scissorsRect.right = scissorsRect.left;
		scissorsRect.bottom = scissorsRect.top;
		
		Invalidate(FALSE);
	}

	
	CDialog::OnRButtonDown(nFlags, point);
}


void CPortraitEditingDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_nStatus == scissors)
	{
		CWnd *pWnd=GetDlgItem(IDC_SOURCE_IMG);
		CPoint pntImg = point;
		::ClientToScreen(this->m_hWnd, &pntImg);
		::ScreenToClient(pWnd->m_hWnd, &pntImg);

		int sourX = (int)((pntImg.x - m_ptLeftTop.x) * 1.0 / m_dScale);
		int sourY = (int)((pntImg.y - m_ptLeftTop.y) * 1.0 / m_dScale);
		if(sourX < 0 || sourX > m_image.Width() || sourY < 0 || sourY > m_image.Height())
		{
			if(scissorsRect.right > scissorsRect.left && scissorsRect.bottom > scissorsRect.top)
			{
				CRect tempRect;
				tempRect.left = (int)((scissorsRect.left - m_ptLeftTop.x) * 1.0 / m_dScale);
				tempRect.top = (int)((scissorsRect.top - m_ptLeftTop.y) * 1.0 / m_dScale);
				tempRect.right = (int)((scissorsRect.right - m_ptLeftTop.x) * 1.0 / m_dScale);
				tempRect.bottom = (int)((scissorsRect.bottom - m_ptLeftTop.y) * 1.0 / m_dScale);

				CvvImage tempImage;
				tempImage.Create(m_image.Width(), m_image.Height(), 24);
				cvCopy(m_image.GetImage(), tempImage.GetImage());
				m_image.Destroy();
				m_image.Create(tempRect.Width(), tempRect.Height(), 24);

			
				BYTE *buf1 = (BYTE *)m_image.GetImage()->imageData;
				BYTE *buf2 = (BYTE *)tempImage.GetImage()->imageData;
				int WidthPix1 = WIDTHBYTES(m_image.Width() * 24);
				int WidthPix2 = WIDTHBYTES(tempImage.Width() * 24);

				for(int i = 0; i < m_image.Height(); i++)
				{
					for(int j = 0; j < m_image.Width(); j++)
					{
						buf1[i * WidthPix1 + 3 * j] = buf2[(i + tempRect.top) * WidthPix2 + 3 * (j + tempRect.left)];
						buf1[i * WidthPix1 + 3 * j + 1] = buf2[(i + tempRect.top) * WidthPix2 + 3 * (j + tempRect.left) + 1]; 
						buf1[i * WidthPix1 + 3 * j + 2] = buf2[(i + tempRect.top) * WidthPix2 + 3 * (j + tempRect.left) + 2];
					}
				}
         
				ImageZoom(m_image);
				m_image.Save(m_sPath + "\\temp" + "\\input.bmp");
				
				m_TargetImg.Destroy();
				m_TargetImg.Create(m_image.Width(), m_image.Height(), 24);
				ImageChanged(m_TargetImg.GetImage(), 255);
				m_pCurImg.ImageClear();
				m_pCurImg.ReadFromFile(m_sPath + "\\temp" + "\\input.bmp");

				scissorsRect.right = scissorsRect.left;
				scissorsRect.bottom = scissorsRect.top;
				
				Invalidate(FALSE);		
			}
		}
	}

	CDialog::OnLButtonDblClk(nFlags, point);
}

void CPortraitEditingDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bDrag = true;
	m_bDown = true;

	if(m_nStatus == feature || m_nStatus == background || m_nStatus == skin || m_nStatus == hair || m_nStatus == collar)
	{
		CWnd *pWnd = GetDlgItem(IDC_SOURCE_IMG);
//		CDC *pDC = pWnd->GetDC();
		CPoint pntImg(point);
		::ClientToScreen(this->m_hWnd, &pntImg);
		::ScreenToClient(pWnd->m_hWnd, &pntImg);

		int sourX = (int)((pntImg.x - m_ptLeftTop.x) * 1.0 / m_dScale);
		int sourY = (int)((pntImg.y - m_ptLeftTop.y) * 1.0 / m_dScale);
		if(sourX >= 0 && sourX < m_image.Width() && sourY >= 0 && sourY < m_image.Height() && m_bRectSelected == false)
		{
			::SetCursor(::LoadCursor(NULL,IDC_CROSS));

//			COLORREF XorColor = pDC->GetBkColor() ^ RGB(255,0,0);
//			CPen pen(PS_DASH, 1,  XorColor);
//			int oldmode = pDC->GetROP2();
//
//			CPen* oldpen = pDC->SelectObject(&pen);
//			//设置为“空”刷，即不填充。
//			CBrush* oldbrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
//			pDC->SetROP2(R2_XORPEN);
//			
//			//删除原图形
//			pDC->Rectangle(scissorsRect);
//		
//			//以下恢复DC
//			pDC->SetROP2(oldmode);
//			pDC->SelectObject(oldpen);
//			pen.DeleteObject();

			m_bStart = true;
		}

		scissorsRect.left = pntImg.x;
		scissorsRect.top = pntImg.y;	
		scissorsRect.right = pntImg.x;
		scissorsRect.bottom = pntImg.y;

		if(m_bRectSelected == true)
		{
			m_bRectMove = true;
		}
	}

	if(m_nStatus == graphcutFore || m_nStatus == graphcutBack)
	{
		CWnd *pWnd = GetDlgItem(IDC_SOURCE_IMG);
		CPoint pntImg(point);
		::ClientToScreen(this->m_hWnd, &pntImg);
		::ScreenToClient(pWnd->m_hWnd, &pntImg);

		int sourX = (int)((pntImg.x - m_ptLeftTop.x) * 1.0 / m_dScale);
		int sourY = (int)((pntImg.y - m_ptLeftTop.y) * 1.0 / m_dScale);
		if(sourX >= 0 && sourX < m_image.Width() && sourY >= 0 && sourY < m_image.Height())
		{
			::SetCursor(::LoadCursor(NULL,IDC_SIZEALL));
			
			oldPoint = pntImg;

			m_bStart = true;
		}
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CPortraitEditingDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bImgOpened)
	{
		CWnd *pWnd = GetDlgItem(IDC_SOURCE_IMG);
		CPoint pntImg(point);
		::ClientToScreen(this->m_hWnd, &pntImg);
		::ScreenToClient(pWnd->m_hWnd, &pntImg);

		CPoint pointback;
		pointback.x = pntImg.x;
		pointback.y = pntImg.y;

		pntImg.x = (long)((pntImg.x - m_ptLeftTop.x) * 1.0 / m_dScale);
		pntImg.y = (long)((pntImg.y - m_ptLeftTop.y) * 1.0 / m_dScale);
		
		m_bDrag = false;
		m_bDown = false;

		
		BYTE *pbuf = m_pCurImg.m_pData;
		BYTE *buf1 = (BYTE *)m_image.GetImage()->imageData;
		BYTE *buf2 = (BYTE *)m_TargetImg.GetImage()->imageData;
		int WidthPix1 = WIDTHBYTES(m_image.Width() * 24);
		int WidthPix2 = WIDTHBYTES(m_TargetImg.Width() * 24);

		if((m_nStatus == feature || m_nStatus == background || m_nStatus == skin || m_nStatus == hair || m_nStatus == collar) && m_bStart == true)
		{
			if (scissorsRect.left > pointback.x )
			{	
				scissorsRect.right = scissorsRect.left;
				scissorsRect.left = pointback.x;
			}
			else
			{
				scissorsRect.right = pointback.x;
			}

			if (scissorsRect.top > pointback.y)
			{		
				scissorsRect.bottom = scissorsRect.top;
				scissorsRect.top = pointback.y;
			}
			else
			{
				scissorsRect.bottom = pointback.y;
			}

		
			if(scissorsRect.right > m_image.Width()  * m_dScale + m_ptLeftTop.x || scissorsRect.bottom > m_image.Height() * m_dScale + m_ptLeftTop.y
			 ||scissorsRect.left < m_ptLeftTop.x || scissorsRect.top < m_ptLeftTop.y)
			{
				CDC *pDC = pWnd->GetDC();
				COLORREF XorColor = pDC->GetBkColor() ^ RGB(255,0,0);
				CPen pen(PS_DASH, 1,  XorColor);
				int oldmode = pDC->GetROP2();

				CPen* oldpen = pDC->SelectObject(&pen);
				//设置为“空”刷，即不填充。
				CBrush* oldbrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

				pDC->SetROP2(R2_XORPEN);
				//删除原图形
				pDC->Rectangle(scissorsRect);
				//更改角点
				if(scissorsRect.left <  m_ptLeftTop.x)
					scissorsRect.left = m_ptLeftTop.x; 
				if(scissorsRect.top < m_ptLeftTop.y)
					scissorsRect.top = m_ptLeftTop.y;
				if(scissorsRect.right > m_image.Width()  * m_dScale + m_ptLeftTop.x)
					scissorsRect.right = m_image.Width() * m_dScale + m_ptLeftTop.x; 
				if(scissorsRect.bottom > m_image.Height() * m_dScale + m_ptLeftTop.y)
					scissorsRect.bottom = m_image.Height() * m_dScale + m_ptLeftTop.y;
				//绘新图形
				//pDC->Rectangle(scissorsRect);
				//以下恢复DC
				pDC->SetROP2(oldmode);
				pDC->SelectObject(oldpen);
				pen.DeleteObject();
				pWnd->ReleaseDC(pDC);
			}

			if(m_nStatus == feature)
			{
				for(int i = 0; i < 89; i++)
				{
					int nXX = (int) (m_dpLandMarks[i].x * m_dScale + m_ptLeftTop.x);
					int nYY = (int) (m_dpLandMarks[i].y * m_dScale + m_ptLeftTop.y);

					if(scissorsRect.PtInRect(CPoint(nXX, nYY)))
					{
						 if(i == 70 || i == 73 || i == 76 || i == 79 || i == 82 || i == 85 || i == 88
						 || i == 16 || i == 20 || i == 21 || i == 25 || i == 26 || i == 30 || i == 31 || i == 35
						 || i == 0 || i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14
						 || i == 36 || i == 39 || i == 40 || i == 43 || i == 44 || i == 47
						 || i == 48 || i == 50 || i == 51 || i == 52 || i == 54 || i == 57)
						 {
							m_nSelectedVector.push_back(i);
							Invalidate(FALSE);

							m_bRectSelected = true;
						 }
					}
				}

				if(m_bRectSelected == true)
				{
					for(int i = 0; i < 91; i++)
					{
						m_dpLandMarksCopy[i].x = m_dpLandMarks[i].x;
						m_dpLandMarksCopy[i].y = m_dpLandMarks[i].y;
					}
				}
			}

			if(m_nStatus == background)
			{
				for(int i = 0; i < m_backgroundVector.size(); i++)
				{
					int nXX = (int) (m_backgroundVector[i].x * m_dScale + m_ptLeftTop.x);
					int nYY = (int) (m_backgroundVector[i].y * m_dScale + m_ptLeftTop.y);

					if(scissorsRect.PtInRect(CPoint(nXX, nYY)))
					{
						m_nSelectedVector.push_back(i);
						Invalidate(FALSE);

						m_bRectSelected = true;
					}
				}
			}

			if(m_nStatus == skin)
			{
				for(int i = 0; i < m_skinVector.size(); i++)
				{
					int nXX = (int) (m_skinVector[i].x * m_dScale + m_ptLeftTop.x);
					int nYY = (int) (m_skinVector[i].y * m_dScale + m_ptLeftTop.y);

					if(scissorsRect.PtInRect(CPoint(nXX, nYY)))
					{
						m_nSelectedVector.push_back(i);
						Invalidate(FALSE);

						m_bRectSelected = true;
					}
				}

				if(m_bRectSelected == true)
				{
					m_skinVectorCopy = m_skinVector;
				}
			}

			if(m_nStatus == hair)
			{
				for(int i = 0; i < hairVectorX.size(); i++)
				{
					int nXX = (int) (hairVectorX[i] * m_dScale + m_ptLeftTop.x);
					int nYY = (int) (hairVectorY[i] * m_dScale + m_ptLeftTop.y);

					if(scissorsRect.PtInRect(CPoint(nXX, nYY)))
					{
						m_nSelectedVector.push_back(i);
						Invalidate(FALSE);

						m_bRectSelected = true;
					}
				}

				if(m_bRectSelected == true)
				{
					vectorXCopy = hairVectorX;
					vectorYCopy = hairVectorY;
				}
			}

			if(m_nStatus == collar)
			{
				for(int i = 0; i < clothVectorX.size(); i++)
				{
					int nXX = (int) (clothVectorX[i] * m_dScale + m_ptLeftTop.x);
					int nYY = (int) (clothVectorY[i] * m_dScale + m_ptLeftTop.y);

					if(scissorsRect.PtInRect(CPoint(nXX, nYY)))
					{
						m_nSelectedVector.push_back(i);
						Invalidate(FALSE);

						m_bRectSelected = true;
					}
				}

				if(m_bRectSelected == true)
				{
					vectorXCopy = clothVectorX;
					vectorYCopy = clothVectorY;
				}
			}
		
			m_bStart = false;
		}

		if((m_nStatus == graphcutFore || m_nStatus == graphcutBack) && m_bStart == true)
		{
			m_bStart = false;
		}

		if(m_nStatus == nColor)
		{
			int nX = pntImg.x - (int)(343.92 / m_dScale);
			int nY = pntImg.y;
			if(nX >= 0 && nX < m_TargetImg.Width() && nY >= 0 && nY < m_TargetImg.Height())
			{
				m_nB = buf2[nY * WidthPix2 + 3 * nX];
				m_nG = buf2[nY * WidthPix2 + 3 * nX + 1];
				m_nR = buf2[nY * WidthPix2 + 3 * nX + 2];

				m_nStatus = 0;
			}
		}

	
		if(m_nStatus == feature)
		{
			if(m_nSelectedFeature >= 0)
			{
				ConnectLandmark91(CV_RGB(255,255,255), 2);

				m_dpLandMarks[m_nSelectedFeature].x = pntImg.x;
				m_dpLandMarks[m_nSelectedFeature].y = pntImg.y;
				m_vbConstrained[m_nSelectedFeature] = true;
				m_ConstrainShape.PointX(m_nSelectedFeature) = pntImg.x;
				m_ConstrainShape.PointY(m_nSelectedFeature) = pntImg.y;

				ConnectLandmark91(CV_RGB(0,0,0), 1);
				DrawEyeball();
			}
              
			if(m_bRectMove == true)
			{
				dPoint tempLandMarks[91];
				for(int i = 0; i < 91; i++)
				{
					tempLandMarks[i].x = m_dpLandMarks[i].x;
					tempLandMarks[i].y = m_dpLandMarks[i].y;
				}

				for(i = 0; i < 91; i++)
				{
                    m_dpLandMarks[i].x = m_dpLandMarksCopy[i].x;
					m_dpLandMarks[i].y = m_dpLandMarksCopy[i].y;
				}
				ConnectLandmark91(CV_RGB(255,255,255), 2);
				
				for(i = 0; i < 91; i++)
				{
					m_dpLandMarks[i].x = tempLandMarks[i].x;
					m_dpLandMarks[i].y = tempLandMarks[i].y;
				}
				ConnectLandmark91(CV_RGB(0,0,0), 1);
				DrawEyeball();

				for(i = 0; i < m_nSelectedVector.size(); i++)
				{
					m_vbConstrained[m_nSelectedVector[i]] = true;
					m_ConstrainShape.PointX(m_nSelectedVector[i]) = m_dpLandMarks[m_nSelectedVector[i]].x;
					m_ConstrainShape.PointY(m_nSelectedVector[i]) = m_dpLandMarks[m_nSelectedVector[i]].y;
				}

				m_nSelectedVector.clear();
				m_bRectSelected = false;
				m_bRectMove = false;
			}
			
			m_TargetImgCopy.Create(m_TargetImg.Width(), m_TargetImg.Height(), 24);
			cvCopy(m_TargetImg.GetImage(), m_TargetImgCopy.GetImage());
			
			
			Invalidate(FALSE);
		}

		if(m_nStatus == background)
		{
			if(m_nSelectedFeature >= 0)
			{	
				m_backgroundVector[m_nSelectedFeature].x = pntImg.x;
				m_backgroundVector[m_nSelectedFeature].y = pntImg.y;
			}
		
			if(m_bRectMove == true)
			{
				m_nSelectedVector.clear();
				m_bRectSelected = false;
				m_bRectMove = false;
			}


			int nNum = m_backgroundVector.size();
			CPoint *points = new CPoint[nNum];
			for(int k = 0; k < nNum; k++)
			{
				points[k].x = m_backgroundVector[k].x;
				points[k].y = m_backgroundVector[k].y;
			}

			bgRgn.DeleteObject();
			bgRgn.CreatePolygonRgn(points, nNum, WINDING);
			bgRgn.GetRgnBox(bgRect);

			delete [] points;

			Invalidate(FALSE);
		}

		if(m_nStatus == skin)
		{
			if(m_nSelectedFeature >= 0)
			{
				CvPoint pt1, pt2, pt3;
				int last = m_nSelectedFeature - 1;
				int next = m_nSelectedFeature + 1;
				if(last < 0)
					last = m_skinVector.size() - 1;
				if(next > m_skinVector.size() - 1)
					next = 0;
				pt1.x = m_skinVector[last].x;
				pt1.y = m_skinVector[last].y;
				pt2.x = m_skinVector[m_nSelectedFeature].x;
				pt2.y = m_skinVector[m_nSelectedFeature].y;
				pt3.x = m_skinVector[next].x;
				pt3.y = m_skinVector[next].y;
				cvLine(m_TargetImg.GetImage(), pt1, pt2, CV_RGB(255,255,255), 2, CV_AA);
				cvLine(m_TargetImg.GetImage(), pt2, pt3, CV_RGB(255,255,255), 2, CV_AA);
			
				m_skinVector[m_nSelectedFeature].x = pntImg.x;
				m_skinVector[m_nSelectedFeature].y = pntImg.y;

				pt2.x = m_skinVector[m_nSelectedFeature].x;
				pt2.y = m_skinVector[m_nSelectedFeature].y;
				cvLine(m_TargetImg.GetImage(), pt1, pt2, CV_RGB(0,0,0), 1, CV_AA);
				cvLine(m_TargetImg.GetImage(), pt2, pt3, CV_RGB(0,0,0), 1, CV_AA);
			}

			if(m_bRectMove == true)
			{
				vector<dPoint> tempVector;
                tempVector = m_skinVector;
				m_skinVector = m_skinVectorCopy;
				ConncetContour(m_skinVector, m_TargetImg.GetImage(), CV_RGB(255,255,255), 2, true);
				m_skinVector = tempVector;
				vector<double> faceVectorX, faceVectorY;
				for(int i = 0; i < 19; i++)
				{
					faceVectorX.push_back(m_dpLandMarks[70 + i].x);
					faceVectorY.push_back(m_dpLandMarks[70 + i].y);
				}
				ConncetContour(faceVectorX, faceVectorY, m_TargetImg.GetImage(), CV_RGB(0,0,0), 1, true);
				ConncetContour(m_skinVector, m_TargetImg.GetImage(), CV_RGB(0,0,0), 1, true);
			
				m_nSelectedVector.clear();
				m_bRectSelected = false;
				m_bRectMove = false;
			}
			
			m_TargetImgCopy.Create(m_TargetImg.Width(), m_TargetImg.Height(), 24);
			cvCopy(m_TargetImg.GetImage(), m_TargetImgCopy.GetImage());

			int nNum = m_skinVector.size();
			CPoint *points = new CPoint[nNum];
			for(int k = 0; k < nNum; k++)
			{
				points[k].x = m_skinVector[k].x;
				points[k].y = m_skinVector[k].y;
			}

			skinRgn.DeleteObject();
			skinRgn.CreatePolygonRgn(points, nNum, WINDING);
			skinRgn.GetRgnBox(skRect);

			delete [] points;

			Invalidate(FALSE);
		}

		if(m_nStatus == ear)
		{
			if(m_bLeftEar == true)
			{
				leftEarVector[m_nSelectedFeature].x = pntImg.x;
				leftEarVector[m_nSelectedFeature].y = pntImg.y;

				int nNum = leftEarVector.size();
				CPoint *points = new CPoint[nNum];
				for(int k = 0; k < nNum; k++)
				{
					points[k].x = leftEarVector[k].x;
					points[k].y = leftEarVector[k].y;
				}

				leftEarRgn.DeleteObject();
				leftEarRgn.CreatePolygonRgn(points, nNum, WINDING);
				leftEarRgn.GetRgnBox(leftEarRect);

				delete [] points;
			}

			if(m_bRightEar == true)
			{
				rightEarVector[m_nSelectedFeature].x = pntImg.x;
				rightEarVector[m_nSelectedFeature].y = pntImg.y;
	
				int nNum = rightEarVector.size();
				CPoint *points = new CPoint[nNum];
				for(int k = 0; k < nNum; k++)
				{
					points[k].x = rightEarVector[k].x;
					points[k].y = rightEarVector[k].y;
				}

				rightEarRgn.DeleteObject();
				rightEarRgn.CreatePolygonRgn(points, nNum, WINDING);
				rightEarRgn.GetRgnBox(rightEarRect);

				delete [] points;
			}
					
			Invalidate(FALSE);
		}

		if(m_nStatus == hair)
		{
			if(m_nSelectedFeature >= 0)
			{
				ConncetContour(hairVectorX, hairVectorY, m_TargetImg.GetImage(), CV_RGB(255,255,255), 2);

				hairVectorX[m_nSelectedFeature] = pntImg.x;
				hairVectorY[m_nSelectedFeature] = pntImg.y;

				ConncetContour(hairVectorX, hairVectorY, m_TargetImg.GetImage(), CV_RGB(0,0,0), 1);
			}

			if(m_bRectMove == true)
			{
				vector<double> tempVectorX, tempVectorY;
                tempVectorX = hairVectorX;
				tempVectorY = hairVectorY;

				hairVectorX = vectorXCopy;
				hairVectorY = vectorYCopy;
				ConncetContour(hairVectorX, hairVectorY, m_TargetImg.GetImage(), CV_RGB(255,255,255), 2);

				hairVectorX = tempVectorX;
				hairVectorY = tempVectorY;
				ConncetContour(hairVectorX, hairVectorY, m_TargetImg.GetImage(), CV_RGB(0,0,0), 1);
			
				m_nSelectedVector.clear();
				m_bRectSelected = false;
				m_bRectMove = false;
			}

			m_TargetImgCopy.Create(m_TargetImg.Width(), m_TargetImg.Height(), 24);
			cvCopy(m_TargetImg.GetImage(), m_TargetImgCopy.GetImage());

			int nNum = hairVectorX.size();
			CPoint *points = new CPoint[nNum];
			for(int k = 0; k < nNum; k++)
			{
				points[k].x = hairVectorX[k];
				points[k].y = hairVectorY[k];
			}

			hairRgn.DeleteObject();
			hairRgn.CreatePolygonRgn(points, nNum, WINDING);
			hairRgn.GetRgnBox(hairRect);

			delete [] points;

			Invalidate(FALSE);
		}

		if(m_nStatus == collar)
		{
			if(m_nSelectedFeature >= 0)
			{
				ConncetContour(clothVectorX, clothVectorY, m_TargetImg.GetImage(), CV_RGB(255,255,255), 2);

				clothVectorX[m_nSelectedFeature] = pntImg.x;
				clothVectorY[m_nSelectedFeature] = pntImg.y;

				ConncetContour(clothVectorX, clothVectorY, m_TargetImg.GetImage(), CV_RGB(0,0,0), 1);
			}

			if(m_bRectMove == true)
			{
				vector<double> tempVectorX, tempVectorY;
                tempVectorX = clothVectorX;
				tempVectorY = clothVectorY;

				clothVectorX = vectorXCopy;
				clothVectorY = vectorYCopy;
				ConncetContour(clothVectorX, clothVectorY, m_TargetImg.GetImage(), CV_RGB(255,255,255), 2);

				clothVectorX = tempVectorX;
				clothVectorY = tempVectorY;
				ConncetContour(clothVectorX, clothVectorY, m_TargetImg.GetImage(), CV_RGB(0,0,0), 1);
			
				m_nSelectedVector.clear();
				m_bRectSelected = false;
				m_bRectMove = false;
			}
	
			m_TargetImgCopy.Create(m_TargetImg.Width(), m_TargetImg.Height(), 24);
			cvCopy(m_TargetImg.GetImage(), m_TargetImgCopy.GetImage());

			int nNum = clothVectorX.size();
			CPoint *points = new CPoint[nNum];
			for(int k = 0; k < nNum; k++)
			{
				points[k].x = clothVectorX[k];
				points[k].y = clothVectorY[k];
			}

			clothRgn.DeleteObject();
			clothRgn.CreatePolygonRgn(points, nNum, WINDING);
			clothRgn.GetRgnBox(clothRect);

			delete [] points;

			Invalidate(FALSE);
		}
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CPortraitEditingDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bImgOpened)
	{
		CWnd *pWnd=GetDlgItem(IDC_SOURCE_IMG);
		CDC *pDC = pWnd->GetDC();
		CPoint pntImg = point;
		::ClientToScreen(this->m_hWnd, &pntImg);
		::ScreenToClient(pWnd->m_hWnd, &pntImg);
		CRect rectImg;
		pWnd->GetWindowRect(rectImg);
		rectImg.CopyRect( CRect(0, 0, rectImg.Width(), rectImg.Height() ) );

		int tarX = (int)((pntImg.x - m_ptLeftTop.x) * 1.0 / m_dScale - (343.92 / m_dScale));
		int tarY = (int)((pntImg.y - m_ptLeftTop.y) * 1.0 / m_dScale);

		if(m_nStatus == feature || m_nStatus == background || m_nStatus == skin || m_nStatus == hair || m_nStatus == collar)
		{
			int sourX = (int)((pntImg.x - m_ptLeftTop.x) * 1.0 / m_dScale);
			int sourY = (int)((pntImg.y - m_ptLeftTop.y) * 1.0 / m_dScale);
			if(sourX >= 0 && sourX < m_image.Width() && sourY >= 0 && sourY < m_image.Height())
			{
				::SetCursor(::LoadCursor(NULL,IDC_CROSS));
			}

			if(m_bStart == true)
			{
				COLORREF XorColor = pDC->GetBkColor() ^ RGB(255,0,0);
				CPen pen(PS_DASH, 1,  XorColor);
				int oldmode = pDC->GetROP2();

				CPen* oldpen = pDC->SelectObject(&pen);
				//设置为“空”刷，即不填充。
				CBrush* oldbrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

				pDC->SetROP2(R2_XORPEN);
				//删除原图形
				pDC->Rectangle(scissorsRect);
				//更改角点
				scissorsRect.right = pntImg.x;
				scissorsRect.bottom = pntImg.y;
				//绘新图形
				pDC->Rectangle(scissorsRect);
				//以下恢复DC
				pDC->SetROP2(oldmode);
				pDC->SelectObject(oldpen);
				pen.DeleteObject();
			}
			
			if(m_bRectSelected == true && m_bRectMove == true)
			{
				int diffX = pntImg.x - scissorsRect.left;
				int diffY = pntImg.y - scissorsRect.top;

				if(m_nStatus == feature)
				{
					for(int i = 0; i < m_nSelectedVector.size(); i++)
					{
						m_dpLandMarks[m_nSelectedVector[i]].x += diffX;
						m_dpLandMarks[m_nSelectedVector[i]].y += diffY;

						if(m_dpLandMarks[m_nSelectedVector[i]].x < 0)
							m_dpLandMarks[m_nSelectedVector[i]].x = 0;
						if(m_dpLandMarks[m_nSelectedVector[i]].x > m_image.Width())
							m_dpLandMarks[m_nSelectedVector[i]].x = m_image.Width() - 1;
						if(m_dpLandMarks[m_nSelectedVector[i]].y < 0)
							m_dpLandMarks[m_nSelectedVector[i]].y = 0;
						if(m_dpLandMarks[m_nSelectedVector[i]].y > m_image.Height())
							m_dpLandMarks[m_nSelectedVector[i]].y = m_image.Height() - 1;
					}
				}

				if(m_nStatus == background)
				{
					for(int i = 0; i < m_nSelectedVector.size(); i++)
					{
						m_backgroundVector[m_nSelectedVector[i]].x += diffX;
						m_backgroundVector[m_nSelectedVector[i]].y += diffY;

						if(m_backgroundVector[m_nSelectedVector[i]].x < 0)
							m_backgroundVector[m_nSelectedVector[i]].x = 0;
						if(m_backgroundVector[m_nSelectedVector[i]].x > m_image.Width())
							m_backgroundVector[m_nSelectedVector[i]].x = m_image.Width() - 1;
						if(m_backgroundVector[m_nSelectedVector[i]].y < 0)
							m_backgroundVector[m_nSelectedVector[i]].y = 0;
						if(m_backgroundVector[m_nSelectedVector[i]].y > m_image.Height())
							m_backgroundVector[m_nSelectedVector[i]].y = m_image.Height() - 1;
					}
				}

				if(m_nStatus == skin)
				{
					for(int i = 0; i < m_nSelectedVector.size(); i++)
					{
						m_skinVector[m_nSelectedVector[i]].x += diffX;
						m_skinVector[m_nSelectedVector[i]].y += diffY;

						if(m_skinVector[m_nSelectedVector[i]].x < 0)
							m_skinVector[m_nSelectedVector[i]].x = 0;
						if(m_skinVector[m_nSelectedVector[i]].x > m_image.Width())
							m_skinVector[m_nSelectedVector[i]].x = m_image.Width() - 1;
						if(m_skinVector[m_nSelectedVector[i]].y < 0)
							m_skinVector[m_nSelectedVector[i]].y = 0;
						if(m_skinVector[m_nSelectedVector[i]].y > m_image.Height())
							m_skinVector[m_nSelectedVector[i]].y = m_image.Height() - 1;
					}
				}

				if(m_nStatus == hair)
				{
					for(int i = 0; i < m_nSelectedVector.size(); i++)
					{
						hairVectorX[m_nSelectedVector[i]] += diffX;
						hairVectorY[m_nSelectedVector[i]] += diffY;

						if(hairVectorX[m_nSelectedVector[i]] < 0)
							hairVectorX[m_nSelectedVector[i]] = 0;
						if(hairVectorX[m_nSelectedVector[i]] > m_image.Width())
							hairVectorX[m_nSelectedVector[i]] = m_image.Width() - 1;
						if(hairVectorY[m_nSelectedVector[i]] < 0)
							hairVectorY[m_nSelectedVector[i]] = 0;
						if(hairVectorY[m_nSelectedVector[i]] > m_image.Height())
							hairVectorY[m_nSelectedVector[i]] = m_image.Height() - 1;
					}
				}

				if(m_nStatus == collar)
				{
					for(int i = 0; i < m_nSelectedVector.size(); i++)
					{
						clothVectorX[m_nSelectedVector[i]] += diffX;
						clothVectorY[m_nSelectedVector[i]] += diffY;

						if(clothVectorX[m_nSelectedVector[i]] < 0)
							clothVectorX[m_nSelectedVector[i]] = 0;
						if(clothVectorX[m_nSelectedVector[i]] > m_image.Width())
							clothVectorX[m_nSelectedVector[i]] = m_image.Width() - 1;
						if(clothVectorY[m_nSelectedVector[i]] < 0)
							clothVectorY[m_nSelectedVector[i]] = 0;
						if(clothVectorY[m_nSelectedVector[i]] > m_image.Height())
							clothVectorY[m_nSelectedVector[i]] = m_image.Height() - 1;
					}
				}

				Invalidate(FALSE);

				scissorsRect.left = pntImg.x;
				scissorsRect.top = pntImg.y;
			}
		}

		if(m_nStatus == graphcutFore || m_nStatus == graphcutBack)
		{
			int sourX = (int)((pntImg.x - m_ptLeftTop.x) * 1.0 / m_dScale);
			int sourY = (int)((pntImg.y - m_ptLeftTop.y) * 1.0 / m_dScale);
			BYTE *buf = (BYTE *)m_image.GetImage()->imageData;
			int WidthPix = WIDTHBYTES(m_image.Width() * 24);
			if(sourX >= 1 && sourX < m_image.Width() - 1 && sourY >= 1 && sourY < m_image.Height() - 1)
			{
				::SetCursor(::LoadCursor(NULL,IDC_SIZEALL));
				if(m_bStart == true)
				{
					CPoint curPoint;
					curPoint.x = sourX;
					curPoint.y = sourY;

					Ptposition tempPt;
					if(m_nStatus == graphcutFore)
					{
						CPen RedPen(PS_SOLID, 3, RGB(255, 0, 0) );
						CPen *pOldPen = pDC->SelectObject(&RedPen);
						pDC->MoveTo(oldPoint);
						pDC->LineTo(pntImg);
						pDC->SelectObject(pOldPen);

						for(int i = -1; i <= 1; i++)
						{
							for(int j = -1; j <= 1; j++)
							{
								tempPt.x = sourX + j;
								tempPt.y = sourY + i;
								FrontStroke.push_back(tempPt);
							}
						}
					}

					if(m_nStatus == graphcutBack)
					{
						CPen GreenPen(PS_SOLID, 3, RGB(0, 255, 0) );
						CPen *pOldPen = pDC->SelectObject(&GreenPen);
						pDC->MoveTo(oldPoint);
						pDC->LineTo(pntImg);
						pDC->SelectObject(pOldPen);

						
						for(int i = -1; i <= 1; i++)
						{
							for(int j = -1; j <= 1; j++)
							{
								tempPt.x = sourX + j;
								tempPt.y = sourY + i;
								BackStroke.push_back(tempPt);
							}
						}
					}
				
					oldPoint = pntImg;
				}
			}
		}

		if (m_bFeatureLocalized == true)
		{
			if(m_nStatus == nColor || m_nStatus == nBrush)
			{
				if(tarX >= 0 && tarX < m_TargetImg.Width() && tarY >= 0 && tarY < m_TargetImg.Height())
				{
					::SetCursor(::LoadCursor(NULL, IDC_CROSS));
				}
			}

			BYTE *buf = (BYTE *)m_TargetImg.GetImage()->imageData;
			int WidthPix = WIDTHBYTES(m_TargetImg.Width() * 24);
			if(m_nStatus == nBrush && m_bDown == true)
			{
				if(tarX >= 1 && tarX < m_TargetImg.Width() - 1 && tarY >= 1 && tarY < m_TargetImg.Height() - 1)
				{
					::SetCursor(::LoadCursor(NULL, IDC_CROSS));

					for(int i = -1; i < 2; i++)
					{
						for(int j = -1; j < 2; j++)
						{
							buf[(tarY + i) * WidthPix + 3 * (tarX + j)] = m_nB;
							buf[(tarY + i) * WidthPix + 3 * (tarX + j) + 1] = m_nG;
							buf[(tarY + i) * WidthPix + 3 * (tarX + j) + 2] = m_nR;
						}
					}

					Invalidate(FALSE);
				}
			}

			if(m_bDrag == false && rectImg.PtInRect(pntImg) == TRUE)
			{
				double dCloseLimit = 5;
				double dCloseDis = 100;
				int nClosePnt = -1;

				if(m_nStatus == feature)
				{
					for (int np = 0; np < NUM_OF_LANDMARKS; np++)
					{
//						int nx = (int) (m_ptArrLandMarks[np].x * m_dScale + m_ptLeftTop.x);
//						int ny = (int) (m_ptArrLandMarks[np].y * m_dScale + m_ptLeftTop.y);
						int nx = (int) (m_dpLandMarks[np].x * m_dScale + m_ptLeftTop.x);
						int ny = (int) (m_dpLandMarks[np].y * m_dScale + m_ptLeftTop.y);
						
						double dDis = (nx - pntImg.x) * (nx - pntImg.x) + (ny - pntImg.y) * (ny - pntImg.y);
						if (dDis < dCloseDis)
						{
							dCloseDis = dDis;
							nClosePnt = np;
						}		
					}
				}

				if(m_nStatus == background)
				{
					for (int np = 0; np < m_backgroundVector.size(); np++)
					{
						int nx = (int) (m_backgroundVector[np].x * m_dScale + m_ptLeftTop.x);
						int ny = (int) (m_backgroundVector[np].y * m_dScale + m_ptLeftTop.y);
						
						double dDis = (nx - pntImg.x) * (nx - pntImg.x) + (ny - pntImg.y) * (ny - pntImg.y);
						if (dDis < dCloseDis)
						{
							dCloseDis = dDis;
							nClosePnt = np;
						}		
					}
				}

				if(m_nStatus == skin)
				{
					for (int np = 0; np < m_skinVector.size(); np++)
					{
						int nx = (int) (m_skinVector[np].x * m_dScale + m_ptLeftTop.x);
						int ny = (int) (m_skinVector[np].y * m_dScale + m_ptLeftTop.y);
						
						double dDis = (nx - pntImg.x) * (nx - pntImg.x) + (ny - pntImg.y) * (ny - pntImg.y);
						if (dDis < dCloseDis)
						{
							dCloseDis = dDis;
							nClosePnt = np;
						}		
					}
				}

				if(m_nStatus == ear)
				{
					if(pntImg.x < m_dpLandMarks[79].x * m_dScale + m_ptLeftTop.x)
					{
						m_bLeftEar = true;
						m_bRightEar = false;

						for (int np = 0; np < leftEarVector.size(); np++)
						{
							int nx = (int) (leftEarVector[np].x * m_dScale + m_ptLeftTop.x);
							int ny = (int) (leftEarVector[np].y * m_dScale + m_ptLeftTop.y);
							
							double dDis = (nx - pntImg.x) * (nx - pntImg.x) + (ny - pntImg.y) * (ny - pntImg.y);
							if (dDis < dCloseDis)
							{
								dCloseDis = dDis;
								nClosePnt = np;
							}		
						}
					}

					if(pntImg.x > m_dpLandMarks[79].x * m_dScale + m_ptLeftTop.x)
					{
						m_bRightEar = true;
						m_bLeftEar = false;

						for (int np = 0; np < rightEarVector.size(); np++)
						{
							int nx = (int) (rightEarVector[np].x * m_dScale + m_ptLeftTop.x);
							int ny = (int) (rightEarVector[np].y * m_dScale + m_ptLeftTop.y);
							
							double dDis = (nx - pntImg.x) * (nx - pntImg.x) + (ny - pntImg.y) * (ny - pntImg.y);
							if (dDis < dCloseDis)
							{
								dCloseDis = dDis;
								nClosePnt = np;
							}		
						}
					}
				}

				if(m_nStatus == hair)
				{
					for (int np = 0; np < hairVectorX.size(); np++)
					{
						int nx = (int) (hairVectorX[np] * m_dScale + m_ptLeftTop.x);
						int ny = (int) (hairVectorY[np] * m_dScale + m_ptLeftTop.y);
						
						double dDis = (nx - pntImg.x) * (nx - pntImg.x) + (ny - pntImg.y) * (ny - pntImg.y);
						if (dDis < dCloseDis)
						{
							dCloseDis = dDis;
							nClosePnt = np;
						}		
					}
				}

				if(m_nStatus == collar)
				{
					for (int np = 0; np < clothVectorX.size(); np++)
					{
						int nx = (int) (clothVectorX[np] * m_dScale + m_ptLeftTop.x);
						int ny = (int) (clothVectorY[np] * m_dScale + m_ptLeftTop.y);
						
						double dDis = (nx - pntImg.x) * (nx - pntImg.x) + (ny - pntImg.y) * (ny - pntImg.y);
						if (dDis < dCloseDis)
						{
							dCloseDis = dDis;
							nClosePnt = np;
						}		
					}
				}

				if (dCloseDis < dCloseLimit)
				{
					m_nSelectedFeature = nClosePnt;

					if(m_nStatus == feature)
					{
						 if(nClosePnt == 70 || nClosePnt == 73 || nClosePnt == 76 || nClosePnt == 79 || nClosePnt == 82 || nClosePnt == 85 || nClosePnt == 88
					 || nClosePnt == 16 || nClosePnt == 20 || nClosePnt == 21 || nClosePnt == 25 || nClosePnt == 26 || nClosePnt == 30 || nClosePnt == 31 || nClosePnt == 35
					 || nClosePnt == 0 || nClosePnt == 2 || nClosePnt == 4 || nClosePnt == 6 || nClosePnt == 8 || nClosePnt == 10 || nClosePnt == 12 || nClosePnt == 14
					 || nClosePnt == 36 || nClosePnt == 39 || nClosePnt == 40 || nClosePnt == 43 || nClosePnt == 44 || nClosePnt == 47
					 || nClosePnt == 48 || nClosePnt == 50 || nClosePnt == 51 || nClosePnt == 52 || nClosePnt == 54 || nClosePnt == 57)
								m_nSelectedFeature = nClosePnt;
						 else
							 	m_nSelectedFeature = -1;
					}
					Invalidate(FALSE);
				}
				else
				{
					if (m_nSelectedFeature != -1)
					{
						m_nSelectedFeature = -1;
						Invalidate(FALSE);
					}
				}
			}
		}

		pWnd->ReleaseDC(pDC);
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void CPortraitEditingDlg::LoadFile(CString sFileName)
{
		FILE *fp;
		fp = fopen(sFileName, "r");
		if(fp != NULL)
		{
			int size;
			fscanf(fp, "%d\n", &size);
			char *buf = new char[size+1];
       
			for(int k = 0; k < size+1; k++)
				fscanf(fp, "%c", &buf[k]);

			buf[size] = '\0';
			strImgName.Format("%s", buf);
			delete [] buf;

			int nx, ny;
			fscanf(fp, "%d\n", &size);
			for(k = 0; k < size; k++)
			{
				fscanf(fp, "%d %d\n", &nx, &ny);
				m_ptArrLandMarks[k].x = nx;
				m_ptArrLandMarks[k].y = ny;
			}

			double dx, dy;
			fscanf(fp, "%d\n", &size);
			for(k = 0; k < size; k++)
			{
				fscanf(fp, "%lf %lf\n", &dx, &dy);
				m_dpLandMarks[k].x = dx;
				m_dpLandMarks[k].y = dy;
			}

			fscanf(fp, "%d\n", &size);
			m_backgroundVector.resize(size);
			CPoint *pBackPoints = new CPoint[size];
			for(k = 0; k < size; k++)
			{	
				fscanf(fp, "%lf %lf\n", &dx, &dy);
				m_backgroundVector[k].x = dx;
				m_backgroundVector[k].y = dy;
				pBackPoints[k].x = dx;
				pBackPoints[k].y = dy;
			}
			if(size > 0)
			{
				bgRgn.DeleteObject();
				bgRgn.CreatePolygonRgn(pBackPoints, size, WINDING);
				bgRgn.GetRgnBox(bgRect);
			}
			delete [] pBackPoints;

			fscanf(fp, "%d\n", &size);
			m_skinVector.resize(size);
			CPoint *pSkinPoints = new CPoint[size];
			for(k = 0; k < size; k++)
			{	
				fscanf(fp, "%lf %lf\n", &dx, &dy);
				m_skinVector[k].x = dx;
				m_skinVector[k].y = dy;
				pSkinPoints[k].x = dx;
				pSkinPoints[k].y = dy;
			}
			if(size > 0)
			{
				skinRgn.DeleteObject();
				skinRgn.CreatePolygonRgn(pSkinPoints, size, WINDING);
				skinRgn.GetRgnBox(skRect);
			}
			delete [] pSkinPoints;

			fscanf(fp, "%d\n", &size);
			hairVectorX.resize(size);
			hairVectorY.resize(size);
			CPoint *pHairPoints = new CPoint[size];
			for(k = 0; k < size; k++)
			{	
				fscanf(fp, "%lf %lf\n", &dx, &dy);
				hairVectorX[k] = dx;
				hairVectorY[k] = dy;
				pHairPoints[k].x = dx;
				pHairPoints[k].y = dy;
			}
			if(size > 0)
			{
				hairRgn.DeleteObject();
				hairRgn.CreatePolygonRgn(pHairPoints, size, WINDING);
				hairRgn.GetRgnBox(hairRect);
			}
			delete [] pHairPoints;

			fscanf(fp, "%d\n", &size);
			clothVectorX.resize(size);
			clothVectorY.resize(size);
			CPoint *pClothPoints = new CPoint[size];
			for(k = 0; k < size; k++)
			{
				fscanf(fp, "%lf %lf\n", &dx, &dy);
				clothVectorX[k] = dx;
				clothVectorY[k] = dy;
				pClothPoints[k].x = dx;
				pClothPoints[k].y = dy;
			}
			if(size > 0)
			{
				clothRgn.DeleteObject();
				clothRgn.CreatePolygonRgn(pClothPoints, size, WINDING);
				clothRgn.GetRgnBox(clothRect);
			}
			delete [] pClothPoints;

			fscanf(fp, "%d\n", &size);
			leftEarVector.resize(size);
			CPoint *pLEarPoints = new CPoint[size];
			for(k = 0; k < size; k++)
			{
				fscanf(fp, "%lf %lf\n", &dx, &dy);
				leftEarVector[k].x = dx;
				leftEarVector[k].y = dy;
				pLEarPoints[k].x = dx;
				pLEarPoints[k].y = dy;
			}
			if(size > 0)
			{
				leftEarRgn.DeleteObject();
				leftEarRgn.CreatePolygonRgn(pLEarPoints, size, WINDING);
				leftEarRgn.GetRgnBox(leftEarRect);
			}
			delete [] pLEarPoints;

			fscanf(fp, "%d\n", &size);
			rightEarVector.resize(size);
			CPoint *pREarPoints = new CPoint[size];
			for(k = 0; k < size; k++)
			{
				fscanf(fp, "%lf %lf\n", &dx, &dy);
				rightEarVector[k].x = dx;
				rightEarVector[k].y = dy;
				pREarPoints[k].x = dx;
				pREarPoints[k].y = dy;
			}
			if(size > 0)
			{
				rightEarRgn.DeleteObject();
				rightEarRgn.CreatePolygonRgn(pREarPoints, size, WINDING);
				rightEarRgn.GetRgnBox(rightEarRect);
			}
			delete [] pREarPoints;

			m_bFeatureLocalized = true;
			m_bFeature = true;
			fscanf(fp, "%d\n", &size);
			if(size == 1)
				m_bBackground = true;
			else
				m_bBackground = false;

			fscanf(fp, "%d\n", &size);
			if(size == 1)
				m_bSkin = true;
			else
				m_bSkin = false;

			fscanf(fp, "%d\n", &size);
			if(size == 1)
				m_bHair = true;
			else
				m_bHair = false;

			fscanf(fp, "%d\n", &size);
			if(size == 1)
				m_bCollar = true;
			else
				m_bCollar = false;

			fscanf(fp, "%d\n", &size);
			if(size == 1)
				m_bEar = true;
			else
				m_bEar = false;

			for(k = 0; k < 8; k++)
			{
				fscanf(fp, "%d\n", &m_nCompSelected[k]);
			}

			fscanf(fp, "%d\n", &hairIndex[0]);
			fscanf(fp, "%d\n", &clothIndex[0]);
		}

		fclose(fp);
}

void CPortraitEditingDlg::DrawPortrait()
{
	::SetCursor(::LoadCursor(NULL,IDC_WAIT));

	m_CurShape.Resize(91);
	m_ConstrainShape.Resize(91);
	m_vbConstrained.resize(91);
	for(int i = 0; i < 91; i++)
	{
		m_CurShape.PointX(i) = m_dpLandMarks[i].x;
		m_CurShape.PointY(i) = m_dpLandMarks[i].y;

		m_ConstrainShape.PointX(i) = m_CurShape.PointX(i);
		m_ConstrainShape.PointY(i) = m_CurShape.PointY(i);
	}
	
	BYTE *buf = (BYTE *)m_TargetImg.GetImage()->imageData;
	int WidthPix = WIDTHBYTES(m_TargetImg.Width() * 24);

	int tempTop = clothRect.top;
	clothRect.top = m_TargetImg.Height();
	ConncetContour(m_skinVector, m_TargetImg.GetImage(), CV_RGB(0,0,0), 1, true);

	CRgn tempRgn;
	CRect tempRect;
	CPoint tempPoints[23];
	for(int k = 0; k < 23; k++)
	{
		tempPoints[k].x = m_dpLandMarks[k + 68].x;
		tempPoints[k].y = m_dpLandMarks[k + 68].y + 8;
	}
	tempRgn.CreatePolygonRgn(tempPoints, 23, WINDING);
	tempRgn.GetRgnBox(tempRect);

	clothRect.top = tempTop;
	for(i = clothRect.top; i < clothRect.bottom; i++)
	{
		for(int j = clothRect.left; j < clothRect.right; j++)
		{
			if(!tempRgn.PtInRegion(CPoint(j, i)))
			{
				memset((void *) (buf + i * WidthPix + 3 * j), 255, 3 * sizeof(BYTE));
			}
		}
	}
	ConncetContour(m_skinVector, m_TargetImg.GetImage(), CV_RGB(0,0,0), 1, true);
	ConncetContour(clothVectorX, clothVectorY, m_TargetImg.GetImage(), CV_RGB(0,0,0), 1);

	for(i = tempRect.top; i < tempRect.bottom; i++)
	{
		for(int j = tempRect.left; j < tempRect.right; j++)
		{
			if(tempRgn.PtInRegion(CPoint(j,i)))
			{
				memset((void *) (buf + i * WidthPix + 3 * j), 255, 3 * sizeof(BYTE));	
			}
		}
	}
	ConnectLandmark91(CV_RGB(0,0,0), 1);
	DrawEyeball();

	for(i = hairRect.top; i < hairRect.bottom; i++)
	{
		for(int j = hairRect.left; j < hairRect.right; j++)
		{
			if(hairRgn.PtInRegion(CPoint(j, i)))
			{
				memset((void *) (buf + i * WidthPix + 3 * j), 255, 3 * sizeof(BYTE));
			}
		}
	}
	ConncetContour(hairVectorX, hairVectorY, m_TargetImg.GetImage(), CV_RGB(0,0,0), 1);

	m_TargetImgCopy.Create(m_TargetImg.Width(), m_TargetImg.Height(), 24);
	cvCopy(m_TargetImg.GetImage(), m_TargetImgCopy.GetImage());

	m_TargetImg.Save("c:\\portrait-1.jpg");
	CString templatePath(m_sPath + "\\mode");

	if(m_nCompSelected[1] > 0)
		RenderEyebrow(m_nCompSelected[1] - 1, templatePath);
	if(m_nCompSelected[2] > 0)
		RenderEye(m_nCompSelected[2] - 1, templatePath);
	if(m_nCompSelected[3] > 0)
		RenderNose(m_nCompSelected[3] - 1, templatePath);
	if(m_nCompSelected[4] > 0)
		RenderMouth(m_nCompSelected[4] - 1, templatePath);
	if(m_nCompSelected[6] > 0)
		RenderEar(m_nCompSelected[6] - 1, templatePath);
	if(m_nCompSelected[0] > 0)
		RenderHair(m_nCompSelected[0] - 1, templatePath);
	if(m_nCompSelected[7] > 0)
		RenderFaceContour(m_nCompSelected[7] - 1, templatePath);
	if(m_nCompSelected[5] > 0)
		RenderCollar(m_nCompSelected[5] - 1, templatePath);

	m_TargetImgCopy.Create(m_TargetImg.Width(), m_TargetImg.Height(), 24);
	cvCopy(m_TargetImg.GetImage(), m_TargetImgCopy.GetImage());

	::SetCursor(::LoadCursor(NULL,IDC_ARROW));
}

void CPortraitEditingDlg::OnOpenImage() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, "jpg", _T("*.jpg"),
	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
	"JPEG Files (*.jpg)|*.jpg|Bitmap Files (*.bmp)|*.bmp|P2A Files (*.p2a)|*.p2a||");
	if( IDOK != dlg.DoModal())
	{
		return;
	}

	bool isImage;
	if(dlg.m_ofn.nFilterIndex == 1 || dlg.m_ofn.nFilterIndex == 2)
	{
		strImgName = dlg.GetPathName();
	
		if(m_image.Load(strImgName) == false)
		{
			MessageBox("你所选的图片格式不对!");
			return;
		}
		m_TargetImg.Load(strImgName);
		m_dRatio = ImageZoom(m_image);
		m_image.Save(m_sPath + "\\temp" + "\\input.bmp");

		isImage = true;
	}
	else
	{
		CString sName = dlg.GetPathName();

//		LoadFile(sName);
//		m_image.Load(strImgName);
//		m_TargetImg.Load(strImgName);
//		m_dRatio = ImageZoom(m_image);
//		m_image.Save(m_sPath + "\\temp" + "\\input.bmp");

		LoadXML(sName);
		m_image.Load(m_sPath + "\\temp" + "\\input.bmp");

		isImage = false;
	}
	
	m_TargetImg.Destroy();
    m_TargetImg.Create(m_image.Width(), m_image.Height(), 24);
	ImageChanged(m_TargetImg.GetImage(), 255);
	m_RegionImg.Destroy();
    m_RegionImg.Create(m_image.Width(), m_image.Height(), 24);
	ImageChanged(m_RegionImg.GetImage(), 255);
	m_RenderImg.Destroy();
	m_RenderImg.Create(m_TargetImg.Width(), m_TargetImg.Height(), 24);
	ImageChanged(m_RenderImg.GetImage(), 255);
	m_pCurImg.ImageClear();
	m_pCurImg.ReadFromFile(m_sPath + "\\temp" + "\\input.bmp");

	OrignalImage.load_bmp(m_sPath + "\\temp" + "\\input.bmp");

	m_SrcImage.Deallocate();
	m_SrcImage.ReadFile(m_sPath + "\\temp" + "\\input.bmp");
	m_SrcGrayImage.Allocate(m_SrcImage.Shape());
	m_SrcGrayImage = VisBrightnessFromRGBA(m_SrcImage);

	m_LstCtrComponent.DeleteAllItems();
	scissorsRect.bottom = scissorsRect.top;
	scissorsRect.right = scissorsRect.left;

	if (Gcs != NULL)
	{
		delete Gcs;
		Gcs = NULL;
	}
	if (NULL == Gcs)
	{
		Gcs = new GCsegmentation;
	}

	m_nSelectedVector.clear();
	FrontStroke.clear();
	BackStroke.clear();
	
	m_bImgOpened = true;
	m_bGCInit = false;
	m_bAutoColor = false;
	
	if(isImage)
	{
		OnScissors();
		m_bFeatureLocalized = false;
		m_bFeature = false;
		m_bBackground = false;
		m_bSkin = false;
		m_bHair = false;
		m_bCollar = false;
		m_bEar = false;

		m_backgroundVector.clear();
		m_skinVector.clear();
		hairVectorX.clear();
		hairVectorY.clear();
		clothVectorX.clear();
		clothVectorY.clear();
		leftEarVector.clear();
		rightEarVector.clear();

		m_btBackground.EnableWindow(FALSE);
		m_btSkin.EnableWindow(FALSE);
		m_btHair.EnableWindow(FALSE);
		m_btCollar.EnableWindow(FALSE);
		m_btDefaultColor.EnableWindow(FALSE);
		m_btHairColor.EnableWindow(FALSE);
		m_btSkinColor.EnableWindow(FALSE);
		m_btClothColor.EnableWindow(FALSE);
		m_btBackgroundColor.EnableWindow(FALSE);
		m_btShadowColor.EnableWindow(FALSE);
		m_btCrossStitch.EnableWindow(FALSE);
		m_btRender.EnableWindow(FALSE);
		m_btEar.EnableWindow(FALSE);
		m_btAdjust.EnableWindow(FALSE);

		for(int i = 0; i < 8; i++)
		{
			m_nCompSelected[i] = 0;
		}

		hairIndex[0] = 0;
		hairIndex[1] = 0;
		clothIndex[0] = 0;
		clothIndex[1] = 0;
	}
	else
	{
        DrawPortrait();
		
		m_btFeature.EnableWindow(TRUE);
		m_btBackground.EnableWindow(TRUE);
		m_btEar.EnableWindow(TRUE);
		if(m_bBackground)
			m_btSkin.EnableWindow(TRUE);
		else
			m_btSkin.EnableWindow(FALSE);
		if(m_bSkin)
			m_btHair.EnableWindow(TRUE);
		else
			m_btHair.EnableWindow(FALSE);
		if(m_bHair)
			m_btCollar.EnableWindow(TRUE);
		else
			m_btCollar.EnableWindow(FALSE);
		if(m_bCollar)
			m_btDefaultColor.EnableWindow(TRUE);
		else
			m_btDefaultColor.EnableWindow(FALSE);
	
		m_btHairColor.EnableWindow(FALSE);
		m_btSkinColor.EnableWindow(FALSE);
		m_btClothColor.EnableWindow(FALSE);
		m_btBackgroundColor.EnableWindow(FALSE);
		m_btShadowColor.EnableWindow(FALSE);
		m_btCrossStitch.EnableWindow(FALSE);
		m_btRender.EnableWindow(FALSE);
		m_btAdjust.EnableWindow(FALSE);
	}

	m_nRecordIndex = -1;
	m_nRecordMaxIndex = -1;
	m_nPart = -1;
	UpdateData(FALSE);
	
	Invalidate();			
}


void CPortraitEditingDlg::OnFeatureLocal() 
{
	// TODO: Add your control notification handler code here
	if(m_bImgOpened)
	{
		::SetCursor(::LoadCursor(NULL,IDC_WAIT));
		//LoadCursor：
		//该函数从一个与应用事例相关的可执行文件（EXE文件）中载入指定的光标资源。
		//该函数已被Loadlmage函数替代。

		if(m_bFeature == false)
		{
			bool bOK = true;
			FaceAlgError err;
			int unImgWidth = m_pCurImg.Width();
			int unImgHeight = m_pCurImg.Height();

			CImage ImgTemp(m_pCurImg);

			//STEP0 验证检测算法（调用FLLValidateFD）。
			CString sFDAlgID;
			int nLen;
			if ( !FDGetAlgID(sFDAlgID.GetBuffer(_MAX_PATH), nLen) && bOK)
			{
				err = FDGetLastError();
				bOK = false;
			}
			int nFDVersion = FDGetVersion();

			//STEP1 检验算法标志串和版本（调用FLLGetAlgID和FLLGetVersion）。
			CString sAlgID;
			if ( !FLLGetAlgID(sAlgID.GetBuffer(_MAX_PATH), nLen) && bOK)
			{
				err = FLLGetLastError();
				bOK = false;
			}
			int nFLLVersion = FLLGetVersion();

			if( !FLLValidateFD(sFDAlgID, nFDVersion) )
			{
				err = FDGetLastError();
				bOK = false;
			}


			//STEP2 初始化工作（调用FLLInitialize）。

			if ( !FLLInitialize(m_sPath+ "\\fll.dat", modeEyes) && bOK )
			{
				err = FLLGetLastError();
				bOK = false;
			}

			//STEP3 调节参数，设定算法运行的精度和速度（调用FLLSetAccuracy和FLLSetSpeed）
			double lfAccuracy = 0.95;
			double lfSpeed = 0;
			if(	!FLLSetAccuracy(lfAccuracy, lfSpeed) && bOK)
			{
				err = FLLGetLastError();
				bOK = false;
			}

			//Detect Faces
			const char*  v=NULL;
			if( !FDInitialize(v) )
			{
				err = FLLGetLastError();
				bOK = false;
			}

			int nDetectedFaces=0;
			int nImageBit=ImgTemp.Bits();
			if( !FDDetectFaces(ImgTemp.Data(), unImgWidth, unImgHeight,
				nImageBit, nDetectedFaces) && bOK)
			{
				nDetectedFaces = 0;
				err = FLLGetLastError();
				bOK = false;
			}

			if(nDetectedFaces != 1)
			{
				MessageBox("请找开一张正脸图片!");
				return;
			}

			int nWritedFaces = 0;
			FACEINFO *pFaceInfos = new FACEINFO[nDetectedFaces];
			FDGetFaces(pFaceInfos, nWritedFaces);

			//process each detected faces
			if ( ImgTemp.Bits() == 24 )
				ImgTemp.CreateGreyDib();
			CPoint **ppPntLandmarks = new CPoint*[MAX_NUM_FACES];
			for(int nDF = 0; nDF < nDetectedFaces; nDF++)
				ppPntLandmarks[nDF] = new CPoint[NUM_OF_LANDMARKS];
		//	for(int nf = 0; nf < nDetectedFaces; nf++)
			for(int nf = 0; nf < nDetectedFaces; nf++)//只处理单张人脸
			{

				//STEP4 定位特征点，返回被定位点的个数（调用FLLLocatLandmarks）。
				int nLandmarks = 0;

				int nImageBit = ImgTemp.Bits();
				if (!FLLLocateLandmarks(ImgTemp.Data(), unImgWidth, unImgHeight,
					nImageBit, pFaceInfos[nf], nLandmarks) && bOK)
				{
					err = FLLGetLastError();
					bOK = false;
				}

				//STEP5 得到上一步定位的各个点的数据（调用FLLGetLandmarks）。
				int nWritedLandmarks = NUM_OF_LANDMARKS;

				LANDMARKINFO *lpLandmarks = new LANDMARKINFO[nWritedLandmarks];
				if ( !FLLGetLandmarks(lpLandmarks, nWritedLandmarks) && bOK)
				{
					err = FLLGetLastError();
					bOK = false;
				}
				else
				{
					for(int nlm = 0; nlm < NUM_OF_LANDMARKS - 2; nlm++)
					{
						ppPntLandmarks[nf][nlm].x = lpLandmarks[nlm].x;
						ppPntLandmarks[nf][nlm].y = lpLandmarks[nlm].y;

						m_ptArrLandMarks[nlm].x = lpLandmarks[nlm].x;
						m_ptArrLandMarks[nlm].y = lpLandmarks[nlm].y;

						int lm_x = m_ptArrLandMarks[nlm].x;
						int lm_y = m_ptArrLandMarks[nlm].y;

					}  
					int nTempHeight = ( ppPntLandmarks[nf][96].y - ppPntLandmarks[nf][12].y ) / 2;

					ppPntLandmarks[nf][NUM_OF_LANDMARKS - 2].x = ppPntLandmarks[nf][96].x;
					ppPntLandmarks[nf][NUM_OF_LANDMARKS - 2].y = ppPntLandmarks[nf][96].y + nTempHeight;
					m_ptArrLandMarks[NUM_OF_LANDMARKS - 2] = ppPntLandmarks[nf][NUM_OF_LANDMARKS - 2];

					ppPntLandmarks[nf][NUM_OF_LANDMARKS - 1].x = ppPntLandmarks[nf][97].x;
					ppPntLandmarks[nf][NUM_OF_LANDMARKS - 1].y = ppPntLandmarks[nf][97].y + nTempHeight;
					m_ptArrLandMarks[NUM_OF_LANDMARKS - 1] = ppPntLandmarks[nf][NUM_OF_LANDMARKS - 1];

					
					m_bFeatureLocalized = true;
					m_nStatus = feature;
					
					Shapeinit();

					ConnectLandmark91(CV_RGB(0,0,0), 1);
					DrawEyeball();

					Invalidate(FALSE);
				}


				delete[] lpLandmarks;

			}//end for each detected face

			delete[] pFaceInfos;

			for (int nfc = 0; nfc < nDetectedFaces; nfc++)
				delete[] ppPntLandmarks[nfc];
			delete []ppPntLandmarks;

			m_bFeature = true;
			m_btBackground.EnableWindow(TRUE);
			m_btEar.EnableWindow(TRUE);
			m_btAdjust.EnableWindow(TRUE);

			::SetCursor(::LoadCursor(NULL,IDC_ARROW));
		}
		else
		{
			ConnectLandmark91(CV_RGB(0,0,0), 1);
			DrawEyeball();

			m_nStatus = feature;
			m_btAdjust.EnableWindow(TRUE);
			Invalidate(FALSE);
		}
	}
	else
	{
		MessageBox("请打开一张正脸图片!");	
	}
	cvSaveImage("c:\\1.jpg",m_TargetImg.GetImage());
}

void CPortraitEditingDlg::OnDetectBackground() 
{
	// TODO: Add your control notification handler code here
	
	if(m_bImgOpened && m_bFeatureLocalized)
	{
		int i, j;

		IplImage* tempImage = 0;
		tempImage = cvCreateImage(cvGetSize(m_image.GetImage()), IPL_DEPTH_8U, 3);
		cvCvtColor(m_image.GetImage(), tempImage, CV_BGR2YCrCb);

		IplImage* grayImage = 0;
		grayImage = cvCreateImage(cvGetSize(m_image.GetImage()), IPL_DEPTH_8U, 1);

		BYTE *pbuf = m_pCurImg.m_pData;
		BYTE *buf = (BYTE *)tempImage->imageData;
		BYTE *buf1 = (BYTE *)m_image.GetImage()->imageData;
		BYTE *buf2 = (BYTE *)grayImage->imageData;

		int WidthPix1 = WIDTHBYTES(m_image.Width() * 24);
		int WidthPix2 = WIDTHBYTES(grayImage->width * 8);

		if(m_bBackground == false)
		{
			CRect backgroundRect(0, 0, m_image.Width(), 30);
			
			double meanY = 0;
			double meanCr = 0;
			double meanCb = 0;
			double max_diffY = 0;
			double max_diffCr = 0;
			double max_diffCb = 0;
			Mean(tempImage, backgroundRect, meanY, meanCr, meanCb, max_diffY, max_diffCr, max_diffCb);

			int controlY = 0;
			if(fabs(meanCr -128) + fabs(meanCb - 128) < 15)
				controlY = 1;

			double diff = max_diffY + max_diffCr + max_diffCb;
				if(diff < 20)
					diff = 20;
				if(diff > 30)
					diff = 30;

			for(i = 0; i < m_image.Height(); i++)
			{
				for(j = 0; j < m_image.Width(); j++)
				{
				
					if(fabs(buf[ i * WidthPix1 + 3 * j] - meanY) * controlY + fabs(buf[ i * WidthPix1 + 3 * j + 1] - meanCr) + fabs(buf[ i * WidthPix1 + 3 * j + 2] - meanCb) < diff )
					{
						buf2[i * WidthPix2 + j] = 255;
					}
					else
					{
						buf2[ i * WidthPix2 + j] = 0;
					}
				}
			}

			backgroundRect.right = 50;
			backgroundRect.bottom = m_dpLandMarks[79].y;
			
			Mean(tempImage, backgroundRect, meanY, meanCr, meanCb, max_diffY, max_diffCr, max_diffCb);

			controlY = 0;
			if(fabs(meanCr -128) + fabs(meanCb - 128) < 15)
				controlY = 1;

			diff = max_diffY + max_diffCr + max_diffCb;
				if(diff < 20)
					diff = 20;
				if(diff > 30)
					diff = 30;

			for(i = 0; i < m_image.Height(); i++)
			{
				for(j = 0; j < m_image.Width(); j++)
				{
				
					if(fabs(buf[ i * WidthPix1 + 3 * j] - meanY) * controlY + fabs(buf[ i * WidthPix1 + 3 * j + 1] - meanCr) + fabs(buf[ i * WidthPix1 + 3 * j + 2] - meanCb) < diff )
					{
						buf2[i * WidthPix2 + j] = 255;
					}
				}
			}
		

			backgroundRect.left = m_image.Width() - 50;
			backgroundRect.right = m_image.Width();
			
			Mean(tempImage, backgroundRect, meanY, meanCr, meanCb, max_diffY, max_diffCr, max_diffCb);

			controlY = 0;
			if(fabs(meanCr -128) + fabs(meanCb - 128) < 15)
				controlY = 1;

			diff = max_diffY + max_diffCr + max_diffCb;
				if(diff < 20)
					diff = 20;
				if(diff > 30)
					diff = 30;

			for(i = 0; i < m_image.Height(); i++)
			{
				for(j = 0; j < m_image.Width(); j++)
				{
				
					if(fabs(buf[ i * WidthPix1 + 3 * j] - meanY) * controlY + fabs(buf[ i * WidthPix1 + 3 * j + 1] - meanCr) + fabs(buf[ i * WidthPix1 + 3 * j + 2] - meanCb) < diff )
					{
						buf2[i * WidthPix2 + j] = 255;
					}
				}
			}

			cvErode(grayImage, grayImage, NULL, 4);
			cvDilate(grayImage, grayImage, NULL, 5);

			CvMemStorage* storage = cvCreateMemStorage(0);
			CvSeq* contours = 0;
			cvFindContours(grayImage, storage, &contours, sizeof(CvContour),
				CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0,0) );

			contours = cvApproxPoly(contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, 5, 1 );
			//cvDrawContours(m_TargetImg.GetImage(), contours, CV_RGB(0,0,0), CV_RGB(0,255,0), 1, 1, CV_AA, cvPoint(0,0) );
			
			bool isSuccess = false;
			CvTreeNodeIterator iteratorBackground;
			cvInitTreeNodeIterator( &iteratorBackground, contours, 1);
			while( (contours = (CvSeq*)cvNextTreeNode( &iteratorBackground )) != 0 )
			{
				 CvSeqReader reader;
				 int count = contours->total;
				 cvStartReadSeq(contours, &reader, 0);
				 CvPoint pt1;
				 dPoint  pt2;  
				 if(count > 16)
				 {
					 m_backgroundVector.clear();
					 
					 CPoint *pBackPoints = new CPoint[count];
					 for( int i = 0; i < count; i++ )
					 {
						 CV_READ_SEQ_ELEM( pt1, reader );
						 pt2.x = pt1.x;
						 pt2.y = pt1.y;
						 m_backgroundVector.push_back(pt2);
						 
						 pBackPoints[i].x = pt1.x;
						 pBackPoints[i].y = pt1.y;
					 }

					 bgRgn.DeleteObject();
					 bgRgn.CreatePolygonRgn(pBackPoints, count, WINDING);
					 bgRgn.GetRgnBox(bgRect);
					 delete [] pBackPoints;

					 isSuccess = true;

					 if(bgRect.top < m_ptArrLandMarks[0].y)
						break;
				 }	
			}

			if(isSuccess == true)
			{
				m_bBackground = true;
				m_btSkin.EnableWindow(TRUE);
				m_btAdjust.EnableWindow(FALSE);
			}
			else
			{
				AfxMessageBox("背景检测失败，请手工分割");
			}
		}
		else
		{
			m_btAdjust.EnableWindow(FALSE);	
		}

		cvReleaseImage(&tempImage);
		cvReleaseImage(&grayImage);

		FrontStroke.clear();
		BackStroke.clear();
		m_nStatus = background;
		m_nGCStatus = 1;
		Invalidate(FALSE);
	}
	cvSaveImage("c:\\2.jpg",m_TargetImg.GetImage());
}

void CPortraitEditingDlg::OnDetectSkin() 
{
	// TODO: Add your control notification handler code here
	if(m_bImgOpened && m_bFeatureLocalized)
	{
			int i, j;
			
			IplImage* tempImage = 0;
			tempImage = cvCreateImage(cvGetSize(m_image.GetImage()), IPL_DEPTH_8U, 3);
			cvCvtColor(m_image.GetImage(), tempImage, CV_BGR2YCrCb);

			IplImage* grayImage = 0;
			grayImage = cvCreateImage(cvGetSize(m_image.GetImage()), IPL_DEPTH_8U, 1);

			CvvImage earGrayImage;
			earGrayImage.Create(m_image.Width(), m_image.Height(), 8);

			BYTE *pbuf = m_pCurImg.m_pData;
			BYTE *buf = (BYTE *)tempImage->imageData;
			BYTE *buf1 = (BYTE *)m_image.GetImage()->imageData;
			BYTE *buf2 = (BYTE *)grayImage->imageData;
			BYTE *buf3 = (BYTE *)earGrayImage.GetImage()->imageData;
			BYTE *buf4 = (BYTE *)m_TargetImg.GetImage()->imageData;

			int WidthPix1 = WIDTHBYTES(m_image.Width() * 24);
			int WidthPix2 = WIDTHBYTES(grayImage->width * 8);
			int WidthPix3 = WIDTHBYTES(earGrayImage.Width() * 8);
			int WidthPix4 = WIDTHBYTES(m_TargetImg.Width() * 24);

		if(m_bSkin == false)
		{
			CRect skinRect1(m_ptArrLandMarks[9].x, m_ptArrLandMarks[83].y, m_ptArrLandMarks[16].x, m_ptArrLandMarks[12].y);
			CRect skinRect2(m_ptArrLandMarks[1].x, m_ptArrLandMarks[1].y, m_ptArrLandMarks[19].x, m_ptArrLandMarks[19].y);
			
			double meanY1, meanY2;
			double meanCr1, meanCr2;
			double meanCb1, meanCb2;
			double max_diffY1, max_diffY2;
			double max_diffCr1, max_diffCr2;
			double max_diffCb1, max_diffCb2;
			Mean(tempImage, skinRect1, meanY1, meanCr1, meanCb1, max_diffY1, max_diffCr1, max_diffCb1);
			Mean(tempImage, skinRect2, meanY2, meanCr2, meanCb2, max_diffY2, max_diffCr2, max_diffCb2);

			double diff1 = max_diffCr1 + max_diffCb1;
			if(diff1 < 15)
				diff1 = 15;
			if(diff1 > 20)
				diff1 = 20;

			double diff2 = max_diffCr2 + max_diffCb2;
			if(diff2 < 15)
				diff2 = 15;
			if(diff2 > 20)
				diff2 = 20;

			for(i = 0; i < m_image.Height(); i++)
			{
				for(j = 0; j < m_image.Width(); j++)
				{
					
					if(fabs(buf[ i * WidthPix1 + 3 * j + 1] - meanCr1) + fabs(buf[ i * WidthPix1 + 3 * j + 2] - meanCb1) < diff1 - 5
					|| fabs(buf[ i * WidthPix1 + 3 * j + 1] - meanCr2) + fabs(buf[ i * WidthPix1 + 3 * j + 2] - meanCb2) < diff2 - 5)
					{
						buf2[ i * WidthPix2 + j] = 255;
					}
					else
					{
						buf2[ i * WidthPix2 + j] = 0;
					}
				
			        if(i > m_ptArrLandMarks[0].y && i < m_ptArrLandMarks[12].y)
					{
						if(fabs(buf[ i * WidthPix1 + 3 * j + 1] - meanCr1) + fabs(buf[ i * WidthPix1 + 3 * j + 2] - meanCb1) < diff1 + 5
						|| fabs(buf[ i * WidthPix1 + 3 * j + 1] - meanCr2) + fabs(buf[ i * WidthPix1 + 3 * j + 2] - meanCb2) < diff2 + 5)
						{
							buf2[ i * WidthPix2 + j] = 255;
						}
						else
						{
							buf2[ i * WidthPix2 + j] = 0;
						}
					}	
				}
			}

			//mask face
			CRgn	 faceRgn;
			CRect    faceRect;
			
			CPoint facePoint[23];
			for(i = 0; i < 23; i++)
			{
				facePoint[i].x = m_dpLandMarks[i + 68].x;
				facePoint[i].y = m_dpLandMarks[i + 68].y;
			}

			faceRgn.CreatePolygonRgn(facePoint, 23, WINDING);
			faceRgn.GetRgnBox(faceRect);

			for(i = faceRect.top; i < faceRect.bottom; i++)
			{
				for(j = faceRect.left; j < faceRect.right; j++)
				{
					if(faceRgn.PtInRegion(CPoint(j,i)))
					{
						buf2[i * WidthPix2 + j] = 255;
					}
				}
			}

			for(i = grayImage->height - 15; i < grayImage->height - 1; i++)
			{
				for(j = m_ptArrLandMarks[6].x; j < m_ptArrLandMarks[17].x; j++)
				{
					buf2[i * WidthPix2 + j] = 0;
				}
			}
		
			cvErode(grayImage, grayImage, NULL, 4);
			cvDilate(grayImage, grayImage, NULL, 3);	
			cvCopy(grayImage, earGrayImage.GetImage());

			//cvSmooth(grayImage, grayImage, CV_GAUSSIAN, 9, 9, 1);

			CvMemStorage* storage = cvCreateMemStorage(0);
			CvSeq* contours = 0;
			cvFindContours( grayImage, storage, &contours, sizeof(CvContour),
				CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0,0) );

			contours = cvApproxPoly( contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, 3, 1 );
			//cvDrawContours(m_TargetImg.GetImage(), contours, CV_RGB(0,0,0), CV_RGB(0,255,0), 1, 1, CV_AA, cvPoint(0,0) );

			 bool isSuccess = false;
			 CvTreeNodeIterator iteratorSkin;
			 cvInitTreeNodeIterator( &iteratorSkin, contours, 1);
			 while( (contours = (CvSeq*)cvNextTreeNode( &iteratorSkin )) != 0 )
			 {
				 CvSeqReader reader;
				 int count = contours->total;
				 cvStartReadSeq( contours, &reader, 0 );
				 CvPoint pt1;
				 dPoint  pt2;
				 double  minY = 10000;
				 if(count > 15)
				 {
					 m_skinVector.clear();

					 for( int i = 0; i < count; i++ )
					 {
						 CV_READ_SEQ_ELEM( pt1, reader );

						 pt2.x = pt1.x;
						 pt2.y = pt1.y;
						 m_skinVector.push_back(pt2);

						 isSuccess = true;
						 if(minY > pt1.y)
							 minY = pt1.y;
					 }
			
					 if(minY < m_dpLandMarks[70].y)
						break;
				 }	
			 }

		 if(isSuccess == true)
		 {
			int size = m_skinVector.size() + 1;
				CShape sourShape, destShape;
				sourShape.Resize(size);
				for(i = 0; i < size; i++)
				{
					if(i < size - 1)
					{
						sourShape.PointX(i) = m_skinVector[i].x;
						sourShape.PointY(i) = m_skinVector[i].y;
					}
					else
					{
						sourShape.PointX(i) = m_skinVector[0].x;
						sourShape.PointY(i) = m_skinVector[0].y;
					}
				
				}
/*	// 	*/		ResampleCurve(sourShape, destShape, 20);
				m_skinVector.clear();
				dPoint temPoint;
				for(i = 0; i < destShape.PointNum(); i++)
				{
					temPoint.x = destShape.PointX(i);
					temPoint.y = destShape.PointY(i);
					if(temPoint.x < 0)
						temPoint.x = 0;
					if(temPoint.x > m_image.Width() - 1)
						temPoint.x = m_image.Width() - 1;
					if(temPoint.y < 0)
						temPoint.y = 0;
					if(temPoint.y > m_image.Height() - 1)
						temPoint.y = m_image.Height() - 1;

					m_skinVector.push_back(temPoint);
				}

				int nNum = m_skinVector.size();
				CPoint *points = new CPoint[nNum];
				for(int k = 0; k < nNum; k++)
				{
					points[k].x = m_skinVector[k].x;
					points[k].y = m_skinVector[k].y;
				}

				skinRgn.DeleteObject();
				skinRgn.CreatePolygonRgn(points, nNum, WINDING);
				skinRgn.GetRgnBox(skRect);

				delete [] points;
		 
				cvReleaseImage(&tempImage);
				clothRect.top = m_TargetImg.Height();
				ConncetContour(m_skinVector, m_TargetImg.GetImage(), CV_RGB(0,0,0), 1, true);

				for(i = faceRect.top; i < faceRect.bottom; i++)
				{
					for(j = faceRect.left; j < faceRect.right; j++)
					{
						if(faceRgn.PtInRegion(CPoint(j,i)))
						{
							memset((void *) (buf4 + i * WidthPix4 + 3 * j), 255, 3 * sizeof(BYTE));	
						}
					}
				}
				ConnectLandmark91(CV_RGB(0,0,0), 1);
				DrawEyeball();


				m_bSkin = true;
				m_btHair.EnableWindow(TRUE);
			 }
			else
			{
				AfxMessageBox("肤色检测失败，请人工分割");
			}	
		}
		else
		{
			ConncetContour(m_skinVector, m_TargetImg.GetImage(), CV_RGB(0,0,0), 1, true);

			m_btAdjust.EnableWindow(FALSE);
		}

		FrontStroke.clear();
		BackStroke.clear();
		m_nStatus = skin;
		m_nGCStatus = 2;
		Invalidate(FALSE);	
	}
	cvSaveImage("c:\\3.jpg",m_TargetImg.GetImage());
}

void CPortraitEditingDlg::OnDetectEar() 
{
	// TODO: Add your control notification handler code here
	if(m_bImgOpened && m_bFeatureLocalized)
	{
		IplImage* tempImage = 0;
		tempImage = cvCreateImage(cvGetSize(m_image.GetImage()), IPL_DEPTH_8U, 3);
		cvCvtColor(m_image.GetImage(), tempImage, CV_BGR2YCrCb);

		CvvImage earGrayImage;
		earGrayImage.Create(m_image.Width(), m_image.Height(), 8);

		BYTE *pbuf = m_pCurImg.m_pData;
		BYTE *buf = (BYTE *)tempImage->imageData;
		BYTE *buf1 = (BYTE *)m_image.GetImage()->imageData;
		BYTE *buf2 = (BYTE *)earGrayImage.GetImage()->imageData;

		int WidthPix1 = WIDTHBYTES(m_image.Width() * 24);
		int WidthPix2 = WIDTHBYTES(earGrayImage.Width() * 8);
         
		int i, j;
		if(m_bEar == false)
		{	
			CRect skinRect1(m_ptArrLandMarks[9].x, m_ptArrLandMarks[83].y, m_ptArrLandMarks[16].x, m_ptArrLandMarks[12].y);
			CRect skinRect2(m_ptArrLandMarks[1].x, m_ptArrLandMarks[1].y, m_ptArrLandMarks[19].x, m_ptArrLandMarks[19].y);
			
			double meanY1, meanY2;
			double meanCr1, meanCr2;
			double meanCb1, meanCb2;
			double max_diffY1, max_diffY2;
			double max_diffCr1, max_diffCr2;
			double max_diffCb1, max_diffCb2;
			Mean(tempImage, skinRect1, meanY1, meanCr1, meanCb1, max_diffY1, max_diffCr1, max_diffCb1);
			Mean(tempImage, skinRect2, meanY2, meanCr2, meanCb2, max_diffY2, max_diffCr2, max_diffCb2);

			double diff1 = max_diffCr1 + max_diffCb1;
			if(diff1 < 15)
				diff1 = 15;
			if(diff1 > 25)
				diff1 = 25;

			double diff2 = max_diffCr2 + max_diffCb2;
			if(diff2 < 15)
				diff2 = 15;
			if(diff2 > 25)
				diff2 = 25;
		
			CPoint leftEarPoint[8], rightEarPoint[8];
			for(i = 0; i < 8; i++)
			{
				if(i < 6)
				{
					leftEarPoint[i].x = m_dpLandMarks[69 + i].x;
					leftEarPoint[i].y = m_dpLandMarks[69 + i].y;
					rightEarPoint[i].x = m_dpLandMarks[84 + i].x;
					rightEarPoint[i].y = m_dpLandMarks[84 + i].y;
				}
				else
				{
					leftEarPoint[6].x = leftEarPoint[5].x -  50;
					leftEarPoint[6].y = leftEarPoint[5].y;
					leftEarPoint[7].x = leftEarPoint[0].x -  50;
					leftEarPoint[7].y = leftEarPoint[0].y;
					rightEarPoint[6].x = rightEarPoint[5].x +  50;
					rightEarPoint[6].y = rightEarPoint[5].y;
					rightEarPoint[7].x = rightEarPoint[0].x +  50;
					rightEarPoint[7].y = rightEarPoint[0].y;
				}
			}

			leftEarRgn.DeleteObject();
			leftEarRgn.CreatePolygonRgn(leftEarPoint, 8, WINDING);
			leftEarRgn.GetRgnBox(leftEarRect);
			rightEarRgn.DeleteObject();
			rightEarRgn.CreatePolygonRgn(rightEarPoint, 8, WINDING);
			rightEarRgn.GetRgnBox(rightEarRect);

			for(i = 0; i < m_image.Height(); i++)
			{
				for(j = 0; j < m_image.Width(); j++)
				{
					if(leftEarRgn.PtInRegion(CPoint(j, i)) || rightEarRgn.PtInRegion(CPoint(j, i)))
					{
						if(fabs(buf[ i * WidthPix1 + 3 * j + 1] - meanCr1) + fabs(buf[ i * WidthPix1 + 3 * j + 2] - meanCb1) < diff1
						|| fabs(buf[ i * WidthPix1 + 3 * j + 1] - meanCr2) + fabs(buf[ i * WidthPix1 + 3 * j + 2] - meanCb2) < diff2)
						{
							buf2[ i * WidthPix2 + j] = 255;
						}
						else
						{
							buf2[ i * WidthPix2 + j] = 0;
						}
					}
					else
					{
						buf2[ i * WidthPix2 + j] = 0;
					}
				}
			}
			cvErode(earGrayImage.GetImage(), earGrayImage.GetImage(), NULL, 3);
			cvDilate(earGrayImage.GetImage(), earGrayImage.GetImage(), NULL, 4);
			earGrayImage.Save("c:\\ear.jpg");

			int sumWhite = 0;
			for(i = 0; i < m_image.Height(); i++)
			{
				for(j = 0; j < m_image.Width(); j++)
				{
					if(buf2[ i * WidthPix2 + j] == 255)
					{
					  sumWhite++;
					}
				}
			}
			if(sumWhite < 20)
				return;


			CvMemStorage* storage = cvCreateMemStorage(0);
			CvSeq* contours = 0;
			cvFindContours( earGrayImage.GetImage(), storage, &contours, sizeof(CvContour),
				CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0,0) );

			contours = cvApproxPoly( contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, 3, 1 );

			CvTreeNodeIterator iteratorEar;
			cvInitTreeNodeIterator( &iteratorEar, contours, 1);

			leftEarVector.clear();
			rightEarVector.clear();
			leftEarRgn.DeleteObject();
			rightEarRgn.DeleteObject();

			 while( (contours = (CvSeq*)cvNextTreeNode( &iteratorEar )) != 0 )
			 {
				 CvSeqReader reader;
				 int count = contours->total;
				 cvStartReadSeq( contours, &reader, 0 );
				 CvPoint pt1;
				 dPoint  pt2;
				 if(count > 3)
				 {
					 CPoint *pEarPoints = new CPoint[count];
					 for( int i = 0; i < count; i++ )
					 {
						 CV_READ_SEQ_ELEM( pt1, reader );
						 pEarPoints[i].x = pt1.x;
						 pEarPoints[i].y = pt1.y;

						 pt2.x = pt1.x;
						 pt2.y = pt1.y;

						 if(pt2.x < m_dpLandMarks[79].x)
							leftEarVector.push_back(pt2);
						 if(pt2.x > m_dpLandMarks[79].x)
							 rightEarVector.push_back(pt2);
					 }
			
					 if(pEarPoints[0].x < m_dpLandMarks[79].x)
					 {
						 leftEarRgn.CreatePolygonRgn(pEarPoints, count, WINDING);
						 leftEarRgn.GetRgnBox(leftEarRect);
					 }

					 if(pEarPoints[0].x > m_dpLandMarks[79].x)
					 {
						 rightEarRgn.CreatePolygonRgn(pEarPoints, count, WINDING);
						 rightEarRgn.GetRgnBox(rightEarRect);
					 }

					 delete [] pEarPoints;
				 }	
			 }

			cvReleaseImage(&tempImage);
			
			m_bEar = true;
			m_nStatus = ear;
			Invalidate(FALSE);	
		}
		else
		{
			m_nStatus = ear;
			m_btAdjust.EnableWindow(FALSE);
			Invalidate(FALSE);	
		}
	}
}

void CPortraitEditingDlg::OnDetectHair() 
{
	// TODO: Add your control notification handler code here
	if(m_bImgOpened && m_bFeatureLocalized)
	{
		int i, j;
		BYTE *pbuf = m_pCurImg.m_pData;
		BYTE *buf1 = (BYTE *)m_image.GetImage()->imageData;
		int WidthPix1 = WIDTHBYTES(m_image.Width() * 24);
		BYTE *buf2 = (BYTE *)m_TargetImg.GetImage()->imageData;
		int WidthPix2 = WIDTHBYTES(m_TargetImg.Width() * 24);

		if(m_bHair == false)
		{
			m_binaryImg.Create(m_image.Width(), m_image.Height(), IPL_DEPTH_8U);
			BYTE *buf = (BYTE *)m_binaryImg.GetImage()->imageData;
			int WidthPix = WIDTHBYTES(m_binaryImg.Width() * 8);

			for(i = bgRect.top; i < bgRect.bottom; i++)
			{
				for(j = bgRect.left; j < bgRect.right; j++)
				{
					if(bgRgn.PtInRegion(CPoint(j,i)))
					{
						buf[i * WidthPix + j] = 255;
					}
					else
					{
						buf[i * WidthPix + j] = 0;
					}
				}
			}

			for(i = skRect.top; i < skRect.bottom; i++)
			{
				for(j = skRect.left; j < skRect.right; j++)
				{
					if(skinRgn.PtInRegion(CPoint(j,i)))
					{
						buf[i * WidthPix + j] = 255;
					}
				}
			}

			for(i = 0; i < m_binaryImg.Height(); i++)
			{
				for(j = 0; j < m_binaryImg.Width(); j++)
				{
					if(i > m_ptArrLandMarks[10].y)
						buf[i * WidthPix + j] = 180;
					if(buf[i * WidthPix + j] == 0)
						buf[i * WidthPix + j] = 90;
					if(buf[i * WidthPix + j] == 255)
						buf[i * WidthPix + j] = 180;
				}
			}

			for(i = 0; i < m_binaryImg.Height(); i++)
			{
				for(j = 0; j < m_binaryImg.Width(); j++)
				{
					if(buf[i * WidthPix + j] == 90)
						buf[i * WidthPix + j] = 255;
					if(buf[i * WidthPix + j] == 180)
						buf[i * WidthPix + j] = 0;
				}
			}

			cvErode(m_binaryImg.GetImage(), m_binaryImg.GetImage(), NULL, 4);
			cvDilate(m_binaryImg.GetImage(), m_binaryImg.GetImage(), NULL, 5);
			m_binaryImg.Save(m_sPath + "\\temp\\hair_mask.jpg");
			
			CvMemStorage* storage = cvCreateMemStorage(0);
			CvSeq* contours = 0;
			cvFindContours( m_binaryImg.GetImage(), storage, &contours, sizeof(CvContour),
				CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0,0) );

			contours = cvApproxPoly( contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, 5, 1 );
			
			bool isSuccess = false;
			///////////////////////////////get hair contour
			 CvTreeNodeIterator iteratorHair;
			 cvInitTreeNodeIterator( &iteratorHair, contours, 1);

			 while( (contours = (CvSeq*)cvNextTreeNode( &iteratorHair )) != 0 )
			 {
				 CvSeqReader reader;
				 int count = contours->total;
				 cvStartReadSeq( contours, &reader, 0 );
				 CvPoint pt1;
				 if(count >= 10)
				 {
					 hairVectorX.clear();
					 hairVectorY.clear();

					 for(i = 0; i < count; i++ )
					 {
						 CV_READ_SEQ_ELEM( pt1, reader );
						 
						 hairVectorX.push_back(pt1.x);
						 hairVectorY.push_back(pt1.y);
					 }

					 isSuccess = true;
				 }	
			 }	

			 if(isSuccess == true)
			 {
				int size = hairVectorX.size() + 1;
				CShape sourShape, destShape;
				sourShape.Resize(size);
				for(i = 0; i < size; i++)
				{
					if(i < size - 1)
					{
						sourShape.PointX(i) = hairVectorX[i];
						sourShape.PointY(i) = hairVectorY[i];
					}
					else
					{
						sourShape.PointX(i) = hairVectorX[0];
						sourShape.PointY(i) = hairVectorY[0];
					}
				
				}
				ResampleCurve(sourShape, destShape, 20);
				hairVectorX.clear();
				hairVectorY.clear();
				dPoint temPoint;
				for(i = 0; i < destShape.PointNum(); i++)
				{
					temPoint.x = destShape.PointX(i);
					temPoint.y = destShape.PointY(i);
					if(temPoint.x < 0)
						temPoint.x = 0;
					if(temPoint.x > m_image.Width() - 1)
						temPoint.x = m_image.Width() - 1;
					if(temPoint.y < 0)
						temPoint.y = 0;
					if(temPoint.y > m_image.Height() - 1)
						temPoint.y = m_image.Height() - 1;

					hairVectorX.push_back(temPoint.x);
					hairVectorY.push_back(temPoint.y);
				}

				int nNum = hairVectorX.size();
				CPoint *points = new CPoint[nNum];
				for(int k = 0; k < nNum; k++)
				{
					points[k].x = hairVectorX[k];
					points[k].y = hairVectorY[k];
				}

				hairRgn.DeleteObject();
				hairRgn.CreatePolygonRgn(points, nNum, WINDING);
				hairRgn.GetRgnBox(hairRect);
				delete [] points;

				for(i = hairRect.top; i < hairRect.bottom; i++)
				{
					for(j = hairRect.left; j < hairRect.right; j++)
					{
						if(hairRgn.PtInRegion(CPoint(j, i)))
						{
							memset((void *) (buf2 + i * WidthPix2 + 3 * j), 255, 3 * sizeof(BYTE));
						}
					}
				}
				ConncetContour(hairVectorX, hairVectorY, m_TargetImg.GetImage(), CV_RGB(0,0,0), 1);

				m_bHair = true;
				m_btCollar.EnableWindow(TRUE);
			 }
			 else
			 {
				 MessageBox("头发检测失败，请人工分割");
			 }
		}
		else
		{
			ConncetContour(hairVectorX, hairVectorY, m_TargetImg.GetImage(), CV_RGB(0,0,0), 1);

			m_btAdjust.EnableWindow(FALSE);	
		}

		FrontStroke.clear();
		BackStroke.clear();
		m_nStatus = hair;
		m_nGCStatus = 3;
		Invalidate(FALSE);
	}
	cvSaveImage("c:\\4.jpg",m_TargetImg.GetImage());
}

void CPortraitEditingDlg::OnDetectCollar() 
{
	// TODO: Add your control notification handler code here
	if(m_bImgOpened && m_bFeatureLocalized)
	{
		int i, j;
		BYTE *pbuf = m_pCurImg.m_pData;
		BYTE *buf1 = (BYTE *)m_image.GetImage()->imageData;
		int WidthPix1 = WIDTHBYTES(m_image.Width() * 24);
		BYTE *buf2 = (BYTE *)m_TargetImg.GetImage()->imageData;
		int WidthPix2 = WIDTHBYTES(m_TargetImg.Width() * 24);
		
		if(m_bCollar == false)
		{
			BYTE *buf = (BYTE *)m_binaryImg.GetImage()->imageData;
			int WidthPix = WIDTHBYTES(m_binaryImg.Width() * 8);

			for(i = bgRect.top; i < bgRect.bottom; i++)
			{
				for(j = bgRect.left; j < bgRect.right; j++)
				{
					if(bgRgn.PtInRegion(CPoint(j,i)))
					{
						buf[i * WidthPix + j] = 255;
					}
					else
					{
						buf[i * WidthPix + j] = 0;
					}
				}
			}

			for(i = skRect.top; i < skRect.bottom; i++)
			{
				for(j = skRect.left; j < skRect.right; j++)
				{
					if(skinRgn.PtInRegion(CPoint(j,i)))
					{
						buf[i * WidthPix + j] = 255;
					}
				}
			}

			for(i = hairRect.top; i < hairRect.bottom; i++)
			{
				for(j = hairRect.left; j < hairRect.right; j++)
				{
					if(hairRgn.PtInRegion(CPoint(j,i)))
					{
						buf[i * WidthPix + j] = 255;
					}
				}
			}

			for(i = 0; i < m_binaryImg.Height(); i++)
			{
				for(j = 0; j < m_binaryImg.Width(); j++)
				{
					if(i < m_ptArrLandMarks[7].y)
						buf[i * WidthPix + j] = 180;
					if(buf[i * WidthPix + j] == 0)
						buf[i * WidthPix + j] = 90;
					if(buf[i * WidthPix + j] == 255)
						buf[i * WidthPix + j] = 180;
				}
			}

			for(i = 0; i < m_binaryImg.Height(); i++)
			{
				for(j = 0; j < m_binaryImg.Width(); j++)
				{
					if(buf[i * WidthPix + j] == 90)
						buf[i * WidthPix + j] = 255;
					if(buf[i * WidthPix + j] == 180)
						buf[i * WidthPix + j] = 0;
				}
			}

			cvErode(m_binaryImg.GetImage(), m_binaryImg.GetImage(), NULL, 4);
			cvDilate(m_binaryImg.GetImage(), m_binaryImg.GetImage(), NULL, 4);
			m_binaryImg.Save(m_sPath + "\\temp\\collar_mask.jpg");
			
			CvMemStorage* storage = cvCreateMemStorage(0);
			CvSeq* contours = 0;
			cvFindContours( m_binaryImg.GetImage(), storage, &contours, sizeof(CvContour),
				CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0,0) );

			contours = cvApproxPoly( contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, 3, 1 );
			
			bool isSuccess = false;
			///////////////////////////////get hair contour
			 CvTreeNodeIterator iteratorHair;
			 cvInitTreeNodeIterator( &iteratorHair, contours, 1);

			 while( (contours = (CvSeq*)cvNextTreeNode( &iteratorHair )) != 0 )
			 {
				 CvSeqReader reader;
				 int count = contours->total;
				 cvStartReadSeq( contours, &reader, 0 );
				 CvPoint pt1;
				 if(count >= 10)
				 {
					 clothVectorX.clear();
					 clothVectorY.clear();

					 for(i = 0; i < count; i++ )
					 {
						 CV_READ_SEQ_ELEM( pt1, reader );

						 clothVectorX.push_back(pt1.x);
						 clothVectorY.push_back(pt1.y);
					 }

					 isSuccess = true;
				 }	
			 }	
			if(isSuccess == true)
			{
				vector<double> tempVectorX1, tempVectorX2;
				vector<double> tempVectorY1, tempVectorY2;
				bool isOne = true;
				for(i = 0; i < clothVectorY.size(); i++)
				{
					if(clothVectorY[i] < m_image.Height() - 10)
					{
						if(isOne == true)
						{	
							tempVectorX1.push_back(clothVectorX[i]);
							tempVectorY1.push_back(clothVectorY[i]);
						}
						else
						{
							tempVectorX2.push_back(clothVectorX[i]);
							tempVectorY2.push_back(clothVectorY[i]);
						}
					}
					else
					{
						isOne = false;
					}
				}
			
				int size1 = tempVectorX1.size();
				int size2 = tempVectorX2.size();
				int size = size1 + size2;
				CShape sourShape, destShape;
				sourShape.Resize(size);
				for(i = 0; i < size; i++)
				{
					if(i < size2)
					{
						sourShape.PointX(i) = tempVectorX2[i];
						sourShape.PointY(i) = tempVectorY2[i];	
					}
					else
					{
						sourShape.PointX(i) = tempVectorX1[i - size2];
						sourShape.PointY(i) = tempVectorY1[i - size2];	
					}
				}
				ResampleCurve(sourShape, destShape, 20);
				clothVectorX.clear();
				clothVectorY.clear();
				dPoint temPoint;
				for(i = 0; i < destShape.PointNum(); i++)
				{
					temPoint.x = destShape.PointX(i);
					temPoint.y = destShape.PointY(i);
					if(temPoint.x < 0)
						temPoint.x = 0;
					if(temPoint.x > m_image.Width() - 1)
						temPoint.x = m_image.Width() - 1;
					if(temPoint.y < 0)
						temPoint.y = 0;
					if(temPoint.y > m_image.Height() - 1)
						temPoint.y = m_image.Height() - 1;

					clothVectorX.push_back(temPoint.x);
					clothVectorY.push_back(temPoint.y);
				}
				if(clothVectorX[destShape.PointNum() - 1] > m_image.Width()/2)
				{
					clothVectorX.push_back(m_image.Width() - 1);
					clothVectorY.push_back(m_image.Height() - 1);
					clothVectorX.push_back(0);
					clothVectorY.push_back(m_image.Height() - 1);
				}
				else
				{
					clothVectorX.push_back(0);
					clothVectorY.push_back(m_image.Height() - 1);
					clothVectorX.push_back(m_image.Width() - 1);
					clothVectorY.push_back(m_image.Height() - 1);
				}

				int nNum = clothVectorX.size();
				CPoint *points = new CPoint[nNum];
				for(int k = 0; k < nNum; k++)
				{
					points[k].x = clothVectorX[k];
					points[k].y = clothVectorY[k];
				}

				clothRgn.DeleteObject();
				clothRgn.CreatePolygonRgn(points, nNum, WINDING);
				clothRgn.GetRgnBox(clothRect);

				delete [] points;

				CRgn tempRgn;
				CPoint tempPoints[23];
				for(k = 0; k < 23; k++)
				{
					tempPoints[k].x = m_dpLandMarks[k + 68].x;
					tempPoints[k].y = m_dpLandMarks[k + 68].y + 8;
				}
				tempRgn.CreatePolygonRgn(tempPoints, 23, WINDING);

				for(i = clothRect.top; i < clothRect.bottom; i++)
				{
					for(j = clothRect.left; j < clothRect.right; j++)
					{
						if(!tempRgn.PtInRegion(CPoint(j, i)))
						{
							memset((void *) (buf2 + i * WidthPix2 + 3 * j), 255, 3 * sizeof(BYTE));
						}
					}
				}
				ConncetContour(m_skinVector, m_TargetImg.GetImage(), CV_RGB(0,0,0), 1, true);
				ConncetContour(clothVectorX, clothVectorY, m_TargetImg.GetImage(), CV_RGB(0,0,0), 1);

				m_nRecordIndex++;
				m_TargetImg.Save(m_sPath + "\\temp\\portrait0.jpg");

				m_TargetImgCopy.Create(m_TargetImg.Width(), m_TargetImg.Height(), 24);
				cvCopy(m_TargetImg.GetImage(), m_TargetImgCopy.GetImage());

				m_bCollar = true;

				int mmmmm = m_skinVector.size();
				CImg<unsigned char> img;
				//img.load("D:\\mf.bmp");
				for(int nnnn = 0;nnnn < mmmmm; nnnn++)
				{
					unsigned char _color[3];
					_color[0] = 0;
					_color[1] = 255;
					_color[2] = 0;
					//img.draw_point(m_skinVector[0 +nnnn ].x, m_skinVector[0 + nnnn].y, _color, 4);
				}
				//img.save("D:\\dot1.bmp");

				m_btDefaultColor.EnableWindow(TRUE);
				m_btRender.EnableWindow(TRUE);
			}
			else
			{
				MessageBox("衣服检测失败，请分工分割");
			}
		}
		else
		{
			ConncetContour(clothVectorX, clothVectorY, m_TargetImg.GetImage(), CV_RGB(0,0,0), 1);
			
			m_btAdjust.EnableWindow(FALSE);	
		}
		
		FrontStroke.clear();
		BackStroke.clear();
		m_nStatus = collar;
		m_nGCStatus = 4;
		Invalidate(FALSE);
	}
	cvSaveImage("c:\\5.jpg",m_TargetImg.GetImage());
}

void CPortraitEditingDlg::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(m_bImgOpened && m_bFeatureLocalized)
	{
		NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
		int m_nSelected = pNMListView->iItem;
		CString templatePath(m_sPath + "\\mode");
		if(m_nRender == 1)
		{
			if(m_nSelected >= 0 && m_bHair == true)
			{
				RenderHair(m_nSelected, templatePath);

				m_nCompSelected[0] = m_nSelected + 1;
			}
			else
			{
				MessageBox("请先执行头发定位操作!");
			}
			cvSaveImage("c:\\onHair.jpg",m_TargetImg.GetImage());
		}

		if(m_nRender == 2)
		{
			if(m_nSelected >= 0)
			{
				RenderEyebrow(m_nSelected, templatePath);

				m_nCompSelected[1] = m_nSelected + 1;
			}
			cvSaveImage("c:\\onEyebrow.jpg",m_TargetImg.GetImage());
		}

		if(m_nRender == 3)
		{
			if(m_nSelected >= 0)
			{
				RenderEye(m_nSelected, templatePath);

				m_nCompSelected[2] = m_nSelected + 1;
			}
			cvSaveImage("c:\\onEye.jpg",m_TargetImg.GetImage());
		}

		if(m_nRender == 4)
		{
			if(m_nSelected >= 0)
			{
				RenderNose(m_nSelected, templatePath);

				m_nCompSelected[3] = m_nSelected + 1;
			}
			cvSaveImage("c:\\onNose.jpg",m_TargetImg.GetImage());
		}

		if(m_nRender == 5)
		{
			if(m_nSelected >= 0)
			{
				RenderMouth(m_nSelected, templatePath);

				m_nCompSelected[4] = m_nSelected + 1;
			}
			cvSaveImage("c:\\onMouth.jpg",m_TargetImg.GetImage());
		}

		if(m_nRender == 6)
		{
			if(m_nSelected >= 0 && m_bCollar == true)
			{
				RenderCollar(m_nSelected, templatePath);

				m_nCompSelected[5] = m_nSelected + 1;
			}
			else
			{
				MessageBox("请先执行衣领定位操作!");
			}
			cvSaveImage("c:\\onCollar.jpg",m_TargetImg.GetImage());
		}

		if(m_nRender == 8)
		{
			if(m_nSelected >= 0 && m_bEar == true && m_bHair == true)
			{
				RenderEar(m_nSelected, templatePath);

				m_nCompSelected[6] = m_nSelected + 1;
			}
			else
			{
				MessageBox("请先执行耳朵和头发定位操作!");
			}
			cvSaveImage("c:\\oneEar.jpg",m_TargetImg.GetImage());
		}

		if(m_nRender == 9)
		{
			if(m_nSelected >= 0)
			{
				RenderFaceContour(m_nSelected, templatePath);

				m_nCompSelected[7] = m_nSelected + 1;
			}
			cvSaveImage("c:\\onFacecontour.jpg",m_TargetImg.GetImage());
		}

		if(m_nRender == 7)
		{
			if(m_nSelected >= 0 && m_bAutoColor == true)
			{
				CvvImage backgroundImage;
				CvvImage compositeImage;
				CString path(m_sPath + "\\mode\\background");
				CString fileName;
				fileName.Format("\\background%d", m_nSelected + 1);
				backgroundImage.Load(path + fileName + ".jpg");
				int backWidth = backgroundImage.Width();
				int backHeight = backgroundImage.Height();
				compositeImage.Create(backWidth, backHeight, 24);

				BYTE *buf1 = (BYTE *)compositeImage.GetImage()->imageData;
				int WidthPix1 = WIDTHBYTES(compositeImage.Width() * 24);
   				BYTE *buf2 = (BYTE *)backgroundImage.GetImage()->imageData;
				int WidthPix2 = WIDTHBYTES(backgroundImage.Width() * 24);
				BYTE *buf3 = (BYTE *)m_TargetImg.GetImage()->imageData;
				int WidthPix3 = WIDTHBYTES(m_TargetImg.Width() * 24);

				int foreWidth = m_TargetImg.Width();
				int foreHeight = m_TargetImg.Height();

				int bgR = buf3[5 * WidthPix3 + 3 * (m_TargetImg.Width() - 5)];
				int bgG = buf3[5 * WidthPix3 + 3 * (m_TargetImg.Width() - 5) + 1];
				int bgB = buf3[5 * WidthPix3 + 3 * (m_TargetImg.Width() - 5) + 2];

				for(int i = 0; i < backHeight; i++)
				{
					for(int j = 0; j < backWidth; j++)
					{
						if(j > (backWidth - foreWidth)/2 && j < (backWidth + foreWidth)/2 - 2 && i > (backHeight - bgRect.bottom) && i < (backHeight - 2))
						{
							int y = i - (backHeight - bgRect.bottom);
							int x = j - (backWidth - foreWidth)/2;
							//if(bgRgn.PtInRegion(CPoint(x, y)))
							if(buf3[y * WidthPix3 + 3 * x] == bgR && buf3[y * WidthPix3 + 3 * x + 1] == bgG && buf3[y * WidthPix3 + 3 * x + 2] == bgB)
							{
								buf1[i * WidthPix1 + 3 * j] = buf2[i * WidthPix2 + 3 * j];
								buf1[i * WidthPix1 + 3 * j + 1] = buf2[i * WidthPix2 + 3 * j + 1];
								buf1[i * WidthPix1 + 3 * j + 2] = buf2[i * WidthPix2+ 3 * j + 2];
							}
							else
							{
								buf1[i * WidthPix1 + 3 * j] = buf3[y * WidthPix3 + 3 * x];
								buf1[i * WidthPix1 + 3 * j + 1] = buf3[y * WidthPix3 + 3 * x + 1];
								buf1[i * WidthPix1 + 3 * j + 2] = buf3[y * WidthPix3 + 3 * x + 2];
							}
						}
						else
						{
							buf1[i * WidthPix1 + 3 * j] = buf2[i * WidthPix2 + 3 * j];
							buf1[i * WidthPix1 + 3 * j + 1] = buf2[i * WidthPix2 + 3 * j + 1];
							buf1[i * WidthPix1 + 3 * j + 2] = buf2[i * WidthPix2 + 3 * j + 2];
						}
					}
				}

				CString saveName;
				saveName.Format("\\composite%d", m_nSelected + 1);
				compositeImage.Save(m_sPath + "\\temp" + saveName + ".jpg");

				cvNamedWindow("background", 1);
				cvShowImage("background", compositeImage.GetImage());

//				CvvImage outputImg;
//				CvvImage crossImg;
//				crossImg.Load(m_sPath + "\\res\\px12.bmp");
//				ImageZoom(compositeImage, 0.5, 0.5);
//				CrossTransfer(compositeImage, outputImg, crossImg);
//				outputImg.Save(m_sPath + "\\temp" + saveName + "Cross.jpg");
			}
			else
			{
				MessageBox("请先执行自动上色操作!");
			}
		}				
	}
	else
	{
		MessageBox("请打开一张正脸图片并定位好特征点!");
	}

	*pResult = 0;
	cvSaveImage("c:\\onBackground.jpg",m_TargetImg.GetImage());

}

void CPortraitEditingDlg::RenderEyebrow(int index, CString &templatePath) 
{
	// TODO: Add your control notification handler code here
	int i, j;
	CRgn	 leftEyebrowRgn, rightEyebrowRgn;
	CRect    leftEyebrowRect, rightEyebrowRect;
	
	CPoint leftEyebrowPoint[8], rightEyebrowPoint[8];
	for(i = 0; i < 8; i++)
	{
		leftEyebrowPoint[i].x = m_ptArrLandMarks[33 + i].x;
		leftEyebrowPoint[i].y = m_ptArrLandMarks[33 + i].y;
	}
	leftEyebrowRgn.CreatePolygonRgn(leftEyebrowPoint, 8, WINDING);
	leftEyebrowRgn.GetRgnBox(leftEyebrowRect);
	for(i = 0; i < 8; i++)
	{
		rightEyebrowPoint[i].x = m_ptArrLandMarks[25 + i].x;
		rightEyebrowPoint[i].y = m_ptArrLandMarks[25 + i].y;
	}
	rightEyebrowRgn.CreatePolygonRgn(rightEyebrowPoint, 8, WINDING);
	rightEyebrowRgn.GetRgnBox(rightEyebrowRect);

	CvvImage leftEyebrowImage, rightEyebrowImage;
	CString path(templatePath + "\\eyebrow");
	CString fileName;

	fileName.Format("\\leftEyebrow%d", index + 1);
	leftEyebrowImage.Load(path + fileName + ".jpg");
	fileName.Format("\\rightEyebrow%d", index + 1);
	rightEyebrowImage.Load(path + fileName + ".jpg");

	int lWidth = leftEyebrowImage.Width(); 
	int extra = 4;
	double leftRatioX = leftEyebrowRect.Width() * 1.0 / (leftEyebrowImage.Width() * 1.0);
	double leftRatioY = (leftEyebrowRect.Height() + extra) * 1.0 / (leftEyebrowImage.Height() * 1.0);
	double rightRatioX = rightEyebrowRect.Width() * 1.0 / (rightEyebrowImage.Width() * 1.0);
	double rightRatioY = (rightEyebrowRect.Height() + extra) * 1.0 / (rightEyebrowImage.Height() * 1.0); 
	ImageZoom(leftEyebrowImage, leftRatioX, leftRatioY);
	ImageZoom(rightEyebrowImage, rightRatioX, rightRatioY);
	cvSmooth(leftEyebrowImage.GetImage(), leftEyebrowImage.GetImage(), CV_GAUSSIAN, 3, 3, 1);
	cvSmooth(rightEyebrowImage.GetImage(), rightEyebrowImage.GetImage(), CV_GAUSSIAN, 3, 3, 1);

	vector< vector<CPoint> > pointVVector;
	fileName.Format("\\keyPoints%d.pts", index + 1);
	LoadTemplate(path + fileName, pointVVector);
	CImg<unsigned char> RetImg(m_TargetImg.Width(), m_TargetImg.Height(), 1, 3, 255);
	unsigned char black[3] = {10, 10, 10};
	for(i = 0; i < pointVVector.size(); i++)
	{
		 for(j = 0; j < pointVVector[i].size() - 1; j++)
		 { 
			 int px1 = pointVVector[i][j].x * leftRatioX + leftEyebrowRect.left;  
			 int py1 = pointVVector[i][j].y * leftRatioY + leftEyebrowRect.top - extra/2;
			 int px2 = pointVVector[i][j + 1].x * leftRatioX + leftEyebrowRect.left; 
			 int py2 = pointVVector[i][j + 1].y * leftRatioY + leftEyebrowRect.top - extra/2;
	
			 RetImg.draw_line(px1, py1, px2, py2, black);
		 }
	}
	for(i = 0; i < pointVVector.size(); i++)
	{
		 for(j = 0; j < pointVVector[i].size() - 1; j++)
		 { 
			 int px1 = (lWidth - pointVVector[i][j].x) * rightRatioX + rightEyebrowRect.left;  
			 int py1 = pointVVector[i][j].y * rightRatioY + rightEyebrowRect.top - extra/2;
			 int px2 = (lWidth - pointVVector[i][j + 1].x) * rightRatioX + rightEyebrowRect.left; 
			 int py2 = pointVVector[i][j + 1].y * rightRatioY + rightEyebrowRect.top - extra/2;
	
			 RetImg.draw_line(px1, py1, px2, py2, black);
		 }
	}

	BYTE *buf1 = (BYTE *)m_TargetImg.GetImage()->imageData;
	BYTE *buf2 = (BYTE *)leftEyebrowImage.GetImage()->imageData;
	BYTE *buf3 = (BYTE *)rightEyebrowImage.GetImage()->imageData;
	BYTE *buf4 = (BYTE *)m_TargetImgCopy.GetImage()->imageData;

	int WidthPix1 = WIDTHBYTES(m_TargetImg.Width() * 24);
	int WidthPix2 = WIDTHBYTES(leftEyebrowImage.Width() * 24);
	int WidthPix3 = WIDTHBYTES(rightEyebrowImage.Width() * 24);
	int WidthPix4 = WIDTHBYTES(m_TargetImgCopy.Width() * 24);

	ImageChanged(m_RegionImg.GetImage(), 255);
	BYTE *buf5 = (BYTE *)m_RegionImg.GetImage()->imageData;
	int WidthPix5 = WIDTHBYTES(m_RegionImg.Width() * 24);
	BYTE *buf6 = (BYTE *)m_RenderImg.GetImage()->imageData;
	int WidthPix6 = WIDTHBYTES(m_RenderImg.Width() * 24);

	CString tempStr("mode");
	for(i = 0; i < leftEyebrowImage.Height(); i++)
	{
		for(j = 0; j < leftEyebrowImage.Width(); j++)
		{
			if(buf2[i * WidthPix2 + 3 * j] > 200 && buf2[i * WidthPix2 + 3 * j + 1] > 200 && buf2[i * WidthPix2 + 3 * j + 2] > 200)
				continue;
			else
			{
				if(templatePath.Right(4) != tempStr)
				{
					buf5[(i + leftEyebrowRect.top - extra/2) * WidthPix5 + 3 * (j + leftEyebrowRect.left)] = 114;
					buf5[(i + leftEyebrowRect.top - extra/2) * WidthPix5 + 3 * (j + leftEyebrowRect.left) + 1] = 126;
					buf5[(i + leftEyebrowRect.top - extra/2) * WidthPix5 + 3 * (j + leftEyebrowRect.left) + 2] = 138;
				}
			}
		}
	}
	for(i = 0; i < rightEyebrowImage.Height(); i++)
	{
		for(j = 0; j < rightEyebrowImage.Width(); j++)
		{
			if(buf3[i * WidthPix3 + 3 * j] > 220 && buf3[i * WidthPix3 + 3 * j + 1] > 220 && buf3[i * WidthPix3 + 3 * j + 2] > 220)
				continue;
			else
			{
				if(templatePath.Right(4) != tempStr)
				{
					buf5[(i + rightEyebrowRect.top - extra/2) * WidthPix5 + 3 * (j + rightEyebrowRect.left)] = 114;
					buf5[(i + rightEyebrowRect.top - extra/2) * WidthPix5 + 3 * (j + rightEyebrowRect.left) + 1] = 126;
					buf5[(i + rightEyebrowRect.top - extra/2) * WidthPix5 + 3 * (j + rightEyebrowRect.left) + 2] = 138;
				}
			}
		}
	}
	for(i = leftEyebrowRect.top - 10; i < leftEyebrowRect.bottom + 10; i++)
	{
		for(j = leftEyebrowRect.left - 5; j < rightEyebrowRect.right + 5; j++)
		{
			if(RetImg(j, i, 0) < 220)
			{
				buf5[i * WidthPix5 + 3 * j] = RetImg(j, i, 0);
				buf5[i * WidthPix5 + 3 * j + 1] = RetImg(j, i, 1);
				buf5[i * WidthPix5 + 3 * j + 2] = RetImg(j, i, 2);
			}
		}
	}

	for(i = leftEyebrowRect.top - 5; i < leftEyebrowRect.bottom + 5; i++)
	{
		for(j = leftEyebrowRect.left - 5; j < leftEyebrowRect.right + 5; j++)
		{
			buf1[i * WidthPix1 + 3 * j] = 255;
			buf1[i * WidthPix1 + 3 * j + 1] = 255;
			buf1[i * WidthPix1 + 3 * j + 2] = 255;

			buf4[i * WidthPix4 + 3 * j] = 255;
			buf4[i * WidthPix4 + 3 * j + 1] = 255;
			buf4[i * WidthPix4 + 3 * j + 2] = 255;
		}
	}

	for(i = 0; i < leftEyebrowImage.Height(); i++)
	{
		for(j = 0; j < leftEyebrowImage.Width(); j++)
		{
			buf1[(i + leftEyebrowRect.top - extra/2) * WidthPix1 + 3 * (j + leftEyebrowRect.left)] = buf2[i * WidthPix2 + 3 * j];
			buf1[(i + leftEyebrowRect.top - extra/2) * WidthPix1 + 3 * (j + leftEyebrowRect.left) + 1] = buf2[i * WidthPix2 + 3 * j + 1];
			buf1[(i + leftEyebrowRect.top - extra/2) * WidthPix1 + 3 * (j + leftEyebrowRect.left) + 2] = buf2[i * WidthPix2 + 3 * j + 2];

			buf4[(i + leftEyebrowRect.top - extra/2) * WidthPix4 + 3 * (j + leftEyebrowRect.left)] = buf2[i * WidthPix2 + 3 * j];
			buf4[(i + leftEyebrowRect.top - extra/2) * WidthPix4 + 3 * (j + leftEyebrowRect.left) + 1] = buf2[i * WidthPix2 + 3 * j + 1];
			buf4[(i + leftEyebrowRect.top - extra/2) * WidthPix4 + 3 * (j + leftEyebrowRect.left) + 2] = buf2[i * WidthPix2 + 3 * j + 2];

			buf6[(i + leftEyebrowRect.top - extra/2) * WidthPix6 + 3 * (j + leftEyebrowRect.left)] = buf5[(i + leftEyebrowRect.top - extra/2) * WidthPix5 + 3 * (j + leftEyebrowRect.left)];
			buf6[(i + leftEyebrowRect.top - extra/2) * WidthPix6 + 3 * (j + leftEyebrowRect.left) + 1] = buf5[(i + leftEyebrowRect.top - extra/2) * WidthPix5 + 3 * (j + leftEyebrowRect.left) + 1];
			buf6[(i + leftEyebrowRect.top - extra/2) * WidthPix6 + 3 * (j + leftEyebrowRect.left) + 2] = buf5[(i + leftEyebrowRect.top - extra/2) * WidthPix5 + 3 * (j + leftEyebrowRect.left) + 2];
		}
	}

	for(i = rightEyebrowRect.top - 5; i < rightEyebrowRect.bottom + 5; i++)
	{
		for(j = rightEyebrowRect.left - 5; j < rightEyebrowRect.right + 5; j++)
		{
			buf1[i * WidthPix1 + 3 * j] = 255;
			buf1[i * WidthPix1 + 3 * j + 1] = 255;
			buf1[i * WidthPix1 + 3 * j + 2] = 255;

			buf4[i * WidthPix4 + 3 * j] = 255;
			buf4[i * WidthPix4 + 3 * j + 1] = 255;
			buf4[i * WidthPix4 + 3 * j + 2] = 255;
		}
	}

	for(i = 0; i < rightEyebrowImage.Height(); i++)
	{
		for(j = 0; j < rightEyebrowImage.Width(); j++)
		{
			buf1[(i + rightEyebrowRect.top - extra/2) * WidthPix1 + 3 * (j + rightEyebrowRect.left)] = buf3[i * WidthPix3 + 3 * j];
			buf1[(i + rightEyebrowRect.top - extra/2) * WidthPix1 + 3 * (j + rightEyebrowRect.left) + 1] = buf3[i * WidthPix3 + 3 * j + 1];
			buf1[(i + rightEyebrowRect.top - extra/2) * WidthPix1 + 3 * (j + rightEyebrowRect.left) + 2] = buf3[i * WidthPix3 + 3 * j + 2];

			buf4[(i + rightEyebrowRect.top - extra/2) * WidthPix4 + 3 * (j + rightEyebrowRect.left)] = buf3[i * WidthPix3 + 3 * j];
			buf4[(i + rightEyebrowRect.top - extra/2) * WidthPix4 + 3 * (j + rightEyebrowRect.left) + 1] = buf3[i * WidthPix3 + 3 * j + 1];
			buf4[(i + rightEyebrowRect.top - extra/2) * WidthPix4 + 3 * (j + rightEyebrowRect.left) + 2] = buf3[i * WidthPix3 + 3 * j + 2];

			buf6[(i + rightEyebrowRect.top - extra/2) * WidthPix6 + 3 * (j + rightEyebrowRect.left)] = buf5[(i + rightEyebrowRect.top - extra/2) * WidthPix5 + 3 * (j + rightEyebrowRect.left)];
			buf6[(i + rightEyebrowRect.top - extra/2) * WidthPix6 + 3 * (j + rightEyebrowRect.left) + 1] = buf5[(i + rightEyebrowRect.top - extra/2) * WidthPix5 + 3 * (j + rightEyebrowRect.left) + 1];
			buf6[(i + rightEyebrowRect.top - extra/2) * WidthPix6 + 3 * (j + rightEyebrowRect.left) + 2] = buf5[(i + rightEyebrowRect.top - extra/2) * WidthPix5 + 3 * (j + rightEyebrowRect.left) + 2];
		}
	}

	m_nRecordIndex++;
	m_nRecordMaxIndex = m_nRecordIndex;
	CString tempName;
	tempName.Format("\\temp\\portrait%d.jpg", m_nRecordIndex);
	m_TargetImg.Save(m_sPath + tempName);
	
	Invalidate(FALSE);
}

void CPortraitEditingDlg::RenderMouth(int index, CString &templatePath)
{
	int i, j;
	CRgn mouthRgn;
	CRect mouthRect;
	CPoint mouthPoints[12];

	vector<double> targetVectorX;
	vector<double> targetVectorY;
	targetVectorX.clear();
	targetVectorY.clear();
	for(i = 0; i < 12; i++)
	{
		targetVectorX.push_back(m_dpLandMarks[i + 48].x);
		targetVectorY.push_back(m_dpLandMarks[i + 48].y);

		mouthPoints[i].x = m_dpLandMarks[i + 48].x;
		mouthPoints[i].y = m_dpLandMarks[i + 48].y;
	}
	mouthRgn.CreatePolygonRgn(mouthPoints, 12, WINDING);
	mouthRgn.GetRgnBox(mouthRect);

	CvvImage mouthImage;
	CString path(templatePath + "\\mouth");
	CString fileName;

	fileName.Format("\\mouth%d", index + 1);
	mouthImage.Load(path + fileName + ".jpg");

	vector<double> sourceVectorX;
	vector<double> sourceVectorY;
	sourceVectorX.clear();
	sourceVectorY.clear();
	LoadTemplate(path + fileName + ".pts", sourceVectorX, sourceVectorY);
	
	vector< vector<CPoint> > pointVVector;
	fileName.Format("\\keyPoints%d.pts", index + 1);
	LoadTemplate(path + fileName, pointVVector);
	CImg<unsigned char> RetImg(m_TargetImg.Width(), m_TargetImg.Height(), 1, 3, 255);
	unsigned char black[3] = {10, 10, 10};

	CMyTPS myTPS;
	myTPS.Create(sourceVectorX, sourceVectorY, targetVectorX, targetVectorY, 0);
	double tx, ty;
	for(i = 0; i < pointVVector.size(); i++)
	{
		 for(j = 0; j < pointVVector[i].size() - 1; j++)
		 { 
			 myTPS.TPSTransform(pointVVector[i][j].x, pointVVector[i][j].y, tx, ty);
			 int px1 = (int)tx;  
			 int py1 = (int)ty;

			 myTPS.TPSTransform(pointVVector[i][j + 1].x, pointVVector[i][j + 1].y, tx, ty);
			 int px2 = (int)tx;  
			 int py2 = (int)ty;
	
			 RetImg.draw_line(px1, py1, px2, py2, black);
		 }
	}
	RetImg.save("c:\\mouse.bmp");

//	CRgn   tempRgn;
//	CRect  tempRect;
//	CPoint tempPoint[12];
//	for(i = 0; i < 12; i++)
//	{
//		tempPoint[i].x = sourceVectorX[i];
//		tempPoint[i].y = sourceVectorY[i];
//	}
//
//	tempRgn.CreatePolygonRgn(tempPoint, 12, WINDING);
//	tempRgn.GetRgnBox(tempRect);

//	int extra = 0;
//	double RatioX = mouthRect.Width() * 1.0 / (tempRect.Width() * 1.0);
//	double RatioY = (mouthRect.Height() + extra) * 1.0 / (tempRect.Height() * 1.0);
//
//	ImageZoom(mouthImage, RatioX, RatioY);
//	cvSmooth(mouthImage.GetImage(), mouthImage.GetImage(), CV_GAUSSIAN, 3, 3, 1);

	BYTE *buf1 = (BYTE *)m_TargetImg.GetImage()->imageData;
	BYTE *buf2 = (BYTE *)mouthImage.GetImage()->imageData;
	BYTE *buf3 = (BYTE *)m_TargetImgCopy.GetImage()->imageData;

	int WidthPix1 = WIDTHBYTES(m_TargetImg.Width() * 24);
	int WidthPix2 = WIDTHBYTES(mouthImage.Width() * 24);
	int WidthPix3 = WIDTHBYTES(m_TargetImgCopy.Width() * 24);

	ImageChanged(m_RegionImg.GetImage(), 255);
	BYTE *buf5 = (BYTE *)m_RegionImg.GetImage()->imageData;
	int WidthPix5 = WIDTHBYTES(m_RegionImg.Width() * 24);
	BYTE *buf6 = (BYTE *)m_RenderImg.GetImage()->imageData;
	int WidthPix6 = WIDTHBYTES(m_RenderImg.Width() * 24);

	for(i = mouthRect.top - 15; i < mouthRect.bottom + 15; i++)
	{
		for(j = mouthRect.left - 15; j < mouthRect.right + 15; j++)
		{
			buf1[i * WidthPix1 + 3 * j] = 255;
			buf1[i * WidthPix1 + 3 * j + 1] = 255;
			buf1[i * WidthPix1 + 3 * j + 2] = 255;

			buf3[i * WidthPix3 + 3 * j] = 255;
			buf3[i * WidthPix3 + 3 * j + 1] = 255;
			buf3[i * WidthPix3 + 3 * j + 2] = 255;
		}
	}
	
//	for(i = 0; i < mouthImage.Height(); i++)
//	{
//		for(j = 0; j < mouthImage.Width(); j++)
//		{
//			buf1[(i + mouthRect.top - extra/2) * WidthPix1 + 3 * (j + mouthRect.left)] = buf2[i * WidthPix2 + 3 * j];
//			buf1[(i + mouthRect.top - extra/2) * WidthPix1 + 3 * (j + mouthRect.left) + 1] = buf2[i * WidthPix2 + 3 * j + 1];
//			buf1[(i + mouthRect.top - extra/2) * WidthPix1 + 3 * (j + mouthRect.left) + 2] = buf2[i * WidthPix2 + 3 * j + 2];
//
//			buf3[(i + mouthRect.top - extra/2) * WidthPix3 + 3 * (j + mouthRect.left)] = buf2[i * WidthPix2 + 3 * j];
//			buf3[(i + mouthRect.top - extra/2) * WidthPix3 + 3 * (j + mouthRect.left) + 1] = buf2[i * WidthPix2 + 3 * j + 1];
//			buf3[(i + mouthRect.top - extra/2) * WidthPix3 + 3 * (j + mouthRect.left) + 2] = buf2[i * WidthPix2 + 3 * j + 2];
//		}
//	}
	
	CMyTPS TPS;
	TPS.Create(targetVectorX, targetVectorY, sourceVectorX, sourceVectorY, 0);

	for(i = mouthRect.top - 15; i < mouthRect.bottom + 15; i++)
	{
		for(j = mouthRect.left - 15; j < mouthRect.right + 15; j++)
		{
			TPS.TPSTransform(j, i, tx, ty);

			if(tx > 0 && tx < mouthImage.Width() && ty > 0 && ty < mouthImage.Height())
			{
				buf3[i * WidthPix3 + j * 3] = buf2[(int)(ty) * WidthPix2 + (int)(tx) * 3];
				buf3[i * WidthPix3 + j * 3 + 1] = buf2[(int)(ty) * WidthPix2 + (int)(tx) * 3 + 1];
				buf3[i * WidthPix3 + j * 3 + 2] = buf2[(int)(ty) * WidthPix2 + (int)(tx) * 3 + 2];
			}
		}
	}

	CString tempStr("mode");
	for(i = mouthRect.top - 15; i < mouthRect.bottom + 15; i++)
	{
		for(j = mouthRect.left - 15; j < mouthRect.right + 15; j++)
		{	
			if(buf3[i * WidthPix3 + j * 3] > 220 && buf3[i * WidthPix3 + j * 3 + 1] > 220 && buf3[i * WidthPix3 + j * 3 + 2] > 220)
			   continue;
			else
			{
				if(templatePath.Right(4) == tempStr)
				{
					memset((void *) (buf5 + i * WidthPix5 + 3 * j), 255, 3 * sizeof(BYTE));	
				}
				else
				{
					if(mouthRgn.PtInRegion(CPoint(j, i)))
					{
						buf5[i * WidthPix5 + j * 3] = 169;
						buf5[i * WidthPix5 + j * 3 + 1] = 183;
						buf5[i * WidthPix5 + j * 3 + 2] = 225;
					}
				}
			}
		}
	}
	
	cvSmooth(m_TargetImgCopy.GetImage(), m_TargetImgCopy.GetImage(), CV_GAUSSIAN, 3, 3, 1);
	for(i = mouthRect.top - 15; i < mouthRect.bottom + 15; i++)
	{
		for(j = mouthRect.left - 15; j < mouthRect.right + 15; j++)
		{				
			if(RetImg(j, i, 0) < 220)
			{
				buf5[i * WidthPix5 + j * 3] = RetImg(j, i, 0);
				buf5[i * WidthPix5 + j * 3 + 1] = RetImg(j, i, 1);
				buf5[i * WidthPix5 + j * 3 + 2] = RetImg(j, i, 2);
			}
		}
	}

	for(i = mouthRect.top - 15; i < mouthRect.bottom + 15; i++)
	{
		for(j = mouthRect.left - 15; j < mouthRect.right + 15; j++)
		{
			buf1[i * WidthPix1 + j * 3] = buf3[i * WidthPix3 + j * 3];
			buf1[i * WidthPix1 + j * 3 + 1] = buf3[i * WidthPix3 + j * 3 + 1];
			buf1[i * WidthPix1 + j * 3 + 2] = buf3[i * WidthPix3 + j * 3 + 2];

			buf6[i * WidthPix6 + j * 3] = buf5[i * WidthPix5 + j * 3];
			buf6[i * WidthPix6 + j * 3 + 1] = buf5[i * WidthPix5 + j * 3 + 1];
			buf6[i * WidthPix6 + j * 3 + 2] = buf5[i * WidthPix5 + j * 3 + 2];
		}
	}
	m_TargetImgCopy.Create(m_TargetImg.Width(), m_TargetImg.Height(), 24);
	cvCopy(m_TargetImg.GetImage(), m_TargetImgCopy.GetImage());

	m_nRecordIndex++;
	m_nRecordMaxIndex = m_nRecordIndex;
	CString tempName;
	tempName.Format("\\temp\\portrait%d.jpg", m_nRecordIndex);
	m_TargetImg.Save(m_sPath + tempName);

	Invalidate(FALSE);
}

void CPortraitEditingDlg::RenderNose(int index, CString &templatePath) 
{
	// TODO: Add your control notification handler code here
	int i, j;
	CRgn    leftEyeRgn, rightEyeRgn;
	CRect   leftEyeRect, rightEyeRect;
	CPoint  leftEyePoint[8], rightEyePoint[8];

	for(i = 0; i < 8; i++)
	{
		leftEyePoint[i].x = m_dpLandMarks[i].x;
		leftEyePoint[i].y = m_dpLandMarks[i].y;	
	}
	for(i = 0; i < 8; i++)
	{
		rightEyePoint[i].x = m_dpLandMarks[8 + i].x;
		rightEyePoint[i].y = m_dpLandMarks[8 + i].y;
	}
	leftEyeRgn.CreatePolygonRgn(leftEyePoint, 8, WINDING);
	leftEyeRgn.GetRgnBox(leftEyeRect);
	leftEyeRect.right += 8;
	leftEyeRect.bottom += 5;
	rightEyeRgn.CreatePolygonRgn(rightEyePoint, 8, WINDING);
	rightEyeRgn.GetRgnBox(rightEyeRect);
	rightEyeRect.left -= 8;
	rightEyeRect.bottom += 5;
	
	CRgn	 noseRgn;
	CRect    noseRect;
	
	CPoint nosePoint[13];
	for(i = 0; i < 13; i++)
	{
		nosePoint[i].x = m_ptArrLandMarks[61 + i].x;
		nosePoint[i].y = m_ptArrLandMarks[61 + i].y;
	}
	noseRgn.CreatePolygonRgn(nosePoint, 13, WINDING);
	noseRgn.GetRgnBox(noseRect);


	CvvImage noseImage;
	CString path(templatePath + "\\nose");
	CString fileName;

	fileName.Format("\\nose%d", index + 1);
	noseImage.Load(path + fileName + ".jpg");

	int extra = 6;
	double RatioX = (noseRect.Width() + extra) * 1.0 / (noseImage.Width() * 1.0);
	double RatioY = noseRect.Height() * 1.0 / (noseImage.Height() * 1.0);

	ImageZoom(noseImage, RatioX, RatioY);
	cvSmooth(noseImage.GetImage(), noseImage.GetImage(), CV_GAUSSIAN, 3, 3, 1);

	vector< vector<CPoint> > pointVVector;
	fileName.Format("\\keyPoints%d.pts", index + 1);
	LoadTemplate(path + fileName, pointVVector);
	CImg<unsigned char> RetImg(m_TargetImg.Width(), m_TargetImg.Height(), 1, 3, 255);
	unsigned char black[3] = {10, 10, 10};
	for(i = 0; i < pointVVector.size(); i++)
	{
		 for(j = 0; j < pointVVector[i].size() - 1; j++)
		 { 
			 int px1 = pointVVector[i][j].x * RatioX + noseRect.left - extra/2;  
			 int py1 = pointVVector[i][j].y * RatioY + noseRect.top;
			 int px2 = pointVVector[i][j + 1].x * RatioX + noseRect.left - extra/2; 
			 int py2 = pointVVector[i][j + 1].y * RatioY + noseRect.top;
	
			 RetImg.draw_line(px1, py1, px2, py2, black);
		 }
	}
	RetImg.save("c:\\nose.bmp");

	BYTE *buf1 = (BYTE *)m_TargetImg.GetImage()->imageData;
	BYTE *buf2 = (BYTE *)noseImage.GetImage()->imageData;
	BYTE *buf3 = (BYTE *)m_TargetImgCopy.GetImage()->imageData;

	int WidthPix1 = WIDTHBYTES(m_TargetImg.Width() * 24);
	int WidthPix2 = WIDTHBYTES(noseImage.Width() * 24);
	int WidthPix3 = WIDTHBYTES(m_TargetImgCopy.Width() * 24);

	BYTE *buf6 = (BYTE *)m_RenderImg.GetImage()->imageData;
	int WidthPix6 = WIDTHBYTES(m_RenderImg.Width() * 24);

	for(i = noseRect.top - 5; i < noseRect.bottom + 5; i++)
	{
		for(j = noseRect.left - 5; j < noseRect.right + 5; j++)
		{
			if(!leftEyeRect.PtInRect(CPoint(j, i)) && !rightEyeRect.PtInRect(CPoint(j, i)))
			{
				buf1[i * WidthPix1 + 3 * j] = 255;
				buf1[i * WidthPix1 + 3 * j + 1] = 255;
				buf1[i * WidthPix1 + 3 * j + 2] = 255;

				buf3[i * WidthPix3 + 3 * j] = 255;
				buf3[i * WidthPix3 + 3 * j + 1] = 255;
				buf3[i * WidthPix3 + 3 * j + 2] = 255;

				buf6[i * WidthPix6 + 3 * j] = RetImg(j, i, 0);
				buf6[i * WidthPix6 + 3 * j + 1] = RetImg(j, i, 1);
				buf6[i * WidthPix6 + 3 * j + 2] = RetImg(j, i, 2);
			}
		}
	}
	
	for(i = 0; i < noseImage.Height(); i++)
	{
		for(j = 0; j < noseImage.Width(); j++)
		{
			if(!leftEyeRect.PtInRect(CPoint(j + noseRect.left - extra/2, i + noseRect.top)) 
			&& !rightEyeRect.PtInRect(CPoint(j + noseRect.left - extra/2, i + noseRect.top)))
			{
				buf1[(i + noseRect.top) * WidthPix1 + 3 * (j + noseRect.left - extra/2)] = buf2[i * WidthPix2 + 3 * j];
				buf1[(i + noseRect.top) * WidthPix1 + 3 * (j + noseRect.left - extra/2) + 1] = buf2[i * WidthPix2 + 3 * j + 1];
				buf1[(i + noseRect.top) * WidthPix1 + 3 * (j + noseRect.left - extra/2) + 2] = buf2[i * WidthPix2 + 3 * j + 2];
				
				buf3[(i + noseRect.top) * WidthPix3 + 3 * (j + noseRect.left - extra/2)] = buf2[i * WidthPix2 + 3 * j];
				buf3[(i + noseRect.top) * WidthPix3 + 3 * (j + noseRect.left - extra/2) + 1] = buf2[i * WidthPix2 + 3 * j + 1];
				buf3[(i + noseRect.top) * WidthPix3 + 3 * (j + noseRect.left - extra/2) + 2] = buf2[i * WidthPix2 + 3 * j + 2];
			}
		}
	}

	m_nRecordIndex++;
	m_nRecordMaxIndex = m_nRecordIndex;
	CString tempName;
	tempName.Format("\\temp\\portrait%d.jpg", m_nRecordIndex);
	m_TargetImg.Save(m_sPath + tempName);

	Invalidate(FALSE);

}

void CPortraitEditingDlg::RenderEye(int index, CString &templatePath) 
{
	// TODO: Add your control notification handler code here
	int i, j;
	CRgn	 leftEyeRgn, rightEyeRgn;
	CRect    leftEyeRect, rightEyeRect;

	vector<double> targetVectorXL;
	vector<double> targetVectorYL;
	vector<double> targetVectorXR;
	vector<double> targetVectorYR;
	targetVectorXL.clear();
	targetVectorYL.clear();
	targetVectorXR.clear();
	targetVectorYR.clear();
	
	CPoint leftEyePoint[8], rightEyePoint[8];
	for(i = 0; i < 8; i++)
	{
		leftEyePoint[i].x = m_dpLandMarks[i].x;
		leftEyePoint[i].y = m_dpLandMarks[i].y;

		targetVectorXL.push_back(m_dpLandMarks[i].x);
		targetVectorYL.push_back(m_dpLandMarks[i].y);
	}
	leftEyeRgn.CreatePolygonRgn(leftEyePoint, 8, WINDING);
	leftEyeRgn.GetRgnBox(leftEyeRect);
	for(i = 0; i < 8; i++)
	{
		rightEyePoint[i].x = m_dpLandMarks[8 + i].x;
		rightEyePoint[i].y = m_dpLandMarks[8 + i].y;

		targetVectorXR.push_back(m_dpLandMarks[8 + i].x);
		targetVectorYR.push_back(m_dpLandMarks[8 + i].y);
	}
	rightEyeRgn.CreatePolygonRgn(rightEyePoint, 8, WINDING);
	rightEyeRgn.GetRgnBox(rightEyeRect);

	CvvImage leftEyeImage, rightEyeImage;
	CString path(templatePath + "\\eye");
	CString fileName;

	vector<double> sourceVectorXL;
	vector<double> sourceVectorYL;
	vector<double> sourceVectorXR;
	vector<double> sourceVectorYR;
	sourceVectorXL.clear();
	sourceVectorYL.clear();
	sourceVectorXR.clear();
	sourceVectorYR.clear();

	fileName.Format("\\leftEye%d", index + 1);
	leftEyeImage.Load(path + fileName + ".jpg");
	LoadTemplate(path + fileName + ".pts", sourceVectorXL, sourceVectorYL);

	fileName.Format("\\rightEye%d", index + 1);
	rightEyeImage.Load(path + fileName + ".jpg");
	LoadTemplate(path + fileName + ".pts", sourceVectorXR, sourceVectorYR);


	vector< vector<CPoint> > pointVVector;
	fileName.Format("\\keyPoints%d.pts", index + 1);
	LoadTemplate(path + fileName, pointVVector);
	CImg<unsigned char> RetImg(m_TargetImg.Width(), m_TargetImg.Height(), 1, 3, 255);
	unsigned char black[3] = {10, 10, 10};

	CMyTPS myTPS;
	myTPS.Create(sourceVectorXL, sourceVectorYL, targetVectorXL, targetVectorYL, 0);
	double tx, ty;
	for(i = 0; i < pointVVector.size(); i++)
	{
		 for(j = 0; j < pointVVector[i].size() - 1; j++)
		 { 
			 myTPS.TPSTransform(pointVVector[i][j].x, pointVVector[i][j].y, tx, ty);
			 int px1 = (int)tx;  
			 int py1 = (int)ty;

			 myTPS.TPSTransform(pointVVector[i][j + 1].x, pointVVector[i][j + 1].y, tx, ty);
			 int px2 = (int)tx;  
			 int py2 = (int)ty;
	
			 RetImg.draw_line(px1, py1, px2, py2, black);
		 }
	}
	myTPS.Create(sourceVectorXR, sourceVectorYR, targetVectorXR, targetVectorYR, 0);
	for(i = 0; i < pointVVector.size(); i++)
	{
		 for(j = 0; j < pointVVector[i].size() - 1; j++)
		 { 
			 myTPS.TPSTransform(rightEyeImage.Width() - pointVVector[i][j].x, pointVVector[i][j].y, tx, ty);
			 int px1 = (int)tx;  
			 int py1 = (int)ty;

			 myTPS.TPSTransform(rightEyeImage.Width() - pointVVector[i][j + 1].x, pointVVector[i][j + 1].y, tx, ty);
			 int px2 = (int)tx;  
			 int py2 = (int)ty;
	
			 RetImg.draw_line(px1, py1, px2, py2, black);
		 }
	}

//	int extra = 4;
//	if(index == 2)
//		extra = 6;
//	if(index == 3)
//		extra = 8;
//	
//	double leftRatioX = leftEyeRect.Width() * 1.0 / (leftEyeImage.Width() * 1.0);
//	double leftRatioY = (leftEyeRect.Height() + extra) * 1.0 / (leftEyeImage.Height() * 1.0);
//	double rightRatioX = rightEyeRect.Width() * 1.0 / (rightEyeImage.Width() * 1.0);
//	double rightRatioY = (rightEyeRect.Height() + extra) * 1.0 / (rightEyeImage.Height() * 1.0); 
	//ImageZoom(leftEyeImage, leftRatioX, leftRatioY);
	//ImageZoom(rightEyeImage, rightRatioX, rightRatioY);
	//cvSmooth(leftEyeImage.GetImage(), leftEyeImage.GetImage(), CV_GAUSSIAN, 3, 3, 1);
	//cvSmooth(rightEyeImage.GetImage(), rightEyeImage.GetImage(), CV_GAUSSIAN, 3, 3, 1);

	BYTE *buf1 = (BYTE *)m_TargetImg.GetImage()->imageData;
	BYTE *buf2 = (BYTE *)leftEyeImage.GetImage()->imageData;
	BYTE *buf3 = (BYTE *)rightEyeImage.GetImage()->imageData;
	BYTE *buf4 = (BYTE *)m_TargetImgCopy.GetImage()->imageData;

	int WidthPix1 = WIDTHBYTES(m_TargetImg.Width() * 24);
	int WidthPix2 = WIDTHBYTES(leftEyeImage.Width() * 24);
	int WidthPix3 = WIDTHBYTES(rightEyeImage.Width() * 24);
	int WidthPix4 = WIDTHBYTES(m_TargetImgCopy.Width() * 24);

	BYTE *buf6 = (BYTE *)m_RenderImg.GetImage()->imageData;
	int WidthPix6 = WIDTHBYTES(m_RenderImg.Width() * 24);

	for(i = leftEyeRect.top - 8; i < leftEyeRect.bottom + 10; i++)
	{
		for(j = leftEyeRect.left - 10; j < leftEyeRect.right + 10; j++)
		{
			if(buf1[i * WidthPix1 + 3 * j] == 0 || buf6[i * WidthPix6 + 3 * j] == 0)
			{
				if(leftEyeRgn.PtInRegion(CPoint(j, i)))
				{
					RetImg(j, i, 0) = 0;
					RetImg(j, i, 1) = 0;
					RetImg(j, i, 2) = 0;
				}
			}

			buf1[i * WidthPix1 + 3 * j] = 255;
			buf1[i * WidthPix1 + 3 * j + 1] = 255;
			buf1[i * WidthPix1 + 3 * j + 2] = 255;

			buf4[i * WidthPix4 + 3 * j] = 255;
			buf4[i * WidthPix4 + 3 * j + 1] = 255;
			buf4[i * WidthPix4 + 3 * j + 2] = 255;
		}
	}
//	for(i = 0; i < leftEyeImage.Height(); i++)
//	{
//		for(j = 0; j < leftEyeImage.Width(); j++)
//		{
//			buf1[(i + leftEyeRect.top - extra/2) * WidthPix1 + 3 * (j + leftEyeRect.left)] = buf2[i * WidthPix2 + 3 * j];
//			buf1[(i + leftEyeRect.top - extra/2) * WidthPix1 + 3 * (j + leftEyeRect.left) + 1] = buf2[i * WidthPix2 + 3 * j + 1];
//			buf1[(i + leftEyeRect.top - extra/2) * WidthPix1 + 3 * (j + leftEyeRect.left) + 2] = buf2[i * WidthPix2 + 3 * j + 2];
//
//			buf4[(i + leftEyeRect.top - extra/2) * WidthPix4 + 3 * (j + leftEyeRect.left)] = buf2[i * WidthPix2 + 3 * j];
//			buf4[(i + leftEyeRect.top - extra/2) * WidthPix4 + 3 * (j + leftEyeRect.left) + 1] = buf2[i * WidthPix2 + 3 * j + 1];
//			buf4[(i + leftEyeRect.top - extra/2) * WidthPix4 + 3 * (j + leftEyeRect.left) + 2] = buf2[i * WidthPix2 + 3 * j + 2];
//		}
//	}
	CMyTPS TPS;
	TPS.Create(targetVectorXL, targetVectorYL, sourceVectorXL, sourceVectorYL, 0);

	for(i = leftEyeRect.top - 8; i < leftEyeRect.bottom + 10; i++)
	{
		for(j = leftEyeRect.left - 10; j < leftEyeRect.right + 10; j++)
		{
			TPS.TPSTransform(j, i, tx, ty);

			if(tx > 0 && tx < leftEyeImage.Width() && ty > 0 && ty < leftEyeImage.Height())
			{
				buf4[i * WidthPix4 + j * 3] = buf2[(int)(ty) * WidthPix2 + (int)(tx) * 3];
				buf4[i * WidthPix4 + j * 3 + 1] = buf2[(int)(ty) * WidthPix2 + (int)(tx) * 3 + 1];
				buf4[i * WidthPix4 + j * 3 + 2] = buf2[(int)(ty) * WidthPix2 + (int)(tx) * 3 + 2];			
			}
		}
	}
	cvSmooth(m_TargetImgCopy.GetImage(), m_TargetImgCopy.GetImage(), CV_GAUSSIAN, 3, 3, 1);
	for(i = leftEyeRect.top - 8; i < leftEyeRect.bottom + 10; i++)
	{
		for(j = leftEyeRect.left - 10; j < leftEyeRect.right + 10; j++)
		{
			buf1[i * WidthPix1 + j * 3] = buf4[i * WidthPix4 + j * 3];
			buf1[i * WidthPix1 + j * 3 + 1] = buf4[i * WidthPix4 + j * 3 + 1];
			buf1[i * WidthPix1 + j * 3 + 2] = buf4[i * WidthPix4 + j * 3 + 2];	
			
			buf6[i * WidthPix6 + j * 3] = RetImg(j, i, 0);
			buf6[i * WidthPix6 + j * 3 + 1] = RetImg(j, i, 1);
			buf6[i * WidthPix6 + j * 3 + 2] = RetImg(j, i, 2);
		}
	}
	m_TargetImgCopy.Create(m_TargetImg.Width(), m_TargetImg.Height(), 24);
	cvCopy(m_TargetImg.GetImage(), m_TargetImgCopy.GetImage());

	for(i = rightEyeRect.top - 8; i < rightEyeRect.bottom + 10; i++)
	{
		for(j = rightEyeRect.left - 10; j < rightEyeRect.right + 10; j++)
		{
			if(buf1[i * WidthPix1 + 3 * j] == 0 || buf6[i * WidthPix6 + 3 * j] == 0)
			{
				if(rightEyeRgn.PtInRegion(CPoint(j, i)))
				{
					RetImg(j, i, 0) = 0;
					RetImg(j, i, 1) = 0;
					RetImg(j, i, 2) = 0;
				}
			}

			buf1[i * WidthPix1 + 3 * j] = 255;
			buf1[i * WidthPix1 + 3 * j + 1] = 255;
			buf1[i * WidthPix1 + 3 * j + 2] = 255;

			buf4[i * WidthPix4 + 3 * j] = 255;
			buf4[i * WidthPix4 + 3 * j + 1] = 255;
			buf4[i * WidthPix4 + 3 * j + 2] = 255;
		}
	}

//	for(i = 0; i < rightEyeImage.Height(); i++)
//	{
//		for(j = 0; j < rightEyeImage.Width(); j++)
//		{
//			buf1[(i + rightEyeRect.top - extra/2) * WidthPix1 + 3 * (j + rightEyeRect.left)] = buf3[i * WidthPix3 + 3 * j];
//			buf1[(i + rightEyeRect.top - extra/2) * WidthPix1 + 3 * (j + rightEyeRect.left) + 1] = buf3[i * WidthPix3 + 3 * j + 1];
//			buf1[(i + rightEyeRect.top - extra/2) * WidthPix1 + 3 * (j + rightEyeRect.left) + 2] = buf3[i * WidthPix3 + 3 * j + 2];
//
//			buf4[(i + rightEyeRect.top - extra/2) * WidthPix4 + 3 * (j + rightEyeRect.left)] = buf3[i * WidthPix3 + 3 * j];
//			buf4[(i + rightEyeRect.top - extra/2) * WidthPix4 + 3 * (j + rightEyeRect.left) + 1] = buf3[i * WidthPix3 + 3 * j + 1];
//			buf4[(i + rightEyeRect.top - extra/2) * WidthPix4 + 3 * (j + rightEyeRect.left) + 2] = buf3[i * WidthPix3 + 3 * j + 2];
//		}
//	}
	TPS.Create(targetVectorXR, targetVectorYR, sourceVectorXR, sourceVectorYR, 0);
	for(i = rightEyeRect.top - 8; i < rightEyeRect.bottom + 10; i++)
	{
		for(j = rightEyeRect.left - 10; j < rightEyeRect.right + 10; j++)
		{
			TPS.TPSTransform(j, i, tx, ty);

			if(tx > 0 && tx < leftEyeImage.Width() && ty > 0 && ty < leftEyeImage.Height())
			{
				buf4[i * WidthPix4 + j * 3] = buf3[(int)(ty) * WidthPix3 + (int)(tx) * 3];
				buf4[i * WidthPix4 + j * 3 + 1] = buf3[(int)(ty) * WidthPix3 + (int)(tx) * 3 + 1];
				buf4[i * WidthPix4 + j * 3 + 2] = buf3[(int)(ty) * WidthPix3 + (int)(tx) * 3 + 2];
			}
		}
	}
	cvSmooth(m_TargetImgCopy.GetImage(), m_TargetImgCopy.GetImage(), CV_GAUSSIAN, 3, 3, 1);
	for(i = rightEyeRect.top - 8; i < rightEyeRect.bottom + 10; i++)
	{
		for(j = rightEyeRect.left - 10; j < rightEyeRect.right + 10; j++)
		{
			buf1[i * WidthPix1 + j * 3] = buf4[i * WidthPix4 + j * 3];
			buf1[i * WidthPix1 + j * 3 + 1] = buf4[i * WidthPix4 + j * 3 + 1];
			buf1[i * WidthPix1 + j * 3 + 2] = buf4[i * WidthPix4 + j * 3 + 2];	
			
			buf6[i * WidthPix6 + j * 3] = RetImg(j, i, 0);
			buf6[i * WidthPix6 + j * 3 + 1] = RetImg(j, i, 1);
			buf6[i * WidthPix6 + j * 3 + 2] = RetImg(j, i, 2);
		}
	}
	m_TargetImgCopy.Create(m_TargetImg.Width(), m_TargetImg.Height(), 24);
	cvCopy(m_TargetImg.GetImage(), m_TargetImgCopy.GetImage());

	m_nRecordIndex++;
	m_nRecordMaxIndex = m_nRecordIndex;
	CString tempName;
	tempName.Format("\\temp\\portrait%d.jpg", m_nRecordIndex);
	m_TargetImg.Save(m_sPath + tempName);

	Invalidate(FALSE);

}

void CPortraitEditingDlg::RenderFaceContour(int index, CString &templatePath) 
{
	// TODO: Add your control notification handler code here
	int i, j;
	CRgn faceContourRgn;
	CRect faceContourRect;
	CPoint temPoints[19];
	CvPoint FacePoint[19];

	CRgn    leftEyeRgn, rightEyeRgn;
	CRect   leftEyeRect, rightEyeRect;
	CPoint  leftEyePoint[8], rightEyePoint[8];

	for(i = 0; i < 8; i++)
	{
		leftEyePoint[i].x = m_dpLandMarks[i].x;
		leftEyePoint[i].y = m_dpLandMarks[i].y;	
	}

	for(i = 0; i < 8; i++)
	{
		rightEyePoint[i].x = m_dpLandMarks[8 + i].x;
		rightEyePoint[i].y = m_dpLandMarks[8 + i].y;
	}

	leftEyeRgn.CreatePolygonRgn(leftEyePoint, 8, WINDING);
	leftEyeRgn.GetRgnBox(leftEyeRect);
	leftEyeRect.left -= 8;
	leftEyeRect.top -= 8;
	leftEyeRect.right += 8;
	leftEyeRect.bottom += 8;
	rightEyeRgn.CreatePolygonRgn(rightEyePoint, 8, WINDING);
	rightEyeRgn.GetRgnBox(rightEyeRect);
	rightEyeRect.left -= 8;
	rightEyeRect.top -= 8;
	rightEyeRect.right += 8;
	rightEyeRect.bottom += 8;

	CRgn	 mouthRgn;
	CRect    mouthRect;
	CPoint mouthPoint[12];
	for(i = 0; i < 12; i++)
	{
		mouthPoint[i].x = m_ptArrLandMarks[74 + i].x;
		mouthPoint[i].y = m_ptArrLandMarks[74 + i].y;
	}
	mouthRgn.CreatePolygonRgn(mouthPoint, 12, WINDING);
	mouthRgn.GetRgnBox(mouthRect);
	mouthRect.left -= 8;
	mouthRect.top -= 8;
	mouthRect.right += 8;
	mouthRect.bottom += 8;
	
	CRgn	 noseRgn;
	CRect    noseRect;
	CPoint nosePoint[13];
	for(i = 0; i < 13; i++)
	{
		nosePoint[i].x = m_ptArrLandMarks[61 + i].x;
		nosePoint[i].y = m_ptArrLandMarks[61 + i].y;
	}
	noseRgn.CreatePolygonRgn(nosePoint, 13, WINDING);
	noseRgn.GetRgnBox(noseRect);
	noseRect.left -= 2;
	noseRect.top -= 2;
	noseRect.right += 2;
	noseRect.bottom += 2;
	
	vector<double> targetVectorX;
	vector<double> targetVectorY;
	targetVectorX.clear();
	targetVectorY.clear();
	for(i = 0; i < 19; i++)
	{
		targetVectorX.push_back(m_dpLandMarks[i + 70].x);
		targetVectorY.push_back(m_dpLandMarks[i + 70].y);
		
		temPoints[i].x = m_dpLandMarks[i + 70].x;
		temPoints[i].y = m_dpLandMarks[i + 70].y;
		
		FacePoint[i].x = m_dpLandMarks[70 + i].x;
		FacePoint[i].y = m_dpLandMarks[70 + i].y;
	}

	faceContourRgn.CreatePolygonRgn(temPoints, 19, WINDING);
	faceContourRgn.GetRgnBox(faceContourRect);
	
	for(i = 0; i < 18; i++)
	{
		cvLine(m_TargetImg.GetImage(), FacePoint[i], FacePoint[i + 1], CV_RGB(255,255,255), 2, CV_AA);
		cvLine(m_TargetImgCopy.GetImage(), FacePoint[i], FacePoint[i + 1], CV_RGB(255,255,255), 2, CV_AA);
	}

	 CvvImage sourceImage;
	 vector<double> sourceVectorX;
	 vector<double> sourceVectorY;

	 CString path(templatePath + "\\faceContour");
	 CString fileName;

	 sourceVectorX.clear();
	 sourceVectorY.clear();

	 fileName.Format("\\faceContour%d", index + 1);
	 sourceImage.Load(path + fileName + ".jpg");
	 LoadTemplate(path + fileName + ".pts", sourceVectorX, sourceVectorY);

	 for(i = 0; i < sourceVectorX.size(); i++)
	 {
		 if(i < 10)
			 sourceVectorX[i] += 3;
		 if(i > 10)
			 sourceVectorX[i] -= 3;
	 }

	CMyTPS  TPS;
	TPS.Create(targetVectorX, targetVectorY, sourceVectorX, sourceVectorY, 0);

	CvvImage tempImg;
	tempImg.Destroy();
	tempImg.Create(m_TargetImg.Width(), m_TargetImg.Height(), 24);
	ImageChanged(tempImg.GetImage(), 255);
	ImageChanged(m_RegionImg.GetImage(), 255);
	
	BYTE *buf1 = (BYTE *)m_TargetImg.GetImage()->imageData;
	int WidthPix1 = WIDTHBYTES(m_TargetImg.Width() * 24);
	BYTE *buf2 = (BYTE *)sourceImage.GetImage()->imageData;
	int WidthPix2 = WIDTHBYTES(sourceImage.Width() * 24);
	BYTE *buf3 = (BYTE *)m_TargetImgCopy.GetImage()->imageData;
	int WidthPix3 = WIDTHBYTES(m_TargetImgCopy.Width() * 24);
	BYTE *buf4 = (BYTE *)tempImg.GetImage()->imageData;
	int WidthPix4 = WIDTHBYTES(tempImg.Width() * 24);
	BYTE *buf5 = (BYTE *)m_RegionImg.GetImage()->imageData;
	int WidthPix5 = WIDTHBYTES(m_RegionImg.Width() * 24);
	BYTE *buf6 = (BYTE *)m_RenderImg.GetImage()->imageData;
	int WidthPix6 = WIDTHBYTES(m_RenderImg.Width() * 24);
	
	double tx, ty;
	for(i = faceContourRect.top; i < faceContourRect.bottom; i++)
	{
		for(j = faceContourRect.left; j < faceContourRect.right; j++)
		{
			TPS.TPSTransform(j, i, tx, ty);
			if(tx > 0 && tx < sourceImage.Width() && ty > 0 && ty < sourceImage.Height())
			{
				buf4[i * WidthPix4 + j * 3] = buf2[(int)(ty) * WidthPix2 + (int)(tx) * 3];
				buf4[i * WidthPix4 + j * 3 + 1] = buf2[(int)(ty) * WidthPix2 + (int)(tx) * 3 + 1];
				buf4[i * WidthPix4 + j * 3 + 2] = buf2[(int)(ty) * WidthPix2 + (int)(tx) * 3 + 2];
			}
		}
	}
	
	 TPS.Create(sourceVectorX, sourceVectorY, targetVectorX, targetVectorY, 0);
	 
	 vector< vector<CPoint> > pointVVector;
	 fileName.Format("\\keyPoints%d.pts", index + 1);
	 LoadTemplate(path + fileName, pointVVector);
	 double tx1,ty1,tx2,ty2;

	 CImg<unsigned char> RetImg(m_TargetImg.Width(), m_TargetImg.Height(), 1, 3, 255);
	 CImg<unsigned char> flag(m_TargetImg.Width(), m_TargetImg.Height(), 1, 1, 0);
	 CImg<unsigned char> FlagImg(m_TargetImg.Width(), m_TargetImg.Height(), 1, 1, 0);
	 CImg<unsigned char> AlphaImg;	
	 unsigned char black[3] = {10, 10, 10};
	 for(i = 0; i < pointVVector.size(); i++)
	 {
		 vector<DoublePoint> MidPoints;
		 for(j = 0; j < pointVVector[i].size() - 1; j++)
		 { 
			 TPS.TPSTransform(pointVVector[i][j].x, pointVVector[i][j].y, tx1, ty1);
			 TPS.TPSTransform(pointVVector[i][j + 1].x, pointVVector[i][j + 1].y, tx2, ty2);
	
			 CPoint Zpoint1, Zpoint2;
			 Zpoint1.x = (int)(tx1 + 0.5);
			 Zpoint1.y = (int)(ty1 + 0.5);
			 Zpoint2.x = (int)(tx2 + 0.5);
			 Zpoint2.y = (int)(ty2 + 0.5);

			 RetImg.draw_line(Zpoint1.x, Zpoint1.y, Zpoint2.x, Zpoint2.y, black);
			 
			 DoublePoint dp;
			 dp.x = Zpoint1.x;
			 dp.y = Zpoint1.y;
			 if(pointVVector[i].size() < 5)
			 {
				 MidPoints.push_back(dp);
				 AlphaImg.load(m_sPath + "\\brush\\-1.bmp");
			 }
			 else
			 {
				 if(j % 2 == 0 && j != pointVVector[i].size() - 2)
				 {
					 MidPoints.push_back(dp);
					 AlphaImg.load(m_sPath + "\\brush\\000.bmp");
				 }
			 }

			 if(j == pointVVector[i].size() - 2)
			 {
				 dp.x = Zpoint2.x;
				 dp.y = Zpoint2.y;
				 MidPoints.push_back(dp);
			 }
		 }

			CCurveMorphing CMorphing;
			if(MidPoints.size() > 5)
				CMorphing.MorphingToCurve(MidPoints, 20, 10, 10, AlphaImg, 2, BrushImg, flag, FlagImg);
			
			MidPoints.clear();
	 }
	 BrushImg.save("D:\\BrushImg.bmp");
	 for(i = 0; i < m_TargetImg.Height(); i++)
	 {
		 for(j = 0; j < m_TargetImg.Width(); j++)
		 {
			 if(BrushImg(j, i, 0) < 220)
			 {
				 buf4[i * WidthPix4 + j * 3] = BrushImg(j, i, 0);
				 buf4[i * WidthPix4 + j * 3 + 1] = BrushImg(j, i, 1);
				 buf4[i * WidthPix4 + j * 3 + 2] = BrushImg(j, i, 2);
			 }

			 if(RetImg(j, i, 0) < 220)
			 {
				 buf5[i * WidthPix5 + j * 3] = RetImg(j, i, 0);
				 buf5[i * WidthPix5 + j * 3 + 1] = RetImg(j, i, 1);
				 buf5[i * WidthPix5 + j * 3 + 2] = RetImg(j, i, 2);
			 }
		 }
	 }
	 cvSmooth(tempImg.GetImage(), tempImg.GetImage(), CV_GAUSSIAN, 3, 3, 1);

	 cimg_mapXY(BrushImg, x, y)
	 {
		BrushImg(x, y, 0) = 255;
		BrushImg(x, y, 1) = 255;
		BrushImg(x, y, 2) = 255;
	 }
	 cimg_mapXY(RetImg, xx, yy)
	 {
		RetImg(xx, yy, 0) = 255;
		RetImg(xx, yy, 1) = 255;
		RetImg(xx, yy, 2) = 255;
	 }

	for(i = faceContourRect.top - 5; i < faceContourRect.bottom + 5; i++)
	{
		for(j = faceContourRect.left - 5; j < faceContourRect.right + 5; j++)
		{	
			if((faceContourRgn.PtInRegion(CPoint(j + 3,i)) || faceContourRgn.PtInRegion(CPoint(j - 3,i)) || faceContourRgn.PtInRegion(CPoint(j,i - 3)))
			&& !leftEyeRect.PtInRect(CPoint(j,i)) && !rightEyeRect.PtInRect(CPoint(j,i)) && !noseRect.PtInRect(CPoint(j,i)) && !mouthRect.PtInRect(CPoint(j,i)))
			{
				buf1[i * WidthPix1 + j * 3] = buf4[i * WidthPix4 + j * 3];
				buf1[i * WidthPix1 + j * 3 + 1] = buf4[i * WidthPix4 + j * 3 + 1];
				buf1[i * WidthPix1 + j * 3 + 2] = buf4[i * WidthPix4 + j * 3 + 2];

				buf3[i * WidthPix3 + j * 3] = buf4[i * WidthPix4 + j * 3];
				buf3[i * WidthPix3 + j * 3 + 1] = buf4[i * WidthPix4 + j * 3 + 1];
				buf3[i * WidthPix3 + j * 3 + 2] = buf4[i * WidthPix4 + j * 3 + 2];

				buf6[i * WidthPix6 + j * 3] = buf5[i * WidthPix5 + j * 3];
				buf6[i * WidthPix6 + j * 3 + 1] = buf5[i * WidthPix5 + j * 3 + 1];
				buf6[i * WidthPix6 + j * 3 + 2] = buf5[i * WidthPix5 + j * 3 + 2];
			}
		}
	}
	
	ConncetContour(m_skinVector, m_TargetImg.GetImage(), CV_RGB(0,0,0), 1, true);
	ConncetContour(m_skinVector, m_TargetImgCopy.GetImage(), CV_RGB(0,0,0), 1, true);

	m_nRecordIndex++;
	m_nRecordMaxIndex = m_nRecordIndex;
	CString tempName;
	tempName.Format("\\temp\\portrait%d.jpg", m_nRecordIndex);
	m_TargetImg.Save(m_sPath + tempName);

	Invalidate(FALSE);
}

void CPortraitEditingDlg::RenderHair(int index, CString &templatePath) 

{
	// TODO: Add your control notification handler code here
	 int i, j;

	 vector<double> targetVectorX;
	 vector<double> targetVectorY;
	 targetVectorX.clear();
	 targetVectorY.clear();
	 targetVectorX = hairVectorX;
	 targetVectorY = hairVectorY;

	 CvvImage sourceImage;
	 vector<double> sourceHairVectorX;
	 vector<double> sourceHairVectorY;
	 vector<double> sourceVectorX;
	 vector<double> sourceVectorY;
	 vector<double> curX;
	 vector<double>	curY;

	 CSCParam param(type0);
	 param.nIter = 3;
	 int loop, size;

	 CString path(templatePath + "\\hair");
	 CString styleName;
	 CString fileName;

	 CDLL_ShapeContext matchShapeContext;
	 sourceHairVectorX.clear();
	 sourceHairVectorY.clear();
	 sourceVectorX.clear();
	 sourceVectorY.clear();
	 curX.clear();
	 curY.clear();

	 styleName.Format("\\style%d", hairIndex[0] + 1);
	 fileName.Format("\\hair%d", index + 1);
	 sourceImage.Load(path + styleName + fileName + ".jpg");
	 LoadTemplate(path + styleName + fileName + ".pts", sourceHairVectorX, sourceHairVectorY);

  	 int hairSize = sourceHairVectorX.size() + 1;
	 CShape sourShape, destShape;
 	 sourShape.Resize(hairSize);
	 for(i = 0; i < hairSize; i++)
	 {
		if(i < hairSize - 1)
		{
			sourShape.PointX(i) = sourceHairVectorX[i];
			sourShape.PointY(i) = sourceHairVectorY[i];
		}
		else
		{
			sourShape.PointX(i) = sourceHairVectorX[0];
			sourShape.PointY(i) = sourceHairVectorY[0];
		}
	
	 }
	 ResampleCurve(sourShape, destShape, 20);
	 for(i = 0; i < destShape.PointNum(); i++)
	 {
		sourceVectorX.push_back(destShape.PointX(i));
		sourceVectorY.push_back(destShape.PointY(i));
	 }

	matchShapeContext.Initialization(targetVectorX, targetVectorY, sourceVectorX, sourceVectorY, param);
	matchShapeContext.DoMatching();
	//matchShapeContext.GetUpdateData(targetVectorX, targetVectorY, sourceVectorX, sourceVectorY, curX, curY, size, loop);

	CvvImage tempImg;
	tempImg.Destroy();
	tempImg.Create(m_TargetImg.Width(), m_TargetImg.Height(), 24);
	ImageChanged(tempImg.GetImage(), 255);
	ImageChanged(m_RegionImg.GetImage(), 255);

	BYTE *buf1 = (BYTE *)m_TargetImg.GetImage()->imageData;
	int WidthPix1 = WIDTHBYTES(m_TargetImg.Width() * 24);
	BYTE *buf2 = (BYTE *)sourceImage.GetImage()->imageData;
	int WidthPix2 = WIDTHBYTES(sourceImage.Width() * 24);
	BYTE *buf3 = (BYTE *)m_TargetImgCopy.GetImage()->imageData;
	int WidthPix3 = WIDTHBYTES(m_TargetImgCopy.Width() * 24);
	BYTE *buf4 = (BYTE *)tempImg.GetImage()->imageData;
	int WidthPix4 = WIDTHBYTES(tempImg.Width() * 24);
	BYTE *buf5 = (BYTE *)m_RegionImg.GetImage()->imageData;
	int WidthPix5 = WIDTHBYTES(m_RegionImg.Width() * 24);
	BYTE *buf6 = (BYTE *)m_RenderImg.GetImage()->imageData;
	int WidthPix6 = WIDTHBYTES(m_RenderImg.Width() * 24);

	ConncetContour(hairVectorX, hairVectorY, m_TargetImg.GetImage(), CV_RGB(255,255,255), 2);
	ConncetContour(hairVectorX, hairVectorY, m_TargetImgCopy.GetImage(), CV_RGB(255,255,255), 2);
	vector<double> faceVectorX, faceVectorY;
	for(i = 0; i < 19; i++)
	{
		faceVectorX.push_back(m_dpLandMarks[70 + i].x);
		faceVectorY.push_back(m_dpLandMarks[70 + i].y);
	}
	ConncetContour(faceVectorX, faceVectorY, m_TargetImg.GetImage(), CV_RGB(0,0,0), 1, true);
	ConncetContour(faceVectorX, faceVectorY, m_TargetImgCopy.GetImage(), CV_RGB(0,0,0), 1, true);
	
	double tx, ty;
	for(i = hairRect.top; i < hairRect.bottom; i++)
	{
		for(j = hairRect.left; j < hairRect.right; j++)
		{	
			matchShapeContext.GetTPS()->TPSTransform(j, i, tx, ty);
			if(tx > 0 && tx < sourceImage.Width() && ty > 0 && ty < sourceImage.Height())
			{
				buf4[i * WidthPix4 + j * 3] = buf2[(int)(ty) * WidthPix2 + (int)(tx) * 3];
				buf4[i * WidthPix4 + j * 3 + 1] = buf2[(int)(ty) * WidthPix2 + (int)(tx) * 3 + 1];
				buf4[i * WidthPix4 + j * 3 + 2] = buf2[(int)(ty) * WidthPix2 + (int)(tx) * 3 + 2];
			}
		}
	}

	CString tempStr("mode");
	for(i = hairRect.top; i < hairRect.bottom; i++)
	{
		for(j = hairRect.left; j < hairRect.right; j++)
		{	
			if(buf4[i * WidthPix4 + j * 3] > 220 && buf4[i * WidthPix4 + j * 3 + 1] > 220 && buf4[i * WidthPix4 + j * 3 + 2] > 220)
			   continue;
			else
			{
				if(templatePath.Right(4) == tempStr)
				{
					memset((void *) (buf4 + i * WidthPix4 + 3 * j), 255, 3 * sizeof(BYTE));	
				}
				else
				{
					buf4[i * WidthPix4 + j * 3] = 139;
					buf4[i * WidthPix4 + j * 3 + 1] = 199;
					buf4[i * WidthPix4 + j * 3 + 2] = 235;
				}
			}
		}
	}

	for(i = hairRect.top; i < hairRect.bottom; i++)
	{
		for(j = hairRect.left; j < hairRect.right; j++)
		{	
			buf5[i * WidthPix5 + j * 3] = buf4[i * WidthPix4 + j * 3];
			buf5[i * WidthPix5 + j * 3 + 1] = buf4[i * WidthPix4 + j * 3 + 1];
			buf5[i * WidthPix5 + j * 3 + 2] = buf4[i * WidthPix4 + j * 3 + 2];
		}
	}

	 matchShapeContext.Initialization(sourceVectorX, sourceVectorY, targetVectorX, targetVectorY, param);
	 matchShapeContext.DoMatching();
	 vector< vector<CPoint> > pointVVector;
	 fileName.Format("\\keyPoints%d.pts", index + 1);
	 LoadTemplate(path + styleName + fileName, pointVVector);
	 double tx1,ty1,tx2,ty2;

	CImg<unsigned char> RetImg(m_TargetImg.Width(), m_TargetImg.Height(), 1, 3, 255);
	CImg<unsigned char> flag(m_TargetImg.Width(), m_TargetImg.Height(), 1, 1, 0);
	CImg<unsigned char> FlagImg(m_TargetImg.Width(), m_TargetImg.Height(), 1, 1, 0);
	CImg<unsigned char> AlphaImg;		
	unsigned char black[3] = {10,10,10};
	 for(i = 0; i < pointVVector.size(); i++)
	 {
		 vector<DoublePoint> MidPoints;
		 for(j = 0; j < pointVVector[i].size() - 1; j++)
		 { 
			 matchShapeContext.GetTPS()->TPSTransform(pointVVector[i][j].x, pointVVector[i][j].y, tx1, ty1);
			 matchShapeContext.GetTPS()->TPSTransform(pointVVector[i][j + 1].x, pointVVector[i][j + 1].y, tx2, ty2);
	
			 CPoint Zpoint1, Zpoint2;
			 Zpoint1.x = (int)(tx1 + 0.5);
			 Zpoint1.y = (int)(ty1 + 0.5);
			 Zpoint2.x = (int)(tx2 + 0.5);
			 Zpoint2.y = (int)(ty2 + 0.5);

			 
			 RetImg.draw_line(Zpoint1.x, Zpoint1.y, Zpoint2.x, Zpoint2.y, black);

			DoublePoint dp;
			dp.x = Zpoint1.x;
			dp.y = Zpoint1.y;
			if(pointVVector[i].size() < 5)
			{
				MidPoints.push_back(dp);
				AlphaImg.load(m_sPath + "\\brush\\-1.bmp");
			}
			else if(pointVVector[i].size() < 15)
			{
				if(j % 2 == 0 && j != pointVVector[i].size() - 2)
				{
					MidPoints.push_back(dp);
					AlphaImg.load(m_sPath + "\\brush\\0.bmp");
				}
			}
			else if(pointVVector[i].size() < 30)
			{
				if(j % 2 == 0 && j != pointVVector[i].size() - 2)
				{
					MidPoints.push_back(dp);
					AlphaImg.load(m_sPath + "\\brush\\00.bmp");
				}
			}
			else
			{
				if(j % 2 == 0 && j != pointVVector[i].size() - 2)
				{
					MidPoints.push_back(dp);
					AlphaImg.load(m_sPath + "\\brush\\000.bmp");
				}
			}

			if(j == pointVVector[i].size() - 2)
			{
				dp.x = Zpoint2.x;
				dp.y = Zpoint2.y;
				MidPoints.push_back(dp);
			}
		 }

			CCurveMorphing CMorphing;
			CMorphing.MorphingToCurve(MidPoints, 20, 10, 10, AlphaImg, 2, BrushImg, flag, FlagImg);
			MidPoints.clear();
	 }
	 
	for(i = hairRect.top; i < hairRect.bottom; i++)
	{
		for(j = hairRect.left; j < hairRect.right; j++)
		{
			if(BrushImg(j, i, 0) < 220)
			{
				buf4[i * WidthPix4 + j * 3] = BrushImg(j, i, 0);
				buf4[i * WidthPix4 + j * 3 + 1] = BrushImg(j, i, 1);
				buf4[i * WidthPix4 + j * 3 + 2] = BrushImg(j, i, 2);
			}

			
			if(RetImg(j, i, 0) < 220)
			{
				buf5[i * WidthPix5 + j * 3] = RetImg(j, i, 0);
				buf5[i * WidthPix5 + j * 3 + 1] = RetImg(j, i, 1);
				buf5[i * WidthPix5 + j * 3 + 2] = RetImg(j, i, 2);
			}
		}
	}
	cvSmooth(tempImg.GetImage(), tempImg.GetImage(), CV_GAUSSIAN, 3, 3, 1);


	 cimg_mapXY(BrushImg, x, y)
	 {
		BrushImg(x, y, 0) = 255;
		BrushImg(x, y, 1) = 255;
		BrushImg(x, y, 2) = 255;
	 }
	 cimg_mapXY(RetImg, xx, yy)
	 {
		RetImg(xx, yy, 0) = 255;
		RetImg(xx, yy, 1) = 255;
		RetImg(xx, yy, 2) = 255;
	 }

	 CRgn   faceContourRgn;
	 CPoint temPoints[19];
	 for(i = 0; i < 19; i++)
	 {
		temPoints[i].x = m_dpLandMarks[i + 70].x;
		temPoints[i].y = m_dpLandMarks[i + 70].y;
	 }
	 faceContourRgn.CreatePolygonRgn(temPoints, 19, WINDING);

	for(i = hairRect.top; i < hairRect.bottom; i++)
	{
		for(j = hairRect.left; j < hairRect.right; j++)
		{			
			if(!skinRgn.PtInRegion(CPoint(j, i)) && !clothRgn.PtInRegion(CPoint(j, i)))
			{
				buf1[(i + 5) * WidthPix1 + j * 3] = buf4[i * WidthPix4 + j * 3];
				buf1[(i + 5) * WidthPix1 + j * 3 + 1] = buf4[i * WidthPix4 + j * 3 + 1];
				buf1[(i + 5) * WidthPix1 + j * 3 + 2] = buf4[i * WidthPix4 + j * 3 + 2];

				buf3[(i + 5) * WidthPix3 + j * 3] = buf4[i * WidthPix4 + j * 3];
				buf3[(i + 5) * WidthPix3 + j * 3 + 1] = buf4[i * WidthPix4 + j * 3 + 1];
				buf3[(i + 5) * WidthPix3 + j * 3 + 2] = buf4[i * WidthPix4 + j * 3 + 2];
				
				buf6[i * WidthPix6 + j * 3] = buf5[i * WidthPix5 + j * 3];
				buf6[i * WidthPix6 + j * 3 + 1] = buf5[i * WidthPix5 + j * 3 + 1];
				buf6[i * WidthPix6 + j * 3 + 2] = buf5[i * WidthPix5 + j * 3 +  2];
			}
		}
	}
	m_RenderImg.Save("c:\\mix.bmp");

	m_nRecordIndex++;
	m_nRecordMaxIndex = m_nRecordIndex;
	CString tempName;
	tempName.Format("\\temp\\portrait%d.jpg", m_nRecordIndex);
//	m_TargetImg.Save(m_sPath + tempName);
	Invalidate(FALSE);
}

void CPortraitEditingDlg::RenderCollar(int index, CString &templatePath) 
{
	// TODO: Add your control notification handler code here
	 int i, j;
	 vector<double> targetVectorX;
	 vector<double> targetVectorY;
	 targetVectorX.clear();
	 targetVectorY.clear();

	 int mmmmm = clothVectorX.size();

	 double clothArrX[40] = {0};
	 double clothArrY[40] = {0};
	 for(int nnn = 0; nnn < clothVectorX.size() ; nnn++)
	 {
		clothArrX[nnn] = clothVectorX[nnn];
		clothArrY[nnn] = clothVectorY[nnn];
	 }

	 double minY1 = 10000.0;
	 double minY2 = 10000.0;
     int minT1 = 0;
	 int minT2 = 0;
	 for(i = 0 ; i < targetVectorX.size() / 2; i++)
	 {
		 if(minY1 > targetVectorY[i])
			 minY1 = targetVectorY[i];
	 }
	 for(i = targetVectorX.size() / 2; i < targetVectorX.size(); i++)
	 {
		 if(minY2 > targetVectorY[i])
			 minY2 = targetVectorY[i];
	 }
	 for(i = 0; i < targetVectorX.size(); i++)
	 {
		 if(targetVectorY[i] == minY1 && i < targetVectorX.size() / 2)
            minT1 = i;
		 if(targetVectorY[i] == minY2 && i >= targetVectorX.size() / 2)
			minT2 = i;
	 }

	 double maxY0 = 0.0;
	 int maxT0 = 0;
	 for(i = minT1; i <= minT2; i++)
	 {
		 if(maxY0 < targetVectorY[i])
			 maxY0 = targetVectorY[i];
	 }
	 for(i = 0; i < targetVectorX.size(); i++)
	 {
		 if(targetVectorY[i] == maxY0 && i <= minT2 && i >= minT1)
            maxT0 = i;
	 }

	 vector<dPoint> part1Vector, part2Vector;
	 dPoint pp;
	 bool isUp = true;
	 for(i = 0; i < m_skinVector.size(); i++)
	 {
		 if((m_skinVector[i].y >= minY1 && m_skinVector[i].x < targetVectorX[maxT0])
		  ||(m_skinVector[i].y >= minY2 && m_skinVector[i].x >= targetVectorX[maxT0]))
		 {
			isUp = false;
			continue; 
		 }
		 pp.x = m_skinVector[i].x;
		 pp.y = m_skinVector[i].y;

		 if(isUp == true)
			part1Vector.push_back(pp);
		 else
			part2Vector.push_back(pp);
	 }

	 m_skinVector.clear();
	 for(i = 0; i < part1Vector.size() + part2Vector.size(); i++)
	 {
		 if(i < part2Vector.size())
		 {
			 pp.x = part2Vector[i].x;
			 pp.y = part2Vector[i].y;
		 }
		 else
		 {
			 pp.x = part1Vector[i - part2Vector.size()].x;
			 pp.y = part1Vector[i - part2Vector.size()].y;
		 }

		 m_skinVector.push_back(pp);
	 }

	 double backX = m_skinVector.back().x;
	 if(backX < targetVectorX[maxT0])
	 {
		 for(i = minT1; i <= minT2; i++)
		 {
			pp.x = targetVectorX[i];
			pp.y = targetVectorY[i];
			m_skinVector.push_back(pp);
		 }
	 }
	 else
	 {
		 for(i = minT2; i >= minT1; i--)
		 {
			pp.x = targetVectorX[i];
			pp.y = targetVectorY[i];
			m_skinVector.push_back(pp);
		 }
	 }
	 
	int nNum = m_skinVector.size();
	CPoint *skinPoints = new CPoint[nNum];
	for(i = 0; i < nNum; i++)
	{
		skinPoints[i].x = m_skinVector[i].x;
		skinPoints[i].y = m_skinVector[i].y;
	}
	skinRgn.DeleteObject();
	skinRgn.CreatePolygonRgn(skinPoints, nNum, WINDING);
	skinRgn.GetRgnBox(skRect);
	delete [] skinPoints;

	 CvvImage sourceImage;
	 vector<double> sourceClothVectorX;
	 vector<double> sourceClothVectorY;
	 vector<double> sourceVectorX;
	 vector<double> sourceVectorY;
	 vector<double> curX;
	 vector<double>	curY;

	 CSCParam param(type0);
	 param.nIter = 3;
	 int loop, size;

	 CString path(templatePath + "\\cloth");
	 CString styleName;
	 CString fileName;

   	 CDLL_ShapeContext matchShapeContext;
	 
	 sourceClothVectorX.clear();
	 sourceClothVectorY.clear();
	 sourceVectorX.clear();
	 sourceVectorY.clear();
	 curX.clear();
	 curY.clear();

	 styleName.Format("\\style%d", clothIndex[0] + 1);
	 fileName.Format("\\cloth%d", index + 1);
	 sourceImage.Load(path + styleName + fileName + ".jpg");
	 LoadTemplate(path + styleName + fileName + ".pts", sourceClothVectorX, sourceClothVectorY);

	vector<double> tempVectorX1, tempVectorX2;
	vector<double> tempVectorY1, tempVectorY2;
	bool isOne = true;

	for(i = 0; i < sourceClothVectorY.size(); i++)
	{
		if(sourceClothVectorY[i] < sourceImage.Height() - 10)
		{
			if(isOne == true)
			{	
				tempVectorX1.push_back(sourceClothVectorX[i]);
				tempVectorY1.push_back(sourceClothVectorY[i]);
			}
			else
			{
				tempVectorX2.push_back(sourceClothVectorX[i]);
				tempVectorY2.push_back(sourceClothVectorY[i]);
			}
		}
		else
		{
			isOne = false;
		}
	}
		
	int size1 = tempVectorX1.size();
	int size2 = tempVectorX2.size();
	int sumSize = size1 + size2;
	CShape sourShape, destShape;
	sourShape.Resize(sumSize);
	for(i = 0; i < sumSize; i++)
	{
		if(i < size2)
		{
			sourShape.PointX(i) = tempVectorX2[i];
			sourShape.PointY(i) = tempVectorY2[i];	
		}
		else
		{
			sourShape.PointX(i) = tempVectorX1[i - size2];
			sourShape.PointY(i) = tempVectorY1[i - size2];	
		}
	}
	ResampleCurve(sourShape, destShape, 10);
	sourceVectorX.clear();
	sourceVectorY.clear();

	for(i = 0; i < destShape.PointNum(); i++)
	{
		sourceVectorX.push_back(destShape.PointX(i));
		sourceVectorY.push_back(destShape.PointY(i));
	}

	 double minY11 = 10000.0;
	 double minY22 = 10000.0;
     int minT11 = 0;
	 int minT22 = 0;
	 for(i = 0 ; i < sourceVectorX.size() / 2; i++)
	 {
		 if(minY11 > sourceVectorY[i])
			 minY11 = sourceVectorY[i];
	 }
	 for(i = sourceVectorX.size() / 2; i < sourceVectorX.size(); i++)
	 {
		 if(minY22 > sourceVectorY[i])
			 minY22 = sourceVectorY[i];
	 }
	 for(i = 0; i < sourceVectorX.size(); i++)
	 {
		 if(sourceVectorY[i] == minY11 && i < sourceVectorY.size() / 2)
            minT11 = i;
		 if(sourceVectorY[i] == minY22 && i >= sourceVectorY.size() / 2)
			minT22 = i;
	 }

	 double maxY00 = 0.0;
	 int maxT00 = 0;
	 for(i = minT11; i <= minT22; i++)
	 {
		 if(maxY00 < sourceVectorY[i])
			 maxY00 = sourceVectorY[i];
	 }
	 for(i = 0; i < sourceVectorY.size(); i++)
	 {
		 if(sourceVectorY[i] == maxY00 && i <= minT22 && i >= minT11)
            maxT00 = i;
	 }

	vector<double> douX, douY;
	vector<double> litX, litY;
	vector<double> comX, comY;
	for(i = 0; i <= minT11; i++)
	{
		douX.push_back(sourceVectorX[i]);
		douY.push_back(sourceVectorY[i]);
	}
	SelectContourPoint(douX, douY, litX, litY, minT1 + 1);
	for(i = 0; i < litX.size(); i++)
	{
		comX.push_back(litX[i]);
		comY.push_back(litY[i]);
	}
	douX.clear();
	douY.clear();
	litX.clear();
	litY.clear();
	for(i = minT11; i <= maxT00; i++)
	{
		douX.push_back(sourceVectorX[i]);
		douY.push_back(sourceVectorY[i]);
	}
	SelectContourPoint(douX, douY, litX, litY, maxT0 - minT1 + 1);
	for(i = 1; i < litX.size(); i++)
	{
		comX.push_back(litX[i]);
		comY.push_back(litY[i]);
	}
	douX.clear();
	douY.clear();
	litX.clear();
	litY.clear();
	for(i = maxT00; i <= minT22; i++)
	{
		douX.push_back(sourceVectorX[i]);
		douY.push_back(sourceVectorY[i]);
	}
	SelectContourPoint(douX, douY, litX, litY, minT2 - maxT0 + 1);
	for(i = 1; i < litX.size(); i++)
	{
		comX.push_back(litX[i]);
		comY.push_back(litY[i]);
	}
	douX.clear();
	douY.clear();
	litX.clear();
	litY.clear();
	for(i = minT22; i < sourceVectorX.size(); i++)
	{
		douX.push_back(sourceVectorX[i]);
		douY.push_back(sourceVectorY[i]);
	}
	SelectContourPoint(douX, douY, litX, litY, targetVectorX.size() - minT2 + 1);
	for(i = 1; i < litX.size(); i++)
	{
		comX.push_back(litX[i]);
		comY.push_back(litY[i]);
	}
	
//	CMyTPS  TPS;
//	TPS.Create(targetVectorX, targetVectorY, comX, comY, 0);

	CRgn tempRgn;
	CPoint points[23];
	for(int k = 0; k < 23; k++)
	{
		points[k].x = m_dpLandMarks[k + 68].x;
		points[k].y = m_dpLandMarks[k + 68].y + 8;
	}
	tempRgn.CreatePolygonRgn(points, 23, WINDING);
	
	CImg<unsigned char> img55;
	img55.load("D:\\mf.bmp");
	for(int mk = 0;mk < 23; mk++)
	{
		unsigned char _color[3];
		_color[0] = 0;
		_color[1] = 255;
		_color[2] = 0;
		img55.draw_point(points[0 + mk].x, points[0 + mk].y, _color, 4);
	}
	img55.save("D:\\dot2.bmp");

	double targetArryX[40] = {0}, targetArryY[40] = {0};
	double comArrX[40] = {0}, comArrY[40] = {0};
	for(int nk=0;nk<targetVectorX.size();nk++)
	{
		targetArryX[nk] = targetVectorX[nk];
		targetArryY[nk] = targetVectorY[nk];
	}
	for(int ak=0;ak<comX.size();ak++)
	{
		comArrX[ak] = comX[ak];
		comArrY[ak] = comY[ak];
	}
	matchShapeContext.Initialization(targetVectorX, targetVectorY, comX, comY, param);
	matchShapeContext.DoMatching();
//	matchShapeContext.GetUpdateData(targetVectorX, targetVectorY, sourceVectorX, sourceVectorY, curX, curY, size, loop);

	CvvImage tempImg;
	tempImg.Destroy();
	tempImg.Create(m_TargetImg.Width(), m_TargetImg.Height(), 24);
	ImageChanged(tempImg.GetImage(), 255);
	ImageChanged(m_RegionImg.GetImage(), 255);

	BYTE *buf1 = (BYTE *)m_TargetImg.GetImage()->imageData;
	int WidthPix1 = WIDTHBYTES(m_TargetImg.Width() * 24);
	BYTE *buf2 = (BYTE *)sourceImage.GetImage()->imageData;
	int WidthPix2 = WIDTHBYTES(sourceImage.Width() * 24);
	BYTE *buf3 = (BYTE *)m_TargetImgCopy.GetImage()->imageData;
	int WidthPix3 = WIDTHBYTES(m_TargetImgCopy.Width() * 24);
	BYTE *buf4 = (BYTE *)tempImg.GetImage()->imageData;
	int WidthPix4 = WIDTHBYTES(tempImg.Width() * 24);
	BYTE *buf5 = (BYTE *)m_RegionImg.GetImage()->imageData;
	int WidthPix5 = WIDTHBYTES(m_RegionImg.Width() * 24);
	BYTE *buf6 = (BYTE *)m_RenderImg.GetImage()->imageData;
	int WidthPix6 = WIDTHBYTES(m_RenderImg.Width() * 24);

	double tx, ty;
	for(i = clothRect.top; i < clothRect.bottom; i++)
	{
		for(j = clothRect.left; j < clothRect.right; j++)
		{
			matchShapeContext.GetTPS()->TPSTransform(j, i, tx, ty);
			if(tx > 0 && tx < sourceImage.Width() && ty > 0 && ty < sourceImage.Height())
			{
				buf4[i * WidthPix4 + j * 3] = buf2[(int)(ty) * WidthPix2 + (int)(tx) * 3];
				buf4[i * WidthPix4 + j * 3 + 1] = buf2[(int)(ty) * WidthPix2 + (int)(tx) * 3 + 1];
				buf4[i * WidthPix4 + j * 3 + 2] = buf2[(int)(ty) * WidthPix2 + (int)(tx) * 3 + 2];
			}
		}
	}

	CString tempStr("mode");
	for(i = clothRect.top; i < clothRect.bottom; i++)
	{
		for(j = clothRect.left; j < clothRect.right; j++)
		{	
			if(buf4[i * WidthPix4 + j * 3] > 220 && buf4[i * WidthPix4 + j * 3 + 1] > 220 && buf4[i * WidthPix4 + j * 3 + 2] > 220)
			   continue;
			else
			{
				if(templatePath.Right(4) == tempStr)
				{
					buf4[i * WidthPix4 + j * 3] = 255;
					buf4[i * WidthPix4 + j * 3 + 1] = 255;
					buf4[i * WidthPix4 + j * 3 + 2] = 255;
				}
				else
				{
					buf4[i * WidthPix4 + j * 3] = 210;
					buf4[i * WidthPix4 + j * 3 + 1] = 210;
					buf4[i * WidthPix4 + j * 3 + 2] = 194;
				}
			}
		}
	}

	for(i = clothRect.top; i < clothRect.bottom; i++)
	{
		for(j = clothRect.left; j < clothRect.right; j++)
		{	
			buf5[i * WidthPix5 + j * 3] = buf4[i * WidthPix4 + j * 3];
			buf5[i * WidthPix5 + j * 3 + 1] = buf4[i * WidthPix4 + j * 3 + 1];
			buf5[i * WidthPix5 + j * 3 + 2] = buf4[i * WidthPix4 + j * 3 + 2];
		}
	}

	 matchShapeContext.Initialization(comX, comY, targetVectorX, targetVectorY, param);
	 matchShapeContext.DoMatching();
	 //matchShapeContext.GetUpdateData(sourceVectorX, sourceVectorY, targetVectorX, targetVectorY, curX, curY, size, loop);

	 vector< vector<CPoint> > pointVVector;
	 fileName.Format("\\keyPoints%d.pts", index + 1);
	 LoadTemplate(path + styleName + fileName, pointVVector);
	 double tx1,ty1,tx2,ty2;

	 CImg<unsigned char> RetImg(m_TargetImg.Width(), m_TargetImg.Height(), 1, 3, 255);
	 CImg<unsigned char> flag(m_TargetImg.Width(), m_TargetImg.Height(), 1, 1, 0);
	 CImg<unsigned char> FlagImg(m_TargetImg.Width(), m_TargetImg.Height(), 1, 1, 0);
	 CImg<unsigned char> AlphaImg;

	 DoublePoint dp;
	 vector<DoublePoint> MidPoints;
	 CCurveMorphing CMorphing;
	 unsigned char black[3] = {10,10,10};
	 for(i = 0; i < pointVVector.size(); i++)
	 {
		 for(j = 0; j < pointVVector[i].size() - 1; j++)
		 { 
			 matchShapeContext.GetTPS()->TPSTransform(pointVVector[i][j].x, pointVVector[i][j].y, tx1, ty1);
			 matchShapeContext.GetTPS()->TPSTransform(pointVVector[i][j + 1].x, pointVVector[i][j + 1].y, tx2, ty2);
	
			 CPoint Zpoint1, Zpoint2;
			 Zpoint1.x = (int)(tx1 + 0.5);
			 Zpoint1.y = (int)(ty1 + 0.5);
			 Zpoint2.x = (int)(tx2 + 0.5);
			 Zpoint2.y = (int)(ty2 + 0.5);

			 RetImg.draw_line(Zpoint1.x, Zpoint1.y, Zpoint2.x, Zpoint2.y, black);

			dp.x = Zpoint1.x;
			dp.y = Zpoint1.y;
			if(pointVVector[i].size() < 5)
			{
				MidPoints.push_back(dp);
				AlphaImg.load(m_sPath + "\\brush\\-1.bmp");
			}
			else
			{
				if(j % 2 == 0 && j != pointVVector[i].size() - 2)
				{
					MidPoints.push_back(dp);
					AlphaImg.load(m_sPath + "\\brush\\000.bmp");
				}
			}
		
			if(j == pointVVector[i].size() - 2)
			{
				dp.x = Zpoint2.x;
				dp.y = Zpoint2.y;
				MidPoints.push_back(dp);
			}
		 }

		CMorphing.MorphingToCurve(MidPoints, 20, 10, 10, AlphaImg, 2, BrushImg, flag, FlagImg);
		MidPoints.clear();
	 }

      BrushImg.save("C:\\BrushImg.bmp"); 
	 AlphaImg.load(m_sPath + "\\brush\\000.bmp");
	 vector<DoublePoint> skinPoints1, skinPoints2;
	 if(m_skinVector.back().y == minY2)
	 {
          dp.x = targetVectorX[minT2];
		  dp.y = targetVectorY[minT2] + 8;
		  skinPoints2.push_back(dp);
	 }
	 else
	 {
		  dp.x = targetVectorX[minT1];
		  dp.y = targetVectorY[minT1] + 8;
		  skinPoints1.push_back(dp);
	 }
	 
	 for(i = 0; i < m_skinVector.size(); i++)
	 {	
		if(m_skinVector[i].y <= m_dpLandMarks[69].y)
			continue;

		if(m_skinVector[i].y < minY1 && m_skinVector[i].x < targetVectorX[maxT0])
		{
			dp.x = m_skinVector[i].x;
			dp.y = m_skinVector[i].y;

			skinPoints1.push_back(dp);	
		}

		if(m_skinVector[i].y < minY2 && m_skinVector[i].x > targetVectorX[maxT0])
		{
			dp.x = m_skinVector[i].x;
			dp.y = m_skinVector[i].y;

			skinPoints2.push_back(dp);	
		}
	 }

	 if(skinPoints1.front().y != minY1 + 8)
	 {
		  dp.x = targetVectorX[minT1];
		  dp.y = targetVectorY[minT1] + 8;
		  skinPoints1.push_back(dp);
	 }
	 if(skinPoints2.front().y != minY2 + 8)
	 {
          dp.x = targetVectorX[minT2];
		  dp.y = targetVectorY[minT2] + 8;
		  skinPoints2.push_back(dp);
	 }

	 CMorphing.MorphingToCurve(skinPoints1, 20, 10, 10, AlphaImg, 2, BrushImg, flag, FlagImg);
	 CMorphing.MorphingToCurve(skinPoints2, 20, 10, 10, AlphaImg, 2, BrushImg, flag, FlagImg);
	 for(i = 0; i < (int)skinPoints1.size() - 1; i++)
	 {
		 RetImg.draw_line((int)(skinPoints1[i].x), (int)(skinPoints1[i].y), (int)(skinPoints1[i + 1].x), (int)(skinPoints1[i + 1].y), black);
	 }
	 for(i = 0; i < (int)skinPoints2.size() - 1; i++)
	 {
		  RetImg.draw_line((int)(skinPoints2[i].x), (int)(skinPoints2[i].y), (int)(skinPoints2[i + 1].x), (int)(skinPoints2[i + 1].y), black);
	 }

	 for(i = 0; i < m_TargetImg.Height(); i++)
	 {
		 for(j = 0; j < m_TargetImg.Width(); j++)
		 {
			 if(i < m_dpLandMarks[75].y)
			 {
				 buf4[i * WidthPix4 + j * 3] = buf1[i * WidthPix1 + j * 3];
				 buf4[i * WidthPix4 + j * 3 + 1] = buf1[i * WidthPix1 + j * 3 + 1];
				 buf4[i * WidthPix4 + j * 3 + 2] = buf1[i * WidthPix1 + j * 3 + 2];

				 buf5[i * WidthPix5 + j * 3] = buf6[i * WidthPix6 + j * 3];
				 buf5[i * WidthPix5 + j * 3 + 1] = buf6[i * WidthPix6 + j * 3 + 1];
				 buf5[i * WidthPix5 + j * 3 + 2] = buf6[i * WidthPix6 + j * 3 + 2];
			 }

			 if(BrushImg(j, i, 0) < 220)
			 {
				 buf4[i * WidthPix4 + j * 3] = BrushImg(j, i, 0);
				 buf4[i * WidthPix4 + j * 3 + 1] = BrushImg(j, i, 1);
				 buf4[i * WidthPix4 + j * 3 + 2] = BrushImg(j, i, 2);
			 }

			 if(RetImg(j, i, 0) < 220)
			 {
				 buf5[i * WidthPix5 + j * 3] = RetImg(j, i, 0);
				 buf5[i * WidthPix5 + j * 3 + 1] = RetImg(j, i, 1);
				 buf5[i * WidthPix5 + j * 3 + 2] = RetImg(j, i, 2);
			 }
		 }
	 }
	 cimg_mapXY(BrushImg, x, y)
	 {
		BrushImg(x, y, 0) = 255;
		BrushImg(x, y, 1) = 255;
		BrushImg(x, y, 2) = 255;
	 }
	 cimg_mapXY(RetImg, xx, yy)
	 {
		RetImg(xx, yy, 0) = 255;
		RetImg(xx, yy, 1) = 255;
		RetImg(xx, yy, 2) = 255;
	 }
	 cvSmooth(tempImg.GetImage(), tempImg.GetImage(), CV_GAUSSIAN, 3, 3, 1);

	for(i = 0; i < m_TargetImg.Height(); i++)
	{
		for(j = 0; j < m_TargetImg.Width(); j++)
		{
			if(i < m_dpLandMarks[75].y && (tempRgn.PtInRegion(CPoint(j - 3, i)) || tempRgn.PtInRegion(CPoint(j + 3, i))))
			{
				continue;
			}
			
			if(!tempRgn.PtInRegion(CPoint(j,i + 5)) && !hairRgn.PtInRegion(CPoint(j,i)))
			{
				buf1[i * WidthPix1 + j * 3] = buf4[i * WidthPix4 + j * 3];
				buf1[i * WidthPix1 + j * 3 + 1] = buf4[i * WidthPix4 + j * 3 + 1];
				buf1[i * WidthPix1 + j * 3 + 2] = buf4[i * WidthPix4 + j * 3 + 2];

				buf3[i * WidthPix3 + j * 3] = buf4[i * WidthPix4 + j * 3];
				buf3[i * WidthPix3 + j * 3 + 1] = buf4[i * WidthPix4 + j * 3 + 1];
				buf3[i * WidthPix3 + j * 3 + 2] = buf4[i * WidthPix4 + j * 3 + 2];

				buf6[i * WidthPix6 + j * 3] = buf5[i * WidthPix5 + j * 3];
				buf6[i * WidthPix6 + j * 3 + 1] = buf5[i * WidthPix5 + j * 3 + 1];
				buf6[i * WidthPix6 + j * 3 + 2] = buf5[i * WidthPix5 + j * 3 + 2];
			}
		}
	}
	m_RenderImg.Save("c:\\tmix.bmp");
	
	m_nRecordIndex++;
	m_nRecordMaxIndex = m_nRecordIndex;
	CString tempName;
	tempName.Format("\\temp\\portrait%d.jpg", m_nRecordIndex);
	m_TargetImg.Save(m_sPath + tempName);

	Invalidate(FALSE);	
}

void CPortraitEditingDlg::RenderEar(int index, CString &templatePath) 
{
	// TODO: Add your control notification handler code here
	 int i, j;
	 vector<dPoint> tempVector;
	 dPoint p1, p2;
	 for(j = 0; j < leftEarVector.size(); j++)
	 {
		 int t;
		 if(j < leftEarVector.size() - 1)
			 t = j + 1;
		 else
			 t = 0;

		 p1.x = leftEarVector[j].x;
		 p1.y = leftEarVector[j].y;
		 p2.x = leftEarVector[t].x;
		 p2.y = leftEarVector[t].y;
		 SampleLine(p1, p2, 15, tempVector);

		 cvLine(m_TargetImg.GetImage(), cvPoint(p1.x, p1.y), cvPoint(p2.x, p2.y), CV_RGB(255,255,255), 2, CV_AA);
	 }
	 vector<double> targetVectorX;
	 vector<double> targetVectorY;
	 for(i = 0; i < tempVector.size(); i++)
	 {
		 targetVectorX.push_back(tempVector[i].x);
		 targetVectorY.push_back(tempVector[i].y);
	 }

	 CvvImage sourceImage;
	 vector<double> sourceLeftEarVectorX;
	 vector<double> sourceLeftEarVectorY;
	 vector<double> sourceVectorX;
	 vector<double> sourceVectorY;
	 vector<double> curX;
	 vector<double>	curY;

	 CSCParam param(type0);
	 param.nIter = 3;
	 int loop, size;

	 CString path(templatePath + "\\ear");
	 CString fileName;

   	 CDLL_ShapeContext matchShapeContext;
	 
	 sourceLeftEarVectorX.clear();
	 sourceLeftEarVectorY.clear();
	 sourceVectorX.clear();
	 sourceVectorY.clear();
	 curX.clear();
	 curY.clear();

	 fileName.Format("\\leftEar%d", index + 1);
	 sourceImage.Load(path + fileName + ".jpg");
	 LoadTemplate(path + fileName + ".pts", sourceLeftEarVectorX, sourceLeftEarVectorY);

	 tempVector.clear();
	 for(j = 0; j < sourceLeftEarVectorX.size(); j++)
	 {
		 int n;
		 if(j < sourceLeftEarVectorX.size() - 1)
			 n = j + 1;
		 else
			 n = 0;

		 p1.x = sourceLeftEarVectorX[j];
		 p1.y = sourceLeftEarVectorY[j];
		 p2.x = sourceLeftEarVectorX[n];
		 p2.y = sourceLeftEarVectorY[n];
		 SampleLine(p1, p2, 20, tempVector);
	 }

	 for(i = 0; i < tempVector.size(); i++)
	 {
		 sourceVectorX.push_back(tempVector[i].x);
		 sourceVectorY.push_back(tempVector[i].y);
	 }

	CvvImage tempImg;
	tempImg.Destroy();
	tempImg.Create(m_TargetImg.Width(), m_TargetImg.Height(), 24);
	ImageChanged(tempImg.GetImage(), 255);
	BYTE *buf1 = (BYTE *)m_TargetImg.GetImage()->imageData;
	int WidthPix1 = WIDTHBYTES(m_TargetImg.Width() * 24);
	BYTE *buf2 = (BYTE *)sourceImage.GetImage()->imageData;
	int WidthPix2 = WIDTHBYTES(sourceImage.Width() * 24);
	BYTE *buf3 = (BYTE *)m_TargetImgCopy.GetImage()->imageData;
	int WidthPix3 = WIDTHBYTES(m_TargetImgCopy.Width() * 24);
	BYTE *buf4 = (BYTE *)tempImg.GetImage()->imageData;
	int WidthPix4 = WIDTHBYTES(tempImg.Width() * 24);
	double tx, ty;

	if(leftEarVector.size() > 0)
	{
		matchShapeContext.Initialization(targetVectorX, targetVectorY, sourceVectorX, sourceVectorY, param);
		matchShapeContext.DoMatching();
		//matchShapeContext.GetUpdateData(targetVectorX, targetVectorY, sourceVectorX, sourceVectorY, curX, curY, size, loop);

		for(i = leftEarRect.top; i < leftEarRect.bottom; i++)
		{
			for(j = leftEarRect.left; j < leftEarRect.right; j++)
			{
				if(!hairRgn.PtInRegion(CPoint(j, i)))
				{
					buf1[i * WidthPix1 + j * 3] = 255;
					buf1[i * WidthPix1 + j * 3 + 1] = 255;
					buf1[i * WidthPix1 + j * 3 + 2] = 255;

					buf3[i * WidthPix3 + j * 3] = 255;
					buf3[i * WidthPix3 + j * 3 + 1] = 255;
					buf3[i * WidthPix3 + j * 3 + 2] = 255;
				}


				if(leftEarRgn.PtInRegion(CPoint(j,i)))
				{
					matchShapeContext.GetTPS()->TPSTransform(j, i, tx, ty);
					if(tx > 0 && tx < sourceImage.Width() && ty > 0 && ty < sourceImage.Height())
					{
						buf4[i * WidthPix4 + j * 3] = buf2[(int)(ty) * WidthPix2 + (int)(tx) * 3];
						buf4[i * WidthPix4 + j * 3 + 1] = buf2[(int)(ty) * WidthPix2 + (int)(tx) * 3 + 1];
						buf4[i * WidthPix4 + j * 3 + 2] = buf2[(int)(ty) * WidthPix2 + (int)(tx) * 3 + 2];
					}
				}
			}
		}
	}

	cvSmooth(tempImg.GetImage(), tempImg.GetImage(), CV_GAUSSIAN, 3, 3, 1);
	for(i = leftEarRect.top; i < leftEarRect.bottom; i++)
	{
		for(j = leftEarRect.left; j < leftEarRect.right; j++)
		{	
			if(leftEarRgn.PtInRegion(CPoint(j,i)))
			{
				{
					buf1[i * WidthPix1 + j * 3] = buf4[i * WidthPix4 + j * 3];
					buf1[i * WidthPix1 + j * 3 + 1] = buf4[i * WidthPix4 + j * 3 + 1];
					buf1[i * WidthPix1 + j * 3 + 2] = buf4[i * WidthPix4 + j * 3 + 2];

					buf3[i * WidthPix3 + j * 3] = buf4[i * WidthPix4 + j * 3];
					buf3[i * WidthPix3 + j * 3 + 1] = buf4[i * WidthPix4 + j * 3 + 1];
					buf3[i * WidthPix3 + j * 3 + 2] = buf4[i * WidthPix4 + j * 3 + 2];
				}
			}
		}
	}


	 tempVector.clear();
	 targetVectorX.clear();
	 targetVectorY.clear();
	 for(j = 0; j < rightEarVector.size(); j++)
	 {
		 int t;
		 if(j < rightEarVector.size() - 1)
			 t = j + 1;
		 else
			 t = 0;

		 p1.x = rightEarVector[j].x;
		 p1.y = rightEarVector[j].y;
		 p2.x = rightEarVector[t].x;
		 p2.y = rightEarVector[t].y;
		 SampleLine(p1, p2, 15, tempVector);

		 cvLine(m_TargetImg.GetImage(), cvPoint(p1.x, p1.y), cvPoint(p2.x, p2.y), CV_RGB(255,255,255), 2, CV_AA);
	 }
	 for(i = 0; i < tempVector.size(); i++)
	 {
		 targetVectorX.push_back(tempVector[i].x);
		 targetVectorY.push_back(tempVector[i].y);
	 }

	 vector<double> sourceRightEarVectorX;
	 vector<double> sourceRightEarVectorY;
	 sourceRightEarVectorX.clear();
	 sourceRightEarVectorY.clear();
	 sourceVectorX.clear();
	 sourceVectorY.clear();
	 curX.clear();
	 curY.clear();

	 fileName.Format("\\rightEar%d", index + 1);
	 sourceImage.Load(path + fileName + ".jpg");
	 LoadTemplate(path + fileName + ".pts", sourceRightEarVectorX, sourceRightEarVectorY);

	 tempVector.clear();
	 for(j = 0; j < sourceRightEarVectorX.size(); j++)
	 {
		 int n;
		 if(j < sourceRightEarVectorX.size() - 1)
			 n = j + 1;
		 else
			 n = 0;

		 p1.x = sourceRightEarVectorX[j];
		 p1.y = sourceRightEarVectorY[j];
		 p2.x = sourceRightEarVectorX[n];
		 p2.y = sourceRightEarVectorY[n];
		 SampleLine(p1, p2, 20, tempVector);
	 }

	 for(i = 0; i < tempVector.size(); i++)
	 {
		 sourceVectorX.push_back(tempVector[i].x);
		 sourceVectorY.push_back(tempVector[i].y);
	 }

	ImageChanged(tempImg.GetImage(), 255);
	if(rightEarVector.size() > 0)
	{
		matchShapeContext.Initialization(targetVectorX, targetVectorY, sourceVectorX, sourceVectorY, param);
		matchShapeContext.DoMatching();
		//matchShapeContext.GetUpdateData(targetVectorX, targetVectorY, sourceVectorX, sourceVectorY, curX, curY, size, loop);

		for(i = rightEarRect.top; i < rightEarRect.bottom; i++)
		{
			for(j = rightEarRect.left; j < rightEarRect.right; j++)
			{
				if(!hairRgn.PtInRegion(CPoint(j, i)))
				{
					buf1[i * WidthPix1 + j * 3] = 255;
					buf1[i * WidthPix1 + j * 3 + 1] = 255;
					buf1[i * WidthPix1 + j * 3 + 2] = 255;

					buf3[i * WidthPix3 + j * 3] = 255;
					buf3[i * WidthPix3 + j * 3 + 1] = 255;
					buf3[i * WidthPix3 + j * 3 + 2] = 255;
				}

				if(rightEarRgn.PtInRegion(CPoint(j,i)))
				{
					matchShapeContext.GetTPS()->TPSTransform(j, i, tx, ty);
					if(tx > 0 && tx < sourceImage.Width() && ty > 0 && ty < sourceImage.Height())
					{
						buf4[i * WidthPix4 + j * 3] = buf2[(int)(ty) * WidthPix2 + (int)(tx) * 3];
						buf4[i * WidthPix4 + j * 3 + 1] = buf2[(int)(ty) * WidthPix2 + (int)(tx) * 3 + 1];
						buf4[i * WidthPix4 + j * 3 + 2] = buf2[(int)(ty) * WidthPix2 + (int)(tx) * 3 + 2];
					}
				}
			}
		}

		cvSmooth(tempImg.GetImage(), tempImg.GetImage(), CV_GAUSSIAN, 3, 3, 1);
		for(i = rightEarRect.top; i < rightEarRect.bottom; i++)
		{
			for(j = rightEarRect.left; j < rightEarRect.right; j++)
			{	
				if(rightEarRgn.PtInRegion(CPoint(j,i)))
				{
					{
						buf1[i * WidthPix1 + j * 3] = buf4[i * WidthPix4 + j * 3];
						buf1[i * WidthPix1 + j * 3 + 1] = buf4[i * WidthPix4 + j * 3 + 1];
						buf1[i * WidthPix1 + j * 3 + 2] = buf4[i * WidthPix4 + j * 3 + 2];

						buf3[i * WidthPix3 + j * 3] = buf4[i * WidthPix4 + j * 3];
						buf3[i * WidthPix3 + j * 3 + 1] = buf4[i * WidthPix4 + j * 3 + 1];
						buf3[i * WidthPix3 + j * 3 + 2] = buf4[i * WidthPix4 + j * 3 + 2];
					}
				}
			}
		}
	}


    vector<double> faceVectorX, faceVectorY;
	for(i = 0; i < 19; i++)
	{
		faceVectorX.push_back(m_dpLandMarks[70 + i].x);
		faceVectorY.push_back(m_dpLandMarks[70 + i].y);
	}
	ConncetContour(faceVectorX, faceVectorY, m_TargetImg.GetImage(), CV_RGB(0,0,0), 1, true);
	ConncetContour(faceVectorX, faceVectorY, m_TargetImgCopy.GetImage(), CV_RGB(0,0,0), 1, true);

	m_nRecordIndex++;
	m_nRecordMaxIndex = m_nRecordIndex;
	CString tempName;
	tempName.Format("\\temp\\portrait%d.jpg", m_nRecordIndex);
	m_TargetImg.Save(m_sPath + tempName);
	
	Invalidate(FALSE);	
}

BOOL CPortraitEditingDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = dcMem.SelectObject(&m_bmMainBg);

	CRect rcClient;
	GetClientRect(&rcClient);
	//pDC->GetClipBox(&rcClient);
	pDC->BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(),
	&dcMem, 0, 0, SRCCOPY);
	dcMem.SelectObject(pOldBitmap);
	dcMem.DeleteDC();

	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}



void CPortraitEditingDlg::ColorAdjust() 
{
	// TODO: Add your control notification handler code here
	if(m_bColorAdjust == true)
	{
		COLORREF color = RGB(m_nR, m_nG, m_nB);
		CColorDialog dlg(color);

		if(IDOK == dlg.DoModal())
		{
			COLORREF myColor = dlg.GetColor();
			m_nR = GetRValue(myColor);
			m_nG = GetGValue(myColor);
			m_nB = GetBValue(myColor);
		}
	}
}

void CPortraitEditingDlg::OnScissors() 
{
	// TODO: Add your control notification handler code here
//	if(m_bImgOpened)
//		m_nStatus = scissors;
//	else
//		MessageBox("请打开一张正脸图片！");

	if(m_bImgOpened)
	{
		::SetCursor(::LoadCursor(NULL,IDC_WAIT));
		
		double dist = 1.0;

		bool bOK = true;
		FaceAlgError err;
		int unImgWidth = m_pCurImg.Width();
		int unImgHeight = m_pCurImg.Height();

		CImage ImgTemp(m_pCurImg);

		//STEP0 验证检测算法（调用FLLValidateFD）。
		CString sFDAlgID;
		int nLen;
		if ( !FDGetAlgID(sFDAlgID.GetBuffer(_MAX_PATH), nLen) && bOK)
		{
			err = FDGetLastError();
			bOK = false;
		}
		int nFDVersion = FDGetVersion();

		//STEP1 检验算法标志串和版本（调用FLLGetAlgID和FLLGetVersion）。
		CString sAlgID;
		if ( !FLLGetAlgID(sAlgID.GetBuffer(_MAX_PATH), nLen) && bOK)
		{
			err = FLLGetLastError();
			bOK = false;
		}
		int nFLLVersion = FLLGetVersion();

		if( !FLLValidateFD(sFDAlgID, nFDVersion) )
		{
			err = FDGetLastError();
			bOK = false;
		}


		//STEP2 初始化工作（调用FLLInitialize）。

		if ( !FLLInitialize(m_sPath+ "\\fll.dat", modeEyes) && bOK )
		{
			err = FLLGetLastError();
			bOK = false;
		}

		//STEP3 调节参数，设定算法运行的精度和速度（调用FLLSetAccuracy和FLLSetSpeed）
		double lfAccuracy = 0.95;
		double lfSpeed = 0;
		if(	!FLLSetAccuracy(lfAccuracy, lfSpeed) && bOK)
		{
			err = FLLGetLastError();
			bOK = false;
		}

		//Detect Faces
		const char*  v=NULL;
		if( !FDInitialize(v) )
		{
			err = FLLGetLastError();
			bOK = false;
		}

		int nDetectedFaces=0;
		int nImageBit=ImgTemp.Bits();
		if( !FDDetectFaces(ImgTemp.Data(), unImgWidth, unImgHeight,
			nImageBit, nDetectedFaces) && bOK)
		{
			nDetectedFaces = 0;
			err = FLLGetLastError();
			bOK = false;
		}

		if(nDetectedFaces != 1)
		{
			MessageBox("请找开一张正脸图片!");
			return;
		}

		int nWritedFaces = 0;
		FACEINFO *pFaceInfos = new FACEINFO[nDetectedFaces];
		FDGetFaces(pFaceInfos, nWritedFaces);

		//process each detected faces
		if ( ImgTemp.Bits() == 24 )
			ImgTemp.CreateGreyDib();

		for(int nf = 0; nf < nDetectedFaces; nf++)//只处理单张人脸
		{

			//STEP4 定位特征点，返回被定位点的个数（调用FLLLocatLandmarks）。
			int nLandmarks = 0;

			int nImageBit = ImgTemp.Bits();
			if (!FLLLocateLandmarks(ImgTemp.Data(), unImgWidth, unImgHeight,
				nImageBit, pFaceInfos[nf], nLandmarks) && bOK)
			{
				err = FLLGetLastError();
				bOK = false;
			}

			//STEP5 得到上一步定位的各个点的数据（调用FLLGetLandmarks）。
			int nWritedLandmarks = NUM_OF_LANDMARKS;

			LANDMARKINFO *lpLandmarks = new LANDMARKINFO[nWritedLandmarks];
			if ( !FLLGetLandmarks(lpLandmarks, nWritedLandmarks) && bOK)
			{
				err = FLLGetLastError();
				bOK = false;
			}
			else
			{
				dist = sqrt(Sqr(lpLandmarks[96].x - lpLandmarks[97].x) * 1.0 + Sqr(lpLandmarks[96].y - lpLandmarks[97].y) * 1.0);

				double ratio = 80.0 / dist;
				double zoomRatio = m_dRatio * ratio;

				CvvImage tempImage;
				tempImage.Load(strImgName);
				ImageZoom(tempImage, zoomRatio, zoomRatio);

				int diffX = lpLandmarks[96].x * ratio - 176;
				int diffY = lpLandmarks[96].y * ratio - 200;

				if(diffX < 0)
					diffX = 0;
				if(diffY < 0)
					diffY = 0;

				if(tempImage.Width() < 450)
				{
					double tempRatio = 450.0 / tempImage.Width();
					ImageZoom(tempImage, tempRatio, tempRatio);

					diffX = 0;
					diffY = 0;
				}
			
				m_image.Destroy();
				m_image.Create(450, 622, 24);

				BYTE *buf1 = (BYTE *)m_image.GetImage()->imageData;
				BYTE *buf2 = (BYTE *)tempImage.GetImage()->imageData;
				int WidthPix1 = WIDTHBYTES(m_image.Width() * 24);
				int WidthPix2 = WIDTHBYTES(tempImage.Width() * 24);
            
				for(int i = 0; i < m_image.Height(); i++)
				{
					for(int j = 0; j < m_image.Width(); j++)
					{
						if(i + diffY >= 0 && i + diffY < tempImage.Height() && j + diffX >= 0 && j + diffX < tempImage.Width())
						{
							buf1[i * WidthPix1 + 3 * j] = buf2[(i + diffY) * WidthPix2 + 3 * (j + diffX)];
							buf1[i * WidthPix1 + 3 * j + 1] = buf2[(i + diffY) * WidthPix2 + 3 * (j + diffX) + 1];
							buf1[i * WidthPix1 + 3 * j + 2] = buf2[(i + diffY) * WidthPix2 + 3 * (j + diffX) + 2];
						}
						else
						{
							buf1[i * WidthPix1 + 3 * j] = 255;
							buf1[i * WidthPix1 + 3 * j + 1] = 255;
							buf1[i * WidthPix1 + 3 * j + 2] = 255;
						}	
					}
				}

				m_image.Save(m_sPath + "\\temp" + "\\input.bmp");

				m_TargetImg.Destroy();
				m_TargetImg.Create(m_image.Width(), m_image.Height(), 24);
				ImageChanged(m_TargetImg.GetImage(), 255);
				m_pCurImg.ImageClear();
				m_pCurImg.ReadFromFile(m_sPath + "\\temp" + "\\input.bmp");
				
				OrignalImage.load_bmp(m_sPath + "\\temp" + "\\input.bmp");

				m_RegionImg.Destroy();
				m_RegionImg.Create(m_image.Width(), m_image.Height(), 24);
				ImageChanged(m_RegionImg.GetImage(), 255);	
				m_RenderImg.Destroy();
				m_RenderImg.Create(m_image.Width(), m_image.Height(), 24);
				ImageChanged(m_RenderImg.GetImage(), 255);
				
				Invalidate(FALSE);		

			}
				
			delete[] lpLandmarks;

		}//end for each detected face

		delete[] pFaceInfos;

		m_btFeature.EnableWindow(TRUE);

		::SetCursor(::LoadCursor(NULL,IDC_ARROW));
	}
	else
		MessageBox("请打开一张正脸图片！");

}

void CPortraitEditingDlg::OnGetColor() 
{
	// TODO: Add your control notification handler code here
	m_nStatus = nColor;
}

void CPortraitEditingDlg::OnBrushColor() 
{
	// TODO: Add your control notification handler code here
	m_nStatus = nBrush;
}

void CPortraitEditingDlg::OnHairCOLOR() 
{
	// TODO: Add your control notification handler code here
	ColorAdjust();
	BYTE *buf = (BYTE *)m_TargetImg.GetImage()->imageData;
	int WidthPix = WIDTHBYTES(m_TargetImg.Width() * 24);
	BYTE *buf1 = (BYTE *)m_TargetImgCopy.GetImage()->imageData;
	int WidthPix1 = WIDTHBYTES(m_TargetImgCopy.Width() * 24);
	BYTE *buf2 = (BYTE *)m_RegionImg.GetImage()->imageData;
	int WidthPix2 = WIDTHBYTES(m_RegionImg.Width() * 24);
	BYTE *buf3 = (BYTE *)m_ColorImgCopy.GetImage()->imageData;
	int WidthPix3 = WIDTHBYTES(m_ColorImgCopy.Width() * 24);

	for(int i = hairRect.top; i <= hairRect.bottom; i++)
	{
		for(int j = hairRect.left; j <= hairRect.right; j++)
		{
			if(hairRgn.PtInRegion(CPoint(j,i)))
			{			
				if(buf1[i * WidthPix1 + 3 * j] > 220)
				{
					buf[i * WidthPix + 3 * j] = m_nB;
					buf[i * WidthPix + 3 * j + 1] = m_nG;
					buf[i * WidthPix + 3 * j + 2] = m_nR;
				}
				
				buf2[i * WidthPix2 + 3 * j] = m_nB;
				buf2[i * WidthPix2 + 3 * j + 1] = m_nG;
				buf2[i * WidthPix2 + 3 * j + 2] = m_nR;
			}
		}
	}
	
	ConncetContour(hairVectorX, hairVectorY, m_TargetImg.GetImage(), CV_RGB(m_nR, m_nG, m_nB), 2);

/*	CRgn faceRgn;
	CPoint points[21];
	for(int k = 0; k < 21; k++)
	{
		points[k].x = m_dpLandMarks[k + 69].x;
		points[k].y = m_dpLandMarks[k + 69].y;
	}
	faceRgn.CreatePolygonRgn(points, 21, WINDING);
	for(int i = hairRect.top; i <= hairRect.bottom; i++)
	{
		for(int j = hairRect.left; j <= hairRect.right; j++)
		{
			if(!faceRgn.PtInRegion(CPoint(j,i)))
			{			
				if(abs(buf3[i * WidthPix3 + 3 * j] -  139) + abs(buf3[i * WidthPix3 + 3 * j + 1] -  198) + abs(buf3[i * WidthPix3 + 3 * j + 2] - 254) < 100)
				{
					buf[i * WidthPix + 3 * j] = m_nB;
					buf[i * WidthPix + 3 * j + 1] = m_nG;
					buf[i * WidthPix + 3 * j + 2] = m_nR;
				}
			}
		}
	}*/

    Invalidate(FALSE);
}

void CPortraitEditingDlg::FillSkin(CvvImage &tarImage, int x, int y, int r, int g, int b)
{
	BYTE *buf = (BYTE *)tarImage.GetImage()->imageData;
	int WidthPix = WIDTHBYTES(tarImage.Width() * 24);
	
    if(buf[y * WidthPix + 3 * x] == b && buf[y * WidthPix + 3 * x + 1] == g && buf[y * WidthPix + 3 * x + 2] == r)
	{
		if(buf[y * WidthPix + 3 * (x + 1)] > 240 && buf[y * WidthPix + 3 * (x + 1) + 1] > 240 && buf[y * WidthPix + 3 * (x + 1) + 2] > 240)
		{
			buf[y * WidthPix + 3 * (x + 1)] = b;
			buf[y * WidthPix + 3 * (x + 1) + 1] = g;
			buf[y * WidthPix + 3 * (x + 1) + 2] = r;	
		}
		if(buf[y * WidthPix + 3 * (x - 1)] > 240 && buf[y * WidthPix + 3 * (x - 1) + 1] > 240 && buf[y * WidthPix + 3 * (x - 1) + 2] > 240)
		{
			buf[y * WidthPix + 3 * (x - 1)] = b;
			buf[y * WidthPix + 3 * (x - 1) + 1] = g;
			buf[y * WidthPix + 3 * (x - 1) + 2] = r;
		}
		if(buf[(y + 1) * WidthPix + 3 * x] > 240 && buf[(y + 1) * WidthPix + 3 * x + 1] > 240 && buf[(y + 1) * WidthPix + 3 * x + 2] > 240)
		{
			buf[(y + 1) * WidthPix + 3 * x ] = b;
			buf[(y + 1) * WidthPix + 3 * x + 1] = g;
			buf[(y + 1) * WidthPix + 3 * x + 2] = r;
		}
		if(buf[(y - 1) * WidthPix + 3 * x] > 240 && buf[(y - 1) * WidthPix + 3 * x + 1] > 240 && buf[(y - 1) * WidthPix + 3 * x + 2] > 240)
		{
			buf[(y - 1) * WidthPix + 3 * x ] = b;
			buf[(y - 1) * WidthPix + 3 * x + 1] = g;
			buf[(y - 1) * WidthPix + 3 * x + 2] = r;
		}
	}
}

void CPortraitEditingDlg::OnSkinColor() 
{
	// TODO: Add your control notification handler code here
	ColorAdjust();

	int i, j;
	CRgn	 LeftEyeRgn, RightEyeRgn;
	CRect    LeftRect, RightRect;
	CPoint LeftEyePoint[8];
	for(i = 0; i < 8; i++)
	{
		LeftEyePoint[i].x = m_ptArrLandMarks[41 + i].x;
		LeftEyePoint[i].y = m_ptArrLandMarks[41 + i].y;
	}
	LeftEyeRgn.CreatePolygonRgn(LeftEyePoint, 8, WINDING);
	LeftEyeRgn.GetRgnBox(LeftRect);

	CPoint RightEyePoint[8];
	for(i = 0; i < 8; i++)
	{
		RightEyePoint[i].x = m_ptArrLandMarks[49 + i].x;
		RightEyePoint[i].y = m_ptArrLandMarks[49 + i].y;
	}
	RightEyeRgn.CreatePolygonRgn(RightEyePoint, 8, WINDING);
	RightEyeRgn.GetRgnBox(RightRect);

	BYTE *buf = (BYTE *)m_TargetImg.GetImage()->imageData;
	int WidthPix = WIDTHBYTES(m_TargetImg.Width() * 24);
	BYTE *buf1 = (BYTE *)m_TargetImgCopy.GetImage()->imageData;
	int WidthPix1 = WIDTHBYTES(m_TargetImgCopy.Width() * 24);
	BYTE *buf2 = (BYTE *)m_RenderImg.GetImage()->imageData;
	int WidthPix2 = WIDTHBYTES(m_RenderImg.Width() * 24);


	for(i = skRect.top; i < skRect.bottom; i++)
	{
		for(j = skRect.left; j < skRect.right; j++)
		{
			if(skinRgn.PtInRegion(CPoint(j,i)))
			{
				if(buf1[i * WidthPix1 + 3 * j] > 220  && !RightEyeRgn.PtInRegion(CPoint(j,i)) && !LeftEyeRgn.PtInRegion(CPoint(j,i)))
				{
					buf[i * WidthPix + 3 * j] = m_nB;
					buf[i * WidthPix + 3 * j + 1] = m_nG;
					buf[i * WidthPix + 3 * j + 2] = m_nR;
				}
				
				if(buf2[i * WidthPix2 + 3 * j] > 220 && !RightEyeRgn.PtInRegion(CPoint(j,i)) && !LeftEyeRgn.PtInRegion(CPoint(j,i)))
				{
					buf2[i * WidthPix2 + 3 * j] = m_nB;
					buf2[i * WidthPix2 + 3 * j + 1] = m_nG;
					buf2[i * WidthPix2 + 3 * j + 2] = m_nR;
				}
			}
		}
	}

	for(j = skRect.left; j < skRect.right; j++)
	{
		for(i = m_TargetImg.Height() - 2; i > 2 ; i--)
		{	
			if(!bgRgn.PtInRegion(CPoint(j,i)) && (i > clothRect.top || i < m_dpLandMarks[74].y))
			{
				FillSkin(m_TargetImg, j, i, m_nR, m_nG, m_nB);
				FillSkin(m_RenderImg, j, i, m_nR, m_nG, m_nB);
			}
		}

		for(i = 2; i < m_TargetImg.Height() - 2 ; i++)
		{	
			if(!bgRgn.PtInRegion(CPoint(j,i)) && (i > clothRect.top || i < m_dpLandMarks[74].y))
			{
				FillSkin(m_TargetImg, j, i, m_nR, m_nG, m_nB);
				FillSkin(m_RenderImg, j, i, m_nR, m_nG, m_nB);
			}
		}
	}

	Invalidate(FALSE);
}

void CPortraitEditingDlg::FillCloth()
{
	BYTE *buf = (BYTE *)m_TargetImg.GetImage()->imageData;
	int WidthPix = WIDTHBYTES(m_TargetImg.Width() * 24);
	BYTE *buf1 = (BYTE *)m_TargetImgCopy.GetImage()->imageData;
	int WidthPix1 = WIDTHBYTES(m_TargetImgCopy.Width() * 24);

	int k;
	for(int i = clothRect.left; i < clothRect.right; i++)
	{
		for(int j = clothRect.bottom - 1; j >= clothRect.top; j--)
		{
			if(m_nCompSelected[5] <= 0)
			{
				if(clothRgn.PtInRegion(CPoint(i,j)))
				{	
					if(buf1[j * WidthPix1 + 3 * i] > 200)
					{
						buf[j * WidthPix + 3 * i] = m_nB;
						buf[j * WidthPix + 3 * i + 1] = m_nG;
						buf[j * WidthPix + 3 * i + 2] = m_nR;
					}
				}
			}
			else
			{
				if(buf[j * WidthPix1 + 3 * i] < 230)
				{
					k = j;
					break;
				}
                else
				{
					if(clothRgn.PtInRegion(CPoint(i,j)) && !clothRgn.PtInRegion(CPoint(i,j - 1)))
						k = j;
				}
			}	
		}

		if(m_nCompSelected[5] > 0)
		{
			for(int j = clothRect.bottom - 1; j >= k; j--)
			{
				buf[j * WidthPix + 3 * i] = 210;
				buf[j * WidthPix + 3 * i + 1] = 210;
				buf[j * WidthPix + 3 * i + 2] = 194;
			}
		}
	}

}

void CPortraitEditingDlg::OnClothColor() 
{
	// TODO: Add your control notification handler code here
	ColorAdjust();

	CvvImage grayImage;
	grayImage.Create(m_image.Width(), m_image.Height(), IPL_DEPTH_8U);
	cvCvtColor(m_image.GetImage(), grayImage.GetImage(), CV_BGR2GRAY);

	CvvImage edgeImage;
	edgeImage.Create(m_image.Width(), m_image.Height(), IPL_DEPTH_8U);
	cvCanny(grayImage.GetImage(), edgeImage.GetImage(), 50, 150, 3);

	BYTE *buf = (BYTE *)m_TargetImg.GetImage()->imageData;
	int WidthPix = WIDTHBYTES(m_TargetImg.Width() * 24);
	BYTE *buf1 = (BYTE *)m_TargetImgCopy.GetImage()->imageData;
	int WidthPix1 = WIDTHBYTES(m_TargetImgCopy.Width() * 24);
	BYTE *buf2 = (BYTE *)edgeImage.GetImage()->imageData;
	int WidthPix2 = WIDTHBYTES(edgeImage.Width() * 8);
	BYTE *buf3 = (BYTE *)m_RegionImg.GetImage()->imageData;
	int WidthPix3 = WIDTHBYTES(m_RegionImg.Width() * 24);
	BYTE *buf4 = (BYTE *)m_ColorImgCopy.GetImage()->imageData;
	int WidthPix4 = WIDTHBYTES(m_ColorImgCopy.Width() * 24);

//	for(int i = clothRect.top; i < clothRect.bottom; i++)
//	{
//		for(int j = clothRect.left; j < clothRect.right; j++)
//		{
//			if(clothRgn.PtInRegion(CPoint(j,i)))
//			{	
//				if(buf1[i * WidthPix1 + 3 * j] > 200)
//				{
//					buf[i * WidthPix + 3 * j] = m_nB;
//					buf[i * WidthPix + 3 * j + 1] = m_nG;
//					buf[i * WidthPix + 3 * j + 2] = m_nR;
//				}
//				
//				buf3[i * WidthPix3 + 3 * j] = m_nB;
//				buf3[i * WidthPix3 + 3 * j + 1] = m_nG;
//				buf3[i * WidthPix3 + 3 * j + 2] = m_nR;
//
//			}
//		}
//	}

	for(int i = clothRect.top; i <= clothRect.bottom; i++)
	{
		for(int j = clothRect.left; j <= clothRect.right; j++)
		{	
			if(abs(buf4[i * WidthPix4 + 3 * j] -  210) + abs(buf4[i * WidthPix4 + 3 * j + 1] -  210) + abs(buf4[i * WidthPix4 + 3 * j + 2] - 194) < 20)
			{
				buf[i * WidthPix + 3 * j] = m_nB;
				buf[i * WidthPix + 3 * j + 1] = m_nG;
				buf[i * WidthPix + 3 * j + 2] = m_nR;
			}
		}
	}
	
	Invalidate(FALSE);	
}

void CPortraitEditingDlg::OnBackgroundColor() 
{
	// TODO: Add your control notification handler code here
	ColorAdjust();

	BYTE *buf = (BYTE *)m_TargetImg.GetImage()->imageData;
	int WidthPix = WIDTHBYTES(m_TargetImg.Width() * 24);
	BYTE *buf1 = (BYTE *)m_TargetImgCopy.GetImage()->imageData;
	int WidthPix1 = WIDTHBYTES(m_TargetImgCopy.Width() * 24);
	BYTE *buf2 = (BYTE *)m_RegionImg.GetImage()->imageData;
	int WidthPix2 = WIDTHBYTES(m_RegionImg.Width() * 24);

	int b = buf[0];
	int g = buf[1];
	int r = buf[2];

	for(int i = bgRect.top; i < bgRect.bottom; i++)
	{
		for(int j = bgRect.left; j < bgRect.right; j++)
		{
			if(abs(buf[i * WidthPix + 3 * j] - b) + abs(buf[i * WidthPix + 3 * j + 1] - g) + abs(buf[i * WidthPix + 3 * j + 2] - r) < 15)
			{
				buf[i * WidthPix + 3 * j] = m_nB;
				buf[i * WidthPix + 3 * j + 1] = m_nG;
				buf[i * WidthPix + 3 * j + 2] = m_nR;

				buf2[i * WidthPix2 + 3 * j] = m_nB;
				buf2[i * WidthPix2 + 3 * j + 1] = m_nG;
				buf2[i * WidthPix2 + 3 * j + 2] = m_nR;
			}
		}
	}

	Invalidate(FALSE);
}

void CPortraitEditingDlg::OnShadowColor() 
{
	// TODO: Add your control notification handler code here
	ColorAdjust();

	int i, j;
	CRgn	 LeftEyeRgn, RightEyeRgn;
	CRect    LeftRect, RightRect;
	CPoint LeftEyePoint[8];
	for(i = 0; i < 8; i++)
	{
		LeftEyePoint[i].x = m_ptArrLandMarks[41 + i].x;
		LeftEyePoint[i].y = m_ptArrLandMarks[41 + i].y;
	}
	LeftEyeRgn.CreatePolygonRgn(LeftEyePoint, 8, WINDING);
	LeftEyeRgn.GetRgnBox(LeftRect);

	CPoint RightEyePoint[8];
	for(i = 0; i < 8; i++)
	{
		RightEyePoint[i].x = m_ptArrLandMarks[49 + i].x;
		RightEyePoint[i].y = m_ptArrLandMarks[49 + i].y;
	}
	RightEyeRgn.CreatePolygonRgn(RightEyePoint, 8, WINDING);
	RightEyeRgn.GetRgnBox(RightRect);

	CRgn	 noseRgn;
	CRect    noseRect;
	CPoint nosePoint[13];
	for(i = 0; i < 13; i++)
	{
		nosePoint[i].x = m_ptArrLandMarks[61 + i].x;
		nosePoint[i].y = m_ptArrLandMarks[61 + i].y;
	}
	noseRgn.CreatePolygonRgn(nosePoint, 13, WINDING);
	noseRgn.GetRgnBox(noseRect);

	CRgn	 mouthRgn;
	CRect    mouthRect;
	CPoint mouthPoint[12];
	for(i = 0; i < 12; i++)
	{
		mouthPoint[i].x = m_ptArrLandMarks[74 + i].x;
		mouthPoint[i].y = m_ptArrLandMarks[74 + i].y;
	}
	mouthRgn.CreatePolygonRgn(mouthPoint, 12, WINDING);
	mouthRgn.GetRgnBox(mouthRect);

	CvvImage grayImage;
	grayImage.Create(m_image.Width(), m_image.Height(), IPL_DEPTH_8U);
	cvCvtColor(m_image.GetImage(), grayImage.GetImage(), CV_BGR2GRAY);

	BYTE *buf = (BYTE *)m_TargetImg.GetImage()->imageData;
	int WidthPix = WIDTHBYTES(m_TargetImg.Width() * 24);
	BYTE *buf1 = (BYTE *)grayImage.GetImage()->imageData;
	int WidthPix1 = WIDTHBYTES(grayImage.Width() * 8);
	BYTE *buf2 = (BYTE *)m_TargetImgCopy.GetImage()->imageData;
	int WidthPix2 = WIDTHBYTES(m_TargetImgCopy.Width() * 24);
	BYTE *buf3 = (BYTE *)m_RegionImg.GetImage()->imageData;
	int WidthPix3 = WIDTHBYTES(m_RegionImg.Width() * 24);

	int sum =0;
	int count = 0;
	for(i = skRect.top; i < skRect.bottom; i++)
	{
		for(j = skRect.left; j < skRect.right; j++)
		{
			if(skinRgn.PtInRegion(CPoint(j,i)))
			{	
			  sum += buf1[i * WidthPix1 + j];	
			  count++;
			}
		}
	}
	int mean = sum / count;

	sum = 0;
	count = 0;
	CRect tnRect(noseRect.left - 10, noseRect.top - 10, noseRect.right + 10, noseRect.bottom + 10);
	for(i = tnRect.top; i < tnRect.bottom; i++)
	{
		for(j = tnRect.left; j < tnRect.right; j++)
		{
		  sum += buf1[i * WidthPix1 + j];	
		  count++;
		}
	}
	int tnMean = sum / count;


	for(i = 0; i < grayImage.Height(); i++)
	{
		for(j = 0; j < grayImage.Width(); j++)
		{
			if(tnRect.PtInRect(CPoint(j, i)))
			{
				if(buf1[i * WidthPix1 + j] < tnMean - 10)
					buf1[i * WidthPix1 + j] = 0;
			}

			if(buf1[i * WidthPix1 + j] < mean - 30 || RightEyeRgn.PtInRegion(CPoint(j, i - 8)) || LeftEyeRgn.PtInRegion(CPoint(j, i - 8)))
				buf1[i * WidthPix1 + j] = 0;
			else if(RightEyeRgn.PtInRegion(CPoint(j, i + 5)) || LeftEyeRgn.PtInRegion(CPoint(j, i + 5)))
				buf1[i * WidthPix1 + j] = 0;
//			else if(!noseRgn.PtInRegion((CPoint(j, i))) && noseRgn.PtInRegion((CPoint(j + 15, i))))
//				buf1[i * WidthPix1 + j] = 0;
			else
				buf1[i * WidthPix1 + j] = 255;
		}
	}

	cvDilate(grayImage.GetImage(), grayImage.GetImage(), NULL, 2);
	cvErode(grayImage.GetImage(), grayImage.GetImage(), NULL, 1);
	cvSmooth(grayImage.GetImage(), grayImage.GetImage(), CV_GAUSSIAN, 3, 3, 1);

	grayImage.Save(m_sPath + "\\temp\\shadow.jpg");

	for(i = skRect.top; i < skRect.bottom; i++)
	{
		for(j = skRect.left; j < skRect.right; j++)
		{
			if(skinRgn.PtInRegion(CPoint(j,i)))
			{
				if(buf2[i * WidthPix2 + 3 * j] > 240  && !RightEyeRgn.PtInRegion(CPoint(j,i)) && !LeftEyeRgn.PtInRegion(CPoint(j,i)))
				{
					if((buf1[i * WidthPix1 + j] < 20) || mouthRgn.PtInRegion(CPoint(j,i)))
					{
						buf[i * WidthPix + 3 * j] = m_nB;
						buf[i * WidthPix + 3 * j + 1] = m_nG;
						buf[i * WidthPix + 3 * j + 2] = m_nR;

						if(mouthRgn.PtInRegion(CPoint(j,i)))
						{
							buf[i * WidthPix + 3 * j] = Min(255, m_nB - 21);
							buf[i * WidthPix + 3 * j + 1] = Max(0, m_nB - 69);
							buf[i * WidthPix + 3 * j + 2] = Min(255, m_nR + 3);;
						}
					}
				}
				
				if((buf1[i * WidthPix1 + j] < 20) || mouthRgn.PtInRegion(CPoint(j,i)))
				{
						buf3[i * WidthPix3 + 3 * j] = m_nB;
						buf3[i * WidthPix3 + 3 * j + 1] = m_nG;
						buf3[i * WidthPix3 + 3 * j + 2] = m_nR;

						if(mouthRgn.PtInRegion(CPoint(j,i)))
						{
							buf3[i * WidthPix3 + 3 * j] = Min(255, m_nB - 21);
							buf3[i * WidthPix3 + 3 * j + 1] = Max(0, m_nB - 69);
							buf3[i * WidthPix3 + 3 * j + 2] = Min(255, m_nR + 3);;
						}
				}
			}
		}
	}

	Invalidate(FALSE);	
}

void CPortraitEditingDlg::OnDefaultCOLOR() 
{
	// TODO: Add your control notification handler code here
	m_bColorAdjust = false;

   //m_nB = 139; m_nG = 198; m_nR = 254;
   //OnHairCOLOR();

    m_nB = 255; m_nG = 250; m_nR = 243;//！
    OnBackgroundColor();//！

    m_nB = 206; m_nG = 224; m_nR = 255;
    OnSkinColor();
	//m_nB = 203; m_nG = 222; m_nR = 255;


    CString templatePath(m_sPath + "\\colorMode");
	if(m_nCompSelected[1] > 0)
		RenderEyebrow(m_nCompSelected[1] - 1, templatePath);
	if(m_nCompSelected[2] > 0)
		RenderEye(m_nCompSelected[2] - 1, templatePath);
	if(m_nCompSelected[3] > 0)
		RenderNose(m_nCompSelected[3] - 1, templatePath);
	if(m_nCompSelected[4] > 0)
		RenderMouth(m_nCompSelected[4] - 1, templatePath);
	if(m_nCompSelected[6] > 0)
		RenderEar(m_nCompSelected[6] - 1, templatePath);
	if(m_nCompSelected[0] > 0)
		RenderHair(m_nCompSelected[0] - 1, templatePath);
	if(m_nCompSelected[7] > 0)
		RenderFaceContour(m_nCompSelected[7] - 1, templatePath);
	if(m_nCompSelected[5] > 0)
		RenderCollar(m_nCompSelected[5] - 1, templatePath);

	m_nB = 207; m_nG = 209; m_nR = 207;
	FillCloth();

	m_nB = 203; m_nG = 222; m_nR = 255;//m_nB = 129; m_nG = 161; m_nR = 222;
	OnSkinColor();

	//阴影部分包括：头发和鼻子周围的阴影和嘴唇的颜色的绘制
    m_nB = 180; m_nG = 202; m_nR = 244;//m_nB = 75; m_nG = 140; m_nR = 209;
    OnShadowColor();

	BYTE *buf1 = (BYTE *)m_TargetImg.GetImage()->imageData;
	int WidthPix1 = WIDTHBYTES(m_TargetImg.Width() * 24);
	BYTE *buf2 = (BYTE *)m_RenderImg.GetImage()->imageData;
	int WidthPix2 = WIDTHBYTES(m_RenderImg.Width() * 24);
	for(int i = 0; i < m_TargetImg.Height(); i++)
	{
		for(int j = 0; j < m_TargetImg.Width(); j++)
		{
			if(buf2[i * WidthPix2 + 3 * j] > 50)
			{
				if(abs(buf1[i * WidthPix1 + 3 * j] -  210) + abs(buf1[i * WidthPix1 + 3 * j + 1] -  210) + abs(buf1[i * WidthPix1 + 3 * j + 2] - 194) < 10)
				{
					buf2[i * WidthPix2 + 3 * j] = 210;
					buf2[i * WidthPix2 + 3 * j + 1] = 210;
					buf2[i * WidthPix2 + 3 * j + 2] = 194;
				}
				//!!!
				if(abs(buf1[i * WidthPix1 + 3 * j] -  212) + abs(buf1[i * WidthPix1 + 3 * j + 1] -  216) + abs(buf1[i * WidthPix1 + 3 * j + 2] - 235) < 15)
				{
					buf2[i * WidthPix2 + 3 * j] = 212;
					buf2[i * WidthPix2 + 3 * j + 1] = 216;
					buf2[i * WidthPix2 + 3 * j + 2] = 235;
				}//!!!
			}
		}
	}
	m_RenderImg.Save("c:\\rr.bmp");
	 
	//m_nB = 128; m_nG = 128; m_nR = 64;//！
	//OnBackgroundColor();//！
	
	m_btHairColor.EnableWindow(TRUE);
	m_btSkinColor.EnableWindow(TRUE);
	m_btClothColor.EnableWindow(TRUE);
	m_btBackgroundColor.EnableWindow(TRUE);//！
	m_btShadowColor.EnableWindow(TRUE);//！
	m_btCrossStitch.EnableWindow(TRUE);

	m_bAutoColor = true;
	m_bColorAdjust = true;

	m_TargetImg.Save(m_sPath + "\\temp\\color.jpg");

	m_ColorImgCopy.Destroy();
	m_ColorImgCopy.Create(m_TargetImg.Width(), m_TargetImg.Height(), 24);
	cvCopy(m_TargetImg.GetImage(), m_ColorImgCopy.GetImage());
}

void CPortraitEditingDlg::ColorTransfer(CvvImage &inputImage, int r, int g, int b, CvvImage &outputImage)
{
	 outputImage.Create(inputImage.Width(), inputImage.Height(), 24);
     BYTE *buf1 = (BYTE *)inputImage.GetImage()->imageData;
	 int WidthPix1 = WIDTHBYTES(inputImage.Width() * 24);
   	 BYTE *buf2 = (BYTE *)outputImage.GetImage()->imageData;
	 int WidthPix2 = WIDTHBYTES(outputImage.Width() * 24);

	 int w = inputImage.Width();
	 int h = inputImage.Height();
 
	 int x, y;
	 int meancolor = 0;
	 for (y = 0; y < h; y++)
	 {
		for(x = 0; x < w; x++)
		{
			meancolor += buf1[y * WidthPix1 + 3 * x];
		}
	 }

	 meancolor /= w * h;
	 int iTemp;
	 for (y = 0; y < h; y++)
	 {
	   for(x = 0; x < w; x++)
	   {   
		   iTemp = int((buf1[y * WidthPix1 + 3 * x] - meancolor) * 0.3 + r);
		   iTemp = iTemp > 255 ? 510 - iTemp : iTemp;
		   iTemp = iTemp < 0   ?     - iTemp : iTemp; 
		   buf2[y * WidthPix2 + 3 * x] = iTemp;

		   iTemp = int((buf1[y * WidthPix1 + 3 * x + 1] - meancolor) * 0.3 + g);
		   iTemp = iTemp > 255 ? 510 - iTemp : iTemp;
		   iTemp = iTemp < 0   ?     - iTemp : iTemp;
		   buf2[y * WidthPix2 + 3 * x + 1] = iTemp;

		   iTemp = int((buf1[y * WidthPix1 + 3 * x + 2] - meancolor) * 0.3 + b);
		   iTemp = iTemp > 255 ? 510 - iTemp : iTemp;
		   iTemp = iTemp < 0   ?     - iTemp : iTemp;
		   buf2[y * WidthPix2 + 3 * x + 2] = iTemp;
	   }
	}	
}

void CPortraitEditingDlg::CrossTransfer(CvvImage &inputImage, CvvImage &outputImage, CvvImage &crossImage)
{
	 int w = inputImage.Width();
	 int h = inputImage.Height();

	 outputImage.Create(w * crossImage.Width(), h * crossImage.Height(), 24);
     BYTE *buf1 = (BYTE *)inputImage.GetImage()->imageData;
	 int WidthPix1 = WIDTHBYTES(inputImage.Width() * 24);
   	 BYTE *buf2 = (BYTE *)outputImage.GetImage()->imageData;
	 int WidthPix2 = WIDTHBYTES(outputImage.Width() * 24);
	 BYTE *buf3 = (BYTE *)crossImage.GetImage()->imageData;
	 int WidthPix3 = WIDTHBYTES(crossImage.Width() * 24);

	 int i, j;
	 int meancolor = 0;
	 for (i = 0; i < crossImage.Height(); i++)
	 {
		for(j = 0; j < crossImage.Width(); j++)
		{
			meancolor += buf3[i * WidthPix3 + 3 * j];
		}
	 }

	 meancolor /= crossImage.Width() * crossImage.Height();

	 int iTemp;
     for(i = 0; i < h; i++)
	 {
		 for(j = 0; j < w; j++)
		 {
			for(int y = 0; y < crossImage.Height(); y++)
			{
				for(int x = 0; x < crossImage.Width(); x++)
				{			
					iTemp = int((buf3[y* WidthPix3 + 3 * x] - meancolor) * 0.3 + buf1[i* WidthPix1 + 3 * j]);
					iTemp = iTemp > 255 ? 510 - iTemp : iTemp;
					iTemp = iTemp < 0   ?     - iTemp : iTemp; 
					buf2[(i * crossImage.Height() + y) * WidthPix2 + 3 * (j * crossImage.Width() + x)] = iTemp;

					iTemp = int((buf3[y* WidthPix3 + 3 * x + 1] - meancolor) * 0.3 + buf1[i* WidthPix1 + 3 * j + 1]);
					iTemp = iTemp > 255 ? 510 - iTemp : iTemp;
					iTemp = iTemp < 0   ?     - iTemp : iTemp; 
					buf2[(i * crossImage.Height() + y) * WidthPix2 + 3 * (j * crossImage.Width() + x) + 1] = iTemp;

					iTemp = int((buf3[y* WidthPix3 + 3 * x + 2] - meancolor) * 0.3 + buf1[i* WidthPix1 + 3 * j + 2]);
					iTemp = iTemp > 255 ? 510 - iTemp : iTemp;
					iTemp = iTemp < 0   ?     - iTemp : iTemp; 
					buf2[(i * crossImage.Height() + y) * WidthPix2 + 3 * (j * crossImage.Width() + x) + 2] = iTemp;	 
				}
			}
		 }
	 }
}

void  CPortraitEditingDlg::CrossTransfer2(CvvImage &inputImage, CvvImage &outputImage, CvvImage &crossImage)
{
	 int w = inputImage.Width();
	 int h = inputImage.Height();

	 outputImage.Create(w * crossImage.Width() / 2, h * crossImage.Height() / 2, 24);
     BYTE *buf1 = (BYTE *)inputImage.GetImage()->imageData;
	 int WidthPix1 = WIDTHBYTES(inputImage.Width() * 24);
   	 BYTE *buf2 = (BYTE *)outputImage.GetImage()->imageData;
	 int WidthPix2 = WIDTHBYTES(outputImage.Width() * 24);
	 BYTE *buf3 = (BYTE *)crossImage.GetImage()->imageData;
	 int WidthPix3 = WIDTHBYTES(crossImage.Width() * 24);

	 int i, j;
	 int meancolor = 0;
	 for (i = 0; i < crossImage.Height(); i++)
	 {
		for(j = 0; j < crossImage.Width(); j++)
		{
			meancolor += buf3[i * WidthPix3 + 3 * j];
		}
	 }

	 meancolor /= crossImage.Width() * crossImage.Height();

	 int iTemp;
     for(i = 0; i < h - 2; i = i + 2)
	 {
		 for(j = 0; j < w - 2; j = j + 2)
		 {
			for(int y = 0; y < crossImage.Height(); y++)
			{
				for(int x = 0; x < crossImage.Width(); x++)
				{	
					if(x < crossImage.Width() / 2 && y < crossImage.Height() / 2)
					{
						iTemp = int((buf3[y* WidthPix3 + 3 * x] - meancolor) * 0.3 + buf1[i * WidthPix1 + 3 * j]);
						iTemp = iTemp > 255 ? 510 - iTemp : iTemp;
						iTemp = iTemp < 0   ?     - iTemp : iTemp; 
						buf2[(i * crossImage.Height()/2 + y) * WidthPix2 + 3 * (j * crossImage.Width()/2 + x)] = iTemp;

						iTemp = int((buf3[y* WidthPix3 + 3 * x + 1] - meancolor) * 0.3 + buf1[i * WidthPix1 + 3 * j + 1]);
						iTemp = iTemp > 255 ? 510 - iTemp : iTemp;
						iTemp = iTemp < 0   ?     - iTemp : iTemp; 
						buf2[(i * crossImage.Height()/2 + y) * WidthPix2 + 3 * (j * crossImage.Width()/2 + x) + 1] = iTemp;

						iTemp = int((buf3[y* WidthPix3 + 3 * x + 2] - meancolor) * 0.3 + buf1[i * WidthPix1 + 3 * j + 2]);
						iTemp = iTemp > 255 ? 510 - iTemp : iTemp;
						iTemp = iTemp < 0   ?     - iTemp : iTemp; 
						buf2[(i * crossImage.Height()/2 + y) * WidthPix2 + 3 * (j * crossImage.Width()/2 + x) + 2] = iTemp;	 
					}
					else if(x >= crossImage.Width() / 2 && y < crossImage.Height() / 2)
					{
						iTemp = int((buf3[y* WidthPix3 + 3 * x] - meancolor) * 0.3 + buf1[i * WidthPix1 + 3 * (j + 1)]);
						iTemp = iTemp > 255 ? 510 - iTemp : iTemp;
						iTemp = iTemp < 0   ?     - iTemp : iTemp; 
						buf2[(i * crossImage.Height()/2 + y) * WidthPix2 + 3 * (j * crossImage.Width()/2 + x)] = iTemp;

						iTemp = int((buf3[y* WidthPix3 + 3 * x + 1] - meancolor) * 0.3 + buf1[i * WidthPix1 + 3 * (j + 1) + 1]);
						iTemp = iTemp > 255 ? 510 - iTemp : iTemp;
						iTemp = iTemp < 0   ?     - iTemp : iTemp; 
						buf2[(i * crossImage.Height()/2 + y) * WidthPix2 + 3 * (j * crossImage.Width()/2 + x) + 1] = iTemp;

						iTemp = int((buf3[y* WidthPix3 + 3 * x + 2] - meancolor) * 0.3 + buf1[i * WidthPix1 + 3 * (j + 1) + 2]);
						iTemp = iTemp > 255 ? 510 - iTemp : iTemp;
						iTemp = iTemp < 0   ?     - iTemp : iTemp; 
						buf2[(i * crossImage.Height()/2 + y) * WidthPix2 + 3 * (j * crossImage.Width()/2 + x) + 2] = iTemp;	 
					}
					else if(x < crossImage.Width() / 2 && y >= crossImage.Height() / 2)
					{
						iTemp = int((buf3[y* WidthPix3 + 3 * x] - meancolor) * 0.3 + buf1[(i + 1) * WidthPix1 + 3 * j]);
						iTemp = iTemp > 255 ? 510 - iTemp : iTemp;
						iTemp = iTemp < 0   ?     - iTemp : iTemp; 
						buf2[(i * crossImage.Height()/2 + y) * WidthPix2 + 3 * (j * crossImage.Width()/2 + x)] = iTemp;

						iTemp = int((buf3[y* WidthPix3 + 3 * x + 1] - meancolor) * 0.3 + buf1[(i + 1) * WidthPix1 + 3 * j + 1]);
						iTemp = iTemp > 255 ? 510 - iTemp : iTemp;
						iTemp = iTemp < 0   ?     - iTemp : iTemp; 
						buf2[(i * crossImage.Height()/2 + y) * WidthPix2 + 3 * (j * crossImage.Width()/2 + x) + 1] = iTemp;

						iTemp = int((buf3[y* WidthPix3 + 3 * x + 2] - meancolor) * 0.3 + buf1[(i + 1) * WidthPix1 + 3 * j  + 2]);
						iTemp = iTemp > 255 ? 510 - iTemp : iTemp;
						iTemp = iTemp < 0   ?     - iTemp : iTemp; 
						buf2[(i * crossImage.Height()/2 + y) * WidthPix2 + 3 * (j * crossImage.Width()/2 + x) + 2] = iTemp;	 
					}
					else
					{
						iTemp = int((buf3[y* WidthPix3 + 3 * x] - meancolor) * 0.3 + buf1[(i + 1) * WidthPix1 + 3 * (j + 1)]);
						iTemp = iTemp > 255 ? 510 - iTemp : iTemp;
						iTemp = iTemp < 0   ?     - iTemp : iTemp; 
						buf2[(i * crossImage.Height()/2 + y) * WidthPix2 + 3 * (j * crossImage.Width()/2 + x)] = iTemp;

						iTemp = int((buf3[y* WidthPix3 + 3 * x + 1] - meancolor) * 0.3 + buf1[(i + 1) * WidthPix1 + 3 * (j + 1) + 1]);
						iTemp = iTemp > 255 ? 510 - iTemp : iTemp;
						iTemp = iTemp < 0   ?     - iTemp : iTemp; 
						buf2[(i * crossImage.Height()/2 + y) * WidthPix2 + 3 * (j * crossImage.Width()/2 + x) + 1] = iTemp;

						iTemp = int((buf3[y* WidthPix3 + 3 * x + 2] - meancolor) * 0.3 + buf1[(i + 1) * WidthPix1 + 3 * (j + 1) + 2]);
						iTemp = iTemp > 255 ? 510 - iTemp : iTemp;
						iTemp = iTemp < 0   ?     - iTemp : iTemp; 
						buf2[(i * crossImage.Height()/2 + y) * WidthPix2 + 3 * (j * crossImage.Width()/2 + x) + 2] = iTemp;	 
					}
	
				}
			}
		 }
	 }

}

void CPortraitEditingDlg::OnCrossStitch() 
{
	// TODO: Add your control notification handler code here
	CvvImage outputImg;
	CvvImage crossImg;
	crossImg.Load(m_sPath + "\\res\\spx12.bmp");

//	BYTE *buf = (BYTE *)m_RenderImg.GetImage()->imageData;
//	int WidthPix = WIDTHBYTES(m_RenderImg.Width() * 24);
//	BYTE *buf1 = (BYTE *)m_RegionImg.GetImage()->imageData;
//	int WidthPix1 = WIDTHBYTES(m_RegionImg.Width() * 24);
//	for(int i = 0; i < m_RenderImg.Height(); i++)
//	{
//		for(int j = 0; j < m_RenderImg.Width(); j++)
//		{
//			if(buf[i * WidthPix + 3 * j] > 200)
//			{
//				buf[i * WidthPix + 3 * j] = 255;
//				buf[i * WidthPix + 3 * j + 1] = 255;
//				buf[i * WidthPix + 3 * j + 2] = 255;
//			}
//			else
//			{
//				buf[i * WidthPix + 3 * j] = 55;
//				buf[i * WidthPix + 3 * j + 1] = 55;
//				buf[i * WidthPix + 3 * j + 2] = 55;
//			}
//		}
//	}
//	m_RenderImg.Save(m_sPath + "\\temp\\binary.jpg");
//	m_RegionImg.Save(m_sPath + "\\temp\\region.jpg");
//
//	for(i = 0; i < m_RenderImg.Height(); i++)
//	{
//		for(int j = 0; j < m_RenderImg.Width(); j++)
//		{
//			if(buf[i * WidthPix + 3 * j] > 200)
//			{
//				buf[i * WidthPix + 3 * j] = buf1[i * WidthPix1 + 3 * j];
//				buf[i * WidthPix + 3 * j + 1] = buf1[i * WidthPix1 + 3 * j + 1];
//				buf[i * WidthPix + 3 * j + 2] = buf1[i * WidthPix1 + 3 * j + 2];
//			}
//		}
//	}
//	m_RenderImg.Save(m_sPath + "\\temp\\mixture.jpg");

	//ImageZoom(m_TargetImg, 0.5, 0.5);
	//CrossTransfer(m_RenderImg, outputImg, crossImg);

	CrossTransfer2(m_RenderImg, outputImg, crossImg);

	outputImg.Save(m_sPath + "\\temp\\cross.jpg");

    CcrossShow dglCrossShow;
	dglCrossShow.crossImg.Load(m_sPath + "\\temp\\cross.jpg");
	if (dglCrossShow.DoModal() != IDOK)
	{
		return;
	}

	Invalidate(FALSE);

	m_btCrossStitch.EnableWindow(FALSE);
}

void CPortraitEditingDlg::OnPortrait() 
{
	if(m_bImgOpened)
	{
		::SetCursor(::LoadCursor(NULL,IDC_WAIT));

		ImageChanged(m_TargetImg.GetImage(), 255);
		
		OnFeatureLocal();
		OnDetectBackground();
		OnDetectSkin();
		OnDetectHair();
		OnDetectCollar();

		m_nStatus = 0;
		m_image.Load(m_sPath + "\\temp" + "\\input.bmp");

		::SetCursor(::LoadCursor(NULL,IDC_ARROW));

		m_btFeature.EnableWindow(TRUE);
		m_btBackground.EnableWindow(TRUE);
		m_btSkin.EnableWindow(TRUE);
		m_btHair.EnableWindow(TRUE);
		m_btCollar.EnableWindow(TRUE);
		m_btDefaultColor.EnableWindow(TRUE);
		m_btHairColor.EnableWindow(TRUE);
		m_btSkinColor.EnableWindow(TRUE);
		m_btClothColor.EnableWindow(TRUE);
		m_btBackgroundColor.EnableWindow(TRUE);
		m_btShadowColor.EnableWindow(TRUE);
		m_btCrossStitch.EnableWindow(TRUE);
		m_btRender.EnableWindow(TRUE);
	}
	else
	{
		MessageBox("请打开一张正面图片!");
	}

}

void CPortraitEditingDlg::OnSave() 
{
	// TODO: Add your control notification handler code here
	if(m_bImgOpened)
	{
		CString strFileName;
		CString strExtension;

		CFileDialog dlg(FALSE, NULL, NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"JPEG Files (*.jpg)|*.jpg|Bitmap Files (*.bmp)|*.bmp|P2A Files (*.p2a)|*.p2a||");
		if( IDOK != dlg.DoModal())
		{
			return;
		}

		strFileName = dlg.m_ofn.lpstrFile;
		if (dlg.m_ofn.nFileExtension == 0)
		{
			switch (dlg.m_ofn.nFilterIndex)
			{
			case 1:
				strExtension = "jpg"; break;
			case 2:
				strExtension = "bmp"; break;
			case 3:
				strExtension = "p2a"; break;
			default:
				break;
			}
			strFileName = strFileName + '.' + strExtension;
		}

		if(strExtension == "jpg" || strExtension == "bmp")
			m_TargetImg.Save(strFileName);
		if(strExtension == "p2a")
		{
//			FILE *fp;
//			fp = fopen(strFileName, "w");
//			fprintf(fp, "%d\n", strImgName.GetLength());
//			fprintf(fp, "%s\n", strImgName);
//		
//			fprintf(fp, "%d\n", 105);
//			for(int i = 0; i < 105; i++)
//			{
//				fprintf(fp, "%d %d\n", m_ptArrLandMarks[i].x, m_ptArrLandMarks[i].y);
//			}
//
//			fprintf(fp, "%d\n", 91);
//			for(i = 0; i < 91; i++)
//			{
//				fprintf(fp, "%lf %lf\n", m_dpLandMarks[i].x, m_dpLandMarks[i].y);
//			}
//
//			int size = (int)m_backgroundVector.size();
//			fprintf(fp, "%d\n", size);
//			for(i = 0; i < size; i++)
//			{
//				fprintf(fp, "%lf %lf\n", m_backgroundVector[i].x, m_backgroundVector[i].y);
//			}
//
//			size = (int)m_skinVector.size();
//			fprintf(fp, "%d\n", size);
//			for(i = 0; i < size; i++)
//			{
//				fprintf(fp, "%lf %lf\n", m_skinVector[i].x, m_skinVector[i].y);
//			}
//
//			size = (int)hairVectorX.size();
//			fprintf(fp, "%d\n", size);
//			for(i = 0; i < size; i++)
//			{
//				fprintf(fp, "%lf %lf\n", hairVectorX[i], hairVectorY[i]);
//			}
//
//			size = (int)clothVectorX.size();
//			fprintf(fp, "%d\n", size);
//			for(i = 0; i < size; i++)
//			{
//				fprintf(fp, "%lf %lf\n", clothVectorX[i], clothVectorY[i]);
//			}
//
//			size = (int)leftEarVector.size();
//			fprintf(fp, "%d\n", size);
//			for(i = 0; i < size; i++)
//			{
//				fprintf(fp, "%lf %lf\n", leftEarVector[i].x, leftEarVector[i].y);
//			}
//
//			size = (int)rightEarVector.size();
//			fprintf(fp, "%d\n", size);
//			for(i = 0; i < size; i++)
//			{
//				fprintf(fp, "%lf %lf\n", rightEarVector[i].x, rightEarVector[i].y);
//			}
//
//			if(m_bBackground)
//				fprintf(fp, "%d\n", 1);
//			else
//				fprintf(fp, "%d\n", 0);
//			if(m_bSkin)
//				fprintf(fp, "%d\n", 1);
//			else
//				fprintf(fp, "%d\n", 0);
//			if(m_bHair)
//				fprintf(fp, "%d\n", 1);
//			else
//				fprintf(fp, "%d\n", 0);
//			if(m_bCollar)
//				fprintf(fp, "%d\n", 1);
//			else
//				fprintf(fp, "%d\n", 0);
//			if(m_bEar)
//				fprintf(fp, "%d\n", 1);
//			else
//				fprintf(fp, "%d\n", 0);
//
//			for(i = 0; i < 8; i++)
//			{
//				fprintf(fp, "%d\n", m_nCompSelected[i]);
//			}
//
//			fprintf(fp, "%d\n", hairIndex[0]);
//			fprintf(fp, "%d\n", clothIndex[0]);
//				
//			fclose(fp);

			SaveXML(strFileName);
		}
	}
	else
	{
		MessageBox("请打开一张正脸图片!");
	}

}

void CPortraitEditingDlg::OnExit() 
{
	// TODO: Add your control notification handler code here
	if(AfxMessageBox("确定退出?",MB_OKCANCEL) == IDOK)
	{
		CDialog::OnOK();
	}
	
}


void CPortraitEditingDlg::OnHair() 
{
	// TODO: Add your control notification handler code here

	if(m_bHair == true)
	{
		CString hairPath(m_sPath + "\\mode\\hair");
		AutoSelected(hairVectorX, hairVectorY, 10, hairPath, "\\hair", hairIndex);

		CString  styleName;
		CString  tempName;
		CvvImage tempImage;
		styleName.Format("\\style%d", hairIndex[0] + 1);
		for(int iCount = 1; ;iCount++)
		{
			tempName.Format("\\hair%d.bmp", iCount);
			if(tempImage.Load(m_sPath + "\\ico\\hair" + styleName + tempName) == false)
				break;
		}
		
		CString  FilePathName, FileName;
		m_ImageListHair.DeleteImageList();
		m_ImageListHair.Create (64,64,ILC_COLOR24,5,3);
		m_LstCtrComponent.DeleteAllItems();
		m_LstCtrComponent.SetImageList (&m_ImageListHair,LVSIL_NORMAL);
		for(int index = 0; index < iCount - 1; index++)
		{
			FileName.Format("\\hair%d.bmp", index + 1);
			FilePathName = m_sPath + "\\ico\\hair" + styleName + FileName;

			int len = FilePathName.GetLength() + 1;
			char *pName = new char[len];
			for(int t = 0; t < len - 1; t++)
				pName[t] = FilePathName.GetAt(t);
			pName[len - 1] = '\0';
			//ft.DeCryptFile(pName);
			
			HBITMAP hBitmap;
			CBitmap *pBitmap;
			pBitmap = new CBitmap;
			hBitmap = (HBITMAP)LoadImage(NULL,FilePathName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
			pBitmap->Attach (hBitmap);
			m_ImageListHair.Add (pBitmap,RGB(0,0,0));
			m_LstCtrComponent.InsertItem (index,NULL,index);
			delete pBitmap;
		
			//ft.EncryptFile(pName);
			delete [] pName;
		}
		
		m_nRender = 1;
	}
	else
	{
		MessageBox("请先执行头发定位操作!");
		
		m_nPart = -1;
		UpdateData(FALSE);
	}

}

void CPortraitEditingDlg::OnEyebrow() 
{
	// TODO: Add your control notification handler code here
	CString  FilePathName, FileName;
	m_ImageListEyebrow.DeleteImageList();
	m_ImageListEyebrow.Create (64,32,ILC_COLOR24,5,3);
	m_LstCtrComponent.DeleteAllItems();
	m_LstCtrComponent.SetImageList (&m_ImageListEyebrow,LVSIL_NORMAL);
	for(int index = 0; index < 20; index++)
	{
		FileName.Format("eyebrow%d.bmp", index + 1);
		FilePathName = m_sPath + "\\ico\\" + FileName;
		
		int len = FilePathName.GetLength() + 1;
		char *pName = new char[len];
		for(int t = 0; t < len - 1; t++)
			pName[t] = FilePathName.GetAt(t);
		pName[len - 1] = '\0';
		//ft.DeCryptFile(pName);

		HBITMAP hBitmap;
		CBitmap *pBitmap;
		pBitmap = new CBitmap;
		hBitmap = (HBITMAP)LoadImage(NULL,FilePathName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		pBitmap->Attach (hBitmap);
		m_ImageListEyebrow.Add (pBitmap,RGB(0,0,0));
		m_LstCtrComponent.InsertItem (index,NULL,index);
		delete pBitmap;

		//ft.EncryptFile(pName);
		delete [] pName;
	}

	m_nRender = 2;

}

void CPortraitEditingDlg::OnEye() 
{
	// TODO: Add your control notification handler code here
	CString  FilePathName, FileName;
	m_ImageListEye.DeleteImageList();
	m_ImageListEye.Create (64,32,ILC_COLOR24,5,3);
	m_LstCtrComponent.DeleteAllItems();
	m_LstCtrComponent.SetImageList (&m_ImageListEye,LVSIL_NORMAL);
	for(int index = 0; index < 34; index++)
	{
		FileName.Format("eye%d.bmp", index + 1);
		FilePathName = m_sPath + "\\ico\\" + FileName;
				
		int len = FilePathName.GetLength() + 1;
		char *pName = new char[len];
		for(int t = 0; t < len - 1; t++)
			pName[t] = FilePathName.GetAt(t);
		pName[len - 1] = '\0';
		//ft.DeCryptFile(pName);
		
		HBITMAP hBitmap;
		CBitmap *pBitmap;
		pBitmap = new CBitmap;
		hBitmap = (HBITMAP)LoadImage(NULL,FilePathName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		pBitmap->Attach (hBitmap);
		m_ImageListEye.Add (pBitmap,RGB(0,0,0));
		m_LstCtrComponent.InsertItem (index,NULL,index);
		delete pBitmap;

		//ft.EncryptFile(pName);
		delete [] pName;
	}

	m_nRender = 3;
}

void CPortraitEditingDlg::OnNose() 
{
	// TODO: Add your control notification handler code here
	CString  FilePathName, FileName;
	m_ImageListNose.DeleteImageList();
	m_ImageListNose.Create (64,64,ILC_COLOR24,5,3);
	m_LstCtrComponent.DeleteAllItems();
	m_LstCtrComponent.SetImageList (&m_ImageListNose,LVSIL_NORMAL);
	for(int index = 0; index < 32; index++)
	{
		FileName.Format("nose%d.bmp", index + 1);
		FilePathName = m_sPath + "\\ico\\" + FileName;

		int len = FilePathName.GetLength() + 1;
		char *pName = new char[len];
		for(int t = 0; t < len - 1; t++)
			pName[t] = FilePathName.GetAt(t);
		pName[len - 1] = '\0';
		//ft.DeCryptFile(pName);

		HBITMAP hBitmap;
		CBitmap *pBitmap;
		pBitmap = new CBitmap;
		hBitmap = (HBITMAP)LoadImage(NULL,FilePathName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		pBitmap->Attach (hBitmap);
	    m_ImageListNose.Add (pBitmap,RGB(0,0,0));
		m_LstCtrComponent.InsertItem (index,NULL,index);
		delete pBitmap;

		//ft.EncryptFile(pName);
		delete [] pName;
	}	

	m_nRender = 4;

}

void CPortraitEditingDlg::OnMouth() 
{
	// TODO: Add your control notification handler code here
	CString  FilePathName, FileName;
	m_ImageListMouth.DeleteImageList();
	m_ImageListMouth.Create (64,32,ILC_COLOR24,5,3);
	m_LstCtrComponent.DeleteAllItems();
	m_LstCtrComponent.SetImageList (&m_ImageListMouth,LVSIL_NORMAL);
	for(int index = 0; index < 33; index++)
	{
		FileName.Format("mouth%d.bmp", index + 1);
		FilePathName = m_sPath + "\\ico\\" + FileName;

		int len = FilePathName.GetLength() + 1;
		char *pName = new char[len];
		for(int t = 0; t < len - 1; t++)
			pName[t] = FilePathName.GetAt(t);
		pName[len - 1] = '\0';
		//ft.DeCryptFile(pName);

		HBITMAP hBitmap;
		CBitmap *pBitmap;
		pBitmap = new CBitmap;
		hBitmap = (HBITMAP)LoadImage(NULL,FilePathName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		pBitmap->Attach (hBitmap);
		m_ImageListMouth.Add (pBitmap,RGB(0,0,0));
		m_LstCtrComponent.InsertItem (index,NULL,index);
		delete pBitmap;

		//ft.EncryptFile(pName);
		delete [] pName;
	}	

	m_nRender = 5;

}

void CPortraitEditingDlg::OnCollar() 
{
	// TODO: Add your control notification handler code here
	if(m_bCollar == true)
	{
		CString clothPath(m_sPath + "\\mode\\cloth");
		AutoSelected(clothVectorX, clothVectorY, 7, clothPath, "\\cloth", clothIndex);

		CString  styleName;
		CString  tempName;
		CvvImage tempImage;
		styleName.Format("\\style%d", clothIndex[0] + 1);
		for(int iCount = 1; ;iCount++)
		{
			tempName.Format("\\collar%d.bmp", iCount);
			if(tempImage.Load(m_sPath + "\\ico\\cloth" + styleName + tempName) == false)
				break;
		}
		
		CString  FilePathName, FileName;
		m_ImageListCollar.DeleteImageList();
		m_ImageListCollar.Create (64,32,ILC_COLOR24,5,3);
		m_LstCtrComponent.DeleteAllItems();
		m_LstCtrComponent.SetImageList (&m_ImageListCollar,LVSIL_NORMAL);
		for(int index = 0; index < iCount - 1; index++)
		{
			FileName.Format("\\collar%d.bmp", index + 1);
			FilePathName = m_sPath + "\\ico\\cloth" + styleName + FileName;

			int len = FilePathName.GetLength() + 1;
			char *pName = new char[len];
			for(int t = 0; t < len - 1; t++)
				pName[t] = FilePathName.GetAt(t);
			pName[len - 1] = '\0';
			//ft.DeCryptFile(pName);

			HBITMAP hBitmap;
			CBitmap *pBitmap;
			pBitmap = new CBitmap;
			hBitmap = (HBITMAP)LoadImage(NULL,FilePathName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
			pBitmap->Attach (hBitmap);
			m_ImageListCollar.Add (pBitmap,RGB(0,0,0));
			m_LstCtrComponent.InsertItem (index,NULL,index);
			delete pBitmap;
		
			//ft.EncryptFile(pName);
			delete [] pName;;
		}
	}
	else
	{
		MessageBox("请先执行衣服定位操作!");

		m_nPart = -1;
		UpdateData(FALSE);
	}

	m_nRender = 6;
}

void CPortraitEditingDlg::OnBackground() 
{
	// TODO: Add your control notification handler code here
	CString  FilePathName, FileName;
	m_ImageListBG.DeleteImageList();
	m_ImageListBG.Create (64,48,ILC_COLOR24,5,3);
	m_LstCtrComponent.DeleteAllItems();
	m_LstCtrComponent.SetImageList (&m_ImageListBG,LVSIL_NORMAL);
	for(int index = 0; index < 22; index++)
	{
		FileName.Format("background%d.bmp", index + 1);
		FilePathName = m_sPath + "\\ico\\" + FileName;

		int len = FilePathName.GetLength() + 1;
		char *pName = new char[len];
		for(int t = 0; t < len - 1; t++)
			pName[t] = FilePathName.GetAt(t);
		pName[len - 1] = '\0';
		//ft.DeCryptFile(pName);

		HBITMAP hBitmap;
		CBitmap *pBitmap;
		pBitmap = new CBitmap;
		hBitmap = (HBITMAP)LoadImage(NULL,FilePathName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		pBitmap->Attach (hBitmap);
		m_ImageListBG.Add (pBitmap,RGB(0,0,0));
		m_LstCtrComponent.InsertItem (index,NULL,index);
		delete pBitmap;
	
		//ft.EncryptFile(pName);
		delete [] pName;;
	}

	m_nRender = 7;	

}

void CPortraitEditingDlg::OnEar() 
{
	// TODO: Add your control notification handler code here
	CString  FilePathName, FileName;
	m_ImageListEar.DeleteImageList();
	m_ImageListEar.Create (32,64,ILC_COLOR24,5,3);
	m_LstCtrComponent.DeleteAllItems();
	m_LstCtrComponent.SetImageList (&m_ImageListEar,LVSIL_NORMAL);
	for(int index = 0; index < 11; index++)
	{
		FileName.Format("ear%d.bmp", index + 1);
		FilePathName = m_sPath + "\\ico\\" + FileName;

		int len = FilePathName.GetLength() + 1;
		char *pName = new char[len];
		for(int t = 0; t < len - 1; t++)
			pName[t] = FilePathName.GetAt(t);
		pName[len - 1] = '\0';
		//ft.DeCryptFile(pName);

		HBITMAP hBitmap;
		CBitmap *pBitmap;
		pBitmap = new CBitmap;
		hBitmap = (HBITMAP)LoadImage(NULL,FilePathName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		pBitmap->Attach (hBitmap);
		m_ImageListEar.Add (pBitmap,RGB(0,0,0));
		m_LstCtrComponent.InsertItem (index,NULL,index);
		delete pBitmap;

		//ft.EncryptFile(pName);
		delete [] pName;
	}	

	m_nRender = 8;
}

void CPortraitEditingDlg::OnFacecontour() 
{
	// TODO: Add your control notification handler code here
	CString  FilePathName, FileName;
	m_ImageListFaceContour.DeleteImageList();
	m_ImageListFaceContour.Create (64,64,ILC_COLOR24,5,3);
	m_LstCtrComponent.DeleteAllItems();
	m_LstCtrComponent.SetImageList (&m_ImageListFaceContour,LVSIL_NORMAL);
	for(int index = 0; index < 20; index++)
	{
		FileName.Format("faceContour%d.bmp", index + 1);
		FilePathName = m_sPath + "\\ico\\" + FileName;

		int len = FilePathName.GetLength() + 1;
		char *pName = new char[len];
		for(int t = 0; t < len - 1; t++)
			pName[t] = FilePathName.GetAt(t);
		pName[len - 1] = '\0';
		//ft.DeCryptFile(pName);

		HBITMAP hBitmap;
		CBitmap *pBitmap;
		pBitmap = new CBitmap;
		hBitmap = (HBITMAP)LoadImage(NULL,FilePathName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		pBitmap->Attach (hBitmap);
		m_ImageListFaceContour.Add (pBitmap,RGB(0,0,0));
		m_LstCtrComponent.InsertItem (index,NULL,index);
		delete pBitmap;

		//ft.EncryptFile(pName);
		delete [] pName;
	}	

	m_nRender = 9;
}

void  CPortraitEditingDlg::AutoSelected(vector<double> &pointVectorX, vector<double> &pointVectorY, int templateSize, CString path, CString fileName, int index[])
{
	 int i,j;
 
	 vector<double> sourceVectorX;
	 vector<double> sourceVectorY;
	 vector<double> curX;
	 vector<double>	curY;
	 double *energy = new double[templateSize];
	 vector <double> energyVector;

	 for(int count = 0; count < templateSize; count++)
	 {
		 mySCEnergy    scEnergy;
		 
		 sourceVectorX.clear();
		 sourceVectorY.clear();
		 curX.clear();
		 curY.clear();
         
		 CString strCount;
		 strCount.Format("\\style%d", count + 1);
		 LoadTemplate(path + strCount + fileName+ "1.pts", sourceVectorX, sourceVectorY);
	 		
		int nSamp1 = pointVectorX.size();
		int nSamp2 = sourceVectorX.size();

		CvMat* pSrc = cvCreateMat(nSamp1, 2, CV_64FC1);
		CvMat* pTar = cvCreateMat(nSamp2, 2, CV_64FC1);
		
		for(i = 0; i < nSamp1; i++)
		{
			cvmSet(pSrc, i, 0, pointVectorX[i]);
			cvmSet(pSrc, i, 1, pointVectorY[i]);
		}
		for(i = 0; i < nSamp2; i++)
		{
			cvmSet(pTar, i, 0, sourceVectorX[i]);
			cvmSet(pTar, i, 1, sourceVectorY[i]);
		}

		energy[count] = scEnergy.SCEnergy(pSrc, pTar);
		energyVector.push_back(scEnergy.SCEnergy(pSrc, pTar));
	
	 }

	 FILE *fp;
	 fp = fopen("c:\\enery.txt", "w");
	 for(i = 0; i < templateSize; i++)
	 {
		 if(energy[i] < 0.15)
			fprintf(fp, "%d %lf\n", i + 1, energy[i]);
	 }
	 for(i = 0; i < templateSize; i++)
	 {
		 if(energy[i] >= 0.15 && energy[i] < 0.2)
			fprintf(fp, "%d %lf\n", i + 1, energy[i]);
	 }
	 for(i = 0; i < templateSize; i++)
	 {
		 if(energy[i] >= 0.2 && energy[i] < 0.25)
			fprintf(fp, "%d %lf\n", i + 1, energy[i]);
	 }
	 for(i = 0; i < templateSize; i++)
	 {
		 if(energy[i] >= 0.25 && energy[i] < 0.3)
			fprintf(fp, "%d %lf\n", i + 1, energy[i]);
	 }
	 for(i = 0; i < templateSize; i++)
	 {
		 if(energy[i] >= 0.3)
			fprintf(fp, "%d %lf\n", i + 1, energy[i]);
	 }
	 fclose(fp);

	 for(i = 0; i < templateSize - 1; i++)
	 {
		 for(j = i + 1; j < templateSize; j++)
		 {
			 if(energyVector[i] > energyVector[j])
			 {
				 double tempV = energyVector[i];
				 energyVector[i] = energyVector[j];
				 energyVector[j] = tempV;
			 }
		 }
	 }

	 for(j = 0; j < 2; j++)
	 {
		 for(i = 0; i < templateSize; i++)
		 {
			 if(energy[i] == energyVector[j])
			 {
				 index[j] = i;
			 }
		 }
	 }

	 delete [] energy;


//	 fp = fopen("c:\\eneryN.txt", "w");
//	 for(int mi = 0; mi < 110; mi++)
//	 {
//		 mySCEnergy    scEnergy;
//		 
//		 sourceVectorX.clear();
//		 sourceVectorY.clear();
//		 curX.clear();
//		 curY.clear();
//         
//		 CString strCount;
//		 strCount.Format("\\hair%d.pts", mi+1);//E:\\temp\\hair
//		 LoadTemplate("D:\\MyProject\\Component database\\hair" + strCount, sourceVectorX, sourceVectorY);
//
//		 		int size = sourceVectorX.size() + 1;
//				CShape sourShape, destShape;
//				sourShape.Resize(size);
//				for(i = 0; i < size; i++)
//				{
//					if(i < size - 1)
//					{
//						sourShape.PointX(i) = sourceVectorX[i];
//						sourShape.PointY(i) = sourceVectorY[i];
//					}
//					else
//					{
//						sourShape.PointX(i) = sourceVectorX[0];
//						sourShape.PointY(i) = sourceVectorY[0];
//					}
//				
//				}
//				ResampleCurve(sourShape, destShape, 20);
//				sourceVectorX.clear();
//				sourceVectorY.clear();
//				for(i = 0; i < destShape.PointNum(); i++)
//				{
//					sourceVectorX.push_back(destShape.PointX(i));
//					sourceVectorY.push_back(destShape.PointY(i));
//				}
//	 		
//		int nSamp1 = pointVectorX.size();
//		int nSamp2 = sourceVectorX.size();
//
//		CvMat* pSrc = cvCreateMat(nSamp1, 2, CV_64FC1);
//		CvMat* pTar = cvCreateMat(nSamp2, 2, CV_64FC1);
//		
//		for(i = 0; i < nSamp1; i++)
//		{
//			cvmSet(pSrc, i, 0, pointVectorX[i]);
//			cvmSet(pSrc, i, 1, pointVectorY[i]);
//		}
//		for(i = 0; i < nSamp2; i++)
//		{
//			cvmSet(pTar, i, 0, sourceVectorX[i]);
//			cvmSet(pTar, i, 1, sourceVectorY[i]);
//		}
//
//		double en = scEnergy.SCEnergy(pSrc, pTar);
//	
//		fprintf(fp, "%lf ", en);
//	 }
//	 fclose(fp);

}

void CPortraitEditingDlg::OnAutorender() 
{
	// TODO: Add your control notification handler code here
	::SetCursor(::LoadCursor(NULL,IDC_WAIT));

	 CString hairPath(m_sPath + "\\mode\\hair");
	 AutoSelected(hairVectorX, hairVectorY, 10, hairPath, "\\hair", hairIndex);

//	 CString clothPath(m_sPath + "\\mode\\cloth");
//	 int clothIndex[4];
//	 AutoSelected(clothVectorX, clothVectorY, 21, clothPath, "\\cloth", clothIndex);
	 
//	 CString eyebrowPath(m_sPath + "\\mode\\eyebrow");
//	 int eyebrowIndex[4];
//	 vector<double> eyebrowVectorX;
//	 vector<double> eyebrowVectorY;
//	 for(int i = 0; i < 8; i++)
//	 {
//		eyebrowVectorX.push_back(m_ptArrLandMarks[33 + i].x);
//		eyebrowVectorY.push_back(m_ptArrLandMarks[33 + i].y);
//	 }
//	  AutoSelected(eyebrowVectorX, eyebrowVectorY, 13, eyebrowPath, "\\eyebrow", eyebrowIndex);
//
//	 CString nosePath(m_sPath + "\\mode\\nose");
//	 int noseIndex[4];
//	 vector<double> noseVectorX;
//	 vector<double> noseVectorY;
//	 for(i = 0; i < 13; i++)
//	 {
//	 	noseVectorX.push_back(m_ptArrLandMarks[61 + i].x);
//		noseVectorY.push_back(m_ptArrLandMarks[61 + i].y);
//	 }
//	  AutoSelected(noseVectorX, noseVectorY, 13, nosePath, "\\nose", noseIndex);
//
//	 CString mouthPath(m_sPath + "\\mode\\mouth");
//	 int mouthIndex[4];
//	 vector<double> mouthVectorX;
//	 vector<double> mouthVectorY;
//	 for(i = 0; i < 12; i++)
//	 {
//		mouthVectorX.push_back(m_ptArrLandMarks[74 + i].x);
//		mouthVectorY.push_back(m_ptArrLandMarks[74 + i].y);
//	 }
//	  AutoSelected(mouthVectorX, mouthVectorY, 14, mouthPath, "\\mouth", mouthIndex);
//	  
//	  	RenderHair(hairIndex[1]);
//	    RenderEyebrow(eyebrowIndex[1]);
//	   	RenderNose(noseIndex[1]);
//		RenderMouth(mouthIndex[1]);
//		m_TargetImg.Save(m_sPath + "\\temp" + "\\Render1.jpg");
//
//		RenderHair(hairIndex[2]);
//	    RenderEyebrow(eyebrowIndex[2]);
//	   	RenderNose(noseIndex[2]);
//		RenderMouth(mouthIndex[2]);
//		m_TargetImg.Save(m_sPath + "\\temp" + "\\Render2.jpg");
//
//			  	
//		RenderHair(hairIndex[3]);
//	    RenderEyebrow(eyebrowIndex[3]);
//	   	RenderNose(noseIndex[3]);
//		RenderMouth(mouthIndex[3]);
//		m_TargetImg.Save(m_sPath + "\\temp" + "\\Render3.jpg");
//		
//	  	RenderHair(hairIndex[0]);
//	    RenderEyebrow(eyebrowIndex[0]);
//	   	RenderNose(noseIndex[0]);
//		RenderMouth(mouthIndex[0]);
//
//		m_bAuto = true;

	 ::SetCursor(::LoadCursor(NULL,IDC_ARROW));	
}

void CPortraitEditingDlg::SmoothCurve(vector<dPoint> &pointVector, double param1, double param2)
{
	vector<dPoint> vSmoothedPoints;

	// Smooth
	vSmoothedPoints.resize(pointVector.size());
	vSmoothedPoints[0].x = pointVector[0].x;
	vSmoothedPoints[0].y = pointVector[0].y;

	vSmoothedPoints[pointVector.size()-1].x = pointVector[pointVector.size()-1].x;
	vSmoothedPoints[pointVector.size()-1].y = pointVector[pointVector.size()-1].y;

	for (int i=1; i<pointVector.size()-1; i++)
	{
		vSmoothedPoints[i].x = pointVector[i].x * (1 - param1) + (pointVector[i-1].x + pointVector[i+1].x) * 0.5 * param1;
		vSmoothedPoints[i].y = pointVector[i].y * (1 - param1) + (pointVector[i-1].y + pointVector[i+1].y) * 0.5 * param1;
	}
		
	// push
	vector<dPoint> vDet;
	vDet.resize(pointVector.size());
	for (i=0; i<pointVector.size(); i++)
	{
		vDet[i].x = vSmoothedPoints[i].x - pointVector[i].x;
		vDet[i].y = vSmoothedPoints[i].y - pointVector[i].y;
	}
			
	dPoint SumDet;
	for (i=1; i<pointVector.size()-1; i++)
	{
		SumDet.x = (vDet[i].x + vDet[i-1].x + vDet[i+1].x)/3;
		SumDet.y = (vDet[i].y + vDet[i-1].y + vDet[i+1].y)/3;

		pointVector[i].x = vSmoothedPoints[i].x - SumDet.x * param2;
		pointVector[i].y = vSmoothedPoints[i].y - SumDet.y * param2;
	}
}

void CPortraitEditingDlg::LoadASMModel()
{
	if (NULL == m_pModel)
	{
		m_pModel = new CASMMLSearchModelBayesian;
	}
	
	CInputDataFile ifs(m_sPath + "\\mode" + "\\xu91.asm");
	ifs >> *(CASMML2DSearchModel*)m_pModel;
	if (ifs.HasError())
	{
		return;
	}
	//MessageBox("载入模型成功！");
}

void CPortraitEditingDlg::ChangeLandmarks()
{
	m_CurShape.PointX(0) = m_ptArrLandMarks[45].x;
	m_CurShape.PointY(0) = m_ptArrLandMarks[45].y;
	m_CurShape.PointX(1) = m_ptArrLandMarks[46].x;
	m_CurShape.PointY(1) = m_ptArrLandMarks[46].y;
	m_CurShape.PointX(2) = m_ptArrLandMarks[47].x;
	m_CurShape.PointY(2) = m_ptArrLandMarks[47].y;
	m_CurShape.PointX(3) = m_ptArrLandMarks[48].x;
	m_CurShape.PointY(3) = m_ptArrLandMarks[48].y;
	m_CurShape.PointX(4) = m_ptArrLandMarks[41].x;
	m_CurShape.PointY(4) = m_ptArrLandMarks[41].y;
	m_CurShape.PointX(5) = m_ptArrLandMarks[42].x;
	m_CurShape.PointY(5) = m_ptArrLandMarks[42].y;
	m_CurShape.PointX(6) = m_ptArrLandMarks[43].x;
	m_CurShape.PointY(6) = m_ptArrLandMarks[43].y;
	m_CurShape.PointX(7) = m_ptArrLandMarks[44].x;
	m_CurShape.PointY(7) = m_ptArrLandMarks[45].y;
	m_CurShape.PointX(8) = m_ptArrLandMarks[49].x;
	m_CurShape.PointY(8) = m_ptArrLandMarks[49].y;
	for(int i = 9; i < 16; i++)
	{
		m_CurShape.PointX(i) = m_ptArrLandMarks[65 - i].x;
		m_CurShape.PointY(i) = m_ptArrLandMarks[65 - i].y;
	}

	m_CurShape.PointX(16) = m_ptArrLandMarks[33].x;
	m_CurShape.PointY(16) = m_ptArrLandMarks[33].y - 2;
	m_CurShape.PointX(17) = m_ptArrLandMarks[34].x;
	m_CurShape.PointY(17) = m_ptArrLandMarks[34].y;
	m_CurShape.PointX(18) = m_ptArrLandMarks[35].x;
	m_CurShape.PointY(18) = m_ptArrLandMarks[35].y;
	m_CurShape.PointX(19) = m_ptArrLandMarks[36].x;
	m_CurShape.PointY(19) = m_ptArrLandMarks[36].y;
	m_CurShape.PointX(20) = m_ptArrLandMarks[37].x;
	m_CurShape.PointY(20) = m_ptArrLandMarks[37].y - 2;
	m_CurShape.PointX(21) = m_ptArrLandMarks[37].x;
	m_CurShape.PointY(21) = m_ptArrLandMarks[37].y + 2;
	m_CurShape.PointX(22) = m_ptArrLandMarks[38].x;
	m_CurShape.PointY(22) = m_ptArrLandMarks[38].y;
	m_CurShape.PointX(23) = m_ptArrLandMarks[39].x;
	m_CurShape.PointY(23) = m_ptArrLandMarks[39].y;
	m_CurShape.PointX(24) = m_ptArrLandMarks[40].x;
	m_CurShape.PointY(24) = m_ptArrLandMarks[40].y;
	m_CurShape.PointX(25) = m_ptArrLandMarks[33].x;
	m_CurShape.PointY(25) = m_ptArrLandMarks[33].y + 2;

	m_CurShape.PointX(26) = m_ptArrLandMarks[29].x;
	m_CurShape.PointY(26) = m_ptArrLandMarks[29].y - 2;
	m_CurShape.PointX(27) = m_ptArrLandMarks[28].x;
	m_CurShape.PointY(27) = m_ptArrLandMarks[28].y;
	m_CurShape.PointX(28) = m_ptArrLandMarks[27].x;
	m_CurShape.PointY(28) = m_ptArrLandMarks[27].y;
	m_CurShape.PointX(29) = m_ptArrLandMarks[26].x;
	m_CurShape.PointY(29) = m_ptArrLandMarks[26].y;
	m_CurShape.PointX(30) = m_ptArrLandMarks[25].x;
	m_CurShape.PointY(30) = m_ptArrLandMarks[25].y - 2;
	m_CurShape.PointX(31) = m_ptArrLandMarks[25].x;
	m_CurShape.PointY(31) = m_ptArrLandMarks[25].y + 2;
	m_CurShape.PointX(32) = m_ptArrLandMarks[32].x;
	m_CurShape.PointY(32) = m_ptArrLandMarks[32].y;
	m_CurShape.PointX(33) = m_ptArrLandMarks[31].x;
	m_CurShape.PointY(33) = m_ptArrLandMarks[31].y;
	m_CurShape.PointX(34) = m_ptArrLandMarks[30].x;
	m_CurShape.PointY(34) = m_ptArrLandMarks[30].y;
	m_CurShape.PointX(35) = m_ptArrLandMarks[29].x;
	m_CurShape.PointY(35) = m_ptArrLandMarks[29].y + 2;

	m_CurShape.PointX(36) = m_ptArrLandMarks[61].x;
	m_CurShape.PointY(36) = m_ptArrLandMarks[61].y;
	m_CurShape.PointX(37) = m_ptArrLandMarks[62].x;
	m_CurShape.PointY(37) = m_ptArrLandMarks[62].y;
	m_CurShape.PointX(38) = m_ptArrLandMarks[64].x;
	m_CurShape.PointY(38) = m_ptArrLandMarks[64].y;
	m_CurShape.PointX(39) = m_ptArrLandMarks[65].x;
	m_CurShape.PointY(39) = m_ptArrLandMarks[65].y;
	m_CurShape.PointX(40) = m_ptArrLandMarks[66].x;
	m_CurShape.PointY(40) = m_ptArrLandMarks[66].y;
	m_CurShape.PointX(41) = m_ptArrLandMarks[67].x - 2;
	m_CurShape.PointY(41) = m_ptArrLandMarks[67].y;
	m_CurShape.PointX(42) = m_ptArrLandMarks[67].x + 2;
	m_CurShape.PointY(42) = m_ptArrLandMarks[67].y;
	m_CurShape.PointX(43) = m_ptArrLandMarks[68].x;
	m_CurShape.PointY(43) = m_ptArrLandMarks[68].y;
	m_CurShape.PointX(44) = m_ptArrLandMarks[69].x;
	m_CurShape.PointY(44) = m_ptArrLandMarks[69].y;
	m_CurShape.PointX(45) = m_ptArrLandMarks[70].x;
	m_CurShape.PointY(45) = m_ptArrLandMarks[70].y;
	m_CurShape.PointX(46) = m_ptArrLandMarks[72].x;
	m_CurShape.PointY(46) = m_ptArrLandMarks[72].y;
	m_CurShape.PointX(47) = m_ptArrLandMarks[73].x;
	m_CurShape.PointY(47) = m_ptArrLandMarks[73].y;

	for(i = 48; i < 68; i++)
	{
		if(i < 65)
		{
			m_CurShape.PointX(i) = m_ptArrLandMarks[26 + i].x;
			m_CurShape.PointY(i) = m_ptArrLandMarks[26 + i].y;
		}
		else
		{
			m_CurShape.PointX(i) = m_ptArrLandMarks[27 + i].x;
			m_CurShape.PointY(i) = m_ptArrLandMarks[27 + i].y;
		}	
	}

	m_CurShape.PointX(68) = m_ptArrLandMarks[0].x + 2;
	m_CurShape.PointY(68) = 3 * m_ptArrLandMarks[0].y - 2 * m_ptArrLandMarks[1].y;
	m_CurShape.PointX(69) = m_ptArrLandMarks[0].x + 1;
	m_CurShape.PointY(69) = 2 * m_ptArrLandMarks[0].y - m_ptArrLandMarks[1].y;
	m_CurShape.PointX(70) = m_ptArrLandMarks[0].x;
	m_CurShape.PointY(70) = m_ptArrLandMarks[0].y;
	m_CurShape.PointX(71) = m_ptArrLandMarks[2].x;
	m_CurShape.PointY(71) = m_ptArrLandMarks[2].y;
	m_CurShape.PointX(72) = m_ptArrLandMarks[3].x;
	m_CurShape.PointY(72) = m_ptArrLandMarks[3].y;
	m_CurShape.PointX(73) = m_ptArrLandMarks[4].x;
	m_CurShape.PointY(73) = m_ptArrLandMarks[4].y;
	m_CurShape.PointX(74) = m_ptArrLandMarks[5].x;
	m_CurShape.PointY(74) = m_ptArrLandMarks[5].y;
	m_CurShape.PointX(75) = m_ptArrLandMarks[7].x;
	m_CurShape.PointY(75) = m_ptArrLandMarks[7].y;
	m_CurShape.PointX(76) = m_ptArrLandMarks[9].x;
	m_CurShape.PointY(76) = m_ptArrLandMarks[9].y;
	m_CurShape.PointX(77) = m_ptArrLandMarks[10].x;
	m_CurShape.PointY(77) = m_ptArrLandMarks[10].y;
	m_CurShape.PointX(78) = m_ptArrLandMarks[11].x;
	m_CurShape.PointY(78) = m_ptArrLandMarks[11].y;
	m_CurShape.PointX(79) = m_ptArrLandMarks[12].x;
	m_CurShape.PointY(79) = m_ptArrLandMarks[12].y;
	m_CurShape.PointX(80) = m_ptArrLandMarks[13].x;
	m_CurShape.PointY(80) = m_ptArrLandMarks[13].y;
	m_CurShape.PointX(81) = m_ptArrLandMarks[14].x;
	m_CurShape.PointY(81) = m_ptArrLandMarks[14].y;
	m_CurShape.PointX(82) = m_ptArrLandMarks[15].x;
	m_CurShape.PointY(82) = m_ptArrLandMarks[15].y;
	m_CurShape.PointX(83) = m_ptArrLandMarks[17].x;
	m_CurShape.PointY(83) = m_ptArrLandMarks[17].y;
	m_CurShape.PointX(84) = m_ptArrLandMarks[19].x;
	m_CurShape.PointY(84) = m_ptArrLandMarks[19].y;
	m_CurShape.PointX(85) = m_ptArrLandMarks[20].x;
	m_CurShape.PointY(85) = m_ptArrLandMarks[20].y;
	m_CurShape.PointX(86) = m_ptArrLandMarks[21].x;
	m_CurShape.PointY(86) = m_ptArrLandMarks[21].y;
	m_CurShape.PointX(87) = m_ptArrLandMarks[22].x;
	m_CurShape.PointY(87) = m_ptArrLandMarks[22].y;
	m_CurShape.PointX(88) = m_ptArrLandMarks[24].x;
	m_CurShape.PointY(88) = m_ptArrLandMarks[24].y;
	m_CurShape.PointX(89) = m_ptArrLandMarks[24].x - 1;
	m_CurShape.PointY(89) = 2 * m_ptArrLandMarks[24].y - m_ptArrLandMarks[23].y;
	m_CurShape.PointX(90) = m_ptArrLandMarks[24].x - 2;
	m_CurShape.PointY(90) = 3 * m_ptArrLandMarks[24].y - 2 * m_ptArrLandMarks[23].y;
}

void CPortraitEditingDlg::ModifyLandmarks()
{
	m_ptArrLandMarks[45].x = m_CurShape.PointX(0);
	m_ptArrLandMarks[45].y = m_CurShape.PointY(0);
	m_ptArrLandMarks[46].x = m_CurShape.PointX(1);
	m_ptArrLandMarks[46].y = m_CurShape.PointY(1);
	m_ptArrLandMarks[47].x = m_CurShape.PointX(2);
	m_ptArrLandMarks[47].y = m_CurShape.PointY(2);
	m_ptArrLandMarks[48].x = m_CurShape.PointX(3);
	m_ptArrLandMarks[48].y = m_CurShape.PointY(3);
	m_ptArrLandMarks[41].x = m_CurShape.PointX(4);
	m_ptArrLandMarks[41].y = m_CurShape.PointY(4);
	m_ptArrLandMarks[42].x = m_CurShape.PointX(5);
	m_ptArrLandMarks[42].y = m_CurShape.PointY(5);
	m_ptArrLandMarks[43].x = m_CurShape.PointX(6);
	m_ptArrLandMarks[43].y = m_CurShape.PointY(6);
	m_ptArrLandMarks[44].x = m_CurShape.PointX(7);
	m_ptArrLandMarks[45].y = m_CurShape.PointY(7);
	m_ptArrLandMarks[49].x = m_CurShape.PointX(8);
	m_ptArrLandMarks[49].y = m_CurShape.PointY(8);
	for(int i = 9; i < 16; i++)
	{
		m_ptArrLandMarks[65 - i].x = m_CurShape.PointX(i);
		m_ptArrLandMarks[65 - i].y = m_CurShape.PointY(i);
	}

	m_ptArrLandMarks[33].x = (m_CurShape.PointX(16) + m_CurShape.PointX(25))/2;
	m_ptArrLandMarks[33].y = (m_CurShape.PointY(16) + m_CurShape.PointY(25))/2;
	m_ptArrLandMarks[34].x = m_CurShape.PointX(17);
	m_ptArrLandMarks[34].y = m_CurShape.PointY(17);
    m_ptArrLandMarks[35].x = m_CurShape.PointX(18);
	m_ptArrLandMarks[35].y = m_CurShape.PointY(18);
	m_ptArrLandMarks[36].x = m_CurShape.PointX(19);
	m_ptArrLandMarks[36].y = m_CurShape.PointY(19);
	m_ptArrLandMarks[37].x = (m_CurShape.PointX(20) + m_CurShape.PointX(20))/2;
	m_ptArrLandMarks[37].y = (m_CurShape.PointY(21) + m_CurShape.PointY(21))/2;
	m_ptArrLandMarks[38].x = m_CurShape.PointX(22);
	m_ptArrLandMarks[38].y = m_CurShape.PointY(22);
	m_ptArrLandMarks[39].x = m_CurShape.PointX(23);
	m_ptArrLandMarks[39].y = m_CurShape.PointY(23);
	m_ptArrLandMarks[40].x = m_CurShape.PointX(24);
	m_ptArrLandMarks[40].y = m_CurShape.PointY(24);

    m_ptArrLandMarks[29].x = (m_CurShape.PointX(26) + m_CurShape.PointX(35))/2;
	m_ptArrLandMarks[29].y = (m_CurShape.PointY(26) + m_CurShape.PointY(35))/2;
	m_ptArrLandMarks[28].x = m_CurShape.PointX(27);
	m_ptArrLandMarks[28].y = m_CurShape.PointY(27);
	m_ptArrLandMarks[27].x = m_CurShape.PointX(28);
	m_ptArrLandMarks[27].y = m_CurShape.PointY(28);
	m_ptArrLandMarks[26].x = m_CurShape.PointX(29);
	m_ptArrLandMarks[26].y = m_CurShape.PointY(29);
	m_ptArrLandMarks[25].x = (m_CurShape.PointX(30) + m_CurShape.PointX(31))/2;
	m_ptArrLandMarks[25].y = (m_CurShape.PointY(30) + m_CurShape.PointY(31))/2;
	m_ptArrLandMarks[32].x = m_CurShape.PointX(32);
	m_ptArrLandMarks[32].y = m_CurShape.PointY(32);
	m_ptArrLandMarks[31].x = m_CurShape.PointX(33);
	m_ptArrLandMarks[31].y = m_CurShape.PointY(33);
	m_ptArrLandMarks[30].x = m_CurShape.PointX(34);
	m_ptArrLandMarks[30].y = m_CurShape.PointY(34);


	m_ptArrLandMarks[61].x = m_CurShape.PointX(36);
	m_ptArrLandMarks[61].y = m_CurShape.PointY(36);
	m_ptArrLandMarks[62].x = m_CurShape.PointX(37);
	m_ptArrLandMarks[62].y = m_CurShape.PointY(37);
//	m_ptArrLandMarks[63].x = (m_CurShape.PointX(37) + m_CurShape.PointX(38)) / 2;
//	m_ptArrLandMarks[63].y = (m_CurShape.PointY(37) + m_CurShape.PointY(38)) / 2;
//	m_ptArrLandMarks[64].x = m_CurShape.PointX(38);
//	m_ptArrLandMarks[64].y = m_CurShape.PointY(38);
//	m_ptArrLandMarks[65].x = m_CurShape.PointX(39);
//	m_ptArrLandMarks[65].y = m_CurShape.PointY(39);
//	m_ptArrLandMarks[66].x = m_CurShape.PointX(40);
//	m_ptArrLandMarks[66].y = m_CurShape.PointY(40);
//	m_ptArrLandMarks[68].x = m_CurShape.PointX(43);
//	m_ptArrLandMarks[68].y = m_CurShape.PointY(43);
//	m_ptArrLandMarks[69].x = m_CurShape.PointX(44);
//	m_ptArrLandMarks[69].y = m_CurShape.PointY(44);
//	m_ptArrLandMarks[70].x = m_CurShape.PointX(45);
//	m_ptArrLandMarks[70].y = m_CurShape.PointY(45);
//	m_ptArrLandMarks[71].x = (m_CurShape.PointX(45) + m_CurShape.PointX(45)) / 2;
//	m_ptArrLandMarks[71].y = (m_CurShape.PointY(46) + m_CurShape.PointY(46)) / 2;
	m_ptArrLandMarks[72].x = m_CurShape.PointX(46);
	m_ptArrLandMarks[72].y = m_CurShape.PointY(46);
	m_ptArrLandMarks[73].x = m_CurShape.PointX(47);
	m_ptArrLandMarks[73].y = m_CurShape.PointY(47);

	for(i = 48; i < 60; i++)
	{
		m_ptArrLandMarks[26 + i].x = m_CurShape.PointX(i);
		m_ptArrLandMarks[26 + i].y = m_CurShape.PointY(i);
	}
	
}

void CPortraitEditingDlg::ConnectLandmark91(CvScalar &color, int lineWidth)
{
	int i = 0;
	CvPoint FacePoint[19];
	for(i = 0; i < 19; i++)
	{
		FacePoint[i].x = m_dpLandMarks[70 + i].x;
		FacePoint[i].y = m_dpLandMarks[70 + i].y;
	}
	for(i = 0; i < 18; i++)
	{
		cvLine(m_TargetImg.GetImage(), FacePoint[i], FacePoint[i + 1], color, lineWidth, CV_AA);
	}

	//connect right eyebrow
	CvPoint RightEyebrowPoint[10];
	for(i = 0; i < 10; i++)
	{
		RightEyebrowPoint[i].x = m_dpLandMarks[26 + i].x;
		RightEyebrowPoint[i].y = m_dpLandMarks[26 + i].y;
	}
	for(i = 0; i < 10; i++)
	{
		if(i < 9)
			cvLine(m_TargetImg.GetImage(), RightEyebrowPoint[i], RightEyebrowPoint[i + 1], color, lineWidth, CV_AA);
		else
			cvLine(m_TargetImg.GetImage(), RightEyebrowPoint[9], RightEyebrowPoint[0], color, lineWidth, CV_AA);
	}

	//connect left eyebrow
	CvPoint LeftEyebrowPoint[10];
	for(i = 0; i < 10; i++)
	{
		LeftEyebrowPoint[i].x = m_dpLandMarks[16 + i].x;
		LeftEyebrowPoint[i].y = m_dpLandMarks[16 + i].y;
	}
	for(i = 0; i < 10; i++)
	{
		if(i < 9)
			cvLine(m_TargetImg.GetImage(), LeftEyebrowPoint[i], LeftEyebrowPoint[i + 1], color, lineWidth, CV_AA);
		else
			cvLine(m_TargetImg.GetImage(), LeftEyebrowPoint[9], LeftEyebrowPoint[0], color, lineWidth, CV_AA);
	}

	//connect left eye
	CvPoint LeftEyePoint[8];
	for(i = 0; i < 8; i++)
	{
		LeftEyePoint[i].x = m_dpLandMarks[i].x;
		LeftEyePoint[i].y = m_dpLandMarks[i].y;
	}
	for(i = 0; i < 8; i++)
	{
		if(i < 7)
			cvLine(m_TargetImg.GetImage(), LeftEyePoint[i], LeftEyePoint[i + 1], color, lineWidth, CV_AA);
		else
			cvLine(m_TargetImg.GetImage(), LeftEyePoint[7], LeftEyePoint[0], color, lineWidth, CV_AA);
	}

	//connect right eye
	CvPoint RightEyePoint[8];
	for(i = 0; i < 8; i++)
	{
		RightEyePoint[i].x = m_dpLandMarks[8 + i].x;
		RightEyePoint[i].y = m_dpLandMarks[8 + i].y;
	}
	for(i = 0; i < 8; i++)
	{
		if(i < 7)
			cvLine(m_TargetImg.GetImage(), RightEyePoint[i], RightEyePoint[i + 1], color, lineWidth, CV_AA);
		else
			cvLine(m_TargetImg.GetImage(), RightEyePoint[7], RightEyePoint[0], color, lineWidth, CV_AA);
	}

	//connect nose
	CvPoint NosePoint[15];
	for(i = 0; i < 15; i++)
	{
		if(i < 6)
		{
			NosePoint[i].x = m_ptArrLandMarks[61 + i].x;
			NosePoint[i].y = m_ptArrLandMarks[61 + i].y;
		}
		if(i == 6)
		{
			NosePoint[i].x = m_ptArrLandMarks[98].x;
			NosePoint[i].y = m_ptArrLandMarks[98].y;
		}
		if(i == 7)
		{
			NosePoint[i].x = m_ptArrLandMarks[67].x;
			NosePoint[i].y = m_ptArrLandMarks[67].y;
		}
		if(i == 8)
		{
			NosePoint[i].x = m_ptArrLandMarks[99].x;
			NosePoint[i].y = m_ptArrLandMarks[99].y;
		}
		if(i > 8)
		{
			NosePoint[i].x = m_ptArrLandMarks[59 + i].x;
			NosePoint[i].y = m_ptArrLandMarks[59 + i].y;
		}
	}

	CShape sourShape, destShape;
	sourShape.Resize(11);
	for(i = 0; i < 11; i++)
	{
		sourShape.PointX(i) = NosePoint[i + 1].x;
		sourShape.PointY(i) = NosePoint[i + 1].y;
	}
	ResampleCurve(sourShape, destShape, 10);
	for(i = 0; i < destShape.PointNum() - 1; i++)
	{
		CvPoint pt1, pt2;
		pt1.x = destShape.PointX(i);
		pt1.y = destShape.PointY(i);
		pt2.x = destShape.PointX(i + 1);
		pt2.y = destShape.PointY(i + 1);

		cvLine(m_TargetImg.GetImage(), pt1, pt2, color, lineWidth, CV_AA);
	}


	//connect mouth
	CvPoint MouthPoint[20];
	for(i = 0; i < 20; i++)
	{
		MouthPoint[i].x = m_dpLandMarks[48 + i].x;
		MouthPoint[i].y = m_dpLandMarks[48 + i].y;
	}

	double dist = GetDistance(MouthPoint[13].x, MouthPoint[13].y, MouthPoint[13].x, MouthPoint[19].y);
	for(i = 0; i < 20; i++)
	{
		if(i < 11)
			cvLine(m_TargetImg.GetImage(), MouthPoint[i], MouthPoint[i + 1], color, lineWidth, CV_AA);
		else
			cvLine(m_TargetImg.GetImage(), MouthPoint[11], MouthPoint[0], color, lineWidth, CV_AA);

		if(i > 15 && i < 19)
		{
			cvLine(m_TargetImg.GetImage(), MouthPoint[0], MouthPoint[12], color, lineWidth, CV_AA);
			cvLine(m_TargetImg.GetImage(), MouthPoint[i], MouthPoint[i + 1], color, lineWidth, CV_AA);
			cvLine(m_TargetImg.GetImage(), MouthPoint[19], MouthPoint[12], color, lineWidth, CV_AA);
			cvLine(m_TargetImg.GetImage(), MouthPoint[16], MouthPoint[6], color, lineWidth, CV_AA);
		}

		if(dist > 8 && i > 11 && i < 16)
		{
			cvLine(m_TargetImg.GetImage(), MouthPoint[i], MouthPoint[i + 1], color, lineWidth, CV_AA);
		}
	}
	
	Invalidate(FALSE);

	CImg<unsigned char> RetImg(m_TargetImg.Width(), m_TargetImg.Height(), 1, 3, 255);
//	CImg<unsigned char> flag(m_TargetImg.Width(), m_TargetImg.Height(), 1, 1, 0);
//	CImg<unsigned char> FlagImg(m_TargetImg.Width(), m_TargetImg.Height(), 1, 1, 0);
//	CImg<unsigned char> AlphaImg;
//
//	DoublePoint dp;
//	vector<DoublePoint> MidPoints;
//	CCurveMorphing CMorphing;
//	
//	for(i = 0; i < destShape.PointNum(); i++)
//	{
//		dp.x = destShape.PointX(i);
//		dp.y = destShape.PointY(i);
//		MidPoints.push_back(dp);
//	}
//	AlphaImg.load(m_sPath + "\\brush\\00.bmp");
//
//	CMorphing.MorphingToCurve(MidPoints, 20, 10, 10, AlphaImg, 2, RetImg, flag, FlagImg);
//	MidPoints.clear();
//
//	for(i = 0; i < 7; i++)
//	{
//		dp.x = MouthPoint[i].x;
//		dp.y = MouthPoint[i].y;
//		MidPoints.push_back(dp);
//	}
//	CMorphing.MorphingToCurve(MidPoints, 20, 10, 10, AlphaImg, 2, RetImg, flag, FlagImg);
//	MidPoints.clear();
//
//	for(i = 6; i < 12; i++)
//	{
//		dp.x = MouthPoint[i].x;
//		dp.y = MouthPoint[i].y;
//		MidPoints.push_back(dp);
//	}
//	dp.x = MouthPoint[0].x;
//	dp.y = MouthPoint[0].y;
//	MidPoints.push_back(dp);
//	CMorphing.MorphingToCurve(MidPoints, 20, 10, 10, AlphaImg, 2, RetImg, flag, FlagImg);
//	MidPoints.clear();
//
//	dp.x = MouthPoint[6].x;
//	dp.y = MouthPoint[6].y;
//	MidPoints.push_back(dp);
//	for(i = 16; i < 20; i++)
//	{
//		dp.x = MouthPoint[i].x;
//		dp.y = MouthPoint[i].y;
//		MidPoints.push_back(dp);
//	}
//	dp.x = MouthPoint[12].x;
//	dp.y = MouthPoint[12].y;
//	MidPoints.push_back(dp);
//	dp.x = MouthPoint[0].x;
//	dp.y = MouthPoint[0].y;
//	MidPoints.push_back(dp);
//	CMorphing.MorphingToCurve(MidPoints, 20, 10, 10, AlphaImg, 2, RetImg, flag, FlagImg);
//	MidPoints.clear();
//
	BrushImg = RetImg;
	RetImg.clear();
}

void CPortraitEditingDlg::Shapeinit()
{	
	m_pModel->SetAvgShape();
	m_CurShape = m_pModel->SearchResult();
//	double cx, cy;
//	m_CurShape.CenterPoint(cx, cy);
//	m_CurShape.Translate(m_SrcImage.Width()*0.5-cx, m_SrcImage.Height()*0.5-cy);

	ChangeLandmarks();
	m_fbInitShape = true;

	m_ConstrainShape = m_CurShape;
	m_vbConstrained.resize(m_ConstrainShape.PointNum());
	for (int iPoint=0; iPoint<m_ConstrainShape.PointNum(); iPoint++ )
	{
		 if(iPoint == 70 || iPoint == 73 || iPoint == 76 || iPoint == 79 || iPoint == 82 || iPoint == 85 || iPoint == 88
		 || iPoint == 0 || iPoint == 2 || iPoint == 4 || iPoint == 6 || iPoint == 8 || iPoint == 10 || iPoint == 12 || iPoint == 14
		 || iPoint == 48 || iPoint == 50 || iPoint == 51 || iPoint == 52 || iPoint == 54 || iPoint == 57)
			m_vbConstrained[iPoint] = true;
		 else
			m_vbConstrained[iPoint] = false;
	}

	m_pModel->SetSearchImage(&m_SrcGrayImage);
//	m_pModel->SetShape(m_CurShape);
//	m_pModel->SetConstrain(4.0, m_vbConstrained, m_ConstrainShape);
//	m_pModel->m_SearchMode = 1;
//	bool bConv = m_pModel->SearchIterative(1);
//	m_CurShape = m_pModel->SearchResult();

//	CvPoint facePoints[19];
//	for(int i = 0; i < 19; i++)
//	{
//		facePoints[i].x = m_CurShape.PointX(70 + i);
//		facePoints[i].y = m_CurShape.PointY(70 + i);
//	}
//	int length = 19;
//	float alpha = 0.1;
//	float beta = 0.5;
//	float gamma = 1.5;
//	CvSize win;
//	win.width = 3;
//	win.height = 3;
//
//	CvTermCriteria criteria; 
//	criteria.type=CV_TERMCRIT_ITER+CV_TERMCRIT_EPS; 
//	criteria.max_iter=100; 
//	criteria.epsilon=0.1; 
//
//	IplImage* grayImage = 0;		
//	grayImage = cvCreateImage(cvGetSize(m_image.GetImage()), IPL_DEPTH_8U, 1);
//	cvCvtColor(m_image.GetImage(), grayImage, CV_BGR2GRAY);	
//	cvSnakeImage(grayImage, facePoints, length, &alpha, &beta, &gamma, CV_VALUE, win, criteria);
//	
//	for(i = 0; i < 19; i++)
//	{
//		 m_CurShape.PointX(70 + i) = facePoints[i].x;
//		 m_CurShape.PointY(70 + i) = facePoints[i].y;
//	}
//
//	cvReleaseImage(&grayImage);


	for(int i = 0; i < 91; i++)
	{
		m_dpLandMarks[i].x = m_CurShape.PointX(i);
		m_dpLandMarks[i].y = m_CurShape.PointY(i);

		m_ConstrainShape.PointX(i) = m_CurShape.PointX(i);
		m_ConstrainShape.PointY(i) = m_CurShape.PointY(i);
	}
    

//	ModifyLandmarks();
}

void CPortraitEditingDlg::ConstrainASMSearch()
{
	if(m_fbInitShape)
	{
		m_pModel->NeedConstraint(true);

		m_pModel->SetShape(m_CurShape);
		m_pModel->SetConstrain(4.0, m_vbConstrained, m_ConstrainShape);
		m_pModel->m_SearchMode = 1;
		bool bConv = m_pModel->SearchIterative(1);
		m_CurShape = m_pModel->SearchResult();

		for(int i = 0; i < 91; i++)
		{
			m_dpLandMarks[i].x = m_CurShape.PointX(i);
			m_dpLandMarks[i].y = m_CurShape.PointY(i);

			m_ConstrainShape.PointX(i) = m_CurShape.PointX(i);
			m_ConstrainShape.PointY(i) = m_CurShape.PointY(i);
		}	
	}
}

HBRUSH CPortraitEditingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	CBrush brush;
	
	// TODO: Change any attributes of the DC here
	int nCtr = pWnd->GetDlgCtrlID();

	switch(nCtr)
	{
		case IDC_EFFECT1:
		case IDC_EFFECT2:
		case IDC_EFFECT3:
		case IDC_HAIR:
		case IDC_EYEBROW:
		case IDC_EYE:
		case IDC_NOSE:
		case IDC_MOUTH:
		case IDC_COLLAR:
		case IDC_BACKGROUND:
		case IDC_FORE:
		case IDC_BACKG:
		case IDC_EAR:
		case IDC_FACECONTOUR:

			brush.CreateStockObject(HOLLOW_BRUSH);
			pDC->SetBkMode(TRANSPARENT); 
			pDC->SetTextColor(RGB(0, 0, 0)); 
			return   brush;   
			break;
	
		default:;

	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void  CPortraitEditingDlg::VectorLine(CvvImage &templateImage, CvvImage &zoomImage, CPoint &point1, CPoint &point2, CPoint &Zpoint1, CPoint &Zpoint2)
{
	BYTE *buf = (BYTE *)templateImage.GetImage()->imageData;
	int WidthPix = WIDTHBYTES(templateImage.Width() * 24);
	BYTE *buf1 = (BYTE *)zoomImage.GetImage()->imageData;
	int WidthPix1 = WIDTHBYTES(zoomImage.Width() * 24);

	vector<CPoint> pointsVector, ZpointsVector;
	PointOnLine(point1, point2, pointsVector);
	PointOnLine(Zpoint1, Zpoint2, ZpointsVector);
	int sum = pointsVector.size();
	int Zsum = ZpointsVector.size();
	double ratio = sum * 1.0 / (Zsum * 1.0);

	for(int i = 0; i < Zsum; i++)
	{
		int Zx = ZpointsVector[i].x;
		int Zy = ZpointsVector[i].y;

		int ti = (int) (i * ratio);
		double diff = i * ratio - ti;
		int x = pointsVector[ti].x;
		int y = pointsVector[ti].y;

		int next = ti + 1;
		if(next >= sum)
			next = sum - 1;

		int nx = pointsVector[next].x;
		int ny = pointsVector[next].y;

		for(int j = -2; j <= 2; j++)
		{
			for(int k = -2; k <= 2; k++)
			{
				if(Zy + j >= 0 && Zy + j < zoomImage.Height() && Zx + k >= 0 && Zx + k < zoomImage.Width()
				&& y + j >= 0 && y + j < templateImage.Height() && x + k >= 0 && x + k < templateImage.Width()
				&& ny + j >= 0 && ny + j < templateImage.Height() && nx + k >= 0 && nx + k < templateImage.Width())
				{
					buf1[(Zy + j) * WidthPix1 + 3 * (Zx + k)] = (int)((1 - diff) * buf[(y + j) * WidthPix + 3 * (x + k)] + diff * buf[(ny + j) * WidthPix + 3 * (nx + k)]);
					buf1[(Zy + j) * WidthPix1 + 3 * (Zx + k) + 1] = (int)((1 - diff) * buf[(y + j) * WidthPix + 3 * (x + k) + 1] + diff * buf[(ny + j) * WidthPix + 3 * (nx + k) + 1]);
					buf1[(Zy + j) * WidthPix1 + 3 * (Zx + k) + 2] = (int)((1 - diff) * buf[(y + j) * WidthPix + 3 * (x + k) + 2] + diff * buf[(ny + j) * WidthPix + 3 * (nx + k) + 2]);
				}	
			}
		}
	}

//	int length = 3;
//	int num1 = abs(point1.x - point2.x);
//	int num2 = abs(point1.y - point2.y);
//	double k;
//	int xx, yy;
//	int x1, x2, x3, x4, y1, y2, y3, y4;
//	
//	if(num1 >= num2)
//	{
//		k = -(point2.y - point1.y) * 1.0 / ((point2.x - point1.x) * 1.0);
//	
//		xx = point1.x;	
//		yy = point1.y;
//		y1 = yy + length - 1;
//		y2 = yy - length + 1;
//		x1 = (int)((length - 1) * k + 0.5) + xx;
//		x2 = (int)((1 - length) * k + 0.5) + xx;
//
//		xx = point2.x;	
//		yy = point2.y;
//		y3 = yy + length - 1;
//		y4 = yy - length + 1;
//		x3 = (int)((length - 1) * k + 0.5) + xx;
//		x4 = (int)((1 - length) * k + 0.5) + xx;
//	}
//	else
//	{
//		k = -(point2.x - point1.x) * 1.0 / ((point2.y - point1.y) * 1.0);
//
//		xx = point1.x;	
//		yy = point1.y;
//		x1 = xx + length - 1;
//		x2 = xx - length + 1;
//		y1 = (int)((length - 1) * k + 0.5) + yy;
//		y2 = (int)((1 - length) * k + 0.5) + yy;
//
//		xx = point2.x;	
//		yy = point2.y;
//		x3 = xx + length - 1;
//		x4 = xx - length + 1;
//		y3 = (int)((length - 1) * k + 0.5) + yy;
//		y4 = (int)((1 - length) * k + 0.5) + yy;
//	}
//
//	int Znum1 = abs(Zpoint1.x - Zpoint2.x);
//	int Znum2 = abs(Zpoint1.y - Zpoint2.y);
//	double Zk;
//	int Zxx, Zyy;
//	int Zx1, Zx2, Zx3, Zx4, Zy1, Zy2, Zy3, Zy4;
//	
//	if(Znum1 >= Znum2)
//	{
//		Zk = -(Zpoint2.y - Zpoint1.y) * 1.0 / ((Zpoint2.x - Zpoint1.x) * 1.0);
//	
//		Zxx = Zpoint1.x;	
//		Zyy = Zpoint1.y;
//		Zy1 = Zyy + length - 1;
//		Zy2 = Zyy - length + 1;
//		Zx1 = (int)((length - 1) * Zk + 0.5) + Zxx;
//		Zx2 = (int)((1 - length) * Zk + 0.5) + Zxx;
//
//		Zxx = Zpoint2.x;	
//		Zyy = Zpoint2.y;
//		Zy3 = Zyy + length - 1;
//		Zy4 = Zyy - length + 1;
//		Zx3 = (int)((length - 1) * Zk + 0.5) + Zxx;
//		Zx4 = (int)((1 - length) * Zk + 0.5) + Zxx;
//	}
//	else
//	{
//		Zk = -(Zpoint2.x - Zpoint1.x) * 1.0 / ((Zpoint2.y - Zpoint1.y) * 1.0);
//
//		Zxx = Zpoint1.x;	
//		Zyy = Zpoint1.y;
//		Zx1 = Zxx + length - 1;
//		Zx2 = Zxx - length + 1;
//		Zy1 = (int)((length - 1) * Zk + 0.5) + Zyy;
//		Zy2 = (int)((1 - length) * Zk + 0.5) + Zyy;
//
//		Zxx = Zpoint2.x;	
//		Zyy = Zpoint2.y;
//		Zx3 = Zxx + length - 1;
//		Zx4 = Zxx - length + 1;
//		Zy3 = (int)((length - 1) * Zk + 0.5) + Zyy;
//		Zy4 = (int)((1 - length) * Zk + 0.5) + Zyy;
//	}
//
//	CPoint temPointL[3], temPointR[3];
//	temPointL[0].x = Zx1; temPointL[0].y = Zy1;
//	temPointL[1].x = Zx2; temPointL[1].y = Zy2;
//	temPointL[2].x = Zx3; temPointL[2].y = Zy3;
//	
//	temPointR[0].x = Zx4; temPointR[0].y = Zy4;
//	temPointR[1].x = Zx2; temPointR[1].y = Zy2;
//	temPointR[2].x = Zx3; temPointR[2].y = Zy3;
//
//	CRgn temRgnL, temRgnR;
//	CRect temRectL, temRectR;
//	temRgnL.CreatePolygonRgn(temPointL, 3, WINDING);
//	temRgnL.GetRgnBox(temRectL);
//
//	temRgnR.CreatePolygonRgn(temPointR, 3, WINDING);
//	temRgnR.GetRgnBox(temRectR);
//
//	int x, y;
//	double tx, ty;
//	for(y = temRectL.top; y < temRectL.bottom; y++)
//	{
//		for(x = temRectL.left; x < temRectL.right; x++)
//		{
//			if(temRgnL.PtInRegion(CPoint(x, y)))
//			{
//				TransformPoint(Zx1, Zy1, Zx2, Zy2, Zx3, Zy3, x1, y1, x2, y2, x3, y3, x, y, tx, ty);
//
//				buf1[y * WidthPix1 + 3 * x] = buf[(int)(ty) * WidthPix + 3 * (int)(tx)];
//				buf1[y * WidthPix1 + 3 * x + 1] = buf[(int)(ty) * WidthPix + 3 * (int)(tx) + 1];
//				buf1[y * WidthPix1 + 3 * x + 2] = buf[(int)(ty) * WidthPix + 3 * (int)(tx) + 2];
//			}
//		}
//	}
//	
//	for(y = temRectR.top; y < temRectR.bottom; y++)
//	{
//		for(x = temRectR.left; x < temRectR.right; x++)
//		{
//			if(temRgnR.PtInRegion(CPoint(x, y)))
//			{
//				TransformPoint(Zx4, Zy4, Zx2, Zy2, Zx3, Zy3, x4, y4, x2, y2, x3, y3, x, y, tx, ty);
//                  
//				if(buf[(int)(ty) * WidthPix + 3 * (int)(tx)] < 200)
//				{
//					buf1[y * WidthPix1 + 3 * x] = buf[(int)(ty) * WidthPix + 3 * (int)(tx)];
//					buf1[y * WidthPix1 + 3 * x + 1] = buf[(int)(ty) * WidthPix + 3 * (int)(tx) + 1];
//					buf1[y * WidthPix1 + 3 * x + 2] = buf[(int)(ty) * WidthPix + 3 * (int)(tx) + 2];
//				}
//			}
//		}
//	}

//	CPoint temPoint[4];
//	if(Zx1 > Zx3)
//	{
//		temPoint[0].x = Zx1 + 1; temPoint[0].y = Zy1;
//		temPoint[1].x = Zx2 + 1; temPoint[1].y = Zy2;
//		temPoint[2].x = Zx4 - 1; temPoint[2].y = Zy4;
//		temPoint[3].x = Zx3 - 1; temPoint[3].y = Zy3;
//	}
//	else
//	{
//		temPoint[0].x = Zx1 - 1; temPoint[0].y = Zy1;
//		temPoint[1].x = Zx2 - 1; temPoint[1].y = Zy2;
//		temPoint[2].x = Zx4 + 1; temPoint[2].y = Zy4;
//		temPoint[3].x = Zx3 + 1; temPoint[3].y = Zy3;
//	}
//
//	CRgn temRgn;
//	CRect temRect;
//	temRgn.CreatePolygonRgn(temPoint, 4, WINDING);
//	temRgn.GetRgnBox(temRect);
//
//	for(y = temRect.top; y < temRect.bottom; y++)
//	{
//		for(x = temRect.left; x < temRect.right; x++)
//		{
//			if(temRgn.PtInRegion(CPoint(x, y)))
//			{
//				buf1[y * WidthPix1 + 3 * x] = 0;
//				buf1[y * WidthPix1 + 3 * x + 1] = 0;
//				buf1[y * WidthPix1 + 3 * x + 2] = 0;
//			}
//		}
//	}
}

void  CPortraitEditingDlg::VectorCurve(CvvImage &templateImage, CvvImage &zoomImage, vector<double> &sPointsX, vector<double> &sPointsY, vector<double> &dPointsX, vector<double> &dPointsY)
{
	if(sPointsX.size() != dPointsX.size())
		return;

	BYTE *buf = (BYTE *)templateImage.GetImage()->imageData;
	int WidthPix = WIDTHBYTES(templateImage.Width() * 24);
	BYTE *buf1 = (BYTE *)zoomImage.GetImage()->imageData;
	int WidthPix1 = WIDTHBYTES(zoomImage.Width() * 24);

	CMyTPS  TPS;
	TPS.Create(dPointsX, dPointsY, sPointsX, sPointsY, 0);
	double tx, ty;

	int minX = 10000;
	int minY = 10000;
	int maxX = 0;
	int maxY = 0;
	for(int i = 0; i < dPointsX.size(); i++)
	{
		if(minX > dPointsX[i])
			minX = dPointsX[i];

		if(maxX < dPointsX[i])
			maxX = dPointsX[i];
	}
	for(i = 0; i < dPointsY.size(); i++)
	{
		if(minY > dPointsY[i])
			minY = dPointsY[i];

		if(maxY < dPointsY[i])
			maxY = dPointsY[i];
	}

	for(i = minY; i <= maxY; i++)
	{
		for(int j = minX; j <= maxX; j++)
		{
			TPS.TPSTransform(j, i, tx, ty);
			if(tx > 0 && tx < templateImage.Width() && ty > 0 && ty < templateImage.Height())
			{
				if(buf[(int)(ty) * WidthPix + (int)(tx) * 3] < 200)
				{
					buf1[i * WidthPix1 + j * 3] = buf[(int)(ty) * WidthPix + (int)(tx) * 3];
					buf1[i * WidthPix1 + j * 3 + 1] = buf[(int)(ty) * WidthPix + (int)(tx) * 3 + 1];
					buf1[i * WidthPix1 + j * 3 + 2] = buf[(int)(ty) * WidthPix + (int)(tx) * 3 + 2];
				}
			}
		}
	}	
}

void CPortraitEditingDlg::OnEffect1() 
{
	// TODO: Add your control notification handler code here
	if(m_bAuto == true)
	{
		m_TargetImg.Load(m_sPath + "\\temp" + "\\Render1.jpg");
		Invalidate(FALSE);
	}
	cvSaveImage("c:\\OnEffect1.jpg",m_TargetImg.GetImage());

}

void CPortraitEditingDlg::OnEffect2() 
{
	// TODO: Add your control notification handler code here
	if(m_bAuto == true)
	{
		m_TargetImg.Load(m_sPath + "\\temp" + "\\Render2.jpg");
		Invalidate(FALSE);
	}
	cvSaveImage("c:\\OnEffect2.jpg",m_TargetImg.GetImage());
}

void CPortraitEditingDlg::OnEffect3() 
{
	// TODO: Add your control notification handler code here
	if(m_bAuto == true)
	{
		m_TargetImg.Load(m_sPath + "\\temp" + "\\Render3.jpg");
		Invalidate(FALSE);
	}
	cvSaveImage("c:\\OnEffect3.jpg",m_TargetImg.GetImage());	
}

void CPortraitEditingDlg::OnFore() 
{
	if(m_image.GetImage()==NULL)
	{
		MessageBox("你还没有载入图像哦~!");
		return;
	}
	// TODO: Add your control notification handler code here
	m_nStatus = graphcutFore;

	int order[15];
	SelectNose(order);
}


void CPortraitEditingDlg::OnBackg() 
{
	if(m_image.GetImage()==NULL)
	{
		MessageBox("你还没有载入图像哦~!");
		return;
	}
	// TODO: Add your control notification handler code here
	m_nStatus = graphcutBack;
}

void CPortraitEditingDlg::OnReset() 
{
	// TODO: Add your control notification handler code here
	FrontStroke.clear();
	BackStroke.clear();

	Invalidate(FALSE);	
}

void CPortraitEditingDlg::OnGcut() 
{
	// TODO: Add your control notification handler code here
	if(FrontStroke.size() > 0 && BackStroke.size() > 0)
	{
		if(m_bGCInit == false)
		{
			Gcs->Initialize(OrignalImage);
			m_bGCInit = true;
		}

		ResultImg=Gcs->segmentation(FrontStroke, BackStroke);
		ResultImg=Gcs->segmentation(FrontStroke, BackStroke);

		IplImage* grayImage = 0;
		grayImage = cvCreateImage(cvGetSize(m_image.GetImage()), IPL_DEPTH_8U, 1);

		BYTE *buf = (BYTE *)grayImage->imageData;
		int WidthPix = WIDTHBYTES(grayImage->width * 8);
		BYTE *buf1 = (BYTE *)m_image.GetImage()->imageData;
		int WidthPix1 = WIDTHBYTES(m_image.Width() * 24);

		int i, j;
		for(i = 0; i < m_image.Height(); i++)
		{
			for(j = 0; j < m_image.Width(); j++)
			{
				buf[i * WidthPix + j] = ResultImg(j, i, 0);
			}
		}
         
		CvvImage tempImg;
		tempImg.Create(m_image.Width(), m_image.Height(), 8);
		cvCopy(grayImage, tempImg.GetImage());
		tempImg.Save("c:\\tt.jpg");
	

		cvErode(grayImage, grayImage, NULL, 3);
		cvDilate(grayImage, grayImage, NULL, 3);

		CvMemStorage* storage = cvCreateMemStorage(0);
		CvSeq* contours = 0;
		cvFindContours(grayImage, storage, &contours, sizeof(CvContour),
			CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0,0) );

		contours = cvApproxPoly(contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, 3, 1 );
			 
		CvTreeNodeIterator iteratorBackground;
		cvInitTreeNodeIterator( &iteratorBackground, contours, 1);
		while( (contours = (CvSeq*)cvNextTreeNode( &iteratorBackground )) != 0 )
		{
			 CvSeqReader reader;
			 int count = contours->total;
			 cvStartReadSeq( contours, &reader, 0 );
			 CvPoint pt1;
			 dPoint  pt2;  
			 if(count > 10)
			 {
				 m_nPart = -1;
				 UpdateData(FALSE);
				 
				 if(m_nGCStatus == 1)
				 {
					 m_backgroundVector.clear();
					 
					 CPoint *pBackPoints = new CPoint[count];
					 for( int i = 0; i < count; i++ )
					 {
						 CV_READ_SEQ_ELEM( pt1, reader );
						 pt2.x = pt1.x;
						 pt2.y = pt1.y;
						 m_backgroundVector.push_back(pt2);
						 
						 pBackPoints[i].x = pt1.x;
						 pBackPoints[i].y = pt1.y;
					 }

					 bgRgn.DeleteObject();
					 bgRgn.CreatePolygonRgn(pBackPoints, count, WINDING);
					 bgRgn.GetRgnBox(bgRect);
					 delete [] pBackPoints;

					 m_nStatus = background;
					 m_bBackground = true;
					 m_btSkin.EnableWindow(TRUE);
					 
					 break;
				 }
				 else if(m_nGCStatus == 2)
				 {
					 m_skinVectorCopy = m_skinVector;
					 m_skinVector.clear();

					 for( int i = 0; i < count; i++ )
					 {
						 CV_READ_SEQ_ELEM( pt1, reader );
						 pt2.x = pt1.x;
						 pt2.y = pt1.y;
						 m_skinVector.push_back(pt2);
					 }

					int size = m_skinVector.size() + 1;
					CShape sourShape, destShape;
					sourShape.Resize(size);
					for(i = 0; i < size; i++)
					{
						if(i < size - 1)
						{
							sourShape.PointX(i) = m_skinVector[i].x;
							sourShape.PointY(i) = m_skinVector[i].y;
						}
						else
						{
							sourShape.PointX(i) = m_skinVector[0].x;
							sourShape.PointY(i) = m_skinVector[0].y;
						}
					
					}
					ResampleCurve(sourShape, destShape, 20);
					m_skinVector.clear();
					dPoint temPoint;
					for(i = 0; i < destShape.PointNum(); i++)
					{
						temPoint.x = destShape.PointX(i);
						temPoint.y = destShape.PointY(i);
						if(temPoint.x < 0)
							temPoint.x = 0;
						if(temPoint.x > m_image.Width() - 1)
							temPoint.x = m_image.Width() - 1;
						if(temPoint.y < 0)
							temPoint.y = 0;
						if(temPoint.y > m_image.Height() - 1)
							temPoint.y = m_image.Height() - 1;

						m_skinVector.push_back(temPoint);
					}

					int nNum = m_skinVector.size();
					CPoint *points = new CPoint[nNum];
					for(int k = 0; k < nNum; k++)
					{
						points[k].x = m_skinVector[k].x;
						points[k].y = m_skinVector[k].y;
					}

					skinRgn.DeleteObject();
					skinRgn.CreatePolygonRgn(points, nNum, WINDING);
					skinRgn.GetRgnBox(skRect);
					delete [] points;
					
					ConncetContour(m_skinVectorCopy, m_TargetImg.GetImage(), CV_RGB(255,255,255), 2, true);
					vector<double> faceVectorX, faceVectorY;
					for(i = 0; i < 19; i++)
					{
						faceVectorX.push_back(m_dpLandMarks[70 + i].x);
						faceVectorY.push_back(m_dpLandMarks[70 + i].y);
					}
					ConncetContour(faceVectorX, faceVectorY, m_TargetImg.GetImage(), CV_RGB(0,0,0), 1, true);
					ConncetContour(m_skinVector, m_TargetImg.GetImage(), CV_RGB(0,0,0), 1, true);

					 m_nStatus = skin;
					 m_bSkin= true;
					 m_btHair.EnableWindow(TRUE);
					 
					 break;
				 }
				 else if(m_nGCStatus == 3)
				 {
					vectorXCopy = hairVectorX;
					vectorYCopy = hairVectorY;
					hairVectorX.clear();
					hairVectorY.clear();

					 for( int i = 0; i < count; i++ )
					 {
						 CV_READ_SEQ_ELEM( pt1, reader );
						 hairVectorX.push_back(pt1.x);
						 hairVectorY.push_back(pt1.y);
					 }

					int size = hairVectorX.size() + 1;
					CShape sourShape, destShape;
					sourShape.Resize(size);
					for(i = 0; i < size; i++)
					{
						if(i < size - 1)
						{
							sourShape.PointX(i) = hairVectorX[i];
							sourShape.PointY(i) = hairVectorY[i];
						}
						else
						{
							sourShape.PointX(i) = hairVectorX[0];
							sourShape.PointY(i) = hairVectorY[0];
						}
					
					}
					ResampleCurve(sourShape, destShape, 20);
					hairVectorX.clear();
					hairVectorY.clear();
					dPoint temPoint;
					for(i = 0; i < destShape.PointNum(); i++)
					{
						temPoint.x = destShape.PointX(i);
						temPoint.y = destShape.PointY(i);
						if(temPoint.x < 0)
							temPoint.x = 0;
						if(temPoint.x > m_image.Width() - 1)
							temPoint.x = m_image.Width() - 1;
						if(temPoint.y < 0)
							temPoint.y = 0;
						if(temPoint.y > m_image.Height() - 1)
							temPoint.y = m_image.Height() - 1;

						hairVectorX.push_back(temPoint.x);
						hairVectorY.push_back(temPoint.y);
					}

					int nNum = hairVectorX.size();
					CPoint *points = new CPoint[nNum];
					for(int k = 0; k < nNum; k++)
					{
						points[k].x = hairVectorX[k];
						points[k].y = hairVectorY[k];
					}

					hairRgn.DeleteObject();
					hairRgn.CreatePolygonRgn(points, nNum, WINDING);
					hairRgn.GetRgnBox(hairRect);
					delete [] points;

					ConncetContour(vectorXCopy, vectorYCopy, m_TargetImg.GetImage(), CV_RGB(255, 255, 255), 2);
					ConncetContour(hairVectorX, hairVectorY, m_TargetImg.GetImage(), CV_RGB(0, 0, 0), 1);
					
					m_nStatus = hair;
					m_bHair= true;
					m_btCollar.EnableWindow(TRUE);
					 
					break;	
				 }
				 else if(m_nGCStatus == 4)
				 {
					vectorXCopy = clothVectorX;
					vectorYCopy = clothVectorY;
					clothVectorX.clear();
					clothVectorY.clear();
					
					 for( int i = 0; i < count; i++ )
					 {
						 CV_READ_SEQ_ELEM( pt1, reader );
						 clothVectorX.push_back(pt1.x);
						 clothVectorY.push_back(pt1.y);
					 }

					vector<double> tempVectorX1, tempVectorX2;
					vector<double> tempVectorY1, tempVectorY2;
					bool isOne = true;
					for(i = 0; i < clothVectorY.size(); i++)
					{
						if(clothVectorY[i] < m_image.Height() - 10)
						{
							if(isOne == true)
							{	
								tempVectorX1.push_back(clothVectorX[i]);
								tempVectorY1.push_back(clothVectorY[i]);
							}
							else
							{
								tempVectorX2.push_back(clothVectorX[i]);
								tempVectorY2.push_back(clothVectorY[i]);
							}
						}
						else
						{
							isOne = false;
						}
					}
				
					int size1 = tempVectorX1.size();
					int size2 = tempVectorX2.size();
					int size = size1 + size2;
					CShape sourShape, destShape;
					sourShape.Resize(size);
					for(i = 0; i < size; i++)
					{
						if(i < size2)
						{
							sourShape.PointX(i) = tempVectorX2[i];
							sourShape.PointY(i) = tempVectorY2[i];	
						}
						else
						{
							sourShape.PointX(i) = tempVectorX1[i - size2];
							sourShape.PointY(i) = tempVectorY1[i - size2];	
						}
					}
					ResampleCurve(sourShape, destShape, 20);
					clothVectorX.clear();
					clothVectorY.clear();
					dPoint temPoint;
					for(i = 0; i < destShape.PointNum(); i++)
					{
						temPoint.x = destShape.PointX(i);
						temPoint.y = destShape.PointY(i);
						if(temPoint.x < 0)
							temPoint.x = 0;
						if(temPoint.x > m_image.Width() - 1)
							temPoint.x = m_image.Width() - 1;
						if(temPoint.y < 0)
							temPoint.y = 0;
						if(temPoint.y > m_image.Height() - 1)
							temPoint.y = m_image.Height() - 1;

						clothVectorX.push_back(temPoint.x);
						clothVectorY.push_back(temPoint.y);
					}
					if(clothVectorX[destShape.PointNum() - 1] > m_image.Width()/2)
					{
						clothVectorX.push_back(m_image.Width() - 1);
						clothVectorY.push_back(m_image.Height() - 1);
						clothVectorX.push_back(0);
						clothVectorY.push_back(m_image.Height() - 1);
					}
					else
					{
						clothVectorX.push_back(0);
						clothVectorY.push_back(m_image.Height() - 1);
						clothVectorX.push_back(m_image.Width() - 1);
						clothVectorY.push_back(m_image.Height() - 1);
					}

					int nNum = clothVectorX.size();
					CPoint *points = new CPoint[nNum];
					for(int k = 0; k < nNum; k++)
					{
						points[k].x = clothVectorX[k];
						points[k].y = clothVectorY[k];
					}

					clothRgn.DeleteObject();
					clothRgn.CreatePolygonRgn(points, nNum, WINDING);
					clothRgn.GetRgnBox(clothRect);
					delete [] points;

					ConncetContour(vectorXCopy, vectorYCopy, m_TargetImg.GetImage(), CV_RGB(255, 255, 255), 2);
					ConncetContour(clothVectorX, clothVectorY, m_TargetImg.GetImage(), CV_RGB(0,0,0), 1);

					m_TargetImgCopy.Create(m_TargetImg.Width(), m_TargetImg.Height(), 24);
					cvCopy(m_TargetImg.GetImage(), m_TargetImgCopy.GetImage());

					m_nStatus = collar;
					m_bCollar= true;
					m_btDefaultColor.EnableWindow(TRUE);
					m_btRender.EnableWindow(TRUE);
				 }
			 }	
		}

		Invalidate(FALSE);	
	}		
}

void CPortraitEditingDlg::OnConstrainAsm() 
{
	// TODO: Add your control notification handler code here
	if(m_bFeatureLocalized)
	{
		::SetCursor(::LoadCursor(NULL,IDC_WAIT));

		ConnectLandmark91(CV_RGB(255,255,255), 2);
	
		ConstrainASMSearch();
		ModifyLandmarks();
		
		ConnectLandmark91(CV_RGB(0,0,0), 1);
		DrawEyeball();

		m_TargetImgCopy.Create(m_TargetImg.Width(), m_TargetImg.Height(), 24);
		cvCopy(m_TargetImg.GetImage(), m_TargetImgCopy.GetImage());

		Invalidate(FALSE);

		::SetCursor(::LoadCursor(NULL,IDC_ARROW));
	}
	cvSaveImage("c:\\OnConstrainAsm.jpg",m_TargetImg.GetImage());
}

void CPortraitEditingDlg::OnRedo() 
{
	// TODO: Add your control notification handler code here
	if(m_nRecordIndex < m_nRecordMaxIndex)
	{
		m_nRecordIndex++;
		CString tempName;
		tempName.Format("\\temp\\portrait%d.jpg", m_nRecordIndex);
		m_TargetImg.Load(m_sPath + tempName);

		Invalidate(FALSE);
	}

	 int i, j;
	 vector<double> targetVectorX;
	 vector<double> targetVectorY;
	 targetVectorX.clear();
	 targetVectorY.clear();
	
	 CvvImage tempImg, ttImg;
	 tempImg.Load("c:\\hair96.jpg");
	 LoadTemplate("c:\\hair96.pts", targetVectorX, targetVectorY);

	 vector<double> sourceVectorX;
	 vector<double> sourceVectorY;

	 CSCParam param(type0);
	 param.nIter = 3;

	 CDLL_ShapeContext matchShapeContext;
	 sourceVectorX.clear();
	 sourceVectorY.clear();

	 ttImg.Load("C:\\hair\\style5\\hair2.jpg");
	 LoadTemplate("C:\\hair\\style5\\hair2.pts", sourceVectorX, sourceVectorY);

	 matchShapeContext.Initialization(sourceVectorX, sourceVectorY, targetVectorX, targetVectorY, param);
	 matchShapeContext.DoMatching();
	 vector< vector<CPoint> > pointVVector;
	 LoadTemplate("C:\\hair\\style5\\keyPoints2.pts", pointVVector);
	 double tx1,ty1,tx2,ty2;

	CImg<unsigned char> BImg(ttImg.Width(), ttImg.Height(), 1, 3, 255);
	CImg<unsigned char> BBImg(tempImg.Width(), tempImg.Height(), 1, 3, 255);
	CImg<unsigned char> RetImg(tempImg.Width(), tempImg.Height(), 1, 3, 255);
	CImg<unsigned char> flag(tempImg.Width(), tempImg.Height(), 1, 1, 0);
	CImg<unsigned char> FlagImg(tempImg.Width(), tempImg.Height(), 1, 1, 0);
	CImg<unsigned char> AlphaImg;		
	unsigned char black[3] = {10,10,10};
	 for(i = 0; i < pointVVector.size(); i++)
	 {
		 vector<DoublePoint> MidPoints;
		 for(j = 0; j < pointVVector[i].size() - 1; j++)
		 { 
			 matchShapeContext.GetTPS()->TPSTransform(pointVVector[i][j].x, pointVVector[i][j].y, tx1, ty1);
			 matchShapeContext.GetTPS()->TPSTransform(pointVVector[i][j + 1].x, pointVVector[i][j + 1].y, tx2, ty2);
	
			 CPoint Zpoint1, Zpoint2;
			 Zpoint1.x = (int)(tx1 + 0.5);
			 Zpoint1.y = (int)(ty1 + 0.5);
			 Zpoint2.x = (int)(tx2 + 0.5);
			 Zpoint2.y = (int)(ty2 + 0.5);

			 int p1x = pointVVector[i][j].x;
			 int p1y = pointVVector[i][j].y;
			 int p2x = pointVVector[i][j+1].x;
			 int p2y = pointVVector[i][j+1].y;

			 BImg.draw_line(p1x, p1y, p2x, p2y, black);
			 BImg.draw_circle(p1x, p1y, 1.2, black);
			 BImg.draw_circle(p2x, p2y, 1.2, black);
			 RetImg.draw_line(Zpoint1.x, Zpoint1.y, Zpoint2.x, Zpoint2.y, black);
			 RetImg.draw_circle(Zpoint1.x, Zpoint1.y, 1.2, black);
			 RetImg.draw_circle(Zpoint2.x, Zpoint2.y, 1.2, black);

			DoublePoint dp;
			dp.x = Zpoint1.x;
			dp.y = Zpoint1.y;
			if(pointVVector[i].size() < 5)
			{
				MidPoints.push_back(dp);
				AlphaImg.load("c:\\brush\\-1.bmp");
			}
			else if(pointVVector[i].size() < 15)
			{
				if(j % 2 == 0 && j != pointVVector[i].size() - 2)
				{
					MidPoints.push_back(dp);
					AlphaImg.load("c:\\brush\\0.bmp");
				}
			}
			else if(pointVVector[i].size() < 30)
			{
				if(j % 2 == 0 && j != pointVVector[i].size() - 2)
				{
					MidPoints.push_back(dp);
					AlphaImg.load("c:\\brush\\00.bmp");
				}
			}
			else
			{
				if(j % 2 == 0 && j != pointVVector[i].size() - 2)
				{
					MidPoints.push_back(dp);
					AlphaImg.load("c:\\brush\\000.bmp");
				}
			}

			if(j == pointVVector[i].size() - 2)
			{
				dp.x = Zpoint2.x;
				dp.y = Zpoint2.y;
				MidPoints.push_back(dp);
			}
		 }

			CCurveMorphing CMorphing;
			CMorphing.MorphingToCurve(MidPoints, 20, 10, 10, AlphaImg, 2, BBImg, flag, FlagImg);
			MidPoints.clear();
	 }

	 RetImg.save("c:\\tpsHair.bmp");
	 BBImg.save("c:\\brushHair.bmp");
	 BImg.save("c:\\oriHair.bmp");
}

void CPortraitEditingDlg::OnUndo() 
{
	// TODO: Add your control notification handler code here
	if(m_nRecordIndex > 0)
	{
		m_nRecordIndex--;
		CString tempName;
		tempName.Format("\\temp\\portrait%d.jpg", m_nRecordIndex);
		m_TargetImg.Load(m_sPath + tempName);

		Invalidate(FALSE);
	}
}

void CPortraitEditingDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_pModel != NULL)
	{
		delete m_pModel;
		m_pModel = NULL;
	}

	if (Gcs != NULL)
	{
		delete Gcs;
		Gcs = NULL;
	}
}

void CPortraitEditingDlg::SelectContourPoint(vector<double> &DenseX, vector<double> &DenseY, vector<double> &SparseX, vector<double> &SparseY, int pointCount)
{
	int size = DenseX.size();
	double step = size * 1.0 / (pointCount - 1);
	SparseX.clear();
	SparseY.clear();
	SparseX.resize(pointCount);
	SparseY.resize(pointCount);
	for(int i = 0; i < pointCount; i++)
	{
		int t = (int) (i * step);
		SparseX[i] = DenseX[t];
		SparseY[i] = DenseY[t];
	}
	
	SparseX[pointCount - 1] = DenseX.back();
	SparseY[pointCount - 1] = DenseY.back();
}

void CPortraitEditingDlg::SaveXML(CString sFileName)
{
	CXmlFile XMLOperate;
	XMLOperate.m_sSrcImgFilename = m_sPath + "\\temp" + "\\input.bmp";

	vector<CPoint> cPointVector;
	for(int i = 0; i < 105; i++)
	{
		cPointVector.push_back(m_ptArrLandMarks[i]);
	}
	XMLOperate.m_vPnt105 = cPointVector;

	vector<dPoint> dPointVector;
	for(i = 0; i < 91; i++)
	{
		dPointVector.push_back(m_dpLandMarks[i]);
	}
	XMLOperate.m_vPnt91 = dPointVector;

	XMLOperate.m_vPntBgContour = m_backgroundVector;

	XMLOperate.m_vPntSkinContour = m_skinVector;

	XMLOperate.m_vPntLEarContour = leftEarVector;

	XMLOperate.m_vPntREarContour = rightEarVector;

	dPoint temPoint;
	dPointVector.clear();
	for(i = 0; i < (int)hairVectorX.size(); i++)
	{
		temPoint.x = hairVectorX[i];
		temPoint.y = hairVectorY[i];
		dPointVector.push_back(temPoint);
	}
	XMLOperate.m_vPntHairContour = dPointVector;

	dPointVector.clear();
	for(i = 0; i < (int)clothVectorX.size(); i++)
	{
		temPoint.x = clothVectorX[i];
		temPoint.y = clothVectorY[i];
		dPointVector.push_back(temPoint);
	}
	XMLOperate.m_vPntClothContour = dPointVector;

	if(m_bBackground == true)
		XMLOperate.m_nBackground = 1;
	else
		XMLOperate.m_nBackground = 0;

	if(m_bSkin == true)
		XMLOperate.m_nSkin = 1;
	else
		XMLOperate.m_nSkin = 0;

	if(m_bHair = true)
		XMLOperate.m_nHair = 1;
	else
		XMLOperate.m_nHair = 0;

	if(m_bCollar == true)
		XMLOperate.m_nCloth = 1;
	else
		XMLOperate.m_nCloth = 0;

	if(m_bEar == true)
		XMLOperate.m_nEar = 1;
	else
		XMLOperate.m_nEar = 0;


	XMLOperate.m_nHairIndex = hairIndex[0];

	XMLOperate.m_nClothIndex = clothIndex[0];

	for(i = 0; i < 8; i++)
	{
		XMLOperate.m_nCompIDs[i] = m_nCompSelected[i];
	}

	XMLOperate.SaveAsXml(sFileName);
}

void CPortraitEditingDlg::LoadXML(CString sFileName)
{
	CXmlFile XMLOperate;
	XMLOperate.m_sSrcImgFilename = m_sPath + "\\temp" + "\\input.bmp";
	XMLOperate.ReadXml(sFileName);

	for(int i = 0; i < 105; i++)
	{
		m_ptArrLandMarks[i].x = XMLOperate.m_vPnt105[i].x;
		m_ptArrLandMarks[i].y = XMLOperate.m_vPnt105[i].y;
	}

	for(i = 0; i < 91; i++)
	{
		m_dpLandMarks[i].x = XMLOperate.m_vPnt91[i].x;
		m_dpLandMarks[i].y = XMLOperate.m_vPnt91[i].y;
	}

	m_backgroundVector = XMLOperate.m_vPntBgContour; 
	int size = (int)m_backgroundVector.size();
	CPoint *pBackPoints = new CPoint[size];
	for(int k = 0; k < size; k++)
	{	
		pBackPoints[k].x = m_backgroundVector[k].x;
		pBackPoints[k].y = m_backgroundVector[k].y;
	}
	if(size > 0)
	{
		bgRgn.DeleteObject();
		bgRgn.CreatePolygonRgn(pBackPoints, size, WINDING);
		bgRgn.GetRgnBox(bgRect);
	}
	delete [] pBackPoints;

	m_skinVector = XMLOperate.m_vPntSkinContour;
	size = (int)m_skinVector.size();
	CPoint *pSkinPoints = new CPoint[size];
	for(k = 0; k < size; k++)
	{	
		pSkinPoints[k].x = m_skinVector[k].x;
		pSkinPoints[k].y = m_skinVector[k].y;
	}
	if(size > 0)
	{
		skinRgn.DeleteObject();
		skinRgn.CreatePolygonRgn(pSkinPoints, size, WINDING);
		skinRgn.GetRgnBox(skRect);
	}
	delete [] pSkinPoints;

	leftEarVector = XMLOperate.m_vPntLEarContour;
	size = (int)leftEarVector.size();
	CPoint *pLEarPoints = new CPoint[size];
	for(k = 0; k < size; k++)
	{	
		pLEarPoints[k].x = leftEarVector[k].x;
		pLEarPoints[k].y = leftEarVector[k].y;
	}
	if(size > 0)
	{
		leftEarRgn.DeleteObject();
		leftEarRgn.CreatePolygonRgn(pLEarPoints, size, WINDING);
		leftEarRgn.GetRgnBox(leftEarRect);
	}
	delete [] pLEarPoints;

	rightEarVector = XMLOperate.m_vPntREarContour;
	size = (int)rightEarVector.size();
	CPoint *pREarPoints = new CPoint[size];
	for(k = 0; k < size; k++)
	{	
		pREarPoints[k].x = rightEarVector[k].x;
		pREarPoints[k].y = rightEarVector[k].y;
	}
	if(size > 0)
	{
		rightEarRgn.DeleteObject();
		rightEarRgn.CreatePolygonRgn(pREarPoints, size, WINDING);
		rightEarRgn.GetRgnBox(rightEarRect);
	}
	delete [] pREarPoints;

    size = (int)XMLOperate.m_vPntHairContour.size();
	CPoint *pHairPoints = new CPoint[size];
	hairVectorX.resize(size);
	hairVectorY.resize(size);
	for(i = 0; i < size; i++)
	{
		hairVectorX[i] = XMLOperate.m_vPntHairContour[i].x;
		hairVectorY[i] = XMLOperate.m_vPntHairContour[i].y;
		pHairPoints[i].x = XMLOperate.m_vPntHairContour[i].x;
		pHairPoints[i].y = XMLOperate.m_vPntHairContour[i].y;
	}
	if(size > 0)
	{
		hairRgn.DeleteObject();
		hairRgn.CreatePolygonRgn(pHairPoints, size, WINDING);
		hairRgn.GetRgnBox(hairRect);
	}
	delete [] pHairPoints;
	
	size = (int)XMLOperate.m_vPntClothContour.size();
	CPoint *pClothPoints = new CPoint[size];
	clothVectorX.resize(size);
	clothVectorY.resize(size);
	for(i = 0; i < size; i++)
	{
		clothVectorX[i] = XMLOperate.m_vPntClothContour[i].x;
		clothVectorY[i] = XMLOperate.m_vPntClothContour[i].y;
		pClothPoints[i].x = XMLOperate.m_vPntClothContour[i].x;
		pClothPoints[i].y = XMLOperate.m_vPntClothContour[i].y;
	}
	if(size > 0)
	{
		clothRgn.DeleteObject();
		clothRgn.CreatePolygonRgn(pClothPoints, size, WINDING);
		clothRgn.GetRgnBox(clothRect);
	}
	delete [] pClothPoints;

	m_bFeatureLocalized = true;
	m_bFeature = true;
	if(XMLOperate.m_nBackground == 1)
		m_bBackground = true;
	else
		m_bBackground = false;

	if(XMLOperate.m_nSkin == 1)
		m_bSkin = true;
	else
		m_bSkin = false;

	if(XMLOperate.m_nHair == 1)
		m_bHair = true;
	else
		m_bHair = false;

	if(XMLOperate.m_nCloth == 1)
		m_bCollar = true;
	else
		m_bCollar = false;

	if(XMLOperate.m_nEar == 1)
		m_bEar = true;
	else
		m_bEar = false;

	hairIndex[0] = XMLOperate.m_nHairIndex;

	clothIndex[0] = XMLOperate.m_nClothIndex;

	for(i = 0; i < 8; i++)
	{
		m_nCompSelected[i] = XMLOperate.m_nCompIDs[i];
	}

}


void CPortraitEditingDlg::SobelOperator(IplImage* grayImage, double *pGradX, double *pGradY, double *pMod)
{
	int nWidth = grayImage->width;
	int nHeight = grayImage->height;
	BYTE *buf = (BYTE *) grayImage->imageData;
	int WidthPix = WIDTHBYTES(nWidth * 8);

	// 遍历图象的纵坐标
	int row;

	// 遍历图象的横坐标
	int col;

	for (row = 0; row < nHeight; row++) 
	{
		for (col = 0; col < nWidth; col++) 
			// 初始化
		{
			*(pGradX + row * nWidth + col) = 0;
			*(pGradY + row * nWidth + col) = 0;
			*(pMod + row * nWidth+ col) = 0;
		}
	}

	// 设置模板系数
	static int nWeightX[3][3], nWeightY[3][3];
	nWeightX[0][0] = -1 ;   
	nWeightX[0][1] = -2 ;   
	nWeightX[0][2] = -1 ;   
	nWeightX[1][0] = 0 ;   
	nWeightX[1][1] = 0;   
	nWeightX[1][2] = 0 ;   
	nWeightX[2][0] = 1 ;   
	nWeightX[2][1] = 2 ;   
	nWeightX[2][2] = 1 ;   

	nWeightY[0][0] = 1 ;   
	nWeightY[0][1] = 0 ;   
	nWeightY[0][2] = -1 ;   
	nWeightY[1][0] = 2 ;   
	nWeightY[1][1] = 0;   
	nWeightY[1][2] = -2 ;   
	nWeightY[2][0] = 1 ;   
	nWeightY[2][1] = 0 ;   
	nWeightY[2][2] = -1 ;  

	//这个变量用来表示Sobel算子象素值
	int nTmp[3][3];

	// 临时变量
	double dGradX, dGradY;

	// 模板循环控制变量
	int yy ;
	int xx ;


	// 下面开始利用Laplacian算子进行计算，为了保证计算所需要的
	// 的数据位于图像数据的内部，下面的两重循环的条件是
	// row < nHeight - 2 而不是row < nHeight，相应的x方向也是
	//col < nWidth - 2, 而不是col < nWidth
	for(row = 1; row < nHeight - 2 ; row++ )
	{
		for(col = 1 ; col < nWidth - 2; col++ )
		{
			dGradX = 0;
			dGradY = 0;

			// Sobel算子需要的各点象素值

			// 模板第一行
			nTmp[0][0] = *(buf + (row - 1) * WidthPix + (col - 1)); 
			nTmp[0][1] = *(buf + (row - 1) * WidthPix + col); 
			nTmp[0][2] = *(buf + (row - 1) * WidthPix + (col + 1)); 

			// 模板第二行
			nTmp[1][0] = *(buf + row * WidthPix + (col - 1)); 
			nTmp[1][1] = *(buf + row * WidthPix + col); 
			nTmp[1][2] = *(buf + row * WidthPix + (col + 1)); 

			// 模板第三行
			nTmp[2][0] = *(buf + (row + 1) * WidthPix + (col - 1)); 
			nTmp[2][1] = *(buf + (row + 1) * WidthPix + col); 
			nTmp[2][2] = *(buf + (row + 1) * WidthPix + (col + 1)); 

			// 计算梯度
			for(yy = 0; yy < 3; yy++)
			{
				for(xx = 0; xx < 3; xx++)
				{
					dGradX += nTmp[yy][xx] * nWeightX[yy][xx];
					dGradY += nTmp[yy][xx] * nWeightY[yy][xx];
				}
			}

			// 梯度值写入内存
			*(pGradX + row * nWidth + col) = dGradX;
			*(pGradY + row * nWidth + col) = dGradY;
			*(pMod + row * nWidth + col) = pow(dGradX * dGradX + dGradY * dGradY, 0.5);
		}
	}
}


void CPortraitEditingDlg::CropImage(CvvImage &comImage, vector<dPoint> &inPoints, vector<dPoint> &outPoints)
{
	int num = (int) inPoints.size();
	CRect comRect(10000, 10000, 0, 0);
	for(int i = 0; i < num; i++)
	{
		if(comRect.left > inPoints[i].x)
			comRect.left = inPoints[i].x; 
		if(comRect.right < inPoints[i].x)
			comRect.right = inPoints[i].x; 
		if(comRect.top > inPoints[i].y)
			comRect.top = inPoints[i].y;
		if(comRect.bottom < inPoints[i].y)
			comRect.bottom = inPoints[i].y;
	}

	BYTE *buf1 = (BYTE *)m_image.GetImage()->imageData;
	int WidthPix1 = WIDTHBYTES(m_image.Width() * 24);
	comImage.Create(comRect.Width() + 20, comRect.Height() + 20, 24);
	BYTE *buf2 = (BYTE *)comImage.GetImage()->imageData;
	int WidthPix2 = WIDTHBYTES(comImage.Width() * 24);

	for(int y = 0; y < comImage.Height(); y++)
	{
		for(int x = 0; x < comImage.Width(); x++)
		{
			buf2[y * WidthPix2 + 3 * x] = buf1[(y + comRect.top - 10) * WidthPix1 + 3 * (x + comRect.left - 10)];
			buf2[y * WidthPix2 + 3 * x + 1] = buf1[(y + comRect.top - 10) * WidthPix1 + 3 * (x + comRect.left - 10) + 1];
			buf2[y * WidthPix2 + 3 * x + 2] = buf1[(y + comRect.top - 10) * WidthPix1 + 3 * (x + comRect.left - 10) + 2];
		}
	}

	dPoint tp;
	for(i = 0; i < num; i++)
	{
		tp.x = inPoints[i].x - (comRect.left - 10);
		tp.y = inPoints[i].y - (comRect.top - 10);
		outPoints.push_back(tp);
	}
}

void CPortraitEditingDlg::AlignEyeImage(CvvImage &comImage, CvvImage &stdImage, vector<dPoint> &comPoints, vector<dPoint> &stdPoints, int stdWidth, int stdHeight)
{
	CvvImage nImage;
	double ratio = (stdWidth * 1.0) / (comImage.Width() * 1.0); 
	int nWidth = (int) (comImage.Width() * ratio);
	int nHeight = (int) (comImage.Height() * ratio);
	nImage.Destroy();
	nImage.Create(nWidth, nHeight, 24);
	cvResize(comImage.GetImage(), nImage.GetImage());
	CvvImage grayImg;
	grayImg.Create(nImage.Width(), nImage.Height(), 8);
	cvCvtColor(nImage.GetImage(), grayImg.GetImage(), CV_BGR2GRAY);

	int num = (int)comPoints.size();
	for(int i = 0; i < num; i++)
	{
		comPoints[i].x = comPoints[i].x * ratio;
		comPoints[i].y = comPoints[i].y * ratio;
	}

	vector<double> targetX, targetY, sourceX, sourceY;
	for(i = 0; i < num; i++)
	{
		targetX.push_back(stdPoints[i].x);
		targetY.push_back(stdPoints[i].y);
		sourceX.push_back(comPoints[i].x);
		sourceY.push_back(comPoints[i].y);
	}
	CMyTPS myTPS;
	myTPS.Create(targetX, targetY, sourceX, sourceY, 0);

	CvvImage normalImg;
	normalImg.Create(stdWidth, stdHeight, 8);
	BYTE *buf1 = (BYTE *)normalImg.GetImage()->imageData;
	int WidthPix1 = WIDTHBYTES(normalImg.Width() * 8);
	BYTE *buf2 = (BYTE *)grayImg.GetImage()->imageData;
	int WidthPix2 = WIDTHBYTES(grayImg.Width() * 8);

	double dx, dy;
	for(int y = 0; y < stdHeight; y++)
	{
		for(int x = 0; x < stdWidth; x++)
		{
			myTPS.TPSTransform(x, y, dx, dy);
			int nx = (int)(dx + 0.5);
			int ny = (int)(dy + 0.5);

			if(nx < 0)
				nx = 0;
			if(nx > grayImg.Width() - 1)
				nx = grayImg.Width() -1;
			if(ny < 0)
				ny = 0;
			if(ny > grayImg.Height() - 1)
				ny = grayImg.Height() - 1;

			if(nx >= 0 && nx < grayImg.Width() && ny >= 0 && ny < grayImg.Height())
			{
				buf1[y * WidthPix1 + x] = buf2[ny * WidthPix2 + nx];
			}		
		}
	}

	cvSmooth(normalImg.GetImage(), normalImg.GetImage(), CV_GAUSSIAN, 3, 3, 1 );

	CRgn  tempRgn;
	int nPoint = targetX.size();
	CPoint *temPoints = new CPoint[nPoint];
	for(int j = 0; j < nPoint; j++)
	{
		temPoints[j].x = (LONG)targetX[j];
		temPoints[j].y = (LONG)targetY[j];
	}
	temPoints[0].x += 5;
	temPoints[1].y += 5;
	temPoints[2].y += 5;
	temPoints[3].y += 5;
	temPoints[4].x -= 5;
	tempRgn.DeleteObject();
	tempRgn.CreatePolygonRgn(temPoints, 8, WINDING);
	delete [] temPoints;

	double *pGradX = new double[stdHeight * stdWidth];
	double *pGradY = new double[stdHeight * stdWidth];
	double *pMod = new double[stdHeight * stdWidth];
	SobelOperator(normalImg.GetImage(), pGradX, pGradY, pMod);

	stdImage.Create(stdWidth, stdHeight, 8);
	BYTE *bufG = (BYTE *)stdImage.GetImage()->imageData;
	int WidthPixG = WIDTHBYTES(stdImage.Width() * 8);
	for(y = 0; y < stdHeight; y++)
	{
		for(int x = 0; x < stdWidth; x++)
		{
			if(pMod[y * stdWidth + x] > 255)
				bufG[y * WidthPixG + x] = 255;
			else if(pMod[y * stdWidth + x] < 40)
				bufG[y * WidthPixG + x] = 0;
			else
				bufG[y * WidthPixG + x] = pMod[y * stdWidth + x];

			if(tempRgn.PtInRegion(CPoint(x,y)))
				bufG[y * WidthPixG + x] = 0;
		}
	}

	delete [] pGradX;
	delete [] pGradY;
	delete [] pMod;
}

void CPortraitEditingDlg::AlignNoseImage(CvvImage &comImage, CvvImage &stdImage, vector<dPoint> &comPoints, vector<dPoint> &stdPoints, int stdWidth, int stdHeight)
{
	CvvImage nImage;
	double ratio = (stdWidth * 1.0) / (comImage.Width() * 1.0); 
	int nWidth = (int) (comImage.Width() * ratio);
	int nHeight = (int) (comImage.Height() * ratio);
	nImage.Destroy();
	nImage.Create(nWidth, nHeight, 24);
	cvResize(comImage.GetImage(), nImage.GetImage());
	CvvImage grayImg;
	grayImg.Create(nImage.Width(), nImage.Height(), 8);
	cvCvtColor(nImage.GetImage(), grayImg.GetImage(), CV_BGR2GRAY);

	int num = (int)comPoints.size();
	for(int i = 0; i < num; i++)
	{
		comPoints[i].x = comPoints[i].x * ratio;
		comPoints[i].y = comPoints[i].y * ratio;
	}

	vector<double> targetX, targetY, sourceX, sourceY;
	for(i = 0; i < num; i++)
	{
		targetX.push_back(stdPoints[i].x);
		targetY.push_back(stdPoints[i].y);
		sourceX.push_back(comPoints[i].x);
		sourceY.push_back(comPoints[i].y);
	}
	CMyTPS myTPS;
	myTPS.Create(targetX, targetY, sourceX, sourceY, 0);

	CvvImage normalImg;
	normalImg.Create(stdWidth, stdHeight, 8);
	BYTE *buf1 = (BYTE *)normalImg.GetImage()->imageData;
	int WidthPix1 = WIDTHBYTES(normalImg.Width() * 8);
	BYTE *buf2 = (BYTE *)grayImg.GetImage()->imageData;
	int WidthPix2 = WIDTHBYTES(grayImg.Width() * 8);

	int iSum = 0;
	double dx, dy;
	for(int y = 0; y < stdHeight; y++)
	{
		for(int x = 0; x < stdWidth; x++)
		{
			myTPS.TPSTransform(x, y, dx, dy);
			int nx = (int)(dx + 0.5);
			int ny = (int)(dy + 0.5);

			if(nx < 0)
				nx = 0;
			if(nx > grayImg.Width() - 1)
				nx = grayImg.Width() -1;
			if(ny < 0)
				ny = 0;
			if(ny > grayImg.Height() - 1)
				ny = grayImg.Height() - 1;

			if(nx >= 0 && nx < grayImg.Width() && ny >= 0 && ny < grayImg.Height())
			{
				buf1[y * WidthPix1 + x] = buf2[ny * WidthPix2 + nx];
				
				iSum += buf1[y * WidthPix1 + x];
			}		
		}
	}

	CRgn  tempRgn1, tempRgn2;
	CPoint temPoints1[5], temPoints2[5];
	temPoints1[0].x = 0;
	temPoints1[0].y = 0;
	temPoints1[1].x = (LONG)targetX[0] - 2;
	temPoints1[1].y = 0;
	temPoints1[2].x = (LONG)targetX[1] - 2;
	temPoints1[2].y = (LONG)targetY[1];
	temPoints1[3].x = (LONG)targetX[2] - 2;
	temPoints1[3].y = (LONG)targetY[2];
	temPoints1[4].x = 0;
	temPoints1[4].y = (LONG)targetY[2];
	temPoints2[0].x = stdWidth;
	temPoints2[0].y = 0;
	temPoints2[1].x = (LONG)targetX[12] + 2;
	temPoints2[1].y = 0;
	temPoints2[2].x = (LONG)targetX[11] + 2;
	temPoints2[2].y = (LONG)targetY[11];
	temPoints2[3].x = (LONG)targetX[10] + 2;
	temPoints2[3].y = (LONG)targetY[10];
	temPoints2[4].x = stdWidth;
	temPoints2[4].y = (LONG)targetY[10];
	tempRgn1.DeleteObject();
	tempRgn1.CreatePolygonRgn(temPoints1, 5, WINDING);
	tempRgn2.DeleteObject();
	tempRgn2.CreatePolygonRgn(temPoints2, 5, WINDING);

	int graymean = iSum/(stdHeight * stdWidth);
	for(y = 0; y < stdHeight; y++)
	{
		for(int x = 0; x < stdWidth; x++)
		{
			if(tempRgn1.PtInRegion(CPoint(x, y)) || tempRgn2.PtInRegion(CPoint(x, y)))
			{
				buf1[y * WidthPix1 + x] = graymean;
			}
		}
	}
		
	cvSmooth(normalImg.GetImage(), normalImg.GetImage(), CV_GAUSSIAN, 3, 3, 1 );

	double *pGradX = new double[stdHeight * stdWidth];
	double *pGradY = new double[stdHeight * stdWidth];
	double *pMod = new double[stdHeight * stdWidth];
	SobelOperator(normalImg.GetImage(), pGradX, pGradY, pMod);

	stdImage.Create(stdWidth, stdHeight, 8);
	BYTE *bufG = (BYTE *)stdImage.GetImage()->imageData;
	int WidthPixG = WIDTHBYTES(stdImage.Width() * 8);
	for(y = 0; y < stdHeight; y++)
	{
		for(int x = 0; x < stdWidth; x++)
		{
			if(pMod[y * stdWidth + x] > 255)
				bufG[y * WidthPixG + x] = 255;
			else
				bufG[y * WidthPixG + x] = pMod[y * stdWidth + x];
		}
	}

	delete [] pGradX;
	delete [] pGradY;
	delete [] pMod;
}

void CPortraitEditingDlg::AlignImage(CvvImage &comImage, CvvImage &stdImage, vector<dPoint> &comPoints, vector<dPoint> &stdPoints, int stdWidth, int stdHeight)
{
	CvvImage nImage;
	double ratio = (stdWidth * 1.0) / (comImage.Width() * 1.0); 
	int nWidth = (int) (comImage.Width() * ratio);
	int nHeight = (int) (comImage.Height() * ratio);
	nImage.Destroy();
	nImage.Create(nWidth, nHeight, 24);
	cvResize(comImage.GetImage(), nImage.GetImage());
	CvvImage grayImg;
	grayImg.Create(nImage.Width(), nImage.Height(), 8);
	cvCvtColor(nImage.GetImage(), grayImg.GetImage(), CV_BGR2GRAY);

	int num = (int)comPoints.size();
	for(int i = 0; i < num; i++)
	{
		comPoints[i].x = comPoints[i].x * ratio;
		comPoints[i].y = comPoints[i].y * ratio;
	}

	vector<double> targetX, targetY, sourceX, sourceY;
	for(i = 0; i < num; i++)
	{
		targetX.push_back(stdPoints[i].x);
		targetY.push_back(stdPoints[i].y);
		sourceX.push_back(comPoints[i].x);
		sourceY.push_back(comPoints[i].y);
	}
	CMyTPS myTPS;
	myTPS.Create(targetX, targetY, sourceX, sourceY, 0);

	CvvImage normalImg;
	normalImg.Create(stdWidth, stdHeight, 8);
	BYTE *buf1 = (BYTE *)normalImg.GetImage()->imageData;
	int WidthPix1 = WIDTHBYTES(normalImg.Width() * 8);
	BYTE *buf2 = (BYTE *)grayImg.GetImage()->imageData;
	int WidthPix2 = WIDTHBYTES(grayImg.Width() * 8);

	double dx, dy;
	for(int y = 0; y < stdHeight; y++)
	{
		for(int x = 0; x < stdWidth; x++)
		{
			myTPS.TPSTransform(x, y, dx, dy);
			int nx = (int)(dx + 0.5);
			int ny = (int)(dy + 0.5);

			if(nx < 0)
				nx = 0;
			if(nx > grayImg.Width() - 1)
				nx = grayImg.Width() -1;
			if(ny < 0)
				ny = 0;
			if(ny > grayImg.Height() - 1)
				ny = grayImg.Height() - 1;

			if(nx >= 0 && nx < grayImg.Width() && ny >= 0 && ny < grayImg.Height())
			{
				buf1[y * WidthPix1 + x] = buf2[ny * WidthPix2 + nx];
			}		
		}
	}
	
	cvSmooth(normalImg.GetImage(), normalImg.GetImage(), CV_GAUSSIAN, 3, 3, 1 );

	double *pGradX = new double[stdHeight * stdWidth];
	double *pGradY = new double[stdHeight * stdWidth];
	double *pMod = new double[stdHeight * stdWidth];
	SobelOperator(normalImg.GetImage(), pGradX, pGradY, pMod);

	stdImage.Create(stdWidth, stdHeight, 8);
	BYTE *bufG = (BYTE *)stdImage.GetImage()->imageData;
	int WidthPixG = WIDTHBYTES(stdImage.Width() * 8);
	for(y = 0; y < stdHeight; y++)
	{
		for(int x = 0; x < stdWidth; x++)
		{
			if(pMod[y * stdWidth + x] > 255)
				bufG[y * WidthPixG + x] = 255;
			else
				bufG[y * WidthPixG + x] = pMod[y * stdWidth + x];
		}
	}

	delete [] pGradX;
	delete [] pGradY;
	delete [] pMod;
}

void CPortraitEditingDlg::SelectEye(int *order)
{
	FILE *fp1 = fopen(m_sPath + "\\mode\\eye_fld.txt", "r");
	FILE *fp2 = fopen(m_sPath + "\\mode\\eye_mean.txt", "r");
	FILE *fp3 = fopen(m_sPath + "\\mode\\eye_weight.txt", "r");

	int i, j;
	double  FLD1[7300], FLD2[7300], MV[7300], IMG[7300];
	double  WT1[150], WT2[150], Dist[150]; 

	if(fp1 != NULL && fp2 != NULL)
	{
		double dx, dy, dn;
		for(i = 0; i < 7300; i++)
		{	
			fscanf(fp1, "%lf ", &dx);
			FLD1[i] = dx;
		}

		for(i = 0; i < 7300; i++)
		{
			fscanf(fp1, "%lf ", &dy);
			FLD2[i] = dy;
		}

		for(i = 0; i < 7300; i++)
		{
			fscanf(fp2, "%lf ", &dn);
			MV[i] = dn;
		}

		for(i = 0; i < 150; i++)
		{	
			fscanf(fp3, "%lf ", &dx);
			WT1[i] = dx;
		}

		for(i = 0; i < 150; i++)
		{
			fscanf(fp3, "%lf ", &dy);
			WT2[i] = dy;
		}
		
		fclose(fp1);
		fclose(fp2);
		fclose(fp3);
	}
	
	dPoint tp;
	vector<dPoint> eyePoints, outPoints;
	tp.x = m_ptArrLandMarks[49].x;
	tp.y = m_ptArrLandMarks[49].y;
	eyePoints.push_back(tp);
	for(i = 56; i > 49; i--)
	{
		tp.x = m_ptArrLandMarks[i].x;
		tp.y = m_ptArrLandMarks[i].y;
		eyePoints.push_back(tp);
	}

	CvvImage eyeImg;
	CropImage(eyeImg, eyePoints, outPoints);
	eyeImg.Save("e:\\ee.jpg");

	CvvImage stdImg;
	vector<dPoint> stdPoints;
	LoadTemplate(m_sPath + "\\mode\\stdEye.pts", stdPoints);
	AlignEyeImage(eyeImg, stdImg, outPoints, stdPoints, 100, 73);
	stdImg.Save("e:\\std.jpg");

	BYTE *buf = (BYTE *)stdImg.GetImage()->imageData;
	int WidthPix = WIDTHBYTES(stdImg.Width() * 8);
	for(i = 0; i < stdImg.Height(); i++)
	{
		for(j = 0; j < stdImg.Width(); j++)
		{
			IMG[i * stdImg.Width() + j] = buf[i * WidthPix + j];
		}
	}

	double sum1 = 0; 
	double sum2 = 0;
	for(i = 0; i < 7300; i++)
	{
		sum1 += (IMG[i] - MV[i]) * FLD1[i];
		sum2 += (IMG[i] - MV[i]) * FLD2[i];
	}

	for(i = 0; i < 150; i++)
	{
	  Dist[i] = GetDistance(sum1, sum2, WT1[i], WT2[i]);
	}

	double DistCopy[150];
	for(i = 0; i < 150; i++)
	{
		DistCopy[i] = Dist[i];
	}

	FILE *fp = fopen("E:\\dist.txt", "w");
	for(i = 0; i < 149; i++)
	{
		for(j = i + 1; j < 150; j++)
		{
			if(DistCopy[i] > DistCopy[j])
			{
				double temp = DistCopy[i];
				DistCopy[i] = DistCopy[j];
				DistCopy[j] = temp;
			}
		}

		fprintf(fp, "%lf ", DistCopy[i]);
	}

	fprintf(fp, "\n");
	for(i = 0; i < 15; i++)
	{
		for(j = 0; j < 150; j++)
		{
			if(DistCopy[i] == Dist[j])
			{
				order[i] = j;
				break;
			}
		}
        fprintf(fp, "%d\n", order[i]);
	}

	fclose(fp);
}

void CPortraitEditingDlg::SelectNose(int *order)
{
	FILE *fp1 = fopen(m_sPath + "\\mode\\nose_fld.txt", "r");
	FILE *fp2 = fopen(m_sPath + "\\mode\\nose_mean.txt", "r");
	FILE *fp3 = fopen(m_sPath + "\\mode\\nose_weight.txt", "r");

	int i, j;
	double  FLD1[2000], FLD2[2000], FLD3[2000], MV[2000], IMG[2000];
	double  WT1[200], WT2[200], WT3[200], Dist[200]; 

	if(fp1 != NULL && fp2 != NULL)
	{
		double dx, dy, dz, dn;
		for(i = 0; i < 2000; i++)
		{	
			fscanf(fp1, "%lf ", &dx);
			FLD1[i] = dx;
		}

		for(i = 0; i < 2000; i++)
		{
			fscanf(fp1, "%lf ", &dy);
			FLD2[i] = dy;
		}

		for(i = 0; i < 2000; i++)
		{
			fscanf(fp1, "%lf ", &dz);
			FLD3[i] = dz;
		}

		for(i = 0; i < 2000; i++)
		{
			fscanf(fp2, "%lf ", &dn);
			MV[i] = dn;
		}

		for(i = 0; i < 200; i++)
		{	
			fscanf(fp3, "%lf ", &dx);
			WT1[i] = dx;
		}

		for(i = 0; i < 200; i++)
		{
			fscanf(fp3, "%lf ", &dy);
			WT2[i] = dy;
		}

		for(i = 0; i < 200; i++)
		{
			fscanf(fp3, "%lf ", &dz);
			WT3[i] = dz;
		}
		
		fclose(fp1);
		fclose(fp2);
		fclose(fp3);
	}
	
	dPoint tp;
	vector<dPoint> nosePoints, outPoints;
	for(i = 0; i < 13; i++)
	{
		tp.x = m_ptArrLandMarks[61 + i].x;
		tp.y = m_ptArrLandMarks[61 + i].y;
		nosePoints.push_back(tp);
	}

	CvvImage noseImg;
	CropImage(noseImg, nosePoints, outPoints);
	noseImg.Save("e:\\ee.jpg");

	CvvImage stdImg;
	vector<dPoint> stdPoints;
	LoadTemplate(m_sPath + "\\mode\\stdNose.pts", stdPoints);
	AlignNoseImage(noseImg, stdImg, outPoints, stdPoints, 40, 50);
	stdImg.Save("e:\\std.jpg");

	BYTE *buf = (BYTE *)stdImg.GetImage()->imageData;
	int WidthPix = WIDTHBYTES(stdImg.Width() * 8);
	for(i = 0; i < stdImg.Height(); i++)
	{
		for(j = 0; j < stdImg.Width(); j++)
		{
			IMG[i * stdImg.Width() + j] = buf[i * WidthPix + j];
		}
	}

	double sum1 = 0; 
	double sum2 = 0;
	double sum3 = 0;
	for(i = 0; i < 2000; i++)
	{
		sum1 += (IMG[i] - MV[i]) * FLD1[i];
		sum2 += (IMG[i] - MV[i]) * FLD2[i];
		sum3 += (IMG[i] - MV[i]) * FLD3[i];
	}

	CString aa;
	aa.Format("%lf %lf %lf", sum1, sum2, sum3);
	MessageBox(aa);

	for(i = 0; i < 200; i++)
	{
	  Dist[i] = sqrt(Sqr(sum1 - WT1[i]) + Sqr(sum2 - WT2[i]) + Sqr(sum3 - WT3[i]));
	}

	double DistCopy[200];
	for(i = 0; i < 200; i++)
	{
		DistCopy[i] = Dist[i];
	}

	FILE *fp = fopen("E:\\dist.txt", "w");
	for(i = 0; i < 199; i++)
	{
		for(j = i + 1; j < 200; j++)
		{
			if(DistCopy[i] > DistCopy[j])
			{
				double temp = DistCopy[i];
				DistCopy[i] = DistCopy[j];
				DistCopy[j] = temp;
			}
		}

		fprintf(fp, "%lf ", DistCopy[i]);
	}

	fprintf(fp, "\n");
	for(i = 0; i < 15; i++)
	{
		for(j = 0; j < 200; j++)
		{
			if(DistCopy[i] == Dist[j])
			{
				order[i] = j;
				break;
			}
		}
        fprintf(fp, "%d\n", order[i]);
	}

	fclose(fp);
}

void CPortraitEditingDlg::SelectMouth(int *order)
{
	FILE *fp1 = fopen(m_sPath + "\\mode\\mouth_fld.txt", "r");
	FILE *fp2 = fopen(m_sPath + "\\mode\\mouth_mean.txt", "r");
	FILE *fp3 = fopen(m_sPath + "\\mode\\mouth_weight.txt", "r");

	int i, j;
	double  FLD1[5500], FLD2[5500], FLD3[5500], MV[5500], IMG[5500];
	double  WT1[200], WT2[200], WT3[200], Dist[200]; 

	if(fp1 != NULL && fp2 != NULL)
	{
		double dx, dy, dz, dn;
		for(i = 0; i < 5500; i++)
		{	
			fscanf(fp1, "%lf ", &dx);
			FLD1[i] = dx;
		}

		for(i = 0; i < 5500; i++)
		{
			fscanf(fp1, "%lf ", &dy);
			FLD2[i] = dy;
		}

		for(i = 0; i < 5500; i++)
		{
			fscanf(fp1, "%lf ", &dz);
			FLD3[i] = dz;
		}

		for(i = 0; i < 5500; i++)
		{
			fscanf(fp2, "%lf ", &dn);
			MV[i] = dn;
		}

		for(i = 0; i < 200; i++)
		{	
			fscanf(fp3, "%lf ", &dx);
			WT1[i] = dx;
		}

		for(i = 0; i < 200; i++)
		{
			fscanf(fp3, "%lf ", &dy);
			WT2[i] = dy;
		}

		for(i = 0; i < 200; i++)
		{
			fscanf(fp3, "%lf ", &dz);
			WT3[i] = dz;
		}
		
		fclose(fp1);
		fclose(fp2);
		fclose(fp3);
	}
	
	dPoint tp;
	vector<dPoint> mouthPoints, outPoints;
	for(i = 0; i < 12; i++)
	{
		tp.x = m_ptArrLandMarks[74 + i].x;
		tp.y = m_ptArrLandMarks[74 + i].y;
		mouthPoints.push_back(tp);
	}

	CvvImage mouthImg;
	CropImage(mouthImg, mouthPoints, outPoints);
	mouthImg.Save("e:\\ee.jpg");

	CvvImage stdImg;
	vector<dPoint> stdPoints;
	LoadTemplate(m_sPath + "\\mode\\stdMouth.pts", stdPoints);
	AlignImage(mouthImg, stdImg, outPoints, stdPoints, 110, 50);
	stdImg.Save("e:\\std.jpg");

	BYTE *buf = (BYTE *)stdImg.GetImage()->imageData;
	int WidthPix = WIDTHBYTES(stdImg.Width() * 8);
	for(i = 0; i < stdImg.Height(); i++)
	{
		for(j = 0; j < stdImg.Width(); j++)
		{
			IMG[i * stdImg.Width() + j] = buf[i * WidthPix + j];
		}
	}

	double sum1 = 0; 
	double sum2 = 0;
	double sum3 = 0;
	for(i = 0; i < 5500; i++)
	{
		sum1 += (IMG[i] - MV[i]) * FLD1[i];
		sum2 += (IMG[i] - MV[i]) * FLD2[i];
		sum3 += (IMG[i] - MV[i]) * FLD3[i];
	}

	CString aa;
	aa.Format("%lf %lf %lf", sum1, sum2, sum3);
	MessageBox(aa);

	for(i = 0; i < 200; i++)
	{
	  Dist[i] = sqrt(Sqr(sum1 - WT1[i]) + Sqr(sum2 - WT2[i]) + Sqr(sum3 - WT3[i]));
	}

	double DistCopy[200];
	for(i = 0; i < 200; i++)
	{
		DistCopy[i] = Dist[i];
	}

	FILE *fp = fopen("E:\\dist.txt", "w");
	for(i = 0; i < 199; i++)
	{
		for(j = i + 1; j < 200; j++)
		{
			if(DistCopy[i] > DistCopy[j])
			{
				double temp = DistCopy[i];
				DistCopy[i] = DistCopy[j];
				DistCopy[j] = temp;
			}
		}

		fprintf(fp, "%lf ", DistCopy[i]);
	}

	fprintf(fp, "\n");
	for(i = 0; i < 15; i++)
	{
		for(j = 0; j < 200; j++)
		{
			if(DistCopy[i] == Dist[j])
			{
				order[i] = j;
				break;
			}
		}
        fprintf(fp, "%d\n", order[i]);
	}

	fclose(fp);
}

void CPortraitEditingDlg::SelectEyebrow(int *order)
{
	FILE *fp1 = fopen(m_sPath + "\\mode\\eyebrow_fld.txt", "r");
	FILE *fp2 = fopen(m_sPath + "\\mode\\eyebrow_mean.txt", "r");
	FILE *fp3 = fopen(m_sPath + "\\mode\\eyebrow_weight.txt", "r");

	int i, j;
	double  FLD1[2400], FLD2[2400], MV[2400], IMG[2400];
	double  WT1[150], WT2[150], Dist[150]; 

	if(fp1 != NULL && fp2 != NULL)
	{
		double dx, dy, dn;
		for(i = 0; i < 2400; i++)
		{	
			fscanf(fp1, "%lf ", &dx);
			FLD1[i] = dx;
		}

		for(i = 0; i < 2400; i++)
		{
			fscanf(fp1, "%lf ", &dy);
			FLD2[i] = dy;
		}

		for(i = 0; i < 2400; i++)
		{
			fscanf(fp2, "%lf ", &dn);
			MV[i] = dn;
		}

		for(i = 0; i < 150; i++)
		{	
			fscanf(fp3, "%lf ", &dx);
			WT1[i] = dx;
		}

		for(i = 0; i < 150; i++)
		{
			fscanf(fp3, "%lf ", &dy);
			WT2[i] = dy;
		}
		
		fclose(fp1);
		fclose(fp2);
		fclose(fp3);
	}
	
	dPoint tp;
	vector<dPoint> eyebrowPoints, outPoints;
	for(i = 0; i < 8; i++)
	{
		tp.x = m_ptArrLandMarks[25 + i].x;
		tp.y = m_ptArrLandMarks[25 + i].y;
		eyebrowPoints.push_back(tp);
	}

	CvvImage eyebrowImg;
	CropImage(eyebrowImg, eyebrowPoints, outPoints);
	eyebrowImg.Save("e:\\ee.jpg");

	CvvImage stdImg;
	vector<dPoint> stdPoints;
	LoadTemplate(m_sPath + "\\mode\\stdEyebrow.pts", stdPoints);
	AlignImage(eyebrowImg, stdImg, outPoints, stdPoints, 80, 30);
	stdImg.Save("e:\\std.jpg");

	BYTE *buf = (BYTE *)stdImg.GetImage()->imageData;
	int WidthPix = WIDTHBYTES(stdImg.Width() * 8);
	for(i = 0; i < stdImg.Height(); i++)
	{
		for(j = 0; j < stdImg.Width(); j++)
		{
			IMG[i * stdImg.Width() + j] = buf[i * WidthPix + j];
		}
	}

	double sum1 = 0; 
	double sum2 = 0;
	for(i = 0; i < 2400; i++)
	{
		sum1 += (IMG[i] - MV[i]) * FLD1[i];
		sum2 += (IMG[i] - MV[i]) * FLD2[i];
	}

	for(i = 0; i < 150; i++)
	{
	  Dist[i] = GetDistance(sum1, sum2, WT1[i], WT2[i]);
	}

	double DistCopy[150];
	for(i = 0; i < 150; i++)
	{
		DistCopy[i] = Dist[i];
	}

	FILE *fp = fopen("E:\\dist.txt", "w");
	for(i = 0; i < 149; i++)
	{
		for(j = i + 1; j < 150; j++)
		{
			if(DistCopy[i] > DistCopy[j])
			{
				double temp = DistCopy[i];
				DistCopy[i] = DistCopy[j];
				DistCopy[j] = temp;
			}
		}

		fprintf(fp, "%lf ", DistCopy[i]);
	}

	fprintf(fp, "\n");
	for(i = 0; i < 15; i++)
	{
		for(j = 0; j < 150; j++)
		{
			if(DistCopy[i] == Dist[j])
			{
				order[i] = j;
				break;
			}
		}
        fprintf(fp, "%d\n", order[i]);
	}

	fclose(fp);
}

void CPortraitEditingDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	OnBackground();
}

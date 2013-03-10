// Portrait EditingDlg.h : header file
//

#if !defined(AFX_PORTRAITEDITINGDLG_H__E585AB8B_CE71_41BE_9C19_511CF6E2A74D__INCLUDED_)
#define AFX_PORTRAITEDITINGDLG_H__E585AB8B_CE71_41BE_9C19_511CF6E2A74D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPortraitEditingDlg dialog
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
#define NUM_OF_LANDMARKS 105
#define MAX_NUM_FACES 5

#include "BtnST.h"
#include "FileTool.h"

#include "Image.h"
#include "Predefine.h"
#include "fllapi.h"
#include "fdapi.h"

#include "cv.h"
#include "highgui.h"

#include "DLL_ShapeContext.h"
#include "SCEnergy.h"

#include "Common.h"
#include <vector>

#include "..\Gc\GCsegmentation.h"
#include "..\renderCurve\curvemorphing.h"
//#include "gdiplusbase.h"
//using namespace Gdiplus;
using namespace std;

enum myStatus
{
	feature = 1, 
	background = 2, 
	hair = 3,
	skin = 4,
	collar = 5,
	nColor = 6,
	nBrush = 7,
	scissors = 8,
	graphcutFore = 9,
	graphcutBack = 10,
	ear = 11
};

class CPortraitEditingDlg : public CDialog
{
// Construction
public:
	CBitmap			m_bmMainBg;
	CButtonST       m_btOpen;
	CButtonST       m_btScissors;
	CButtonST       m_btFeature;
	CButtonST       m_btBackground;
	CButtonST       m_btSkin;
	CButtonST       m_btHair;
	CButtonST       m_btCollar;
	CButtonST       m_btGetColor;
	CButtonST       m_btBrushColor;
	CButtonST       m_btHairColor;
	CButtonST       m_btSkinColor;
	CButtonST       m_btClothColor;
	CButtonST       m_btBackgroundColor;
	CButtonST       m_btShadowColor;
	CButtonST       m_btDefaultColor;
	CButtonST       m_btCrossStitch;
	CButtonST       m_btCartoon;
	CButtonST       m_btSave;
	CButtonST       m_btExit;
	CButtonST       m_btRender;
	CButtonST       m_btEar;
	CButtonST       m_btAdjust;

	int             m_nR, m_nG, m_nB;

	FileTool		ft;

	CPoint			m_ptArrLandMarks[NUM_OF_LANDMARKS];
	int             m_nStatus;
	int             m_nRender;
	int				m_nSelectedFeature; 
	int             m_nRecordIndex;        //记录前进后退的标号；
	int             m_nRecordMaxIndex;
	vector<int>     m_nSelectedVector;
	bool            m_bRectMove;
	bool            m_bRectSelected;
	bool			m_bDrag;
	bool            m_bDown;
	bool			m_bImgOpened;
	bool			m_bFeatureLocalized;
	bool            m_bAuto;
	CImage  		m_pCurImg;
	CvvImage		m_TargetImg; //生成肖像图像
	CvvImage		m_image;     //当前显示图像
	CvvImage        m_binaryImg;
	CvvImage        m_TargetImgCopy;
	CvvImage        m_ColorImgCopy;//缺省淡彩图像的副本

	CvvImage        m_RenderImg;
	CvvImage        m_RegionImg;

	bool            m_bLeftEar;
	bool            m_bRightEar;
	///////////////////////////////////用于避免重复定位
	bool            m_bFeature;
	bool            m_bBackground;
	bool            m_bSkin;
	bool            m_bHair;
	bool            m_bCollar;
	bool            m_bEar;
	bool            m_bAutoColor;
	///////////////////////////////////用于筛选模板类型
	int				hairIndex[2];
	int             clothIndex[2];
	////////////////////////////////////用于记录手工选择的模板
	int             m_nCompSelected[8];
	
	GCsegmentation  *Gcs;
	CImg<unsigned char>OrignalImage;  //用于GC的图，因为GC的图象是CIMG类型
	CImg<unsigned char> ResultImg;    //GC后，生成的一个黑白图象
	bool            m_bGCInit;
	int             m_nGCStatus;
	CImg<unsigned char> BrushImg;       //用于矢量化加刷子的图

	vector<dPoint>  m_stdNoseVector;
	vector<dPoint>  m_stdMouthVector;
	vector<dPoint>  m_stdLeftEyeVector;
	vector<dPoint>  m_stdRightEyeVector;
	vector<dPoint>  m_stdLeftEyebrowVector;
	vector<dPoint>  m_stdRightEyebrowVector;

	//for scissors
	CRect		scissorsRect;
	CString		strImgName;
	bool		m_bColorAdjust;
	bool        m_bStart;
	double      m_dRatio;

	CPoint      oldPoint;
	vector<Ptposition>FrontStroke;
	vector<Ptposition>BackStroke;
	
	//for image and feature point show
	CPoint m_ptLeftTop;
	double m_dScale;

	CRgn bgRgn;
    CRect bgRect;
	vector<dPoint>  m_backgroundVector;

	CRgn skinRgn;
	CRect skRect;
	vector<dPoint> m_skinVector;
	vector<dPoint> m_skinVectorCopy;

	CRgn	 leftEarRgn, rightEarRgn;
	CRect    leftEarRect, rightEarRect;
	vector<dPoint> leftEarVector;
	vector<dPoint> rightEarVector;

	CRgn hairRgn;
	CRect hairRect;
	vector<double> hairVectorX;
	vector<double> hairVectorY;

	CRgn clothRgn;
	CRect clothRect;
	vector<double> clothVectorX;
	vector<double> clothVectorY;

	vector<double> vectorXCopy;
	vector<double> vectorYCopy;

	////zijian ASM
	dPoint m_dpLandMarks[91];
	dPoint m_dpLandMarksCopy[91];
	bool m_fbInitShape;
	std::vector<bool> m_vbConstrained;
	
	CVisRGBAByteImage m_SrcImage;
	CVisGrayByteImage m_SrcGrayImage;

	CShape m_CurShape;
	CShape m_ConstrainShape;
	CASMMLSearchModelBayesian* m_pModel;


	void			ImageChanged(IplImage* grayImage, int gray);
	double          ImageZoom(CvvImage &Image);
	void            ImageZoom(CvvImage &Image, double ratioX, double ratioY);
	void			Mean(IplImage* YCrCbImage, CRect rect, double &meanY, double &meanCr, double &meanCb, double &max_diffY, double &max_diffCr, double &max_diffCb);
    void            LoadFile(CString sFileName);
	void			LoadTemplate(CString sFileName, vector<dPoint> &pointVector);
	void			LoadTemplate(CString sFileName, vector<double> &pointVectorX, vector<double> &pointVectorY);
	void            LoadTemplate(CString sFileName, vector< vector<CPoint> > &pointVVector);
	void			ReadTriangle(CString sFileName,  vector<int> &nV1,  vector<int> &nV2, vector<int> &nV3);
	void			SampleLine(dPoint p1, dPoint p2, double length, vector<dPoint> &pointVector);
	void			ImageWarp(IplImage *sourceImage, IplImage *targetImage, vector<dPoint> &sourceVector, vector<dPoint> &targetVector, bool isErase = true);
	void			WarpImg2Img(IplImage *imgDes, IplImage *imgSrc, vector<CPoint> &vecDes, vector<CPoint> &vecSrc, vector<int> &vMeshV1, vector<int> &vMeshV2, vector<int> &vMeshV3);
	void            ExtractPnt(vector<CPoint> &vecDes);

	void            ConncetContour(vector<dPoint> &pointVector, IplImage *img, CvScalar &color, int lineWidth, bool isSkin = false);
	void            ConncetContour(vector<double> &pointVectorX, vector<double> &pointVectorY, IplImage *img, CvScalar &color, int lineWidth, bool isOpen = false);
	void            ConnectLandmark(CvScalar &color, int lineWidth);
	void            DrawEyeball();
	void            DrawPortrait();
	void            DetectHairCollar();

	void			ColorAdjust(); 
	void            ColorTransfer(CvvImage &inputImage, int r, int g, int b, CvvImage &outputImage);
	void            CrossTransfer(CvvImage &inputImage, CvvImage &outputImage, CvvImage &crossImage);
	void            CrossTransfer2(CvvImage &inputImage, CvvImage &outputImage, CvvImage &crossImage);
 
	void            RenderHair(int index, CString &templatePath);
	void            RenderEyebrow(int index, CString &templatePath);
	void            RenderEye(int index, CString &templatePath);
	void            RenderNose(int index, CString &templatePath);
	void            RenderMouth(int index, CString &templatePath);
	void            RenderCollar(int index, CString &templatePath);
	void            RenderEar(int index, CString &templatePath);
	void            RenderFaceContour(int index, CString &templatePath);
	void            AutoSelected(vector<double> &pointVectorX, vector<double> &pointVectorY, int templateSize, CString path, CString fileName, int index[]);

	void            SmoothCurve(vector<dPoint> &pointVector, double param1 = 0.5, double param2 = 1.0);
	void            LoadASMModel();
	void            ChangeLandmarks();
	void            ModifyLandmarks();
	void            Shapeinit();
	void            ConstrainASMSearch();
	void            ConnectLandmark91(CvScalar &color, int lineWidth);

	void            VectorLine(CvvImage &templateImage, CvvImage &zoomImage, CPoint &point1, CPoint &point2, CPoint &Zpoint1, CPoint &Zpoint2);
	void            VectorCurve(CvvImage &templateImage, CvvImage &zoomImage, vector<double> &sPointsX, vector<double> &sPointsY, vector<double> &dPointsX, vector<double> &dPointsY);
    void            FillCloth();
	void            FillSkin(CvvImage &tarImage, int x, int y, int r, int g, int b);
	void			SelectContourPoint(vector<double> &DenseX, vector<double> &DenseY, vector<double> &SparseX, vector<double> &SparseY, int pointCount);
	
	void            SaveXML(CString sFileName);
	void            LoadXML(CString sFileName);

	void			SobelOperator(IplImage* grayImage, double *pGradX, double *pGradY, double *pMod);
	void            CropImage(CvvImage &comImage, vector<dPoint> &inPoints, vector<dPoint> &outPoints);
	void            AlignEyeImage(CvvImage &comImage, CvvImage &stdImage, vector<dPoint> &comPoints, vector<dPoint> &stdPoints, int stdWidth, int stdHeight);
	void            AlignNoseImage(CvvImage &comImage, CvvImage &stdImage, vector<dPoint> &comPoints, vector<dPoint> &stdPoints, int stdWidth, int stdHeight);
	void            AlignImage(CvvImage &comImage, CvvImage &stdImage, vector<dPoint> &comPoints, vector<dPoint> &stdPoints, int stdWidth, int stdHeight);
	void            SelectEye(int *order);
	void            SelectNose(int *order);
	void            SelectMouth(int *order);
	void            SelectEyebrow(int *order);

public:
	CPortraitEditingDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPortraitEditingDlg)
	enum { IDD = IDD_PORTRAITEDITING_DIALOG };
	CListCtrl	m_LstCtrComponent;
	CImageList  m_ImageListHair;
	CImageList  m_ImageListEye;
	CImageList  m_ImageListEyebrow;
	CImageList  m_ImageListNose;
	CImageList  m_ImageListMouth;
	CImageList  m_ImageListCollar;
	CImageList  m_ImageListBG;
	CImageList  m_ImageListEar;
	CImageList  m_ImageListFaceContour;
	int		m_nPart;
	//}}AFX_DATA

	CString m_sPath;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPortraitEditingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPortraitEditingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOpenImage();
	afx_msg void OnFeatureLocal();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDetectBackground();
	afx_msg void OnDetectSkin();
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHairCOLOR();
	afx_msg void OnSkinColor();
	afx_msg void OnClothColor();
	afx_msg void OnBackgroundColor();
	afx_msg void OnShadowColor();
	afx_msg void OnDefaultCOLOR();
	afx_msg void OnDetectHair();
	afx_msg void OnDetectCollar();
	afx_msg void OnCrossStitch();
	afx_msg void OnGetColor();
	afx_msg void OnBrushColor();
	afx_msg void OnPortrait();
	afx_msg void OnSave();
	afx_msg void OnExit();
	afx_msg void OnScissors();
	afx_msg void OnHair();
	afx_msg void OnEyebrow();
	afx_msg void OnEye();
	afx_msg void OnNose();
	afx_msg void OnMouth();
	afx_msg void OnCollar();
	afx_msg void OnBackground();
	afx_msg void OnAutorender();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEffect1();
	afx_msg void OnEffect2();
	afx_msg void OnEffect3();
	afx_msg void OnFore();
	afx_msg void OnBackg();
	afx_msg void OnGcut();
	afx_msg void OnConstrainAsm();
	afx_msg void OnDestroy();
	afx_msg void OnDetectEar();
	afx_msg void OnEar();
	afx_msg void OnFacecontour();
	afx_msg void OnRedo();
	afx_msg void OnUndo();
	afx_msg void OnReset();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTRAITEDITINGDLG_H__E585AB8B_CE71_41BE_9C19_511CF6E2A74D__INCLUDED_)

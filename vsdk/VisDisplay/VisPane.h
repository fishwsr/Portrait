////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISPANE
//
// @module VisPane.h |
//
//  This file declares classes (derived from MFC's <c CWnd> class)
//  which are used to display images.
//
// <nl>
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisPane\.cpp>
//
// <nl>
// @index | VISPANE
//
////////////////////////////////////////////////////////////////////////////



#ifndef __VIS_PANE_H__
#define __VIS_PANE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



////////////////////////////////////////////////////////////////////////////
//  
// @enum EVisPane |
// Constants used to specify CVisPane options.
//
// LATER:  Add options to put image size and type in status bar and
// add option to have pane automatically open files dragged onto it?
//
////////////////////////////////////////////////////////////////////////////
enum EVisPane
{
	//------------------------------------------------------------------
	// @group General CVisPane options

	// @emem
	// Destroy the pane window when all CVisPane objects refering to
	// it are destroyed.  If this flag is not specified, the window
	// will need to be explicitly destroyed by the DestroyWnd method
	// or closed by the user.
	evispaneAutoDestroy				= 0x0001,

	// @emem
	// Create a new thread for the pane (or the window containing
	// an array of panes).  This option is ignored if a parent window
	// is specified.
	evispaneOwnThread				= 0x0002,

	// @emem
	// Changes to window styles may change the size of the rectangle
	// inside the pane that is used to display images.  If this flag
	// is not set, changes to window styles may change the outer
	// dimensions of the pane window.
	evispaneStyleChangesSizeInside	= 0x0004,

	// @emem
	// Should panes in an array of panes have borders drawn by Windows?
	evispanePaneBorder				= 0x0008,

	// @emem
	// Get the title for this pane from the image that it contains.
	evispaneAutoTitle				= 0x0010,

	// @emem
	// Should panes in an array of panes have their own titles?
	evispanePaneTitle				= 0x0020,

	// @emem
	// Should panes in an array of panes have their own status bars?
	evispanePaneStatusBar			= 0x0040,

	// @emem
	// When giving status information about an pane, include the
	// name of the pane.
	evispaneNameInStatus			= 0x0080,

	//------------------------------------------------------------------
	// @group CVisPane image display options

	// @emem
	// Resize the pane to fit the current image.  (This option is ignored if
	// this pane is part of a larger array of panes.)
	evispaneResizeToFit				= 0x0100,

	// @emem
	// Stretch the image to fit the pane.  Aspect may not be preserved.
	// This option is not supported in the current version of the SDK.
	evispaneStretchToFit			= 0x0200,

	// @emem
	// Stretch the image to fit the pane, preserve image aspect ratio.
	// This option is not supported in the current version of the SDK.
	evispaneStretchPreserveAspect	= 0x0400,

	// @emem
	// Use scroll bars to scroll the image in the pane.  This option
	// is not supported in the current version of the SDK.
	evispaneScrollImage				= 0x0800,

	// @emem
	// If an image can't be displayed, put an error message in the
	// status bar when the mouse moves over the pane.
	evispaneBadImageErrorStatus		= 0x1000,

	// @emem
	// If an image can't be displayed, put an error message in the
	// pane window.
	evispaneBadImageErrorInWindow	= 0x2000,

	// @emem
	// If there's no image to display, put a message in the status
	// bar when the mouse moves over the pane.
	evispaneNoImageErrorStatus		= 0x4000,

	// @emem
	// If there's no image to display, put a message in the pane
	// window.
	evispaneNoImageErrorInWindow	= 0x8000,

	//------------------------------------------------------------------
	// @group CVisPaneArray options

	// @emem
	// Use the Pane size specified as the size of the whole array of
	// panes, not the size of each individual pane.
	evispaneSizeForArray			= 0x010000,

	// @emem
	// Should the array of panes have a title?
	evispaneArrayTitle				= 0x020000,

	// @emem
	// Should the array of panes have a border drawn by Windows?
	evispaneArrayBorder				= 0x040000,

	// @emem
	// Add a status bar to the array of panes (or to a single pane
	// if it is not part of an array of panes).
	evispaneArrayStatusBar			= 0x080000,

	//------------------------------------------------------------------
	// @group Window styles

	// @emem
	// Can the main window (for the pane or array of panes) be resized?
	evispaneCanResize				= 0x100000,

	// @emem
	// Can the main window (for the pane or array of panes) be maximized?
	evispaneCanMaximize				= 0x200000,

	// @emem
	// Use this option to prevent the user from moving the main window
	// (for the pane or array of panes) when the main window has a
	// title bar.
	evispaneNoMove					= 0x400000,

	// @emem
	// Should the pane or array of panes be made visible when it is
	// created?
	evispaneVisible					= 0x800000,

	// @emem
	// Can the tab key be used to move from the main window (for the
	// pane or array of panes) to its siblings.
	evispaneTabStop					= 0x01000000,

	// @emem
	// Should the main window (for the pane or array of panes) be
	// made to stay on top of all other windows?
	evispaneTopMost					= 0x02000000,

	// LATER:  Add a Transparent option?
	// LATER:  Add a ContextHelp option and message handlers?

	//------------------------------------------------------------------
	// @group CVisPane message handlers

	// @emem
	// Add a mouse event handler to allow this pane to be selected.
	evispaneSelectHandler			= 0x10000000,

	// @emem
	// Add a mouse event handler to show pixel values for this image.
	evispaneShowValues				= 0x20000000,

	//------------------------------------------------------------------
	// @group Common combinations of pane options

	// @emem
	// Default status options
	evispaneStatusDefault			= evispaneNameInStatus
									| evispaneBadImageErrorStatus
									| evispaneBadImageErrorInWindow
									| evispaneNoImageErrorStatus
									| evispaneArrayStatusBar,

	// @emem
	// Default pane options
	evispaneDefault					= evispaneAutoTitle
									| evispanePaneTitle
									| evispanePaneBorder
									| evispaneArrayTitle
									| evispaneArrayBorder
									| evispaneStatusDefault
									| evispaneCanResize
									| evispaneCanMaximize
									| evispaneVisible
									| evispaneSelectHandler
									| evispaneShowValues,

	// @emem
	// Fixed-size pane options.
	evispaneDefaultFixedSize		= evispaneResizeToFit
									| evispaneAutoTitle
									| evispanePaneTitle
									| evispanePaneBorder
									| evispaneArrayTitle
									| evispaneArrayBorder
									| evispaneStatusDefault
									| evispaneVisible
									| evispaneSelectHandler
									| evispaneShowValues,

	// @emem
	// Pane that is automatically destroyed.
	evispaneDefaultAutoDestroy		= evispaneAutoDestroy
									| evispaneAutoTitle
									| evispanePaneTitle
									| evispanePaneBorder
									| evispaneArrayTitle
									| evispaneArrayBorder
									| evispaneStatusDefault
									| evispaneCanResize
									| evispaneCanMaximize
									| evispaneVisible
									| evispaneSelectHandler
									| evispaneShowValues,

	// @emem
	// Fixed-size pane that is automatically destroyed.
	evispaneDefaultFixedAutoDestroy	= evispaneAutoDestroy
									| evispaneResizeToFit
									| evispaneAutoTitle
									| evispanePaneTitle
									| evispanePaneBorder
									| evispaneArrayTitle
									| evispaneArrayBorder
									| evispaneStatusDefault
									| evispaneVisible
									| evispaneSelectHandler
									| evispaneShowValues,

	evispaneUnknown					= - 1
};




////////////////////////////////////////////////////////////////////////////
//  
// @enum EVisPaneDisp |
// Display options
//
////////////////////////////////////////////////////////////////////////////
enum EVisPaneDisp
{
	evispanedispDefault = 0,

	// @emem
	// Copy the original image and use a buffer for the screen image.
	evispanedispCopyOrigUseScrnBuf = 0,

	// @emem
	// Copy the original image but don't use a buffer for the screen image.
	evispanedispCopyOrigNoScrnBuf,

	// @emem
	// Alias the original image and use a buffer for the screen image.
	evispanedispAliasOrigUseScrnBuf,

	// @emem
	// Alias the original image but don't use a buffer for the screen image.
	evispanedispAliasOrigNoScrnBuf,

	// @emem
	// Don't store (a copy or alias of) the original image, but use a buffer
	// for the screen image.
	evispanedispNoOrigUseScrnBuf,

	// @emem
	// Don't store (a copy or alias of) the original image and don't use a
	// buffer for the screen image.
	evispanedispNoOrigNoScrnBuf
};



////////////////////////////////////////////////////////////////////////////
//  
// @enum EVisMouse |
// Constants used to specify mouse events.
//
////////////////////////////////////////////////////////////////////////////
enum EVisMouse
{
	//------------------------------------------------------------------
	// @group Actions

	// @emem
	// A button is pressed.  (If no button is specified, the left
	// button is assumed.)
	evismouseButtonDown			= 0x0001,

	// @emem
	// A button is released after being pressed.  (If no button is
	// specified, the left button is assumed.)
	evismouseButtonUp			= 0x0002,

	// @emem
	// A button is double-clicked.  (If no button is
	// specified, the left button is assumed.)
	evismouseDoubleClick		= 0x0004,

	// @emem
	// The mouse is moved.
	evismouseMouseMove			= 0x0008,

	// @emem
	// Action mask.
	evismouseMaskAction			= 0x000f,


	//------------------------------------------------------------------
	// @group Button

	// @emem
	// Used to specify that no buttons are pressed when the mouse
	// is moved.  Not used with any other actions.
	evismouseNoButton			= 0x0010,

	// @emem
	// Used to specify that the left button is pressed, released,
	// double-clicked, or held down when the mouse is moved.
	evismouseLeftButton			= 0x0020,

	// @emem
	// Used to specify that the right button is pressed, released,
	// double-clicked, or held down when the mouse is moved.
	evismouseRightButton		= 0x0040,

	// @emem
	// Used to specify that both buttons are pressed when the mouse
	// is moved.  Not used with any other actions.
	evismouseBothButtons		= 0x0060,

	// @emem
	// Button mask.
	evismouseMaskButton			= 0x00f0,


	//------------------------------------------------------------------
	// @group Modifier keys

	// @emem
	// Used to specify that the control and shift keys are not pressed
	// at the time of this action.
	evismouseNoModifiers		= 0x0100,

	// @emem
	// Used to specify that the shift key is pressed and the control
	// key is not pressed at the time of this action.
	evismouseShift				= 0x0200,

	// @emem
	// Used to specify that the control key is pressed and the shift
	// key is not pressed at the time of this action.
	evismouseControl			= 0x0400,

	// @emem
	// Used to specify that the control and shift keys are both
	// pressed at the time of this action.
	evismouseCtrlShift			= 0x0600,

	// @emem
	// Modifier mask.
	evismouseMaskModifier		= 0x0f00,


	//------------------------------------------------------------------
	// @group Common combinations

	// @emem
	// The left button is pressed.
	evismouseLButtonDown
			= evismouseButtonDown | evismouseLeftButton,

	// @emem
	// The left button is released after being pressed.
	evismouseLButtonUp
			= evismouseButtonUp | evismouseLeftButton,

	// @emem
	// The left button is double-clicked.
	evismouseLDoubleClick
			= evismouseDoubleClick | evismouseLeftButton,

	// @emem
	// The right button is pressed.
	evismouseRButtonDown
			= evismouseButtonDown | evismouseRightButton,

	// @emem
	// The right button is released after being pressed.
	evismouseRButtonUp
			= evismouseButtonUp | evismouseRightButton,

	// @emem
	// The right button is double-clicked.
	evismouseRDoubleClick
			= evismouseDoubleClick | evismouseRightButton,

	// LATER:  Add more combinations?

	//------------------------------------------------------------------
	// @group Default

	// @emem
	// The default event is the left-button-down event, which happens
	// when the left button is pressed.
	evismouseDefault			= evismouseLButtonDown
};



// Forward declarations
class CVisPane;
class CVisRefCntWnd;
class CVisPaneWndBase;
class CVisPaneWnd;
class CVisPaneThread;


////////////////////////////////////////////////////////////////////////////
//  
// @cb void | VisPfnMouseEventCallback |
//  Type of callback function used to process mouse events.
//
// @parm UINT | flags |
//  Passed from MFC's event handler.
//  @todo Document this better.
//
// @parm CVisPoint | point |
//  Location where event took place.
//  @todo Document this better.
//
// @parm CVisPane * | pane |
//  CVisPane in which the event took place.
//  @todo Is this right, or is this just the pane giving a notification
//  message?
//
// @parm void * | params |
//	User defined pointer used to pass data to the event handler.
//
////////////////////////////////////////////////////////////////////////////
// UNDONE:  This is a temporary workaround.
#if VIS_USER_BUILD_mturk
typedef void (VISAPI *VisPfnMouseEventCallback)(UINT nFlags, CPoint point,
		CVisPaneWnd *ppanewnd, void *pvUser, bool fOutside);
#else // VIS_USER_BUILD_mturk
typedef void (VISAPI *VisPfnMouseEventCallback)(UINT nFlags, CPoint point,
		CVisPaneWnd *ppanewnd, void *pvUser);
#endif // VIS_USER_BUILD_mturk

////////////////////////////////////////////////////////////////////////////
//  
// @class ***
//
////////////////////////////////////////////////////////////////////////////
class VisDisplayExport CVisPane
{
// @access Public members
public:
    //------------------------------------------------------------------
	// @group Constructors and destructor

    // @cmember
	// Default constructor.  This does not create a window.  The
	// CreateWnd method must be called to create the window for the pane.
	CVisPane(CWnd *pwndParent = 0);

    // @cmember
	// Constructor.  This does not create a window.  The
	// CreateWnd method must be called to create the window for the pane.
	CVisPane(int evispane, CWnd *pwndParent = 0);

    // @cmember
	// Constructor.  If fCreateWnd is true, a window will be created for
	// the pane.  Otherwise, the CreateWnd method must be called to create
	// the window for the pane.
	CVisPane(int dx, int dy, const char *szTitle = 0,
			int evispane = evispaneDefault, CWnd *pwndParent = 0,
			bool fCreateWnd = true);

    // @cmember
	// Constructor.  If fCreateWnd is true, a window will be created for
	// the pane.  Otherwise, the CreateWnd method must be called to create
	// the window for the pane.
	CVisPane(SIZE size, const char *szTitle = 0,
			int evispane = evispaneDefault, CWnd *pwndParent = 0,
			bool fCreateWnd = true);

    // @cmember
	// Constructor.  If fCreateWnd is true, a window will be created for
	// the pane.  Otherwise, the CreateWnd method must be called to create
	// the window for the pane.
	CVisPane(POINT point, SIZE size, const char *szTitle = 0,
			int evispane = evispaneDefault, CWnd *pwndParent = 0,
			bool fCreateWnd = true);

    // @cmember
	// Constructor.  If fCreateWnd is true, a window will be created for
	// the pane.  Otherwise, the CreateWnd method must be called to create
	// the window for the pane.
	CVisPane(const RECT& refrect, const char *szTitle = 0,
			int evispane = evispaneDefault, CWnd *pwndParent = 0,
			bool fCreateWnd = true);

    // @cmember
	// Constructor to wrap a CVisPaneWnd.
	CVisPane(CVisPaneWnd *ppanewnd);

    // @cmember
	// Copy constructor.
	CVisPane(const CVisPane& refpane);

    // @cmember
	// Assignment operator.
	CVisPane& operator=(const CVisPane& refpane);

    // @cmember
	// Destructor.
	virtual ~CVisPane(void);


// Attributes
public:
    //------------------------------------------------------------------
	// @group Attributes

	int XOffsetInParent(void) const;
	void SetXOffsetInParent(int x, bool fRepaint = true);

	int YOffsetInParent(void) const;
	void SetYOffsetInParent(int y, bool fRepaint = true);

	CVisPoint PointOffsetInParent(void) const;
	void SetPointOffsetInParent(POINT point, bool fRepaint = true);

	int WidthInside(void) const;
	void SetWidthInside(int dx, bool fRepaint = true);

	int WidthOutside(void) const;
	void SetWidthOutside(int dx, bool fRepaint = true);

	int WidthBorder(void) const;
	void SetWidthBorder(int dx, bool fRepaint = true);

	int HeightInside(void) const;
	void SetHeightInside(int dy, bool fRepaint = true);

	int HeightOutside(void) const;
	void SetHeightOutside(int dy, bool fRepaint = true);

	CSize SizeInside(void) const;
	void SetSizeInside(int dx, int dy, bool fRepaint = true);
	void SetSizeInside(SIZE size, bool fRepaint = true);

	CSize SizeOutside(void) const;
	void SetSizeOutside(int dx, int dy, bool fRepaint = true);
	void SetSizeOutside(SIZE size, bool fRepaint = true);

	CSize SizeInOutDifference(void) const;

	CVisPoint ImageOffset(void) const;
    void SetImageOffset(int dx, int dy, bool fRefresh = true);
    void SetImageOffset(POINT point, bool fRefresh = true);

	CVisRect RectOutside(void) const;
	void SetRectOutside(const RECT& refrect, bool fRepaint = true);

	DWORD Style(void) const;
	void SetStyle(DWORD dw);

	DWORD ExStyle(void) const;
	void SetExStyle(DWORD dw);

	const char *Title(void);
	void SetTitle(const char *szTitle);

	bool HasWnd(void) const;
	const CWnd *PWnd(void) const;
	CWnd *PWnd(void);

	const CWnd *PWndParent(void) const;
	CWnd *PWndParent(void);

    bool HasValidImage(void) const;
    bool CouldNotDisplayImage(void) const;

	const CVisImageBase& Image(void) const;
	const CVisImageBase& ScreenImage(void) const;

    bool IsSelected(void) const;
    void SetIsSelected(bool fIsSelected, bool fUpdateNow = false);

	COLORREF ColorrefBackground(void) const;
	void SetColorrefBackground(COLORREF colorref);

	bool HasRefreshed(void) const;


	int EvispaneOptions(void) const;

	// Is this pane is part of an array of panes?  If so, its parent is
	// really a CVisPaneArrayWnd.
	bool IsPartOfArray(void) const;
	int IndexInParent(void) const;

	void SetMessageText(const char *szMessage = 0);

	void SetMirrorImage(bool fMirror);
	bool GetMirrorImage() const;

	//------------------------------------------------------------------
	// @group Window creation and destruction
    
	BOOL CreateWnd(int evispane = evispaneUnknown, CWnd *pwndParent = 0,
			bool fChangeStyles = true);

	BOOL ShowWnd(int nCmdShow = SW_SHOWNA);

	void DestroyWnd(void);


    //------------------------------------------------------------------
	// @group Displaying the image

	// LATER:  Stretch and resize display options?

    // @cmember ***
	// If fUpdateNow is false, a WM_PAINT message will be posted to the
	// window's message queue and this method will return before the
	// window has been redrawn.  If fUpdateNow is true, this method will
	// not return until the window has been redrawn.
    void Refresh(bool fUpdateNow = false);

    // @cmember ***
    void Clear(bool fRemovePointsAndLines = true,
			bool fUpdateNow = false);

    // @cmember ***
    void Clear(COLORREF colorref, bool fRemovePointsAndLines = true,
			bool fUpdateNow = false);

	void Display(const CVisImageBase& refimage,
			EVisPaneDisp evispanedisp = evispanedispDefault,
			bool fUpdateNow = false, bool fCreateWndIfNeeded = true);

	void Display(const CVisImageBase& refimage, POINT pointImageOffset,
			EVisPaneDisp evispanedisp = evispanedispDefault,
			bool fUpdateNow = false, bool fCreateWndIfNeeded = true);

	void Display(EVisNormalize evisnormalize, const CVisImageBase& refimage,
			EVisPaneDisp evispanedisp = evispanedispDefault,
			bool fUpdateNow = false, bool fCreateWndIfNeeded = true);

	void Display(EVisNormalize evisnormalize, const CVisImageBase& refimage,
			POINT pointImageOffset,
			EVisPaneDisp evispanedisp = evispanedispDefault,
			bool fUpdateNow = false, bool fCreateWndIfNeeded = true);

	void Display(EVisNormalize evisnormalize, double dblMin, double dblMax,
			const CVisImageBase& refimage,
			EVisPaneDisp evispanedisp = evispanedispDefault,
			bool fUpdateNow = false, bool fCreateWndIfNeeded = true);

	void Display(EVisNormalize evisnormalize, double dblMin, double dblMax,
			const CVisImageBase& refimage, POINT pointImageOffset,
			EVisPaneDisp evispanedisp = evispanedispDefault,
			bool fUpdateNow = false, bool fCreateWndIfNeeded = true);

	// LATER:  Add fRefresh flag to Display and Clear methods?


    //------------------------------------------------------------------
	// @group Adding points and lines to the image

    void AddPoint(int x, int y, int nPenStyle = PS_SOLID,
			int dxThickness = 1, COLORREF colorref = RGB(255, 0, 0),
			bool fRefresh = true);

    // @cmember ***
    void AddLine(int x1, int y1, int x2, int y2, int nPenStyle = PS_SOLID,
			int dxThickness = 1, COLORREF colorref = RGB(255, 0, 0),
			bool fRefresh = true);

    // @cmember ***
    void DrawLine(int x1, int y1, int x2, int y2, int nPenStyle = PS_SOLID,
			int dxThickness = 1, COLORREF colorref = RGB(255, 0, 0));

    // @cmember ***
    void RemovePoint(int x, int y, bool fRefresh = true);

    // @cmember ***
    void RemoveLine(int x1, int y1, int x2, int y2, bool fRefresh = true);

    // @cmember ***
    void RemoveAllPointsAndLines(bool fRefresh = true);


    //------------------------------------------------------------------
	// @group Mouse event callback functions

	// @cmember
	// Add a callback function to handle the specified event.
	void AddMouseEventCallback(int evismouse,
			VisPfnMouseEventCallback pfnCallback, const char *sz = 0,
			void *pvUser = 0);

	// @cmember
	// Remove a callback function to handle the specified event.
	void RemoveMouseEventCallback(int evismouse,
			VisPfnMouseEventCallback pfnCallback, void *pvUser = 0);

	void RemoveMouseEventCallback(VisPfnMouseEventCallback pfnCallback,
			void *pvUser = 0);


	// LATER:  Add keyboard input events?


protected:
	friend class CVisPaneArrayWnd;

	void SetPWndParent(CWnd *pWndParent);

	void SetIsPartOfArray(bool fIsPartOfArray);
	void SetIndexInParent(int ipaneInParent);

	void SetEvispaneOptions(int evispane, bool fChangeStyles = true);


private:
	CVisPaneWnd *m_ppanewnd;


#ifdef VIS_PANE_OLD_MOUSE_METHODS
public:
    void PushEventLeftButtonUp(void (*f)(UINT, CVisPoint, CVisPane *, void *), void *params=NULL);
    void PushEventLeftButtonDown_all_cases(void (*f)(UINT, CVisPoint, CVisPane *, void *), const CString& string="", void *params=NULL);
    void PushEventLeftButtonDown(void (*f)(UINT, CVisPoint, CVisPane *, void *), const CString& string="", void *params=NULL);
    void PushEventLeftButtonDown_c(void (*f)(UINT, CVisPoint, CVisPane *, void *), const CString& string="", void *params=NULL);
    void PushEventLeftButtonDown_s(void (*f)(UINT, CVisPoint, CVisPane *, void *), const CString& string="", void *params=NULL);
    void PushEventLeftButtonDown_cs(void (*f)(UINT, CVisPoint, CVisPane *, void *), const CString& string="", void *params=NULL);
    void PushEventLeftButtonDblClk_all_cases(void (*f)(UINT, CVisPoint, CVisPane *, void *), void *params=NULL);
    void PushEventLeftButtonDblClk(void (*f)(UINT, CVisPoint, CVisPane *, void *), void *params=NULL);
    void PushEventLeftButtonDblClk_c(void (*f)(UINT, CVisPoint, CVisPane *, void *), void *params=NULL);
    void PushEventLeftButtonDblClk_s(void (*f)(UINT, CVisPoint, CVisPane *, void *), void *params=NULL);
    void PushEventLeftButtonDblClk_cs(void (*f)(UINT, CVisPoint, CVisPane *, void *), void *params=NULL);
    void PushEventRightButtonUp(void (*f)(UINT, CVisPoint, CVisPane *, void *), void *params=NULL);
    void PushEventRightButtonDown_all_cases(void (*f)(UINT, CVisPoint, CVisPane *, void *), const CString& string="", void *params=NULL);
    void PushEventRightButtonDown(void (*f)(UINT, CVisPoint, CVisPane *, void *), const CString& string="", void *params=NULL);
    void PushEventRightButtonDown_c(void (*f)(UINT, CVisPoint, CVisPane *, void *), const CString& string="", void *params=NULL);
    void PushEventRightButtonDown_s(void (*f)(UINT, CVisPoint, CVisPane *, void *), const CString& string="", void *params=NULL);
    void PushEventRightButtonDown_cs(void (*f)(UINT, CVisPoint, CVisPane *, void *), const CString& string="", void *params=NULL);
    void PushEventRightButtonDblClk_all_cases(void (*f)(UINT, CVisPoint, CVisPane *, void *), void *params=NULL);
    void PushEventRightButtonDblClk(void (*f)(UINT, CVisPoint, CVisPane *, void *), void *params=NULL);
    void PushEventRightButtonDblClk_c(void (*f)(UINT, CVisPoint, CVisPane *, void *), void *params=NULL);
    void PushEventRightButtonDblClk_s(void (*f)(UINT, CVisPoint, CVisPane *, void *), void *params=NULL);
    void PushEventRightButtonDblClk_cs(void (*f)(UINT, CVisPoint, CVisPane *, void *), void *params=NULL);
    void PushEventMouseMove_all_cases(void (*f)(UINT, CVisPoint, CVisPane *, void *), void *params=NULL);
    void PushEventMouseMoveR_all_cases(void (*f)(UINT, CVisPoint, CVisPane *, void *), void *params=NULL);
    void PushEventMouseMoveL_all_cases(void (*f)(UINT, CVisPoint, CVisPane *, void *), void *params=NULL);
    void PushEventMouseMove(void (*f)(UINT, CVisPoint, CVisPane *, void *), void *params=NULL);
    void PushEventMouseMove_c(void (*f)(UINT, CVisPoint, CVisPane *, void *), void *params=NULL);
    void PushEventMouseMove_s(void (*f)(UINT, CVisPoint, CVisPane *, void *), void *params=NULL);
    void PushEventMouseMove_cs(void (*f)(UINT, CVisPoint, CVisPane *, void *), void *params=NULL);
    void PushEventMouseMoveL(void (*f)(UINT, CVisPoint, CVisPane *, void *), void *params=NULL);
    void PushEventMouseMoveL_c(void (*f)(UINT, CVisPoint, CVisPane *, void *), void *params=NULL);
    void PushEventMouseMoveL_s(void (*f)(UINT, CVisPoint, CVisPane *, void *), void *params=NULL);
    void PushEventMouseMoveL_cs(void (*f)(UINT, CVisPoint, CVisPane *, void *), void *params=NULL);
    void PushEventMouseMoveR(void (*f)(UINT, CVisPoint, CVisPane *, void *), void *params=NULL);
    void PushEventMouseMoveR_c(void (*f)(UINT, CVisPoint, CVisPane *, void *), void *params=NULL);
    void PushEventMouseMoveR_s(void (*f)(UINT, CVisPoint, CVisPane *, void *), void *params=NULL);
    void PushEventMouseMoveR_cs(void (*f)(UINT, CVisPoint, CVisPane *, void *), void *params=NULL);

    void PopEventLeftButtonUp();
    void PopEventLeftButtonDown_all_cases();
    void PopEventLeftButtonDown();
    void PopEventLeftButtonDown_c();
    void PopEventLeftButtonDown_s();
    void PopEventLeftButtonDown_cs();
    void PopEventLeftButtonDblClk_all_cases();
    void PopEventLeftButtonDblClk();
    void PopEventLeftButtonDblClk_c();
    void PopEventLeftButtonDblClk_s();
    void PopEventLeftButtonDblClk_cs();
    void PopEventRightButtonUp();
    void PopEventRightButtonDown_all_cases();
    void PopEventRightButtonDown();
    void PopEventRightButtonDown_c();
    void PopEventRightButtonDown_s();
    void PopEventRightButtonDown_cs();
    void PopEventRightButtonDblClk_all_cases();
    void PopEventRightButtonDblClk();
    void PopEventRightButtonDblClk_c();
    void PopEventRightButtonDblClk_s();
    void PopEventRightButtonDblClk_cs();
    void PopEventMouseMove_all_cases();
    void PopEventMouseMoveR_all_cases();
    void PopEventMouseMoveL_all_cases();
    void PopEventMouseMove();
    void PopEventMouseMove_c();
    void PopEventMouseMove_s();
    void PopEventMouseMove_cs();
    void PopEventMouseMoveL();
    void PopEventMouseMoveL_c();
    void PopEventMouseMoveL_s();
    void PopEventMouseMoveL_cs();
    void PopEventMouseMoveR();
    void PopEventMouseMoveR_c();
    void PopEventMouseMoveR_s();
    void PopEventMouseMoveR_cs();
#endif // VIS_PANE_OLD_MOUSE_METHODS
};
    



// Helper structures used internally.
// (These need to be public so that they can be used in STL containers.)
enum EVisDraw
{
	evisdrawPoint,
	evisdrawLine
};


struct SVisDraw
{
	EVisDraw m_evisdraw;
	int m_wPenStyle;
	int m_wPenWidth;
	COLORREF m_colorref;
	CVisPoint m_point1;
	CVisPoint m_point2;
};


struct SVisMouseEvent
{
	CString m_string;
	void *m_pvUser;
	VisPfnMouseEventCallback m_pfnCallback;
	SVisMouseEvent *m_pmouseeventNext;
};


////////////////////////////////////////////////////////////////////////////
//  
// @class ***
// @base public | CWnd
//
// This is the base class for reference-counted CWnd-derived classes used
// in the SDK.  The reference counting helps us prevent memory leaks.
//
////////////////////////////////////////////////////////////////////////////
// LATER:  Only export methods that need to be exported.
class VisDisplayExport CVisRefCntWnd : public CWnd
{
// @access Public members
public:
    //------------------------------------------------------------------
	// @group Constructors and destructor

    // @cmember
	// Constructor.  This does not create a window.  The
	// CreateWnd method must be called to create the window for the pane.
	CVisRefCntWnd(bool fAutoDestroy = true);

    // @cmember
	// Destructor.
	virtual ~CVisRefCntWnd(void);

    //------------------------------------------------------------------
	// @group Reference counting
    
	unsigned long AddRef(void);

	unsigned long Release(void);


    //------------------------------------------------------------------
	// @group Attributes

	bool HasWnd(void) const;

	const char *Title(void);
	void SetTitle(const char *szTitle);

	bool AutoDestroy(void) const;
	void SetAutoDestroy(bool fAutoDestroy = true);

	bool HasRefreshed(void) const;


    //------------------------------------------------------------------
	// @group Window creation and destruction
    
	BOOL ShowWnd(int nCmdShow = SW_SHOWNA);

	void DestroyWnd(void);

    // @cmember
	// This function is called when we attempt to create the window.
    virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

    // @cmember
	// This function is called when the window is destroyed (or when
	// an attempt to create a window fails).
	virtual void PostNcDestroy(void);

    //------------------------------------------------------------------
	// @group Refreshing the window
    
    // @cmember
	// If fUpdateNow is false, a WM_PAINT message will be posted to the
	// window's message queue and this method will return before the
	// window has been redrawn.  If fUpdateNow is true, this method will
	// not return until the window has been redrawn.
    virtual void Refresh(bool fUpdateNow = false);



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisRefCntWnd)
	//}}AFX_VIRTUAL


protected:
	void SetHasRefreshed(bool fHasRefreshed);


private:
	unsigned long m_cRef;
	std::string m_strTitle;
	bool m_fAutoDestroy;
	bool m_fHasRefreshed;
    // Generated message map functions


    // Generated message map functions
protected:
	//{{AFX_MSG(CVisRefCntWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


////////////////////////////////////////////////////////////////////////////
//  
// @class ***
// @base public | CVisRefCntWnd
//
//
// This base class is used with the CVisPaneWnd and CVisPaneArrayWnd classes.
// It help keep the inner and outer window dimensions in synch when they are
// changed and when window styles are changed.
//
////////////////////////////////////////////////////////////////////////////
// LATER:  Only export methods that need to be exported.
class VisDisplayExport CVisPaneWndBase : public CVisRefCntWnd
{
// @access Public members
public:
    //------------------------------------------------------------------
	// @group Destructor

    // @cmember
	// Destructor.
	virtual ~CVisPaneWndBase(void);


// Attributes
public:
    //------------------------------------------------------------------
	// @group Attributes

	int XOffsetInParent(void) const;
	void SetXOffsetInParent(int x, bool fRepaint = true);

	int YOffsetInParent(void) const;
	void SetYOffsetInParent(int y, bool fRepaint = true);

	CVisPoint PointOffsetInParent(void) const;
	void SetPointOffsetInParent(POINT point, bool fRepaint = true);

	int WidthInside(void) const;
	void SetWidthInside(int dx, bool fRepaint = true);

	int WidthOutside(void) const;
	void SetWidthOutside(int dx, bool fRepaint = true);

	int WidthBorder(void) const;
	void SetWidthBorder(int dx, bool fRepaint = true);

	int HeightInside(void) const;
	void SetHeightInside(int dy, bool fRepaint = true);

	int HeightOutside(void) const;
	void SetHeightOutside(int dy, bool fRepaint = true);

	CSize SizeInside(void) const;
	void SetSizeInside(int dx, int dy, bool fRepaint = true);
	void SetSizeInside(SIZE size, bool fRepaint = true);

	CSize SizeOutside(void) const;
	void SetSizeOutside(int dx, int dy, bool fRepaint = true);
	void SetSizeOutside(SIZE size, bool fRepaint = true);

	CSize SizeInOutDifference(void) const;

	CVisRect RectOutside(void) const;
	void SetRectOutside(const RECT& refrect, bool fRepaint = true);

	DWORD Style(void) const;
	void SetStyle(DWORD dw);

	DWORD ExStyle(void) const;
	void SetExStyle(DWORD dw);

	COLORREF ColorrefBackground(void) const;
	void SetColorrefBackground(COLORREF colorref);

	const CWnd *PWndParent(void) const;
	CWnd *PWndParent(void);

	bool UseStatusBar(void) const;
	void SetUseStatusBar(bool fUseStatusBar);

	const CStatusBar& StatusBar(void) const {return m_statusbar;};
	CStatusBar& StatusBar(void) {return m_statusbar;};

	// If this pane is part of an array of panes?  If so, its parent is
	// really a CVisPaneArrayWnd.
	bool IsPartOfArray(void) const;
	bool IsPaneWnd(void) const;
	bool IsPaneArrayWnd(void) const;
	int IndexInParent(void) const;

	int EvispaneOptions(void) const;

	void SetMessageText(const char *szMessage = 0);


    //------------------------------------------------------------------
	// @group Window creation and destruction

	BOOL CreateWnd(int evispane = evispaneUnknown, CWnd *pwndParent = 0,
			bool fChangeStyles = true);

    // @cmember
	// This function is called when the window is destroyed (or when
	// an attempt to create a window fails).
	virtual void PostNcDestroy(void);


    //------------------------------------------------------------------
	// @group Refreshing the window
    
    // @cmember
	// If fUpdateNow is false, a WM_PAINT message will be posted to the
	// window's message queue and this method will return before the
	// window has been redrawn.  If fUpdateNow is true, this method will
	// not return until the window has been redrawn.
    virtual void Refresh(bool fUpdateNow = false);


    //------------------------------------------------------------------
	// @group Thread used with this window

	bool HasPaneThread(void) const;
	const CVisPaneThread& PaneThread(void) const;
	CVisPaneThread& PaneThread(void);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisPaneWndBase)
	//}}AFX_VIRTUAL


protected:
	// friend BOOL CVisPaneThread::InitInstance(void);
	friend void CVisPane::SetPWndParent(CWnd *pWndParent);
	friend void CVisPane::SetIsPartOfArray(bool fIsPartOfArray);
	friend void CVisPane::SetIndexInParent(int m_ipaneInParent);
	friend void CVisPane::SetEvispaneOptions(int evispane,
			bool fChangeStyles);
	friend CVisPaneThread;

    // @cmember
	CVisPaneWndBase(int evispane, bool fIsPaneArrayWnd,
			bool fAutoDestroy = true, bool fUseStatusBar = false);

	// Default border width
	enum { edxBorderDefault = 1 };

	bool StyleChangesSizeInside(void) const;

	void SetPWndParent(CWnd *pWndParent);

	void SetIsPartOfArray(bool fIsPartOfArray);
	void SetIndexInParent(int ipaneInParent);

	virtual void SetEvispaneOptions(int evispane, bool fChangeStyles = true);

	// The entire client area.  This assumes that a window is up.
	CVisRect RectClient(void) const;

	// The location of the status bar in the client area.
	// This assumes that a window is up.
	CVisRect RectClientStatusBar(void) const;
	CVisRect RectClientStatusBar(const CVisRect& refrectClient) const;

	// The location of the image in the client area.
	// This assumes that a window is up.
	CVisRect RectClientInside(void) const;
	CVisRect RectClientInside(const CVisRect& refrectClient) const;
	CVisPoint PointTopLeftClientInside(void) const;

	// The location of the border in the client area.
	// This assumes that a window is up.
	CVisRect RectClientBorder(void) const;
	CVisRect RectClientBorder(const CVisRect& refrectClient) const;

	virtual BOOL RealCreateWnd(const char *szCursor, bool fNewThread = false);

	void DrawBorders(CDC& dc, bool fSolidBgColor = false,
				bool fSelected = false) const;
	void FillBackgroundBetweenRects(CDC& dc, const RECT& rectInner,
				const RECT& rectOuter) const;
	void DrawTextInDcRect(CDC& dc, const RECT& rectText, const char *szText,
			bool fFillBackground = true) const;

	static int DyStatusBar(void);


private:
	CVisRect m_rectInParent;
	int m_dxBorder;
	int m_evispane;
	DWORD m_dwStyle;
	DWORD m_dwExStyle;
	COLORREF m_colorrefBackground;
	CWnd *m_pWndParent;
	int m_ipaneInParent;
	CVisPaneThread *m_ppanethread;
	CStatusBar m_statusbar;
	bool m_fUseStatusBar;  // LATER:  Only use the flag in the pane options?
	bool m_fIsPaneArrayWnd;
	bool m_fIsPartOfArray;

	static int s_dyStatusBar;

#ifdef _DEBUG
	void CheckRectInParent();
#endif // _DEBUG

	void FindRectInParent(RECT& refrect) const;
	void FindRectInParent(void);
	void FindDwStyle(void);
	void FindDwExStyle(void);
	void FindTitle(void);

	void FindStyles(int evispane);

#ifdef VIS_DISPLAY_LIB
	HICON LoadDisplayIcon() const;
#endif // VIS_DISPLAY_LIB


    // Generated message map functions
protected:
	//{{AFX_MSG(CVisPaneWndBase)
	afx_msg int OnDestroy();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CVisPoint point);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CVisPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


////////////////////////////////////////////////////////////////////////////
//  
// @class ***
// @base public | CVisPaneWndBase
//
////////////////////////////////////////////////////////////////////////////
// LATER:  Only export methods that need to be exported.
class VisDisplayExport CVisPaneWnd : public CVisPaneWndBase
{
// @access Public members
public:
    //------------------------------------------------------------------
	// @group Constructors and destructor

    // @cmember
	// Constructor.  This does not create a window.  The
	// CreateWnd method must be called to create the window for the pane.
	CVisPaneWnd(int evispane = evispaneDefault, CWnd *pwndParent = 0);

    // @cmember
	// Destructor.
	virtual ~CVisPaneWnd(void);


// Attributes
public:
    //------------------------------------------------------------------
	// @group Attributes

	CVisPoint ImageOffset(void) const;
    void SetImageOffset(int dx, int dy, bool fRefresh = true);
    void SetImageOffset(POINT point, bool fRefresh = true);

    bool IsSelected(void) const;
    void SetIsSelected(bool fIsSelected, bool fUpdateNow = false);

    bool HasValidImage(void) const;
	const CVisImageBase& Image(void) const;  // LATER:  Return a CVisImageBase object?
	const CVisImageBase& ScreenImage(void) const;
    bool CouldNotDisplayImage(void) const;


// Operations
public:
    //------------------------------------------------------------------
	// @group Displaying the image

	// LATER:  Stretch and resize display options?

    // @cmember ***
    void Clear(bool fRemovePointsAndLines = true,
			bool fUpdateNow = false);

    // @cmember ***
    void Clear(COLORREF colorref, bool fRemovePointsAndLines = true,
			bool fUpdateNow = false);

	void Display(const CVisImageBase& refimage,
			EVisPaneDisp evispanedisp = evispanedispDefault,
			bool fUpdateNow = false, bool fCreateWndIfNeeded = true);

	void Display(const CVisImageBase& refimage, POINT pointImageOffset,
			EVisPaneDisp evispanedisp = evispanedispDefault,
			bool fUpdateNow = false, bool fCreateWndIfNeeded = true);

	void Display(EVisNormalize evisnormalize, const CVisImageBase& refimage,
			EVisPaneDisp evispanedisp = evispanedispDefault,
			bool fUpdateNow = false, bool fCreateWndIfNeeded = true);

	void Display(EVisNormalize evisnormalize, const CVisImageBase& refimage,
			POINT pointImageOffset,
			EVisPaneDisp evispanedisp = evispanedispDefault,
			bool fUpdateNow = false, bool fCreateWndIfNeeded = true);

	void Display(EVisNormalize evisnormalize, double dblMin, double dblMax,
			const CVisImageBase& refimage,
			EVisPaneDisp evispanedisp = evispanedispDefault,
			bool fUpdateNow = false, bool fCreateWndIfNeeded = true);

	void Display(EVisNormalize evisnormalize, double dblMin, double dblMax,
			const CVisImageBase& refimage, POINT pointImageOffset,
			EVisPaneDisp evispanedisp = evispanedispDefault,
			bool fUpdateNow = false, bool fCreateWndIfNeeded = true);


    //------------------------------------------------------------------
	// @group Adding points and lines to the image

    // @cmember ***
    void AddPoint(int x, int y, int nPenStyle = PS_SOLID,
			int dxThickness = 1, COLORREF colorref = RGB(255, 0, 0),
			bool fRefresh = true);

    // @cmember ***
    void AddLine(int x1, int y1, int x2, int y2, int nPenStyle = PS_SOLID,
			int dxThickness = 1, COLORREF colorref = RGB(255, 0, 0),
			bool fRefresh = true);

    // @cmember ***
    void DrawLine(int x1, int y1, int x2, int y2, int nPenStyle = PS_SOLID,
			int dxThickness = 1, COLORREF colorref = RGB(255, 0, 0));

    // @cmember ***
    void RemovePoint(int x, int y, bool fRefresh = true);

    // @cmember ***
    void RemoveLine(int x1, int y1, int x2, int y2, bool fRefresh = true);

    // @cmember ***
    void RemoveAllPointsAndLines(bool fRefresh = true);


    //------------------------------------------------------------------
	// @group Mouse event callback functions

	// @cmember
	// Add a callback function to handle the specified event.
	void AddMouseEventCallback(int evismouse,
			VisPfnMouseEventCallback pfnCallback, const char *sz = 0,
			void *pvUser = 0);

	// @cmember
	// Remove a callback function to handle the specified event.
	void RemoveMouseEventCallback(int evismouse,
			VisPfnMouseEventCallback pfnCallback, void *pvUser = 0);

	void RemoveMouseEventCallback(VisPfnMouseEventCallback pfnCallback,
			void *pvUser = 0);


    //------------------------------------------------------------------
	// @group Standard mouse event callback functions

// UNDONE:  This is a temporary workaround.
#if VIS_USER_BUILD_mturk

	static void VISAPI SelectPaneCallback(UINT nFlags, CPoint point,
			CVisPaneWnd *ppanewnd, void *pvUser, bool fOutside = false);

	static void VISAPI ShowPixelValuesCallback(UINT nFlags, CPoint point,
			CVisPaneWnd *ppanewnd, void *pvUser, bool fOutside = false);

#else // VIS_USER_BUILD_mturk

	static void VISAPI SelectPaneCallback(UINT nFlags, CPoint point,
			CVisPaneWnd *ppanewnd, void *pvUser);

	static void VISAPI ShowPixelValuesCallback(UINT nFlags, CPoint point,
			CVisPaneWnd *ppanewnd, void *pvUser);

#endif // VIS_USER_BUILD_mturk

	void SetMirrorImage(bool fMirror);
	bool GetMirrorImage() const;

	// LATER:  Add keyboard input events?


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisPaneWnd)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	//}}AFX_VIRTUAL


protected:
	friend void CVisPane::SetEvispaneOptions(int evispane,
			bool fChangeStyles);

	virtual void SetEvispaneOptions(int evispane, bool fChangeStyles = true);


private:
	// Possible combinations:
	//   { down, up, dblclick } x { L, R } x { any, none, ctrl, shift, ctrl-shift }
	//   { move } x { any, none, L, R, both } x { any, none, ctrl, shift, ctrl-shift }
	enum { ecMouseEvent = 55 };


	CVisImageBase *m_pimageOriginal;
	CVisImageBase *m_pimageScreen;
	CVisPoint m_pointImageOffset;
	EVisNormalize m_evisnormalize;
	double m_dblMin;
	double m_dblMax;
	int m_evismouseModifierKeysCur;
	std::deque<SVisDraw> m_dequeDraw;
	SVisMouseEvent *m_rgpMouseEvent[ecMouseEvent];
	bool m_fIsSelected;
	bool m_fUseScrnBuf;
	bool m_fCouldNotDisplayImage;


	void DrawImageAndPointsAndLines(CDC& dc);
	void DrawPointsAndLines(CDC& dc, CVisPoint pointOffset = CVisPoint(0, 0));

	// All modification of the image variables should go through these methods
	// so that it is properly synchronized.
	void SetOriginalImage(CVisImageBase *pimage);
	void SetScreenImage(CVisImageBase *pimage);
	void DeleteOriginalImage(void);
	void DeleteScreenImage(void);

	void FindImageRects(CVisRect& rectSrc, CVisRect& rectDest);

	void OnMouseEvent(int evismouse, UINT nFlags, CVisPoint point);

    void FindButtonMessage(EVisMouse evismouseButton,
			CString& refstringMessage) const;
    void ShowEventHandlerMessage(void);

	static bool FCanDirectlyDisplayImage(const CVisImageBase& refimage);

	static int IGetMouse(int evismouse);
	static EVisMouse MouseModifierKeysGetNFlags(UINT nFlags,
			bool fCheckButton);

	bool m_mirrorImage; // If true, display the image left-right mirrored

    // Generated message map functions
protected:
	//{{AFX_MSG(CVisPaneWnd)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CVisPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CVisPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CVisPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CVisPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CVisPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CVisPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CVisPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



/////////////////////////////////////////////////////////////////////////////
// CVisPaneThread thread

class CVisPaneThread : public CWinThread
{
	DECLARE_DYNCREATE(CVisPaneThread)
// Attributes
public:
	void Init(CVisPaneWndBase *ppanewndbase, const char *szCursor);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisPaneThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL


protected:
	friend CVisPaneWndBase::~CVisPaneWndBase(void);

	CVisPaneThread();           // protected constructor used by dynamic creation
	virtual ~CVisPaneThread();


private:
	HANDLE m_heventInitialized;
	CVisPaneWndBase *m_ppanewndbase;
	const char *m_szCursor;


protected:
	// Generated message map functions
	//{{AFX_MSG(CVisPaneThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////
//  CVisDisplayPyramid
//	Temporary class used to display pyramids.  CVisPane should be modified
//	to accept pyramids. (TODO)
//  This code is meant to be a temporary fix......... (mturk 1/31/00)
//
////////////////////////////////////////////////////////////////////////////

template <class T1, class T2> class CVisPyramid; // defined in VisImageProc project
class CVisDisplayPyramid
{
public:
	template <class T1, class T2>
	CVisDisplayPyramid(CVisPyramid<T1, T2>& pyr_image, bool separate_thread = false)
	{
		CVisDisplayPyramidInit();
		Setup(pyr_image, separate_thread);
		Display(pyr_image);
	}
	CVisDisplayPyramid(int height, int width, int levels, bool separate_thread = false)
	{
		CVisDisplayPyramidInit();
		Setup(height, width, levels, separate_thread);
	}

	~CVisDisplayPyramid()
	{
		m_panes[0].PWnd()->DestroyWindow();
		delete [] m_panes;
	}

	CVisPane Pane(int i) { assert( i >= 0 && i < m_numpanes ); return m_panes[i]; }

	template <class T1, class T2>
	void Display(CVisPyramid<T1, T2>& pyr_image)
	{
		m_panes[0].Display(pyr_image[0]);
		int new_w = pyr_image[0].Width();
		for (int i=1; i<pyr_image.NLevels(); i++)
		{
			new_w += pyr_image[i].Width();
			m_panes[i].Display(pyr_image[i]);
		}
	}

protected:
	void CVisDisplayPyramidInit()
	{
		m_panes = NULL;
		m_numpanes = 0;
	}

	template <class T1, class T2>
	void Setup(CVisPyramid<T1, T2>& pyr_image, bool separate_thread = false)
	{
		Setup(pyr_image[0].Width(), pyr_image[0].Height(), pyr_image.NLevels(), separate_thread);
	}

	void Setup(int width, int height, int nlevels = 0, bool separate_thread = false)
	{
		bool keepfirstpane = false;
		assert(nlevels > 0);

		m_panes = new CVisPane [nlevels];
		int w = 0;
		for (int i=0; i<nlevels; i++)
		{
			int test = pow(2,i);
			w += width/(test);
		}
		
		int panetype = evispaneSelectHandler|evispaneStatusDefault|evispaneShowValues|evispaneShowValues|evispanePaneBorder|evispanePaneTitle|evispaneVisible;
		if (separate_thread) panetype |= evispaneOwnThread;
		CVisPane pyrpane(w, height, "", panetype);
		m_panes[0] = pyrpane;
		int new_w = width;
		for (i=1; i<nlevels; i++)
		{
			int panetype = evispaneSelectHandler|evispaneStatusDefault|evispaneShowValues|evispaneShowValues|evispaneVisible;
			if (separate_thread) panetype |= evispaneOwnThread;
			CVisPane pane(CRect(new_w, 0, new_w + width/pow(2,i), height), "", panetype, pyrpane.PWnd());
			new_w += width/pow(2,i);
			m_panes[i] = pane;
		}
		m_numpanes = nlevels;
	}

protected:
	CVisPane *m_panes;
	int m_numpanes;
};

#include "VisPane.inl"


#endif //#ifndef __VIS_PANE_H__

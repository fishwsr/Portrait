; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPortraitEditingDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Portrait Editing.h"

ClassCount=4
Class1=CPortraitEditingApp
Class2=CPortraitEditingDlg
Class3=CAboutDlg

ResourceCount=6
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_PORTRAITEDITING_DIALOG
Resource4=IDD_CROSSSHOW
Resource5=IDD_ABOUTBOX (English (U.S.))
Class4=CcrossShow
Resource6=IDD_PORTRAITEDITING_DIALOG (English (U.S.))

[CLS:CPortraitEditingApp]
Type=0
HeaderFile=Portrait Editing.h
ImplementationFile=Portrait Editing.cpp
Filter=N

[CLS:CPortraitEditingDlg]
Type=0
HeaderFile=Portrait EditingDlg.h
ImplementationFile=Portrait EditingDlg.cpp
Filter=W
LastObject=IDC_BUTTON2
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=Portrait EditingDlg.h
ImplementationFile=Portrait EditingDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg


[DLG:IDD_PORTRAITEDITING_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CPortraitEditingDlg

[DLG:IDD_PORTRAITEDITING_DIALOG (English (U.S.))]
Type=1
Class=CPortraitEditingDlg
ControlCount=43
Control1=IDC_LIST1,SysListView32,1350631424
Control2=IDC_SOURCE_IMG,button,1342177287
Control3=IDC_TARGET_IMG,button,1342177287
Control4=IDC_OPEN_IMAGE,button,1342243072
Control5=IDC_FEATURE_LOCAL,button,1342243072
Control6=IDC_DETECT_BACKGROUND,button,1342243072
Control7=IDC_DETECT_SKIN,button,1342243072
Control8=IDC_SKIN_COLOR,button,1342243072
Control9=IDC_Hair_COLOR,button,1342243072
Control10=IDC_CLOTH_COLOR,button,1342243072
Control11=IDC_BACKGROUND_COLOR,button,1342243072
Control12=IDC_SHADOW_COLOR,button,1342243072
Control13=IDC_Default_COLOR,button,1342243072
Control14=IDC_GET_COLOR,button,1342242816
Control15=IDC_BRUSH_COLOR,button,1342242816
Control16=IDC_DETECT_HAIR,button,1342243072
Control17=IDC_DETECT_COLLAR,button,1342243072
Control18=IDC_CROSS_STITCH,button,1342243072
Control19=IDC_CARTOON,button,1342243072
Control20=IDC_SAVE,button,1342242816
Control21=IDC_EXIT,button,1342242816
Control22=IDC_SCISSORS,button,1342243072
Control23=IDC_HAIR,button,1342308361
Control24=IDC_EYEBROW,button,1342177289
Control25=IDC_EYE,button,1342177289
Control26=IDC_MOUTH,button,1342177289
Control27=IDC_NOSE,button,1342177289
Control28=IDC_COLLAR,button,1342177289
Control29=IDC_BACKGROUND,button,1342177289
Control30=IDC_AUTORENDER,button,1342243072
Control31=IDC_EFFECT1,button,1342177289
Control32=IDC_EFFECT2,button,1342177289
Control33=IDC_EFFECT3,button,1342177289
Control34=IDC_FORE,button,1342177289
Control35=IDC_BACKG,button,1342177289
Control36=IDC_GCUT,button,1342242816
Control37=IDC_CONSTRAIN_ASM,button,1342242816
Control38=IDC_DETECT_EAR,button,1342242816
Control39=IDC_EAR,button,1342177289
Control40=IDC_FACECONTOUR,button,1342177289
Control41=IDC_REDO,button,1342242816
Control42=IDC_UNDO,button,1342242816
Control43=IDC_RESET,button,1342242816

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_CROSSSHOW]
Type=1
Class=CcrossShow
ControlCount=1
Control1=IDC_CROSS_IMG,button,1342177287

[CLS:CcrossShow]
Type=0
HeaderFile=crossShow.h
ImplementationFile=crossShow.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_SCROLLBAR1
VirtualFilter=dWC


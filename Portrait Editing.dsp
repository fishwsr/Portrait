# Microsoft Developer Studio Project File - Name="Portrait Editing" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Portrait Editing - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Portrait Editing.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Portrait Editing.mak" CFG="Portrait Editing - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Portrait Editing - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Portrait Editing - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Portrait Editing - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".\include" /I ".\gdiplus\sdkinc" /I ".\vsdk\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /Zm500 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 fll.LIB FaceDectAll.LIB ijl15.LIB cvd.lib cv.lib highguid.lib highgui.lib cxcored.lib cxcore.lib VisCore.lib VisDisplay.lib VisImageProc.lib VisMatrix.lib BaseModel.lib CSBase.lib ASMModel.lib gdiplus.lib ShapeContext.lib /nologo /subsystem:windows /machine:I386 /libpath:".\lib"

!ELSEIF  "$(CFG)" == "Portrait Editing - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".\include" /I ".\gdiplus\sdkinc" /I ".\vsdk\inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /Zm500 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 fll.LIB FaceDectAll.LIB ijl15.LIB cvd.lib cv.lib highguid.lib highgui.lib cxcored.lib cxcore.lib VisCoreDB.lib VisDisplayDB.lib VisImageProcDB.lib VisMatrixDB.lib BaseModelD.lib CSBaseD.lib ASMModelD.lib gdiplus.lib ShapeContextDB.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:".\lib"

!ENDIF 

# Begin Target

# Name "Portrait Editing - Win32 Release"
# Name "Portrait Editing - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BCMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\BLOWFISH.CPP
# End Source File
# Begin Source File

SOURCE=.\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\Common.cpp
# End Source File
# Begin Source File

SOURCE=.\crossShow.cpp
# End Source File
# Begin Source File

SOURCE=.\renderCurve\cubic.cpp
# End Source File
# Begin Source File

SOURCE=.\renderCurve\curvemorphing.cpp
# End Source File
# Begin Source File

SOURCE=.\FileTool.cpp
# End Source File
# Begin Source File

SOURCE=.\Gc\GCsegmentation.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\Gc\graph.cpp
# End Source File
# Begin Source File

SOURCE=.\Image.cpp
# End Source File
# Begin Source File

SOURCE=.\Gc\maxflow.cpp
# End Source File
# Begin Source File

SOURCE=.\meanshift\ms.cpp
# End Source File
# Begin Source File

SOURCE=.\meanshift\msImageProcessor.cpp
# End Source File
# Begin Source File

SOURCE=.\meanshift\msSys.cpp
# End Source File
# Begin Source File

SOURCE=.\renderCurve\MyPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\renderCurve\NatCubic.cpp
# End Source File
# Begin Source File

SOURCE=".\Portrait Editing.cpp"
# End Source File
# Begin Source File

SOURCE=".\Portrait Editing.rc"
# End Source File
# Begin Source File

SOURCE=".\Portrait EditingDlg.cpp"
# End Source File
# Begin Source File

SOURCE=.\meanshift\RAList.cpp
# End Source File
# Begin Source File

SOURCE=.\meanshift\rlist.cpp
# End Source File
# Begin Source File

SOURCE=.\SCEnergy.cpp
# End Source File
# Begin Source File

SOURCE=.\Gc\skClustering.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\utility.cpp
# End Source File
# Begin Source File

SOURCE=.\XML\XmlFile.cpp
# End Source File
# Begin Source File

SOURCE=.\XML\xmlParserSimple.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BCMenu.h
# End Source File
# Begin Source File

SOURCE=.\Gc\block.h
# End Source File
# Begin Source File

SOURCE=.\BLOWFISH.H
# End Source File
# Begin Source File

SOURCE=.\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\CImg.h
# End Source File
# Begin Source File

SOURCE=.\Common.h
# End Source File
# Begin Source File

SOURCE=.\crossShow.h
# End Source File
# Begin Source File

SOURCE=.\renderCurve\cubic.h
# End Source File
# Begin Source File

SOURCE=.\renderCurve\curvemorphing.h
# End Source File
# Begin Source File

SOURCE=.\DLL_ShapeContext.h
# End Source File
# Begin Source File

SOURCE=.\FaceDefine.h
# End Source File
# Begin Source File

SOURCE=.\fdapi.h
# End Source File
# Begin Source File

SOURCE=.\FileTool.h
# End Source File
# Begin Source File

SOURCE=.\fllapi.h
# End Source File
# Begin Source File

SOURCE=.\Gc\GCsegmentation.h
# End Source File
# Begin Source File

SOURCE=.\GlobalFunc.h
# End Source File
# Begin Source File

SOURCE=.\Gc\graph.h
# End Source File
# Begin Source File

SOURCE=.\Image.h
# End Source File
# Begin Source File

SOURCE=.\meanshift\ms.h
# End Source File
# Begin Source File

SOURCE=.\meanshift\msImageProcessor.h
# End Source File
# Begin Source File

SOURCE=.\meanshift\msSys.h
# End Source File
# Begin Source File

SOURCE=.\renderCurve\MyPoint.h
# End Source File
# Begin Source File

SOURCE=.\renderCurve\NatCubic.h
# End Source File
# Begin Source File

SOURCE=".\Portrait Editing.h"
# End Source File
# Begin Source File

SOURCE=".\Portrait EditingDlg.h"
# End Source File
# Begin Source File

SOURCE=.\PreDefine.h
# End Source File
# Begin Source File

SOURCE=.\meanshift\RAList.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\meanshift\rlist.h
# End Source File
# Begin Source File

SOURCE=.\SCEnergy.h
# End Source File
# Begin Source File

SOURCE=.\Gc\skClustering.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\meanshift\tdef.h
# End Source File
# Begin Source File

SOURCE=.\utility.h
# End Source File
# Begin Source File

SOURCE=.\XML\XmlFile.h
# End Source File
# Begin Source File

SOURCE=.\XML\xmlParserSimple.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\background.BMP
# End Source File
# Begin Source File

SOURCE=.\res\background1.BMP
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\brush.BMP
# End Source File
# Begin Source File

SOURCE=.\res\bt.BMP
# End Source File
# Begin Source File

SOURCE=.\res\color.BMP
# End Source File
# Begin Source File

SOURCE=.\res\logo.BMP
# End Source File
# Begin Source File

SOURCE=".\res\Portrait Editing.ico"
# End Source File
# Begin Source File

SOURCE=".\res\Portrait Editing.rc2"
# End Source File
# Begin Source File

SOURCE=.\res\straw.BMP
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

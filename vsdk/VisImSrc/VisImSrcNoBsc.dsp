# Microsoft Developer Studio Project File - Name="VisImSrcNoBsc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=VisImSrcNoBsc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VisImSrc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VisImSrc.mak" CFG="VisImSrc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VisImSrc - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "VisImSrc - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/BetaVisSDK/VisImSrc", UQFAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VisImSrcNoBsc - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Lib"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "VIS_IMSRC_DLL" /YX"VisImSrcDLL" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /base:"0x2b000000" /subsystem:windows /dll /pdb:"..\bin/VisImSrc.pdb" /machine:I386 /nodefaultlib:"nafxcw.lib" /out:"..\bin\VisImSrc.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "VisImSrcNoBsc - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Lib"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "VIS_IMSRC_DLL" /YX"VisImSrcDLL" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /base:"0x2b000000" /subsystem:windows /dll /incremental:no /pdb:"..\bin/VisImSrcDB.pdb" /debug /machine:I386 /nodefaultlib:"nafxcw.lib" /out:"..\bin\VisImSrcDB.dll"
# SUBTRACT LINK32 /pdb:none /pdbtype:<none>

!ENDIF 

# Begin Target

# Name "VisImSrcNoBsc - Win32 Release"
# Name "VisImSrcNoBsc - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\VisConvertCbitToCbit.cpp
# End Source File
# Begin Source File

SOURCE=.\VisConvertRGB.cpp
# End Source File
# Begin Source File

SOURCE=.\VisConvertYUV.cpp
# End Source File
# Begin Source File

SOURCE=.\VisDlgListSz.cpp
# End Source File
# Begin Source File

SOURCE=.\VisImageFromImSrcFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\VisImageSource.cpp
# End Source File
# Begin Source File

SOURCE=.\VisImSrc.rc
# End Source File
# Begin Source File

SOURCE=.\VisImSrcDLL.cpp
# End Source File
# Begin Source File

SOURCE=.\VisImSrcGUIDs.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\VisSequenceList.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\VisConvertCbitToCbit.h
# End Source File
# Begin Source File

SOURCE=.\VisConvertCbitToCbit.inl
# End Source File
# Begin Source File

SOURCE=.\VisConvertRGB.h
# End Source File
# Begin Source File

SOURCE=.\VisConvertRGB.inl
# End Source File
# Begin Source File

SOURCE=.\VisConvertYUV.h
# End Source File
# Begin Source File

SOURCE=.\VisConvertYUV.inl
# End Source File
# Begin Source File

SOURCE=.\VisDlgListSz.h
# End Source File
# Begin Source File

SOURCE=.\VisImageFromImSrcFrame.h
# End Source File
# Begin Source File

SOURCE=.\VisImageFromImSrcFrame.inl
# End Source File
# Begin Source File

SOURCE=.\VisImagePump.h
# End Source File
# Begin Source File

SOURCE=.\VisImagePump.inl
# End Source File
# Begin Source File

SOURCE=.\VisImageSource.h
# End Source File
# Begin Source File

SOURCE=.\VisImageSource.inl
# End Source File
# Begin Source File

SOURCE=.\VisImEncoding.h
# End Source File
# Begin Source File

SOURCE=.\VisImEncoding.inl
# End Source File
# Begin Source File

SOURCE=.\VisImSrcConst.h
# End Source File
# Begin Source File

SOURCE=.\VisImSrcDLL.h
# End Source File
# Begin Source File

SOURCE=.\VisImSrcIFace.h
# End Source File
# Begin Source File

SOURCE=.\VisImSrcNoMFC.h
# End Source File
# Begin Source File

SOURCE=.\VisImSrcNoMFC.inl
# End Source File
# Begin Source File

SOURCE=.\VisImSrcVersion.h
# End Source File
# Begin Source File

SOURCE=.\VisListSz.h
# End Source File
# Begin Source File

SOURCE=.\VisListSz.inl
# End Source File
# Begin Source File

SOURCE=.\VisOLEHelp.h
# End Source File
# Begin Source File

SOURCE=.\VisRegHelp.h
# End Source File
# Begin Source File

SOURCE=.\VisRegHelp.inl
# End Source File
# Begin Source File

SOURCE=..\VisCore\VisSequence.inl
# End Source File
# Begin Source File

SOURCE=.\VisSequenceList.h
# End Source File
# Begin Source File

SOURCE=.\VisSequenceList.inl
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\VisImSrc.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

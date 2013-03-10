# Microsoft Developer Studio Project File - Name="VisCoreNoBsc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=VisCoreNoBsc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VisCoreNoBsc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VisCoreNoBsc.mak" CFG="VisCoreNoBsc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VisCoreNoBsc - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "VisCoreNoBsc - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/BetaVisSDK", HCBAAAAA"
# PROP Scc_LocalPath ".."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VisCoreNoBsc - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "VisCore_"
# PROP BASE Intermediate_Dir "VisCore_"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Lib"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "VIS_CORE_DLL" /D "VIS_INCLUDE_SDSTREAM" /YX"VisCoreDLL.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 vfw32.lib /nologo /base:"0x64000000" /subsystem:windows /dll /pdb:"..\bin/VisCore.pdb" /machine:I386 /out:"..\bin\VisCore.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "VisCoreNoBsc - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "VisCore0"
# PROP BASE Intermediate_Dir "VisCore0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Lib"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "VIS_CORE_DLL" /D "VIS_INCLUDE_SDSTREAM" /YX"VisCoreDLL.h" /FD /c
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
# ADD LINK32 vfw32.lib DXGUID.LIB ddraw.lib AMSTRMID.LIB STRMBASD.LIB /nologo /base:"0x64000000" /subsystem:windows /dll /incremental:no /pdb:"..\bin/VisCoreDB.pdb" /debug /machine:I386 /out:"..\bin\VisCoreDB.dll"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "VisCoreNoBsc - Win32 Release"
# Name "VisCoreNoBsc - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\VisAVIStreamIO.cpp
# End Source File
# Begin Source File

SOURCE=.\VisCopyGrayPixels.cpp
# End Source File
# Begin Source File

SOURCE=.\VisCopyStandardPixels.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\VisCore.rc
# End Source File
# Begin Source File

SOURCE=.\VisCoreDLL.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\VisDiagnostics.cpp
# End Source File
# Begin Source File

SOURCE=.\VisDibFileIO.cpp
# End Source File
# Begin Source File

SOURCE=.\VisDim.cpp
# End Source File
# Begin Source File

SOURCE=.\VisDShowStreamIO.cpp
# End Source File
# Begin Source File

SOURCE=.\VisEnumPixel.cpp
# End Source File
# Begin Source File

SOURCE=.\VisError.cpp
# End Source File
# Begin Source File

SOURCE=.\VisFileHandlerMagick.cpp
# End Source File
# Begin Source File

SOURCE=.\VisFileIO.cpp
# End Source File
# Begin Source File

SOURCE=.\VisImageBase.cpp
# End Source File
# Begin Source File

SOURCE=.\VisImageWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\VisMemBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\VisMSVFileIO.cpp
# End Source File
# Begin Source File

SOURCE=.\VisPPMFileIO.cpp
# End Source File
# Begin Source File

SOURCE=.\VisProp.cpp
# End Source File
# Begin Source File

SOURCE=.\VisPropList.cpp
# End Source File
# Begin Source File

SOURCE=.\VisPropListInternal.cpp
# End Source File
# Begin Source File

SOURCE=.\VisPSFileIO.cpp
# End Source File
# Begin Source File

SOURCE=.\VisPump.cpp
# End Source File
# Begin Source File

SOURCE=.\VisSDIStream.cpp
# End Source File
# Begin Source File

SOURCE=.\VisSDOStream.cpp
# End Source File
# Begin Source File

SOURCE=.\VisSDStream.cpp
# End Source File
# Begin Source File

SOURCE=.\VisSequence.cpp
# End Source File
# Begin Source File

SOURCE=.\VisStandardProps.cpp
# End Source File
# Begin Source File

SOURCE=.\VisTimer2.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Inline Header Files"

# PROP Default_Filter "inl"
# Begin Source File

SOURCE=.\VisArray.inl
# End Source File
# Begin Source File

SOURCE=.\VisCopyFnOps.inl
# End Source File
# Begin Source File

SOURCE=.\VisCopyGrayPixels.inl
# End Source File
# Begin Source File

SOURCE=.\VisCopyStandardPixels.inl
# End Source File
# Begin Source File

SOURCE=.\VisDim.inl
# End Source File
# Begin Source File

SOURCE=.\VisEnumPixel.inl
# End Source File
# Begin Source File

SOURCE=.\VisError.inl
# End Source File
# Begin Source File

SOURCE=.\VisImage.inl
# End Source File
# Begin Source File

SOURCE=.\VisImageBase.inl
# End Source File
# Begin Source File

SOURCE=.\VisMap.inl
# End Source File
# Begin Source File

SOURCE=.\VisMemBlock.inl
# End Source File
# Begin Source File

SOURCE=.\VisNormalize.inl
# End Source File
# Begin Source File

SOURCE=.\VisPixFmt.inl
# End Source File
# Begin Source File

SOURCE=.\VisProp.inl
# End Source File
# Begin Source File

SOURCE=.\VisPropList.inl
# End Source File
# Begin Source File

SOURCE=.\VisPropListInternal.inl
# End Source File
# Begin Source File

SOURCE=.\VisPump.inl
# End Source File
# Begin Source File

SOURCE=.\VisRGBAPixels.inl
# End Source File
# Begin Source File

SOURCE=.\VisSDIStream.inl
# End Source File
# Begin Source File

SOURCE=.\VisSDOStream.inl
# End Source File
# Begin Source File

SOURCE=.\VisSDStream.inl
# End Source File
# Begin Source File

SOURCE=.\VisSequence.inl
# End Source File
# Begin Source File

SOURCE=.\VisShape.inl
# End Source File
# Begin Source File

SOURCE=.\VisStlWrappers.inl
# End Source File
# Begin Source File

SOURCE=.\VisTimer2.inl
# End Source File
# Begin Source File

SOURCE=.\VisYUVAPixels.inl
# End Source File
# End Group
# Begin Source File

SOURCE=.\VisArray.h
# End Source File
# Begin Source File

SOURCE=.\VisCopyFnOps.h
# End Source File
# Begin Source File

SOURCE=.\VisCopyGrayPixels.h
# End Source File
# Begin Source File

SOURCE=.\VisCopyStandardPixels.h
# End Source File
# Begin Source File

SOURCE=.\VisCoreDLL.h
# End Source File
# Begin Source File

SOURCE=.\VisCoreVersion.h
# End Source File
# Begin Source File

SOURCE=.\VisCritSect.h
# End Source File
# Begin Source File

SOURCE=.\VisDiagnostics.h
# End Source File
# Begin Source File

SOURCE=.\VisDim.h
# End Source File
# Begin Source File

SOURCE=.\visenumpixel.h
# End Source File
# Begin Source File

SOURCE=.\VisError.h
# End Source File
# Begin Source File

SOURCE=.\VisFileIO.h
# End Source File
# Begin Source File

SOURCE=.\VisFileStreamIO.h
# End Source File
# Begin Source File

SOURCE=.\VisImage.h
# End Source File
# Begin Source File

SOURCE=.\VisImageBase.h
# End Source File
# Begin Source File

SOURCE=.\VisImageTypes.h
# End Source File
# Begin Source File

SOURCE=.\VisImageWnd.h
# End Source File
# Begin Source File

SOURCE=..\VisImSrc\VisImEncoding.h
# End Source File
# Begin Source File

SOURCE=.\VisMap.h
# End Source File
# Begin Source File

SOURCE=.\VisMemBlock.h
# End Source File
# Begin Source File

SOURCE=.\VisNormalize.h
# End Source File
# Begin Source File

SOURCE=.\VisPixelFnOps.h
# End Source File
# Begin Source File

SOURCE=.\VisPixFmt.h
# End Source File
# Begin Source File

SOURCE=.\VisProp.h
# End Source File
# Begin Source File

SOURCE=.\VisPropList.h
# End Source File
# Begin Source File

SOURCE=.\VisPropListInternal.h
# End Source File
# Begin Source File

SOURCE=.\VisPump.h
# End Source File
# Begin Source File

SOURCE=.\VisRefCnt.h
# End Source File
# Begin Source File

SOURCE=.\VisRGBAPixels.h
# End Source File
# Begin Source File

SOURCE=.\VisSDIStream.h
# End Source File
# Begin Source File

SOURCE=.\VisSDOStream.h
# End Source File
# Begin Source File

SOURCE=.\VisSDStream.h
# End Source File
# Begin Source File

SOURCE=.\VisSequence.h
# End Source File
# Begin Source File

SOURCE=.\VisShape.h
# End Source File
# Begin Source File

SOURCE=.\VisStlWrappers.h
# End Source File
# Begin Source File

SOURCE=.\VisTimer2.h
# End Source File
# Begin Source File

SOURCE=.\VisYUVAPixels.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\VisCore.rc2
# End Source File
# End Group
# End Target
# End Project

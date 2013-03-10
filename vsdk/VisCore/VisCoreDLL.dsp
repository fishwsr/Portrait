# Microsoft Developer Studio Project File - Name="VisCoreDLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=VisCoreDLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VisCoreDLL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VisCoreDLL.mak" CFG="VisCoreDLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VisCoreDLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "VisCoreDLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/BetaVisSDK/VisCore", ITDAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VisCoreDLL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "RelDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VISCOREDLL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VIS_CORE_BUILD" /D "VIS_NO_MFC" /D "VIS_INCLUDE_SDSTREAM" /YX"VisCoreDLL.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib vfw32.lib DXGUID.LIB ddraw.lib AMSTRMID.LIB STRMBASD.LIB /nologo /dll /machine:I386 /out:"..\bin\VisCoreDLL.dll"

!ELSEIF  "$(CFG)" == "VisCoreDLL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "DbgDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VISCOREDLL_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VIS_CORE_BUILD" /D "VIS_NO_MFC" /D "VIS_INCLUDE_SDSTREAM" /YX"VisCoreDLL.h" /FD /GZ /GZ /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib vfw32.lib DXGUID.LIB ddraw.lib AMSTRMID.LIB STRMBASD.LIB /nologo /base:"0x64000000" /dll /incremental:no /debug /machine:I386 /out:"..\bin\VisCoreDLLDB.dll"
# SUBTRACT LINK32 /pdbtype:<none>

!ENDIF 

# Begin Target

# Name "VisCoreDLL - Win32 Release"
# Name "VisCoreDLL - Win32 Debug"
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
# End Source File
# Begin Source File

SOURCE=.\VisCore.rc
# End Source File
# Begin Source File

SOURCE=.\VisCoreDLL.cpp
# End Source File
# Begin Source File

SOURCE=.\VisDiagnostics.cpp
# End Source File
# Begin Source File

SOURCE=.\VisDiagnosticsMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\VisDib.cpp
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
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\VisCopyFnOps.h
# End Source File
# Begin Source File

SOURCE=.\VisCopyFnOps.inl
# End Source File
# Begin Source File

SOURCE=.\VisCopyGrayPixels.h
# End Source File
# Begin Source File

SOURCE=.\VisCopyGrayPixels.inl
# End Source File
# Begin Source File

SOURCE=.\VisCopyStandardPixels.h
# End Source File
# Begin Source File

SOURCE=.\VisCopyStandardPixels.inl
# End Source File
# Begin Source File

SOURCE=.\VisCore.h
# End Source File
# Begin Source File

SOURCE=.\VisCoreDLL.h
# End Source File
# Begin Source File

SOURCE=.\VisCoreDLLLatest.h
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

SOURCE=.\VisDiagnosticsMsg.h
# End Source File
# Begin Source File

SOURCE=.\VisDiagnosticsMsg.inl
# End Source File
# Begin Source File

SOURCE=.\VisDib.h
# End Source File
# Begin Source File

SOURCE=.\VisDib.inl
# End Source File
# Begin Source File

SOURCE=.\VisDim.h
# End Source File
# Begin Source File

SOURCE=.\VisDim.inl
# End Source File
# Begin Source File

SOURCE=.\VisEnumPixel.h
# End Source File
# Begin Source File

SOURCE=.\VisEnumPixel.inl
# End Source File
# Begin Source File

SOURCE=.\VisError.h
# End Source File
# Begin Source File

SOURCE=.\VisError.inl
# End Source File
# Begin Source File

SOURCE=.\VisFileIO.h
# End Source File
# Begin Source File

SOURCE=.\VisFileStreamIO.h
# End Source File
# Begin Source File

SOURCE=.\VisGrayAPixels.h
# End Source File
# Begin Source File

SOURCE=.\VisGrayAPixels.inl
# End Source File
# Begin Source File

SOURCE=.\VisImage.h
# End Source File
# Begin Source File

SOURCE=.\VisImage.inl
# End Source File
# Begin Source File

SOURCE=.\VisImageBase.h
# End Source File
# Begin Source File

SOURCE=.\VisImageBase.inl
# End Source File
# Begin Source File

SOURCE=.\VisImageTypes.h
# End Source File
# Begin Source File

SOURCE=.\VisImageWnd.h
# End Source File
# Begin Source File

SOURCE=.\VisMap.h
# End Source File
# Begin Source File

SOURCE=.\VisMap.inl
# End Source File
# Begin Source File

SOURCE=.\VisMemBlock.h
# End Source File
# Begin Source File

SOURCE=.\VisMemBlock.inl
# End Source File
# Begin Source File

SOURCE=.\VisNormalize.h
# End Source File
# Begin Source File

SOURCE=.\VisNormalize.inl
# End Source File
# Begin Source File

SOURCE=.\VisPixelFnOps.h
# End Source File
# Begin Source File

SOURCE=.\VisPixFmt.h
# End Source File
# Begin Source File

SOURCE=.\VisPixFmt.inl
# End Source File
# Begin Source File

SOURCE=.\VisProp.h
# End Source File
# Begin Source File

SOURCE=.\VisProp.inl
# End Source File
# Begin Source File

SOURCE=.\VisPropList.h
# End Source File
# Begin Source File

SOURCE=.\VisPropList.inl
# End Source File
# Begin Source File

SOURCE=.\VisPropListInternal.h
# End Source File
# Begin Source File

SOURCE=.\VisPropListInternal.inl
# End Source File
# Begin Source File

SOURCE=.\VisPump.h
# End Source File
# Begin Source File

SOURCE=.\VisPump.inl
# End Source File
# Begin Source File

SOURCE=.\VisRect.h
# End Source File
# Begin Source File

SOURCE=.\VisRefCnt.h
# End Source File
# Begin Source File

SOURCE=.\VisRGBAPixels.h
# End Source File
# Begin Source File

SOURCE=.\VisRGBAPixels.inl
# End Source File
# Begin Source File

SOURCE=.\VisSDIStream.h
# End Source File
# Begin Source File

SOURCE=.\VisSDIStream.inl
# End Source File
# Begin Source File

SOURCE=.\VisSDOStream.h
# End Source File
# Begin Source File

SOURCE=.\VisSDOStream.inl
# End Source File
# Begin Source File

SOURCE=.\VisSDStream.h
# End Source File
# Begin Source File

SOURCE=.\VisSDStream.inl
# End Source File
# Begin Source File

SOURCE=.\VisSequence.h
# End Source File
# Begin Source File

SOURCE=.\VisSequence.inl
# End Source File
# Begin Source File

SOURCE=.\VisShape.h
# End Source File
# Begin Source File

SOURCE=.\VisShape.inl
# End Source File
# Begin Source File

SOURCE=.\VisTimer2.h
# End Source File
# Begin Source File

SOURCE=.\VisTimer2.inl
# End Source File
# Begin Source File

SOURCE=.\VisYUVAPixels.h
# End Source File
# Begin Source File

SOURCE=.\VisYUVAPixels.inl
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

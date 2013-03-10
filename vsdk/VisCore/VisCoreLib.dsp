# Microsoft Developer Studio Project File - Name="VisCoreLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=VisCoreLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VisCoreLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VisCoreLib.mak" CFG="VisCoreLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VisCoreLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "VisCoreLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/BetaVisSDK/VisCore", ITDAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VisCoreLib - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Lib"
# PROP Intermediate_Dir "RelLib"
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "VIS_SDK_LIBS" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "VIS_CORE_BUILD" /D "VIS_INCLUDE_SDSTREAM" /YX"VisCorePch.h" /Fd"..\Lib/VisCoreLib" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "VisCoreLib - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Lib"
# PROP Intermediate_Dir "DbgLib"
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /D "_DEBUG" /D "VIS_SDK_LIBS" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "VIS_CORE_BUILD" /D "VIS_INCLUDE_SDSTREAM" /YX"VisCorePch.h" /Fd"..\Lib/VisCoreLibDB" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Lib\VisCoreLibDB.lib"

!ENDIF 

# Begin Target

# Name "VisCoreLib - Win32 Release"
# Name "VisCoreLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\VisAudioOperation.cpp
# End Source File
# Begin Source File

SOURCE=.\VisAudioSamples.cpp
# End Source File
# Begin Source File

SOURCE=.\VisAVIStreamIO.cpp
# End Source File
# Begin Source File

SOURCE=.\VisConsoleWnd.cpp
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

SOURCE=.\VisDDrawConversion.cpp
# End Source File
# Begin Source File

SOURCE=.\VisDiagnostics.cpp
# End Source File
# Begin Source File

SOURCE=.\VisDiagnosticsManager.cpp
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

SOURCE=.\VisGlobals.cpp
# End Source File
# Begin Source File

SOURCE=.\VisImageBase.cpp
# End Source File
# Begin Source File

SOURCE=.\VisImageInfo.cpp
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

SOURCE=.\VisPictureFileHandler.cpp
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

SOURCE=.\VisRGBAPixels.cpp
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
# Begin Source File

SOURCE=.\VisXIpl.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\VisAudioOperation.h
# End Source File
# Begin Source File

SOURCE=.\VisAudioSamples.h
# End Source File
# Begin Source File

SOURCE=.\VisAudioSamples.inl
# End Source File
# Begin Source File

SOURCE=.\VisConsoleWnd.h
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

SOURCE=.\VisCorePch.h
# End Source File
# Begin Source File

SOURCE=.\VisCoreProj.h
# End Source File
# Begin Source File

SOURCE=.\VisCritSect.h
# End Source File
# Begin Source File

SOURCE=.\VisDDrawConversion.h
# End Source File
# Begin Source File

SOURCE=.\VisDiagnostics.h
# End Source File
# Begin Source File

SOURCE=.\VisDiagnosticsManager.h
# End Source File
# Begin Source File

SOURCE=.\VisDiagnosticsManager.inl
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

SOURCE=.\VisGlobals.h
# End Source File
# Begin Source File

SOURCE=.\VisGlobals.inl
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

SOURCE=.\VisImageInfo.h
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

SOURCE=.\VisPictureFileHandler.h
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

SOURCE=.\VisRefCnt.h
# End Source File
# Begin Source File

SOURCE=..\VisImSrc\VisRegHelp.h
# End Source File
# Begin Source File

SOURCE=..\VisImSrc\VisRegHelp.inl
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

SOURCE=.\VisXIpl.h
# End Source File
# Begin Source File

SOURCE=.\VisXIpl.inl
# End Source File
# Begin Source File

SOURCE=.\VisYUVAPixels.h
# End Source File
# Begin Source File

SOURCE=.\VisYUVAPixels.inl
# End Source File
# End Group
# End Target
# End Project

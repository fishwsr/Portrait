# Microsoft Developer Studio Project File - Name="VisImSrcLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=VisImSrcLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VisImSrcLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VisImSrcLib.mak" CFG="VisImSrcLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VisImSrcLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "VisImSrcLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/BetaVisSDK/VisImSrc", UQFAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VisImSrcLib - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "VIS_SDK_LIBS" /YX"VisImSrcPch.h" /Fd"..\Lib/VisImSrcLib" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "VisImSrcLib - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Lib"
# PROP Intermediate_Dir "DbgLib"
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "VIS_SDK_LIBS" /YX"VisImSrcPch.h" /Fd"..\Lib/VisImSrcLibDB" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Lib\VisImSrcLibDB.lib"

!ENDIF 

# Begin Target

# Name "VisImSrcLib - Win32 Release"
# Name "VisImSrcLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\VisConvertCbitToCbit.cpp

!IF  "$(CFG)" == "VisImSrcLib - Win32 Release"

!ELSEIF  "$(CFG)" == "VisImSrcLib - Win32 Debug"

# SUBTRACT CPP /X /u

!ENDIF 

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

SOURCE=.\VisImSrcGUIDs.cpp
# End Source File
# Begin Source File

SOURCE=.\VisImSrcProj.cpp
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

SOURCE=.\VisImSrcIFace.h
# End Source File
# Begin Source File

SOURCE=.\VisImSrcNoMFC.h
# End Source File
# Begin Source File

SOURCE=.\VisImSrcNoMFC.inl
# End Source File
# Begin Source File

SOURCE=.\VisImSrcProj.h
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

SOURCE=.\VisSequenceList.h
# End Source File
# Begin Source File

SOURCE=.\VisSequenceList.inl
# End Source File
# End Group
# End Target
# End Project

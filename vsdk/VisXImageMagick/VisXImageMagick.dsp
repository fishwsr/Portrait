# Microsoft Developer Studio Project File - Name="VisXImageMagick" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=VisXImageMagick - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VisXImageMagick.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VisXImageMagick.mak" CFG="VisXImageMagick - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VisXImageMagick - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "VisXImageMagick - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/BetaVisSDK", HCBAAAAA"
# PROP Scc_LocalPath ".."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VisXImageMagick - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Lib"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "."
F90=fl32.exe
# ADD BASE F90 /Ox /I "VisXImageMagick\Release/" /c /nologo /MT
# ADD F90 /Ox /I "VisXImageMagick\Release/" /c /nologo /MT
LIB32=link.exe -lib
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W2 /GX /O2 /Ob2 /I "..\..\ImageMagick\xlib\include" /I "..\..\ImageMagick\magick" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "VIS_XIMAGEMAGICK_DLL" /FD /c
# SUBTRACT CPP /Fr /YX
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ..\..\ImageMagick\VisualMagick\lib\IMagick.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x64300000" /subsystem:windows /dll /pdb:"..\bin/VisXImageMagick.pdb" /machine:I386 /nodefaultlib:"nafxcw.lib" /out:"..\bin\VisXImageMagick.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "VisXImageMagick - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\VisXImag"
# PROP BASE Intermediate_Dir ".\VisXImag"
# PROP BASE Target_Dir "."
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Lib"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir "."
F90=fl32.exe
# ADD BASE F90 /Zi /I "VisXImageMagick\VisXImag/" /c /nologo /MT
# ADD F90 /Zi /I "VisXImageMagick\Debug/" /c /nologo /MT
LIB32=link.exe -lib
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "..\..\ImageMagick\xlib\include" /I "..\..\ImageMagick\magick" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "VIS_XIMAGEMAGICK_DLL" /Fd"Debug/VisXImageMagickDB" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 ..\..\ImageMagick\VisualMagick\lib\IMagickDB.lib /nologo /base:"0x64300000" /subsystem:windows /dll /incremental:no /pdb:"..\bin/VisXImageMagickDB.pdb" /debug /machine:I386 /nodefaultlib:"nafxcwd.lib" /out:"..\bin\VisXImageMagickDB.dll"
# SUBTRACT LINK32 /verbose /pdb:none

!ENDIF 

# Begin Target

# Name "VisXImageMagick - Win32 Release"
# Name "VisXImageMagick - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\VisXImageMagick.cpp

!IF  "$(CFG)" == "VisXImageMagick - Win32 Release"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "VisXImageMagick - Win32 Debug"

# SUBTRACT CPP /u /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\VisXImageMagick.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\VisXImageMagick.h
# End Source File
# Begin Source File

SOURCE=.\VisXImageMagickVersion.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

# Microsoft Developer Studio Project File - Name="VisMatrixDLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=VisMatrixDLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VisMatrixDLL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VisMatrixDLL.mak" CFG="VisMatrixDLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VisMatrixDLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "VisMatrixDLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/BetaVisSDK", IBDAAAAA"
# PROP Scc_LocalPath ".."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VisMatrixDLL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "VisMatri"
# PROP BASE Intermediate_Dir "VisMatri"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Lib"
# PROP Intermediate_Dir "RelDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "VIS_MATRIX_BUILD" /D "VIS_INCLUDE_SDSTREAM" /D "VIS_NO_MFC" /YX"VisMatrixDLL.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib /nologo /base:"0x64200000" /subsystem:windows /dll /pdb:"..\bin/VisMatrixDLL.pdb" /machine:I386 /out:"..\bin/VisMatrixDLL.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "VisMatrixDLL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "VisMatr0"
# PROP BASE Intermediate_Dir "VisMatr0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Lib"
# PROP Intermediate_Dir "DbgDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "VIS_MATRIX_BUILD" /D "VIS_INCLUDE_SDSTREAM" /D "VIS_NO_MFC" /D "VIS_NO_LUC_FILES" /YX"VisMatrixDLL.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib /nologo /base:"0x64200000" /subsystem:windows /dll /incremental:no /pdb:"..\bin/VisMatrixDLLDB.pdb" /debug /machine:I386 /out:"..\bin/VisMatrixDLLDB.dll"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "VisMatrixDLL - Win32 Release"
# Name "VisMatrixDLL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\VisCovariance.cpp
# End Source File
# Begin Source File

SOURCE=.\VisDMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\VisMatrix.rc
# End Source File
# Begin Source File

SOURCE=.\VisMatrixDLL.cpp
# End Source File
# Begin Source File

SOURCE=.\VisTransform4x4.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\VisCovariance.h
# End Source File
# Begin Source File

SOURCE=.\VisDMatrix.h
# End Source File
# Begin Source File

SOURCE=.\VisDMatrix.inl
# End Source File
# Begin Source File

SOURCE=.\VisMatrixDLL.h
# End Source File
# Begin Source File

SOURCE=.\VisTransform4x4.h
# End Source File
# Begin Source File

SOURCE=.\VisTransform4x4.inl
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\VisMatrix.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

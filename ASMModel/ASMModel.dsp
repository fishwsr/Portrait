# Microsoft Developer Studio Project File - Name="ASMModel" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ASMModel - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ASMModel.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ASMModel.mak" CFG="ASMModel - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ASMModel - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ASMModel - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ASMModel - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Release/ASMModel/"
# PROP Intermediate_Dir "../Release/ASMModel/"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
LIB32=link.exe -lib
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ASMMODEL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\opencv\cxcore\include" /I "..\opencv\cv\include" /I "..\opencv\otherlibs\highgui" /I "..\opencv\cvaux\include" /I "..\gdiplus\sdkinc" /I "..\vsdk\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ASMMODEL_EXPORTS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib cv.lib highgui.lib cvaux.lib cxcore.lib /nologo /dll /pdb:"..\pdb\ASMModel.pdb" /machine:I386 /out:"..\Bin\ASMModel.dll" /implib:"../lib/ASMModel.lib" /libpath:"..\Lib\\"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ASMModel - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Debug/ASMModel/"
# PROP Intermediate_Dir "../Debug/ASMModel/"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
LIB32=link.exe -lib
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ASMMODEL_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\opencv\cxcore\include" /I "..\opencv\cv\include" /I "..\opencv\otherlibs\highgui" /I "..\opencv\cvaux\include" /I "..\gdiplus\sdkinc" /I "..\vsdk\inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "ASMMODEL_EXPORTS" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 cvd.lib highguid.lib cvauxd.lib cxcored.lib /nologo /dll /pdb:"..\pdb\ASMModelD.pdb" /debug /machine:I386 /out:"../Bin/ASMModelD.dll" /implib:"../lib/ASMModelD.lib" /pdbtype:sept /libpath:"..\Lib\\"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ASMModel - Win32 Release"
# Name "ASMModel - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ASMDll.cpp
# End Source File
# Begin Source File

SOURCE=.\ASMLocalProfile.cpp
# End Source File
# Begin Source File

SOURCE=.\ASMML2DSearchModel.cpp
# End Source File
# Begin Source File

SOURCE=.\ASMMLSearchModel.cpp
# End Source File
# Begin Source File

SOURCE=.\ASMMLSearchModelBayesian.cpp
# End Source File
# Begin Source File

SOURCE=.\ASMModel.cpp
# End Source File
# Begin Source File

SOURCE=.\ASMPCALocalStruct.cpp
# End Source File
# Begin Source File

SOURCE=.\ASMSearchOption.cpp
# End Source File
# Begin Source File

SOURCE=.\GenerativeShapeModel.cpp
# End Source File
# Begin Source File

SOURCE=.\LocalPatchModel.cpp
# End Source File
# Begin Source File

SOURCE=.\PatchShapeModel.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ASMDll.h
# End Source File
# Begin Source File

SOURCE=.\ASMLocalProfile.h
# End Source File
# Begin Source File

SOURCE=.\ASMML2DSearchModel.h
# End Source File
# Begin Source File

SOURCE=.\ASMMLSearchModel.h
# End Source File
# Begin Source File

SOURCE=.\ASMMLSearchModelBayesian.h
# End Source File
# Begin Source File

SOURCE=.\ASMModel.h
# End Source File
# Begin Source File

SOURCE=.\ASMPCALocalStruct.h
# End Source File
# Begin Source File

SOURCE=.\ASMSearchOption.h
# End Source File
# Begin Source File

SOURCE=.\GenerativeShapeModel.h
# End Source File
# Begin Source File

SOURCE=.\LocalPatchModel.h
# End Source File
# Begin Source File

SOURCE=.\PatchShapeModel.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ASMSearchOption.asp
# End Source File
# End Target
# End Project

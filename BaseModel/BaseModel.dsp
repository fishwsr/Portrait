# Microsoft Developer Studio Project File - Name="BaseModel" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BaseModel - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BaseModel.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BaseModel.mak" CFG="BaseModel - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BaseModel - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BaseModel - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BaseModel - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Release/BaseModel/"
# PROP Intermediate_Dir "../Release/BaseModel/"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
LIB32=link.exe -lib
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BASEMODEL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\opencv\cxcore\include" /I "..\opencv\cv\include" /I "..\opencv\otherlibs\highgui" /I "..\opencv\cvaux\include" /I "..\gdiplus\sdkinc" /I "..\vsdk\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BASEMODEL_EXPORTS" /FD /c
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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib cv.lib highgui.lib cvaux.lib cxcore.lib /nologo /dll /pdb:"..\pdb\BaseModel.pdb" /machine:I386 /out:"..\Bin\BaseModel.dll" /implib:"../lib/BaseModel.lib" /libpath:"..\Lib\\"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "BaseModel - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Debug/BaseModel/"
# PROP Intermediate_Dir "../Debug/BaseModel/"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
LIB32=link.exe -lib
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BASEMODEL_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\opencv\cxcore\include" /I "..\opencv\cv\include" /I "..\opencv\otherlibs\highgui" /I "..\opencv\cvaux\include" /I "..\gdiplus\sdkinc" /I "..\vsdk\inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "BASEMODEL_EXPORTS" /FR /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 cvd.lib highguid.lib cvauxd.lib cxcored.lib /nologo /dll /pdb:"..\pdb\BaseModelD.pdb" /debug /machine:I386 /out:"../Bin/BaseModelD.dll" /implib:"../Lib/BaseModelD.lib" /pdbtype:sept /libpath:"..\Lib\\"
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ENDIF 

# Begin Target

# Name "BaseModel - Win32 Release"
# Name "BaseModel - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BaseModel.cpp
# End Source File
# Begin Source File

SOURCE=.\Distribution.cpp
# End Source File
# Begin Source File

SOURCE=.\DistributionMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\GaussianModel.cpp
# End Source File
# Begin Source File

SOURCE=.\GibbsModel.cpp
# End Source File
# Begin Source File

SOURCE=.\Graph.cpp
# End Source File
# Begin Source File

SOURCE=.\MixturedShapeModel.cpp
# End Source File
# Begin Source File

SOURCE=.\MixtureGaussian.cpp
# End Source File
# Begin Source File

SOURCE=.\MixtureGaussianKernel.cpp
# End Source File
# Begin Source File

SOURCE=.\MixturePPCA.cpp
# End Source File
# Begin Source File

SOURCE=.\myrandom.cpp
# End Source File
# Begin Source File

SOURCE=.\NMFModel.cpp
# End Source File
# Begin Source File

SOURCE=.\PCA2Model.cpp
# End Source File
# Begin Source File

SOURCE=.\PCAModel.cpp
# End Source File
# Begin Source File

SOURCE=.\PPCAModel.cpp
# End Source File
# Begin Source File

SOURCE=.\ShapeModel.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BaseModel.h
# End Source File
# Begin Source File

SOURCE=.\Distribution.h
# End Source File
# Begin Source File

SOURCE=.\DistributionMatrix.h
# End Source File
# Begin Source File

SOURCE=.\GaussianModel.h
# End Source File
# Begin Source File

SOURCE=.\GibbsModel.h
# End Source File
# Begin Source File

SOURCE=.\Graph.h
# End Source File
# Begin Source File

SOURCE=.\MixturedShapeModel.h
# End Source File
# Begin Source File

SOURCE=.\MixtureGaussian.h
# End Source File
# Begin Source File

SOURCE=.\MixtureGaussianKernel.h
# End Source File
# Begin Source File

SOURCE=.\MixtureModel.h
# End Source File
# Begin Source File

SOURCE=.\MixturePPCA.h
# End Source File
# Begin Source File

SOURCE=.\myrandom.h
# End Source File
# Begin Source File

SOURCE=.\NMFModel.h
# End Source File
# Begin Source File

SOURCE=.\PCA2Model.h
# End Source File
# Begin Source File

SOURCE=.\PCAModel.h
# End Source File
# Begin Source File

SOURCE=.\PPCAModel.h
# End Source File
# Begin Source File

SOURCE=.\ShapeModel.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

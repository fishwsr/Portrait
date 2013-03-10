# Microsoft Developer Studio Generated NMAKE File, Based on BaseModel.dsp
!IF "$(CFG)" == ""
CFG=BaseModel - Win32 Debug
!MESSAGE No configuration specified. Defaulting to BaseModel - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "BaseModel - Win32 Release" && "$(CFG)" != "BaseModel - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "BaseModel - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\Bin\BaseModel.dll"


CLEAN :
	-@erase "$(INTDIR)\BaseModel.obj"
	-@erase "$(INTDIR)\DataFile.obj"
	-@erase "$(INTDIR)\Distribution.obj"
	-@erase "$(INTDIR)\GaussianModel.obj"
	-@erase "$(INTDIR)\MixtureGaussian.obj"
	-@erase "$(INTDIR)\MixturePPCA.obj"
	-@erase "$(INTDIR)\myrandom.obj"
	-@erase "$(INTDIR)\PCA2Model.obj"
	-@erase "$(INTDIR)\PCAModel.obj"
	-@erase "$(INTDIR)\PPCAModel.obj"
	-@erase "$(INTDIR)\Shape.obj"
	-@erase "$(INTDIR)\ShapeModel.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\..\Bin\BaseModel.dll"
	-@erase "..\lib\BaseModel.exp"
	-@erase "..\lib\BaseModel.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BASEMODEL_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\BaseModel.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"..\pdb\BaseModel.pdb" /machine:I386 /out:"..\..\Bin\BaseModel.dll" /implib:"../lib/BaseModel.lib" 
LINK32_OBJS= \
	"$(INTDIR)\BaseModel.obj" \
	"$(INTDIR)\DataFile.obj" \
	"$(INTDIR)\Distribution.obj" \
	"$(INTDIR)\GaussianModel.obj" \
	"$(INTDIR)\MixtureGaussian.obj" \
	"$(INTDIR)\MixturePPCA.obj" \
	"$(INTDIR)\myrandom.obj" \
	"$(INTDIR)\PCA2Model.obj" \
	"$(INTDIR)\PCAModel.obj" \
	"$(INTDIR)\PPCAModel.obj" \
	"$(INTDIR)\Shape.obj" \
	"$(INTDIR)\ShapeModel.obj" \
	"$(INTDIR)\StdAfx.obj"

"..\..\Bin\BaseModel.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "BaseModel - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\Bin\BaseModelD.dll"


CLEAN :
	-@erase "$(INTDIR)\BaseModel.obj"
	-@erase "$(INTDIR)\DataFile.obj"
	-@erase "$(INTDIR)\Distribution.obj"
	-@erase "$(INTDIR)\GaussianModel.obj"
	-@erase "$(INTDIR)\MixtureGaussian.obj"
	-@erase "$(INTDIR)\MixturePPCA.obj"
	-@erase "$(INTDIR)\myrandom.obj"
	-@erase "$(INTDIR)\PCA2Model.obj"
	-@erase "$(INTDIR)\PCAModel.obj"
	-@erase "$(INTDIR)\PPCAModel.obj"
	-@erase "$(INTDIR)\Shape.obj"
	-@erase "$(INTDIR)\ShapeModel.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "..\..\Bin\BaseModelD.dll"
	-@erase "..\..\Bin\BaseModelD.ilk"
	-@erase "..\Lib\BaseModelD.exp"
	-@erase "..\Lib\BaseModelD.lib"
	-@erase "..\pdb\BaseModelD.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "BASEMODEL_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\BaseModel.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=gdi32.lib /nologo /dll /incremental:yes /pdb:"..\pdb\BaseModelD.pdb" /debug /machine:I386 /out:"..\..\Bin\BaseModelD.dll" /implib:"../Lib/BaseModelD.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\BaseModel.obj" \
	"$(INTDIR)\DataFile.obj" \
	"$(INTDIR)\Distribution.obj" \
	"$(INTDIR)\GaussianModel.obj" \
	"$(INTDIR)\MixtureGaussian.obj" \
	"$(INTDIR)\MixturePPCA.obj" \
	"$(INTDIR)\myrandom.obj" \
	"$(INTDIR)\PCA2Model.obj" \
	"$(INTDIR)\PCAModel.obj" \
	"$(INTDIR)\PPCAModel.obj" \
	"$(INTDIR)\Shape.obj" \
	"$(INTDIR)\ShapeModel.obj" \
	"$(INTDIR)\StdAfx.obj"

"..\..\Bin\BaseModelD.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("BaseModel.dep")
!INCLUDE "BaseModel.dep"
!ELSE 
!MESSAGE Warning: cannot find "BaseModel.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "BaseModel - Win32 Release" || "$(CFG)" == "BaseModel - Win32 Debug"
SOURCE=.\BaseModel.cpp

"$(INTDIR)\BaseModel.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DataFile.cpp

"$(INTDIR)\DataFile.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Distribution.cpp

"$(INTDIR)\Distribution.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GaussianModel.cpp

"$(INTDIR)\GaussianModel.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\MixtureGaussian.cpp

"$(INTDIR)\MixtureGaussian.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\MixturePPCA.cpp

"$(INTDIR)\MixturePPCA.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\myrandom.cpp

"$(INTDIR)\myrandom.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\PCA2Model.cpp

"$(INTDIR)\PCA2Model.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\PCAModel.cpp

"$(INTDIR)\PCAModel.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\PPCAModel.cpp

"$(INTDIR)\PPCAModel.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Shape.cpp

"$(INTDIR)\Shape.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ShapeModel.cpp

"$(INTDIR)\ShapeModel.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\StdAfx.cpp

"$(INTDIR)\StdAfx.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 


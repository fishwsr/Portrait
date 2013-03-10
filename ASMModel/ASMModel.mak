# Microsoft Developer Studio Generated NMAKE File, Based on ASMModel.dsp
!IF "$(CFG)" == ""
CFG=ASMModel - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ASMModel - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ASMModel - Win32 Release" && "$(CFG)" != "ASMModel - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ASMModel - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\..\Bin\ASMModel.dll"

!ELSE 

ALL : "BaseModel - Win32 Release" "..\..\Bin\ASMModel.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"BaseModel - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ASMLocalProfile.obj"
	-@erase "$(INTDIR)\ASMML2DSearchModel.obj"
	-@erase "$(INTDIR)\ASMMLSearchModel.obj"
	-@erase "$(INTDIR)\ASMMLSearchModelBayesian.obj"
	-@erase "$(INTDIR)\ASMModel.obj"
	-@erase "$(INTDIR)\ASMPCALocalStruct.obj"
	-@erase "$(INTDIR)\ConnectedModel.obj"
	-@erase "$(INTDIR)\GenerativeShapeModel.obj"
	-@erase "$(INTDIR)\IndexedContour.obj"
	-@erase "$(INTDIR)\LocalPatchModel.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\..\Bin\ASMModel.dll"
	-@erase "..\lib\ASMModel.exp"
	-@erase "..\lib\ASMModel.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ASMMODEL_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ASMModel.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"..\pdb\ASMModel.pdb" /machine:I386 /out:"..\..\Bin\ASMModel.dll" /implib:"../lib/ASMModel.lib" 
LINK32_OBJS= \
	"$(INTDIR)\ASMLocalProfile.obj" \
	"$(INTDIR)\ASMML2DSearchModel.obj" \
	"$(INTDIR)\ASMMLSearchModel.obj" \
	"$(INTDIR)\ASMMLSearchModelBayesian.obj" \
	"$(INTDIR)\ASMModel.obj" \
	"$(INTDIR)\ASMPCALocalStruct.obj" \
	"$(INTDIR)\ConnectedModel.obj" \
	"$(INTDIR)\GenerativeShapeModel.obj" \
	"$(INTDIR)\IndexedContour.obj" \
	"$(INTDIR)\LocalPatchModel.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"..\lib\BaseModel.lib"

"..\..\Bin\ASMModel.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ASMModel - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\..\Bin\ASMModelD.dll"

!ELSE 

ALL : "BaseModel - Win32 Debug" "..\..\Bin\ASMModelD.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"BaseModel - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ASMLocalProfile.obj"
	-@erase "$(INTDIR)\ASMML2DSearchModel.obj"
	-@erase "$(INTDIR)\ASMMLSearchModel.obj"
	-@erase "$(INTDIR)\ASMMLSearchModelBayesian.obj"
	-@erase "$(INTDIR)\ASMModel.obj"
	-@erase "$(INTDIR)\ASMPCALocalStruct.obj"
	-@erase "$(INTDIR)\ConnectedModel.obj"
	-@erase "$(INTDIR)\GenerativeShapeModel.obj"
	-@erase "$(INTDIR)\IndexedContour.obj"
	-@erase "$(INTDIR)\LocalPatchModel.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "..\..\Bin\ASMModelD.dll"
	-@erase "..\..\Bin\ASMModelD.ilk"
	-@erase "..\lib\ASMModelD.exp"
	-@erase "..\lib\ASMModelD.lib"
	-@erase "..\pdb\ASMModelD.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "ASMMODEL_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ASMModel.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=VisImageProcDB.lib /nologo /dll /incremental:yes /pdb:"..\pdb\ASMModelD.pdb" /debug /machine:I386 /out:"..\..\Bin\ASMModelD.dll" /implib:"../lib/ASMModelD.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\ASMLocalProfile.obj" \
	"$(INTDIR)\ASMML2DSearchModel.obj" \
	"$(INTDIR)\ASMMLSearchModel.obj" \
	"$(INTDIR)\ASMMLSearchModelBayesian.obj" \
	"$(INTDIR)\ASMModel.obj" \
	"$(INTDIR)\ASMPCALocalStruct.obj" \
	"$(INTDIR)\ConnectedModel.obj" \
	"$(INTDIR)\GenerativeShapeModel.obj" \
	"$(INTDIR)\IndexedContour.obj" \
	"$(INTDIR)\LocalPatchModel.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"..\Lib\BaseModelD.lib"

"..\..\Bin\ASMModelD.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ASMModel.dep")
!INCLUDE "ASMModel.dep"
!ELSE 
!MESSAGE Warning: cannot find "ASMModel.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ASMModel - Win32 Release" || "$(CFG)" == "ASMModel - Win32 Debug"
SOURCE=.\ASMLocalProfile.cpp

"$(INTDIR)\ASMLocalProfile.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ASMML2DSearchModel.cpp

"$(INTDIR)\ASMML2DSearchModel.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ASMMLSearchModel.cpp

"$(INTDIR)\ASMMLSearchModel.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ASMMLSearchModelBayesian.cpp

"$(INTDIR)\ASMMLSearchModelBayesian.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ASMModel.cpp

"$(INTDIR)\ASMModel.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ASMPCALocalStruct.cpp

"$(INTDIR)\ASMPCALocalStruct.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ConnectedModel.cpp

"$(INTDIR)\ConnectedModel.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GenerativeShapeModel.cpp

"$(INTDIR)\GenerativeShapeModel.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\IndexedContour.cpp

"$(INTDIR)\IndexedContour.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\LocalPatchModel.cpp

"$(INTDIR)\LocalPatchModel.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\StdAfx.cpp

"$(INTDIR)\StdAfx.obj" : $(SOURCE) "$(INTDIR)"


!IF  "$(CFG)" == "ASMModel - Win32 Release"

"BaseModel - Win32 Release" : 
   cd "\hchen\projects\MFace\sdk\BaseModel"
   $(MAKE) /$(MAKEFLAGS) /F .\BaseModel.mak CFG="BaseModel - Win32 Release" 
   cd "..\ASMModel"

"BaseModel - Win32 ReleaseCLEAN" : 
   cd "\hchen\projects\MFace\sdk\BaseModel"
   $(MAKE) /$(MAKEFLAGS) /F .\BaseModel.mak CFG="BaseModel - Win32 Release" RECURSE=1 CLEAN 
   cd "..\ASMModel"

!ELSEIF  "$(CFG)" == "ASMModel - Win32 Debug"

"BaseModel - Win32 Debug" : 
   cd "\hchen\projects\MFace\sdk\BaseModel"
   $(MAKE) /$(MAKEFLAGS) /F .\BaseModel.mak CFG="BaseModel - Win32 Debug" 
   cd "..\ASMModel"

"BaseModel - Win32 DebugCLEAN" : 
   cd "\hchen\projects\MFace\sdk\BaseModel"
   $(MAKE) /$(MAKEFLAGS) /F .\BaseModel.mak CFG="BaseModel - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\ASMModel"

!ENDIF 


!ENDIF 


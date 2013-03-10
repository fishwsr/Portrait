# Microsoft Developer Studio Project File - Name="VisImageProc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=VisImageProc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VisImageProc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VisImageProc.mak" CFG="VisImageProc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VisImageProc - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "VisImageProc - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/BetaVisSDK", IBDAAAAA"
# PROP Scc_LocalPath ".."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VisImageProc - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "VisImag0"
# PROP BASE Intermediate_Dir "VisImag0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "VIS_INCLUDE_SDSTREAM" /YX"VisImageProcPch.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "VisImageProc - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "VisImag1"
# PROP BASE Intermediate_Dir "VisImag1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "VIS_INCLUDE_SDSTREAM" /YX"VisImageProcPch.h" /Fd"Debug/VisImageProcDB" /FD /GZ /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\VisImageProcDB.lib"

!ENDIF 

# Begin Target

# Name "VisImageProc - Win32 Release"
# Name "VisImageProc - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\VisAlign.cpp
# End Source File
# Begin Source File

SOURCE=.\VisBlend.cpp
# End Source File
# Begin Source File

SOURCE=.\VisBlendedSequence.cpp
# End Source File
# Begin Source File

SOURCE=.\VisBlockAdjust.cpp
# End Source File
# Begin Source File

SOURCE=.\VisCollection.cpp
# End Source File
# Begin Source File

SOURCE=.\VisColorAdjust.cpp
# End Source File
# Begin Source File

SOURCE=.\VisComposite.cpp
# End Source File
# Begin Source File

SOURCE=.\VisConvolve.cpp
# End Source File
# Begin Source File

SOURCE=.\VisCylindrical.cpp
# End Source File
# Begin Source File

SOURCE=.\VisFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\VisFreeRotation.cpp
# End Source File
# Begin Source File

SOURCE=.\VisHistogram.cpp
# End Source File
# Begin Source File

SOURCE=.\VisHistogramEq.cpp
# End Source File
# Begin Source File

SOURCE=.\VisLineExtract.cpp
# End Source File
# Begin Source File

SOURCE=.\VisPolygonRender.cpp
# End Source File
# Begin Source File

SOURCE=.\VisProcMisc.cpp
# End Source File
# Begin Source File

SOURCE=.\VisPyramid.cpp
# End Source File
# Begin Source File

SOURCE=.\VisRectangularGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\VisRegionExtract.cpp
# End Source File
# Begin Source File

SOURCE=.\VisResample.cpp
# End Source File
# Begin Source File

SOURCE=.\VisRGBA.cpp
# End Source File
# Begin Source File

SOURCE=.\VisSDStreamExemplars.cpp
# End Source File
# Begin Source File

SOURCE=.\VisSprites.cpp
# End Source File
# Begin Source File

SOURCE=.\VisStereo.cpp
# End Source File
# Begin Source File

SOURCE=.\VisVoxelColor.cpp
# End Source File
# Begin Source File

SOURCE=.\VisWarp.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm"
# Begin Group "Inline Header Files"

# PROP Default_Filter "inl"
# Begin Source File

SOURCE=.\VisBlob.inl
# End Source File
# Begin Source File

SOURCE=.\VisBlobInternal.inl
# End Source File
# Begin Source File

SOURCE=.\VisComposite.inl
# End Source File
# Begin Source File

SOURCE=.\VisCylindrical.inl
# End Source File
# Begin Source File

SOURCE=.\VisFrame.inl
# End Source File
# Begin Source File

SOURCE=.\VisHistogram.inl
# End Source File
# Begin Source File

SOURCE=.\VisObjList.inl
# End Source File
# Begin Source File

SOURCE=.\VisPixRun.inl
# End Source File
# Begin Source File

SOURCE=.\VisStrip.inl
# End Source File
# Begin Source File

SOURCE=.\VisThresholdFnOps.inl
# End Source File
# End Group
# Begin Source File

SOURCE=.\VisAlign.h
# End Source File
# Begin Source File

SOURCE=.\VisBlend.h
# End Source File
# Begin Source File

SOURCE=.\VisBlendedSequence.h
# End Source File
# Begin Source File

SOURCE=.\VisBlob.h
# End Source File
# Begin Source File

SOURCE=.\VisBlobFnOp.h
# End Source File
# Begin Source File

SOURCE=.\VisBlobInternal.h
# End Source File
# Begin Source File

SOURCE=.\VisBlockAdjust.h
# End Source File
# Begin Source File

SOURCE=.\VisCollection.h
# End Source File
# Begin Source File

SOURCE=.\VisColorAdjust.h
# End Source File
# Begin Source File

SOURCE=.\VisComposite.h
# End Source File
# Begin Source File

SOURCE=.\VisConvolve.h
# End Source File
# Begin Source File

SOURCE=.\VisCylindrical.h
# End Source File
# Begin Source File

SOURCE=.\VisFrame.h
# End Source File
# Begin Source File

SOURCE=.\VisFreeRotation.h
# End Source File
# Begin Source File

SOURCE=.\VisHistogram.h
# End Source File
# Begin Source File

SOURCE=.\VisHistogramEq.h
# End Source File
# Begin Source File

SOURCE=..\inc\VisImageProc.h
# End Source File
# Begin Source File

SOURCE=.\VisImageProc.h
# End Source File
# Begin Source File

SOURCE=.\VisImageProcPch.h
# End Source File
# Begin Source File

SOURCE=.\VisImageProcVersion.h
# End Source File
# Begin Source File

SOURCE=.\VisLineExtract.h
# End Source File
# Begin Source File

SOURCE=.\VisMedian.h
# End Source File
# Begin Source File

SOURCE=.\VisObjList.h
# End Source File
# Begin Source File

SOURCE=.\VisPixRun.h
# End Source File
# Begin Source File

SOURCE=.\VisPolygonRender.h
# End Source File
# Begin Source File

SOURCE=.\VisProcMisc.h
# End Source File
# Begin Source File

SOURCE=.\VisPyramid.h
# End Source File
# Begin Source File

SOURCE=.\VisRectangularGrid.h
# End Source File
# Begin Source File

SOURCE=.\VisRegionExtract.h
# End Source File
# Begin Source File

SOURCE=.\VisResample.h
# End Source File
# Begin Source File

SOURCE=.\VisRGBA.h
# End Source File
# Begin Source File

SOURCE=.\VisSprites.h
# End Source File
# Begin Source File

SOURCE=.\VisStereo.h
# End Source File
# Begin Source File

SOURCE=.\VisStrip.h
# End Source File
# Begin Source File

SOURCE=.\VisThresholdFnOps.h
# End Source File
# Begin Source File

SOURCE=.\VisVoxelColor.h
# End Source File
# Begin Source File

SOURCE=.\VisWarp.h
# End Source File
# End Group
# End Target
# End Project

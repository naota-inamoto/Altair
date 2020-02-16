# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=AlMm - Win32 Debug
!MESSAGE 構成が指定されていません。ﾃﾞﾌｫﾙﾄの AlMm - Win32 Debug を設定します。
!ENDIF 

!IF "$(CFG)" != "AlMm - Win32 Release" && "$(CFG)" != "AlMm - Win32 Debug" &&\
 "$(CFG)" != "AlMm - Win32 Unicode Debug" && "$(CFG)" !=\
 "AlMm - Win32 Unicode Release"
!MESSAGE 指定された ﾋﾞﾙﾄﾞ ﾓｰﾄﾞ "$(CFG)" は正しくありません。
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛ 'CFG' を定義することによって
!MESSAGE NMAKE 実行時にﾋﾞﾙﾄﾞ ﾓｰﾄﾞを指定できます。例えば:
!MESSAGE 
!MESSAGE NMAKE /f "AlMm.mak" CFG="AlMm - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "AlMm - Win32 Release" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "AlMm - Win32 Debug" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "AlMm - Win32 Unicode Debug" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "AlMm - Win32 Unicode Release" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE 
!ERROR 無効な構成が指定されています。
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "AlMm - Win32 Release"
RSC=rc.exe
MTL=mktyplib.exe
CPP=cl.exe

!IF  "$(CFG)" == "AlMm - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP BASE Target_Ext "ocx"
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# PROP Target_Ext "ocx"
OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\AlMm.ocx" "$(OUTDIR)\regsvr32.trg"

CLEAN : 
	-@erase "$(INTDIR)\AlMm.obj"
	-@erase "$(INTDIR)\AlMm.pch"
	-@erase "$(INTDIR)\AlMm.res"
	-@erase "$(INTDIR)\AlMm.tlb"
	-@erase "$(INTDIR)\AlMmCtl.obj"
	-@erase "$(INTDIR)\AlMmPpg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(OUTDIR)\AlMm.exp"
	-@erase "$(OUTDIR)\AlMm.lib"
	-@erase "$(OUTDIR)\AlMm.ocx"
	-@erase "$(OUTDIR)\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)/AlMm.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x411 /fo"$(INTDIR)/AlMm.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/AlMm.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 winmm.lib /nologo /subsystem:windows /dll /machine:I386
LINK32_FLAGS=winmm.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/AlMm.pdb" /machine:I386 /def:"A:\Altair\Src\AlMm\AlMm.def"\
 /out:"$(OUTDIR)/AlMm.ocx" /implib:"$(OUTDIR)/AlMm.lib" 
DEF_FILE= \
	"A:\Altair\Src\AlMm\AlMm.def"
LINK32_OBJS= \
	"$(INTDIR)\AlMm.obj" \
	"$(INTDIR)\AlMm.res" \
	"$(INTDIR)\AlMmCtl.obj" \
	"$(INTDIR)\AlMmPpg.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\AlMm.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - OLE ｺﾝﾄﾛｰﾙを登録しています...
OutDir=.\Release
TargetPath=.\Release\AlMm.ocx
InputPath=.\Release\AlMm.ocx
SOURCE=$(InputPath)

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   regsvr32 /s /c "$(TargetPath)"
   echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg"

# End Custom Build

!ELSEIF  "$(CFG)" == "AlMm - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP BASE Target_Ext "ocx"
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# PROP Target_Ext "ocx"
OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\AlMm.ocx" "$(OUTDIR)\regsvr32.trg"

CLEAN : 
	-@erase "$(INTDIR)\AlMm.obj"
	-@erase "$(INTDIR)\AlMm.pch"
	-@erase "$(INTDIR)\AlMm.res"
	-@erase "$(INTDIR)\AlMm.tlb"
	-@erase "$(INTDIR)\AlMmCtl.obj"
	-@erase "$(INTDIR)\AlMmPpg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\AlMm.exp"
	-@erase "$(OUTDIR)\AlMm.ilk"
	-@erase "$(OUTDIR)\AlMm.lib"
	-@erase "$(OUTDIR)\AlMm.ocx"
	-@erase "$(OUTDIR)\AlMm.pdb"
	-@erase "$(OUTDIR)\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)/AlMm.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x411 /fo"$(INTDIR)/AlMm.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/AlMm.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 winmm.lib /nologo /subsystem:windows /dll /debug /machine:I386
LINK32_FLAGS=winmm.lib /nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)/AlMm.pdb" /debug /machine:I386\
 /def:"A:\Altair\Src\AlMm\AlMm.def" /out:"$(OUTDIR)/AlMm.ocx"\
 /implib:"$(OUTDIR)/AlMm.lib" 
DEF_FILE= \
	"A:\Altair\Src\AlMm\AlMm.def"
LINK32_OBJS= \
	"$(INTDIR)\AlMm.obj" \
	"$(INTDIR)\AlMm.res" \
	"$(INTDIR)\AlMmCtl.obj" \
	"$(INTDIR)\AlMmPpg.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\AlMm.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - OLE ｺﾝﾄﾛｰﾙを登録しています...
OutDir=.\Debug
TargetPath=.\Debug\AlMm.ocx
InputPath=.\Debug\AlMm.ocx
SOURCE=$(InputPath)

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   regsvr32 /s /c "$(TargetPath)"
   echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg"

# End Custom Build

!ELSEIF  "$(CFG)" == "AlMm - Win32 Unicode Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Target_Dir ""
# PROP BASE Target_Ext "ocx"
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Target_Dir ""
# PROP Target_Ext "ocx"
OUTDIR=.\DebugU
INTDIR=.\DebugU
# Begin Custom Macros
OutDir=.\DebugU
# End Custom Macros

ALL : "$(OUTDIR)\AlMm.ocx" "$(OUTDIR)\regsvr32.trg"

CLEAN : 
	-@erase "$(INTDIR)\AlMm.obj"
	-@erase "$(INTDIR)\AlMm.res"
	-@erase "$(INTDIR)\AlMm.tlb"
	-@erase "$(INTDIR)\AlMmCtl.obj"
	-@erase "$(INTDIR)\AlMmPpg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\AlMm.exp"
	-@erase "$(OUTDIR)\AlMm.ilk"
	-@erase "$(OUTDIR)\AlMm.lib"
	-@erase "$(OUTDIR)\AlMm.ocx"
	-@erase "$(OUTDIR)\AlMm.pdb"
	-@erase "$(OUTDIR)\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Fp"$(INTDIR)/AlMm.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\DebugU/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x411 /fo"$(INTDIR)/AlMm.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/AlMm.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)/AlMm.pdb" /debug /machine:I386\
 /def:"A:\Altair\Src\AlMm\AlMm.def" /out:"$(OUTDIR)/AlMm.ocx"\
 /implib:"$(OUTDIR)/AlMm.lib" 
DEF_FILE= \
	"A:\Altair\Src\AlMm\AlMm.def"
LINK32_OBJS= \
	"$(INTDIR)\AlMm.obj" \
	"$(INTDIR)\AlMm.res" \
	"$(INTDIR)\AlMmCtl.obj" \
	"$(INTDIR)\AlMmPpg.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\AlMm.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - OLE ｺﾝﾄﾛｰﾙを登録しています...
OutDir=.\DebugU
TargetPath=.\DebugU\AlMm.ocx
InputPath=.\DebugU\AlMm.ocx
SOURCE=$(InputPath)

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   regsvr32 /s /c "$(TargetPath)"
   echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg"

# End Custom Build

!ELSEIF  "$(CFG)" == "AlMm - Win32 Unicode Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseU"
# PROP BASE Intermediate_Dir "ReleaseU"
# PROP BASE Target_Dir ""
# PROP BASE Target_Ext "ocx"
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseU"
# PROP Intermediate_Dir "ReleaseU"
# PROP Target_Dir ""
# PROP Target_Ext "ocx"
OUTDIR=.\ReleaseU
INTDIR=.\ReleaseU
# Begin Custom Macros
OutDir=.\ReleaseU
# End Custom Macros

ALL : "$(OUTDIR)\AlMm.ocx" "$(OUTDIR)\regsvr32.trg"

CLEAN : 
	-@erase "$(INTDIR)\AlMm.obj"
	-@erase "$(INTDIR)\AlMm.res"
	-@erase "$(INTDIR)\AlMm.tlb"
	-@erase "$(INTDIR)\AlMmCtl.obj"
	-@erase "$(INTDIR)\AlMmPpg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(OUTDIR)\AlMm.exp"
	-@erase "$(OUTDIR)\AlMm.lib"
	-@erase "$(OUTDIR)\AlMm.ocx"
	-@erase "$(OUTDIR)\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Fp"$(INTDIR)/AlMm.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\ReleaseU/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x411 /fo"$(INTDIR)/AlMm.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/AlMm.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/AlMm.pdb" /machine:I386 /def:"A:\Altair\Src\AlMm\AlMm.def"\
 /out:"$(OUTDIR)/AlMm.ocx" /implib:"$(OUTDIR)/AlMm.lib" 
DEF_FILE= \
	"A:\Altair\Src\AlMm\AlMm.def"
LINK32_OBJS= \
	"$(INTDIR)\AlMm.obj" \
	"$(INTDIR)\AlMm.res" \
	"$(INTDIR)\AlMmCtl.obj" \
	"$(INTDIR)\AlMmPpg.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\AlMm.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - OLE ｺﾝﾄﾛｰﾙを登録しています...
OutDir=.\ReleaseU
TargetPath=.\ReleaseU\AlMm.ocx
InputPath=.\ReleaseU\AlMm.ocx
SOURCE=$(InputPath)

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   regsvr32 /s /c "$(TargetPath)"
   echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg"

# End Custom Build

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "AlMm - Win32 Release"
# Name "AlMm - Win32 Debug"
# Name "AlMm - Win32 Unicode Debug"
# Name "AlMm - Win32 Unicode Release"

!IF  "$(CFG)" == "AlMm - Win32 Release"

!ELSEIF  "$(CFG)" == "AlMm - Win32 Debug"

!ELSEIF  "$(CFG)" == "AlMm - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "AlMm - Win32 Unicode Release"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=A:\Altair\Src\AlMm\AlMm.rc

!IF  "$(CFG)" == "AlMm - Win32 Release"

DEP_RSC_ALMM_=\
	".\Release\AlMm.tlb"\
	"A:\Altair\Src\AlMm\AlMm.ico"\
	"A:\Altair\Src\AlMm\AlMmCtl.bmp"\
	

"$(INTDIR)\AlMm.res" : $(SOURCE) $(DEP_RSC_ALMM_) "$(INTDIR)"\
 "$(INTDIR)\AlMm.tlb"
   $(RSC) /l 0x411 /fo"$(INTDIR)/AlMm.res" /i "A:\Altair\Src\AlMm" /i "Release"\
 /d "NDEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "AlMm - Win32 Debug"

DEP_RSC_ALMM_=\
	".\Debug\AlMm.tlb"\
	"A:\Altair\Src\AlMm\AlMm.ico"\
	"A:\Altair\Src\AlMm\AlMmCtl.bmp"\
	

"$(INTDIR)\AlMm.res" : $(SOURCE) $(DEP_RSC_ALMM_) "$(INTDIR)"\
 "$(INTDIR)\AlMm.tlb"
   $(RSC) /l 0x411 /fo"$(INTDIR)/AlMm.res" /i "A:\Altair\Src\AlMm" /i "Debug"\
 /d "_DEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "AlMm - Win32 Unicode Debug"

DEP_RSC_ALMM_=\
	"A:\Altair\Src\AlMm\AlMm.ico"\
	"A:\Altair\Src\AlMm\AlMmCtl.bmp"\
	
NODEP_RSC_ALMM_=\
	".\DebugU\AlMm.tlb"\
	

"$(INTDIR)\AlMm.res" : $(SOURCE) $(DEP_RSC_ALMM_) "$(INTDIR)"\
 "$(INTDIR)\AlMm.tlb"
   $(RSC) /l 0x411 /fo"$(INTDIR)/AlMm.res" /i "A:\Altair\Src\AlMm" /i "DebugU"\
 /d "_DEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "AlMm - Win32 Unicode Release"

DEP_RSC_ALMM_=\
	"A:\Altair\Src\AlMm\AlMm.ico"\
	"A:\Altair\Src\AlMm\AlMmCtl.bmp"\
	
NODEP_RSC_ALMM_=\
	".\ReleaseU\AlMm.tlb"\
	

"$(INTDIR)\AlMm.res" : $(SOURCE) $(DEP_RSC_ALMM_) "$(INTDIR)"\
 "$(INTDIR)\AlMm.tlb"
   $(RSC) /l 0x411 /fo"$(INTDIR)/AlMm.res" /i "A:\Altair\Src\AlMm" /i\
 "ReleaseU" /d "NDEBUG" /d "_AFXDLL" $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\Altair\Src\AlMm\AlMm.cpp
DEP_CPP_ALMM_C=\
	"A:\Altair\Src\AlMm\stdafx.h"\
	{$(INCLUDE)}"\AlMm\AlMm.h"\
	

!IF  "$(CFG)" == "AlMm - Win32 Release"


"$(INTDIR)\AlMm.obj" : $(SOURCE) $(DEP_CPP_ALMM_C) "$(INTDIR)"\
 "$(INTDIR)\AlMm.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "AlMm - Win32 Debug"


"$(INTDIR)\AlMm.obj" : $(SOURCE) $(DEP_CPP_ALMM_C) "$(INTDIR)"\
 "$(INTDIR)\AlMm.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "AlMm - Win32 Unicode Debug"


"$(INTDIR)\AlMm.obj" : $(SOURCE) $(DEP_CPP_ALMM_C) "$(INTDIR)"\
 "$(INTDIR)\AlMm.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "AlMm - Win32 Unicode Release"


"$(INTDIR)\AlMm.obj" : $(SOURCE) $(DEP_CPP_ALMM_C) "$(INTDIR)"\
 "$(INTDIR)\AlMm.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\Altair\Src\AlMm\AlMmCtl.cpp
DEP_CPP_ALMMC=\
	"A:\Altair\Src\AlMm\stdafx.h"\
	{$(INCLUDE)}"\AlMm\AlMm.h"\
	{$(INCLUDE)}"\AlMm\AlMmCtl.h"\
	{$(INCLUDE)}"\AlMm\AlMmPpg.h"\
	

!IF  "$(CFG)" == "AlMm - Win32 Release"


"$(INTDIR)\AlMmCtl.obj" : $(SOURCE) $(DEP_CPP_ALMMC) "$(INTDIR)"\
 "$(INTDIR)\AlMm.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "AlMm - Win32 Debug"


"$(INTDIR)\AlMmCtl.obj" : $(SOURCE) $(DEP_CPP_ALMMC) "$(INTDIR)"\
 "$(INTDIR)\AlMm.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "AlMm - Win32 Unicode Debug"


"$(INTDIR)\AlMmCtl.obj" : $(SOURCE) $(DEP_CPP_ALMMC) "$(INTDIR)"\
 "$(INTDIR)\AlMm.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "AlMm - Win32 Unicode Release"


"$(INTDIR)\AlMmCtl.obj" : $(SOURCE) $(DEP_CPP_ALMMC) "$(INTDIR)"\
 "$(INTDIR)\AlMm.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\Altair\Src\AlMm\AlMmPpg.cpp
DEP_CPP_ALMMP=\
	"A:\Altair\Src\AlMm\stdafx.h"\
	{$(INCLUDE)}"\AlMm\AlMm.h"\
	{$(INCLUDE)}"\AlMm\AlMmPpg.h"\
	

!IF  "$(CFG)" == "AlMm - Win32 Release"


"$(INTDIR)\AlMmPpg.obj" : $(SOURCE) $(DEP_CPP_ALMMP) "$(INTDIR)"\
 "$(INTDIR)\AlMm.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "AlMm - Win32 Debug"


"$(INTDIR)\AlMmPpg.obj" : $(SOURCE) $(DEP_CPP_ALMMP) "$(INTDIR)"\
 "$(INTDIR)\AlMm.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "AlMm - Win32 Unicode Debug"


"$(INTDIR)\AlMmPpg.obj" : $(SOURCE) $(DEP_CPP_ALMMP) "$(INTDIR)"\
 "$(INTDIR)\AlMm.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "AlMm - Win32 Unicode Release"


"$(INTDIR)\AlMmPpg.obj" : $(SOURCE) $(DEP_CPP_ALMMP) "$(INTDIR)"\
 "$(INTDIR)\AlMm.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\Altair\Src\AlMm\StdAfx.cpp
DEP_CPP_STDAF=\
	"A:\Altair\Src\AlMm\stdafx.h"\
	

!IF  "$(CFG)" == "AlMm - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)/AlMm.pch"\
 /Yc"stdafx.h" /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\AlMm.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "AlMm - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fp"$(INTDIR)/AlMm.pch"\
 /Yc"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\AlMm.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "AlMm - Win32 Unicode Debug"


"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"\
 "$(INTDIR)\AlMm.pch"
   $(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Fp"$(INTDIR)/AlMm.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "AlMm - Win32 Unicode Release"


"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"\
 "$(INTDIR)\AlMm.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Fp"$(INTDIR)/AlMm.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)/" /c $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\Altair\Src\AlMm\AlMm.def

!IF  "$(CFG)" == "AlMm - Win32 Release"

!ELSEIF  "$(CFG)" == "AlMm - Win32 Debug"

!ELSEIF  "$(CFG)" == "AlMm - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "AlMm - Win32 Unicode Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\Altair\Src\AlMm\AlMm.odl

!IF  "$(CFG)" == "AlMm - Win32 Release"


"$(OUTDIR)\AlMm.tlb" : $(SOURCE) "$(OUTDIR)"
   $(MTL) /nologo /D "NDEBUG" /tlb "$(OUTDIR)/AlMm.tlb" /win32 $(SOURCE)


!ELSEIF  "$(CFG)" == "AlMm - Win32 Debug"


"$(OUTDIR)\AlMm.tlb" : $(SOURCE) "$(OUTDIR)"
   $(MTL) /nologo /D "_DEBUG" /tlb "$(OUTDIR)/AlMm.tlb" /win32 $(SOURCE)


!ELSEIF  "$(CFG)" == "AlMm - Win32 Unicode Debug"


"$(OUTDIR)\AlMm.tlb" : $(SOURCE) "$(OUTDIR)"
   $(MTL) /nologo /D "_DEBUG" /tlb "$(OUTDIR)/AlMm.tlb" /win32 $(SOURCE)


!ELSEIF  "$(CFG)" == "AlMm - Win32 Unicode Release"


"$(OUTDIR)\AlMm.tlb" : $(SOURCE) "$(OUTDIR)"
   $(MTL) /nologo /D "NDEBUG" /tlb "$(OUTDIR)/AlMm.tlb" /win32 $(SOURCE)


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################

# Microsoft Developer Studio Generated NMAKE File, Based on AlCtrl.dsp
!IF "$(CFG)" == ""
CFG=AlCtrl - Win32 Release
!MESSAGE 構成が指定されていません。ﾃﾞﾌｫﾙﾄの AlCtrl - Win32 Release を設定します。
!ENDIF 

!IF "$(CFG)" != "AlCtrl - Win32 Release" && "$(CFG)" != "AlCtrl - Win32 Debug" && "$(CFG)" != "AlCtrl - Win32 Unicode Debug" && "$(CFG)" != "AlCtrl - Win32 Unicode Release"
!MESSAGE 指定された ﾋﾞﾙﾄﾞ ﾓｰﾄﾞ "$(CFG)" は正しくありません。
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "AlCtrl.mak" CFG="AlCtrl - Win32 Release"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "AlCtrl - Win32 Release" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "AlCtrl - Win32 Debug" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "AlCtrl - Win32 Unicode Debug" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "AlCtrl - Win32 Unicode Release" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE 
!ERROR 無効な構成が指定されています。
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "AlCtrl - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\AlCtrl.ocx" ".\Release\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\AlCtrl.obj"
	-@erase "$(INTDIR)\AlCtrl.pch"
	-@erase "$(INTDIR)\AlCtrl.res"
	-@erase "$(INTDIR)\AlCtrl.tlb"
	-@erase "$(INTDIR)\alctrlctl.obj"
	-@erase "$(INTDIR)\AlCtrlPpg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\AlCtrl.exp"
	-@erase "$(OUTDIR)\AlCtrl.lib"
	-@erase "$(OUTDIR)\AlCtrl.ocx"
	-@erase ".\Release\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "NDEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "AL_LANG_JAPANESE" /Fp"$(INTDIR)\AlCtrl.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x411 /fo"$(INTDIR)\AlCtrl.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\AlCtrl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=alcore1.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\AlCtrl.pdb" /machine:I386 /def:"..\..\Src\AlCtrl\AlCtrl.def" /out:"$(OUTDIR)\AlCtrl.ocx" /implib:"$(OUTDIR)\AlCtrl.lib" 
DEF_FILE= \
	"..\..\Src\AlCtrl\AlCtrl.def"
LINK32_OBJS= \
	"$(INTDIR)\AlCtrl.obj" \
	"$(INTDIR)\alctrlctl.obj" \
	"$(INTDIR)\AlCtrlPpg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\AlCtrl.res"

"$(OUTDIR)\AlCtrl.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Release
TargetPath=.\Release\AlCtrl.ocx
InputPath=.\Release\AlCtrl.ocx
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "AlCtrl - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\AlCtrl.ocx" "$(OUTDIR)\AlCtrl.tlb" ".\Debug\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\AlCtrl.obj"
	-@erase "$(INTDIR)\AlCtrl.pch"
	-@erase "$(INTDIR)\AlCtrl.res"
	-@erase "$(INTDIR)\AlCtrl.tlb"
	-@erase "$(INTDIR)\alctrlctl.obj"
	-@erase "$(INTDIR)\AlCtrlPpg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\AlCtrl.exp"
	-@erase "$(OUTDIR)\AlCtrl.ilk"
	-@erase "$(OUTDIR)\AlCtrl.lib"
	-@erase "$(OUTDIR)\AlCtrl.ocx"
	-@erase "$(OUTDIR)\AlCtrl.pdb"
	-@erase ".\Debug\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "AL_LANG_JAPANESE" /Fp"$(INTDIR)\AlCtrl.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x411 /fo"$(INTDIR)\AlCtrl.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\AlCtrl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=alcore1d.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\AlCtrl.pdb" /debug /machine:I386 /def:"..\..\Src\AlCtrl\AlCtrl.def" /out:"$(OUTDIR)\AlCtrl.ocx" /implib:"$(OUTDIR)\AlCtrl.lib" 
DEF_FILE= \
	"..\..\Src\AlCtrl\AlCtrl.def"
LINK32_OBJS= \
	"$(INTDIR)\AlCtrl.obj" \
	"$(INTDIR)\alctrlctl.obj" \
	"$(INTDIR)\AlCtrlPpg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\AlCtrl.res"

"$(OUTDIR)\AlCtrl.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\Debug
TargetPath=.\Debug\AlCtrl.ocx
InputPath=.\Debug\AlCtrl.ocx
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "AlCtrl - Win32 Unicode Debug"

OUTDIR=.\DebugU
INTDIR=.\DebugU
# Begin Custom Macros
OutDir=.\DebugU
# End Custom Macros

ALL : "$(OUTDIR)\AlCtrl.ocx" "$(OUTDIR)\AlCtrl.tlb" ".\DebugU\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\AlCtrl.obj"
	-@erase "$(INTDIR)\AlCtrl.pch"
	-@erase "$(INTDIR)\AlCtrl.res"
	-@erase "$(INTDIR)\AlCtrl.tlb"
	-@erase "$(INTDIR)\alctrlctl.obj"
	-@erase "$(INTDIR)\AlCtrlPpg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\AlCtrl.exp"
	-@erase "$(OUTDIR)\AlCtrl.ilk"
	-@erase "$(OUTDIR)\AlCtrl.lib"
	-@erase "$(OUTDIR)\AlCtrl.ocx"
	-@erase "$(OUTDIR)\AlCtrl.pdb"
	-@erase ".\DebugU\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "AL_LANG_JAPANESE" /Fp"$(INTDIR)\AlCtrl.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x411 /fo"$(INTDIR)\AlCtrl.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\AlCtrl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\AlCtrl.pdb" /debug /machine:I386 /def:"..\..\Src\AlCtrl\AlCtrl.def" /out:"$(OUTDIR)\AlCtrl.ocx" /implib:"$(OUTDIR)\AlCtrl.lib" 
DEF_FILE= \
	"..\..\Src\AlCtrl\AlCtrl.def"
LINK32_OBJS= \
	"$(INTDIR)\AlCtrl.obj" \
	"$(INTDIR)\alctrlctl.obj" \
	"$(INTDIR)\AlCtrlPpg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\AlCtrl.res"

"$(OUTDIR)\AlCtrl.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\DebugU
TargetPath=.\DebugU\AlCtrl.ocx
InputPath=.\DebugU\AlCtrl.ocx
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ELSEIF  "$(CFG)" == "AlCtrl - Win32 Unicode Release"

OUTDIR=.\ReleaseU
INTDIR=.\ReleaseU
# Begin Custom Macros
OutDir=.\ReleaseU
# End Custom Macros

ALL : "$(OUTDIR)\AlCtrl.ocx" "$(OUTDIR)\AlCtrl.tlb" ".\ReleaseU\regsvr32.trg"


CLEAN :
	-@erase "$(INTDIR)\AlCtrl.obj"
	-@erase "$(INTDIR)\AlCtrl.pch"
	-@erase "$(INTDIR)\AlCtrl.res"
	-@erase "$(INTDIR)\AlCtrl.tlb"
	-@erase "$(INTDIR)\alctrlctl.obj"
	-@erase "$(INTDIR)\AlCtrlPpg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\AlCtrl.exp"
	-@erase "$(OUTDIR)\AlCtrl.lib"
	-@erase "$(OUTDIR)\AlCtrl.ocx"
	-@erase ".\ReleaseU\regsvr32.trg"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "NDEBUG" /D "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "AL_LANG_JAPANESE" /Fp"$(INTDIR)\AlCtrl.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x411 /fo"$(INTDIR)\AlCtrl.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\AlCtrl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\AlCtrl.pdb" /machine:I386 /def:"..\..\Src\AlCtrl\AlCtrl.def" /out:"$(OUTDIR)\AlCtrl.ocx" /implib:"$(OUTDIR)\AlCtrl.lib" 
DEF_FILE= \
	"..\..\Src\AlCtrl\AlCtrl.def"
LINK32_OBJS= \
	"$(INTDIR)\AlCtrl.obj" \
	"$(INTDIR)\alctrlctl.obj" \
	"$(INTDIR)\AlCtrlPpg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\AlCtrl.res"

"$(OUTDIR)\AlCtrl.ocx" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

OutDir=.\ReleaseU
TargetPath=.\ReleaseU\AlCtrl.ocx
InputPath=.\ReleaseU\AlCtrl.ocx
SOURCE="$(InputPath)"

"$(OUTDIR)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
<< 
	

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("AlCtrl.dep")
!INCLUDE "AlCtrl.dep"
!ELSE 
!MESSAGE Warning: cannot find "AlCtrl.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "AlCtrl - Win32 Release" || "$(CFG)" == "AlCtrl - Win32 Debug" || "$(CFG)" == "AlCtrl - Win32 Unicode Debug" || "$(CFG)" == "AlCtrl - Win32 Unicode Release"
SOURCE=..\..\Src\AlCtrl\AlCtrl.cpp

"$(INTDIR)\AlCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\AlCtrl.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\AlCtrl\AlCtrl.odl

!IF  "$(CFG)" == "AlCtrl - Win32 Release"

MTL_SWITCHES=/nologo /D "NDEBUG" /tlb "$(OUTDIR)\AlCtrl.tlb" /mktyplib203 /win32 

"$(OUTDIR)\AlCtrl.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "AlCtrl - Win32 Debug"

MTL_SWITCHES=/nologo /D "_DEBUG" /tlb "$(OUTDIR)\AlCtrl.tlb" /mktyplib203 /win32 

"$(OUTDIR)\AlCtrl.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "AlCtrl - Win32 Unicode Debug"

MTL_SWITCHES=/nologo /D "_DEBUG" /tlb "$(OUTDIR)\AlCtrl.tlb" /mktyplib203 /win32 

"$(OUTDIR)\AlCtrl.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "AlCtrl - Win32 Unicode Release"

MTL_SWITCHES=/nologo /D "NDEBUG" /tlb "$(OUTDIR)\AlCtrl.tlb" /mktyplib203 /win32 

"$(OUTDIR)\AlCtrl.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\Src\AlCtrl\AlCtrl.rc

!IF  "$(CFG)" == "AlCtrl - Win32 Release"


"$(INTDIR)\AlCtrl.res" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\AlCtrl.tlb"
	$(RSC) /l 0x411 /fo"$(INTDIR)\AlCtrl.res" /i "\Altair\Src\AlCtrl" /i ".\Release" /d "NDEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "AlCtrl - Win32 Debug"


"$(INTDIR)\AlCtrl.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x411 /fo"$(INTDIR)\AlCtrl.res" /i "\Altair\Src\AlCtrl" /i ".\Debug" /d "_DEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "AlCtrl - Win32 Unicode Debug"


"$(INTDIR)\AlCtrl.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x411 /fo"$(INTDIR)\AlCtrl.res" /i "\Altair\Src\AlCtrl" /i ".\DebugU" /d "_DEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "AlCtrl - Win32 Unicode Release"


"$(INTDIR)\AlCtrl.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x411 /fo"$(INTDIR)\AlCtrl.res" /i "\Altair\Src\AlCtrl" /i ".\ReleaseU" /d "NDEBUG" /d "_AFXDLL" $(SOURCE)


!ENDIF 

SOURCE=..\..\Src\AlCtrl\alctrlctl.cpp

"$(INTDIR)\alctrlctl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\AlCtrl.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\AlCtrl\AlCtrlPpg.cpp

"$(INTDIR)\AlCtrlPpg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\AlCtrl.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\AlCtrl\StdAfx.cpp

!IF  "$(CFG)" == "AlCtrl - Win32 Release"

CPP_SWITCHES=/nologo /MD /W4 /GX /O2 /D "NDEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "AL_LANG_JAPANESE" /Fp"$(INTDIR)\AlCtrl.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\AlCtrl.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "AlCtrl - Win32 Debug"

CPP_SWITCHES=/nologo /MD /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "AL_LANG_JAPANESE" /Fp"$(INTDIR)\AlCtrl.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\AlCtrl.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "AlCtrl - Win32 Unicode Debug"

CPP_SWITCHES=/nologo /MD /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "AL_LANG_JAPANESE" /Fp"$(INTDIR)\AlCtrl.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\AlCtrl.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "AlCtrl - Win32 Unicode Release"

CPP_SWITCHES=/nologo /MD /W4 /GX /O2 /D "NDEBUG" /D "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "AL_LANG_JAPANESE" /Fp"$(INTDIR)\AlCtrl.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\AlCtrl.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 


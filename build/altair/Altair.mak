# Microsoft Developer Studio Generated NMAKE File, Based on Altair.dsp
!IF "$(CFG)" == ""
CFG=Altair - Win32 Debug
!MESSAGE 構成が指定されていません。ﾃﾞﾌｫﾙﾄの Altair - Win32 Debug を設定します。
!ENDIF 

!IF "$(CFG)" != "Altair - Win32 Release" && "$(CFG)" != "Altair - Win32 Debug"
!MESSAGE 指定された ﾋﾞﾙﾄﾞ ﾓｰﾄﾞ "$(CFG)" は正しくありません。
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "Altair.mak" CFG="Altair - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "Altair - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "Altair - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE 
!ERROR 無効な構成が指定されています。
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Altair - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Altair.exe" "$(OUTDIR)\Altair.tlb"


CLEAN :
	-@erase "$(INTDIR)\Altair.obj"
	-@erase "$(INTDIR)\Altair.pch"
	-@erase "$(INTDIR)\Altair.res"
	-@erase "$(INTDIR)\Altair.tlb"
	-@erase "$(INTDIR)\AltairDoc.obj"
	-@erase "$(INTDIR)\AltairView.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Altair.exe"
	-@erase "$(OUTDIR)\Altair.map"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "AL_USE_DLL" /D "AL_USE_PCH" /Fp"$(INTDIR)\Altair.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 
RSC=rc.exe
RSC_PROJ=/l 0x411 /fo"$(INTDIR)\Altair.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Altair.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=Alcore1.lib /nologo /base:"0x20000000" /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\Altair.pdb" /map:"$(INTDIR)\Altair.map" /machine:I386 /out:"$(OUTDIR)\Altair.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Altair.obj" \
	"$(INTDIR)\AltairDoc.obj" \
	"$(INTDIR)\AltairView.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Altair.res"

"$(OUTDIR)\Altair.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Altair - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\Altaird.exe" "$(OUTDIR)\Altair.tlb"


CLEAN :
	-@erase "$(INTDIR)\Altair.obj"
	-@erase "$(INTDIR)\Altair.pch"
	-@erase "$(INTDIR)\Altair.res"
	-@erase "$(INTDIR)\Altair.tlb"
	-@erase "$(INTDIR)\AltairDoc.obj"
	-@erase "$(INTDIR)\AltairView.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Altaird.exe"
	-@erase "$(OUTDIR)\Altaird.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W4 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "AL_USE_DLL" /D "AL_USE_PCH" /Fp"$(INTDIR)\Altair.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 
RSC=rc.exe
RSC_PROJ=/l 0x411 /fo"$(INTDIR)\Altair.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Altair.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=Alcore1d.lib /nologo /base:"0x20000000" /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\Altaird.pdb" /debug /machine:I386 /out:"$(OUTDIR)\Altaird.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Altair.obj" \
	"$(INTDIR)\AltairDoc.obj" \
	"$(INTDIR)\AltairView.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Altair.res"

"$(OUTDIR)\Altaird.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Altair.dep")
!INCLUDE "Altair.dep"
!ELSE 
!MESSAGE Warning: cannot find "Altair.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Altair - Win32 Release" || "$(CFG)" == "Altair - Win32 Debug"
SOURCE=..\Src\Altair\Altair.cpp

"$(INTDIR)\Altair.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Altair.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Altair\Altair.odl

!IF  "$(CFG)" == "Altair - Win32 Release"

MTL_SWITCHES=/nologo /D "NDEBUG" /tlb "$(OUTDIR)\Altair.tlb" /mktyplib203 /o "NUL" /win32 

"$(OUTDIR)\Altair.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Altair - Win32 Debug"

MTL_SWITCHES=/nologo /D "_DEBUG" /tlb "$(OUTDIR)\Altair.tlb" /mktyplib203 /o "NUL" /win32 

"$(OUTDIR)\Altair.tlb" : $(SOURCE) "$(OUTDIR)"
	$(MTL) @<<
  $(MTL_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Src\Altair\res\Altair.RC

!IF  "$(CFG)" == "Altair - Win32 Release"


"$(INTDIR)\Altair.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x411 /fo"$(INTDIR)\Altair.res" /i "\Altair\Src\Altair\res" /i "Release" /d "NDEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "Altair - Win32 Debug"


"$(INTDIR)\Altair.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x411 /fo"$(INTDIR)\Altair.res" /i "\Altair\Src\Altair\res" /i "Debug" /d "_DEBUG" /d "_AFXDLL" $(SOURCE)


!ENDIF 

SOURCE=..\Src\Altair\AltairDoc.cpp

"$(INTDIR)\AltairDoc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Altair.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Altair\AltairView.cpp

"$(INTDIR)\AltairView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Altair.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Altair\StdAfx.cpp

!IF  "$(CFG)" == "Altair - Win32 Release"

CPP_SWITCHES=/nologo /MD /W4 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "AL_USE_DLL" /D "AL_USE_PCH" /Fp"$(INTDIR)\Altair.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Altair.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Altair - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W4 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "AL_USE_DLL" /D "AL_USE_PCH" /Fp"$(INTDIR)\Altair.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Altair.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 


# Microsoft Developer Studio Generated NMAKE File, Based on Alcore1d.dsp
!IF "$(CFG)" == ""
CFG=Alcore1d - Win32 Release
!MESSAGE 構成が指定されていません。ﾃﾞﾌｫﾙﾄの Alcore1d - Win32 Release を設定します。
!ENDIF 

!IF "$(CFG)" != "Alcore1d - Win32 Release" && "$(CFG)" != "Alcore1d - Win32 Debug"
!MESSAGE 指定された ﾋﾞﾙﾄﾞ ﾓｰﾄﾞ "$(CFG)" は正しくありません。
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "Alcore1d.mak" CFG="Alcore1d - Win32 Release"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "Alcore1d - Win32 Release" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "Alcore1d - Win32 Debug" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE 
!ERROR 無効な構成が指定されています。
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

OUTDIR=.\WinRel
INTDIR=.\WinRel
# Begin Custom Macros
OutDir=.\WinRel
# End Custom Macros

ALL : "$(OUTDIR)\Alcore1.dll"


CLEAN :
	-@erase "$(INTDIR)\Advcapp.obj"
	-@erase "$(INTDIR)\Advcedit.obj"
	-@erase "$(INTDIR)\Advcvw.obj"
	-@erase "$(INTDIR)\Advwnd.obj"
	-@erase "$(INTDIR)\Alcore.obj"
	-@erase "$(INTDIR)\Alcore1.res"
	-@erase "$(INTDIR)\Alcore1d.pch"
	-@erase "$(INTDIR)\big_int.obj"
	-@erase "$(INTDIR)\Bitmap.obj"
	-@erase "$(INTDIR)\Body.obj"
	-@erase "$(INTDIR)\Body_in.obj"
	-@erase "$(INTDIR)\Body_out.obj"
	-@erase "$(INTDIR)\Class.obj"
	-@erase "$(INTDIR)\Copy.obj"
	-@erase "$(INTDIR)\crypt.obj"
	-@erase "$(INTDIR)\Cui.obj"
	-@erase "$(INTDIR)\Debug.obj"
	-@erase "$(INTDIR)\dllcall.obj"
	-@erase "$(INTDIR)\Error.obj"
	-@erase "$(INTDIR)\File.obj"
	-@erase "$(INTDIR)\Gc.obj"
	-@erase "$(INTDIR)\Gdb_intp.obj"
	-@erase "$(INTDIR)\Gdm.obj"
	-@erase "$(INTDIR)\Gp_intp.obj"
	-@erase "$(INTDIR)\Interp1.obj"
	-@erase "$(INTDIR)\Interp2.obj"
	-@erase "$(INTDIR)\j_code.obj"
	-@erase "$(INTDIR)\Mainfrm.obj"
	-@erase "$(INTDIR)\Matrix.obj"
	-@erase "$(INTDIR)\Member.obj"
	-@erase "$(INTDIR)\Ocx.obj"
	-@erase "$(INTDIR)\olectrl.obj"
	-@erase "$(INTDIR)\Parallel.obj"
	-@erase "$(INTDIR)\Pict.obj"
	-@erase "$(INTDIR)\Prcsmgr.obj"
	-@erase "$(INTDIR)\Prog_if.obj"
	-@erase "$(INTDIR)\Project.obj"
	-@erase "$(INTDIR)\Shell.obj"
	-@erase "$(INTDIR)\socket.obj"
	-@erase "$(INTDIR)\Stdafx.obj"
	-@erase "$(INTDIR)\String.obj"
	-@erase "$(INTDIR)\Sysfunc1.obj"
	-@erase "$(INTDIR)\Sysfunc2.obj"
	-@erase "$(INTDIR)\Sysfunc3.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\xml.obj"
	-@erase "$(OUTDIR)\Alcore1.dll"
	-@erase "$(OUTDIR)\Alcore1.exp"
	-@erase "$(OUTDIR)\Alcore1.lib"
	-@erase "$(OUTDIR)\Alcore1.map"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W4 /GX /Gf /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_MBCS" /D "AL_CORE_DLL" /D "AL_LANG_JAPANESE" /D "AL_USE_PCH" /Fp"$(INTDIR)\Alcore1d.pch" /Yu"STDAFX.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x411 /fo"$(INTDIR)\Alcore1.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Alcore1d.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ws2_32.lib netapi32.lib /nologo /base:"0x21000000" /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\Alcore1.pdb" /map:"$(INTDIR)\Alcore1.map" /machine:IX86 /def:".\ALCORE1.DEF" /out:"$(OUTDIR)\Alcore1.dll" /implib:"$(OUTDIR)\Alcore1.lib" 
DEF_FILE= \
	".\ALCORE1.DEF"
LINK32_OBJS= \
	"$(INTDIR)\Advcapp.obj" \
	"$(INTDIR)\Advcedit.obj" \
	"$(INTDIR)\Advcvw.obj" \
	"$(INTDIR)\Advwnd.obj" \
	"$(INTDIR)\Alcore.obj" \
	"$(INTDIR)\big_int.obj" \
	"$(INTDIR)\Bitmap.obj" \
	"$(INTDIR)\Body.obj" \
	"$(INTDIR)\Body_in.obj" \
	"$(INTDIR)\Body_out.obj" \
	"$(INTDIR)\Class.obj" \
	"$(INTDIR)\Copy.obj" \
	"$(INTDIR)\crypt.obj" \
	"$(INTDIR)\Cui.obj" \
	"$(INTDIR)\Debug.obj" \
	"$(INTDIR)\dllcall.obj" \
	"$(INTDIR)\Error.obj" \
	"$(INTDIR)\File.obj" \
	"$(INTDIR)\Gc.obj" \
	"$(INTDIR)\Gdb_intp.obj" \
	"$(INTDIR)\Gdm.obj" \
	"$(INTDIR)\Gp_intp.obj" \
	"$(INTDIR)\Interp1.obj" \
	"$(INTDIR)\Interp2.obj" \
	"$(INTDIR)\j_code.obj" \
	"$(INTDIR)\Mainfrm.obj" \
	"$(INTDIR)\Matrix.obj" \
	"$(INTDIR)\Member.obj" \
	"$(INTDIR)\Ocx.obj" \
	"$(INTDIR)\olectrl.obj" \
	"$(INTDIR)\Parallel.obj" \
	"$(INTDIR)\Pict.obj" \
	"$(INTDIR)\Prcsmgr.obj" \
	"$(INTDIR)\Prog_if.obj" \
	"$(INTDIR)\Project.obj" \
	"$(INTDIR)\Shell.obj" \
	"$(INTDIR)\socket.obj" \
	"$(INTDIR)\Stdafx.obj" \
	"$(INTDIR)\String.obj" \
	"$(INTDIR)\Sysfunc1.obj" \
	"$(INTDIR)\Sysfunc2.obj" \
	"$(INTDIR)\Sysfunc3.obj" \
	"$(INTDIR)\xml.obj" \
	"$(INTDIR)\Alcore1.res"

"$(OUTDIR)\Alcore1.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

OUTDIR=.\WinDebug
INTDIR=.\WinDebug
# Begin Custom Macros
OutDir=.\WinDebug
# End Custom Macros

ALL : "$(OUTDIR)\Alcore1d.dll"


CLEAN :
	-@erase "$(INTDIR)\Advcapp.obj"
	-@erase "$(INTDIR)\Advcedit.obj"
	-@erase "$(INTDIR)\Advcvw.obj"
	-@erase "$(INTDIR)\Advwnd.obj"
	-@erase "$(INTDIR)\Alcore.obj"
	-@erase "$(INTDIR)\Alcore1.res"
	-@erase "$(INTDIR)\Alcore1d.pch"
	-@erase "$(INTDIR)\big_int.obj"
	-@erase "$(INTDIR)\Bitmap.obj"
	-@erase "$(INTDIR)\Body.obj"
	-@erase "$(INTDIR)\Body_in.obj"
	-@erase "$(INTDIR)\Body_out.obj"
	-@erase "$(INTDIR)\Class.obj"
	-@erase "$(INTDIR)\Copy.obj"
	-@erase "$(INTDIR)\crypt.obj"
	-@erase "$(INTDIR)\Cui.obj"
	-@erase "$(INTDIR)\Debug.obj"
	-@erase "$(INTDIR)\dllcall.obj"
	-@erase "$(INTDIR)\Error.obj"
	-@erase "$(INTDIR)\File.obj"
	-@erase "$(INTDIR)\Gc.obj"
	-@erase "$(INTDIR)\Gdb_intp.obj"
	-@erase "$(INTDIR)\Gdm.obj"
	-@erase "$(INTDIR)\Gp_intp.obj"
	-@erase "$(INTDIR)\Interp1.obj"
	-@erase "$(INTDIR)\Interp2.obj"
	-@erase "$(INTDIR)\j_code.obj"
	-@erase "$(INTDIR)\Mainfrm.obj"
	-@erase "$(INTDIR)\Matrix.obj"
	-@erase "$(INTDIR)\Member.obj"
	-@erase "$(INTDIR)\Ocx.obj"
	-@erase "$(INTDIR)\olectrl.obj"
	-@erase "$(INTDIR)\Parallel.obj"
	-@erase "$(INTDIR)\Pict.obj"
	-@erase "$(INTDIR)\Prcsmgr.obj"
	-@erase "$(INTDIR)\Prog_if.obj"
	-@erase "$(INTDIR)\Project.obj"
	-@erase "$(INTDIR)\Shell.obj"
	-@erase "$(INTDIR)\socket.obj"
	-@erase "$(INTDIR)\Stdafx.obj"
	-@erase "$(INTDIR)\String.obj"
	-@erase "$(INTDIR)\Sysfunc1.obj"
	-@erase "$(INTDIR)\Sysfunc2.obj"
	-@erase "$(INTDIR)\Sysfunc3.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\xml.obj"
	-@erase "$(OUTDIR)\Alcore1d.dll"
	-@erase "$(OUTDIR)\Alcore1d.exp"
	-@erase "$(OUTDIR)\Alcore1d.ilk"
	-@erase "$(OUTDIR)\Alcore1d.lib"
	-@erase "$(OUTDIR)\Alcore1d.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W4 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_MBCS" /D "AL_CORE_DLL" /D "AL_LANG_JAPANESE" /D "AL_USE_PCH" /Fp"$(INTDIR)\Alcore1d.pch" /Yu"STDAFX.H" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x411 /fo"$(INTDIR)\Alcore1.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Alcore1d.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ws2_32.lib netapi32.lib /nologo /base:"0x21000000" /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\Alcore1d.pdb" /debug /machine:IX86 /nodefaultlib:"MSVCRT" /def:".\ALCORE1D.DEF" /out:"$(OUTDIR)\Alcore1d.dll" /implib:"$(OUTDIR)\Alcore1d.lib" 
DEF_FILE= \
	".\ALCORE1D.DEF"
LINK32_OBJS= \
	"$(INTDIR)\Advcapp.obj" \
	"$(INTDIR)\Advcedit.obj" \
	"$(INTDIR)\Advcvw.obj" \
	"$(INTDIR)\Advwnd.obj" \
	"$(INTDIR)\Alcore.obj" \
	"$(INTDIR)\big_int.obj" \
	"$(INTDIR)\Bitmap.obj" \
	"$(INTDIR)\Body.obj" \
	"$(INTDIR)\Body_in.obj" \
	"$(INTDIR)\Body_out.obj" \
	"$(INTDIR)\Class.obj" \
	"$(INTDIR)\Copy.obj" \
	"$(INTDIR)\crypt.obj" \
	"$(INTDIR)\Cui.obj" \
	"$(INTDIR)\Debug.obj" \
	"$(INTDIR)\dllcall.obj" \
	"$(INTDIR)\Error.obj" \
	"$(INTDIR)\File.obj" \
	"$(INTDIR)\Gc.obj" \
	"$(INTDIR)\Gdb_intp.obj" \
	"$(INTDIR)\Gdm.obj" \
	"$(INTDIR)\Gp_intp.obj" \
	"$(INTDIR)\Interp1.obj" \
	"$(INTDIR)\Interp2.obj" \
	"$(INTDIR)\j_code.obj" \
	"$(INTDIR)\Mainfrm.obj" \
	"$(INTDIR)\Matrix.obj" \
	"$(INTDIR)\Member.obj" \
	"$(INTDIR)\Ocx.obj" \
	"$(INTDIR)\olectrl.obj" \
	"$(INTDIR)\Parallel.obj" \
	"$(INTDIR)\Pict.obj" \
	"$(INTDIR)\Prcsmgr.obj" \
	"$(INTDIR)\Prog_if.obj" \
	"$(INTDIR)\Project.obj" \
	"$(INTDIR)\Shell.obj" \
	"$(INTDIR)\socket.obj" \
	"$(INTDIR)\Stdafx.obj" \
	"$(INTDIR)\String.obj" \
	"$(INTDIR)\Sysfunc1.obj" \
	"$(INTDIR)\Sysfunc2.obj" \
	"$(INTDIR)\Sysfunc3.obj" \
	"$(INTDIR)\xml.obj" \
	"$(INTDIR)\Alcore1.res"

"$(OUTDIR)\Alcore1d.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Alcore1d.dep")
!INCLUDE "Alcore1d.dep"
!ELSE 
!MESSAGE Warning: cannot find "Alcore1d.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Alcore1d - Win32 Release" || "$(CFG)" == "Alcore1d - Win32 Debug"
SOURCE=..\..\Src\Dev\Advcapp.cpp

"$(INTDIR)\Advcapp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Dev\Advcedit.cpp

"$(INTDIR)\Advcedit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Dev\Advcvw.cpp

"$(INTDIR)\Advcvw.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Dev\Advwnd.cpp

"$(INTDIR)\Advwnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\General\Alcore.cpp

"$(INTDIR)\Alcore.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Res\Alcore1.rc

!IF  "$(CFG)" == "Alcore1d - Win32 Release"


"$(INTDIR)\Alcore1.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x411 /fo"$(INTDIR)\Alcore1.res" /i "\Altair\Src\Res" /d "NDEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"


"$(INTDIR)\Alcore1.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x411 /fo"$(INTDIR)\Alcore1.res" /i "\Altair\Src\Res" /d "_DEBUG" /d "_AFXDLL" $(SOURCE)


!ENDIF 

SOURCE=..\..\Src\Gdm\big_int.cpp

"$(INTDIR)\big_int.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Dev\Bitmap.cpp

"$(INTDIR)\Bitmap.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Cui\Body.cpp

"$(INTDIR)\Body.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Cui\Body_in.cpp

"$(INTDIR)\Body_in.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Cui\Body_out.cpp

"$(INTDIR)\Body_out.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Cui\Class.cpp

"$(INTDIR)\Class.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Gdm\Copy.cpp

"$(INTDIR)\Copy.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Gdm\crypt.cpp

"$(INTDIR)\crypt.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Cui\Cui.cpp

"$(INTDIR)\Cui.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Gdm\Debug.cpp

"$(INTDIR)\Debug.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Dev\dllcall.cpp

"$(INTDIR)\dllcall.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Gdm\Error.cpp

"$(INTDIR)\Error.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Dev\File.cpp

"$(INTDIR)\File.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Gdm\Gc.cpp

"$(INTDIR)\Gc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Interp\Gdb_intp.cpp

"$(INTDIR)\Gdb_intp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Gdm\Gdm.cpp

"$(INTDIR)\Gdm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Interp\Gp_intp.cpp

"$(INTDIR)\Gp_intp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Interp\Interp1.cpp

"$(INTDIR)\Interp1.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Interp\Interp2.cpp

"$(INTDIR)\Interp2.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Gdm\j_code.cpp

"$(INTDIR)\j_code.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Dev\Mainfrm.cpp

"$(INTDIR)\Mainfrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Gdm\Matrix.cpp

"$(INTDIR)\Matrix.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Cui\Member.cpp

"$(INTDIR)\Member.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Dev\Ocx.cpp

"$(INTDIR)\Ocx.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Dev\olectrl.cpp

"$(INTDIR)\olectrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Interp\Parallel.cpp

"$(INTDIR)\Parallel.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Gdm\Pict.cpp

"$(INTDIR)\Pict.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Interp\Prcsmgr.cpp

"$(INTDIR)\Prcsmgr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Interp\Prog_if.cpp

"$(INTDIR)\Prog_if.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Cui\Project.cpp

"$(INTDIR)\Project.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Dev\Shell.cpp

"$(INTDIR)\Shell.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Dev\socket.cpp

"$(INTDIR)\socket.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\General\Stdafx.cpp

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

CPP_SWITCHES=/nologo /MD /W4 /GX /Gf /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_MBCS" /D "AL_CORE_DLL" /D "AL_LANG_JAPANESE" /D "AL_USE_PCH" /Fp"$(INTDIR)\Alcore1d.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Stdafx.obj"	"$(INTDIR)\Alcore1d.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

CPP_SWITCHES=/nologo /MD /W4 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_MBCS" /D "AL_CORE_DLL" /D "AL_LANG_JAPANESE" /D "AL_USE_PCH" /Fp"$(INTDIR)\Alcore1d.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Stdafx.obj"	"$(INTDIR)\Alcore1d.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\Src\Gdm\String.cpp

"$(INTDIR)\String.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Interp\Sysfunc1.cpp

"$(INTDIR)\Sysfunc1.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Interp\Sysfunc2.cpp

"$(INTDIR)\Sysfunc2.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Interp\Sysfunc3.cpp

"$(INTDIR)\Sysfunc3.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Src\Xml\xml.cpp

"$(INTDIR)\xml.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Alcore1d.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 


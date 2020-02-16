# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=Alcore1d - Win32 Debug
!MESSAGE 構成が指定されていません。ﾃﾞﾌｫﾙﾄの Alcore1d - Win32 Debug を設定します。
!ENDIF 

!IF "$(CFG)" != "Alcore1d - Win32 Release" && "$(CFG)" !=\
 "Alcore1d - Win32 Debug"
!MESSAGE 指定された ﾋﾞﾙﾄﾞ ﾓｰﾄﾞ "$(CFG)" は正しくありません。
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛ 'CFG' を定義することによって
!MESSAGE NMAKE 実行時にﾋﾞﾙﾄﾞ ﾓｰﾄﾞを指定できます。例えば:
!MESSAGE 
!MESSAGE NMAKE /f "Alcore1d.mak" CFG="Alcore1d - Win32 Debug"
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
################################################################################
# Begin Project
# PROP Target_Last_Scanned "Alcore1d - Win32 Release"
RSC=rc.exe
MTL=mktyplib.exe
CPP=cl.exe

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WinRel"
# PROP Intermediate_Dir "WinRel"
OUTDIR=.\WinRel
INTDIR=.\WinRel

ALL : "$(OUTDIR)\Alcore1e.dll"

CLEAN : 
	-@erase "$(INTDIR)\ADVCAPP.OBJ"
	-@erase "$(INTDIR)\ADVCEDIT.OBJ"
	-@erase "$(INTDIR)\ADVCVW.OBJ"
	-@erase "$(INTDIR)\ADVWND.OBJ"
	-@erase "$(INTDIR)\ALCORE.OBJ"
	-@erase "$(INTDIR)\Alcore1.res"
	-@erase "$(INTDIR)\Alcore1d.pch"
	-@erase "$(INTDIR)\BITMAP.OBJ"
	-@erase "$(INTDIR)\BODY.OBJ"
	-@erase "$(INTDIR)\BODY_IN.OBJ"
	-@erase "$(INTDIR)\BODY_OUT.OBJ"
	-@erase "$(INTDIR)\CLASS.OBJ"
	-@erase "$(INTDIR)\COPY.OBJ"
	-@erase "$(INTDIR)\CUI.OBJ"
	-@erase "$(INTDIR)\DEBUG.OBJ"
	-@erase "$(INTDIR)\ERROR.OBJ"
	-@erase "$(INTDIR)\File.obj"
	-@erase "$(INTDIR)\GC.OBJ"
	-@erase "$(INTDIR)\Gdb_intp.obj"
	-@erase "$(INTDIR)\GDM.OBJ"
	-@erase "$(INTDIR)\GP_INTP.OBJ"
	-@erase "$(INTDIR)\INTERP.OBJ"
	-@erase "$(INTDIR)\Mainfrm.obj"
	-@erase "$(INTDIR)\MATRIX.OBJ"
	-@erase "$(INTDIR)\MEMBER.OBJ"
	-@erase "$(INTDIR)\ocx.obj"
	-@erase "$(INTDIR)\olectrl.obj"
	-@erase "$(INTDIR)\PARALLEL.OBJ"
	-@erase "$(INTDIR)\PICT.OBJ"
	-@erase "$(INTDIR)\PRCSMGR.OBJ"
	-@erase "$(INTDIR)\PROG_IF.OBJ"
	-@erase "$(INTDIR)\PROJECT.OBJ"
	-@erase "$(INTDIR)\SHELL.OBJ"
	-@erase "$(INTDIR)\Stdafx.obj"
	-@erase "$(INTDIR)\STRING.OBJ"
	-@erase "$(INTDIR)\SYSFUNC.OBJ"
	-@erase "$(OUTDIR)\Alcore1e.dll"
	-@erase "$(OUTDIR)\Alcore1e.exp"
	-@erase "$(OUTDIR)\Alcore1e.lib"
	-@erase "$(OUTDIR)\Alcore1e.map"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

$(OUTDIR)/ALCORE1D.bsc : $(OUTDIR)  $(BSC32_SBRS)
# ADD BASE CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /MD /W4 /GX /Gf /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_MBCS" /D "AL_CORE_DLL" /D "AL_LANG_ENGLISH" /D "AL_USE_PCH" /D "AL_OCX" /Yu"STDAFX.H" /c
# SUBTRACT CPP /Gy /Fr
CPP_PROJ=/nologo /MD /W4 /GX /Gf /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_MBCS" /D "AL_CORE_DLL" /D\
 "AL_LANG_ENGLISH" /D "AL_USE_PCH" /D "AL_OCX" /Fp"$(INTDIR)/Alcore1d.pch"\
 /Yu"STDAFX.H" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\WinRel/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x411 /fo"$(INTDIR)/Alcore1.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Alcore1d.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 oldnames.lib ldllcew.lib winnls.lib /nologo /subsystem:windows /dll /map:"FULL" /machine:IX86
# ADD LINK32 /nologo /subsystem:windows /dll /pdb:none /map /machine:IX86 /out:"WinRel/Alcore1e.dll"
# SUBTRACT LINK32 /profile /debug /nodefaultlib
LINK32_FLAGS=/nologo /subsystem:windows /dll /pdb:none\
 /map:"$(INTDIR)/Alcore1e.map" /machine:IX86 /def:".\ALCORE1.DEF"\
 /out:"$(OUTDIR)/Alcore1e.dll" /implib:"$(OUTDIR)/Alcore1e.lib" 
DEF_FILE= \
	".\ALCORE1.DEF"
LINK32_OBJS= \
	"$(INTDIR)\ADVCAPP.OBJ" \
	"$(INTDIR)\ADVCEDIT.OBJ" \
	"$(INTDIR)\ADVCVW.OBJ" \
	"$(INTDIR)\ADVWND.OBJ" \
	"$(INTDIR)\ALCORE.OBJ" \
	"$(INTDIR)\Alcore1.res" \
	"$(INTDIR)\BITMAP.OBJ" \
	"$(INTDIR)\BODY.OBJ" \
	"$(INTDIR)\BODY_IN.OBJ" \
	"$(INTDIR)\BODY_OUT.OBJ" \
	"$(INTDIR)\CLASS.OBJ" \
	"$(INTDIR)\COPY.OBJ" \
	"$(INTDIR)\CUI.OBJ" \
	"$(INTDIR)\DEBUG.OBJ" \
	"$(INTDIR)\ERROR.OBJ" \
	"$(INTDIR)\File.obj" \
	"$(INTDIR)\GC.OBJ" \
	"$(INTDIR)\Gdb_intp.obj" \
	"$(INTDIR)\GDM.OBJ" \
	"$(INTDIR)\GP_INTP.OBJ" \
	"$(INTDIR)\INTERP.OBJ" \
	"$(INTDIR)\Mainfrm.obj" \
	"$(INTDIR)\MATRIX.OBJ" \
	"$(INTDIR)\MEMBER.OBJ" \
	"$(INTDIR)\ocx.obj" \
	"$(INTDIR)\olectrl.obj" \
	"$(INTDIR)\PARALLEL.OBJ" \
	"$(INTDIR)\PICT.OBJ" \
	"$(INTDIR)\PRCSMGR.OBJ" \
	"$(INTDIR)\PROG_IF.OBJ" \
	"$(INTDIR)\PROJECT.OBJ" \
	"$(INTDIR)\SHELL.OBJ" \
	"$(INTDIR)\Stdafx.obj" \
	"$(INTDIR)\STRING.OBJ" \
	"$(INTDIR)\SYSFUNC.OBJ"

"$(OUTDIR)\Alcore1e.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WinDebug"
# PROP Intermediate_Dir "WinDebug"
OUTDIR=.\WinDebug
INTDIR=.\WinDebug

ALL : "$(OUTDIR)\Alcore1ed.dll"

CLEAN : 
	-@erase "$(INTDIR)\ADVCAPP.OBJ"
	-@erase "$(INTDIR)\ADVCEDIT.OBJ"
	-@erase "$(INTDIR)\ADVCVW.OBJ"
	-@erase "$(INTDIR)\ADVWND.OBJ"
	-@erase "$(INTDIR)\ALCORE.OBJ"
	-@erase "$(INTDIR)\Alcore1.res"
	-@erase "$(INTDIR)\Alcore1d.pch"
	-@erase "$(INTDIR)\BITMAP.OBJ"
	-@erase "$(INTDIR)\BODY.OBJ"
	-@erase "$(INTDIR)\BODY_IN.OBJ"
	-@erase "$(INTDIR)\BODY_OUT.OBJ"
	-@erase "$(INTDIR)\CLASS.OBJ"
	-@erase "$(INTDIR)\COPY.OBJ"
	-@erase "$(INTDIR)\CUI.OBJ"
	-@erase "$(INTDIR)\DEBUG.OBJ"
	-@erase "$(INTDIR)\ERROR.OBJ"
	-@erase "$(INTDIR)\File.obj"
	-@erase "$(INTDIR)\GC.OBJ"
	-@erase "$(INTDIR)\Gdb_intp.obj"
	-@erase "$(INTDIR)\GDM.OBJ"
	-@erase "$(INTDIR)\GP_INTP.OBJ"
	-@erase "$(INTDIR)\INTERP.OBJ"
	-@erase "$(INTDIR)\Mainfrm.obj"
	-@erase "$(INTDIR)\MATRIX.OBJ"
	-@erase "$(INTDIR)\MEMBER.OBJ"
	-@erase "$(INTDIR)\ocx.obj"
	-@erase "$(INTDIR)\olectrl.obj"
	-@erase "$(INTDIR)\PARALLEL.OBJ"
	-@erase "$(INTDIR)\PICT.OBJ"
	-@erase "$(INTDIR)\PRCSMGR.OBJ"
	-@erase "$(INTDIR)\PROG_IF.OBJ"
	-@erase "$(INTDIR)\PROJECT.OBJ"
	-@erase "$(INTDIR)\SHELL.OBJ"
	-@erase "$(INTDIR)\Stdafx.obj"
	-@erase "$(INTDIR)\STRING.OBJ"
	-@erase "$(INTDIR)\SYSFUNC.OBJ"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Alcore1ed.dll"
	-@erase "$(OUTDIR)\Alcore1ed.exp"
	-@erase "$(OUTDIR)\Alcore1ed.ilk"
	-@erase "$(OUTDIR)\Alcore1ed.lib"
	-@erase "$(OUTDIR)\Alcore1ed.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

$(OUTDIR)/ALCORE1D.bsc : $(OUTDIR)  $(BSC32_SBRS)
# ADD BASE CPP /nologo /G3 /MD /W3 /GX /Zi /Od /Gf /Gy /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /MD /W4 /Gm /GX /Zi /Od /Gf /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_MBCS" /D "AL_CORE_DLL" /D "AL_LANG_ENGLISH" /D "AL_USE_PCH" /D "AL_OCX" /Yu"STDAFX.H" /c
# SUBTRACT CPP /Gy /Fr
CPP_PROJ=/nologo /MD /W4 /Gm /GX /Zi /Od /Gf /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_MBCS" /D "AL_CORE_DLL"\
 /D "AL_LANG_ENGLISH" /D "AL_USE_PCH" /D "AL_OCX" /Fp"$(INTDIR)/Alcore1d.pch"\
 /Yu"STDAFX.H" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\WinDebug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x411 /fo"$(INTDIR)/Alcore1.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Alcore1d.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 oldnames.lib ldllcew.lib /nologo /subsystem:windows /dll /map:"FULL" /debug /machine:IX86
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:IX86 /nodefaultlib:"MSVCRT" /out:"WinDebug/Alcore1ed.dll"
# SUBTRACT LINK32 /map
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)/Alcore1ed.pdb" /debug /machine:IX86 /nodefaultlib:"MSVCRT"\
 /def:".\ALCORE1D.DEF" /out:"$(OUTDIR)/Alcore1ed.dll"\
 /implib:"$(OUTDIR)/Alcore1ed.lib" 
DEF_FILE= \
	".\ALCORE1D.DEF"
LINK32_OBJS= \
	"$(INTDIR)\ADVCAPP.OBJ" \
	"$(INTDIR)\ADVCEDIT.OBJ" \
	"$(INTDIR)\ADVCVW.OBJ" \
	"$(INTDIR)\ADVWND.OBJ" \
	"$(INTDIR)\ALCORE.OBJ" \
	"$(INTDIR)\Alcore1.res" \
	"$(INTDIR)\BITMAP.OBJ" \
	"$(INTDIR)\BODY.OBJ" \
	"$(INTDIR)\BODY_IN.OBJ" \
	"$(INTDIR)\BODY_OUT.OBJ" \
	"$(INTDIR)\CLASS.OBJ" \
	"$(INTDIR)\COPY.OBJ" \
	"$(INTDIR)\CUI.OBJ" \
	"$(INTDIR)\DEBUG.OBJ" \
	"$(INTDIR)\ERROR.OBJ" \
	"$(INTDIR)\File.obj" \
	"$(INTDIR)\GC.OBJ" \
	"$(INTDIR)\Gdb_intp.obj" \
	"$(INTDIR)\GDM.OBJ" \
	"$(INTDIR)\GP_INTP.OBJ" \
	"$(INTDIR)\INTERP.OBJ" \
	"$(INTDIR)\Mainfrm.obj" \
	"$(INTDIR)\MATRIX.OBJ" \
	"$(INTDIR)\MEMBER.OBJ" \
	"$(INTDIR)\ocx.obj" \
	"$(INTDIR)\olectrl.obj" \
	"$(INTDIR)\PARALLEL.OBJ" \
	"$(INTDIR)\PICT.OBJ" \
	"$(INTDIR)\PRCSMGR.OBJ" \
	"$(INTDIR)\PROG_IF.OBJ" \
	"$(INTDIR)\PROJECT.OBJ" \
	"$(INTDIR)\SHELL.OBJ" \
	"$(INTDIR)\Stdafx.obj" \
	"$(INTDIR)\STRING.OBJ" \
	"$(INTDIR)\SYSFUNC.OBJ"

"$(OUTDIR)\Alcore1ed.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

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

# Name "Alcore1d - Win32 Release"
# Name "Alcore1d - Win32 Debug"

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ALCORE1D.DEF

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\GDM\MATRIX.CPP

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_MATRI=\
	"A:\Altair\Src\Gdm\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\MATRIX.OBJ" : $(SOURCE) $(DEP_CPP_MATRI) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_MATRI=\
	"A:\Altair\Src\Gdm\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\MATRIX.OBJ" : $(SOURCE) $(DEP_CPP_MATRI) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\DEV\ADVWND.CPP

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_ADVWN=\
	"A:\Altair\Src\Dev\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\ADVWND.OBJ" : $(SOURCE) $(DEP_CPP_ADVWN) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_ADVWN=\
	"A:\Altair\Src\Dev\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\ADVWND.OBJ" : $(SOURCE) $(DEP_CPP_ADVWN) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\GDM\ERROR.CPP

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_ERROR=\
	"A:\Altair\Src\Gdm\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\ERROR.OBJ" : $(SOURCE) $(DEP_CPP_ERROR) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_ERROR=\
	"A:\Altair\Src\Gdm\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\ERROR.OBJ" : $(SOURCE) $(DEP_CPP_ERROR) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\DEV\SHELL.CPP

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_SHELL=\
	"A:\Altair\Src\Dev\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\SHELL.OBJ" : $(SOURCE) $(DEP_CPP_SHELL) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_SHELL=\
	"A:\Altair\Src\Dev\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\SHELL.OBJ" : $(SOURCE) $(DEP_CPP_SHELL) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\GDM\STRING.CPP

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_STRIN=\
	"A:\Altair\Src\Gdm\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\STRING.OBJ" : $(SOURCE) $(DEP_CPP_STRIN) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_STRIN=\
	"A:\Altair\Src\Gdm\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\STRING.OBJ" : $(SOURCE) $(DEP_CPP_STRIN) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\GDM\COPY.CPP

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_COPY_=\
	"A:\Altair\Src\Gdm\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\COPY.OBJ" : $(SOURCE) $(DEP_CPP_COPY_) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_COPY_=\
	"A:\Altair\Src\Gdm\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\COPY.OBJ" : $(SOURCE) $(DEP_CPP_COPY_) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\GENERAL\ALCORE.CPP
DEP_CPP_ALCOR=\
	"A:\Altair\Src\General\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

!IF  "$(CFG)" == "Alcore1d - Win32 Release"


"$(INTDIR)\ALCORE.OBJ" : $(SOURCE) $(DEP_CPP_ALCOR) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) /nologo /MD /W4 /GX /Gf /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_MBCS" /D "AL_CORE_DLL" /D\
 "AL_LANG_ENGLISH" /D "AL_USE_PCH" /D "AL_OCX" /Fp"$(INTDIR)/Alcore1d.pch"\
 /Yu"STDAFX.H" /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

# ADD CPP /Yu

"$(INTDIR)\ALCORE.OBJ" : $(SOURCE) $(DEP_CPP_ALCOR) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) /nologo /MD /W4 /Gm /GX /Zi /Od /Gf /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_MBCS" /D "AL_CORE_DLL"\
 /D "AL_LANG_ENGLISH" /D "AL_USE_PCH" /D "AL_OCX" /Fp"$(INTDIR)/Alcore1d.pch"\
 /Yu"STDAFX.H" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\GDM\GDM.CPP

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_GDM_C=\
	"A:\Altair\Src\Gdm\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\GDM.OBJ" : $(SOURCE) $(DEP_CPP_GDM_C) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_GDM_C=\
	"A:\Altair\Src\Gdm\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\GDM.OBJ" : $(SOURCE) $(DEP_CPP_GDM_C) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\GDM\DEBUG.CPP

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_DEBUG=\
	"A:\Altair\Src\Gdm\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\DEBUG.OBJ" : $(SOURCE) $(DEP_CPP_DEBUG) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_DEBUG=\
	"A:\Altair\Src\Gdm\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\DEBUG.OBJ" : $(SOURCE) $(DEP_CPP_DEBUG) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\DEV\ADVCEDIT.CPP

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_ADVCE=\
	"A:\Altair\Src\Dev\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\ADVCEDIT.OBJ" : $(SOURCE) $(DEP_CPP_ADVCE) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_ADVCE=\
	"A:\Altair\Src\Dev\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\ADVCEDIT.OBJ" : $(SOURCE) $(DEP_CPP_ADVCE) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\GDM\PICT.CPP

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_PICT_=\
	"A:\Altair\Src\Gdm\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\PICT.OBJ" : $(SOURCE) $(DEP_CPP_PICT_) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_PICT_=\
	"A:\Altair\Src\Gdm\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\PICT.OBJ" : $(SOURCE) $(DEP_CPP_PICT_) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\DEV\BITMAP.CPP

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_BITMA=\
	"A:\Altair\Src\Dev\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\BITMAP.OBJ" : $(SOURCE) $(DEP_CPP_BITMA) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_BITMA=\
	"A:\Altair\Src\Dev\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\BITMAP.OBJ" : $(SOURCE) $(DEP_CPP_BITMA) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\INTERP\PARALLEL.CPP

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_PARAL=\
	"A:\Altair\Src\Interp\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\PARALLEL.OBJ" : $(SOURCE) $(DEP_CPP_PARAL) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_PARAL=\
	"A:\Altair\Src\Interp\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\PARALLEL.OBJ" : $(SOURCE) $(DEP_CPP_PARAL) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\INTERP\SYSFUNC.CPP

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_SYSFU=\
	"A:\Altair\Src\Interp\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\SYSFUNC.OBJ" : $(SOURCE) $(DEP_CPP_SYSFU) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_SYSFU=\
	"A:\Altair\Src\Interp\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\SYSFUNC.OBJ" : $(SOURCE) $(DEP_CPP_SYSFU) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\DEV\ADVCVW.CPP

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_ADVCV=\
	"A:\Altair\Src\Dev\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\ADVCVW.OBJ" : $(SOURCE) $(DEP_CPP_ADVCV) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_ADVCV=\
	"A:\Altair\Src\Dev\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\ADVCVW.OBJ" : $(SOURCE) $(DEP_CPP_ADVCV) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\CUI\BODY.CPP

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_BODY_=\
	"A:\Altair\Src\Cui\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\BODY.OBJ" : $(SOURCE) $(DEP_CPP_BODY_) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_BODY_=\
	"A:\Altair\Src\Cui\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\BODY.OBJ" : $(SOURCE) $(DEP_CPP_BODY_) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\CUI\BODY_OUT.CPP

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_BODY_O=\
	"A:\Altair\Src\Cui\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\BODY_OUT.OBJ" : $(SOURCE) $(DEP_CPP_BODY_O) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_BODY_O=\
	"A:\Altair\Src\Cui\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\BODY_OUT.OBJ" : $(SOURCE) $(DEP_CPP_BODY_O) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\DEV\ADVCAPP.CPP

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_ADVCA=\
	"A:\Altair\Src\Dev\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\ADVCAPP.OBJ" : $(SOURCE) $(DEP_CPP_ADVCA) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_ADVCA=\
	"A:\Altair\Src\Dev\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\ADVCAPP.OBJ" : $(SOURCE) $(DEP_CPP_ADVCA) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\INTERP\INTERP.CPP

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_INTER=\
	"A:\Altair\Src\Interp\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\INTERP.OBJ" : $(SOURCE) $(DEP_CPP_INTER) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_INTER=\
	"A:\Altair\Src\Interp\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\INTERP.OBJ" : $(SOURCE) $(DEP_CPP_INTER) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\INTERP\PRCSMGR.CPP

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_PRCSM=\
	"A:\Altair\Src\Interp\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\PRCSMGR.OBJ" : $(SOURCE) $(DEP_CPP_PRCSM) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_PRCSM=\
	"A:\Altair\Src\Interp\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\PRCSMGR.OBJ" : $(SOURCE) $(DEP_CPP_PRCSM) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\CUI\PROJECT.CPP

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_PROJE=\
	"A:\Altair\Src\Cui\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\PROJECT.OBJ" : $(SOURCE) $(DEP_CPP_PROJE) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_PROJE=\
	"A:\Altair\Src\Cui\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\PROJECT.OBJ" : $(SOURCE) $(DEP_CPP_PROJE) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\CUI\BODY_IN.CPP

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_BODY_I=\
	"A:\Altair\Src\Cui\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\BODY_IN.OBJ" : $(SOURCE) $(DEP_CPP_BODY_I) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_BODY_I=\
	"A:\Altair\Src\Cui\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\BODY_IN.OBJ" : $(SOURCE) $(DEP_CPP_BODY_I) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\CUI\MEMBER.CPP

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_MEMBE=\
	"A:\Altair\Src\Cui\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\MEMBER.OBJ" : $(SOURCE) $(DEP_CPP_MEMBE) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_MEMBE=\
	"A:\Altair\Src\Cui\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\MEMBER.OBJ" : $(SOURCE) $(DEP_CPP_MEMBE) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\CUI\CLASS.CPP

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_CLASS=\
	"A:\Altair\Src\Cui\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\CLASS.OBJ" : $(SOURCE) $(DEP_CPP_CLASS) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_CLASS=\
	"A:\Altair\Src\Cui\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\CLASS.OBJ" : $(SOURCE) $(DEP_CPP_CLASS) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\CUI\CUI.CPP

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_CUI_C=\
	"A:\Altair\Src\Cui\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\CUI.OBJ" : $(SOURCE) $(DEP_CPP_CUI_C) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_CUI_C=\
	"A:\Altair\Src\Cui\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\CUI.OBJ" : $(SOURCE) $(DEP_CPP_CUI_C) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\INTERP\GP_INTP.CPP

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_GP_IN=\
	"A:\Altair\Src\Interp\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\GP_INTP.OBJ" : $(SOURCE) $(DEP_CPP_GP_IN) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_GP_IN=\
	"A:\Altair\Src\Interp\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\GP_INTP.OBJ" : $(SOURCE) $(DEP_CPP_GP_IN) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\INTERP\PROG_IF.CPP

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_PROG_=\
	"A:\Altair\Src\Interp\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\PROG_IF.OBJ" : $(SOURCE) $(DEP_CPP_PROG_) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_PROG_=\
	"A:\Altair\Src\Interp\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\PROG_IF.OBJ" : $(SOURCE) $(DEP_CPP_PROG_) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ALCORE1.DEF

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\ALTAIR\SRC\GDM\GC.CPP

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_GC_CP=\
	"A:\Altair\Src\Gdm\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\GC.OBJ" : $(SOURCE) $(DEP_CPP_GC_CP) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_GC_CP=\
	"A:\Altair\Src\Gdm\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\GC.OBJ" : $(SOURCE) $(DEP_CPP_GC_CP) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\Altair\Src\Interp\Gdb_intp.cpp

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_GDB_I=\
	"A:\Altair\Src\Interp\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\Gdb_intp.obj" : $(SOURCE) $(DEP_CPP_GDB_I) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_GDB_I=\
	"A:\Altair\Src\Interp\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\Gdb_intp.obj" : $(SOURCE) $(DEP_CPP_GDB_I) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\Altair\Src\Dev\ocx.cpp

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_OCX_C=\
	"A:\Altair\Src\Dev\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\ocx.obj" : $(SOURCE) $(DEP_CPP_OCX_C) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_OCX_C=\
	"A:\Altair\Src\Dev\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\ocx.obj" : $(SOURCE) $(DEP_CPP_OCX_C) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\Altair\Src\Res_e\Alcore1.rc

!IF  "$(CFG)" == "Alcore1d - Win32 Release"


"$(INTDIR)\Alcore1.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) /l 0x411 /fo"$(INTDIR)/Alcore1.res" /i "A:\Altair\Src\Res_e" /d\
 "NDEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"


"$(INTDIR)\Alcore1.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) /l 0x411 /fo"$(INTDIR)/Alcore1.res" /i "A:\Altair\Src\Res_e" /d\
 "_DEBUG" /d "_AFXDLL" $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\Altair\Src\Dev\Mainfrm.cpp

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_MAINF=\
	"A:\Altair\Src\Dev\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\Mainfrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_MAINF=\
	"A:\Altair\Src\Dev\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\Mainfrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\Altair\Src\Dev\olectrl.cpp

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_OLECT=\
	"A:\Altair\Src\Dev\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\olectrl.obj" : $(SOURCE) $(DEP_CPP_OLECT) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_OLECT=\
	"A:\Altair\Src\Dev\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

"$(INTDIR)\olectrl.obj" : $(SOURCE) $(DEP_CPP_OLECT) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\Altair\Src\General\Stdafx.cpp
DEP_CPP_STDAF=\
	"A:\Altair\Src\General\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

# ADD CPP /Yc"STDAFX.H"

BuildCmds= \
	$(CPP) /nologo /MD /W4 /GX /Gf /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_MBCS" /D "AL_CORE_DLL" /D\
 "AL_LANG_ENGLISH" /D "AL_USE_PCH" /D "AL_OCX" /Fp"$(INTDIR)/Alcore1d.pch"\
 /Yc"STDAFX.H" /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\Stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Alcore1d.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

# ADD CPP /Yc"STDAFX.H"

BuildCmds= \
	$(CPP) /nologo /MD /W4 /Gm /GX /Zi /Od /Gf /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_MBCS" /D "AL_CORE_DLL"\
 /D "AL_LANG_ENGLISH" /D "AL_USE_PCH" /D "AL_OCX" /Fp"$(INTDIR)/Alcore1d.pch"\
 /Yc"STDAFX.H" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\Stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Alcore1d.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=A:\Altair\Src\Dev\File.cpp

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

DEP_CPP_FILE_=\
	"A:\Altair\Src\Dev\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	{$(INCLUDE)}"\sys\Stat.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

"$(INTDIR)\File.obj" : $(SOURCE) $(DEP_CPP_FILE_) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

DEP_CPP_FILE_=\
	"A:\Altair\Src\Dev\Stdafx.h"\
	{$(INCLUDE)}"\Altair\Alext.h"\
	{$(INCLUDE)}"\Altair\Libinc.h"\
	{$(INCLUDE)}"\Altair\Strres.h"\
	{$(INCLUDE)}"\Cui\Body.h"\
	{$(INCLUDE)}"\Cui\Class.h"\
	{$(INCLUDE)}"\Cui\Cui.h"\
	{$(INCLUDE)}"\Cui\Member.h"\
	{$(INCLUDE)}"\Cui\Project.h"\
	{$(INCLUDE)}"\Dev\Advcapp.h"\
	{$(INCLUDE)}"\Dev\Advcedit.h"\
	{$(INCLUDE)}"\Dev\Advcvw.h"\
	{$(INCLUDE)}"\Dev\Advwnd.h"\
	{$(INCLUDE)}"\Dev\Bitmap.h"\
	{$(INCLUDE)}"\Dev\Event.h"\
	{$(INCLUDE)}"\Dev\File.h"\
	{$(INCLUDE)}"\Dev\Mainfrm.h"\
	{$(INCLUDE)}"\Dev\ocx.h"\
	{$(INCLUDE)}"\Dev\olectrl.h"\
	{$(INCLUDE)}"\Dev\Shell.h"\
	{$(INCLUDE)}"\Gdm\Debug.h"\
	{$(INCLUDE)}"\Gdm\Error.h"\
	{$(INCLUDE)}"\Gdm\Gdm.h"\
	{$(INCLUDE)}"\Gdm\Matrix.h"\
	{$(INCLUDE)}"\Gdm\Pict.h"\
	{$(INCLUDE)}"\Gdm\String.h"\
	{$(INCLUDE)}"\Gdm\Vmkey.h"\
	{$(INCLUDE)}"\Interp\Basefunc.h"\
	{$(INCLUDE)}"\Interp\Interp.h"\
	{$(INCLUDE)}"\Interp\Prcsmgr.h"\
	{$(INCLUDE)}"\Interp\Prog_if.h"\
	{$(INCLUDE)}"\Interp\Sysfunc.h"\
	{$(INCLUDE)}"\Occimpl.h"\
	{$(INCLUDE)}"\Ocdb.h"\
	{$(INCLUDE)}"\Ocdbid.h"\
	{$(INCLUDE)}"\Olebind.h"\
	{$(INCLUDE)}"\sys\Stat.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

"$(INTDIR)\File.obj" : $(SOURCE) $(DEP_CPP_FILE_) "$(INTDIR)"\
 "$(INTDIR)\Alcore1d.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################

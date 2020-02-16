# Microsoft Developer Studio Project File - Name="Alcore1d" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Alcore1d - Win32 Release
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "Alcore1d.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Alcore1d - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\WinRel"
# PROP BASE Intermediate_Dir ".\WinRel"
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\WinRel"
# PROP Intermediate_Dir ".\WinRel"
# PROP Ignore_Export_Lib 0
# ADD BASE CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /MD /W4 /GX /Gf /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_MBCS" /D "AL_CORE_DLL" /D "AL_LANG_ENGLISH" /D "AL_USE_PCH" /D "_AL_OPENSSL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 oldnames.lib ldllcew.lib winnls.lib /nologo /subsystem:windows /dll /map:"FULL" /machine:IX86
# ADD LINK32 libeay32.lib ws2_32.lib netapi32.lib odbc32.lib ssleay32.lib /nologo /base:"0x21000000" /subsystem:windows /dll /map /machine:IX86 /out:".\WinRel\Alcore1e.dll"
# SUBTRACT LINK32 /pdb:none /debug
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy C:\altair\build\lib\alcore_e\winrel\alcore1e.dll C:\altair\bin\alcore1e.dll	copy C:\altair\build\lib\alcore_e\winrel\alcore1e.map C:\altair\bin\alcore1e.map	copy C:\altair\build\lib\alcore_e\winrel\alcore1e.lib C:\altair\build\lib\alcore1e.lib
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Alcore1d - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\WinDebug"
# PROP BASE Intermediate_Dir ".\WinDebug"
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\WinDebug"
# PROP Intermediate_Dir ".\WinDebug"
# PROP Ignore_Export_Lib 0
# ADD BASE CPP /nologo /G3 /MD /W3 /GX /Zi /Od /Gf /Gy /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /MD /W4 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_MBCS" /D "AL_CORE_DLL" /D "AL_LANG_ENGLISH" /D "AL_USE_PCH" /D "_AL_OPENSSL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 oldnames.lib ldllcew.lib /nologo /subsystem:windows /dll /map:"FULL" /debug /machine:IX86
# ADD LINK32 ws2_32.lib netapi32.lib odbc32.lib ssleay32.lib libeay32.lib /nologo /base:"0x21000000" /subsystem:windows /dll /debug /machine:IX86 /nodefaultlib:"MSVCRT" /out:".\WinDebug\Alcore1ed.dll"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Alcore1d - Win32 Release"
# Name "Alcore1d - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\..\..\src\src\dev\advcapp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\dev\advcedit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\dev\advcvw.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\dev\advwnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\general\alcore.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\res_e\Alcore1.rc
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\gdm\big_int.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\dev\bitmap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\cui\body.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\cui\body_in.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\cui\body_out.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\cui\class.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\gdm\copy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\net\crypt.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\cui\cui.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\gdm\debug.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\dev\dllcall.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\gdm\error.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\gdm\file.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\gdm\gc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\gdm\gdm.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\interp\gp_intp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\interp\interp1.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\interp\interp2.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\gdm\j_code.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\nl\jana.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\dev\mainfrm.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\gdm\matrix.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\cui\member.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\net\multipart.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\dev\ocx.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\dev\olectrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\interp\parallel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\gdm\pict.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\interp\prcsmgr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\gdm\prof.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\interp\prog_if.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\cui\project.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\dev\shell.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\net\socket.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\solid\solid.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\sql\sql.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\general\stdafx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\gdm\string.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\symbol\symbolic_manip.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\interp\sysfunc1.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\interp\sysfunc2.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\interp\sysfunc3.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\xml\typedic.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\xml\xml.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\src\xml\xslt.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\..\..\src\src\gdm\stdafx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

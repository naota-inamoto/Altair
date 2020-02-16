; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAltairDoc
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "altair.h"
LastPage=0

ClassCount=4
Class1=CAltairApp
Class2=CAboutDlg
Class3=CAltairDoc
Class4=CAltairView

ResourceCount=4
Resource1=IDR_MAINFRAME
Resource2=IDR_ALTAIRFRAME
Resource3=IDR_ALTAIRTYPE
Resource4=IDD_ABOUTBOX

[CLS:CAltairApp]
Type=0
BaseClass=CWinApp
HeaderFile=\Altair\Altair\Altair.h
ImplementationFile=\Altair\Altair\Altair.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=\Altair\Altair\Altair.cpp
ImplementationFile=\Altair\Altair\Altair.cpp
LastObject=CAboutDlg

[CLS:CAltairDoc]
Type=0
HeaderFile=\Altair\Src\Altair\altairdoc.h
ImplementationFile=\Altair\Src\Altair\altairdoc.cpp
BaseClass=CDocument
LastObject=CAltairDoc

[CLS:CAltairView]
Type=0
BaseClass=CView
HeaderFile=\Altair\Altair\AltairView.h
ImplementationFile=\Altair\Altair\AltairView.cpp
LastObject=CAltairView
Filter=C
VirtualFilter=VWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=6
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352

[MNU:IDR_ALTAIRFRAME]
Type=1
Class=?
Command1=ID_FILE_PRINT_SETUP
Command2=ID_APP_EXIT
Command3=ID_PROJ_WND
Command4=ID_VIEW_STATUS_BAR
Command5=ID_APP_ABOUT
CommandCount=5

[MNU:IDR_ALTAIRTYPE]
Type=1
Class=?
Command1=ID_FILE_CLOSE
Command2=ID_FILE_PRINT
Command3=ID_FILE_PRINT_PREVIEW
Command4=ID_FILE_PRINT_SETUP
Command5=ID_APP_EXIT
Command6=ID_PROJ_WND
Command7=ID_VIEW_STATUS_BAR
Command8=ID_WINDOW_CASCADE
Command9=ID_WINDOW_TILE_HORZ
Command10=ID_WINDOW_ARRANGE
Command11=ID_APP_ABOUT
CommandCount=11

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_PRINT_SETUP
Command2=ID_APP_EXIT
Command3=ID_PROJ_WND
Command4=ID_VIEW_STATUS_BAR
Command5=ID_APP_ABOUT
CommandCount=5


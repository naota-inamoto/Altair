; CLW ÉtÉ@ÉCÉãÇÕ MFC ClassWizard ÇÃèÓïÒÇä‹ÇÒÇ≈Ç¢Ç‹Ç∑

[General Info]
Version=1
LastClass=CAlCtrlCtrl
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "AlCtrl.h"
CDK=Y

ClassCount=3
Class1=CAlCtrlCtrl
Class2=CAlCtrlPropPage

ResourceCount=8
Resource1=IDR_MDIFRAME
LastPage=0
Resource2=IDD_ABOUTBOX_ALCTRL
Class3=CMsgDlg
Resource3=IDR_ALTAIRTYPE1
Resource4=IDR_ALTAIRTYPE2
Resource5=IDR_MAINFRAME1
Resource6=IDD_PROPPAGE_ALCTRL
Resource7=IDR_MDICHILD
Resource8=IDR_SDIFRAME

[CLS:CAlCtrlCtrl]
Type=0
HeaderFile=\Altair\include\alctrl\alctrlctl.h
ImplementationFile=alctrlctl.cpp
BaseClass=COleControl
Filter=W
VirtualFilter=WC
LastObject=CAlCtrlCtrl

[CLS:CAlCtrlPropPage]
Type=0
HeaderFile=\Altair\include\alctrl\alctrlppg.h
ImplementationFile=alctrlppg.cpp
BaseClass=COlePropertyPage
Filter=
LastObject=CAlCtrlPropPage

[DLG:IDD_ABOUTBOX_ALCTRL]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_PROPPAGE_ALCTRL]
Type=1
Class=CAlCtrlPropPage
ControlCount=3
Control1=IDC_TRANSPARENT,button,1342242819
Control2=IDC_BORDERWIDTH,edit,1350631424
Control3=IDC_STATIC,static,1342308352

[CLS:CMsgDlg]
Type=0
HeaderFile=MsgDlg.h
ImplementationFile=MsgDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

[MNU:IDR_ALTAIRTYPE1]
Type=1
Command1=ID_FILE_CLOSE
Command2=ID_FILE_PRINT
Command3=ID_FILE_PRINT_PREVIEW
Command4=ID_FILE_PRINT_SETUP
Command5=ID_VIEW_STATUS_BAR
Command6=ID_WINDOW_CASCADE
Command7=ID_WINDOW_TILE_HORZ
Command8=ID_WINDOW_ARRANGE
CommandCount=8

[MNU:IDR_ALTAIRTYPE2]
Type=1
Command1=ID_FILE_CLOSE
Command2=ID_FILE_PRINT
Command3=ID_FILE_PRINT_PREVIEW
Command4=ID_FILE_PRINT_SETUP
Command5=ID_VIEW_STATUS_BAR
CommandCount=5

[MNU:IDR_MAINFRAME1]
Type=1
Command1=ID_FILE_PRINT_SETUP
Command2=ID_VIEW_STATUS_BAR
CommandCount=2

[MNU:IDR_MDIFRAME]
Type=1
Class=?
Command1=ID_FILE_PRINT_SETUP
Command2=ID_VIEW_STATUS_BAR
CommandCount=2

[MNU:IDR_MDICHILD]
Type=1
Class=?
Command1=ID_FILE_CLOSE
Command2=ID_FILE_PRINT
Command3=ID_FILE_PRINT_PREVIEW
Command4=ID_FILE_PRINT_SETUP
Command5=ID_VIEW_STATUS_BAR
Command6=ID_WINDOW_CASCADE
Command7=ID_WINDOW_TILE_HORZ
Command8=ID_WINDOW_ARRANGE
CommandCount=8

[MNU:IDR_SDIFRAME]
Type=1
Class=?
Command1=ID_FILE_CLOSE
Command2=ID_FILE_PRINT
Command3=ID_FILE_PRINT_PREVIEW
Command4=ID_FILE_PRINT_SETUP
Command5=ID_VIEW_STATUS_BAR
CommandCount=5


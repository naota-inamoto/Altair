
#include <afxwin.h>
#include "app.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM w, LPARAM l);
BOOL CALLBACK ModalDlgProc(HWND hDlg, UINT msg, WPARAM w, LPARAM);
BOOL CALLBACK ModelessDlgProc(HWND hDlg, UINT msg, WPARAM w, LPARAM);

HINSTANCE hInst = NULL;
HWND hMainWnd = NULL;
BOOL bPict = FALSE;
HWND hButton1 = NULL;
HWND hButton2 = NULL;
HWND hDialog = NULL;

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE /*hPrevInstance, always NULL in Win32*/,
	LPSTR /*lpCmdLine*/,
	int /*nCmdShow*/) {

	hInst = hInstance;

	WNDCLASS wndClass;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(hInstance, "app_main");
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = "app_main";
	wndClass.lpszClassName = "APP_WNDCLASS";
	if (!RegisterClass(&wndClass))
		return 0;

	hMainWnd = CreateWindow(
		"APP_WNDCLASS", /* window class */
		"Build Sample", /* window text */
		WS_OVERLAPPEDWINDOW, /* style */
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		CW_USEDEFAULT, /* width */
		CW_USEDEFAULT, /* height */
		NULL, /* parent */
		NULL, /* menu */
		hInst,
		NULL);
	if (hMainWnd == NULL)
		return 0;
	ShowWindow(hMainWnd, SW_SHOW);
	UpdateWindow(hMainWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM w, LPARAM l) {
	switch (msg) {
	case WM_NCDESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, w, l);

	case WM_COMMAND:
		// wNotifyCode = HIWORD(wParam); notification code 
		// wID = LOWORD(wParam);         item, control, or accelerator identifier 
		// hwndCtl = (HWND) lParam;      handle of control 
		switch (LOWORD(w)) {
		case ID_FILE_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_EDIT_PICT:
			bPict = !bPict;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_EDIT_CTRL:
			if (hButton1 == NULL) {
				hButton1 = CreateWindow("BUTTON", "Modal Dialog",
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					350, 125, 150, 40,
					hWnd, (HMENU)ID_MODAL_BTN, hInst, NULL);
				hButton2 = CreateWindow("BUTTON", "Modeless Dialog",
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					350, 200, 150, 40,
					hWnd, (HMENU)ID_MODELESS_BTN, hInst, NULL);
			} else {
				DestroyWindow(hButton1);
				DestroyWindow(hButton2);
				hButton1 = hButton2 = NULL;
			}
			break;
		case ID_MODAL_BTN:
			switch (HIWORD(w)) {
			case BN_CLICKED:
				DialogBox(hInst, 
					"modal_dialog",
					hWnd, (DLGPROC)ModalDlgProc);
				break;
			}
			break;
		case ID_MODELESS_BTN:
			switch (HIWORD(w)) {
			case BN_CLICKED:
				if (hDialog)
					return 0;
				hDialog = CreateDialog(hInst, 
					"modeless_dialog",
					hWnd, (DLGPROC)ModelessDlgProc);
				if (hDialog == NULL)
					return 0;
				ShowWindow(hDialog, SW_SHOW);
				break;
			}
			break;
		}
		break;
	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		if (bPict) {
			HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));
			HPEN hOrigPen = (HPEN)SelectObject(hdc, hPen);
			HBRUSH hOrigBrush = (HBRUSH)SelectObject(hdc, hBrush);

			Ellipse(hdc, 100, 100, 150, 150);
			Rectangle(hdc, 100, 175, 150, 225);
			MoveToEx(hdc, 75, 275, NULL);
			LineTo(hdc, 175, 275);

			char *c = "Circle", *r = "Rectangle", *l = "Line";
			TextOut(hdc, 200, 125, c, strlen(c));
			TextOut(hdc, 200, 200, r, strlen(r));
			TextOut(hdc, 200, 275, l, strlen(l));

			SelectObject(hdc, hOrigPen);
			SelectObject(hdc, hOrigBrush);
			DeleteObject(hPen);
			DeleteObject(hBrush);
		}

		EndPaint(hWnd, &ps);
		break;
	}
	return 0;
}

BOOL CALLBACK ModalDlgProc(HWND hDlg, UINT msg, WPARAM w, LPARAM) {
	switch (msg) {
	case WM_INITDIALOG:
		SetDlgItemText(hDlg, ID_END_MODAL, "Close");
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(w)) {
		case ID_END_MODAL:
			switch (HIWORD(w)) {
			case BN_CLICKED:
				EndDialog(hDlg, 0 /* return value of DialogBox */);
				return TRUE;
			}
			break;
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK ModelessDlgProc(HWND hDlg, UINT msg, WPARAM w, LPARAM) {
	switch (msg) {
	case WM_INITDIALOG:
		SetDlgItemText(hDlg, ID_END_MODELESS, "Close");
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(w)) {
		case ID_END_MODELESS:
			switch (HIWORD(w)) {
			case BN_CLICKED:
				hDialog = NULL;
				DestroyWindow(hDlg);
				return TRUE;
			}
			break;
		}
	}
	return FALSE;
}

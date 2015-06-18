#include <windows.h>
#include <stdlib.h>
#include <time.h>
#define ID_BUTTON1 104
#define ID_EDIT1 105
#define MAX_STRING 256
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("OllyTest");
HWND hwndEdit;
HWND hwndButton1;
char tmp_buf[MAX_STRING];
char str_guess[10] = "1ab23";
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow){
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		0, 0, 180, 150,
		NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){
	switch (iMessage){
	case WM_CREATE:
		CreateWindow(TEXT("static"), "Type password", WS_CHILD | WS_VISIBLE,
			10, 10, 150, 30, hWnd, (HMENU)0, g_hInst, NULL);
		hwndEdit = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
			10, 40, 150, 30, hWnd, (HMENU)ID_EDIT1, g_hInst, NULL);
		hwndButton1 = CreateWindow(TEXT("button"), "Try", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			10, 70, 150, 30, hWnd, (HMENU)ID_BUTTON1, g_hInst, NULL);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)){
		case ID_BUTTON1:
			GetDlgItemText(hWnd, ID_EDIT1, tmp_buf, MAX_STRING);
			if (!strcmp(str_guess, tmp_buf)){
				MessageBox(NULL, "BINGO", "successful try", MB_OK);
			}
			else{
				MessageBox(NULL, "Try again", "Mission failed", MB_OK);
			}
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
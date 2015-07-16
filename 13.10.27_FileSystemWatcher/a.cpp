#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("ChangeNoti");

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	WndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);

	while (GetMessage(&Message,NULL,0,0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

HANDLE hChange;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes = TEXT("C:\\ 디렉토리의 변화를 감시합니다");

	switch (iMessage)
	{
	case WM_CREATE:
		hChange = FindFirstChangeNotification(TEXT("c:\\"),TRUE,FILE_NOTIFY_CHANGE_FILE_NAME);
		SetTimer(hWnd,1,100,NULL);
		return 0;
	case WM_TIMER:
		if (WaitForSingleObject(hChange,0) == WAIT_OBJECT_0)
		{
			KillTimer(hWnd,1);
			if (MessageBox(hWnd,TEXT("파일 이름이 바뀌었습니다. 계속 감시하겠습니까?"),
				TEXT("질문"),MB_YESNO) == IDNO)
			{
				FindCloseChangeNotification(hChange);
				hChange = NULL;
			}
			else
			{
				FindNextChangeNotification(hChange);
				SetTimer(hWnd,1,100,NULL);
			}
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd,&ps);
		TextOut(hdc,10,10,Mes,lstrlen(Mes));
		EndPaint(hWnd,&ps);
		return 0;
	case WM_DESTROY:
		if (hChange)
			FindCloseChangeNotification(hChange);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
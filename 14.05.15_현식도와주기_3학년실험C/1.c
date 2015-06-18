#include<Windows.h>
#include<stdio.h>

int main()
{
	POINT a;
	char Old[1024];
	HWND hWnd;
	RECT rect;
		
	while(1)
	{
		system("cls");
		GetCursorPos(&a);
		GetConsoleTitle(Old, 1024);
		hWnd=FindWindow(NULL, Old);
		ScreenToClient(hWnd, &a);
		GetWindowRect(hWnd, &rect);
		if(a.x < rect.right/2){
			printf("\n\n\n\n\n\n\n\n\n\n\n\n                    %d %d", a.x, a.y);
		} else {
			printf("\n\n\n\n\n\n\n\n\n\n\n\n                                                             %d %d", a.y, a.x);
		}		
	}
	return 0;
}
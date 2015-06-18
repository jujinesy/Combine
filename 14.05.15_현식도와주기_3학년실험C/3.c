#include<Windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<process.h>

int x=-1,y=-1;
HANDLE       hIn, hOut;
DWORD        dwNOER;
INPUT_RECORD rec;


void gotoxy(int a, int b) //콘솔의 입력위치를 바꿔주는 함수
{
	COORD ay={a, b}; //좌표값을 저장하는 구조체 COORD
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), ay); //콘솔 handle을 얻어서 ay좌표에 맞게 설정
}

HWND GetConsoleHwnd(void)
{
#define MY_BUFSIZE 1024 // Buffer size for console window titles.
	HWND hwndFound;         // This is what is returned to the caller.
	char pszNewWindowTitle[MY_BUFSIZE]; // Contains fabricated
	// WindowTitle.
	char pszOldWindowTitle[MY_BUFSIZE]; // Contains original
	// WindowTitle.

	// Fetch current window title.

	GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);

	// Format a "unique" NewWindowTitle.

	wsprintf(pszNewWindowTitle,"%d/%d",
		GetTickCount(),
		GetCurrentProcessId());

	// Change current window title.

	SetConsoleTitle(pszNewWindowTitle);

	// Ensure window title has been updated.

	Sleep(40);

	// Look for NewWindowTitle.

	hwndFound=FindWindow(NULL, pszNewWindowTitle);

	// Restore original window title.

	SetConsoleTitle(pszOldWindowTitle);

	return(hwndFound);
}


int main()
{
	char str['ab'];
	int p, q, i, strcnt=0; //좌표값을 계산하기 위한 q, p와 반복문의 위한 변수 r 선언
	POINT a; //마우스 포인터 구조체 a선언
	unsigned long bytes_written; //하이퍼 터미널로 자료보낼떄쓰이는 자료형 선언
	DWORD  dwBytesRead;
	char yes[4]="yes"; // 마우스가 학번근처에 있을떄 하이퍼터미널로 보낼 글자
	char no[3]="no"; // 마우스가 학번근처에 없을떄 하이퍼터미널로 보낼 글자
	char output[32];
	char save[32];
	int x, y, width, height;
	char *token;

	HANDLE hSerial; //통신 디바이스 장치 핸들 선언
	DCB dcbSerialParams={0}; // 통속도를 맞추기위한 구조체 선언
	COMMTIMEOUTS timeouts={0}; //   구조체 선언
	HWND hWnd; // 콘솔창 핸들을 가져오기위한 윈도우 전용 hwnd

	char Old[1024]; //콘솔창 제목얻 얻기위한 배열

	//COM2포트로 통신을하며 기본적으로 READ와 WRITE가 가능하게 통신 설정함
	hSerial = CreateFile("COM2", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hSerial == INVALID_HANDLE_VALUE) //사용할수 없는 핸들일때
	{
		if(GetLastError() == ERROR_FILE_NOT_FOUND) //해당포트가 존재하지 않을때 
		{
			printf("Serial port does not exist\n");
		}
		printf("Some other error occuressd\n"); //해당포트가 존재하나, 다른오류가 있을떄
	}

	//dcbSerialParams의 내부속성의 길이를 dcbSerialParams길이만큼 계산해서 저장
	dcbSerialParams.DCBlength=sizeof(dcbSerialParams);

	//시리얼포트 정보에 dcbSerialParams를 넣는다. 이때오류가나면 아래 에러문을 출력
	if(!GetCommState(hSerial, &dcbSerialParams))
	{
		printf("error getting state\n");
	}

	//초당 115200 비트
	dcbSerialParams.BaudRate = CBR_115200;
	//데이터비트 8비트
	dcbSerialParams.ByteSize = 8;
	// 정지비트 1비트
	dcbSerialParams.StopBits = ONESTOPBIT;
	//패리티비트 없음
	dcbSerialParams.Parity = NOPARITY;

	//시리얼포트 정보에 dcbSerialParams를 넣는다. 이때오류가나면 아래 에러문을 출력
	if(!SetCommState(hSerial, &dcbSerialParams))
	{
		printf("error setting serial port state\n");
	}

	// 도착하는 문자 사이의 최대 시간 간격 
	timeouts.ReadIntervalTimeout=50; 
	// n이라면, 읽기 타임아웃 값은 다음과 같이 계산한다.( n * ReadTotalTimeoutMultiplier + ReadTotalTimeoutConstant)
	timeouts.ReadTotalTimeoutConstant=50;
	// 바이트 당 읽기 타임아웃  값과 상수값을 밀리초 단위로 지정한다. 읽을 바이트수가 모두 0이면, 읽기 타임아웃을 사용하지 않는다는 뜻
	timeouts.ReadTotalTimeoutMultiplier=10;
	// 쓰기 타임아웃값은 다음과 같이 계산한다. 특별히 두 값이 모두 0이면, 쓰기 타임아웃을 사용하지 않는다.
	timeouts.WriteTotalTimeoutConstant=50;
	//바이트당 쓰기 타임아웃 값과 상수 값을 미리초 단위로 지정한다. 보낼 바이트수가 n이라면
	timeouts.WriteTotalTimeoutMultiplier=10;

	//시리얼포트 정보에 dcbSerialParams를 넣는다. 이때오류가나면 아래 에러문을 출력
	if(!SetCommTimeouts(hSerial, &timeouts))
	{
		printf("error occured\n");
	}


	//랜덤씨드 값을 CPU의 쿨럭에서 가져옴
	srand(GetTickCount());
	//for(r=0; r<100; r++)
	//{
	//	//콘솔창 클리어
	//	system("cls");
	//	//MouseClick(&x, &y);
	//	printf("%d %d\n",x,y);
	//	//콘솔 입력위치를 좌우로 0~63, 위아래로 0~24 값을 랜덤하게 배정
	//	//pdf에는 80과 40을 입력하라고 적혀 있으나, 문자열이 개행될경우가 발생하기때문에 이부분을 약간 수정함.
	//	gotoxy(q=rand()%64, p=rand()%25);
	//	printf("%s", str);
	//	//3초정지
	//	Sleep(3000);
	//	//3초후에 마우스 위치를 a에 저장
	//	GetCursorPos(&a);
	//	//현재 콘솔창 타이틀을 Old에 저장
	//	GetConsoleTitle(Old, 1024);
	//	//현재콜솔창에 대한 핸들을 가져옴
	//	hWnd=FindWindow(NULL, Old);
	//	//현대 콘솔창을 기준으로 마우스 상대값을 변환
	//	ScreenToClient(hWnd, &a);
	//	//마우스가 학번주변 6픽셀범위 내에 있으면 하이퍼터미널에 yse 전송 아니면 no전송
	//	if( (a.x>=q*8-6 && a.x<=q*8+125+6) && (a.y>=p*15-6 && a.y<=p*15+30+6))
	//	{
	//		//yse 전송
	//		WriteFile(hSerial, yes, 4, &bytes_written, NULL);
	//	} else 
	//	{
	//		//no전송
	//		WriteFile(hSerial, no, 3, &bytes_written, NULL);
	//	}
	//}

	//개행두번
	for( i=0 ; i<2 ; i++)
	{
		output[0]= 0x0a;
		output[1]= 0x0d;
		dwBytesRead=2;
		WriteFile(hSerial, output, dwBytesRead, &bytes_written, NULL);
		dwBytesRead=0;
	}



	//글시 써주기
	strcpy(output, "Enter Offset(x,y): ");
	dwBytesRead = strlen("Enter Offset(x,y): ");
	WriteFile(hSerial, output, dwBytesRead, &bytes_written, NULL);


	//입력받을숫자 포인터 초기화
	strcnt=0;
	while(1)
	{
		ReadFile(hSerial, output, sizeof(output), &dwBytesRead, NULL);
		
		//엔터를눌렀는지 않눌렀는지 구분.
		if(output[0]== 0x0d) //여기가 엔터
		{
			output[0]= 0x0a;
			output[1]= 0x0d;
			dwBytesRead=2;
			WriteFile(hSerial, output, dwBytesRead, &bytes_written, NULL);
			dwBytesRead=0;
			//다음줄 입력을 위해 while문 나가기
			break;
		} else
		{
			WriteFile(hSerial, output, dwBytesRead, &bytes_written, NULL);
			//숫자를 문자열로 저장. 이때 값 저장후 strcnt값을 1증가
			if(dwBytesRead>0)
				save[strcnt++]=output[0];
		}
	}
	save[strcnt]=0;
	token = strtok(save, ",");    // save을 " "을 기준으로 나눈다.
	x=atoi(token);
	//printf("%s\n",token);
	token = strtok(NULL," "); 
	//전에 나눴던 문자열을 다시 나눌 시 문자열 대신 NULL 입력
	y=atoi(token);
	//첫번째줄 받기완료
	///////////////////////////////////////////////////////////////////////////////


	//글시 써주기
	strcpy(output, "Enter Width: ");
	dwBytesRead = strlen("Enter Width: ");
	WriteFile(hSerial, output, dwBytesRead, &bytes_written, NULL);



	//입력받을숫자 포인터 초기화
	strcnt=0;
	while(1)
	{
		ReadFile(hSerial, output, sizeof(output), &dwBytesRead, NULL);
		
		//엔터를눌렀는지 않눌렀는지 구분.
		if(output[0]== 0x0d) //여기가 엔터
		{
			output[0]= 0x0a;
			output[1]= 0x0d;
			dwBytesRead=2;
			WriteFile(hSerial, output, dwBytesRead, &bytes_written, NULL);
			dwBytesRead=0;
			//다음줄 입력을 위해 while문 나가기
			break;
		} else
		{
			WriteFile(hSerial, output, dwBytesRead, &bytes_written, NULL);
			//숫자를 문자열로 저장. 이때 값 저장후 strcnt값을 1증가
			if(dwBytesRead>0)
				save[strcnt++]=output[0];
		}
	}
	save[strcnt]=0;
	width=atoi(save);
	//////////////////////////////////////////////////////////////////////






	//글시 써주기
	strcpy(output, "Enter Height: ");
	dwBytesRead = strlen("Enter Height: ");
	WriteFile(hSerial, output, dwBytesRead, &bytes_written, NULL);



	//입력받을숫자 포인터 초기화
	strcnt=0;
	while(1)
	{
		ReadFile(hSerial, output, sizeof(output), &dwBytesRead, NULL);
		
		//엔터를눌렀는지 않눌렀는지 구분.
		if(output[0]== 0x0d) //여기가 엔터
		{
			output[0]= 0x0a;
			output[1]= 0x0d;
			dwBytesRead=2;
			WriteFile(hSerial, output, dwBytesRead, &bytes_written, NULL);
			dwBytesRead=0;
			//다음줄 입력을 위해 while문 나가기
			break;
		} else
		{
			WriteFile(hSerial, output, dwBytesRead, &bytes_written, NULL);
			//숫자를 문자열로 저장. 이때 값 저장후 strcnt값을 1증가
			if(dwBytesRead>0)
				save[strcnt++]=output[0];
		}
	}
	save[strcnt]=0;
	height=atoi(save);
	//////////////////////////////////////////////////////////////////////

	GetConsoleTitle(Old, 1024);
	//	//현재콜솔창에 대한 핸들을 가져옴
	hWnd=FindWindow(NULL, Old);
	MoveWindow(hWnd, x, y, width, height, TRUE);

	//시리얼 통신 핸들 클로즈
	CloseHandle(hSerial);
	system("pause");
	return 0;
}
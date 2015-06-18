#include <Windows.h>
#include <stdio.h>
#include <conio.h>

HANDLE COUT = 0;    // 콘솔 출력 장치
HANDLE CIN = 0;        // 콘솔 입력 장치

int be_input()
{
    INPUT_RECORD input_record;
    DWORD input_count;

    PeekConsoleInput(CIN, &input_record, 1, &input_count);
    return input_count;
}

int get_input(WORD *vkey, COORD *pos)
{
    INPUT_RECORD input_record;
    DWORD input_count;

    ReadConsoleInput(CIN, &input_record, 1, &input_count);
    switch(input_record.EventType) {
    case MOUSE_EVENT:
        if(pos && (input_record.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)) {
            CONSOLE_SCREEN_BUFFER_INFO csbi;
    
            GetConsoleScreenBufferInfo(COUT,&csbi);

            *pos = input_record.Event.MouseEvent.dwMousePosition;
            pos->X -= csbi.srWindow.Left;
            pos->Y -= csbi.srWindow.Top;

            return MOUSE_EVENT;
        }
        break;
    case KEY_EVENT:
        if(vkey && (input_record.Event.KeyEvent.bKeyDown)) {
            *vkey = input_record.Event.KeyEvent.wVirtualKeyCode;
            return KEY_EVENT;
        }
    }

//    FlushConsoleInputBuffer(CIN);
    return 0;
}

void gotoxy(int a, int b) //콘솔의 입력위치를 바꿔주는 함수
{
	COORD ay={a, b}; //좌표값을 저장하는 구조체 COORD
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), ay); //콘솔 handle을 얻어서 ay좌표에 맞게 설정
}

int main()
{
	char str['ab'];
    DWORD mode;
    WORD key;
    COORD pos;
    int event;
	int p, q, cnt=0, i,strcnt=0, strcur;
	POINT a;
	char Old[1024];
	HWND hWnd;
	HANDLE hSerial;
	unsigned long bytes_written; //하이퍼 터미널로 자료보낼떄쓰이는 자료형 선언
	char yes[4]="yes"; // 마우스가 학번근처에 있을떄 하이퍼터미널로 보낼 글자
	char no[3]="no"; // 마우스가 학번근처에 없을떄 하이퍼터미널로 보낼 글자
	DCB dcbSerialParams={0}; // 통속도를 맞추기위한 구조체 선언
	COMMTIMEOUTS timeouts={0}; //   구조체 선언



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

    CIN = GetStdHandle(STD_INPUT_HANDLE);
    COUT = GetStdHandle(STD_OUTPUT_HANDLE);

    // 마우스 활성화
    GetConsoleMode(CIN,&mode);
    SetConsoleMode(CIN, mode | ENABLE_MOUSE_INPUT);

	memset( str, 'A', sizeof(str));

	printf("게임에 사용될 문자열을 입력하고 엔터를 눌러주세요\n");
	scanf("%s",str);

	strcur=strcnt=strlen(str);
	str[strcnt]='A';
	gotoxy(q=0, p=0);
	for( i=0 ; i < 25 ; i++)
		printf("                                                                               \n");
	gotoxy(q=rand()%64, p=rand()%25);
	for( i=0 ; i < strcnt ;i++) 
	{
		printf("%c",str[i]);
	}

    while(++cnt)
    {
		if( strcnt == 0 )
		{
			printf("당신이 이겼습니다.\n");
			goto end;
		} else if( strcnt - strcur  >= 10)
		{
			printf("당신이 졌습니다.\n");
			goto end;
		}

		if(be_input()) {
            event = get_input(&key,&pos);
            switch(event) {
            case KEY_EVENT:
                printf("%d 키를 누르셨습니다.\n", key);
                if(key == VK_ESCAPE) goto end;
                break;
            case MOUSE_EVENT:
                //printf("(%d,%d) 에 클릭하셨습니다.\n", pos.X, pos.Y);
				goto click;
                break;
            }
        }
        
		
		if(cnt==240000)
		{
			gotoxy(q=0, p=0);
			for( i=0 ; i<25 ; i++)
				printf("                                                                               \n");

			gotoxy(q=rand()%64, p=rand()%25);
			for( i=0 ; i < strcnt ;i++) 
			{
				printf("%c",str[i]);
			}
			cnt=0;
		}
				
		
		if(0)
		{
			click:
			//3초후에 마우스 위치를 a에 저장
			GetCursorPos(&a);
			//현재 콘솔창 타이틀을 Old에 저장
			GetConsoleTitle(Old, 1024);
			//현재콜솔창에 대한 핸들을 가져옴
			hWnd=FindWindow(NULL, Old);
			//현대 콘솔창을 기준으로 마우스 상대값을 변환
			ScreenToClient(hWnd, &a);
			//마우스가 학번주변 6픽셀범위 내에 있으면 하이퍼터미널에 yse 전송 아니면 no전송
			if( (a.x>=q*8-6 && a.x<=q*8+125+6) && (a.y>=p*15-6 && a.y<=p*15+30+6))
			{
				//yse 전송
				WriteFile(hSerial, yes, 4, &bytes_written, NULL);
				gotoxy(q, p);
				for( i=0 ; i < strcnt ;i++) 
				{
					printf(" ");
				}
				strcnt--;
				gotoxy(q, p);
				for( i=0 ; i < strcnt ;i++) 
				{
					printf("%c",str[i]);
				}
			} else 
			{
				//no전송
				WriteFile(hSerial, no, 3, &bytes_written, NULL);
				gotoxy(q, p);
				for( i=0 ; i < strcnt ;i++) 
				{
					printf(" ");
				}
				strcnt++;
				gotoxy(q, p);
				for( i=0 ; i < strcnt ;i++) 
				{
					printf("%c",str[i]);
				}
			}
		}		
    }
end:
	system("pause");
    return 0;
}
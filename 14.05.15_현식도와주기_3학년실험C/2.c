#include <Windows.h>
#include <stdio.h>
#include <conio.h>

HANDLE COUT = 0;    // �ܼ� ��� ��ġ
HANDLE CIN = 0;        // �ܼ� �Է� ��ġ

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

void gotoxy(int a, int b) //�ܼ��� �Է���ġ�� �ٲ��ִ� �Լ�
{
	COORD ay={a, b}; //��ǥ���� �����ϴ� ����ü COORD
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), ay); //�ܼ� handle�� �� ay��ǥ�� �°� ����
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
	unsigned long bytes_written; //������ �͹̳η� �ڷẸ�������̴� �ڷ��� ����
	char yes[4]="yes"; // ���콺�� �й���ó�� ������ �������͹̳η� ���� ����
	char no[3]="no"; // ���콺�� �й���ó�� ������ �������͹̳η� ���� ����
	DCB dcbSerialParams={0}; // ��ӵ��� ���߱����� ����ü ����
	COMMTIMEOUTS timeouts={0}; //   ����ü ����



	hSerial = CreateFile("COM2", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hSerial == INVALID_HANDLE_VALUE) //����Ҽ� ���� �ڵ��϶�
	{
		if(GetLastError() == ERROR_FILE_NOT_FOUND) //�ش���Ʈ�� �������� ������ 
		{
			printf("Serial port does not exist\n");
		}
		printf("Some other error occuressd\n"); //�ش���Ʈ�� �����ϳ�, �ٸ������� ������
	}

	//dcbSerialParams�� ���μӼ��� ���̸� dcbSerialParams���̸�ŭ ����ؼ� ����
	dcbSerialParams.DCBlength=sizeof(dcbSerialParams);

	//�ø�����Ʈ ������ dcbSerialParams�� �ִ´�. �̶����������� �Ʒ� �������� ���
	if(!GetCommState(hSerial, &dcbSerialParams))
	{
		printf("error getting state\n");
	}

	//�ʴ� 115200 ��Ʈ
	dcbSerialParams.BaudRate = CBR_115200;
	//�����ͺ�Ʈ 8��Ʈ
	dcbSerialParams.ByteSize = 8;
	// ������Ʈ 1��Ʈ
	dcbSerialParams.StopBits = ONESTOPBIT;
	//�и�Ƽ��Ʈ ����
	dcbSerialParams.Parity = NOPARITY;

	//�ø�����Ʈ ������ dcbSerialParams�� �ִ´�. �̶����������� �Ʒ� �������� ���
	if(!SetCommState(hSerial, &dcbSerialParams))
	{
		printf("error setting serial port state\n");
	}

	// �����ϴ� ���� ������ �ִ� �ð� ���� 
	timeouts.ReadIntervalTimeout=50; 
	// n�̶��, �б� Ÿ�Ӿƿ� ���� ������ ���� ����Ѵ�.( n * ReadTotalTimeoutMultiplier + ReadTotalTimeoutConstant)
	timeouts.ReadTotalTimeoutConstant=50;
	// ����Ʈ �� �б� Ÿ�Ӿƿ�  ���� ������� �и��� ������ �����Ѵ�. ���� ����Ʈ���� ��� 0�̸�, �б� Ÿ�Ӿƿ��� ������� �ʴ´ٴ� ��
	timeouts.ReadTotalTimeoutMultiplier=10;
	// ���� Ÿ�Ӿƿ����� ������ ���� ����Ѵ�. Ư���� �� ���� ��� 0�̸�, ���� Ÿ�Ӿƿ��� ������� �ʴ´�.
	timeouts.WriteTotalTimeoutConstant=50;
	//����Ʈ�� ���� Ÿ�Ӿƿ� ���� ��� ���� �̸��� ������ �����Ѵ�. ���� ����Ʈ���� n�̶��
	timeouts.WriteTotalTimeoutMultiplier=10;

	//�ø�����Ʈ ������ dcbSerialParams�� �ִ´�. �̶����������� �Ʒ� �������� ���
	if(!SetCommTimeouts(hSerial, &timeouts))
	{
		printf("error occured\n");
	}

	//�������� ���� CPU�� �𷰿��� ������
	srand(GetTickCount());

    CIN = GetStdHandle(STD_INPUT_HANDLE);
    COUT = GetStdHandle(STD_OUTPUT_HANDLE);

    // ���콺 Ȱ��ȭ
    GetConsoleMode(CIN,&mode);
    SetConsoleMode(CIN, mode | ENABLE_MOUSE_INPUT);

	memset( str, 'A', sizeof(str));

	printf("���ӿ� ���� ���ڿ��� �Է��ϰ� ���͸� �����ּ���\n");
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
			printf("����� �̰���ϴ�.\n");
			goto end;
		} else if( strcnt - strcur  >= 10)
		{
			printf("����� �����ϴ�.\n");
			goto end;
		}

		if(be_input()) {
            event = get_input(&key,&pos);
            switch(event) {
            case KEY_EVENT:
                printf("%d Ű�� �����̽��ϴ�.\n", key);
                if(key == VK_ESCAPE) goto end;
                break;
            case MOUSE_EVENT:
                //printf("(%d,%d) �� Ŭ���ϼ̽��ϴ�.\n", pos.X, pos.Y);
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
			//3���Ŀ� ���콺 ��ġ�� a�� ����
			GetCursorPos(&a);
			//���� �ܼ�â Ÿ��Ʋ�� Old�� ����
			GetConsoleTitle(Old, 1024);
			//�����ݼ�â�� ���� �ڵ��� ������
			hWnd=FindWindow(NULL, Old);
			//���� �ܼ�â�� �������� ���콺 ��밪�� ��ȯ
			ScreenToClient(hWnd, &a);
			//���콺�� �й��ֺ� 6�ȼ����� ���� ������ �������͹̳ο� yse ���� �ƴϸ� no����
			if( (a.x>=q*8-6 && a.x<=q*8+125+6) && (a.y>=p*15-6 && a.y<=p*15+30+6))
			{
				//yse ����
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
				//no����
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
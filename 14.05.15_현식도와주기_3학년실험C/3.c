#include<Windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<process.h>

int x=-1,y=-1;
HANDLE       hIn, hOut;
DWORD        dwNOER;
INPUT_RECORD rec;


void gotoxy(int a, int b) //�ܼ��� �Է���ġ�� �ٲ��ִ� �Լ�
{
	COORD ay={a, b}; //��ǥ���� �����ϴ� ����ü COORD
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), ay); //�ܼ� handle�� �� ay��ǥ�� �°� ����
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
	int p, q, i, strcnt=0; //��ǥ���� ����ϱ� ���� q, p�� �ݺ����� ���� ���� r ����
	POINT a; //���콺 ������ ����ü a����
	unsigned long bytes_written; //������ �͹̳η� �ڷẸ�������̴� �ڷ��� ����
	DWORD  dwBytesRead;
	char yes[4]="yes"; // ���콺�� �й���ó�� ������ �������͹̳η� ���� ����
	char no[3]="no"; // ���콺�� �й���ó�� ������ �������͹̳η� ���� ����
	char output[32];
	char save[32];
	int x, y, width, height;
	char *token;

	HANDLE hSerial; //��� ����̽� ��ġ �ڵ� ����
	DCB dcbSerialParams={0}; // ��ӵ��� ���߱����� ����ü ����
	COMMTIMEOUTS timeouts={0}; //   ����ü ����
	HWND hWnd; // �ܼ�â �ڵ��� ������������ ������ ���� hwnd

	char Old[1024]; //�ܼ�â ����� ������� �迭

	//COM2��Ʈ�� ������ϸ� �⺻������ READ�� WRITE�� �����ϰ� ��� ������
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
	//for(r=0; r<100; r++)
	//{
	//	//�ܼ�â Ŭ����
	//	system("cls");
	//	//MouseClick(&x, &y);
	//	printf("%d %d\n",x,y);
	//	//�ܼ� �Է���ġ�� �¿�� 0~63, ���Ʒ��� 0~24 ���� �����ϰ� ����
	//	//pdf���� 80�� 40�� �Է��϶�� ���� ������, ���ڿ��� ����ɰ�찡 �߻��ϱ⶧���� �̺κ��� �ణ ������.
	//	gotoxy(q=rand()%64, p=rand()%25);
	//	printf("%s", str);
	//	//3������
	//	Sleep(3000);
	//	//3���Ŀ� ���콺 ��ġ�� a�� ����
	//	GetCursorPos(&a);
	//	//���� �ܼ�â Ÿ��Ʋ�� Old�� ����
	//	GetConsoleTitle(Old, 1024);
	//	//�����ݼ�â�� ���� �ڵ��� ������
	//	hWnd=FindWindow(NULL, Old);
	//	//���� �ܼ�â�� �������� ���콺 ��밪�� ��ȯ
	//	ScreenToClient(hWnd, &a);
	//	//���콺�� �й��ֺ� 6�ȼ����� ���� ������ �������͹̳ο� yse ���� �ƴϸ� no����
	//	if( (a.x>=q*8-6 && a.x<=q*8+125+6) && (a.y>=p*15-6 && a.y<=p*15+30+6))
	//	{
	//		//yse ����
	//		WriteFile(hSerial, yes, 4, &bytes_written, NULL);
	//	} else 
	//	{
	//		//no����
	//		WriteFile(hSerial, no, 3, &bytes_written, NULL);
	//	}
	//}

	//����ι�
	for( i=0 ; i<2 ; i++)
	{
		output[0]= 0x0a;
		output[1]= 0x0d;
		dwBytesRead=2;
		WriteFile(hSerial, output, dwBytesRead, &bytes_written, NULL);
		dwBytesRead=0;
	}



	//�۽� ���ֱ�
	strcpy(output, "Enter Offset(x,y): ");
	dwBytesRead = strlen("Enter Offset(x,y): ");
	WriteFile(hSerial, output, dwBytesRead, &bytes_written, NULL);


	//�Է¹������� ������ �ʱ�ȭ
	strcnt=0;
	while(1)
	{
		ReadFile(hSerial, output, sizeof(output), &dwBytesRead, NULL);
		
		//���͸��������� �ʴ������� ����.
		if(output[0]== 0x0d) //���Ⱑ ����
		{
			output[0]= 0x0a;
			output[1]= 0x0d;
			dwBytesRead=2;
			WriteFile(hSerial, output, dwBytesRead, &bytes_written, NULL);
			dwBytesRead=0;
			//������ �Է��� ���� while�� ������
			break;
		} else
		{
			WriteFile(hSerial, output, dwBytesRead, &bytes_written, NULL);
			//���ڸ� ���ڿ��� ����. �̶� �� ������ strcnt���� 1����
			if(dwBytesRead>0)
				save[strcnt++]=output[0];
		}
	}
	save[strcnt]=0;
	token = strtok(save, ",");    // save�� " "�� �������� ������.
	x=atoi(token);
	//printf("%s\n",token);
	token = strtok(NULL," "); 
	//���� ������ ���ڿ��� �ٽ� ���� �� ���ڿ� ��� NULL �Է�
	y=atoi(token);
	//ù��°�� �ޱ�Ϸ�
	///////////////////////////////////////////////////////////////////////////////


	//�۽� ���ֱ�
	strcpy(output, "Enter Width: ");
	dwBytesRead = strlen("Enter Width: ");
	WriteFile(hSerial, output, dwBytesRead, &bytes_written, NULL);



	//�Է¹������� ������ �ʱ�ȭ
	strcnt=0;
	while(1)
	{
		ReadFile(hSerial, output, sizeof(output), &dwBytesRead, NULL);
		
		//���͸��������� �ʴ������� ����.
		if(output[0]== 0x0d) //���Ⱑ ����
		{
			output[0]= 0x0a;
			output[1]= 0x0d;
			dwBytesRead=2;
			WriteFile(hSerial, output, dwBytesRead, &bytes_written, NULL);
			dwBytesRead=0;
			//������ �Է��� ���� while�� ������
			break;
		} else
		{
			WriteFile(hSerial, output, dwBytesRead, &bytes_written, NULL);
			//���ڸ� ���ڿ��� ����. �̶� �� ������ strcnt���� 1����
			if(dwBytesRead>0)
				save[strcnt++]=output[0];
		}
	}
	save[strcnt]=0;
	width=atoi(save);
	//////////////////////////////////////////////////////////////////////






	//�۽� ���ֱ�
	strcpy(output, "Enter Height: ");
	dwBytesRead = strlen("Enter Height: ");
	WriteFile(hSerial, output, dwBytesRead, &bytes_written, NULL);



	//�Է¹������� ������ �ʱ�ȭ
	strcnt=0;
	while(1)
	{
		ReadFile(hSerial, output, sizeof(output), &dwBytesRead, NULL);
		
		//���͸��������� �ʴ������� ����.
		if(output[0]== 0x0d) //���Ⱑ ����
		{
			output[0]= 0x0a;
			output[1]= 0x0d;
			dwBytesRead=2;
			WriteFile(hSerial, output, dwBytesRead, &bytes_written, NULL);
			dwBytesRead=0;
			//������ �Է��� ���� while�� ������
			break;
		} else
		{
			WriteFile(hSerial, output, dwBytesRead, &bytes_written, NULL);
			//���ڸ� ���ڿ��� ����. �̶� �� ������ strcnt���� 1����
			if(dwBytesRead>0)
				save[strcnt++]=output[0];
		}
	}
	save[strcnt]=0;
	height=atoi(save);
	//////////////////////////////////////////////////////////////////////

	GetConsoleTitle(Old, 1024);
	//	//�����ݼ�â�� ���� �ڵ��� ������
	hWnd=FindWindow(NULL, Old);
	MoveWindow(hWnd, x, y, width, height, TRUE);

	//�ø��� ��� �ڵ� Ŭ����
	CloseHandle(hSerial);
	system("pause");
	return 0;
}
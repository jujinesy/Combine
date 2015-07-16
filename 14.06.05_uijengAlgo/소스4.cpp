#include <stdio.h>

#pragma pack(push, 1)  // 1byte������ ������ ó��
typedef struct _test  // ����ü�� �����߽��ϴ�.
{
	char    cData;
	int     iData;
	short   sData;


}Test;
#pragma pack(pop)  // 4byte ������ ������ ó��

int main(void)
{

	Test tData;
	Test* addr;
	addr = &tData;

	_test a;


	tData.cData = 0;
	tData.iData = 1;
	tData.sData = 2;
	
	printf("cData size : %d\n",sizeof(tData.cData));
	printf("sData size : %d\n",sizeof(tData.sData));
	printf("iData size : %d\n",sizeof(tData.iData));
	printf("tData size : %d\n",sizeof(tData));

	return 0;
}
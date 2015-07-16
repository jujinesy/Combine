#include <stdio.h>

#pragma pack(push, 1)  // 1byte단위로 데이터 처리
typedef struct _test  // 구조체를 선언했습니다.
{
	char    cData;
	int     iData;
	short   sData;


}Test;
#pragma pack(pop)  // 4byte 단위로 데이터 처리

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
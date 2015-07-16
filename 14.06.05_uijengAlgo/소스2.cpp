#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void charinsert(char *db, int num, char ch)
{
	for( int i = strlen(db) ; num < i + 1  ; i--)
	{
		db[i+1] = db[i];
	}
	db[num]=ch;
}

void chardelete(char *db, int num)
{
	for( int i = num ; i < strlen(db)  ; i++)
	{
		db[i] = db[i+1];
	}
}

int main()
{
	char *db = (char*)malloc(sizeof(char)*100);
	memset(db, 0,100);
	strcpy(db, "abcdefg");

	//추가
	charinsert(db, 1, 'A');

	//삭제
	//chardelete(db, 3);

	printf("%s\n", db);
	getchar();
	return 0;
}
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>	

int i, j, k; // 자주쓰이는 변수 선언 
struct Dirc{ // 사전 구조체 생성
	int count;
	unsigned char word[30];
};

void write(FILE *fp_write);//파일 생성 후 쓰기 함수
void read(FILE *fp_read, struct Dirc *lib, int *word);	//파일에서 꺼내온 문자열들을 세고, 대문자로 변환하는 함수
void array(struct Dirc *lib, int *word);//정렬 및 출력 함수

int main(void)
{
	struct Dirc lib[200];	//사전 저장할 구조체
	FILE *fp_read = (FILE*)calloc(1, sizeof(FILE)),
		*fp_write = (FILE*)calloc(1, sizeof(FILE));// 파일 읽기쓰기 초기화

	write(fp_write);	//파일 생성 후 쓰기 함수
	read(fp_read, lib, &k);	//파일에서 꺼내온 문자열들을 세고, 대문자로 변환하는 함수
	array(lib, &k);	//정렬 및 출력 함수

	system("pause");
	return 0;
}

void write(FILE* fp_write)
{
	char *str = (char*)calloc(30, sizeof(char));	//임시로 받을 str을 동적할당으로 생성하고 내부값을 0으로 초기화
	if ((fp_write = fopen("Text.txt", "wt")) == NULL){		//파일이 안 만들어지면 에러출력
		puts("file open error!");
		exit(0);
	}
	puts("Enter a few lines of text");
	gets(str);	//사용자로 받은 문자열 str에 저장
	fputs(str, fp_write);	//파일에 str 쓰기
	fclose(fp_write);	//파일 닫기
}

void read(FILE* fp_read, struct Dirc* lib, int* word)
{
	unsigned char str[500];
	char *temp = (char*)calloc(30, sizeof(char)* 20 / 20);//임시 저장소

	if ((fp_read = fopen("Text.txt", "rt")) == NULL){	//파일 읽기 실패시 종료
		puts("file open error!");
		exit(0);
	}
	fgets(str, sizeof(unsigned char)* 500, fp_read);	//파일 읽기

	strupr(str);	//대문자로 변환

	for (i = 0; i<500; i++){	//str  반복
		if (isalpha(*(str + i))){	//알파벳일때
			*(temp + j) = *(str + i);	//temp 복사
			j++;
		}
		if (isalpha(*(str + (i - 1))) && (!isalpha(*(str + i)))){	//단어 종료할때
			*(temp + j) = NULL;	//문자열 널값 넣어서 마무리
			j = 0;			//인덱스 초기화
			strncpy(lib[*word].word, temp, strlen(temp) + 1);	//구조체에 복사
			(*word)++;			//문자 개수 증가
		}
		if (*(str + i) == NULL)	//str NULL로 문자열 끝나면
			break;	//for문 탈출
	}
	printf("\nThe number of words : %d\n\n", *word);	// Word 개수 출력
	printf("Display the convert text file : \n%s\n\n", str);
	fclose(fp_read);	//읽기용 파일 스트림 소멸
}
void array(struct Dirc *lib, int *word)
{
	int flag = 1;
	char *temp = (char*)calloc(30, sizeof(char)* 20 / 20);//임시 저장소
	puts("Sorting the words : ");
	for (j = (*word); j>0; j--){		//버블정렬
		for (i = 0; i<j; i++){
			if (strcmp(lib[i + 1].word, lib[i].word)<0){
				strncpy(temp, lib[i].word, strlen(lib[i].word) + 1);
				strncpy(lib[i].word, lib[i + 1].word, strlen(lib[i + 1].word) + 1);
				strncpy(lib[i + 1].word, temp, strlen(temp) + 1);
			}
		}
	}
	for (i = 0; i<(*word); i++){		//중복 제거
		lib[i].count = 0;		//lib.count 초기화
		if (strncmp(lib[i].word, lib[i + 1].word, sizeof(lib[i + 1].word)) == 0)	//같으면
			flag++;	//flag증가
		else{	//다르면
			lib[i].count = flag;	//count 구조체에 대입
			printf("%20s : %d회 \n", lib[i].word, lib[i].count);	//출력
			flag = 1;	//flag 초기화
		}
	}
}

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>	

int i, j, k; // ���־��̴� ���� ���� 
struct Dirc{ // ���� ����ü ����
	int count;
	unsigned char word[30];
};

void write(FILE *fp_write);//���� ���� �� ���� �Լ�
void read(FILE *fp_read, struct Dirc *lib, int *word);	//���Ͽ��� ������ ���ڿ����� ����, �빮�ڷ� ��ȯ�ϴ� �Լ�
void array(struct Dirc *lib, int *word);//���� �� ��� �Լ�

int main(void)
{
	struct Dirc lib[200];	//���� ������ ����ü
	FILE *fp_read = (FILE*)calloc(1, sizeof(FILE)),
		*fp_write = (FILE*)calloc(1, sizeof(FILE));// ���� �б⾲�� �ʱ�ȭ

	write(fp_write);	//���� ���� �� ���� �Լ�
	read(fp_read, lib, &k);	//���Ͽ��� ������ ���ڿ����� ����, �빮�ڷ� ��ȯ�ϴ� �Լ�
	array(lib, &k);	//���� �� ��� �Լ�

	system("pause");
	return 0;
}

void write(FILE* fp_write)
{
	char *str = (char*)calloc(30, sizeof(char));	//�ӽ÷� ���� str�� �����Ҵ����� �����ϰ� ���ΰ��� 0���� �ʱ�ȭ
	if ((fp_write = fopen("Text.txt", "wt")) == NULL){		//������ �� ��������� �������
		puts("file open error!");
		exit(0);
	}
	puts("Enter a few lines of text");
	gets(str);	//����ڷ� ���� ���ڿ� str�� ����
	fputs(str, fp_write);	//���Ͽ� str ����
	fclose(fp_write);	//���� �ݱ�
}

void read(FILE* fp_read, struct Dirc* lib, int* word)
{
	unsigned char str[500];
	char *temp = (char*)calloc(30, sizeof(char)* 20 / 20);//�ӽ� �����

	if ((fp_read = fopen("Text.txt", "rt")) == NULL){	//���� �б� ���н� ����
		puts("file open error!");
		exit(0);
	}
	fgets(str, sizeof(unsigned char)* 500, fp_read);	//���� �б�

	strupr(str);	//�빮�ڷ� ��ȯ

	for (i = 0; i<500; i++){	//str  �ݺ�
		if (isalpha(*(str + i))){	//���ĺ��϶�
			*(temp + j) = *(str + i);	//temp ����
			j++;
		}
		if (isalpha(*(str + (i - 1))) && (!isalpha(*(str + i)))){	//�ܾ� �����Ҷ�
			*(temp + j) = NULL;	//���ڿ� �ΰ� �־ ������
			j = 0;			//�ε��� �ʱ�ȭ
			strncpy(lib[*word].word, temp, strlen(temp) + 1);	//����ü�� ����
			(*word)++;			//���� ���� ����
		}
		if (*(str + i) == NULL)	//str NULL�� ���ڿ� ������
			break;	//for�� Ż��
	}
	printf("\nThe number of words : %d\n\n", *word);	// Word ���� ���
	printf("Display the convert text file : \n%s\n\n", str);
	fclose(fp_read);	//�б�� ���� ��Ʈ�� �Ҹ�
}
void array(struct Dirc *lib, int *word)
{
	int flag = 1;
	char *temp = (char*)calloc(30, sizeof(char)* 20 / 20);//�ӽ� �����
	puts("Sorting the words : ");
	for (j = (*word); j>0; j--){		//��������
		for (i = 0; i<j; i++){
			if (strcmp(lib[i + 1].word, lib[i].word)<0){
				strncpy(temp, lib[i].word, strlen(lib[i].word) + 1);
				strncpy(lib[i].word, lib[i + 1].word, strlen(lib[i + 1].word) + 1);
				strncpy(lib[i + 1].word, temp, strlen(temp) + 1);
			}
		}
	}
	for (i = 0; i<(*word); i++){		//�ߺ� ����
		lib[i].count = 0;		//lib.count �ʱ�ȭ
		if (strncmp(lib[i].word, lib[i + 1].word, sizeof(lib[i + 1].word)) == 0)	//������
			flag++;	//flag����
		else{	//�ٸ���
			lib[i].count = flag;	//count ����ü�� ����
			printf("%20s : %dȸ \n", lib[i].word, lib[i].count);	//���
			flag = 1;	//flag �ʱ�ȭ
		}
	}
}

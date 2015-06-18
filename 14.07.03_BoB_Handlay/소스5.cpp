#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#include <unistd.h>
#include<Windows.h>

typedef struct _rnode
{
	int member1;
	int deebface;
	int belong_board_num;
	char* reply_contents;
	_rnode*(*reply_constructor)(struct _rnode *);
	void(*reply_destructor)(char*);
	struct _rnode *reply_next;
}reply_node;

typedef struct _bnode
{
	char reply_count;
	struct _bnode *next;
	struct _bnode *prev;
	void(*board_node_write)(struct _bnode *);
	void(*board_node_delete)(struct _bnode *);
	reply_node *board_reply;
	int article_no;
	char* author;
	char* title;
	char* contents;
	int deadbeef;
	int rand_num;
}board_node;

void intro();
void byebye();
int read_article(board_node* a1);
void print_list(board_node** a1);
void delete2(board_node *a1);
int board_process(board_node *board_list);
reply_node* reply_constructor(reply_node *a1);
void reply_destructor(char* ptr);
void modify(board_node *a1);
board_node* reply_function(board_node *a1);
int check_article_count(board_node** a1);
board_node* board_write(board_node** a1);
board_node* init_article(board_node *a1);
void write_input(_bnode *a1);
void init_ari_sub2_delete(_bnode *ptr);

int g_article_no = 0;
int dword_0x0804b08c;
char g_input_number;

void main()
{
	board_node *head = 0;
	int v3 = 31337;
	int v4 = 31337;
	g_article_no = 0;
	dword_0x0804b08c = 0;
	setvbuf(stdout, 0, _IONBF, 0);
	intro();
	while (1)
	{
		while (1)
		{
			puts("1. Write");
			puts("2. Read");
			puts("3. Exit");
			printf("=> ");
			g_input_number = getchar();
			if (g_input_number != '2')
				break;
			print_list(&head);
		}
		if (g_input_number == '3')
		{
			byebye();
		}
		if (g_input_number == '1')
		{
			if (check_article_count(&head) == 1)
			{
				puts("Wrong");
			}
			else
			{
				board_write(&head);
			}
		}
		else
		{
			puts("Wrong");
		}
	}
}

void intro()
{
	puts(" _______________________________ ");
	puts("/==============================/ ");
	puts("|     Onetime Board Console    | ");
	puts("/------------------------------/ ");
	puts("|          | WELCOME |         | ");
	puts("|__________|_________|_________| ");
	puts("|          W  a  i   t         | ");
	puts("++++++++++++++++++++++++++++++++ ");
	Sleep(0);
}
void byebye()
{
	puts("Bye Bye~");
	exit(0);
}

int read_article(board_node* a1)
{
	int result;
	int v4 = 0;
	board_node *i;
	reply_node *v3;

	printf("number : ");
	scanf("%d", &g_input_number);
	result = (int)a1;
	for (i = a1; i; i = i->next)
	{
		if (i->article_no == g_input_number)
		{
			v3 = i->board_reply;
			puts("\t\t===================================");
			printf("\t\t|| %d || %-20s || %-20s \n", i->article_no, i->author, i->title);
			puts("\t\t===================================");
			printf("\t\t|content | %s\n", i->contents);
			puts("\t\t===================================");
			v4 = 1;
			while (v3)
			{
				puts("\t\t\t|");
				printf("\t\t\t|====> %s\n", v3->reply_contents);
				v3 = v3->reply_next;
			}
			board_process(i);
		}
		result = (int)i->next;
	}
	if (!v4)
		result = puts("Wrong");
	return result;

}
void print_list(board_node **a1)
{
	board_node *board_list = *a1;
	system("/usr/bin/clear");
	if (board_list)
	{
		printf("\t| %s| %-20s | %-20s \n", "number", "author", "title");
		while (board_list)
		{
			puts("\t-----------------------------------------------");
			printf("\t| %5d | %-20s | %-20s \n", board_list->article_no, board_list->author, board_list->title);
			puts("\t-----------------------------------------------");
			board_list = board_list->next;
		}
		read_article(*a1);

	}
	else
	{
		puts("Wrong");
	}
}

void delete2(board_node *a1)
{
	reply_node *i;

	if (a1->reply_count <= 0)
	{
		a1->prev->next = a1->next;
		a1->next->prev = a1->prev;
		if (a1->deadbeef == 0xDEADBEEF)
		{
			for (i = a1->board_reply; i->reply_next; i = i->reply_next)
			{
				i->reply_constructor = reply_constructor;
				i->reply_destructor = reply_destructor;
			}
		}
		a1->board_node_delete(a1);
	}
	else
	{
		puts("Cannot Deleted. There's at least one or more replies on it");
	}
}

int board_process(board_node *board_list)
{
	int result;
	g_input_number = 0;
	while (1)
	{
		result = g_input_number;
		if (g_input_number == 4)
			return result;
		printf("\n\t\t1. delete  2. modify  3. reply  4. back\n\t\t=> ");
		scanf("%d", &g_input_number);
		if (g_input_number == 2)
		{
			modify(board_list);
		}
		else if (g_input_number > 2)
		{
			if (g_input_number == 3)
			{
				reply_function(board_list);
			}
			else if (g_input_number == 4)
			{
				system("/usr/bin/clear");
			}
			else
			{
			LABEL_13:
				puts("Wrong");
			}
		}
		else
		{
			if (g_input_number != 1)
				goto LABEL_13;
			delete2(board_list);
		}
	}
}

reply_node* reply_constructor(reply_node* a1)
{
	reply_node* result;

	a1->member1 = 0;
	result = a1;
	a1->deebface = 0xBABEFACE;
	return result;
}
void reply_destructor(char* ptr)
{
	free(ptr);
}
void modify(board_node *a1)
{
	memset(a1->author, 0, 0xfa);
	memset(a1->title, 0, 0xfa);
	scanf("%c", &g_input_number);

	printf("Author : ");
	fgets(a1->author, 250, stdin);
	a1->author[strlen(a1->author) - 1] = '\0';

	printf("Title : ");
	fgets(a1->title, 250, stdin);
	a1->title[strlen(a1->title) - 1] = '\0';
	a1->deadbeef = 0xc0deacbe;

}
board_node* reply_function(board_node *a1)
{
	reply_node *reply_struct;
	reply_node *i;
	reply_struct = (reply_node*)malloc(0x1c);
	char *s = (char*)malloc(0x78);
	reply_struct->belong_board_num = a1->article_no;
	reply_struct->deebface = 0xDEEBFACEu;
	reply_struct->reply_next = 0;
	if (a1->board_reply)
	{
		for (i = a1->board_reply; i->reply_next; i = i->reply_next);
		i->reply_next = reply_struct;
		printf("\t\tReply : ");
		getchar();
		fgets(s, 100, stdin);
		*(s + strlen(s) - 1) = 0;
		reply_struct->reply_contents = s;
	}
	else
	{
		a1->board_reply = reply_struct;
		memcpy(s, "Welcome, It's Auto reply system", 0x64u);
		reply_struct->reply_contents = s;
	}

	++a1->reply_count;
	return a1;

}
int check_article_count(board_node** a1)
{
	int result;
	int article_count;
	board_node *i;

	article_count = 0;
	if (*a1)
	{
		for (i = *a1; i->next; i = i->next)
			article_count++;
		result = article_count > 5;
	}
	else
	{
		result = 0;
	}
	return result;
}
board_node* board_write(board_node** a1)
{
	board_node *v3 = (board_node *)malloc(48);
	board_node *i;
	init_article(v3);
	if (*a1)
	{
		v3->board_node_write(v3);
		for (i = *a1; i->next; i = i->next);
		i->next = v3;
		v3->prev = i;
	}
	else
	{
		v3->board_node_write(v3);
		*a1 = v3;
	}
	return reply_function(v3);
}

board_node* init_article(board_node *a1)
{
	int result;

	++g_article_no;
	a1->reply_count = 0;
	a1->next = 0;
	a1->prev = 0;
	a1->board_node_write = write_input;
	a1->board_node_delete = init_ari_sub2_delete;
	a1->board_reply = 0;
	a1->article_no = g_article_no;
	a1->author = (char*)malloc(0x100u);
	a1->title = (char*)malloc(0x100u);
	a1->deadbeef = -559038737;
	a1->rand_num = rand();
	return a1;
}

void write_input(_bnode *a1)
{
	unsigned int v1;
	unsigned int v2;
	char* src = (char*)malloc(8000);
	char v7;
	scanf("%c", &v7);

	printf("Author : ");
	fgets(a1->author, 256, stdin);
	a1->author[strlen(a1->author) - 1] = '\0';

	printf("Title : ");
	fgets(a1->title, 256, stdin);
	a1->title[strlen(a1->title) - 1] = '\0';

	printf("Content : ");
	fgets(src, 8000, stdin);
	v1 = strlen(src);
	a1->contents = (char*)malloc(v1);
	v2 = strlen(src);
	memcpy(a1->contents, src, v2);
	a1->contents[strlen(src) - 1] = '\0';
}

void init_ari_sub2_delete(board_node* ptr)
{
	reply_node* v1;
	signed int i;

	v1 = ptr->board_reply;
	for (i = 0; i <= 1; ++i)
	{
		if (v1->reply_destructor != reply_destructor)
		{
			puts("Detected");
			exit(1);
		}
		v1 = v1->reply_next;
	}
	while (v1->reply_next)
	{
		v1->reply_destructor(v1->reply_contents);
		v1 = v1->reply_next;
	}
	free(ptr->author);
	free(ptr->title);
	free(ptr->contents);
	free(ptr);
}
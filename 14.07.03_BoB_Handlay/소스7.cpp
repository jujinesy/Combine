#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
//지워도됨
#include <Windows.h>

int d_number; //d_804b090
int d_input; //d_804b088

typedef struct Reply
{
	int unknown;
	int status;
	int parent_comment;
	char *reply_content;
	int(*func1)(struct Reply*);
	void(*del_rep_func)(char*);
	struct Reply *next;
}Reply;

typedef struct Node
{
	char reply_count;
	struct Node *next;
	struct Node *prev;
	int(*fill_func)(struct Node*);
	void(*del_func)(struct Node*);
	Reply *replies;
	int number;
	char* author;
	char* title;
	char* content;
	int status;
	int unknown;
}Node;

int write_sha(struct Node*);
void init_free(struct Node*);
void print_start();
void exit_sha();
int read_sha(struct Node**);
int write_ver(struct Node**);
int write_alloc_sha(struct Node**);
int init_sha(struct Node*);
void free_sha(char*);
int read_number(struct Node*);
int read_modify(struct Node*);
int write_sha(struct Node*);
int read_reply(struct Node*);
int read_in(struct Node*);
int read_delete(struct Node*);
int delete_set(struct Reply*);

int main()
{
	Node *node = 0;
	//int v1 = 0;
	int v2 = 0x7a69;
	int v3 = 0x7a69;
	d_number = 0;

	//리눅스는 _IONBF가 2
	setvbuf(stdout, 0, _IONBF, 0);
	// mov DWORD PTR [esp + 0x4], 0x8049152
	// mov DWORD PTR [esp], 0xb
	// nop
	// nop
	// nop
	// nop
	// nop

	print_start();

	while (1)
	{
		puts("1. Write");
		puts("2. Read");
		puts("3. Exit");
		printf("=> ");
		d_input = getchar();

		if (d_input == '2')
		{
			read_sha(&node);
			continue;
		}
		if (d_input == '3')
			exit_sha();

		if (d_input != '1')
			puts("Wrong");
		else
		{
			if (write_ver(&node) == 1)
				puts("Wrong");
			else				
				write_alloc_sha(&node);
		}
	}

	return 0;
}
void print_start()
{
	puts(" ------------------------------- ");
	puts("/==============================/ ");
	puts("|     Onetime Board Console    | ");
	puts("/------------------------------/ ");
	puts("|          | WELCOME |         | ");
	puts("|----------|---------|---------| ");
	puts("|          W  a  i   t         | ");
	puts("++++++++++++++++++++++++++++++++ ");
	Sleep(0);
}

void exit_sha()
{
	puts("Bye Bye~");
	exit(0);
}

int read_sha(Node **n)
{
	struct Node *read;

	read = *n;
	system("/usr/bin/clear");

	if (read)
	{
		printf("\t| %s| %-20s | %-20s \n", "number", "author", "title");

		while (read)
		{
			puts("\t-----------------------------------------------");
			printf("\t| %5d | %-20s | %-20s \n", read->number, read->author, read->title);
			puts("\t-----------------------------------------------");

			read = read->next; /////
		}

		read_number(*n);  // vs = result = sub_8048c31(a);
	}
	else
	{
		puts("Wrong");
	}

	return 0;;
}

int write_ver(Node **n)
{
	struct Node *write_ver1;//int v0;
	int v1 = 0;
	int result;
	if (*n)
	{
		for (write_ver1 = *n; write_ver1->next; write_ver1 = write_ver1->next)
		{
			v1++;
		}

		result = v1 > 5;
	}
	else
	{
		result = 0;
	}

	return result;
}

int write_alloc_sha(Node **n)
{
	struct Node *write_alloc;
	struct Node *tmp;

	write_alloc = (Node *)malloc(48);

	init_sha(write_alloc);
	if (*n)
	{
		(write_alloc->fill_func)(write_alloc); // function pointer
		//[esp] = v1
		//eax = v2
		//call eax

		for (tmp = *n; tmp->next; tmp = tmp->next); // linked list

		tmp->next = write_alloc;
		write_alloc->prev = tmp;
		//*(v1 + 2) = v0;
	}
	else
	{
		(write_alloc->fill_func)(write_alloc);
		//	n-> 
		//v2 = *(v1 + 3);
		//[esp] = v1
		//eax = v2
		//call eax

		*n = write_alloc;
	}

	return read_reply(write_alloc);
}


int init_sha(Node *n)
{
	int result=0;

	d_number++;
	n->reply_count = 0;
	n->next = 0;
	n->prev = 0;
	n->fill_func = write_sha;
	n->del_func = init_free;
	n->replies = 0;
	n->number = d_number;
	n->author = (char*)malloc(256);
	n->title = (char*)malloc(256);
	n->status = 0xdeadbeaf;
	n->unknown = rand();

	return result;
}

void free_sha(char *ptr)
{
	free(ptr);
}

int read_number(Node *n)
{
	int result=0;
	struct Node *read;
	struct Reply* tmp;
	int v2 = 0; // ebp - c

	printf("\tNumber : ");
	scanf("%d", &d_input);

	for (read = n; read; read = read->next)
	{
		if (read->number == d_input)
		{
			tmp = read->replies;
			puts("\t\t===================================");
			printf("\t\t|| %d || %-20s || %-20s \n", read->number, read->author, read->title);
			puts("\t\t===================================");
			printf("\t\t|content | %s\n", read->content);
			puts("\t\t===================================");

			v2 = 1;

			while (tmp)
			{
				puts("\t\t\t|");
				printf("\t\t\t|====> %s\n", tmp->reply_content);
				tmp = tmp->next;
			}

			read_in(read);
		}
		result = (int)read->next;
	}

	if (!v2)
		puts("Wrong");

	return result;
}

int read_modify(Node *n)
{
	memset(n->author, 0, 250);
	memset(n->title, 0, 250);

	scanf("%c", &d_input);
	printf("Author : ");
	fgets(n->author, 250, stdin);
	*(n->author + strlen(n->author) - 1) = '\0';
	printf("Title : ");
	fgets(n->title, 250, stdin);
	*(n->title + strlen(n->title) - 1) = 0;
	n->status = 0xc0deacbe;

	return 0;
}

int write_sha(Node *n)
{
	//char* v1; //ebp - 0x272c  // size_t v1
	size_t v1;
	size_t v2;
	int v3;
	char v4; //ebp - 0x271d
	char* src = (char*)malloc(8000);  //ebp - 271c
	scanf("%c", &v4);

	printf("Author : ");
	fgets(n->author, 250, stdin);
	//n->author[strlen(auther) - 1] = '\0';
	//v1 = n->author;
	//v1[strlen(v1) - 1] = 0;
	*(n->author + strlen(n->author) - 1) = '\0';

	printf("Title : ");
	fgets(n->title, 250, stdin);
	*(n->title + strlen(n->title) - 1) = '\0';

	printf("Content : ");
	fgets(src, 8000, stdin);

	v3 = strlen(src);
	n->content = (char*)malloc(v3);

	v2 = strlen(src);
	memcpy(n->content, src, v2);
	*(n->content + strlen(src) - 1) = '\0';

	return 0;
}

int read_reply(Node *n)
{
	struct Reply *reply; //void *v3  //ebp - 0x14
	char* reply_content; //void *s   //ebp - 0x10
	struct Reply *tmp;

	reply = (Reply *)malloc(28);
	reply_content = (char*)malloc(120);
	//v2 = 0;

	reply->parent_comment = n->number;
	reply->status = 0xdeebface;
	reply->next = 0;

	if (n->replies)
	{
		//jne 0x8048eff
		for (tmp = n->replies; tmp->next; tmp = tmp->next);
			tmp->next = reply;

		printf("\t\tReply : ");
		getchar();
		fgets(reply_content, 100, stdin);
		*(reply_content + strlen(reply_content) - 1) = '\0';
		//	*a = (char*)(*a + 1);  not in Aida
	}
	else
	{
		n->replies = reply;
		memcpy(reply_content, "Welcome, It's Auto reply system", 100);
	}

	reply->reply_content = reply_content;
	n->reply_count++;

	return 0;
}

void init_free(Node* ptr)
{
	Reply *r;
	int v3; //ebp - 0x10

	r = ptr->replies;

	for (v3 = 0; v3 <= 1; v3++)
	{
		if (r->del_rep_func != free_sha)
		{
			puts("Detected");
			exit(1);
		}

		r = r->next;
	}

	while (r->next)
	{
		//jne 0x80489b4
		(r->del_rep_func)(r->reply_content);
		//[esp] = *(v4 + 3)
		// call eax
		r = r->next;
	}

	free(ptr->author);
	free(ptr->title);
	free(ptr->content);
	free(ptr);
}

int read_in(Node *n)
{
	d_input = 0;
	while (1)
	{
		if (d_input == 4)
			return 0;
		printf("\n\t\t1. delete 2. modify 3. reply 4. back\n\t\t=> ");
		scanf("%d", &d_input);

		if (d_input == 2)
			read_modify(n);
		else
		{
			if (d_input > 2)
			{
				if (d_input == 3)
					read_reply(n);
				else
				{
					if (d_input == 4)
						system("/usr/bin/clear");
					else
					jump:
					puts("Wrong");
				}
			}
			else
			{
				if (d_input != 1)
					goto jump;
				read_delete(n);
			}
		}
	}
}

int read_delete(Node* n)
{
	struct Reply* tmp;

	if (n->reply_count <= 0)
	{
		n->prev->next = n->next;
		n->next->prev = n->prev;
		if (n->status == 0xdeadbeef)
		{
			for (tmp = n->replies; tmp->next; tmp = tmp->next)
			{
				tmp->func1 = delete_set;
				tmp->del_rep_func = free_sha;
			}
		}

		n->del_func(n);
	}
	else
	{
		puts("Cannot Deleted. There's at least one or more replies on it");
	}

	return 0;
}

int delete_set(Reply* r)
{
	r->unknown = 0;
	r->status = 0xbabeface;

	return 0;
}

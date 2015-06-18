#include <stdio.h>
#include <stdlib.h>

typedef struct s4
{
	int i4;
	struct  s4 *p4;
}s4;

int list_init(struct s4 *list_ptr)
{
	list_ptr->p4 = 0;
	list_ptr->i4 = 0;
	return 0;
}
int list_addtotail(struct s4 *list_ptr, int data)
{
	if (list_ptr->p4 != NULL)
	{

		list_ptr->p4->p4 = (struct s4*)malloc(8);
		list_ptr->p4->p4->i4 = data;
		list_ptr->p4->p4->p4 = 0;
		list_ptr->p4 = list_ptr->p4->p4;

	}
	else {
		list_ptr->p4 = (struct s4*)malloc(8);
		list_ptr->p4->i4 = data;
		list_ptr->p4->p4 = 0;
		list_ptr->i4 = (int)list_ptr->p4;
	}
	return 0;
}
int list_printall(struct s4 *list_ptr)
{
	s4* v4 = (s4*)list_ptr->i4;
	while (v4 != 0)
	{
		printf("%d\n", v4->i4);
		v4 = v4->p4;
	}
	return 0;
}

int main()
{
	s4 v8;
	list_init(&v8);
	list_addtotail(&v8, 1);
	list_addtotail(&v8, 2);
	list_addtotail(&v8, 3);
	list_printall(&v8);
	return 0;
}











//
//#include <stdio.h>
//#include <stdlib.h>
//
//typedef struct s4
//{
//	int i4;
//	struct  s4 *p4;
//}s4;
//
//int list_init(int *list_ptr)
//{
//	list_ptr[1] = 0;
//	list_ptr[0] = list_ptr[1];
//	return 0;
//}
//int list_addtotail(int *list_ptr, int data)
//{
//	if (list_ptr[1] != 0)
//	{
//		 list_ptr[1] = (int)malloc(8);
//		 list_ptr[1] = data;
//
//		 list_ptr[1][1] = 0;
//
//		 list_ptr[0] = list_ptr[1];
//	}
//	else {
//		 list_ptr[1][1] = (int)malloc(8);
//		 list_ptr[1][1][0] = data;
//		 list_ptr[1][1][1] = 0;
//		 list_ptr[1] = list_ptr[1][1];
//	}
//	return 0;
//}
//int list_printall(int *v4)
//{
//	while (v4 != 0)
//	{
//		 printf("%d\n", v4[0]);
//		 var_4[0] = var_4[1];
//	}
//	return 0;
//}
//
//int main()
//{
//	struct s4 v8;
//	list_init(v8);
//	list_addtotail(v8, 1);
//	list_addtotail(v8, 2);
//	list_addtotail(v8, 3);
//	list_printall(v8);
//	return 0;
//}
#include <stdio.h>
#include <stdlib.h>

struct MyStruct
{
	int v8;
	int v4;
};

int _ZN5Adder3AddEv(struct MyStruct* s)
{
	if (s->v8 != -1 && s->v4 != -1)
	{
			return s->v8 + s->v4;		
	}
	return -1;
}

int main()
{
	struct MyStruct s;
	s.v8 = 0x63;
	s.v4 = 0x50;
	_ZN5Adder3AddEv(&s);
	printf("%d\n", &s);
	return 0;
}
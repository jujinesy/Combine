#include<stdio.h>

struct flags{
	unsigned int is_a : 2;
	unsigned int is_b : 2;
	unsigned int is_c : 32;
	unsigned int is_d : 2;
};

int main()
{
	struct flags f={0,};
	struct flags* ff;
	ff= &f;
	f.is_a = f.is_b = 7;
	f.is_d = f.is_c = 5;
	f.is_c = f.is_a & f.is_d;

	

	printf("%d %d", sizeof(f), f.is_c);

	return 0;
}
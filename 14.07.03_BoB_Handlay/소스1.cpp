#include <stdio.h>
#include <stdlib.h>

int main()
{
	int a1=2, a2=22;
	signed int result; // eax@2
	int v3; // eax@3
	unsigned __int16 v4; // [sp+0h] [bp-14h]@6
	char v5; // [sp+2h] [bp-12h]@6
	int v6; // [sp+3h] [bp-11h]@6
	int v7; // [sp+7h] [bp-Dh]@6
	char v8; // [sp+Bh] [bp-9h]@6
	unsigned int v9; // [sp+Ch] [bp-8h]@6
	unsigned int v10; // [sp+10h] [bp-4h]@1
	int v11; // [sp+14h] [bp+0h]@1

	v10 = (unsigned int)&v11;// ^ __security_cookie;
	if (a1 == 2)
	{
		v3 = atoi(*(const char **)(a2 + 4));
		if ((unsigned int)(v3 - 1) > 0x3E6)
		{
			printf("Closer, try again\n");
			result = 1;
		}
		else
		{
			if (!(v3 % 47))
			{
				if (!(v3 % 15))
				{
					v4 = 0x3062u;
					v9 = 0x21316Cu;
					v6 = 1601399135;
					v7 = 1601450094;
					v8 = 52;
					v5 = 98;
					printf("Great job!\n");
					printf("The key = %s\n", &v4);
				}
			}
			result = 0;
		}
	}
	else
	{
		printf("Oops, try again\n");
		result = 1;
	}
	return 0;
}
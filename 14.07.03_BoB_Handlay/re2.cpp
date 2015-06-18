#include <stdio.h>
#include <stdlib.h>

int main()
{
	// 	int v13= 0x32415544;
	// 	int vf = 0x90189457;
	char a[10] = { 0x44, 0x55, 0x41, 0x32, 0x57, 0x94, 0x18, 0x90, 0x10, 0x12 };
	int v4 = 0;
	int v8 = 0;
	char v9 = 0;

	while (v4 <= 9)
	{
		v8 = v4;
		while (v8 <= 9)
		{
			if (a[v4] <= a[v8])
			{
				v9 = a[v4];
				a[v4] = a[v8];
				a[v8] = v9;
			}
			v8++;
		}
		v4++;
	}


	v4 = 0;
	while (v4 <= 9)
	{
		printf("%02x\n", a[v4]);
		v4++;
	}
	return 0;
}
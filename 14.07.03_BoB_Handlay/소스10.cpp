#include <stdio.h>
#include <intrin.h>
#include <Windows.h>

int main(void)
{
	unsigned __int64 Regfs = 0;
	HMODULE base;

	Regfs = __readgsqword(0x60);
	//Regfs = *(unsigned __int64 *)Regfs;
	Regfs = *(unsigned long long *)(Regfs + 0x10);
	printf("%I64x\n", Regfs);

	base = GetModuleHandleA(NULL);
	printf("%I64x\n", base);
	return 0;
}
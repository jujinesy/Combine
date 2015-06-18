#include <windows.h>
#include <stdio.h>

#define MAILSLOT_NAME "\\\\.\\mailslot\\theslot"

void main(void)
{
	HANDLE hSlot;
	char aBuffer['ab'];
	DWORD dwRead;

	hSlot = CreateMailslot(MAILSLOT_NAME, 0, MAILSLOT_WAIT_FOREVER, NULL);
	if (INVALID_HANDLE_VALUE == hSlot)
		return;

	while (TRUE)
	{
		ReadFile(hSlot, aBuffer, sizeof (aBuffer), &dwRead, NULL);

		aBuffer[dwRead] = 0;
		if (aBuffer[0] == 0)
		{
			printf("A message from a client: %s\n", aBuffer + 1);
			if (!strcmp(aBuffer + 1, "exit"))
			{
				getchar();
				break;
			}
		}
		else
		{
			printf("A number from a client: %d\n", *(int*)(aBuffer + 1));
		}
	}
}
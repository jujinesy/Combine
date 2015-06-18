#include <windows.h>
#include <stdio.h>

#define MAILSLOT_FORMAT "\\\\%s\\mailslot\\theslot"

void main(void)
{
	HANDLE hSlot;
	char aBuffer[50];
	char aMailslotName[100];
	DWORD dwWritten;

	printf("Mailslot name: ");
	scanf("%s", aBuffer);
	sprintf(aMailslotName, MAILSLOT_FORMAT, aBuffer);
	hSlot = CreateFile(aMailslotName, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hSlot)
		return;

	while (TRUE)
	{
		printf("Your command: ");
		scanf("%s", aBuffer);

		WriteFile(hSlot, aBuffer, strlen(aBuffer) + 1, &dwWritten, NULL);

		if (!strcmp(aBuffer, "exit"))
			break;
	}
}

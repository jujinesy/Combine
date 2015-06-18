#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main() {
	char p[50] = "TEST_BOB_REVERSING_!!!";
	int len = strlen(p);
	for (int i = 0; i < len; i++) {
		p[i] = p[i] ^ 0x44;
	}
	for (int i = 0; i < len; i++) {
		p[i] = p[i] ^ 0x11;
	}
	printf("%x", p);
	getchar();
}
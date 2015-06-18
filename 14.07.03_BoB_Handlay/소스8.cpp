#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct userinfo {
	int id;
	int age;
	char *name;
} USER;

int main(int argc, char *argv[]) {
	FILE *fp;
	USER *u;
	//struct GAME *p;
	char buf[256] = { 0, };
	//int tmp;
	fp = fopen("./data.txt", "r");
	if (!fp) {
		printf("file stream open fail\n");
		exit(0);
	}
	fgets(buf, 200, fp);
	fclose(fp);

	u = (USER*)malloc(sizeof(USER));
	if (!u) {
		printf("malloc fail\n");
		exit(0);
	}
	memset(u, 0x0, sizeof(USER));

	u->name = (char*)malloc(50);
	if (!u->name) {
		printf("malloc fail\n");
		exit(0);
	}
	memset(u->name, 0x0, 50);

	sscanf(buf, "%d %d %s", &u->id, &u->age, (char *)&u->name);

	printf("=======================================\n");
	printf("ID  : %d\n", u->id);
	printf("AGE : %d\n", u->age);
	printf("NAME: %s\n", (char *)&u->name);
	printf("=======================================\n");

	return 0;
}
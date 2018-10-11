#include <stdio.h>

int main() {
	FILE * fp;
	char buffer[100];
	fp = fopen("text.txt", "r");
	if(fp) {
		while(fgets(buffer, 100, (FILE *)fp))
			printf("%s", buffer);
	}
	fclose(fp);

	int ret = system("mkdir testdir");
	if(!ret)
		printf("command Succes\n");
	else
		printf("command Failed\n");
	return 0;
}

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
int main(){
	char * str = malloc(sizeof(char)*6);
	strcpy(str, "boas");
	printf("str = %s, strlen = %d. str[%d]= %c", str, strlen(str), str[strlen(str)]);
	free(str);
	printf("write this: ");
	fprintf(stdin, "endtheworld");
	scanf("%s", str);
	printf("%s\n",str);
	return 0;
}

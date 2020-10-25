#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	printf("Start program\n");
	printf("Allocate 1024 byte memory\n");
	char* text = malloc(1024);
	if (text == NULL)
	{
		fprintf(stderr, "Can't allocate memory\n");
		return 1;
	}
	printf("Set all bytes to 0xCC value\n");
	memset(text, 0xCC, 1024);
	printf("Free allocated memory\n");
	free(text);
	printf("Second free allocated memory\n");
	free(text);	
}

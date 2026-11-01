#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h> 

#include "tinyshell.h"

int main(int argc, char **argv)
{
	if (argc > 1){
		printf("argc: %d\n", argc);
		for (int i = 1; i < argc; i++){
			printf("%s ", argv[i]);
		}
		puts("");
	}
	
	while (1){
		char inbuf[256];
		pshell();

		char* res = fgets(inbuf, sizeof(inbuf), stdin);
		if (res == NULL){
			printf("Error: %d\n", errno);
			perror("fgets");
			exit(EXIT_FAILURE);
		}
		
		process(inbuf);
	}
	return 0;
}

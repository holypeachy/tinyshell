#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

typedef enum {
	CMD_ERR,
	CMD_PWD,
	CMD_CD,
	CMD_EX
} Cmd;

Cmd parse(char*);
void pwd(char*, size_t);
void exec();

char* argv[64];
size_t argc = 0;

void pshell(){
	char buf[256];
	pwd(buf, sizeof(buf));
	printf("tinyshell | %s > ", buf);
}

void process(char* string){
	argc = 0;
	if (string != NULL && strcmp(string, "\n") != 0){
		Cmd cmd = parse(string);
		if (cmd == CMD_PWD){
			char buf[256];
			pwd(buf, sizeof(buf));
			puts(buf);
		} else if (cmd == CMD_CD){
			chdir(argv[1]);
		} else{
			exec();
		}
	}
}

Cmd parse(char* string){
	char* tok = strtok(string, " \t\n");
	argc++;
	argv[0] = tok;
	
	if (strcmp(tok, "pwd") == 0) return CMD_PWD;
	else if (strcmp(tok, "cd") == 0){
		tok = strtok(NULL, " \t\n");
		argc++;
		argv[1] = tok;
		return CMD_CD;
	}
	else if (strcmp(tok, "exit") == 0){
		exit(0);
	} else{
		tok = strtok(NULL, " \t\n");
		while (tok != NULL && argc < 63){
			argc++;
			argv[argc - 1] = tok;
			tok = strtok(NULL, " \t\n");
		}
		argv[argc] = NULL;
		return CMD_EX;
	}

	return CMD_ERR;
}

void pwd(char* buf, size_t size){
	char* res = getcwd(buf, size);
	if (res == NULL){
		printf("Error: %d\n", errno);
		perror("getcwd");
		exit(EXIT_FAILURE);
	}
}

void exec(){
	pid_t pid = fork();
	
	if (pid == -1){
		perror("fork");
		return;
	}
	if (pid == 0){
		execvp(argv[0], argv);
		perror("tinyshell");
		_exit(127);
	}
	if (waitpid(pid, NULL, 0) == -1){
		perror("waitpid");
	}
}

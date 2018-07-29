#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE_LENGTH      1000
#define INPUT_REDIRECTION     ">"
#define INPUT_REDIRECTION2     "2>"
#define OUTPUT_REDIRECTION   "<"
#define PIPE                "|"
#define BACKGROUND           '&'
#define SPACE                " \t\r\n"

int prompt(char*);
int parse(char*, char**, const char*);
int check_background(char*, int);
void redirection(char**, int, int, int);
void inner_excute(char*);
void execute(char*, int);


int main(int argc, char** argv) {
	char line[MAX_LINE_LENGTH];
	int line_lenth;
	int i;

	if (argc>1) {//입력값 있을때

		strcpy(line, argv[1]);
		line_lenth = strlen(line) - 1;
		execute(line, line_lenth);

	}
	else {//입력값 없을때

		while (1) {
			line_lenth = prompt(line);
			execute(line, line_lenth);
		}

	}
	return 0;
}




//쉘 프롬프트 
int prompt(char* line) {

	int n;

	printf("[%s]$ ", get_current_dir_name());
	if (fgets(line, MAX_LINE_LENGTH, stdin) == NULL) {
		printf("\n");
		exit(0);
	}

	n = strlen(line) - 1;

	return n;
}


//백그라운드 확인
int check_background(char* argv, int argc)
{
	if (argv[argc - 1] == BACKGROUND)
		return 1;
	else
		return 0;
}
//파싱
int parse(char* line, char** argv, const char* slice)
{
	int n = 0;
	char*temp = strtok(line, slice);

	if (temp == NULL) return 0;

	while (temp != NULL) {
		argv[n++] = temp;
		temp = strtok(NULL, slice);
	}
	argv[n] = '\0';
	return n;
}
//리다이렉션
void redirection(char** params, int fd, int i, int flag) {

	if ((fd = open(params[i + 1], flag, 0644)) == -1)
		perror("open");
	if (close(STDOUT_FILENO) == -1)
		perror("close");
	if (dup2(fd, STDOUT_FILENO) == -1)
		perror("dup2");
	if (close(fd) == -1)
		perror("close");
}
//파이프
void execute(char* argv, int argc) {
	int pid, i;
	int status;
	int fd[2];
	char* arguments[MAX_LINE_LENGTH];

	int bg = check_background(argv, argc);//백그라운드 확인
	int num = parse(argv, arguments, PIPE);//파이프 파싱

	if ((pid = fork()) == -1)
		perror("fork");

	else if (pid == 0) {
		for (i = 0; i < num - 1; i++) {
			pipe(fd);//pipe
			if (fork() == 0) {
				close(fd[0]);
				dup2(fd[1], STDOUT_FILENO);
				inner_excute(arguments[i]);
			}
			else {
				close(fd[1]);
				dup2(fd[0], STDIN_FILENO);
			}
		}
		inner_excute(arguments[i]);

	}
	else {
		if (bg == 0)//백그라운드&
			if ((pid = waitpid(pid, &status, 0)) == -1)//wait
				perror("waitpid");
	}
}

//명령문, 리다이렉션 실행
void inner_excute(char* argv) {

	char* params[MAX_LINE_LENGTH];
	int argc = parse(argv, params, SPACE);
	int fd = -1;
	int flag = 0;
	int i;

	for (i = 0; i < argc; i++) {
		//리다이렉션
		if (strcmp(params[i], INPUT_REDIRECTION) == 0) {// >

			flag = O_WRONLY | O_CREAT | O_TRUNC;
			redirection(params, fd, i, flag);
			params[i] = '\0';
			i++;
		}

		if (strcmp(params[i], OUTPUT_REDIRECTION) == 0) {// <

			flag = O_WRONLY | O_CREAT;
			redirection(params, fd, i, flag);
			params[i] = '\0';
			i++;
		}
		if (strcmp(params[i], INPUT_REDIRECTION2) == 0) {// 2>

			flag = O_WRONLY | O_CREAT | O_TRUNC;
			redirection(params, fd, i, flag);
			params[i] = '\0';
			i++;
		}

	}

	if (execvp(params[0], params) == -1)//명령어 실행
		perror("execvp");
	exit(0);
}


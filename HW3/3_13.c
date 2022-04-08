#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
	if (argc != 2) {
           printf("Please input a valid number!\n");
           exit(EXIT_FAILURE);
   	}


	int sequence_num = atoi(argv[1]);
	if (sequence_num < 0) {
		printf("Please input a valid number!\n");
           	exit(EXIT_FAILURE);
	}

	pid_t pid;
	pid = fork();

//	printf("s1 = %s\n", argv[1]);
//	printf("s2 = %d\n", sequence_num);

	if (pid < 0) {
		fprintf(stderr, "Fork Failed");
		return 1;
	}
	else if (pid == 0) {	// child process
		int s1 = 0, s2 = 1, now;
		if (sequence_num == 0) {
			printf(" ");
		} else if (sequence_num == 1) {
			printf("0");
		}
		else {
			printf("0, 1");
			for (int i = 2; i < sequence_num; i++) {
				now = s1 + s2;
				s1 = s2;
				s2 = now;
				printf(", %d", now);		
			}
		}
		printf("\n");

//		printf("Child process print!\n");
//		printf("Child's pid = %d\n", pid);
	}
	else {	// parent process
//		pid_t parent = getpid();
//		printf("Parent process print!\n");
//		printf("Parent's pid = %d\n", parent);
		wait(NULL);
//		printf("wait...\n");
	}

	return 0;

}

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/mman.h>
# include <sys/types.h>
# include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

# define MAX_SEQUENCE 10

typedef struct {
    long fib_sequence[MAX_SEQUENCE];
    int sequence_size;
} shared_data;

int main(int argc, char *argv[]){
    if (argc < 2){
	fprintf(stderr, "Please enter a number for Fibonacci sequence.\n");
	exit(EXIT_FAILURE);
    }

    int num, i;
    pid_t pid;
    num = atoi(argv[1]);

    if (num > MAX_SEQUENCE){
	fprintf(stderr,	"Please enter a number less than %d.\n",MAX_SEQUENCE);
	exit(-1);
    }
    else if (num < 0){
	fprintf(stderr, "Please enter a positive number for Fibonacci sequence.\n");
	exit(-1);
    }

    char *shm_name = "/share_memory";
    int shmfd = shm_open(shm_name, O_RDWR | O_CREAT, S_IRWXU);
    ftruncate(shmfd, sizeof(shared_data));
    shared_data *shm_addr;
    shm_addr = (shared_data *)mmap(NULL,
				   sizeof(shared_data),
				   PROT_READ | PROT_WRITE,
				   MAP_SHARED,
				   shmfd,
				   0);
    shm_addr -> sequence_size = num;

    pid = fork();

    if (pid < 0){
	fprintf(stderr, "Fork process failed.\n");
	exit(-1);
    }
    else if (pid == 0){
//	printf("\n###############\n");
//	printf("this is child process\n");
	shm_addr -> fib_sequence[0] = 0;
	shm_addr -> fib_sequence[1] = 1;
	for (i = 2; i < shm_addr -> sequence_size; i++){
	    shm_addr -> fib_sequence[i] = shm_addr -> fib_sequence[i - 1] + shm_addr -> fib_sequence[i - 2];
	}
    }
    else{
	wait(NULL);
//	printf("\n###############\n");
//	printf("this is parent process\n");
	if (shm_addr -> sequence_size == 0) {
		printf(" ");
	} else if (shm_addr -> sequence_size == 1) {
		printf("0");
	}
	else {
		printf("0, 1");
		for (i = 2; i < shm_addr -> sequence_size; i++){
			printf(", %ld", shm_addr -> fib_sequence[i]);
		}
	}
	printf("\n");
    }

    shm_unlink(shm_name);


    return 0;
}

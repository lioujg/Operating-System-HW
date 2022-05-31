# include <stdio.h>
# include <stdlib.h>
# include <sys/shm.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/stat.h>

typedef struct{
    int n;
    int arr[100];
} shared_data;

unsigned long long fact(unsigned long long n) {
    if (n == 0) return 1;
    else if (n == 1) return 1;
    else return n * fact(n-1);
}

int main(int argc, char* argv[])
{
	if (argc != 2){
		fprintf(stderr, "Usage:./Catalan {Catalan_numbers}\n");
		exit(1);
    }

    int size = atoi(argv[1]);
    if (size < 0) {
		fprintf(stderr, "Wrong input number\n");
		exit(1);
    } else if (size >= 11) {
    	fprintf(stderr, "I can't afford...\n");
		exit(1);
    }

    int segment_id = shmget(IPC_PRIVATE, sizeof(shared_data), S_IRUSR | S_IWUSR);
    if (segment_id == -1){
		fprintf(stderr, "Shmget error\n");
		exit(1);
    }

    void* shmaddr = shmat(segment_id, NULL, 0);
    if (shmaddr == (void*)-1){
		fprintf(stderr, "Shmat error\n");
		exit(1);
    }

	pid_t pid;
	pid = fork();

	if (pid < 0) {
		fprintf(stderr, "Fork process failed.\n");
		exit(-1);
	} else if (pid == 0) {
		wait(NULL);
		// printf("this is child process\n");
		shared_data* data_ptr = (shared_data*) shmaddr;
		for (int i = 0; i < data_ptr->n; i++) {
			printf("%d ",data_ptr->arr[i]);
		}
		printf("\n");
		/* remove the shared memory object */
		shmctl(segment_id, IPC_RMID, NULL);
	} else {
		// printf("this is parent process\n");
		/* write to the shared memory object */
		shared_data data;
	    data.n = size;
	    for (int i = 1; i <= size; i++) {
			data.arr[i-1] = fact(2 * i) / fact(i + 1) / fact(i);
	    }
		memset(shmaddr, 0, sizeof(shared_data));
	    memcpy(shmaddr, &data, sizeof(shared_data));

	    if (shmdt(shmaddr) == -1){
			fprintf(stderr, "Shmdt error\n");
			exit(1);
	    }
	}

	return 0;
}

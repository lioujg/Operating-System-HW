# include <stdio.h>
# include <stdlib.h>
# include <sys/shm.h>
# include <unistd.h>
# include <string.h>

int fact(int n){
    if (n == 0) return 1;
    else if (n == 1) return 1;
    else return n * fact(n-1);
}

typedef struct{
    int n;
    int cn[1024];
} shared_data;

int main(int argc, char* argv[]){
    if (argc != 2){
	fprintf(stderr, "usage:./producer {Catalan_numbers}\n");
	exit(1);
    }

    int n = atoi(argv[1]);
    if (n < 0){
	fprintf(stderr, "wrong input number\n");
	exit(1);
    }
    shared_data data;
    data.n = n;

    for (int i = 1; i <= n; i++){
	int c = fact(2 * i) / fact(i + 1) / fact(i);
	data.cn[i-1] = c;
    }

    void* shmaddr = NULL;
    int shmid;

    shmid = shmget(666, sizeof(data), 0644 | IPC_CREAT);
    if (shmid == -1){
	fprintf(stderr, "shmget error\n");
	exit(1);
    }

    shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (void*)-1){
	fprintf(stderr, "shmat error\n");
	exit(1);
    }

    memset(shmaddr, 0, sizeof(data));
    memcpy(shmaddr, &data, sizeof(data));

    if (shmdt(shmaddr) == -1){
	fprintf(stderr, "shmdt error\n");
	exit(1);
    }

    printf("write done\n");

    return 0;
}

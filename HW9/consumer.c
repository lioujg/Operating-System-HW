# include <stdio.h>
# include <stdlib.h>
# include <sys/shm.h>
# include <unistd.h>

typedef struct{
    int n;
    int cn[1024];
} shared_data;

int main(int argc, char* argv[]){
    shared_data* data;
    void* shmaddr;
    int shmid;

    shmid = shmget(666, sizeof(shared_data), 0644 | IPC_CREAT);
    if (shmid == -1){
	fprintf(stderr, "shmget error\n");
	exit(1);
    }

    shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (void*)-1){
	fprintf(stderr, "shmat error\n");
	exit(1);
    }

    data = (shared_data*) shmaddr;
    int n = data -> n;
    for (int i = 0; i < n; i++){
	printf("%d\n", data->cn[i]);
    }

    if (shmdt(shmaddr) == -1){
	fprintf(stderr, "shmdt error\n");
	exit(1);
    }

    if (shmctl(shmid, IPC_RMID, 0) == -1){
	fprintf(stderr, "shmctl error\n");
	exit(1);
    }

    printf("received done\n");

    return 0;
}

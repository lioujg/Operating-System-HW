#include <stdio.h>
#include <stdlib.h>
# define PAGE_SIZE 4096

int main(int argc, char *argv[]){
	unsigned int addr = atoi(argv[1]);
	unsigned int offset = addr % PAGE_SIZE;
	unsigned int page = addr / PAGE_SIZE;
	//printf("The address %d contains:\n", addr);
	//printf("page number = %d\n", page);
	//printf("offset = %d\n", offset);
	printf("%d %d\n", page, offset);


	return 0;


}

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#define MAX_CUS_NUM 50

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// 子執行緒函數
void* child(void* data) {
	int *num = (int*) data; // 取得輸入資料
	pthread_mutex_lock(&mutex);
	printf("Customer %d is cutting hair\n", *num + 1); // 每秒輸出文字
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL); // 離開子執行緒
}

// 主程式
int main(int argc, char *argv[]) {
	int cus_num = atoi(argv[1]);
	int chair_num = atoi(argv[2]);
	if (cus_num > 50) {
		printf("Usage: <CUSTOMER_NUM> no more than 50\n");
		return 0;
	} else if (cus_num == 0) {
		printf("No customer today, end the program!\n");
		return 0;
	} else if (chair_num == 0) {
		printf("Barbershop not available today, end the program!\n");
		return 0;
	} else if (chair_num > 30) {
		printf("Usage: <CHAIR_NUM> no more than 30\n");
		return 0;
	}

	int cus_n[MAX_CUS_NUM] = {};
	pthread_t t[MAX_CUS_NUM]; // 宣告 pthread 變數
	for (int i = 0; i < cus_num; i++) {
		cus_n[i] = i;
		pthread_create(&t[i], NULL, child, (void*) &cus_n[i]); // 建立子執行緒
	}

  // 主執行緒工作

	for (int i = 0; i < cus_num; i++) {
		pthread_join(t[i], NULL); // 等待子執行緒執行完成
	}
	return 0;
}

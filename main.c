#include <semaphore.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 5
/*
  semaphore
 */
int play = 1;
sem_t empty;
sem_t full;
pthread_mutex_t mutex;

/*
  buffer
 */
typedef int buffer_item;
int buffer_top = -1;
buffer_item buffer[BUFFER_SIZE];
int insert_item(buffer_item item){
		if(buffer_top<BUFFER_SIZE-1){
				buffer_top+=1;
				buffer[buffer_top] = item;				
				return 0;
			}
		else
			return -1;
	}
int remove_item(buffer_item *item){
		if(buffer_top>-1){
				*item = buffer[buffer_top];
				buffer_top -= 1;
				return 0;
			}
		else 
			return -1;
	}
int trans_char_int(char words[]){
	/*
	 turn char into int;
	 */
		int length = strlen(words),ans = 0,i;
		for(i=0;i<length;i++){
				ans = ans*10 + words[i] - '0';
			}
		return ans;
	}
void producer(void *arg){
		int sleep_time;
		buffer_item item;
		
		while(1){
				sleep_time = rand()%2;
				sleep(sleep_time);
				item = rand()%1000;
				sem_wait(&empty);
				pthread_mutex_lock(&mutex);
				if(insert_item(item))
					printf("error condition");
				else
					printf("producer produced %d in position %d\n",item,buffer_top+1);
				pthread_mutex_unlock(&mutex);
				sem_post(&full);
				if(play == 0){
						return;
					}
			}
	}
void consumer(void *arg){
		int sleep_time;
		buffer_item item;
		while(1){
				sleep_time = rand()%2;
				sleep(sleep_time);
				item = rand();
				sem_wait(&full);
				pthread_mutex_lock(&mutex);
				if(remove_item(&item))
					printf("error condition");
				else
					printf("consumer consumed %d in position %d\n",item,buffer_top+2);
				pthread_mutex_unlock(&mutex);
				sem_post(&empty);
				if(play == 0){
						return;
					}
			}
			
	}
int main(int argc,char *argv[]){
		int slp,pro,con,i;
		pthread_t tids[1000];
		sem_init(&empty,0,5);
		sem_init(&full,0,0);
		pthread_mutex_init(&mutex,NULL);
		if(argc <3){
				printf("You miss some args,sorry.\n");
				return 0;
			}
		slp = trans_char_int(argv[1]);
		pro = trans_char_int(argv[2]);
		con = trans_char_int(argv[3]);
		//pthread_create(&tids[0],NULL,producer,NULL);
		for(i=1;i<=pro;i++){
				pthread_create(&tids[i],NULL,producer,NULL);
			}
		for(i=1;i<=con;i++){
				pthread_create(&tids[pro+i],NULL,consumer,NULL);
			}		
		sleep(slp);
		play = 0;
		for(i=1;i<=pro+con;i++){
				pthread_join(tids[i],NULL);
			}
		return 0;
	}

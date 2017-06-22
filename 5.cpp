#include<semaphore.h>
#include<pthread.h>
#include<errno.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#define NBUFF 10

int  nitems;
int  buff[NBUFF];
sem_t mutex,nempty,nstored;


void *produce(void *arg){
	int i,j = 0;

	for(i = 1; i <= nitems; i++){
		sem_wait(&nempty);
		sem_wait(&mutex);
		 
		while(buff[j % NBUFF] != 0)\
			j++;
	    buff[j % NBUFF] = i; 
		printf("produce buff[%d] = %d\n",j % NBUFF,buff[j % NBUFF]);

		sem_post(&mutex);
		sem_post(&nstored);
		sleep(1);
	}
	
	return (NULL);
}
 
void *consume(void *arg){
	int i,j = 0;
  
	for(i = 0; i < nitems; i++)
	{
		sem_wait(&nstored);
		sem_wait(&mutex);
	 
		/* 可以写任何的事件 */
		while(buff[j % NBUFF] == 0) j++;	/* 从当前位置寻找存储着数据的单元 */
		printf("consume buff[%d] = %d\n",j % NBUFF,buff[j % NBUFF]);
		buff[j % NBUFF] = 0;
     
		sem_post(&mutex);
		sem_post(&nempty);
     
	  sleep(4);
	}
	return (NULL);
   
 }

int main(int argc,char **argv){
	pthread_t  tid_produce,tid_consume;
 
	if(argc != 2)
		perror("usage: prodcons2<#items>");
	nitems = atoi(argv[1]);	
	
	sem_init(&mutex, 0, 1);
	sem_init(&nempty, 0, NBUFF);
	sem_init(&nstored, 0, 0);
  
	if(pthread_create(&tid_produce,NULL,produce,NULL)){
		perror("pthread_create: tid_produce");
		exit(EXIT_FAILURE);
	}
 
	if(pthread_create(&tid_consume,NULL,consume,NULL)){
		perror("pthread_create: tid_consume");
		exit(EXIT_FAILURE);
	}

	pthread_join(tid_produce,NULL);
	pthread_join(tid_consume,NULL);
 
	sem_destroy(&mutex);
	sem_destroy(&nempty);
	sem_destroy(&nstored);
 
	return 0;
}

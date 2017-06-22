#include<sys/types.h>
#include<pthread.h>	
#include<unistd.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#define NBUFF 10

int  nitems;
int  buff[NBUFF];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *produce(void *arg)
{	  
	int i,j = 0;
	 
	for(i = 1;i <= nitems; i++)
	{
		if(pthread_mutex_lock(&mutex) != 0){	/* ¼ÓËø */
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}  
		while(j < NBUFF && buff[j % NBUFF] != 0) j++;
	    if(j < NBUFF){
			buff[j] = i;
			printf("produce buff[%d] = %d\n", j, buff[j]);
			if(j == NBUFF-1) j = 0; 
		}
    
		if(pthread_mutex_unlock(&mutex) != 0){	/* ¿ªËø */
			perror("pthread_mutex_unlock");
			exit(EXIT_FAILURE);
		}
    
		sleep(1);
	}
	return (NULL);
}
 
void *consume(void *arg){
	int i,j = 0;
	
	for(i = 0;i < nitems; i++){ 
		if(pthread_mutex_trylock(&mutex) != 0){
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}
		
		while(j < NBUFF && buff[j % NBUFF] == 0)
			j++;
		
		if(buff[j % NBUFF] != 0 && j < NBUFF){
			printf("consume buff[%d] = %d\n", j, buff[j]);
			buff[j % NBUFF] = 0;
			if(j == NBUFF-1)j = 0;
		}
		
		if(pthread_mutex_unlock(&mutex) != 0){
			perror("pthread_mutex_unlock");
		    exit(EXIT_FAILURE);
		} 
		
		sleep(3);
	}
	return (NULL);
}

int main(int argc,char **argv){  
	pthread_t  tid_produce,tid_consume;
	
	if(argc != 2)
		perror("usage: prodcons2<#items>");
	nitems = atoi(argv[1]);
  	
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
 
	return 0;
}

#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>

int main(){
	pid_t child;
	int value,i,status;
	
	printf("Please input a value:");
	scanf("%d",&value);
	
	if((child=fork())==-1){
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if(child==0){
		for(i=1;i<6;++i){
			value++;
			printf("\tchild value%d\n",value);
//			sleep(1);
		}
		exit(EXIT_SUCCESS);
	}
	else
		for(i=1;i<6;++i){
			value--;
			printf("\tparent value=%d\n",value);
//			sleep(1);
		}
	waitpid(child,&status,0);
	exit(EXIT_SUCCESS);
	return 0;
}


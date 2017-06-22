#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<sys/shm.h>
#include<stdlib.h>
#define BUFSZ 1024

int main(){
	pid_t child;
    int  status,i,shmid,fd;
    char *shmadr;

    if((shmid = shmget(IPC_PRIVATE,BUFSZ,0666)) < 0) {
    	perror("shmget");
        exit(EXIT_FAILURE);
    }
	printf("segment created: %d\n",shmid);
	system("ipcs -m");
    
	if((shmadr = shmat(shmid,0,0)) < 0) {
    	perror("shmat");
    	exit(EXIT_FAILURE);
    }
	printf("segment attached at %p\n",shmadr);
//	system("ipcs -m");

    for(i = 0;i < 10; i++)
		shmadr[i] = i + '0';
    shmadr[i] = '\0';
      
    if((shmdt(shmadr)) < 0) {
        perror("shmadr");
    	exit(EXIT_FAILURE);
    }
	puts("segment detached");
//    system("ipcs -m");

    if((child = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if(child == 0) {
        puts("in child");
        if((shmadr = shmat(shmid,0,0)) < (char *)0) {
            perror("shmat");
            exit(EXIT_FAILURE);
        }
        printf("segment attached at %p\n",shmadr);
//        system("ipcs -m");        
		printf("The child process change share memory!\n");
        printf("The old content of share memory: %s\n",shmadr);

    	for(i = 0; i < 12; i++)
			shmadr[i] = i + 'a'; /* 修改共享内存区数据 */
        shmadr[i] = '\0';
		printf("The new content of share memory: %s\n",shmadr);

        if((shmdt(shmadr)) < 0) {
            perror("shmdt");
            exit(EXIT_FAILURE);
        }
        puts("segment detached");
//          system("ipcs -m");

        exit(EXIT_SUCCESS);
    } else {
        waitpid(child,&status,0);
        printf("Press any key to continue!\n");
        getchar();
        puts("in parent");
        if((shmadr = shmat(shmid,0,0)) < (char *)0) {
            perror("shmat");
            exit(EXIT_FAILURE);
        }
        printf("segmet attached at %p\n",shmadr);
//      system("ipcs -m");
        printf("The parent process read share memory!\n");    /* 父进程读共享内存区数据 */
        printf("The content of share memory: %s\n",shmadr);

        if((shmdt(shmadr)) < 0) {
            perror("shmdt");
            exit(EXIT_FAILURE);
        }
        puts("segment detached");
		system("ipcs -m");

    	if((shmctl(shmid,IPC_RMID,NULL)) < 0) {
        	perror("shmctl");
        	exit(EXIT_FAILURE);
        }
		puts("segment destroyed");
//        system("ipcs -m");
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_SUCCESS);
    
    return 0;
}	


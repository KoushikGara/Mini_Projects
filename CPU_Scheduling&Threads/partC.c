#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<time.h>
#include<sys/types.h>
#include<sys/wait.h>


int main(){
	printf("mutliproc started\n");
	pid_t pid1,pid2;
	
//	creating a child process
	pid1 = fork();
	
//	checking if child created properly
	if(pid1==-1){
		printf("Error in creating child 1\n");
		return 1;
	}
	
//	child process function	
	if(pid1==0){
//		running partA as exec and passing input arguments 
		static char *args1[] = {"./partA", "hundred.txt", "100", "3",NULL};
		printf("starting exec1 %d\n",getpid());
		execvp(args1[0],&args1[0]);
		printf("Error encountered in child process 1");
	}
//	parent process function
	else{
//		creating a child process
		pid2 = fork();
			
//		checking if child created properly
		if(pid2==-1){
			printf("Error in creating child 2\n");
			return 1;
		}
		
//		child process function	
		if(pid2==0){
//			running partA as exec and passing input arguments 
			static char *args2[] = {"./partB","100","3",NULL};
			printf("starting exec2\n");
			sleep(2);
			execvp(args2[0],args2);
			printf("Error encountered in child process 1");
		}
//		parent process function
		else{
			
//			Round Robin and priority comes here to be said by Janhvi Bahuguna
			
		}
	}

}


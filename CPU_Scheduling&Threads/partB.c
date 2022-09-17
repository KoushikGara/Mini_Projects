#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<sys/shm.h>
#include<pthread.h>


void read_numbers_ipc(int threadcount, int numcount);
void sum_threaded(void *params);

typedef struct{
	int threadnum;
	float partsize;
}readdata;

long int sum = 0;

int main(int argc, char *argv[]){

	printf("partB started\n");
	if(argc != 3){
		printf("Usage: ./partB numbercount threadcount");
		exit(-1);
	}	

	int threadcount = atoi(argv[2]);	
	int numcount = atoi(argv[1]);

/*
//	calculating and printing time taken with different number of threads
	clock_t t;
	int i;
	double time_taken;

	
	for(i=1; i<101; i++){
		sum = 0;
		t = clock();
		read_numbers_ipc(i,numcount);
		t = clock()-t;
		time_taken = ((double)t)/CLOCKS_PER_SEC;
		printf("%d %lf\n",i,time_taken);		
	}
*/

	read_numbers_ipc(threadcount,numcount);
	printf("Sum = %ld\n",sum);


}

/*
Function	:read_numbers_ipc
Description	:generates threads to read and sum the values stored in the shared memory by partA.c
Parameters	:the number of threads required, the number of integers in the input file of partA
Returns	:NONE
*/
void read_numbers_ipc(int threadcount, int numcount){
	int i,j,k;
	int shmid;
	float numbers = numcount;
	
//	Declaring required number of threads and parameters for passing
	pthread_t threads[threadcount];
	readdata passvalues[threadcount];
	
	for( i=0; i<threadcount; i++){
//		Parameter values
		passvalues[i].threadnum = i;
		passvalues[i].partsize = numbers/threadcount;
		
//		Creating threads 
		if(pthread_create(&threads[i], NULL, (void*)sum_threaded, (void*)&passvalues[i])){
			printf("Error in creating thread %d\n",i);
			exit(-1);	
			}
	}
	
//	Joining all the threads
	for( i=0; i<threadcount; i++){
		pthread_join(threads[i],NULL);
		}
}

/*
Function	:sum_threaded
Description	:individual thread program, reads and sums its assigned share of values from the shared memory
Parameters	: void casted pointer of readdata struct
Returns	:NONE
*/
void sum_threaded(void *params){
	int i,j,k,n;
	long int localsum =0;
	int shmid;
	void *shared_memory;

//	extracting information from the passed parameters
	readdata *mydata = (readdata*)params;
	int threadnum = mydata->threadnum;
	float partsize = mydata->partsize;
	
//	calculating shared memory reading interval of the thread
	float startval = threadnum * partsize;
	float endval = (threadnum+1) * partsize;
	
	int startvalenter = startval;
	int endvalenter = endval;
	
//	attaching pointer to the shared memory created in partA.c
	shmid = shmget((key_t)4567, sizeof(int)*1000000, 0666);
	shared_memory = shmat(shmid,NULL,0);
	
//	clock_t t;
//	while( ((int*)shared_memory)[i]==0 && (clock()-t)<2){
//		usleep(500);
//	}
		
//	calculating the sum of assigned values from shared memory
	for( i=startvalenter; i<endvalenter; i++){
		n = ((int*)shared_memory)[i];
		localsum = localsum + n;	
	}
//	printf("Thread %d sum is %ld\n",threadnum,localsum);

//	updating the global variable sum
	sum = sum + localsum;	
}



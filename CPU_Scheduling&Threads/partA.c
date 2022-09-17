#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<sys/shm.h>
#include<pthread.h>



#define MAXTHREAD 150


void read_threaded(void *params);
void read_numbers_from_file(int threadnum, char *ipfile);
void shared_memory_sender(int numberofthreads);


typedef struct{
	char *filename;
	int threadnum;
	float partsize;
}readdata;

int storage[MAXTHREAD][1000000];
int numcountperthread[MAXTHREAD];
int shmid;
void *shared_memory;
int numwrit;

int main(int argc, char *argv[]){
	
	if(argc != 4){
		printf("Usage: ./partA inputfile numbercount threadcount argc passed=%d",argc);
		exit(-1);
	}	
	
	printf("partA started %d\n",getpid());
	int numberofthreads = atoi(argv[3]);	
	int i,j,k=0;
	double time_taken;
	char *filename= argv[1];
	
//	allocating shared memory and attaching pointer to the shared memory	
	shmid = shmget((key_t)4567, sizeof(int)*1000000, 0666|IPC_CREAT);
	shared_memory = shmat(shmid,NULL,0);	
	
//	calculating and printing time taken with different number of threads	
//	clock_t t;
//	
//	for(i=1; i<101; i++){
//		t = clock();
//		read_numbers_from_file(i,filename);
//		t = clock()-t;
//		time_taken = ((double)t)/CLOCKS_PER_SEC;
//		printf("%d %f\n",i,time_taken);
//	}

	read_numbers_from_file(numberofthreads,filename);		
	shared_memory_sender(numberofthreads);
	printf("partA done\n");

/*
//	printing data stored in shared memory
	printf("Main printing\n");
	for(i=0;i<numberofthreads;i++){
		for(j=0;j<numcountperthread[i];j++){
			printf("%d\n",((int*)shared_memory)[k]);
			k++;
		}
	}
*/

}

/*
Function	:read_numbers_from_file
Description	:reads data from the input file, generates threads to read and store different parts of file
Parameters	:the number f threads required, the input file
Returns	:NONE
*/
void read_numbers_from_file(int threadcount, char *ipfile){
	int i;
	
//	Finding the size of input file
	FILE *fp;
	fp = fopen(ipfile,"r");	
	fseek(fp, 0, SEEK_END);
    long int size = ftell(fp);
	float fsize = size;
	fclose(fp);
	
//	Declaring required number of threads and parameters for passing
	pthread_t threads[threadcount];
	readdata passvalues[threadcount];
	
	for( i=0; i<threadcount; i++){
//		Parameter values
		passvalues[i].filename = ipfile;
		passvalues[i].partsize = fsize/threadcount;
		passvalues[i].threadnum = i;
		
//		Creating threads 
		if(pthread_create(&threads[i], NULL, (void*)read_threaded,(void *)&passvalues[i])){
			printf("Error in creating thread %d", i);
			exit(-1);
		}
	}
	
//	Joining all the threads
	for(i=0;i<threadcount; i++){
		pthread_join(threads[i],NULL);
	}
}


/*
Function	:read_threaded
Description	:individual thread program, reads its assigned share of values from the file, stores the readdata into the shared memory
Parameters	: void casted pointer of readdata struct
Returns	:NONE
*/
void read_threaded(void *params){
	int temp, n=0, numcount=0;
	
//	extracting information from the passed parameters
	readdata *mydata = (readdata *)params;
	char *ipfile = mydata->filename;
	int threadnum = mydata->threadnum;
	float partsize = mydata->partsize;
	
//	calculating file reading interval of a thread
	float startval = partsize*threadnum;
	float endval = (threadnum+1)*partsize;
	
	int startvalenter = startval;
	int endvalenter = endval;
	
//	opening the file and positioning read pointer
	FILE *fp = fopen(ipfile,"r");
	fseek(fp, startvalenter, SEEK_SET);
	
	if(ftell(fp)!=0){
		fseek(fp, -1, SEEK_CUR);
		while((temp = fgetc(fp))!=' '){
			if(temp==EOF){
				break;
			}
		}
	}
	
//	reading values from file
	while( ftell(fp) < endvalenter ){
		if(fgetc(fp)==' '){
			if(ftell(fp)==endvalenter)
				break;
		}
		fseek(fp, -1, SEEK_CUR);
		fscanf(fp,"%d",&n);
		
//		storing values to the global array
		storage[threadnum][numcount] = n;
		numcount++;
	}
	
//	storing number of values read by individual thread 
	numcountperthread[threadnum] = numcount;
	shared_memory_sender(threadnum);
	
	fclose(fp);
}


/*
Function	:shared_memory_sender
Description	:Stores the values read by the thread in the shared memory
Parameters	:the index of the thread
Returns	:NONE
*/
void shared_memory_sender(int threadnum){
	int i,j,k=0;
	i=numwrit;
	numwrit = numwrit+numcountperthread[threadnum];
	
//	storing the values in the shared memory
	for(j=0;j<numcountperthread[threadnum];j++){
		((int*)shared_memory)[i+k] = storage[threadnum][j];
		k++;
	}
	
}



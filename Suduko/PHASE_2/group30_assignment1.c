    ////////// Our final submission: Group 30 /////////////
    # include <stdio.h>
    # include <stdlib.h>
    # include <pthread.h>
    # include <math.h>
    # include <stdbool.h>
    # include <string.h>
     
    typedef struct{
        int row;
        int column;
        int number;
    	int *gridin;
    	int paramsize;
    } mynewstruct;
     
     
    void suduko_threaded( void * params);
    void copygrid(int *copythis, int *here, int size);
    bool suduko_valid(int x,int y,int grid[36][36], int size);
    bool sameColumn(int x,int y,int grid[36][36], int size);
    bool sameRow(int x,int y,int grid[36][36], int size);
    bool sameSquare(int x,int y,int grid[36][36], int size);
    bool isanypointempty(int *ptr, int size);
     
     
    void read_grid_from_file(int size, char *ip_file, int grid[36][36]) {
    	FILE *fp;
    	int i, j;
    	fp = fopen(ip_file, "r");
    	for (i=0; i<size; i++){
    		for (j=0; j<size; j++) 
    			fscanf(fp, "%d", &grid[i][j]);
    	}
    } 
     
    void print_grid(int size, int grid[36][36]) {
    	int i, j;
    	/* The segment below prints the grid in a standard format. Do not change */
    	for (i=0; i<size; i++) {
    		for (j=0; j<size; j++)
    			printf("%d\t", grid[i][j]);
    		printf("\n");
    	}
    }
     
     
    int main(int argc, char *argv[]) {	
    	int size,grid[36][36];
    	
     
        if (argc != 3) {
    		printf("Usage: ./sudoku.out grid_size inputfile");
    		exit(-1);
    	}
    	
    	size = atoi(argv[1]);
    	read_grid_from_file(size, argv[2], grid);
     
    	
    	/* Do your thing here */
    	
    	int i=0,j=0;
        while(grid[i][j]!=0){
    		i = ((j+1)%size==0) ? i+1 : i;
    		j = (j+1)%size;
    	}
    	
    	mynewstruct mydata;
    	mydata.row = i;
    	mydata.column = j;
    	mydata.number = -1;
    	mydata.gridin = grid;
    	mydata.paramsize = size;
    	
    	suduko_threaded((void *)&mydata);
    	
    //	print_grid(size, grid);   
    	return 0;
    }
     
     
    void suduko_threaded( void * params){
    	mynewstruct *mydata = (mynewstruct *)params;
    	int x, y, num, *mygrid, size;
    	x = mydata->row;
    	y = mydata->column;
    	num = mydata->number;
    	mygrid = mydata->gridin;
    	size = mydata->paramsize;
    	
    	int i,j,k;
    	int inside_grid[36][36] = {{0}};
    	bool isvalid = false;
    	pthread_t threadvals[size];
    		
    	copygrid(mygrid,inside_grid,size);
     
    	
    	if(inside_grid[x][y]!=0){
    		return;
     
    	}
    	
    	if(num!=-1){
    		inside_grid[x][y]=num;
     
    		isvalid = suduko_valid(x,y,inside_grid,size);
    		if(!isvalid){
    			return;
    		}
    	}	
    	
    	
    	if(!isanypointempty(inside_grid,size)){
    		print_grid(size,inside_grid);
    		exit(0);
    	}
    	
    	mynewstruct datavals[size];
    	for(i=0; i<size; i++){
    		if(num==-1){
    			datavals[i].row = x;
    			datavals[i].column = y;
    		}
    		else{
    			while(inside_grid[x][y]!=0){
    				x = ((y+1)%size==0) ? x+1 : x;
    				y = (y+1)%size;
    			}
    			datavals[i].row = x;
    			datavals[i].column = y;
    		}
    		
    		datavals[i].number = i+1;
    		datavals[i].gridin = (int *)inside_grid;
    		datavals[i].paramsize = size; 
    		
     
    		
    		if(pthread_create(&threadvals[i], NULL, (void *)suduko_threaded,(void *)(&datavals[i]))){
    			printf("Error in creating thread %d",i);
    			exit(-1);
    		}
    	}
    	
    	for(i=0;i<size;i++){
            pthread_join(threadvals[i],NULL);
        }
       
    		
    }
     
    //
    bool isanypointempty(int *ptr, int size){
    	int i,j;	
    	bool isempty;
    	for(i=0; i<size; i++){
    		for(j=0; j<size; j++){
    			if(*( ptr + i*36 + j)==0){
    				return true;
    			}			
    		}
    	}
    	return false;	
    }
     
    void copygrid(int *copythis, int *here, int size){
    	int i,j;	
    	for(i=0; i<size; i++){
    		for(j=0; j<size; j++){
    			*( here + i*36 + j)= *( copythis + i*36 + j);			
    		}
    	}	
    }
     
    bool suduko_valid(int x,int y,int passgrid[36][36], int size){
    	if(!sameColumn(x,y,passgrid,size) && !sameRow(x,y,passgrid,size) && !sameSquare(x,y,passgrid,size))
    		return true;
    		
    	return false;
    }
     
    bool sameColumn(int x,int y,int passgrid[36][36], int size){
    	int i,num;
    	num = passgrid[x][y];
     
       for(i=0; i<size; i++){
       	if(i!=x && passgrid[i][y]==num){
     
       		return true;
    		}
    	}
    	return false;
    }
     
    bool sameRow(int x,int y,int passgrid[36][36], int size){
    	int i,num;
    	num = passgrid[x][y];
       for(i=0; i<size; i++){
       	if(i!=y && passgrid[x][i]==num){
       		return true;
    		}
    	}
    	return false;
    }
     
    bool sameSquare(int x,int y,int passgrid[36][36], int size){
       int i,j,xin,yin;
       
       xin=x/sqrt(size);
       xin *= sqrt(size);
       yin=y/sqrt(size);
       yin *= sqrt(size);
     
     
       for( i=xin;i<xin+sqrt(size);i++){
           for( j=yin;j<yin+sqrt(size);j++){
               if(i!=x && j!=y && passgrid[i][j]==passgrid[x][y]){
                   return true;
               }
           }
       }
       return false;
    }
     
     

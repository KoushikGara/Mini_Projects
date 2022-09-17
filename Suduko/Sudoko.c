    # include <stdio.h>
    # include <stdlib.h>
    # include <pthread.h>
    # include <math.h>
     
     
    int sudokoSolver(int , int);
    void printSudoko();
    void * columnCheck(void * params);
    void * rowCheck(void *params);
    void * squareCheck(void *params);
     
     
    typedef struct{
        int row;
        int column;
        int number;
    } my_parameters;
     
    int size;
    int grid[36][36];
     
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
    	
    	if (argc != 3) {
    		printf("Usage: ./sudoku.out grid_size inputfile");
    		exit(-1);
    	}
    	
    	size = atoi(argv[1]);
    	read_grid_from_file(size, argv[2], grid);
    	
    	/* Do your thing here */
        int x=0,y=0;
     
        sudokoSolver(x, y);
    	
    	print_grid(size, grid);   
    }
     
    int sudokoSolver(int x, int y){
        // 
        void * row_bool, * col_bool, * box_bool;    
        my_parameters  tempvals;
        pthread_t row_check, col_check, box_check;
        // 
      int num =1;
      int tx = 0;
      int ty =0;
    //   printf("%d", &num);
      if(grid[x][y]!=0){
          if(x==(size-1) && y==(size-1)){
              return 1;
          }
          if(x<(size-1)){
              x++;
          }else{
              x = 0;
              y++;
          }
          if(sudokoSolver(x,y)){
              return 1;
          }else{
              return 0;
          }
     
      }
      if(grid[x][y]==0){
          while(num<(size+1)){
            //   
              tempvals.row = x;
              tempvals.column = y;
              tempvals.number = num;
     
              pthread_create(&row_check,NULL,rowCheck,&tempvals);
              pthread_create(&col_check,NULL,columnCheck,&tempvals);
              pthread_create(&box_check,NULL,squareCheck,&tempvals);
     
              pthread_join(row_check,&row_bool);
              pthread_join(col_check,&col_bool);
              pthread_join(box_check,&box_bool);
              
    //          printf("%d %d %d\n", (int)row_bool, (int)col_bool, (int)box_bool);
     
    //          int exp = (int)row_bool==0 && (int)col_bool==0 && (int)box_bool==0;
    //          printf("%d\n",&exp);
            //   
            //   if((!sameColumn(x,y,num) && !sameSquare(x,y,num)) && !sameRow(x,y,num)){
                if((int)row_bool==0 && (int)col_bool==0 && (int)box_bool==0){
    //            	printf("in here");
                  grid[x][y]=num;
                  if(x==(size-1) && y==(size-1)){
                      return 1;
                  }
                  if(x<(size-1)){
                      tx = x+1;
                  }else{
                      tx=0;
                      ty = y+1;
                  }
                //   print_grid(size, grid);
    //              printf("\n");
                  if(sudokoSolver(tx,ty)){
                      return 1;
                  }
              }
              num++;
          }
          grid[x][y]=0;
    //      print_grid(size, grid);
          return 0;
      }
    }
     
    // void * sameColumn(int x,int y,int num){
    void * columnCheck(void * params){
    	int i;
        my_parameters * mydata = (my_parameters *)params;
        int y = mydata->column;
        int num = mydata->number;    
        for( i=0;i<size;i++){
            if(grid[i][y]==num){
                return (void *)1;
            }
        }
        return (void *)0;
    }
     
    // int sameRow(int x,int y,int num){
    void * rowCheck(void *params){
    	int i;
        my_parameters * mydata = (my_parameters *)params;
        int x = mydata->row;
        int num = mydata->number;
        for( i=0;i<size;i++){
            if(grid[x][i]==num){
               return (void *)1;
            }
        }
        return (void *)0;
    }
     
    // int sameSquare(int x,int y,int num){
    void * squareCheck(void *params){
    	int i,j;
        my_parameters * mydata = (my_parameters *)params;
        int x = mydata->row;
        int y = mydata->column;
        int num = mydata->number;
        
        x=x/sqrt(size);
        x *= sqrt(size);
        y=y/sqrt(size);
        y *= sqrt(size);
     
     
        for(i=x;i<x+sqrt(size);i++){
            for( j=y;j<y+sqrt(size);j++){
                if(grid[i][j]==num){
                    return  (void *)1;
                }
            }
        }
        
        return (void *)0;
    }
     

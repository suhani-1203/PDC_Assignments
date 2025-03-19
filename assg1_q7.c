 //Random Walk with MPI 
 
#include <mpi.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
 
#define MAX_STEPS 10  
#define WALKER_ID 0   
 
int main(int argc, char** argv) { 
     
    MPI_Init(&argc, &argv); 
 
    
    int world_rank; 
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); 
    int world_size; 
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); 
 
    if (world_size < 2) { 
        if (world_rank == 0) { 
            printf("This program requires at least 2 processes.\n"); 
        } 
        MPI_Finalize(); 
        return 1; 
    } 
 
    int position = 0;  
    int step; 
     
    if (world_rank == WALKER_ID) { 
        srand(time(NULL)); 
 
        for (int i = 0; i < MAX_STEPS; i++) { 
          
            step = (rand() % 2 == 0) ? -1 : 1; 
            position += step; 
 
            printf("Walker at step %d, new position: %d\n", i + 1, position); 
 
            MPI_Send(&position, 1, MPI_INT, 1, 0, MPI_COMM_WORLD); 
        } 
    }  
    else if (world_rank == 1) { 
        for (int i = 0; i < MAX_STEPS; i++) { 
            MPI_Recv(&position, 1, MPI_INT, WALKER_ID, 0, MPI_COMM_WORLD, 
MPI_STATUS_IGNORE); 
            printf("Observer received position: %d\n", position); 
        } 
    } 
 
    MPI_Finalize(); 
    return 0; 
}

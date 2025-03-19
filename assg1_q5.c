// Querying MPI_status Structure 
 
#include <mpi.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
 
#define MAX_NUMBERS 100  // Maximum number of integers to send 
 
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
 
    int numbers[MAX_NUMBERS];  
    int number_amount;         
 
    if (world_rank == 0) { 
        srand(time(NULL)); 
 
        number_amount = (rand() / (float)RAND_MAX) * MAX_NUMBERS; 
         
        for (int i = 0; i < number_amount; i++) { 
            numbers[i] = rand() % 100;  
        } 
 
        MPI_Send(numbers, number_amount, MPI_INT, 1, 0, MPI_COMM_WORLD); 
        printf("Process 0 sent %d numbers to Process 1\n", number_amount); 
    }  
    else if (world_rank == 1) { 
        MPI_Status status; 
         
        MPI_Recv(numbers, MAX_NUMBERS, MPI_INT, 0, 0, MPI_COMM_WORLD, &status); 
 
        MPI_Get_count(&status, MPI_INT, &number_amount); 
 
        printf("Process 1 received %d numbers from Process 0. Message source = %d, tag = 
%d\n", 
               number_amount, status.MPI_SOURCE, status.MPI_TAG); 
    } 
 
    MPI_Finalize(); 
    return 0; 
} 

//Q1 Hello World!
Ques1 : Hello World  
 
#include <mpi.h> 
#include <stdio.h> 
 
int main(int argc, char** argv) { 
    MPI_Init(NULL, NULL); 
 
    int size; 
    MPI_Comm_size(MPI_COMM_WORLD, size); 
 
    int rank; 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
  
    char proc_name[MPI_MAX_PROCESSOR_NAME]; 
    int name_len; 
    MPI_Get_processor_name(proc_name, &name_len); 
 
    printf("Hello world from processor %s, rank %d out of %d processors\n", 
           processor_name, world_rank, world_size); 

    MPI_Finalize(); 
} 

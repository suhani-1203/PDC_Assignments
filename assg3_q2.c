#include <mpi.h>  
#include <stdio.h>  
  
#define NUM_STEPS 100000 
  
int main(int argc, char *argv[]) {  
    int rank, size, i;  
    long num_steps = NUM_STEPS;  
    double step, x, sum = 0.0, local_sum = 0.0, pi;  
      
    MPI_Init(&argc, &argv);                 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);   
    MPI_Comm_size(MPI_COMM_WORLD, &size);    
        
    step = 1.0 / (double)num_steps;  
  
    MPI_Bcast(&num_steps, 1, MPI_LONG, 0, MPI_COMM_WORLD);  

    for (i = rank; i < num_steps; i += size) {  
        x = (i + 0.5) * step;  
        local_sum += 4.0 / (1.0 + x * x);  
    }  
  
    MPI_Reduce(&local_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);  
  
    
    if (rank == 0) {  
        pi = step * sum;  
        printf("Approximate π: %.15f\n", pi);  
    }  
  
    MPI_Finalize();  
    return 0;  
}  

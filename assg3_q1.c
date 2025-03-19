#include <mpi.h>  
#include <stdio.h>  
#include <stdlib.h>  
  
#define N (1 << 16) 
#define A 2.5       
  
void sequential_daxpy(double* x, double* y) {  
    for (int i = 0; i < N; i++) {  
        x[i] = A * x[i] + y[i];  
    }  
}  
  
void parallel_daxpy(int rank, int size) {  
    int chunk_size = N / size;  
    double *x, *y, *local_x, *local_y;  
      
    if (rank == 0) {  
        x = (double*) malloc(N * sizeof(double));  
        y = (double*) malloc(N * sizeof(double));  
          
        for (int i = 0; i < N; i++) {  
            x[i] = i * 1.0;  
            y[i] = i * 0.5;  
        }  
    }  
      
    local_x = (double*) malloc(chunk_size * sizeof(double));  
    local_y = (double*) malloc(chunk_size * sizeof(double));  
      
    MPI_Scatter(x, chunk_size, MPI_DOUBLE, local_x, chunk_size, MPI_DOUBLE, 0, 
MPI_COMM_WORLD);  
    MPI_Scatter(y, chunk_size, MPI_DOUBLE, local_y, chunk_size, MPI_DOUBLE, 0, 
MPI_COMM_WORLD);  
  
    for (int i = 0; i < chunk_size; i++) {  
        local_x[i] = A * local_x[i] + local_y[i];  
    }  
      
    MPI_Gather(local_x, chunk_size, MPI_DOUBLE, x, chunk_size, MPI_DOUBLE, 0, 
MPI_COMM_WORLD);  
  
    if (rank == 0) {  
        free(x);  
        free(y);  
    }  
      
    free(local_x);  
    free(local_y);  
}  
  
int main(int argc, char* argv[]) {  
    int rank, size;  
    double start, end, seq_time, mpi_time;  
  
    MPI_Init(&argc, &argv);  
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  
    MPI_Comm_size(MPI_COMM_WORLD, &size);  
  
    if (rank == 0) {  
        double* x = (double*) malloc(N * sizeof(double));  
        double* y = (double*) malloc(N * sizeof(double));  
  
        for (int i = 0; i < N; i++) {  
            x[i] = i * 1.0;  
            y[i] = i * 0.5;  
        }  
  
        start = MPI_Wtime();  
        sequential_daxpy(x, y);  
        end = MPI_Wtime();  
        seq_time = end - start;  
        printf("Sequential time: %f seconds\n", seq_time);  
  
        free(x);  
        free(y);  
    }  
  
    MPI_Barrier(MPI_COMM_WORLD);  
    start = MPI_Wtime();  
    parallel_daxpy(rank, size);  
    end = MPI_Wtime();  
    mpi_time = end - start;  
  
    if (rank == 0) {  
        printf("Parallel time with %d processes: %f seconds\n", size, mpi_time);  
        printf("Speedup: %f\n", seq_time / mpi_time);  
    }  
  
    MPI_Finalize();  
    return 0;  
}  

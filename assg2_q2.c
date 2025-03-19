#include <stdio.h> 
#include <stdlib.h> 
#include <mpi.h> 
#include <omp.h> 
 
#define SIZE 70  
 
void multiply_serial(double A[SIZE][SIZE], double B[SIZE][SIZE], double C[SIZE][SIZE]) { 
    for (int i = 0; i < SIZE; i++) { 
        for (int j = 0; j < SIZE; j++) { 
            C[i][j] = 0; 
            for (int k = 0; k < SIZE; k++) { 
                C[i][j] += A[i][k] * B[k][j]; 
            } 
        } 
    } 
} 
 
void multiply_parallel(double A[SIZE][SIZE], double B[SIZE][SIZE], double C[SIZE][SIZE], int rank, int 
size) { 
    int rows_per_process = SIZE / size; 
    int start_row = rank * rows_per_process; 
    int end_row = (rank == size - 1) ? SIZE : start_row + rows_per_process; 
 
    for (int i = start_row; i < end_row; i++) { 
        for (int j = 0; j < SIZE; j++) { 
            C[i][j] = 0; 
            for (int k = 0; k < SIZE; k++) { 
                C[i][j] += A[i][k] * B[k][j]; 
            } 
        } 
    } 
 
     
    MPI_Gather(C[start_row], rows_per_process * SIZE, MPI_DOUBLE, 
               C, rows_per_process * SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD); 
} 
 
int main(int argc, char *argv[]) { 
    int rank, size; 
    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 
 
    double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE]; 
 
    if (rank == 0) { 
        // Initialize matrices 
        for (int i = 0; i < SIZE; i++) { 
            for (int j = 0; j < SIZE; j++) { 
                A[i][j] = rand() % 10; 
                B[i][j] = rand() % 10; 
            } 
        } 
    } 
 
    
    MPI_Bcast(A, SIZE * SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD); 
    MPI_Bcast(B, SIZE * SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD); 
 
    double start_time, run_time; 
 
    
    if (rank == 0) { 
        start_time = omp_get_wtime(); 
        multiply_serial(A, B, C); 
        run_time = omp_get_wtime() - start_time; 
        printf("Serial Execution Time: %f seconds\n", run_time); 
    } 
 
    MPI_Barrier(MPI_COMM_WORLD); 
 
    start_time = omp_get_wtime(); 
    multiply_parallel(A, B, C, rank, size); 
    run_time = omp_get_wtime() - start_time; 
 
    if (rank == 0) { 
        printf("Parallel Execution Time: %f seconds\n", run_time); 
    } 
 
    MPI_Finalize(); 
    return 0; 
}

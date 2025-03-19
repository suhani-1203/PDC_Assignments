#include <stdio.h> 
#include <stdlib.h> 
#include <mpi.h> 
 
#define N 16  // Size of the input array 
 
int main(int argc, char *argv[]) { 
    int rank, size; 
    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 
 
    int local_N = N / size;  
 
     
    int *A = NULL; 
    int *local_A = (int *)malloc(local_N * sizeof(int)); 
    int *local_prefix = (int *)malloc(local_N * sizeof(int)); 
 
    if (rank == 0) { 
        A = (int *)malloc(N * sizeof(int)); 
        for (int i = 0; i < N; i++) { 
            A[i] = i + 1;  
        } 
    } 
 
    MPI_Scatter(A, local_N, MPI_INT, local_A, local_N, MPI_INT, 0, MPI_COMM_WORLD); 
 
    local_prefix[0] = local_A[0]; 
    for (int i = 1; i < local_N; i++) { 
        local_prefix[i] = local_prefix[i - 1] + local_A[i]; 
    } 
 
    int prev_sum = 0; 
    MPI_Scan(&local_prefix[local_N - 1], &prev_sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD); 
 
    if (rank > 0) { 
        for (int i = 0; i < local_N; i++) { 
            local_prefix[i] += prev_sum - local_prefix[local_N - 1]; 
        } 
    } 
 
    MPI_Gather(local_prefix, local_N, MPI_INT, A, local_N, MPI_INT, 0, MPI_COMM_WORLD); 
 
    if (rank == 0) { 
        printf("Prefix Sum: "); 
        for (int i = 0; i < N; i++) { 
            printf("%d ", A[i]); 
        } 
        printf("\n"); 
        free(A); 
    } 
 
    free(local_A); 
    free(local_prefix); 
    MPI_Finalize(); 
    return 0; 
} 

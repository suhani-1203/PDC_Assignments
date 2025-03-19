#include <stdio.h> 
#include <stdlib.h> 
#include <mpi.h> 
 
void swap(int *a, int *b) { 
    int temp = *a; 
    *a = *b; 
    *b = temp; 
} 
 
void odd_even_sort(int *arr, int n) { 
    int sorted = 0; 
    while (!sorted) { 
        sorted = 1; 
 
        for (int i = 1; i < n - 1; i += 2) { 
            if (arr[i] > arr[i + 1]) { 
                swap(&arr[i], &arr[i + 1]); 
                sorted = 0; 
            } 
        } 
 
        for (int i = 0; i < n - 1; i += 2) { 
            if (arr[i] > arr[i + 1]) { 
                swap(&arr[i], &arr[i + 1]); 
                sorted = 0; 
            } 
        } 
    } 
} 
 
void parallel_odd_even_sort(int *local_arr, int local_n, int rank, int size) { 
    int sorted = 0; 
    MPI_Status status; 
 
    while (!sorted) { 
        sorted = 1; 
 
        if (rank % 2 == 1 && rank < size - 1) { 
            MPI_Send(local_arr + local_n - 1, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD); 
            MPI_Recv(local_arr + local_n - 1, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &status); 
        } else if (rank % 2 == 0 && rank > 0) { 
            int recv; 
            MPI_Recv(&recv, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status); 
            if (recv > local_arr[0]) { 
                swap(&recv, &local_arr[0]); 
                sorted = 0; 
            } 
            MPI_Send(&recv, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD); 
        } 
 
        if (rank % 2 == 0 && rank < size - 1) { 
            MPI_Send(local_arr + local_n - 1, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD); 
            MPI_Recv(local_arr + local_n - 1, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &status); 
        } else if (rank % 2 == 1 && rank > 0) { 
            int recv; 
            MPI_Recv(&recv, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status); 
            if (recv > local_arr[0]) { 
                swap(&recv, &local_arr[0]); 
                sorted = 0; 
            } 
            MPI_Send(&recv, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD); 
        } 
 
        int global_sorted; 
        MPI_Allreduce(&sorted, &global_sorted, 1, MPI_INT, MPI_LAND, MPI_COMM_WORLD); 
        sorted = global_sorted; 
    } 
} 
 
int main(int argc, char *argv[]) { 
    int rank, size, n = 16; 
    int arr[16] = {29, 10, 14, 37, 13, 26, 12, 30, 22, 11, 15, 32, 19, 31, 17, 25}; 
 
    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 
 
    int local_n = n / size; 
    int *local_arr = (int *)malloc(local_n * sizeof(int)); 
 
    MPI_Scatter(arr, local_n, MPI_INT, local_arr, local_n, MPI_INT, 0, MPI_COMM_WORLD); 
 
    parallel_odd_even_sort(local_arr, local_n, rank, size); 
 
    MPI_Gather(local_arr, local_n, MPI_INT, arr, local_n, MPI_INT, 0, MPI_COMM_WORLD); 
  
    if (rank == 0) { 
        printf("Sorted Array: "); 
        for (int i = 0; i < n; i++) { 
            printf("%d ", arr[i]); 
        } 
        printf("\n"); 
    } 
 
    free(local_arr); 
    MPI_Finalize(); 
    return 0; 
} 

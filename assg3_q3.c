#include <mpi.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <math.h>  
  
#define MAX_NUM 100  
  
 
int is_prime(int num) {  
    if (num < 2) return 0;  
    for (int i = 2; i <= sqrt(num); i++) {  
        if (num % i == 0) return 0;  
    }  
    return 1;  
}  
  
int main(int argc, char* argv[]) {  
    int rank, size, num, max_val = MAX_NUM;  
    MPI_Init(&argc, &argv);  
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  
    MPI_Comm_size(MPI_COMM_WORLD, &size);  
  
    if (rank == 0) {  
        int next_number = 2;  
        int active_workers = size - 1;   
        int received_number;  
          
        while (active_workers > 0) {  
            MPI_Recv(&received_number, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, 
MPI_STATUS_IGNORE);  
  
            if (received_number > 0) {  
                printf("%d ", received_number);  
            }  
  
            if (next_number <= max_val) {  
                MPI_Send(&next_number, 1, MPI_INT, received_number, 0, MPI_COMM_WORLD);  
                next_number++;  
            } else {  
                int stop_signal = -1;  
                MPI_Send(&stop_signal, 1, MPI_INT, received_number, 0, MPI_COMM_WORLD);  
                active_workers--;  
            }  
        }  
        printf("\n");  
    } else {    
        int number_to_test;  
        while (1) {  
            MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);  
            MPI_Recv(&number_to_test, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  
  
            if (number_to_test < 0) break;  
  
            int result = is_prime(number_to_test) ? number_to_test : -number_to_test;  
            MPI_Send(&result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);  
        }  
    }  
  
    MPI_Finalize();  
    return 0;  
} 

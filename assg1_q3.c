// MPI Ping Pong 
 
#include <mpi.h> 
#include <stdio.h> 
 
#define PING_PONG_LIMIT 10  
 
int main(int argc, char** argv) { 
    
    MPI_Init(&argc, &argv); 
 
    
    int world_rank; 
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); 
    int world_size; 
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); 
 
    if (world_size != 2) { 
        if (world_rank == 0) { 
            printf("This program requires exactly 2 processes.\n"); 
        } 
        MPI_Finalize(); 
        return 1; 
    } 
 
    int ping_pong_count = 0; 
    int partner_rank = (world_rank + 1) % 2; 
 
    while (ping_pong_count < PING_PONG_LIMIT) { 
        if (world_rank == ping_pong_count % 2) { 
            ping_pong_count++; 
            MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD); 
            printf("Process %d sent and incremented ping_pong_count %d to process %d\n", 
                   world_rank, ping_pong_count, partner_rank); 
        } else { 
            MPI_Recv(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD, 
MPI_STATUS_IGNORE); 
            printf("Process %d received ping_pong_count %d from process %d\n", 
                   world_rank, ping_pong_count, partner_rank); 
        } 
    } 
 
    MPI_Finalize(); 
    return 0; 
}

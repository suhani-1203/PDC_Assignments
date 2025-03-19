#include <stdio.h> 
#include <stdlib.h> 
#include <cuda_runtime.h> 
#include <curand_kernel.h> 
#include <mpi.h> 
 
#define NUM_POINTS 1000000  // Number of points per process 
 
__global__ void monteCarloPi(int *d_count, int numPoints, unsigned long seed) { 
    int idx = threadIdx.x + blockIdx.x * blockDim.x; 
    int localCount = 0; 
    curandState state; 
    curand_init(seed, idx, 0, &state); 
 
    for (int i = 0; i < numPoints; i++) { 
        float x = curand_uniform(&state); 
        float y = curand_uniform(&state); 
        if (x * x + y * y <= 1.0f) 
            localCount++; 
    } 
 
    atomicAdd(d_count, localCount); 
} 
 
int main(int argc, char **argv) { 
    int rank, size; 
     
    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
MPI_Comm_size(MPI_COMM_WORLD, &size); 
int pointsPerProcess = NUM_POINTS / size; 
int h_count = 0, totalCount = 0; 
int *d_count; 
cudaMalloc((void**)&d_count, sizeof(int)); 
cudaMemset(d_count, 0, sizeof(int)); 
int threadsPerBlock = 256; 
int blocksPerGrid = (pointsPerProcess + threadsPerBlock - 1) / threadsPerBlock; 

monteCarloPi<<<blocksPerGrid, threadsPerBlock>>>(d_count, pointsPerProcess, rank); 
cudaMemcpy(&h_count, d_count, sizeof(int), cudaMemcpyDeviceToHost); 

MPI_Reduce(&h_count, &totalCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD); 
if (rank == 0) { 
f
loat pi = 4.0f * totalCount / (NUM_POINTS); 
printf("Estimated Pi = %f\n", pi); 
} 

cudaFree(d_count); 
MPI_Finalize(); 
return 0; 
}

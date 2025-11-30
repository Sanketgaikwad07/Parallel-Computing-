#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

   
    std::cout << "Hello from process " << world_rank
              << " out of " << world_size << " processes." << std::endl;

    // Total numbers  of we want
    int N = 100;

    // Divide number as per size
    int part = N / world_size;
    int rem = N % world_size;

    int start = world_rank * part + std::min(world_rank, rem) + 1;
    int end = start + part - 1;
    if (world_rank < rem) {
        end += 1;
    }

    //  give the each process number
    std::cout << "[Process " << world_rank << "] Numbers: ";
    int count = 0;
    for (int i = start; i <= end; ++i) {
        std::cout << i << " ";
        count += i;
    }
    std::cout << "\n";

    //taking the all sum in total
    int total_sum = 0;
    if (world_rank == 0) {
        total_sum = count;
        for (int j = 1; j < world_size; ++j) {
            int recv_val;
            MPI_Recv(&recv_val, 1, MPI_INT, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_sum += recv_val;
        }
} else {
        MPI_Send(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            std::cout <<"done with another exist"<<std::endl;                            
    }

    MPI_Finalize();
 
}


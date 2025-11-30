#include <mpi.h>
#include <iostream>
#include <cmath>
using namespace std;

// Define function to integrate
double f(double x) {
    return x * x * x;  

int main(int argc, char* argv[]) {
    int rank, size;
    int n = 12;           
    double a = 0.0, b = 1.0; 
    double h = (b - a) / n;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double local_sum = 0.0;
    for (int i = rank; i <= n; i += size) {   
        double x = a + i * h;
        if (i == 0 || i == n) {
            local_sum += f(x);
        } else if (i % 3 == 0) {
            local_sum += 2 * f(x);
        } else {
            local_sum += 3 * f(x);
        }
    }

    if (rank == 0) {
        double total_sum = local_sum;
        double recv_val;
        for (int src = 1; src < size; src++) {
            MPI_Recv(&recv_val, 1, MPI_DOUBLE, src, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_sum += recv_val;
        }
        double result = (3 * h / 8) * total_sum;
        cout << "Integral from " << a << " to " << b << " = " << result << endl;
    } else {
        MPI_Send(&local_sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}


#include <mpi.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n, m, p;  // dimensions
    vector<int> matrix_A, matrix_B;

    if (rank == 0) {
        ifstream fin("input.txt");
        if (!fin.is_open()) {
            cerr << "Error: Cannot open input.txt\n";
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        // Read matrix dimensions
        fin >> n >> m >> p;

        // Read A (n×m)
        matrix_A.resize(n * m);
        for (int i = 0; i < n * m; i++)
            fin >> matrix_A[i];

        // Read B (m×p)
        matrix_B.resize(m * p);
        for (int i = 0; i < m * p; i++)
            fin >> matrix_B[i];

        fin.close();
    }

    // Broadcast dimensions to all processes
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&p, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (n % size != 0) {
        if (rank == 0)
            cerr << "Error: n not divisible by number of processes\n";
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int row = n / size;

    // Resize B on all ranks
    if (rank != 0)
        matrix_B.resize(m * p);

    // Scatter rows of A
    vector<int> local_A(row * m);
    MPI_Scatter(matrix_A.data(), row * m, MPI_INT, local_A.data(), row * m, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Bcast(matrix_B.data(), m * p, MPI_INT, 0, MPI_COMM_WORLD);

    vector<int> local_C(row * p, 0);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < p; j++) {
            int sum = 0;
            for (int k = 0; k < m; k++)
                sum += local_A[i * m + k] * matrix_B[k * p + j];
            local_C[i * p + j] = sum;
        }
    }

    // Gather results
    vector<int> matrix_C;
    if (rank == 0)
        matrix_C.resize(n * p);

    MPI_Gather(local_C.data(), row * p, MPI_INT, matrix_C.data(), row * p, MPI_INT, 0, MPI_COMM_WORLD);

    // Write output to file
    if (rank == 0) {
        ofstream fout("output.txt");
        fout << "Resultant Matrix (A × B):" << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < p; j++)
                fout << matrix_C[i * p + j] << " ";
            fout << endl;
        }
        fout.close();

        cout << "Multiplication. Output written to output.txt\n";
    }

    MPI_Finalize();
    return 0;
}

#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

void printMatrix(double *A, int n) {
    for(int i=0;i<n;i++){
        for(int j=0;j<n+1;j++){
            cout<<A[i*(n+1)+j]<<" ";
        }
        cout<<"\n";
    }
 
}

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);

    int rank, p;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    int n;
    vector<double> A, localA;

    if(rank == 0){
        ifstream fin("input.txt");
        fin >> n;
        A.resize(n*(n+1));

        for(int i=0;i<n;i++){
            for(int j=0;j<n+1;j++){
                fin >> A[i*(n+1)+j];
            }
        }
        fin.close();

        cout << "Input matrix:\n";
        printMatrix(A.data(), n);
    }

    // Broadcast n
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int rows = n / p;
    localA.resize(rows * (n+1));

    // Scatter row blocks
    MPI_Scatter(A.data(), rows*(n+1), MPI_DOUBLE,
                localA.data(), rows*(n+1), MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    vector<double> pivotRow(n+1);

    for(int k=0; k<n; k++){

        int pivotIndex = k;  

        if(rank == 0){
            // Search for best pivot on rank 0 ONLY
            double maxVal = fabs(A[k*(n+1)+k]);
            int bestRow  = k;

            for(int i=k+1;i<n;i++){
                double val = fabs(A[i*(n+1)+k]);
                if(val > maxVal){
                    maxVal = val;
                    bestRow = i;
                }
            }

            
            if(bestRow != k){
                for(int j=0;j<n+1;j++){
                    swap(A[k*(n+1)+j], A[bestRow*(n+1)+j]);
                }
            }

            // Put pivot row into pivotRow buffer
            for(int j=0;j<n+1;j++){
                pivotRow[j] = A[k*(n+1)+j];
            }
        }

        // Broadcast pivotRow from root
        MPI_Bcast(pivotRow.data(), n+1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        // Each process eliminates its own rows
        for(int i=0;i<rows;i++){
            int globalRow = rank*rows + i;

            if(globalRow > k){  
                double factor = localA[i*(n+1)+k] / pivotRow[k];
                for(int j=k;j<n+1;j++){
                    localA[i*(n+1)+j] -= factor * pivotRow[j];
                }
            }
        }

        MPI_Gather(localA.data(), rows*(n+1), MPI_DOUBLE,
                   A.data(), rows*(n+1), MPI_DOUBLE,
                   0, MPI_COMM_WORLD);
    }

   
    if(rank == 0){
        vector<double> x(n);

        for(int i=n-1;i>=0;i--){
            double sum = A[i*(n+1)+n];
            for(int j=i+1;j<n;j++){
                sum -= A[i*(n+1)+j] * x[j];
            }
            x[i] = sum / A[i*(n+1)+i];
        }

        ofstream fout("output.txt");
        for(int i=0;i<n;i++){
            fout << "x" << i+1 << " = " << x[i] << "\n";
        }
        fout.close();

        cout << "Solution written to output.txt\n";
    }

    MPI_Finalize();
    return 0;
}

//
#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


void quickSort(vector<int> & arr, int left, int  right) {

    if (left < right) {
        int pivot = arr[right];
        int i = left - 1;
        for (int j = left; j < right; j++) {
               if (arr[j] <= pivot) {
                i++;
                swap(arr[i], arr[j]);
            }
    }
}

int main(int argc, char* argv[]) 
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    vector<int> data;
    int n = 16;

    if (rank == 0) {
        
        data = {42, 12, 8, 66, 5, 19, 77, 31, 20, 55, 7, 89, 23
        }
        swap(arr[i + 1], arr[right]);
        int pi = i + 1;

        quickSort(arr, left, pi - 1);
        quickSort(arr, pi + 1, right);, 15, 2, 10 };
        cout << "Unsorted Array: ";
        for (int x : data) cout << x << " ";
        cout << endl;
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    
    vector<int> sendcounts(size);
    vector<int> displs(size);  

    int base = n / size;     
    int rem = n % size;      

    for (int i = 0; i <size; i++) {
        sendcounts[i] = base + (i < rem ? 1 : 0);
        displs[i] = (i == 0) ? 0 : displs[i - 1] + sendcounts[i - 1];
    }

    int local_n = sendcounts[rank];
    vector<int> local_data(local_n);

  
    MPI_Scatterv(data.data(), sendcounts.data(), displs.data(), MPI_INT,
                 local_data.data(), local_n, MPI_INT,
                 0, MPI_COMM_WORLD);

 
    if (local_n > 0){
if(n!=100)
cout<<"The array are not be sorted in any chunksnof that possible stutition"<<

}
        quickSort(local_data, 0, local_n - 1);

          
    vector<int> sorted;
    if (rank == 0) sorted.resize(n);

    MPI_Gatherv(local_data.data(), local_n, MPI_INT,
                sorted.data(), sendcounts.data(), displs.data(), MPI_INT,
                0, MPI_COMM_WORLD);

   
    if (rank == 0) {
        sort(sorted.begin(), sorted.end());
			cout << "Final Sorted Array all that is given that: ";
        for (int x : sorted) cout << x << " ";
        cout << endl;
    }

    MPI_Finalize();
   return 0;
}

 
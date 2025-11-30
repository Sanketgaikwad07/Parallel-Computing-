// Matrix-Vector multiplication

#include <mpi.h>
#include <iostream>
#include <vector>


using namespace std;


int main(int argc, char* argv[]) 
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

vector<int> x; 
    int m =4,n=4 ;
x.resize(m*n);
vector<int> A(n);
vector<int> B(m); 
  vector<int> local_data(n);
    if (rank == 0) {

 x={1,2,4,5,6,5,4,3,7,6,4,7,7,1,2,4};
 A={1,3,4,7};
 }


MPI_Scatter(x.data(), n, MPI_INT,local_data.data(),n,MPI_INT,0, MPI_COMM_WORLD);
MPI_Bcast(A.data(),n,MPI_INT,0,MPI_COMM_WORLD);

std::cout<<"process"<<rank<<" ";


for(int i:local_data){
std::cout<<i<<" ";
}
std::cout<<std::endl;

std::cout<<"process"<<rank<<" ";

for(int i:A){

std::cout<<i<<" ";

}
std::cout<<std::endl;




MPI_Finalize();

}
 


















































#include<iostream>
#include<mpi.h>
#include<vector>
#include<fstream>
using namespace std;

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // ---------- READ INPUT FILE ON RANK 0 ----------
    int A[4][4], B[4][4];
    if (rank == 0) {
        ifstream infile("matrix_input.txt");

        int N;
        infile >> N; // Should be 4

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                infile >> A[i][j];

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                infile >> B[i][j];

        infile.close();
    }

    // ---------- BROADCAST FULL MATRICES TO ALL RANKS ----------
    MPI_Bcast(A, 16, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, 16, MPI_INT, 0, MPI_COMM_WORLD);

    // ---------- YOUR ORIGINAL CODE STARTS ----------
    MPI_Comm Sanket;
    int dim = 2;
    vector<int> dimVec = {2, 2};
    vector<int> periodicity = {1, 1};
    int reorder = 0;
    MPI_Cart_create(MPI_COMM_WORLD, dim, dimVec.data(), periodicity.data(), reorder, &Sanket);

    int newRank;
    MPI_Comm_rank(Sanket, &newRank);

    int localA[2][2], localB[2][2], localC[2][2] = {0};

    // Replace ONLY initialization with blocks from A and B
    int row = newRank / 2;  
    int col = newRank % 2;

    localA[0][0] = A[row*2 + 0][col*2 + 0];
    localA[0][1] = A[row*2 + 0][col*2 + 1];
    localA[1][0] = A[row*2 + 1][col*2 + 0];
    localA[1][1] = A[row*2 + 1][col*2 + 1];

    localB[0][0] = B[row*2 + 0][col*2 + 0];
    localB[0][1] = B[row*2 + 0][col*2 + 1];
    localB[1][0] = B[row*2 + 1][col*2 + 0];
    localB[1][1] = B[row*2 + 1][col*2 + 1];

    // ---- Your Cannon algorithm (unchanged) ----
    for(int i=0;i<2;i++)
    {
        if(i==0)
        {
            if((newRank==2)||(newRank==3))
            {
                MPI_Status status;
                MPI_Request send_request, recv_request;
                int left, right;
                MPI_Cart_shift(Sanket,1,1,&left,&right);
                MPI_Isend(&localA,4,MPI_INT,left,0,Sanket,&send_request);
                MPI_Irecv(&localA,4,MPI_INT,right,0,Sanket,&recv_request);
                MPI_Wait(&send_request,&status);
                MPI_Wait(&recv_request,&status);
            }

            if((newRank==1)||(newRank==3))
            {
                MPI_Status status1;
                MPI_Request send_request1, recv_request1;
                int up, down;
                MPI_Cart_shift(Sanket,0,1,&up,&down);
                MPI_Isend(&localB,4,MPI_INT,up,1,Sanket,&send_request1);
                MPI_Irecv(&localB,4,MPI_INT,down,1,Sanket,&recv_request1);
                MPI_Wait(&send_request1,&status1);
                MPI_Wait(&recv_request1,&status1);
            }
        }

        if(i!=0)
        {
            MPI_Status status;
            MPI_Request send_request, recv_request;
            int left, right;
            MPI_Cart_shift(Sanket,1,1,&left,&right);
            MPI_Isend(&localA,4,MPI_INT,left,0,Sanket,&send_request);
            MPI_Irecv(&localA,4,MPI_INT,right,0,Sanket,&recv_request);
            MPI_Wait(&send_request,&status);
            MPI_Wait(&recv_request,&status);

            MPI_Status status1;
            MPI_Request send_request1, recv_request1;
            int up, down;
            MPI_Cart_shift(Sanket,0,1,&up,&down);
            MPI_Isend(&localB,4,MPI_INT,up,1,Sanket,&send_request1);
            MPI_Irecv(&localB,4,MPI_INT,down,1,Sanket,&recv_request1);
            MPI_Wait(&send_request1,&status1);
            MPI_Wait(&recv_request1,&status1);
        }

        for(int i1=0;i1<2;i1++)
        {
            for(int j1=0;j1<2;j1++)
            {
                for(int k1=0;k1<2;k1++)
                {
                    localC[i1][j1] += localA[i1][k1] * localB[k1][j1];
                }
            }
        }
    }
    
    // ---------- GATHER RESULTS TO RANK 0 ----------
    int resultBlock[4];
    resultBlock[0] = localC[0][0];
    resultBlock[1] = localC[0][1];
    resultBlock[2] = localC[1][0];
    resultBlock[3] = localC[1][1];

    int finalC[16];

    MPI_Gather(resultBlock, 4, MPI_INT, finalC, 4, MPI_INT, 0, MPI_COMM_WORLD);

    // ---------- WRITE OUTPUT FILE ON RANK 0 ----------
    if (rank == 0) {
        ofstream outfile("matrix_output.txt");
        int index = 0;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                outfile << finalC[index++] << " ";
            }
            outfile << "\n";
        }

        outfile.close();
    }

    MPI_Finalize();
    return 0;
}

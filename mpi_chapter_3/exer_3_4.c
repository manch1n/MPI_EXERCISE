//author : manch1n
// time :3-24
// discription: chapter 3,exercise 3.4

#include <stdio.h>
#include <mpi.h>
#include <string.h>

int main(int argc, char **argv)
{
    int local_rank, comm_sz;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &local_rank);
    if (local_rank != 0)
    {
        char greet[20];
        sprintf(greet, "from rank: %d", local_rank);
        //this will block unitl target start receiving.
        MPI_Send(greet, 20, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
        char greet[20];
        MPI_Status recv_status;
        for (int i = 1; i < comm_sz; i++)
        {
            MPI_Recv(greet, 20, MPI_CHAR, i, 0, MPI_COMM_WORLD, &recv_status);
            printf("%s\n", greet);
        }
    }
    MPI_Finalize();
}
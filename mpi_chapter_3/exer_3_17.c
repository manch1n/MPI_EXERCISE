//author : manch1n
// date : 3-25
// discription : exercise 3.16 of chapter 3.

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

typedef struct
{
    int a;
    double b;
} my_struct;

int main(int argc, char **argv)
{

    int comm_sz, local_rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &local_rank);
    MPI_Datatype my_type;
    my_struct data;
    int block_len[2] = {1, 1};
    MPI_Aint a_addr;
    MPI_Aint b_addr;
    MPI_Address(&data.a, &a_addr);
    MPI_Address(&data.b, &b_addr);
    MPI_Aint displacement[2];
    displacement[0] = 0;
    displacement[1] = b_addr - a_addr;
    MPI_Datatype types[2] = {MPI_INT, MPI_DOUBLE};
    MPI_Type_struct(2, block_len, displacement, types, &my_type);
    MPI_Type_commit(&my_type);
    if (local_rank == 0)
    {
        my_struct cmx;
        cmx.a = 5033;
        cmx.b = 999.1;
        MPI_Send(&cmx, 1, my_type, 1, 0, MPI_COMM_WORLD);
    }
    else
    {
        my_struct crh;
        printf("%d : %d\n", crh.a, crh.b);
        MPI_Recv(&crh, 1, my_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("%d : %f", crh.a, crh.b);
    }

    MPI_Type_free(&my_type);
    MPI_Finalize();
}
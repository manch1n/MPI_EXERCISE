//author : manch1n
// date : 3-24
// discription : exercise 3.11 of chapter 3. Compute the prefix sum

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

//for question a
void serial(int *a, int n, int *o)
{
    int sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += a[i];
        o[i] = sum;
    }
}

//for question b
void p_serial(int *local_data, int local_rank, int comm_sz, MPI_Comm comm)
{
    int prev = 0;
    if (local_rank > 0)
    {
        MPI_Recv(&prev, 1, MPI_INT, local_rank - 1, MPI_ANY_TAG, comm, MPI_STATUS_IGNORE);
        *local_data += prev;
    }
    if (local_rank < comm_sz - 1)
    {
        MPI_Send(local_data, 1, MPI_INT, comm_sz + 1, MPI_ANY_TAG, comm);
    }
}

//local_data has been added locally.
void add(int prefix, int *local_data, int local_n)
{
    for (int i = 0; i < local_n; i++)
    {
        local_data[i] += prefix;
    }
}

//for question c
int main(int argc, char **argv)
{
    int comm_sz, local_rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &local_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    int *data = NULL;
    const int n = 16;
    int local_n = n / comm_sz;
    int *local_data = (int *)malloc(local_n * sizeof(int));

    if (local_rank == 0)
    {
        data = (int *)malloc(n * sizeof(int));
        printf("please input vector: \n");
        for (int i = 0; i < n; i++)
        {
            scanf("%d", &data[i]);
        }
        MPI_Scatter(data, local_n, MPI_INT, local_data, local_n, MPI_INT, 0, MPI_COMM_WORLD);
        free(data);
    }
    else
    {
        MPI_Scatter(data, local_n, MPI_INT, local_data, local_n, MPI_INT, 0, MPI_COMM_WORLD);
    }

    //locally add
    serial(local_data, local_n, local_data);
    int prefix = local_data[local_n - 1];
    printf("%d ",prefix);
    printf("\n");
    int phase = 3; //phase = k , size=2^k
    int offset = 1;
    while (phase--)
    {
        if (((1 + local_rank) % (offset * 2)) == 0)
        {
            if (local_rank - offset >= 0)
            {
                MPI_Recv(&prefix, 1, MPI_INT, local_rank - offset, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                prefix += local_data[local_n - 1];
                printf("from %d : %d ",local_rank,prefix);
                printf("\n");
            }
        }
        else if (((local_rank + offset + 1) % (offset * 2)) == 0)
        {
            if (local_rank + offset >= 0)
            {
                MPI_Send(&prefix, 1, MPI_INT, local_rank + offset, 0, MPI_COMM_WORLD);
            }
        }
        offset *= 2;
        MPI_Barrier(MPI_COMM_WORLD);
    }
    if (local_rank == comm_sz - 1)
    {
        printf("prefix sum: %d\n", prefix);
    }
    MPI_Finalize();
}
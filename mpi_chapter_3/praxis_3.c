//author : manch1n
// date : 3-27
// discription : odd-even exchange sort
// caution : the comm_sz must greater than 2 and should be even

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <time.h>

void get_n_small(int *local, int *neighbor, int n)
{
    int count = 0;
    int i = 0, j = 0;
    int *tmp = (int *)malloc(n * sizeof(int));
    memcpy(tmp, local, n * sizeof(int));
    while (count < n)
    {
        if (tmp[i] <= neighbor[j])
        {
            local[count] = tmp[i];
            i++;
            count++;
        }
        else
        {
            local[count] = neighbor[j];
            j++;
            count++;
        }
    }
    free(tmp);
}

void get_n_big(int *local, int *neighbor, int n)
{
    int count = n - 1;
    int i = n - 1, j = n - 1;
    int *tmp = (int *)malloc(n * sizeof(int));
    memcpy(tmp, local, n * sizeof(int));
    while (count != -1)
    {
        if (tmp[i] >= neighbor[j])
        {
            local[count] = tmp[i];
            i--;
            count--;
        }
        else
        {
            local[count] = neighbor[j];
            j--;
            count--;
        }
    }
    free(tmp);
}

int comparer(const void *small, const void *big)
{
    return (*(int*)small-*(int*)big);
}

int main(int argc, char **argv)
{
    int comm_sz, local_rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &local_rank);

    const int n = 10000;
    int *data = NULL;
    int local_n = n / comm_sz;
    int *local_data = (int *)malloc(local_n * sizeof(int));
    int *neighbor_data = (int *)malloc(local_n * sizeof(int));
    int phase = comm_sz;
    int neighbor;

    if (local_rank == 0)
    {
        srand(time(NULL));
        data = (int *)malloc(n * sizeof(int));
        for (int i = 0; i < n; i++)
        {
            data[i] = rand() % 100;
        }

        double duration = MPI_Wtime();

        MPI_Scatter(data, local_n, MPI_INT, local_data, local_n, MPI_INT, 0, MPI_COMM_WORLD);
        qsort(local_data, local_n, sizeof(int), comparer);
        while (phase)
        {
            if (phase % 2 == 0) //even pahse
            {
                MPI_Ssend(local_data, local_n, MPI_INT, local_rank + 1, 0, MPI_COMM_WORLD);
                MPI_Recv(neighbor_data, local_n, MPI_INT, local_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                get_n_small(local_data, neighbor_data, local_n);
            }
            printf("from %d pahse %d : %d neighbor:%d\n", local_rank, phase, local_data[0],local_rank+1);
            phase--;
            MPI_Barrier(MPI_COMM_WORLD);
        }
        
        MPI_Gather(local_data,local_n,MPI_INT,data,local_n,MPI_INT,0,MPI_COMM_WORLD);
        for(int i=0;i<n;i++)
        {
            printf("%d ",data[i]);
        }
        free(data);
        free(local_data);
        free(neighbor_data);
    }
    else
    {
        MPI_Scatter(data, local_n, MPI_INT, local_data, local_n, MPI_INT, 0, MPI_COMM_WORLD);
        qsort(local_data, local_n, sizeof(int), comparer);
        int get_big = 0;
        while (phase)
        {
            if (phase % 2 == 0)
            {
                if (local_rank % 2 == 0)
                {
                    neighbor = local_rank + 1;
                    get_big = 0;
                }
                else
                {
                    neighbor = local_rank - 1;
                    get_big = 1;
                }
            }
            else
            {
                if (local_rank % 2 == 0)
                {
                    neighbor = local_rank - 1;
                    get_big = 1;
                }
                else
                {
                    neighbor = local_rank + 1;
                    get_big = 0;
                }
            }
            if ((local_rank == comm_sz - 1) && phase % 2 == 1)
            {

                printf("from %d pahse %d : %d\n", local_rank, phase, local_data[0]);
                phase--;
                MPI_Barrier(MPI_COMM_WORLD);
            }
            else
            {
                MPI_Sendrecv(local_data, local_n, MPI_INT, neighbor, 0, neighbor_data, local_n, MPI_INT, neighbor, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if (get_big)
                {
                    get_n_big(local_data, neighbor_data, local_n);
                }
                else
                {
                    get_n_small(local_data, neighbor_data, local_n);
                }
                printf("from %d pahse %d : %d neigbor:%d\n", local_rank, phase, local_data[0],neighbor);
                phase--;
                MPI_Barrier(MPI_COMM_WORLD);
            }
        }
        MPI_Gather(local_data,local_n,MPI_INT,data,local_n,MPI_INT,0,MPI_COMM_WORLD);
        free(local_data);
        free(neighbor_data);
    }
    MPI_Finalize();
}
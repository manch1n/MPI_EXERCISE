//author: manch1n
// date : 3-25
// discription : exercise 3_11 of chapter 3 ,for the d answer

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void local_prefix(int *local_data, int local_n)
{
    for (int i = 1; i < local_n; i++)
    {
        local_data[i] += local_data[i - 1];
    }
}

int main(int argc, char **argv)
{
    int local_rank, comm_sz;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &local_rank);

    const int local_n = 4;
    int *local_data = (int *)malloc(local_n * sizeof(int));
    int *local_result = (int *)malloc(local_n * sizeof(int));
    srand(time(NULL) + comm_sz);
    printf("from : %d\n", local_rank);
    for (int i = 0; i < local_n; i++)
    {
        local_data[i] = rand() % 100;
        printf("%d ", local_data[i]);
    }
    printf("\n");
    //compute using mpi_scan
    local_prefix(local_data, local_n);
    //caution: mpi_scan only the last data of last process store the right answer. 
    MPI_Scan(local_data, local_result, local_n, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    printf("from :%d\n", local_rank);
    for (int i = 0; i < local_n; i++)
    {
        printf("%d ", local_result[i]);
    }
    free(local_data);
    free(local_result);
    MPI_Finalize();
}
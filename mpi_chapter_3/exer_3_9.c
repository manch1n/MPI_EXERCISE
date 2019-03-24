//author : manch1n
// date : 3-24
// discription : exercise 3.11 of chapter 3.To compute the dot product via two vectors.And i use the block-divide method

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

void compute_task(int* a,int* b,int* c,int n)
{
    for(int i=0;i<n;i++)
    {
        c[i]=a[i]*b[i];
    }
}

int main(int argc, char **argv)
{
    int comm_sz, local_rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &local_rank);
    const int n = 12;
    int local_n = n / comm_sz;
    
    int *local_a = (int *)malloc(local_n * sizeof(int));
    int *local_b = (int *)malloc(local_n * sizeof(int));
    int *local_c = (int *)malloc(local_n * sizeof(int));

    int *a = NULL;
    int *b = NULL;
    int *c = NULL; //c=a*b;

    if (local_rank == 0)
    {
        a = (int *)malloc(n * sizeof(int));
        printf("please input the vector a:\n");
        for (int i = 0; i < n; i++)
        {
            scanf("%d", &a[i]);
        }
        b = (int *)malloc(n * sizeof(int));
        c=(int*)malloc(n*sizeof(int));
        printf("please input the vector b:\n");
        for (int i = 0; i < n; i++)
        {
            scanf("%d", &b[i]);
        }
        MPI_Scatter(a, local_n, MPI_INT, local_a, local_n, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(b,local_n,MPI_INT,local_b,local_n,MPI_INT,0,MPI_COMM_WORLD);
        free(a);
        free(b);
        compute_task(local_a,local_b,local_c,local_n);
        free(local_a);free(local_b);
        MPI_Gather(local_c,local_n,MPI_INT,c,local_n,MPI_INT,0,MPI_COMM_WORLD);
        free(local_c);

        printf("the result:\n");
        for(int i=0;i<n;i++)
        {
            printf("%d ",c[i]);
        }
        free(c);
    }
    else
    {
        MPI_Scatter(a,local_n,MPI_INT,local_a,local_n,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Scatter(b,local_n,MPI_INT,local_b,local_n,MPI_INT,0,MPI_COMM_WORLD);
        compute_task(local_a,local_b,local_c,local_n);
        free(local_a);
        free(local_b);
        MPI_Gather(local_c,local_n,MPI_INT,c,local_n,MPI_INT,0,MPI_COMM_WORLD);
        free(local_c);
    }
    MPI_Finalize();
}
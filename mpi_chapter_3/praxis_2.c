//author : manch1n
// date : 3-27
// discription :
/* We can assume that we are now tossing the shurikens to a square board.The board's side length is 2 inch,and bull's eye is in central.And we draw a cycle which radius is 1 inch,cause that its area is one PI square foot.If the point hitting the target is averaged,the ammount hit the cycle can satisfy below function : hit/all=PI/4.Based on above,we can use this function to estimate the PI value.This method is so called Menticano Way. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <time.h>

#define MAX 20000
#define MIN 0
#define DECIMAL 0.0001
#define RANGE ((MAX - MIN + 1) + MIN) //create range [min*,max*]

void compute_task(int n, int *hit)
{
    double i, j, s;
    *hit = 0;
    int offset = -1;
    for (int q = 0; q < n; q++)
    {
        i = (rand() % RANGE) * DECIMAL + offset;
        j = (rand() % RANGE) * DECIMAL + offset;
        s = i * i + j * j;
        if (s <= 1)
            (*hit)++;
    }
}

int main(int argc, char **argv)
{
    int local_rank, comm_sz;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &local_rank);

    int local_n;
    const int target = 1; //the distance from hit to central should less this value.
    int local_result = 0;
    int all_result = 0;
    
    double start=MPI_Wtime();

    srand(time(NULL));

    if (local_rank == 0)
    {
        printf("input n: ");
        int n;
        scanf("%d", &n);
        local_n = n / comm_sz;
        MPI_Bcast(&local_n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        compute_task(local_n, &local_result);
        MPI_Reduce(&local_result, &all_result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        double pi = (double)all_result*4 / (double)n;
        printf("%f\n", pi);
        double end=MPI_Wtime()-start;
        printf("time:%f\n",end);
    }
    else
    {
        MPI_Bcast(&local_n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        compute_task(local_n, &local_result);
        MPI_Reduce(&local_result, &all_result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}
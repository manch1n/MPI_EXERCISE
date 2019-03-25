//author : manch1n
// date : 3-25
// discription : exercise 3.12 of chapter 3.loop communication

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*! for question a */
// int main(int argc, char **agrv)
// {
//     int comm_sz, local_rank;
//     MPI_Init(NULL, NULL);
//     MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
//     MPI_Comm_rank(MPI_COMM_WORLD, &local_rank);

//     //sample data
//     int data = local_rank;
//     int recv;
//     MPI_Sendrecv(&data, 1, MPI_INT, (local_rank + 1) == comm_sz ? 0 : local_rank + 1, 0, &recv, 1, MPI_INT, (local_rank - 1) == -1 ? comm_sz - 1 : local_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//     printf("from : %d, receive : %d", local_rank, recv);
//     MPI_Finalize();
// }



/*! for question b */

void prefix_add(int* data,int n)
{
    for(int i=1;i<n;i++)
    {
        data[i]+=data[i-1];
    }
}

#ifdef USE_VECTOR_ADD
//if require that all the point of the vector has prefix_sum,please define the macro.But time comsumption will increase
void vector_add(int* data,int n,int offset)  
{
    for(int i=0;i<n;i++)
    {
        data[i]+=offset;
    }
}
#else
void vector_add(int* data,int n,int offset)
{
    data[n-1]+=offset;
}
#endif

int main(int argc,char** argv)
{
    int comm_sz,local_rank;
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&local_rank);

    const int local_n=4;
    int* local_data=(int*)malloc(local_n*sizeof(int));
    //simulate the creation of data
    srand(time(NULL));
    for(int i=0;i<local_n;i++)
    {
        local_data[i]=rand()%100;
    }
    prefix_add(local_data,local_n);
    int prefix_sum=0;
    int local_prefix_sum=local_data[local_n-1];
    //make sure that the comm_sz>2
    if(local_rank==0)
    {
        MPI_Send(&local_prefix_sum,1,MPI_INT,1,0,MPI_COMM_WORLD);
    }
    else if(local_rank==comm_sz-1)
    {
        MPI_Recv(&prefix_sum,1,MPI_INT,local_rank-1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }
    else 
    {
        MPI_Recv(&prefix_sum,1,MPI_INT,local_rank-1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        local_prefix_sum+=prefix_sum;
        MPI_Send(&local_prefix_sum,1,MPI_INT,local_rank+1,0,MPI_COMM_WORLD);
    }
    vector_add(local_data,local_n,prefix_sum);
    printf("from %d : %d\n",local_rank,local_data[local_n-1]);
    free(local_data);
    MPI_Finalize();
    
}


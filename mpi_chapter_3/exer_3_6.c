//author : manch1n
// date : 3-24
// discription : exercise 3.6 of chapter 3

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>


/**for A answer */
// int main(int argc,char** argv)
// {
//     int comm_sz,local_rank;
//     MPI_Init(NULL,NULL);
//     MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
//     MPI_Comm_rank(MPI_COMM_WORLD,&local_rank);
//     int* data=NULL;
//     const int n=14;
//     int local_n=n/comm_sz;
//     int* local_data=(int*)malloc(local_n*sizeof(int));
//     if(local_rank==comm_sz-1)
//     {
//         local_n+=n%comm_sz;  //if the remainder is not zero then add it to the last progress
//         local_data=(int*)realloc(local_data,local_n*sizeof(int));
//     }
    
//     //compute
//     if(local_rank==0)
//     {
//         data=(int*)malloc(n*sizeof(int));
//         for(int i=0;i<n;i++)
//         {
//             data[i]=i;  //simulate data input
//         }
//         for(int i=1;i<comm_sz-1;i++)
//         {
//             MPI_Send(data+local_n*i,local_n,MPI_INT,i,0,MPI_COMM_WORLD);
//         }
//         MPI_Send(data+(comm_sz-1)*local_n,local_n+n%comm_sz,MPI_INT,comm_sz-1,0,MPI_COMM_WORLD);
        
//         printf("%d received vector is :");
//         for(int i=0;i<local_n;i++)
//         {
//             printf("%d ",data[i]);
//         }
//         printf("\n");
//     }
//     else
//     {
//         MPI_Recv(local_data,local_n,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
//         printf("%d received vector is ",local_rank);
//         for(int i=0;i<local_n;i++)
//         {
//             printf("%d ",local_data[i]);
//         }
//         printf("\n");
//     }
//     MPI_Finalize();
// }



/**for b answer */
int main(int argc,char** argv)
{
    int comm_sz,local_rank;
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&local_rank);
    const int n=14;
    int local_n=0;
    int* local_data=NULL;
    int* data=NULL;
    if(local_rank==0)
    {
        data=(int*)malloc(sizeof(int)*n);
        for(int i=0;i<n;i++)
        {
            data[i]=i;
        }
    }
}
//temporarily i think loop and loop-blcok divide is useless... so i do not want to go on for now.
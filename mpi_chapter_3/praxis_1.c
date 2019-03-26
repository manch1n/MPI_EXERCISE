//author : manch1n
// date : 3-26
// discription : praxis 1 of chapter 3.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <time.h>

#define DATA_MAX 10000
#define DATA_MIN 0

void compute_task(int min,int max,int* bucket,int bucket_sz,double* local_data,int local_data_sz)
{
    int divide=0.01*(max-min)/bucket_sz;
    for(int i=0;i<local_data_sz;i++)  //due to precision lost,this may discard little float data.
    {
        for(int j=1;j<=bucket_sz;j++)
        {
            if(local_data[i]<=min*0.01+j*divide)
            {
                bucket[j-1]++;
                break;
            }
        }
    }
}

int main(int argc,char** argv)
{
    int comm_sz,local_rank;
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&local_rank);

    const int n=12;
    const int local_n=n/comm_sz;
    const int bucket_sz=4;

    int* bucket=(int*)malloc(bucket_sz*sizeof(int));
    __bzero(bucket,bucket_sz*sizeof(int));
    int* bucket_recv=(int*)malloc(bucket_sz*sizeof(int));
    __bzero(bucket,bucket_sz*sizeof(int));
    double* local_data=(double*)malloc(local_n*sizeof(double));
    double* data=NULL;

    if(local_rank==0)
    {
        srand(time(NULL));
        data=(double*)malloc(n*sizeof(double));
        for(int i=0;i<n;i++)
        {
            data[i]=(rand()%(DATA_MAX-DATA_MIN+1)+DATA_MIN)*0.01;
        }
        MPI_Scatter(data,local_n,MPI_DOUBLE,local_data,local_n,MPI_DOUBLE,0,MPI_COMM_WORLD);
        compute_task(DATA_MIN,DATA_MAX,bucket,bucket_sz,local_data,local_n);

        for(int i=0;i<bucket_sz;i++)
        {
            MPI_Reduce(&bucket[i],&bucket_recv[i],1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
        }

        for(int i=n;i>=0;i--)
        {
            printf("%d ",i);
            for(int j=0;j<bucket_sz;j++)
            {
                if(bucket_recv[j]>=i) printf("* ");
                else printf("  ");
            }
            printf("\n");
        }
    }
    else
    {
        MPI_Scatter(data,local_n,MPI_DOUBLE,local_data,local_n,MPI_DOUBLE,0,MPI_COMM_WORLD);
        compute_task(DATA_MIN,DATA_MAX,bucket,bucket_sz,local_data,local_n);
        for(int i=0;i<bucket_sz;i++)
        {
            MPI_Reduce(&bucket[i],&bucket_recv[i],1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
        }

    }
    MPI_Finalize();
}
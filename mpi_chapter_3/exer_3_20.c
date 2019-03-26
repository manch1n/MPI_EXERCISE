//author : manch1n
// date : 3-26
// discription : exercise 3.26 of chapter 3.pack and unpack,for send different type without using custom struct methods.

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc,char** argv)
{
    int local_rank,comm_sz;
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&local_rank);

    const int i_sz=4;
    const double d_sz=1;
    int* i_data=(int*)malloc(i_sz*sizeof(int));
    double* b_data=(double*)malloc(d_sz*sizeof(double));
    int position=0;
    const int data_sz=i_sz*sizeof(int)+d_sz+sizeof(double);
    void* pack_buffer=malloc(data_sz);
    if(local_rank==0)
    {
        //simulate the creation of data
        for(int i=0;i<i_sz;i++)
        {
            i_data[i]=i+1;
        }
        *b_data=1.2345;
        //you can assume that MPI_Pack is based on queue,sender push and receiver pop.
        MPI_Pack(i_data,i_sz,MPI_INT,pack_buffer,data_sz,&position,MPI_COMM_WORLD);
        MPI_Pack(b_data,1,MPI_DOUBLE,pack_buffer,data_sz,&position,MPI_COMM_WORLD);
        MPI_Bcast(pack_buffer,data_sz,MPI_PACKED,0,MPI_COMM_WORLD);
        //caution : the type should be MPI_PACKED,and the count should be the byte size.
    }
    else
    {
        MPI_Bcast(pack_buffer,data_sz,MPI_PACKED,0,MPI_COMM_WORLD);
        MPI_Unpack(pack_buffer,data_sz,&position,i_data,i_sz,MPI_INT,MPI_COMM_WORLD);
        MPI_Unpack(pack_buffer,data_sz,&position,b_data,d_sz,MPI_DOUBLE,MPI_COMM_WORLD);
        printf("from %d :\n",local_rank);
        for(int i=0;i<i_sz;i++)
        {
            printf("%d ",i_data[i]);
        }
        printf("\n%f",*b_data);
    }
    MPI_Finalize();
}
#include <stdio.h>
#include <mpi.h>
#include <math.h>

double maxval(double *arr,int m)
Add a comment to this line
 
  {
      double max=arr[0];
      for(int i=1;i<m;i++)
       {
            if (arr[i]>arr[i-1])
            {
               max=arr[i];
            }

     

     }   
  return(max);

 }


int main(int argc, char** argv)
 {
  
  int myrank, npes;
  int tagB,tagC,tagD; 
  int N=200;
 double mat[N][N];
double max,global_max;
 MPI_Init(&argc,&argv);
 MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
 MPI_Comm_size(MPI_COMM_WORLD, &npes);
 
 

  if(myrank==0)
 {
  

  FILE *file;
 file=fopen("mat.txt", "r");

  for (int i=0;i<N;i++)
  {
    for(int j=0;j<N;j++)
      {

if (!fscanf(file, "%lf", &mat[i][j]))
{
 printf(“error reading number“);

}
     }
 }

fclose(file);
double A[(N/2)*(N/2)];
double B[(N/2)*(N/2)];
double C[(N/2)*(N/2)];
double D[(N/2)*(N/2)];

 int count=0;
   for(int i=0;i<N/2;i++)
     {
      for(int j=0;j<N/2;j++)
      {

        A[count]=mat[i][j];
        count++;

      }

   }


     count=0;
   for(int i=0;i<N/2;i++)
     {
      for(int j=N/2;j<N;j++)
      {

        B[count]=mat[i][j];
        count++;

      }

   }
     
count=0;
 for(int i=N/2;i<N;i++)
     {
      for(int j=0;j<N/2;j++)
      {

        C[count]=mat[i][j];
        count++;

      }

   }

count=0;
 for(int i=N/2;i<N;i++)
     {
      for(int j=N/2;j<N;j++)
      {

        D[count]=mat[i][j];
        count++;

      }

   }


MPI_Send(B,N*N/4,MPI_DOUBLE,1,10,MPI_COMM_WORLD);
MPI_Send(C,N*N/4,MPI_DOUBLE,2,11,MPI_COMM_WORLD);
MPI_Send(D,N*N/4,MPI_DOUBLE,3,12,MPI_COMM_WORLD);

//Calculate max for root

  max=maxval(A,N*N/4);

//  printf("%lf\n",max);


}

if(myrank==1)
 {

   double B_Recv[N*N/4];
   MPI_Recv(B_Recv,N*N/4,MPI_DOUBLE,0,10,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    max=maxval(B_Recv,N*N/4);  

/*
  for(int i=0;i<N*N/4;i++)
     {
        printf("%lf \t",B_Recv[i]);
    }
*/
  //  printf("%lf \n",max);

}


if(myrank==2)
 {
      double C_Recv[N*N/4];
   MPI_Recv(C_Recv,N*N/4,MPI_DOUBLE,0,11,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
   /*
   for(int i=0;i<N*N/4;i++)
     {
        printf("%lf \t",C_Recv[i]);
    }
*/
    max=maxval(C_Recv,N*N/4);
   // printf("%lf \n",max);

 }
if(myrank==3)
 {
      double D_Recv[N*N/4];
   MPI_Recv(D_Recv,N*N/4,MPI_DOUBLE,0,12,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

     max=maxval(D_Recv,N*N/4);
    // printf("%lf\n",max);

}

//MPI_Barrier(MPI_COMM_WORLD);



MPI_Reduce(&max,&global_max,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);


if(myrank==0)
  {
   printf("max value is :%lf\n",global_max);
   
 
  for(int i=0;i<N;i++)
  {
    for(int j=0;j<N;j++)
    {
       if(mat[i][j]==global_max)
        {
              printf("x index is %d \n",i);
              printf("y index is %d \n",i);

       }
    }

 }
   

  }


MPI_Finalize();


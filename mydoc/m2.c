#include "mpi.h"
#include <stdio.h>
long N;
long i, result, sum0, sum1;

typedef unsigned long long ULONG;

long fallInCircleCount = 0;
long sumcount1 = 0;
const  double a = 1664525;
const  double c = 1013904223;

double startwtime = 0.0, endwtime;

ULONG m = 0;

double getNewDistance(double x, double y)
{
    return (x * x + y * y);
}
//  y=xn;
ULONG getMultiNum(int x, int n)
{
    ULONG y = 1;
    for (int i = 0; i < n; i++)
    {
        y = y * x;
    }

    return y;
}

double getKeyRandomNumber(double px, ULONG n) {

    ULONG py = fmod((a * px + c), m);
    if (n % 2 == 0)
    {
        return 2 * (double)py / m;
    }
    return 2 * (double)py / m - 1;

}


ULONG getRandomPoint(ULONG startI, ULONG n) {

    double x = 0;
    double y = 0;
    ULONG circlePointCount=0;
    for (ULONG i = startI; i < n; i++)
    {

        x = getKeyRandomNumber(i, n);
        y = getKeyRandomNumber(i - x, n - 1);

        if (getNewDistance(x, y) <= 1)
        {

            circlePointCount++;
        }

    }

    return circlePointCount;

}

long mySum(int numproc)
{
    // Master does its own partial sum
    sum0 = 0;
    for (i = 1; i <= N / numproc; i++) {
        sum0 = sum0 + i;
    }
    return sum0;
}

int main(argc,argv)int argc;char *argv[]; 
{
  int numproc, myid, namelen;

 
  char processor_name[MPI_MAX_PROCESSOR_NAME];

  MPI_Status Stat;//status variable, so operations can be checked

  MPI_Init(&argc,&argv);//INITIALIZE
  MPI_Comm_size(MPI_COMM_WORLD, &numproc); //how many processors??
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);    //what is THIS processor-ID?

  //what is THIS processor name (hostname)?
  MPI_Get_processor_name(processor_name,&namelen);
  fprintf(stdout, "Processor ID = %d: %s %d\n", myid, processor_name, numproc);

  // Do this if master
  if (myid == 0) {

    N = atoi(argv[1]);
    startwtime = MPI_Wtime();

    m = getMultiNum(2, 32);

    // Master sends N to all the slave processes
    for  (i=1;i<numproc;i++) {
      MPI_Send(&N, 1, MPI_LONG, i,0, MPI_COMM_WORLD);
    }	

    fallInCircleCount = getRandomPoint(0,N/numproc);
   

    for (i=1;i<numproc;i++) {//receive from all nodes
     
        MPI_Recv(&sumcount1, 1, MPI_LONG, i,0, MPI_COMM_WORLD, &Stat);
      
        fallInCircleCount = fallInCircleCount + sumcount1;//adds the various sums
      
        fprintf(stdout, "node=%ld: %lld %ld\n", i, fallInCircleCount, sumcount1);
    }
    endwtime = MPI_Wtime();

    fprintf(stdout,"Throwing times: %ld\n Hit times: %lld \n elapse time: %f\n",N, fallInCircleCount, endwtime-startwtime);
  } 

  else {//this is not the master
    MPI_Recv(&N, 1, MPI_LONG, 0, 0, MPI_COMM_WORLD, &Stat);
    long scircleCount = 0;
    for(i=(N/numproc*myid)+1;i<=(N/numproc*(myid+1));i++){
        scircleCount = scircleCount + getRandomPoint(i,(N / numproc * (myid + 1)));
    }	
    MPI_Send(&scircleCount, 1, MPI_LONG, 0, 0, MPI_COMM_WORLD);
  }


  MPI_Finalize();
}

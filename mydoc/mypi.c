//first.c Adding numbers using two nodes

#include "mpi.h"
#include <stdio.h>
long N;
long i;

typedef unsigned long long ULONG;



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
    int i;
    for (i=0;i < n; i++)
    {
        y = y * x;
    }

    return y;
}

double getKeyRandomNumber(double px) {

    ULONG py = fmod((a * px + c), m);
/*    if (n % 2 == 0)
    {
        return 2 * (double)py / m;
    }*/
    return 2 * (double)py / m - 1;

}


long getRandomPoint(int startI, long n) {

    double x = 0;
    double y = 0;
    long circlePointCount=0;

    for (i = startI; i < n; i++)
    {

        x = getKeyRandomNumber(i);
        y = getKeyRandomNumber(i - x);

        if (getNewDistance(x, y) <= 1)
        {

            circlePointCount++;
        }

    }

    return circlePointCount;

}
//always use argc and argv, as mpirun will pass the appropriate parms.
int main(argc,argv)int argc;char *argv[];
{
    int N,i,result,sum0,sum1,myid;
    long sumcount1 = 0;
    long sumcount0 = 0;
    long fallInCircleCount = 0;

    // Status variable so that operations can be checked
    MPI_Status stat;

    // Initialize - must do this before calling any other MPI function
    MPI_Init(&argc,&argv);

    // Which node is this?
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    // Do this block if master process
    if (myid == 0) {

        // Get the number from the user
        N=atoi(argv[1]);

        // Master sends 'N' to slave
        MPI_Send(&N, 1, MPI_INT, 1,0, MPI_COMM_WORLD);

            sumcount0 = getRandomPoint(0,N/2);
        fallInCircleCount = fallInCircleCount + sumcount0;
       printf("主线程 sum0: %ld",sumcount0);
        // Master waits to receive 'sum1' from slave
        MPI_Recv(&sumcount1, 1, MPI_INT, 1,0, MPI_COMM_WORLD, &stat);
        fallInCircleCount = fallInCircleCount + sumcount1;

        printf("主线程 接收的sum1: %ld",sumcount1);
        fprintf(stdout,"总击中次数： %ld \n",fallInCircleCount);
    }

        // Do this block if slave process
    else if (myid == 1) {

        // Slave waits to receive 'N' from master
        MPI_Recv(&N, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);

        // Partial result for the slave
        sumcount1 = getRandomPoint((N/2)+1,N);
        printf("奴隶线程击中次数：%ld ",sumcount1);
        // Slave sends 'sum1' to master
        MPI_Send(&sumcount1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}

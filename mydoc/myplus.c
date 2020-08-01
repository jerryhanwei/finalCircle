//first.c Adding numbers using two nodes

#include "mpi.h"
#include <stdio.h>

typedef unsigned long long ULONG;
ULONG m = 0;
double getNewDistance(double x, double y)
{
    return (x*x + y*y);
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

    const  double a = 1664525;
    const  double c = 1013904223;
    ULONG py = fmod((a * px + c), m);

    return 2 * (double)py / m - 1;

}


long getFallInCircleCount(int startI, int n) {

    double x = 0;
    double y = 0;
    long circlePointCount=0;
    int i;
    for (i = startI; i < n; i++)
    {
        x = getKeyRandomNumber(i);
        y = getKeyRandomNumber(i - x);
       // printf("x: %f  y: %f",x,y);
        if (getNewDistance(x, y) <= 1)
        {
           // printf("中了 %d\n",i);

            circlePointCount++;
        }

    }

    return circlePointCount;

}

int myplus(int startI,int n){

    int sum =0;
    int i;
    for(i=startI;i<n;i++)
    {
        sum = sum + i;
    }
    return sum;
}
//always use argc and argv, as mpirun will pass the appropriate parms.
int main(argc,argv)int argc;char *argv[];
{
    m = getMultiNum(2,32);
    int N,i,result,sum0,sum1,myid;
    long allcount=0;
    long masterCount;
    long slaveCount;
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

        //sum0=myplus(0,N/2);
         masterCount = getFallInCircleCount(0,N/2);
        printf(" 主线程 击中次数 ：%ld",masterCount);
        // Master waits to receive 'sum1' from slave
        MPI_Recv(&slaveCount, 1, MPI_LONG, 1,0, MPI_COMM_WORLD, &stat);

        printf(" 主线程 接收奴隶线程 击中次数 ：%ld\n",slaveCount);
        allcount = masterCount + slaveCount;
        double PI = (((double)allcount) / N) * 4;
        printf("一共投掷：%d\n  击中了 -- %ld\n PI:---%f",N,allcount,PI);

        //fprintf(stdout,"投掷总次数：%d\n，共击中次数：%ld\n",N,allcount);
    }

        // Do this block if slave process
    else if (myid == 1) {

        // Slave waits to receive 'N' from master
        MPI_Recv(&N, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);

        slaveCount = getFallInCircleCount((N/2)+1,N);
        printf(" 奴隶线程 击中次数 ：%ld",slaveCount);
        // Slave sends 'sum1' to master
        MPI_Send(&slaveCount, 1, MPI_LONG, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}
//
// Created by wayne on 2020/8/1.
//


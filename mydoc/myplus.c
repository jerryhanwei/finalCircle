//first.c Adding numbers using two nodes

#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

double getNewDistance(double x, double y)
{
    return (x * x + y * y);
}

//  y=xn;
long getMultiNum(int x, int n)
{
    long y = 1;
    int i;
    for (i = 0; i < n; i++)
    {
        y = y * x;
    }

    return y;
}

double getKeyRandomNumber(double px) {

    const  double a = 1664525;
    const  double c = 1013904223;
    const  double m = 4294967296;
    long py = fmod((a * px + c), m);


    return 2 * (double)py / m - 1;

}


long getFallInCircleCount(int startI, int n) {

    double x = 0;
    double y = 0;
    long circlePointCount = 0;
    int i;
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
int main(argc, argv)int argc; char* argv[];
{

    long N;
    int i, numproc, myid;
    long allcount;
    long masterCount;
    long slaveCount;
    int namelen;
    // Status variable so that operations can be checked
    MPI_Status stat;

    // Initialize - must do this before calling any other MPI function
    MPI_Init(&argc, &argv);

    // Which node is this?
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Comm_size(MPI_COMM_WORLD, &numproc); //how many processors??

    //what is THIS processor name (hostname)?
    MPI_Get_processor_name(processor_name, &namelen);
    fprintf(stdout, "Processor myid = %ld: p_name:%s 处理器数量：%ld\n", myid, processor_name, numproc);

    // Do this block if master process
    if (myid == 0) {

        // Get the number from the user
        N = atoi(argv[1]);

        for (i = 1; i < numproc; i++) {
            MPI_Send(&N, 1, MPI_LONG, i, 0, MPI_COMM_WORLD);
        }


        masterCount = getFallInCircleCount(0, N / numproc);
        printf(" 主线程 击中次数 ：%ld", masterCount);

        for (i = 1; i < numproc; i++) {//receive from all nodes

            MPI_Recv(&slaveCount, 1, MPI_LONG, i, 0, MPI_COMM_WORLD, &stat);

            allcount = allcount + slaveCount;//adds the various sums

            fprintf(stdout, "node=%ld: 目前 总计 %ld  当前进程击中点数： %ld\n", i, allcount, slaveCount);
        }



        // printf(" 主线程 接收奴隶线程 击中次数 ：%ld\n",slaveCount);
        allcount = masterCount + allcount;
        double PI = (((double)allcount) / N) * 4;
        printf("一共投掷：%ld\n  击中了 -- %ld\n PI:---%f", N, allcount, PI);


    }

        // Do this block if slave process
    else {

        // Slave waits to receive 'N' from master
        MPI_Recv(&N, 1, MPI_LONG, 0, 0, MPI_COMM_WORLD, &stat);
        long oneSlaveCount = 0;
        for (i = (N / numproc * myid) + 1; i <= (N / numproc * (myid + 1)); i++) {
            oneSlaveCount = oneSlaveCount + getFallInCircleCount(i, (N / numproc * (myid + 1)));
        }

        // slaveCount = getFallInCircleCount((N/2)+1,N);
        printf(" 奴隶线程 %ld 击中次数 ：%ld", myid, oneSlaveCount);
        // Slave sends 'sum1' to master
        MPI_Send(&oneSlaveCount, 1, MPI_LONG, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}
//
// Created by wayne on 2020/8/1.
//


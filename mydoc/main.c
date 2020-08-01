
#include <stdio.h>
#include <math.h>
#include <time.h>
typedef unsigned long long ULONG;

ULONG fallInCircleCount = 0;

const  double a = 1664525;
const  double c = 1013904223;
ULONG m=0;


double getNewDistance(double x, double y)
{
    return (x*x + y*y);
}
//  y=xn;
ULONG getMultiNum(int x,int n)
{
    ULONG y = 1;
    for (int i = 0; i < n; i++)
    {
        y = y * x;
    }

    return y;
}

double getKeyRandomNumber(double px,ULONG n) {

    ULONG py = fmod((a * px + c), m);
    if (n % 2 == 0)
    {
        return 2 * (double)py / m;
    }
    return 2*(double)py / m-1;

}

void getRandomPoint(ULONG n) {

    double x=0;
    double y=0;

    for (ULONG i = 0; i < n; i++)
    {

        x = getKeyRandomNumber(i,n);
        y = getKeyRandomNumber(i-x,n-1);

        if (getNewDistance(x,y) <= 1)
        {

            fallInCircleCount++;
        }

    }

}



void main(void)
{
    long i = 10000;
    long k = 10000;
    printf("%ld",i+k);
    m = getMultiNum(2, 32);
    ULONG N = 10;

    clock_t start, stop;
    start = clock();
    getRandomPoint(N);

    /* 结束计时 */
    stop = clock();

    /* 计算运行时间 */
    double duration = ((double) (stop - start))/CLK_TCK;

    /* 打印运行时间 */
    printf("运行时间：%f\n", duration);

    double PI = (((double)fallInCircleCount) / N) * 4;

    printf("fallInCircleCount:%lld\n",fallInCircleCount);

    printf("PI :%f\n", PI);

    // 149999620/300000000

}



/*
 this is a simple serial program which computes the dot product of two vectors. The threaded version is pthread_dotprod_mutex.c
 * */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    double  *a;
    double  *b;
    double  sum;
    int     veclen;
} DOTDATA;

#define VECLEN  100000
DOTDATA dotstr;

void dotprod()
{
    int start, end, i;
    double mysum, *x, *y;

    start = 0;
    end = dotstr.veclen;
    x = dotstr.a;
    y = dotstr.b;

    mysum = 0;
    for (i=start; i<end; i++) {
        mysum += (x[i]*y[i]);
    }
    dotstr.sum = mysum;
}

int main(int argc, char **argv)
{
    int i, len;
    double *a, *b;

    len = VECLEN;
    a = (double *)malloc(len*sizeof(double));
    b = (double *)malloc(len*sizeof(double));

    for (i=0; i<len; i++) {
        a[i] = 1;
        b[i] = a[i];
    }

    dotstr.veclen = len;
    dotstr.a = a;
    dotstr.b = b;
    dotstr.sum = 0;

    dotprod();

    printf("Sum = %f\n", dotstr.sum);
    free(a);a=NULL;
    free(b);b=NULL;

    return 0;
}


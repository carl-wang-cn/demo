/*
 this example illustrates how to print out the backtrace of calling-functions.
 For systems using the GNU linker, it it necessary to use the -rdynamic linker option.
 Note, "static" function will not be exposed, and won't be available in the backtrace
 */
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void myfunc3(void)
{
    int j, nptrs;
#define SIZE 100
    void *buffer[SIZE];
    char **strings;

    nptrs = backtrace(buffer, SIZE);
    printf("backtrace() returned %d addresses\n", nptrs);

    strings = backtrace_symbols(buffer, nptrs);
    if (NULL == strings) {
        perror("backtrace_symbols");
        exit(EXIT_FAILURE);
    }

    for (j=0; j<nptrs; j++) {
        printf("%s\n", *(strings+j));
    }

    free(strings);
}

static void myfunc2(void) //this will not be printed out
{
    myfunc3();
}

void myfunc(int ncalls)
{
    if (1 < ncalls) {
        myfunc(ncalls-1);
    }
    else {
        myfunc2();
    }
}

int main(int argc, char **argv)
{
    if (2 != argc) {
        fprintf(stderr, "%s num-calls\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    myfunc(atoi(argv[1]));
    exit(EXIT_SUCCESS);
}

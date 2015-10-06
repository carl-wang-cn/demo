#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void myfunc3(void)
{
    int j, nptrs;
#define SIZE 100
    void *buffer[100];
    char **string;

    nptrs = backtrace(buffer, SIZE);
    printf("backtrace() returned %d addresses\n", nptrs);

//    backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO);
    string = backtrace_symbols(buffer, nptrs);
    for (j = 0; j < nptrs; j++)
    {
        printf("%s\n", string[j]);
    }
    free(string);
}

void myfunc(void)
{
    myfunc3();
}

int main(int argc, char **argv)
{
    myfunc();
    return 0;
}

#include <stdio.h>
#include <stdlib.h>

static void my_exit1(void);
static void my_exit2(void);

int main(int argc, char **argv)
{
    if (atexit(my_exit2) != 0)
    {
        printf("register exit2 error\n");
        exit(1);
    }
    if (atexit(my_exit1) != 0)
    {
        printf("register exit1 error 1\n");
        exit(2);
    }
    if (atexit(my_exit1) != 0)
    {
        printf("register exit1 error 2\n");
        exit(3);
    }

    printf("main is done\n");
    return 0;
}

static void my_exit1(void)
{
    printf("my exit1 handler\n");
}

static void my_exit2(void)
{
    printf("my exit2 handler\n");
}

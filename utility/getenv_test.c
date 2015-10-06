#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    char *value = NULL;

    if (argc != 2)
    {
        printf("pls usage like program argv\n");
        return 0;
    }
    value = getenv(argv[1]);
    if (value != NULL)
    {
        printf("getenv:%s = %s\n", argv[1], value);
    }

    return 0;
}

#include <unistd.h>
#include <stdio.h>

#define CWD getcwd(NULL, 0)
int main()
{
    printf("%s%c%s\n", CWD, '/', "hello");

    return 0;
}

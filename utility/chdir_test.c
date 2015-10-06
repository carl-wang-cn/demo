#include <unistd.h>
#include <stdio.h>


int main(void)
{
    char cur_working_dir[256] = {0};

    //get current working directory, getcwd return buf(cur_working_dir) if success, else return NULL
    if (NULL == getcwd(cur_working_dir, sizeof(cur_working_dir)))
    {
        printf("getcwd error\n");
        return -1;
    }
    printf("current working dir:%s\n", cur_working_dir);

    //change current working directory to buf '/tmp'
    if (0 != chdir("/tmp"))
    {
        printf("chdir error\n");
        return -2;
    }

    if (NULL == getcwd(cur_working_dir, sizeof(cur_working_dir)))
    {
        printf("getcwd error 2\n");
        return -1;
    }
    printf("after chdir, current working dir:%s\n", cur_working_dir);

    return 0;
}

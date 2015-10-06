/*================================================================
*   Copyright (C) 2014 All rights reserved.
*   
*   FileName：getopt_test.c
*   Author  ：Carl
*   Creat Date：2014.03.06
*   Description：
*
================================================================*/
#include <stdio.h>
#include <unistd.h>


int main(int argc, char **argv)
{
    char o;
    while (-1 != (o = getopt(argc, argv, "f:h")))
    {
        switch (o)
        {
        case 'f':
            printf("option f\n");
            printf("f followed by:%s\n", optarg);
            break;
        case 'h':
            printf("option h\n");
            break;
        default:
            printf("o is %c\n", o);
            break;
        }
    }

    return 0;
            

}

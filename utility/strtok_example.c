#include <stdio.h>
#include <string.h>


int main(int argc, char **argv)
{
    char token_str[] = "hello world,this is a token example.Yes";
    //char *token_str = "hello world,this is a token example.Yes"; this will not work
    char *pch = NULL;
    const char *delim = ", .";

    printf("%s\n", token_str);

    //this interface will cast the delim char to '\0'
    pch = strtok(token_str, delim); 
    while (pch != NULL)
    {
        printf("%s\n", pch);
        pch = strtok(NULL, delim);
    }
    //ps: the original str is modified!!!!!
    printf("%s\n", token_str);

    return 0;
}

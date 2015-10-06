#include <stdio.h>
#include <setjmp.h>

#define TOK_ADD 5
#define MAXLINE 40

jmp_buf jmpbuffer;
char *tok_ptr;

void do_line(char *ptr);
void cmd_add(void);
int  get_token(void);

int main(int argc, char **argv)
{
    char line[MAXLINE];
    int  ret = 0;

    if ((ret = setjmp(jmpbuffer)) != 0) //类似于goto所用的tag，告诉longjmp应该返回到哪里
    {
        printf("error:%d\n", ret);
    }

    while (fgets(line, MAXLINE, stdin) != NULL)
    {
        do_line(line);
    }

    return 0;
}



void do_line(char *ptr)
{
    int cmd;

    tok_ptr = ptr;
    while ((cmd = get_token()) > 0)
    {
        switch (cmd)
        case TOK_ADD:
            cmd_add();
            printf("cmd add complete\n"); //this will not be printed out
            break;
    }
}

void cmd_add(void)
{
    int token;
    token = get_token();
    if (token > 0)
    {
        longjmp(jmpbuffer, 2); //第2个参数“2”，会是setjmp的返回值
    }
}

int get_token(void)
{
    return TOK_ADD; 
}

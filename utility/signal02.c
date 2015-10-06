#include <unistd.h>
#include <stdio.h>
#include <signal.h>

void sig_handler(int sig)
{
    printf("\nouch, got a signal:%d\n", sig);
    signal(sig, SIG_DFL);//不恢复的话，ctrl + c就停不了程序了
}


int main()
{
    struct sigaction act = {};
    act.sa_handler = sig_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESETHAND;

    sigaction(SIGINT, &act, 0);
    while (1)
    {
        printf("hello world\n");
        sleep(1);
    }

    return 0;
}

#include <unistd.h>
#include <stdio.h>
#include <signal.h>

static int alarm_fired = 0;

void sig_handler(int sig)
{
    printf("\nouch, got a signal:%d\n", sig);
    alarm_fired = 1;
}


int main()
{
    signal(SIGALRM, sig_handler);
    alarm(5);
    pause();

    if (alarm_fired == 1)
        printf("\ngot a signal\n");

    return 0;
}

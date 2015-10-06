#include <unistd.h>
#include <stdio.h>
#include <signal.h>

void sig_handler(int sig)
{
    printf("\nouch, got a signal:%d\n", sig);
}


int main()
{
    sigset_t sigset = {};
    sigset_t ign_set = {};
    struct sigaction act = {};

    sigemptyset(&sigset);
    sigemptyset(&ign_set);
    sigaddset(&sigset, SIGINT);

    act.sa_handler= sig_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, 0);

    printf("waiting for the signal SIGINIT.......\n");
    pause();

    sigprocmask(SIG_SETMASK, &sigset, 0);
    printf("press ctrl C in 10 s....\n");
    sleep(10);

    sigpending(&ign_set);
    if (sigismember(&ign_set, SIGINT))
        printf("SIGINT is ignored\n");

    sigdelset(&sigset, SIGINT);
    printf("wait for the SIGINT signal\n");
    sigsuspend(&sigset);

    printf("exit in 5 s\n");
    sleep(5);

    return 0;


    return 0;
}

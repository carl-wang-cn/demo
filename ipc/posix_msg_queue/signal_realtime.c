#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <assert.h>

typedef void (sigfunc_rt)(int, siginfo_t *, void *);
static void sig_rt(int signo, siginfo_t *info, void *context);
sigfunc_rt *signal_rt(int signo, sigfunc_rt *func, sigset_t *mask);

int main(int argc, char **argv)
{
    int i, j;
    pid_t pid;
    sigset_t newset;
    union sigval val;

    printf("SIGRTMIN=%d, SIGRTMAX=%d\n", (int)SIGRTMIN, (int)SIGRTMAX);

    if ((pid = fork()) == 0)
    {
        sigemptyset(&newset);
        sigaddset(&newset, SIGRTMAX);
        sigaddset(&newset, SIGRTMAX-1);
        sigaddset(&newset, SIGRTMAX-2);
        sigprocmask(SIG_BLOCK, &newset, NULL); // block signal SIGRTMAX SIGRTMAX-1 SIGRTMAX-2

        signal_rt(SIGRTMAX, sig_rt, &newset); // signal_rt set flag SA_SIGINFO
        signal_rt(SIGRTMAX-1, sig_rt, &newset);
        signal_rt(SIGRTMAX-2, sig_rt, &newset);

        sleep(6); // let parent send 9 signals

        sigprocmask(SIG_UNBLOCK, &newset, NULL);
        sleep(3); // let sig_rt call printf 9 times
        exit(0);
    }

    // parent sends 9 signals to child
    sleep(3); // let child block all signals
    printf("SIQUEUE=%d\n", SI_QUEUE);
    for (i=SIGRTMAX; i>=SIGRTMAX-2; i--)
    {
        for (j=0; j<=2; j++)
        {
            val.sival_int = j;
            sigqueue(pid, i, val); // send signals; we expect that the smaller signal should be sent first
            printf("sent signal %d, val = %d\n", i, j);
        }
    }

    exit(0);
}

static void sig_rt(int signo, siginfo_t *info, void *context)
{
    // code==-1==SI_QUEUE
    printf("received signal #%d, code = %d, ival = %d\n", 
            signo, info->si_code, info->si_value.sival_int);
}


sigfunc_rt *signal_rt(int signo, sigfunc_rt *func, sigset_t *mask)
{
    struct sigaction act, oact;

    act.sa_sigaction = func; // must store function addr here
    act.sa_mask = *mask;     // signals to block
    act.sa_flags = SA_SIGINFO; // must specify this for realtime
    if (SIGALRM == signo)
    {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;
#endif
    }
    else
    {
#ifdef SA_RESTART
        act.sa_flags |= SA_RESTART;
#endif
    }

    if (sigaction(signo, &act, &oact) < 0)
    {
        return ((sigfunc_rt *)SIG_ERR);
    }

    return oact.sa_sigaction;
}


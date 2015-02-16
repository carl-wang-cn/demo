#ifndef PRO_CON_H
#define PRO_CON_H
#include <semaphore.h>

#define MESGSIZE 256   // max #bytes per message
#define NMESG    16    // max #message

struct shmstruct // struct stored in shared memory
{
    sem_t mutex;
    sem_t nempty;
    sem_t nstored;
    int nput;  // index into msgoff[] for next put
    long noverflow;  // #overflows by senders
    sem_t noverflowmutex; // mutex for noverflow counter
    long msgoff[NMESG];   // offset in shared memory of each message
    char msgdata[NMESG * MESGSIZE];  // the actual messages
};


#endif

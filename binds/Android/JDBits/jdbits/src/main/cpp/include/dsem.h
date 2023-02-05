#ifndef __DSEM_H

#include <pthread.h>
#include <semaphore.h>

// Max number of conccurent threads ( sem max value )
#define MAX_THREADS_N 10

#ifdef __cplusplus
extern "C"
{
#endif

    static struct _table_sem_t
    {

        pthread_mutex_t _lock;
        sem_t _register_sem;
        sem_t _getter_sem;
        signed int _csem;
    } _table_sem;

    /**
     * @brief Initialize Serialization primitives used to synchronize
     * PacketTable access across multiple threads.
     * This function must be called BEFORE any other library function.
     *
     * @return 1 on success, 0 in case of
     */
    extern unsigned char InitLocks(void);

    void _register_wait(void);

    void _getter_wait(void);

    void _register_post(void);

    void _getter_post(void);

#ifdef __cplusplus
}
#endif
#define __DSEM_H
#endif // __DSEM_H

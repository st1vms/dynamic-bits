#include "dsem.h"

unsigned char InitLocks(void)
{
    if (0 == pthread_mutex_init(&(_table_sem._lock), NULL) &&
        0 == sem_init(&(_table_sem._register_sem), 0, MAX_THREADS_N) &&
        0 == sem_init(&_table_sem._getter_sem, 0, MAX_THREADS_N))
    {
        _table_sem._csem = 0;
        return 1;
    }
    return 0;
}

void _register_wait()
{
    sem_wait(&(_table_sem._register_sem));
    pthread_mutex_lock(&(_table_sem._lock));
    while (_table_sem._csem > 0)
    {
        ;
    }
}

void _getter_wait()
{
    sem_wait(&(_table_sem._getter_sem));
    pthread_mutex_lock(&(_table_sem._lock));

    if (_table_sem._csem < MAX_THREADS_N)
    {
        _table_sem._csem += 1;
    }
}

void _register_post()
{
    _table_sem._csem = 0;
    sem_post(&(_table_sem._getter_sem));
    pthread_mutex_unlock(&(_table_sem._lock));
}

void _getter_post()
{
    if (_table_sem._csem > 0)
    {
        _table_sem._csem -= 1;
    }
    sem_post(&(_table_sem._register_sem));
    pthread_mutex_unlock(&(_table_sem._lock));
}

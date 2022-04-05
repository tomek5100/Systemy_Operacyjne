#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include "semaphores.h"

sem_t * semaphore_init(char *name, int val)
{
    sem_t * semaphore = sem_open(name,O_CREAT | O_EXCL,0777,val);
    if (semaphore == SEM_FAILED)
    {
        perror("Sem open with 4 arguments failed ");
        _exit(1);
    }
    return semaphore;
}
sem_t * semaphore_open(char *name)
{
    sem_t *semaphore = sem_open(name,O_RDWR);
    if(semaphore==SEM_FAILED)
    {
        perror("Sem open with 2 arguments failed");
        _exit(6);
    }
    return semaphore;
}
void semaphore_wait(sem_t * semaphore)
{
    if (sem_wait(semaphore) < 0) 
        {
            perror("sem wait failed on child");
            _exit(5);
        }
}
void semaphore_post(sem_t * semaphore)
{
    if (sem_post(semaphore) < 0) 
        {
            perror("sem post failed on child");
            _exit(5);
        }
}
void semaphore_close(sem_t * semaphore)
{
    if (sem_close(semaphore) < 0)
    {
        perror("Sem close failed");
        exit(7);
    }
}
void semaphore_unlink(char *name)
{
    if(sem_unlink(name)<0)
    {
        perror("Sem unlink error");
        exit(8);
    }
}
int semaphore_value(sem_t * semaphore)
{
    int value;
    if(sem_getvalue(semaphore,&value)==-1)
    {
        perror("Sem get value error");
        _exit(9);
    }
    return value;
}
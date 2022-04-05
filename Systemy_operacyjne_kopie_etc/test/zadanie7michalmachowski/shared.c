#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>

int sminit(char *name)
{
    int fd;
    fd = shm_open(name,O_RDWR | O_CREAT | O_EXCL,0666);
    if(fd==-1)
    {
        perror("Shm_open error");
        _exit(1);
    }
    return fd;
}
int smopen(char *name)
{
    int fd;
    fd = shm_open(name,O_RDWR,0666);
    if(fd==-1)
    {
        perror("Shm_open error");
        _exit(1);
    }
    return fd;
}

void smsetlength(int fd, int len)
{
    if(ftruncate(fd,len)==-1)
    {
        perror("Ftruncate error");
        exit(2);
    }
}

void smclose(int fd)
{
    if(close(fd)==-1)
    {
        perror("Close error");
        exit(3);
    }
}

void smunlink(char *name)
{
    if(shm_unlink(name)==-1)
    {
        perror("Shm_unlink error");
        _exit(4);
    }
}

char * smmap(int len, int fd)
{
    void * ptr;
    ptr = mmap(NULL,len,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    if(ptr==MAP_FAILED)
    {
        perror("mmap error");
        exit(5);
    }
    return ptr;
}

void smunmap(void *ptr, int len)
{
    if(munmap(ptr,len)==-1)
    {
        perror("munmap error");
        exit(5);
    }
}
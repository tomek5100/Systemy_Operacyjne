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
#include "semaphores.h"
#include "shared.h"

char *nameprod;
char *namecons;
char *namemem;

#define buf 20 //Rozmiar elementu
#define bufsize 5 //Ilosc elementow

typedef struct //Baza do pamieci dzielonej
    {
        char bufor[bufsize][buf];
    }SegmentPD;

void done()
{
    smunlink(namemem);
    semaphore_unlink(nameprod);
    semaphore_unlink(namecons);
    printf("Unlinked shared memory and two semaphores\n");
}
void my_handler(int signal_id)
{
    printf("Interrupted\n");
    exit(2);
}

int main(int argc, char *argv[])
{
    if(atexit(done)!=0) //Atexit do usuwania
    {
        perror("atexit error");
        _exit(2);
    }
    if(signal(SIGINT,my_handler)==SIG_ERR) //Obsluga sygnalu ctrl+c
    {
        perror("Problem with custom resolve of a signal");
        exit(4);
    }
    if(argc!=8)
    {
        printf("Wrong number of arguments!\n");
        _exit(1);
    }
    namemem = argv[1];
    nameprod = argv[5];
    namecons = argv[7];
    //return 0;
    int fd;
    fd = sminit(namemem); //Tworzenie pamieci dzielonej
    printf("Opened shared memory with descriptor %d and size %ld\n",fd,sizeof(SegmentPD)); 
    smsetlength(fd,sizeof(SegmentPD)); //Ustalanie dlugosci
    sem_t * sem1;
    sem_t * sem2;
    sem1 = semaphore_init(nameprod,bufsize); //Inicjowanie semafora producenta
    printf("Created semaphore named %s with address %p and initial value of %d\n",nameprod,(void *)sem1,semaphore_value(sem1));
    sem2 = semaphore_init(namecons,0); //Inicjowanie semafora konsumenta
    printf("Created semaphore named %s with address %p and initial value of %d\n",namecons,(void *)sem2,semaphore_value(sem2));
    switch(fork())
    {
        case -1:
        {
            perror("Fork error");
            exit(8);
        }
        case 0:
        {
            //printf("About to go to producer\n");
            execl(argv[2],argv[2],argv[1],argv[4],argv[5],argv[7],NULL);
            perror("Exec error");
            exit(9);
        }
        default:
        {

        }
    }
    switch(fork())
    {
        case -1:
        {
            perror("Fork error");
            exit(8);
        }
        case 0:
        {
            //printf("About to go to consumer\n");
            execl(argv[3],argv[3],argv[1],argv[6],argv[7],argv[5],NULL);
            perror("Exec error");
            exit(9);
        }
        default:
        {
            
        }
    }
    // Czekanie na procesy
    if(wait(NULL)==-1)
    {
        perror("Wait error");
        exit(14);
    }
    if(wait(NULL)==-1)
    {
        perror("Wait error");
        exit(14);
    }
    return 0;
}

// 1 - nazwa pamieci dzielonej
// 2 - nazwa producenta
// 3 - nazwa konsumenta
// 4 - plik tekstowy pierwotny
// 5 - nazwa semafora producenta
// 6 - plik tekstowy docelowy
// 7 - nazwa semafora konsumenta

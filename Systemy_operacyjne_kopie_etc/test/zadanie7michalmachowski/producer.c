#define _DEFAULT_SOURCE
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
#include <time.h>
#include "semaphores.h"
#include "shared.h"

#define buf 20 //Rozmiar elementu
#define bufsize 5 //Ilosc elementow

typedef struct //Baza taka sama jak w main
    {
        char bufor[bufsize][buf];
    }SegmentPD;

int main(int argc, char *argv[])
{
    //printf("Hello from producer!\n");
    if(argc!=5)
    {
        printf("Wrong number of arguments in producer\n");
        _exit(11);
    }
    srand(time(NULL)+getpid());
    int fd;
    fd = smopen(argv[1]); //Otwarcie pamieci dzielonej
    SegmentPD *wpd = (SegmentPD *) smmap(sizeof(SegmentPD),fd); //Mapowanie pamieci
    sem_t * semp = semaphore_open(argv[3]); //Otwarcie semafora producenta
    sem_t * semc = semaphore_open(argv[4]); //Otwarcie semafora konsumenta
    int desc = open(argv[2],O_RDONLY); //Otwarcie pliku
    if(desc==-1)
    {
        perror("Open error");
        _exit(10);
    }
    int r;
    int iter=0;
    do
    {
        semaphore_wait(semp); 
        printf("semp  = %d , semc  = %d , ",semaphore_value(semp),semaphore_value(semc));
        memset(wpd->bufor[iter],'\0',buf); //Oproznienie elementu bufora
        r = read(desc,wpd->bufor[iter],buf); //Czytanie z pliku i wpisanie do pamieci
        if(r==-1)
        {
            perror("Read error");
            _exit(12);
        }
        wpd->bufor[iter][r]='\0';
        printf("R = %d , iter = %d, text = %s\n",r,iter,wpd->bufor[iter]);
        iter = (iter+1)%bufsize; //Iteracja
        usleep((1+rand()%5)*100000); //Generacja "opoznien"
        semaphore_post(semc);

    } while (r==20);
    smunmap(wpd,sizeof(SegmentPD));
    if(close(fd)==-1)
    {
        perror("Close error");
        _exit(16);
    }
    if(close(desc)==-1)
    {
        perror("Close error");
        _exit(16);
    }
    return 0;
}

// 1 - nazwa pamieci dzielonej
// 2 - plik tekstowy
// 3 - nazwa semafora producenta
// 4 - nazwa semafora konsumenta
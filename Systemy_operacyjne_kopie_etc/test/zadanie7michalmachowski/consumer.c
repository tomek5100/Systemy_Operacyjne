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
    //printf("Hello from consumer!\n");
    if(argc!=5)
    {
        printf("Wrong number of arguments in consumer\n");
        _exit(11);
    }
    srand(time(NULL)+getpid());
    int fd;
    fd = smopen(argv[1]); //Otwarcie pamieci dzielonej
    SegmentPD *wpd = (SegmentPD *) smmap(sizeof(SegmentPD),fd); //Mapowanie pamieci
    sem_t * semp = semaphore_open(argv[4]); // Otwarcie semafora producenta
    sem_t * semc = semaphore_open(argv[3]); //Otwarcie semafora konsumenta
    int desc = open(argv[2],O_WRONLY | O_TRUNC); //Otwarcie pliku docelowego
    if(desc==-1)
    {
        perror("Open error");
        _exit(10);
    }
    int w;
    int iter=0;
    do
    {
        semaphore_wait(semc);
        w = 0;
        for(int i=0;i<20;i++) //Ta petla sprawdza ile znakow przed \0 jest
        {
            if(wpd->bufor[iter][i]!='\0')
            {
                w++;
            }
            else
                break;
        }
        printf("    semp = %d , semc = %d  ,",semaphore_value(semp),semaphore_value(semc));
        if(write(desc,wpd->bufor[iter],w)==-1) //Pisanie z pamieci do pliku
        {
            perror("Read error");
            _exit(12);
        }
        printf("W = %d , iter = %d , text = %s\n",w,iter,wpd->bufor[iter]);
        iter = (iter+1)%bufsize; // Iteracja
        usleep((1+rand()%5)*100000); //Generacja "opoznien"
        semaphore_post(semp);

    } while (w==20);
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

// 1 - plik tekstowy
// 2 - nazwa semafora
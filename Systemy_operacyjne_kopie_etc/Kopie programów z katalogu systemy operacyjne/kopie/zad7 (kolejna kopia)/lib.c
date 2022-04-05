/* 
   Program do tworzenia, otwierania, uzyskiwania wartosci, operowania,
   zamykania i usuwania semafora.
   --------------------------------------------------------------------
   Autor: Tomasz Rozycki                                    11.05.2021
   --------------------------------------------------------------------
*/

//makra

//biblioteki dolaczane
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <semaphore.h>
#include "lib.h"

void tworzenie(const char *name, int value){
    if((sem_open(name, O_CREAT | O_EXCL, 0777, value)) == SEM_FAILED){ //tworze semafor nazwany o wartosci X
        perror("sem_open(create) error\n");
        _exit(EXIT_FAILURE);
    }
}

sem_t *otwieranie(const char *name){
    sem_t *S;
    if((S = sem_open(name, O_EXCL)) == SEM_FAILED){ //otwieram semafor
        perror("sem_open(open) error\n");
        exit(EXIT_FAILURE);
    }
    return S;
}

void wartosc(sem_t *S){
    int sval;
    if(sem_getvalue(S, &sval) == -1){
        perror("sem_getvalue error\n");
        exit(EXIT_FAILURE);
    }
    printf("Wartosc semafora wynosi: %d\n",sval);
}

void opusc(sem_t *S){ //(P)
    if(sem_wait(S) == -1){
        perror("sem_wait error\n");
        exit(EXIT_FAILURE);
    }
    printf("Wykonano operacje opuszczania P(S)\n");
}

void podnies(sem_t *S){ //(V)
     if(sem_post(S) == -1){
        perror("sem_post error\n");
        exit(EXIT_FAILURE);
    }
    printf("Wykonano operacje podnoszenia V(S)\n");
}

void zamykanie(sem_t *S){
    if(sem_close(S) == -1){
        perror("sem_close error\n");
        exit(EXIT_FAILURE);
    }
    printf("Semafor zostal zamkniety\n");
}

void usuwanie(const char *name){
    if(sem_unlink(name) == -1){
        perror("sem_unlink error\n");
        _exit(EXIT_FAILURE);
    }
    printf("Semafor zostal usuniety\n");
}

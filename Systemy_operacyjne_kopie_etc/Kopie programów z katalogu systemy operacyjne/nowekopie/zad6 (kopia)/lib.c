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
#include "lib.h"

sem_t *tworzenie(const char *name, int value){
    if((sem_open(name, O_CREAT | O_EXCL, 0666, value)) == SEM_FAILED){ //tworze semafor nazwany o wartosci 1
        perror("sem_open(create) error\n");
        exit(EXIT_FAILURE);
    }
}

sem_t *otwieranie(const char *name){
    sem_t *sem;
    if((sem = sem_open(name, O_EXCL) == SEM_FAILED)){ //otwieram semafor
        perror("sem_open(open) error\n");
        exit(EXIT_FAILURE);
    }
    return sem;
}

void wartosc(sem_t *sem){
    int sval;
    if(sem_getvalue(sem, &sval) == -1){
        perror("sem_getvalue error\n");
        exit(EXIT_FAILURE);
    }
    printf("Wartosc semafora wynosi: %d\n",sval);
}

void opusc(sem_t *sem){ //(P)
    if(sem_wait(sem) == -1){
        perror("sem_wait error\n");
        exit(EXIT_FAILURE);
    }
    printf("Wykonano operacje opuszczania P(S)\n");
}

void podnies(sem_t *sem){ //(V)
     if(sem_post(sem) == -1){
        perror("sem_post error\n");
        exit(EXIT_FAILURE);
    }
    printf("Wykonano operacje podnoszenia V(S)\n");
}

void zamykanie(sem_t *sem){
    if(sem_close(sem) == -1){
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

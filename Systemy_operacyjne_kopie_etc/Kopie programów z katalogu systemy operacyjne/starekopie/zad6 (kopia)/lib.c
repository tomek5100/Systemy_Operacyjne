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

semid adres; //zmienna trzymajaca adres semafora, semid to TYP zmiennej
semid tworzenie(const char *name, int value){ //int value moge usunac
    if((adres = sem_open(name, O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED){ //tworze semafor nazwany o wartosci 1
        perror("sem_open(create) error\n");
        exit(EXIT_FAILURE);
    }
}

semid otwieranie(const char *name){
    if(sem_open(name, 0) == SEM_FAILED){ //otwieram semafor z flaga 0
        perror("sem_open(open) error\n");
        exit(EXIT_FAILURE);
    }
}

void wartosc(semid adres){
    if(sem_getvalue(adres, int *sval == -1){
        perror("sem_getvalue error\n");
        exit(EXIT_FAILURE);
    }
    printf("Wartosc semafora wynosi: %d\n",sval);
}

void opusc(semid adres){ //(P)
    if(sem_wait(adres) == -1){
        perror("sem_wait error\n");
        exit(EXIT_FAILURE);
    }
    printf("Wykonano operacje opuszczania P(S)\n");
}

void podnies(semid adres){ //(V)
     if(sem_post(adres) == -1){
        perror("sem_post error\n");
        exit(EXIT_FAILURE);
    }
    printf("Wykonano operacje podnoszenia V(S)\n");
}

void zamykanie(semid adres){
    if(sem_close(adres) == -1){
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

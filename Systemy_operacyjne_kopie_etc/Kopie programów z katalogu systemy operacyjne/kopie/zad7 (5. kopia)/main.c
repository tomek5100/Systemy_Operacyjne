/* 
   
   --------------------------------------------------------------------
   Autor: Tomasz Rozycki                                    22.05.2021
   --------------------------------------------------------------------
*/

//makra
#define NAMEA "/semaforproducent" //trzyma nazwe semafora - producenta
#define NAMEB "/semaforkonsument" //trzyma nazwe semafora - konsumenta
#define NAMEC "/pamiec" //trzyma nazwe obiektu pamieci dzielonej
#define NELE 20 //rozmiar elementu bufora (jednostki towaru) w bajtach
#define NBUF 5 //liczba elementow bufora
//biblioteki dolaczane
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <signal.h>
#include "libmem.h"
#include "lib.h"

void koniec(void){
    printf("atexit: \n");
    usuwanie(NAMEA); //tutaj usuwam semafor producenta
    usuwanie(NAMEB); //tutaj usuwam semafor konsumenta               
    mem_del(NAMEC); //usuwanie obiektu pamieci dzielonej
    
}

void my_sighandler(){
    printf("*Wlasna obsluga sygnalu*\n");
    usuwanie(NAMEA); //tutaj usuwam semafor producenta
    usuwanie(NAMEB); //tutaj usuwam semafor konsumenta 
    mem_del(NAMEC); //usuwanie obiektu pamieci dzielonej
    
    _exit(EXIT_FAILURE); //zamykam proces
}

int main(int argc, char* argv[]){
    
    //argv[0] nazwa glownego -np. main.x
    //argv[1] producent -np. producent.x 
    //argv[2] konsument -np. konsument.x
    //argv[3] nazwa pliku do czytania - in.txt
    //argv[4] nazwa pliku do zapisywania - out.txt
    argv[5] = NAMEA; //trzyma nazwe semafora - producenta
    argv[6] = NAMEB; //trzyma nazwe semafora - konsumenta
    argv[7] = NAMEC; //trzyma nazwe obiektu pamieci dzielonej
    
   
    if(atexit(koniec) != 0){ //na poczatku main'a aby nie zamknelo semaforow i obiektu p.d ktorych jeszcze nie ma
        perror("atexit error\n");
        exit(EXIT_FAILURE);
    }
    
    //obsluga sygnalu SIGINT, usuwa semafory i obiekt pamieci dzielonej w przypadku przerwania z klawiatury - ctrl C 
    if(signal(SIGINT, my_sighandler) == SIG_ERR){
			perror("Signal (my_sighandler) error\n");
			exit(EXIT_FAILURE);
    }
    //semafor dla producenta: 
    tworzenie(NAMEA, NBUF); //tworze semafor o wartosci NBUF
    sem_t *semaforA = otwieranie(NAMEA);//otwieram semafor
    printf("\nAdres semafora(producenta) wynosi: %p | ",(void *)semaforA); //wypisuje adres semafora
    wartosc(semaforA); //wypisuje informacje o wartosci semafora
    zamykanie(semaforA); //zamykam semafor
    
    //semafor dla konsumenta: 
    tworzenie(NAMEB, 0); //tworze semafor o wartosci 0
    sem_t *semaforB = otwieranie(NAMEB);//otwieram semafor
    printf("Adres semafora(konsumenta) wynosi: %p | ",(void *)semaforB); //wypisuje adres semafora
    wartosc(semaforB); //wypisuje informacje o wartosci semafora
    zamykanie(semaforB);//zamykam semafor
    
    typedef struct{ //segment pamieci dzielonej
        char bufor[NBUF][NELE]; //wspolny bufor danych
        int wstaw; //pozycje wstawiania i wyjmowania z bufora
        int wyjmij;
    }Towar;
    
    //tworze obiekt pamieci dzielonej
    int fd = mem_creat(NAMEC); //zwraca mi fd
    printf("Deskryptor pamieci dzielonej wynosi: %d \n",fd);
    int size = sizeof(Towar);
    mem_ftruncate(fd, size); //ustawiam rozmiar pamieci dzielonej
    printf("Rozmiar pamieci dzielonej (w bajtach): %d \n",size);
    
    switch(fork()){ //tworze proces potomny dla producenta
        case -1:
            perror("fork1 error\n");
            exit(EXIT_FAILURE);
        case 0:
            if(execl(argv[1], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], NULL) == -1){
                perror("execl1 error\n");
                exit(EXIT_FAILURE);
            }
        default:
            break;
    }
    switch(fork()){ //tworze proces potomny dla konsumenta
        case -1:
            perror("fork2 error\n");
            exit(EXIT_FAILURE);
        case 0:
            if(execl(argv[2], argv[2], argv[1], argv[3], argv[4], argv[5], argv[6], argv[7], NULL) == -1){
                perror("execl2 error\n");
                exit(EXIT_FAILURE);
            }
        default:
            break;
    }
    if(wait(NULL) == -1){
            perror("Wait1 error\n");
            exit(EXIT_FAILURE);
    }
    if(wait(NULL) == -1){
            perror("Wait2 error\n");
            exit(EXIT_FAILURE);
    }
    
    return 0;   
}

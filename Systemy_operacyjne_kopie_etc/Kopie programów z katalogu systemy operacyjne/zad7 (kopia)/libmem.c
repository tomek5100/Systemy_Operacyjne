/* 
   Program zawierajacy funkcje do obslugi pamieci dzielonej
   --------------------------------------------------------------------
   Autor: Tomasz Rozycki                                    22.05.2021
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
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <signal.h>
#include "libmem.h"

int mem_creat(const char *name){ //tworzenie obiektu pamieci dzielonej
    int fd; //shared memory descryptor 
    if((fd = shm_open(name, O_CREAT|O_EXCL|O_RDWR, 0666)) == -1){
        perror("Perror shm_open(creat) error\n");
        exit(EXIT_FAILURE);
    }
    return fd;
}

int mem_open(const char *name){ //otwieranie obiektu pamieci dzielonej
    int fd; //shared memory descryptor 
    if((fd = shm_open(name, O_RDWR, 0666)) == -1){
        perror("Perror shm_open error\n");
        exit(EXIT_FAILURE);
    }
    return fd;
}

void mem_ftruncate(int fd, off_t lenght){ //ustawianie rozmiaru dla obiektu pamieci dzielonej
    if(ftruncate(fd, lenght) == -1){ //ustawiam dlugosc obiektu pamieci dzielonej na X bitow
        perror("Perror ftrucnate error\n");
        exit(EXIT_FAILURE);
    }}

void mem_close(int fd){ //zamykanie obiektu pamieci dzielonej
    if(close(fd) == -1){
        perror("Perror close error\n");
        exit(EXIT_FAILURE);
    }
//    printf("Obiekt pamieci dzielonej zostal zamkniety\n");
}

void mem_del(const char *name){ //usuwanie obiektu pamieci dzielonej
    if(shm_unlink(name) == -1){
        perror("Perror shm_unlink error\n");
        exit(EXIT_FAILURE);
    }
    printf("Obiekt pamieci dzielonej zostal usuniety\n");
}
    
void* mem_map(int fd, int size){ //odwzrowowywanie obiektu pamieci dzielonej w witrualna
    char *adres;                 //przestrzen adresowa procesu
    if((adres = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED){
        perror("Perror mmap error\n");
        exit(EXIT_FAILURE);
    }
    return adres;
}
        
void mem_munmap(void *adres, int size){ //usuwanie odwzorowania obszaru pamieci dzielonej
    if(munmap(adres, size) == -1){
        perror("Perror munmap error\n");
        exit(EXIT_FAILURE);
    }
}
    

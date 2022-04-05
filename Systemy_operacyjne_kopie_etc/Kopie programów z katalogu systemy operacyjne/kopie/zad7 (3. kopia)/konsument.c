/* 
   Program zawierajacy "konsumenta"
   --------------------------------------------------------------------
   Autor: Tomasz Różycki                                    22.05.2021
   --------------------------------------------------------------------
*/

//makra
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

int main(int argc, char* argv[]){
    
    sem_t *semaforA = otwieranie(argv[1]);//otwieram semafor producenta
    printf("Adres semafora wynosi: %p\n",(void *)semaforA); //wypisuje adres semafora
    wartosc(semaforA); //wypisuje informacje o wartosci semafora
    
    sem_t *semaforB = otwieranie(argv[2]);//otwieram semafor konsumenta
    printf("Adres semafora wynosi: %p\n",(void *)semaforB); //wypisuje adres semafora
    wartosc(semaforB); //wypisuje informacje o wartosci semafora
      
    typedef struct{ //segment pamieci dzielonej
        char bufor[NBUF][NELE]; //wspolny bufor danych
        int wstaw; //pozycje wstawiania i wyjmowania z bufora
        int wyjmij;
    }Towar;
    
    
    int fd = mem_open(argv[3]); //otwieram obiekt pamieci dzielonej
    printf("Deskryptor pamieci dzielonej wynosi: %d \n",fd);
    int size = sizeof(Towar);
//    printf("Rozmiar pamieci dzielonej (w bajtach): %d \n",size);
        
    Towar *adres = (Towar *)mem_map(fd, size); //rzutowanie obszaru pamieci dzielonej 
                                                //w wirtualna przestrzen adresowa procesu
        
    //otwieram plik do kopiowania
    int otworzkopie;  //zwroci mi Deskryptor open
    if((otworzkopie = open(argv[5], O_WRONLY|O_CREAT|O_TRUNC, 0644)) == -1){ //tworze plik out.txt
        perror("Open otworzkopie error\n");
        exit(EXIT_FAILURE); }
    
    int zapisz;
    char wyp[128]; 
    while(1){
    
        //opuszczam semafor konsumenta poczatek sekcji krytycznej
    opusc(semaforB);
                    //sekcja krytyczna
    
    if((zapisz = read(otworzkopie, wyp, NELE)) == -1){ //odczytuje tekst do tablicy wyp
        perror("Read2 error\n");                                        
        exit(EXIT_FAILURE);
    }
    else if(zapisz == 0){                       
        //gdy znak \0 zakoncz dzialanie
        strcmp(adres->bufor[adres->wstaw], "\0"); //porownuje tablice z pamieci dzielonej ze znakiem konca pliku
        break; //wychodze z petli
    }
    else{
        memcpy(otworzkopie, wyp, zapisz); //kopiuje 'zapisz' bajtow, z tablicy wyp do pliku o deskryptorze otworzkopie
        printf("Konsument zapisal %s o wielkosci %d bajtow do indeksu %d\n", wyp, zapisz, adres->wyjmij);
        adres->wyjmij = (adres->wyjmij + 1) % NBUF; //iteracja. Cyklicznosc, uzyskana operacja modulo
    }            
            
    memset(wyp, 0, NELE); //funkcja memset wypelnia kolejne bajty ustalona wartoscia, tutaj zerami aby wyczyscic tablice          
    
    podnies(semaforA);//podnosze semafor producenta koniec seksji krytycznej
    }//koniec petli
    
    mem_close(fd); //zamykanie obiektu pamieci dzielonej
    mem_munmap(adres, size); //usuwanie odwzorowania obszaru pamieci dzielonej
    zamykanie(semaforA);
    zamykanie(semaforB);
    
    if(close(otworzkopie) == -1){ //Deskryptor pliku tekstowego zostal zwolniony
        perror("Close otworzkopie error\n");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}

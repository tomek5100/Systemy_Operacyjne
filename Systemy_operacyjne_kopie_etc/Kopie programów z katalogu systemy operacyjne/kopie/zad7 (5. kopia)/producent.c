/* 
   Program zawierajacy "producenta" 
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
    
    //argv[0] producent -np. producent.x
    //argv[1] konsument -np. konsument.x
    //argv[2] nazwa pliku do czytania - in.txt
    //argv[3] nazwa pliku do zapisywania - out.txt
    //argv[4] = NAMEA; //trzyma nazwe semafora - producenta
    //argv[5] = NAMEB; //trzyma nazwe semafora - konsumenta
    //argv[6] = NAMEC; //trzyma nazwe obiektu pamieci dzielonej
    sleep(1);
    sem_t *semaforA = otwieranie(argv[4]);//otwieram semafor producenta
    printf("\t[PRODUCENT]Adres semafora(producenta) wynosi: %p | ",(void *)semaforA); //wypisuje adres semafora
    wartosc(semaforA); //wypisuje informacje o wartosci semafora
    
    sem_t *semaforB = otwieranie(argv[5]);//otwieram semafor konsumenta
    printf("\t[PRODUCENT]Adres semafora(konsumenta) wynosi: %p | ",(void *)semaforB); //wypisuje adres semafora
    wartosc(semaforB); //wypisuje informacje o wartosci semafora
      
    typedef struct{ //segment pamieci dzielonej
        char bufor[NBUF][NELE]; //wspolny bufor danych
        int wstaw; //pozycje wstawiania i wyjmowania z bufora
        int wyjmij;
    }Towar;
    
    
    int fd = mem_open(argv[6]); //otwieram obiekt pamieci dzielonej
    printf("\t[PRODUCENT]Deskryptor pamieci dzielonej wynosi: %d \n",fd);
    int size = sizeof(Towar);
//    printf("Rozmiar pamieci dzielonej (w bajtach): %d \n",size);
        
    Towar *adres = (Towar *)mem_map(fd, size); //rzutowanie obszaru pamieci dzielonej 
                                                //w wirtualna przestrzen adresowa procesu
        
    //otwieram plik z tekstem
    int otworz;  //zwroci mi Deskryptor open
    if((otworz = open(argv[2], O_RDONLY)) == -1){ //otwieram plik in.txt
        perror("Open otworz error\n");
        exit(EXIT_FAILURE); }
    
    int czytaj;
    char wypisz[128]; 
    while(1){
    
        //opuszczam semafor producenta poczatek sekcji krytycznej
    opusc(semaforA);
                    //sekcja krytyczna
    
    if((czytaj = read(otworz, wypisz, NELE)) == -1){ //odczytuje tekst do tablicy wypisz
        perror("Read1 error\n");                                        
        exit(EXIT_FAILURE);
    }
    else if(czytaj == 0){                       
        //wpisz znak \0 gdy skonczy czytanie
        strcpy(adres->bufor[adres->wstaw], "\0");
        break; //wychodze z petli
    }
    else{
        memcpy(adres->bufor[adres->wstaw], wypisz, czytaj); //kopiuje 'czytaj' bajtow, z tablicy wypisz do tablicy bufor
        printf("\t\t[PRODUCENT] przeslal %s o wielkosci %d bajtow do indeksu %d\n", wypisz, czytaj, adres->wstaw);
        adres->wstaw = (adres->wstaw + 1) % NBUF; //iteracja. Cyklicznosc, uzyskana operacja modulo
    }            
            
    memset(wypisz, 0, NELE); //funkcja memset wypelnia kolejne bajty ustalona wartoscia, tutaj zerami aby wyczyscic tablice          
    
    podnies(semaforB);//podnosze semafor konsumenta koniec seksji krytycznej
    }//koniec petli
    
    mem_close(fd); //zamykanie obiektu pamieci dzielonej
    mem_munmap(adres, size); //usuwanie odwzorowania obszaru pamieci dzielonej
    zamykanie(semaforA);
    zamykanie(semaforB);
    
    if(close(otworz) == -1){ //Deskryptor pliku tekstowego zostal zwolniony
        perror("Close otworz error\n");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}

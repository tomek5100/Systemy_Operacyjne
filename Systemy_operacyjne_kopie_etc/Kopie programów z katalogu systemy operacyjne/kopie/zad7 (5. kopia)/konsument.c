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
    
    //argv[0] konsument -np. konsument.x
    //argv[1] producent -np. producent.x 
    //argv[2] nazwa pliku do czytania - in.txt
    //argv[3] nazwa pliku do zapisywania - out.txt
    //argv[4] = NAMEA; //trzyma nazwe semafora - producenta
    //argv[5] = NAMEB; //trzyma nazwe semafora - konsumenta
    //argv[6] = NAMEC; //trzyma nazwe obiektu pamieci dzielonej 
    
    sem_t *semaforA = otwieranie(argv[4]);//otwieram semafor producenta
    printf("\t[KONSUMENT]Adres semafora(producenta) wynosi: %p | ",(void *)semaforA); //wypisuje adres semafora
    wartosc(semaforA); //wypisuje informacje o wartosci semafora
    
    sem_t *semaforB = otwieranie(argv[5]);//otwieram semafor konsumenta
    printf("\t[KONSUMENT]Adres semafora(konsumenta) wynosi: %p | ",(void *)semaforB); //wypisuje adres semafora
    wartosc(semaforB); //wypisuje informacje o wartosci semafora
      
    typedef struct{ //segment pamieci dzielonej
        char bufor[NBUF][NELE]; //wspolny bufor danych
        int wstaw; //pozycje wstawiania i wyjmowania z bufora
        int wyjmij;
    }Towar;
    
    
    int fd = mem_open(argv[6]); //otwieram obiekt pamieci dzielonej
    printf("\t[KONSUMENT]Deskryptor pamieci dzielonej wynosi: %d \n",fd);
    int size = sizeof(Towar);
//    printf("Rozmiar pamieci dzielonej (w bajtach): %d \n",size);
        
    Towar *adres = (Towar *)mem_map(fd, size); //rzutowanie obszaru pamieci dzielonej 
                                                //w wirtualna przestrzen adresowa procesu
        
    //otwieram plik do kopiowania
    int otworzkopie;  //zwroci mi Deskryptor open
    if((otworzkopie = open(argv[3], O_WRONLY|O_CREAT|O_TRUNC, 0644)) == -1){ //tworze plik out.txt
        perror("Open otworzkopie error\n");
        exit(EXIT_FAILURE); }
    int zapisz,czyt,czytkopia;
    char wyp[128];
    int i = 0;
    int w = 0;
    while(1){
        //opuszczam semafor konsumenta poczatek sekcji krytycznej
        opusc(semaforB);
                    //sekcja krytyczna
        if((czyt = read(fd, wyp, NELE)) == -1){
            perror("Read2 error\n");                                        
            exit(EXIT_FAILURE);
        }
        czytkopia = czyt;
        
        for(i=0; i<czyt; i++){ //sprawdzam czy jest EOF w tej czesci przesylanego tekstu
            if(wyp[i] == "\0"){ //porownuje odczytana tablice ze znakiem konca pliku
                printf("znalazlem znak EOF\n");
                w = i;
                czyt = i; //jestli wyp[i] == EOF wtedy wiem ze element 'i' oznacza \0
                break;
            }
        }
        if((zapisz = write(otworzkopie, wyp, czyt)) == -1){ //zapisuje 'czyt' bajtow danych z tablicy 'wyp' do pliku o deskryptorze 'otworzkopie'
            perror("Write error\n");
            exit(EXIT_FAILURE);
        }
        printf("\t\t[KONSUMENT] zapisal %s o wielkosci %d bajtow\n", wyp, zapisz);
        adres->wyjmij = (adres->wyjmij + 1) % NBUF; //iteracja. Cyklicznosc, uzyskana operacja modulo
        
        if(czyt == w){
            printf("znalazlem koniec pliku i koncze petle while(1)\n");
            break; //wychodze z petli while(1)
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

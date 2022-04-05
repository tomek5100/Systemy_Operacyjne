/* 
   Program zawierajacy "producenta" 
   --------------------------------------------------------------------
   Autor: Tomasz Różycki                                    25.04.2021
   --------------------------------------------------------------------
*/

//makra
#define SIZE 8
#define ROZM 4
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

int main(int argc, char* argv[]){
    
    char bufor[SIZE+1];
    bufor[SIZE] = 0;
    if(argc!=4){ 
        perror("Podano bledna ilosc argumentow wywolania producentowi\n");
        exit(EXIT_FAILURE);
    }
    
    //otwieram potok nazwany
    int fd; //trzyma deskryptor otworzonego potoku nazwanego
    if((fd = open(argv[3], O_WRONLY)) == -1){
        perror("Open FIFO for producent error\n");
        exit(EXIT_FAILURE);
    }
    
    //otwieram plik z tekstem
        int otworz;  //zwroci mi Deskryptor open
        if((otworz = open(argv[1], O_RDONLY)) == -1){
            perror("Open otworz error\n");
            exit(EXIT_FAILURE); }
    
    srand(time(NULL));
    int czas = rand() % 4 + 1; //generuje liczbe z zakresu 1-4

        int czytaj;
        char wypisz[128]; 
        while((czytaj = read(otworz, bufor, SIZE)) > 0){ 
            sleep(czas); //spi od 1-4s
            if(write(fd, bufor, czytaj) == -1){
                perror("Write czytaj error\n");
                exit(EXIT_FAILURE); 
            }
            
            sprintf(wypisz, "Producent odczytal: %s o wielkosci %d bajtow\n",bufor, czytaj);
            if(write(STDOUT_FILENO, wypisz, sizeof(char)*strlen(wypisz)) == -1){
            	perror("Write on screen2 error\n");
            	exit(EXIT_FAILURE);
            }
            
            memset(bufor, 0, SIZE); //funkcja memset wypelnia kolejne bajty ustalona wartoscia, tutaj zerami
                                    //oby wyczyscic tablice
            sleep(1);
        } //koniec petli
        if(czytaj == -1){
            perror("Read czytaj error\n");
            exit(EXIT_FAILURE);
            }
        
        if(close(fd == -1)){ //zamykam deskryptor dla potoku nazwanego
            perror("Close4 error\n");
            exit(EXIT_FAILURE);
        }
            
        if(close(otworz) == -1){ //Deskryptor pliku tekstowego zostal zwolniony
                perror("Close otworz error\n");
                exit(EXIT_FAILURE);
        }
    
    return 0;
}

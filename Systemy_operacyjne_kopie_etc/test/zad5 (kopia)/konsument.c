/* 
   Program zawierajacy "konsumenta"
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
        perror("Podano bledna ilosc argumentow wywolania konsumentowi\n");
        exit(EXIT_FAILURE);
    }
    
   int fd; //trzyma deskryptor otworzonego potoku nazwanego
    if((fd = open(argv[3], O_RDONLY)) == -1){
        perror("Open FIFO for consument error\n");
        exit(EXIT_FAILURE);
    }
   
   //otwieram plik do kopiowania
        int otworzkopie; //zwroci mi Deskryptor open
        if((otworzkopie = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644 )) == -1){
            perror("Open otworzkopie error\n");
            exit(EXIT_FAILURE); }
    
    srand(time(NULL));
    int czas = rand() % 4 + 1; //generuje liczbe z zakresu 1-4
    
        sleep(1);
        char wyp[128];
        int zapisz;
        while((zapisz = read(fd, bufor, ROZM))>0){
            sleep(czas); //spi od 1-4s
            if(write(otworzkopie, bufor, zapisz) == -1){
            perror("Write error\n");
            exit(EXIT_FAILURE);
        }
            sprintf(wyp, "Konsument zapisal: %s o wielkosci %d bajtow\n",bufor, zapisz);
            if(write(STDOUT_FILENO, wyp, sizeof(char)*strlen(wyp)) == -1){
            	perror("Write on screen error\n");
            	exit(EXIT_FAILURE);
            }
        }
        memset(bufor, 0, ROZM); //funkcja memset wypelnia kolejne bajty ustalona wartoscia, tutaj zerami
                                //oby wyczyscic tablice
        if(zapisz == -1){
            perror("Read zapisz error\n");
            exit(EXIT_FAILURE);
            }
           
        if(close(fd == -1)){ //zamykam deskryptor dla potoku nazwanego
            perror("Close5 error\n");
            exit(EXIT_FAILURE);
        }
        
        if(close(otworzkopie) == -1){ //zwalniam deskryptor
                perror("Close otworzkopie error\n");
                exit(EXIT_FAILURE);
        }
    
    return 0;
}


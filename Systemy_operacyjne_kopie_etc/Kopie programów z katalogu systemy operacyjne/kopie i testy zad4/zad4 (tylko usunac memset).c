/* 
   Program implementujacy problem Producent-konsument przy uzyciu potokow
   nienazwanych. 
   --------------------------------------------------------------------
   Autor: Tomasz Różycki                                    21.04.2021
   --------------------------------------------------------------------
*/

//makra
#define SIZE 4

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

char bufor[SIZE];

int main(int argc, char* argv[]){
    
    if(argc!=3){ 
        perror("Podano bledna ilosc argumentow wywolania\n");
        exit(EXIT_FAILURE);
    }
    
   int fd[2]; //filedescryptor
   //fd[0] - read
   //fd[1] - write
   if(pipe(fd) == -1){
       perror("Pipe error\n");
       exit(EXIT_FAILURE);
   }
        //otwieram plik z tekstem
        int otworz;  //zwroci mi Deskryptor open
        if((otworz = open(argv[1], O_RDONLY, 444)) == -1){
            perror("Open error\n");
            exit(EXIT_FAILURE); }
        
        //otwieram plik do kopiowania
        int otworzkopie; //zwroci mi Deskryptor open
        if((otworzkopie = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644 )) == -1){
            perror("Open copy error\n");
            exit(EXIT_FAILURE); }
    
    srand(time(NULL));
    int czas = rand() % 4 + 1; //generuje liczbe z zakresu 1-4
    int id = fork();
    switch(id)
    {
    case -1: //blad
        perror("Fork error\n"); 
        exit(EXIT_FAILURE);
        break;
    
    case 0: //potomny, konsument
        if(close(fd[1]) == -1){ //zamykam Deskryptor do zapisu
            perror("Close3 error\n");
            exit(EXIT_FAILURE);
        }
        sleep(1);
        int zapisz;
        while((zapisz = read(fd[0], bufor, SIZE))>0){
            sleep(czas); //spi od 1-4s
            memset(bufor, 0, SIZE); //funkcja memset wypelnia kolejne bajty ustalona wartoscia
            if(write(otworzkopie, bufor, SIZE) == -1){
            perror("Write error\n");
            exit(EXIT_FAILURE);
        }
            printf("Konsument zapisal do pliku: %s\n",bufor); //zmienic na write
        }
        if(zapisz == -1){
            perror("Read error\n");
            exit(EXIT_FAILURE);
            }
            
        if(close(fd[0]) == -1){
            perror("Close1 error\n");
                exit(EXIT_FAILURE);
        }
        
        if(close(otworzkopie) == -1){ //zwalniam deskryptor
                perror("Close4 error\n");
                exit(EXIT_FAILURE);
            }
        
        break;
    
    
    default: //macierzysty, producent
        
        if(close(fd[0]) == -1){ //zamykam standardowy strumien WE
        perror("Close error\n");
                exit(EXIT_FAILURE);
        }

        int czytaj;
        while(czytaj = read(otworz, bufor, SIZE) > 0){ 
            sleep(czas); //spi od 1-4s
            memset(bufor, 0, SIZE); //funkcja memset wypelnia kolejne bajty ustalona wartoscia
            if(write(fd[1], bufor, SIZE) == -1){
                perror("Write error\n");
                exit(EXIT_FAILURE); 
            }
            printf("Producent odczytal z pliku: %s\n",bufor); //dodaj write
            sleep(1);
        } //koniec petli
        if(czytaj == -1){
            perror("Read error\n");
            exit(EXIT_FAILURE);
            }
        
        if(close(fd[1]) == -1){
            perror("Close error\n");
                exit(EXIT_FAILURE);
            }
            
        if(close(otworz) == -1){ //Deskryptor pliku tekstowego zostal zwolniony
                perror("Close2 error\n");
                exit(EXIT_FAILURE);
            }
        
        if(wait(NULL) == -1){
            perror("Wait error\n");
            exit(EXIT_FAILURE);
            }
    break;
    }
    return 0;
}


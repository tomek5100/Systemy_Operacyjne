/* 
   Program implementujacy problem Producent-konsument przy uzyciu potokow
   nienazwanych. 
   --------------------------------------------------------------------
   Autor: Tomasz Różycki                                    21.04.2021
   --------------------------------------------------------------------
*/

//makra
#define _POSIX_C_SOURCE 200112L
#define _XOPEN_SOURCE 500
#define SIZE 8

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
    srand(time(NULL));
    int czas = rand() % 10 + 1; //generuje liczbe z zakresu 1-10
    int id = fork();
    switch(id)
    {
    case -1: //blad
        perror("Fork error\n"); 
        exit(EXIT_FAILURE);
        break;
    case 0: //potomny
        printf("hejka potomny sie klania\n");
        if(close(fd[0]) == -1){ //zamykam standardowy strumien WE
        perror("Close1 error\n");
                exit(EXIT_FAILURE);
        }
        
        //otwieram plik z tekstem
        int otworz;  //zwroci mi Deskryptor open
        if((otworz = open(argv[1], O_RDONLY, 444)) == -1){
            perror("Open error\n");
            exit(EXIT_FAILURE);
        }
        
        int czytaj;
        while((czytaj = read(otworz, bufor, SIZE))>0){
        if(czytaj == -1){
            perror("Read error\n");
            exit(EXIT_FAILURE);
        }}
        if(czytaj == 0){
            printf("Zakonczono odczytywanie\n"); }
           /* if(close(otworz) == -1){
                perror("Close2 error\n");
                exit(EXIT_FAILURE);
            }
            printf("Deskryptor zostal zwolniony\n");
        } */
        break;
    default: //macierzysty
        sleep(3);
        printf("hejka macierzysty sie klania\n");
        if(close(fd[1]) == -1){ //zamknac Deskryptor do zapisu
            perror("Close3 error\n");
            exit(EXIT_FAILURE);
        }
       
        int otworzkopie; //zwroci mi Deskryptor open
        if((otworzkopie = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644 )) == -1){
            perror("Open copy error\n");
            exit(EXIT_FAILURE);
        }
        
        int zapisz;
        while(czytaj>0){
        zapisz = write(otworzkopie, bufor, SIZE);
            if(zapisz == -1){
            perror("Write error\n");
            exit(EXIT_FAILURE);
        }}
        /*if(zapisz == 0){ zapisz zwraca liczbe zapisanych bajtow 
            printf("Zakonczono zapisywanie\n");
            if(close(zapisz) == -1){
                perror("Close4 error\n");
                exit(EXIT_FAILURE);
            }
            printf("Deskryptor zostal zwolniony\n");
        } */
        
        if(close(otworz) == -1){
                perror("Close2 error\n");
                exit(EXIT_FAILURE);
            }
            printf("Deskryptor zostal zwolniony\n");
     if(close(otworzkopie) == -1){
                perror("Close4 error\n");
                exit(EXIT_FAILURE);
            }
            printf("Deskryptor zostal zwolniony\n");    
    break;
         
    return 0;
}}

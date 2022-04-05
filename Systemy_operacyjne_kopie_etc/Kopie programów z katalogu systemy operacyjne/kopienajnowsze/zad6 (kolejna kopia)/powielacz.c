/* 
   Program glowny
   --------------------------------------------------------------------
   Autor: Tomasz Rozycki                                    11.05.2021
   --------------------------------------------------------------------
*/
//makra
#define NAME "/semafor" //trzyma nazwe semafora
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
#include <signal.h>
#include "lib.h"

void koniec(void){
    printf("atexit: ");
    usuwanie(NAME); //tutaj usuwam semafor
}

void my_sighandler(){
    printf("*Wlasna obsluga sygnalu*\n");
    usuwanie(NAME); //tutaj usuwam semafor
    _exit(EXIT_FAILURE); //zamykam proces
}



int main(int argc, char* argv[]){
    //argv[0] nazwa glownego - main.x
    //argv[1] wzajemne wykluczanie - zew.x
    //argv[2] liczba dzieci
    //argv[3] liczba sekcji krytycznych w kazdym z potomnych do zrobienia
    
    if(argc!=4){
        perror("Podano bledna ilosc argumentow wywolania\n");
        _exit(EXIT_FAILURE);
    }
    if(atexit(koniec) != 0){ //na poczatku main'a aby nie zamknelo semafora ktorego jeszcze nie ma
        perror("atexit error\n");
        exit(EXIT_FAILURE);
    }
    
    tworzenie(NAME, 1); //tworze semafor o nazwie "semafor", o wartosci 1
    sem_t *semafor = otwieranie(NAME);//otwieram semafor
    printf("Adres semafora wynosi: %p\n",(void *)semafor); //wypisuje adres semafora
    wartosc(semafor); //wypisuje informacje o wartosci semafora
    zamykanie(semafor);//zamykam semafor
    
   
    
    //tworze plik numer.txt
    int otworz;
    if((otworz = open("numer.txt", O_CREAT|O_TRUNC|O_RDWR, 0666)) == -1){
        perror("Open tworzenie pliku error\n");
        exit(EXIT_FAILURE); }
    
    int liczba = 0;
    if(write(otworz, &liczba, sizeof(liczba)) == -1){ //wpisuje 0 do pliku
        perror("Write wpisz 0 error\n");
        exit(EXIT_FAILURE);}
    
    if(close(otworz) == -1){ //zamykam deksryptor pliku
        perror("Close otworz1 error\n");
        exit(EXIT_FAILURE);}
    int j;
    int i = atoi(argv[2]);
    for(j=0; j<i; j++){ //tworze argv[2] procesow potomnych
        switch(fork()){
            case -1:
                perror("fork error\n");
                exit(EXIT_FAILURE);
            case 0:
                
                if(execl("./zew.x", "zew.x", argv[3], NULL) == -1){
                perror("execl error\n");
                exit(EXIT_FAILURE);
                }
            default:
                sleep(1);
                break;
    }}

    for(j=0; j<i; j++){ //czekam na zakonczenie potomnych
        if(wait(NULL) == -1){
            perror("Wait error\n");
            exit(EXIT_FAILURE);}}
                        
    //porownac wartosc pliku z oczekiwana (potomkowie*sekcje krytyczne)
    
    if((otworz = open("./numer.txt", O_RDONLY)) == -1){
        perror("Open otworz error\n");
        exit(EXIT_FAILURE); }
    
    //czytam z pliku
    int bufor;
    if(read(otworz, &bufor, sizeof(bufor)) == -1){
        perror("Read for check error\n");
        exit(EXIT_FAILURE);}
    
    if(close(otworz) == -1){ //zamykam deksryptor pliku
        perror("Close otworz2 error\n");
        exit(EXIT_FAILURE);}
    
    int l = atoi(argv[2]);
    int p = atoi(argv[3]);
    if(bufor == (l*p)){
        printf("Koncowy numer zapisany w pliku zgadza sie!\n");
    }
    else
        printf("Koncowy numer zapisany w pliku nie zgadza sie!\n");
	
    //obsluga sygnalu SIGINT, usuwa semafor w przypadku przerwania z klawiatury - ctrl C 
    if(signal(SIGINT, my_sighandler) == SIG_ERR){
			perror("Signal (my_sighandler) error\n");
			exit(EXIT_FAILURE);
    }
    
    return 0;
}

/* 
   Program realizujacy wzajemne wykluczanie.
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

void my_sighandler(){
    printf("*Wlasna obsluga sygnalu*\n");
    usuwanie(NAME); //tutaj usuwam semafor
    _exit(EXIT_FAILURE); //zamykam proces
}

int main(int argc, char* argv[]){
    //argv[0] zew.x
    //argv[1] to argv[3] z powielacz.c czyli liczba sekcji krytycznych
    
    sem_t *semafor = otwieranie(NAME);
    
    srand(time(NULL));
    int czas = rand() % 4 + 1; //generuje liczbe z zakresu 1-4
    
    printf("Sekcja prywatna: PID: %d ",getpid()); wartosc(semafor); //wypisuje pid oraz wartosc semafora
        //jeszcze mam wypisywac numer wykonywanej sekcji krytycznej
    sleep(czas); //usypiam na losowy czas z przedzialu 1-4 przed wejsciem do sekcji krytycznej
    
    int u = atoi(argv[1]);
    //printf("u czyli argv[1] =  %d\n",u);
    for(int k=1; k<=u; k++){//poczatek petli
    opusc(semafor); //wykonuje operacje opuszczenia semafora //poczatek sekcji krytycznej

    //otwieram plik z tekstem
    int otworz;  //zwroci mi Deskryptor open
    if((otworz = open("numer.txt", O_RDONLY)) == -1){
        perror("Open otworz3 error\n");
        exit(EXIT_FAILURE); }
    
    //czytam z pliku
    int bufor;
    if(read(otworz, &bufor, sizeof(bufor)) == -1){
        perror("Read error\n");
        exit(EXIT_FAILURE);}
    
    printf("\tSekcja krytyczna nr: %d. PID: %d. Odczytana wartosc: %d ",k, getpid(), bufor); wartosc(semafor);
    
    if(close(otworz) == -1){//zamykam deskryptor pliku do czytania
        perror("Close otworz1 error\n");
        exit(EXIT_FAILURE);}
    
    sleep(czas); //usypiam na losowy czas z przedzialu 1-4
    bufor++; //inkrementuje wartosc w pliku

    //otwieram plik z prawem do zapisu
    int zapisz;
    if((zapisz = open("numer.txt", O_WRONLY)) == -1){
        perror("Open zapisz error\n");
        exit(EXIT_FAILURE); }
    
    if(write(zapisz, &bufor, sizeof(bufor)) == -1){
        perror("Write error\n");
        exit(EXIT_FAILURE);} 
        printf("\t(inkrementacja)Zapisana wartosc %d\n",bufor);
    if(close(zapisz) == -1){
        perror("Close zapisz error\n");
        exit(EXIT_FAILURE);}
        
    
    podnies(semafor); //koniec sekcji krytycznej
    }//koniec petli
    printf("Po sekcji krytycznej: PID: %d ",getpid()); wartosc(semafor);
    
    //obsluga sygnalu SIGINT, usuwa semafor w przypadku przerwania z klawiatury - ctrl C 
    if(signal(SIGINT, my_sighandler) == SIG_ERR){
			perror("Signal (my_sighandler) error\n");
			exit(EXIT_FAILURE);
    }

    return 0;
}

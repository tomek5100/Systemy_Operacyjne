/* 
   
   --------------------------------------------------------------------
   Autor: Tomasz Rozycki                                    11.05.2021
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
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>
#include "lib.h"


int main(int argc, char* argv[]){
    //argv[0] nazwa glownego - main.x
    //argv[1] wzajemne wykluczanie - zew.x
    //argv[2] liczba dzieci
    //argv[3] liczba sekcji krytycznych w kazdym z potomnych do zrobienia
    
    srand(time(NULL));
    int czas = rand() % 4 + 1; //generuje liczbe z zakresu 1-4
    
    printf("Sekcja prywatna: PID: %d",getpid()); wartosc(adres); //wypisuje pid oraz wartosc semafora
        //jeszcze mam wypisywac numer wykonywanej sekcji krytycznej
    sleep(czas); //usypiam na losowy czas z przedzialu 1-4 przed wejsciem do sekcji krytycznej
    
    int u = atoi(argv[3]);
    for(int k=1; k<u; k++){//poczatek petli
    opusc(adres); //wykonuje operacje opuszczenia semafora
    //tu juz jest sekcja krytyczna
    //otwieram plik z tekstem
    int otworz;  //zwroci mi Deskryptor open
    if((otworz = open("./numer.txt", O_RDONLY)) == -1){
        perror("Open otworz error\n");
        exit(EXIT_FAILURE); }
    
    //czytam z pliku
    int bufor = 0;
    if(read(otworz, &bufor, sizeof(bufor)) == -1){
        perror("Read error\n");
        exit(EXIT_FAILURE);}
    
    printf("\tSekcja krytyczna nr: %d. PID: %d. Odczytana wartosc: %d.",k, getpid(), bufor); wartosc(adres);
    
    //zamykam deskryptor pliku do czytania
    if(close(otworz) == -1){
        perror("Close otworz1 error\n");
        exit(EXIT_FAILURE);}
    
    sleep(czas); //usypiam na losowy czas z przedzialu 1-4
    bufor++; //inkrementuje wartosc w pliku

    //otwieram plik z prawem do zapisu
    if((otworz = open("./numer.txt", O_WRONLY)) == -1){
        perror("Open otworz error\n");
        exit(EXIT_FAILURE); }
    
    if(write(otworz, &bufor, sizeof(bufor)) == -1){
        perror("Write error\n");
        exit(EXIT_FAILURE);}
        
    if(close(otworz) == -1){
        perror("Close otworz1 error\n");
        exit(EXIT_FAILURE);}
        
    
    opusc(adres); //koniec sekcji krytycznej
    }//koniec petli
    usuwanie(name); //usuwam semafor
    printf("Po sekcji krytycznej: PID: %d.",getpid()); wartosc(adres);

    return 0;
}

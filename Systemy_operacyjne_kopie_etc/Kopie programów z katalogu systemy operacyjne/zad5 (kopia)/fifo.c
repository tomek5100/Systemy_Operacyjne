/* 
   Program implementujacy problem Producent-konsument przy uzyciu potokow
   nienazwanych. 
   --------------------------------------------------------------------
   Autor: Tomasz Różycki                                    21.04.2021
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

const char *nazwa = NULL; //zmienna globalna trzymajaca nazwe dla potoku

void koniec(void){
    if(unlink(nazwa) == -1){ //tutaj ma byc usuniecie potoku
        perror("unlink error\n");
        _exit(EXIT_FAILURE);
    } 
    printf("Koniec procesu - atexit: usunieto potok!\n");
}

int main(int argc, char* argv[]){
    
    //argv[0] fifo.x
    //argv[1] in.txt
    //argv[2] out.txt
    //argv[3] nazwa_potoku
    
    nazwa = argv[3]; //przypisanie nazwy potoku do zmiennej globalnej 
    
    if(argc!=4){ // a.out in.txt out.txt nazwa_potoku
        perror("Podano bledna ilosc argumentow wywolania\n");
        exit(EXIT_FAILURE);
    }
    
    if(atexit(koniec) != 0){ //na poczatku aby nie zamknelo potoku ktorego jeszcze nie ma
        perror("atexit error\n");
        exit(EXIT_FAILURE);
    }
        
    
    //tworze potok nazwany 
    if(mkfifo(nazwa, 0644) == -1){
        perror("mkfifo error\n");
        exit(EXIT_FAILURE);
    }
    
    //otwieram potok nazwany
    
    
    switch(fork()){ //tworze proces potomny dla producenta
        case -1:
            perror("fork error\n");
            exit(EXIT_FAILURE);
        case 0:
            //tu mam producenta
            execl("./producent.x","producent",argv[1],argv[3], NULL);
            perror("Exec producent error\n"); 
            exit(EXIT_FAILURE);	
        
        default:
            //idzie sobie do kolejnego forka
    }
    
    switch(fork()){ //tworze proces potomny dla konsumenta
        case -1:
            perror("fork error\n");
            exit(EXIT_FAILURE);
        case 0:
            //tu mam proces konsumenta
            execl("./konsument.x","konsument",argv[2],argv[3], NULL);
            perror("Exec konsument error\n"); 
            exit(EXIT_FAILURE);	
        
        default:
            //idzie sobie do atexit itd
    }
    
    //tu juz mam tylko macierzysty
    for(int i=0;i<2;i++){ //czekam na zakonczenie potomnych
        if(wait(NULL) == -1){
            perror("Wait %d error\n",i);
            exit(EXIT_FAILURE);
        }}
 
            
    
    //zrobic mozliwosc wyslania sygnalu ctrl C SIGINT, do grupy procesow potomnych czyli PPID macierzystego
    
    
    
    return 0;
}

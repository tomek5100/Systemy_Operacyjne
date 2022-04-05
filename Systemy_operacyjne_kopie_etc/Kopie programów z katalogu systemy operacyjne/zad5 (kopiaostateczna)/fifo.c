/* 
   Program implementujacy problem Producent-konsument przy uzyciu potokow
   nazwanych. 
   --------------------------------------------------------------------
   Autor: Tomasz Różycki                                    21.04.2021
   --------------------------------------------------------------------
*/

//makra
#define _POSIX_C_SOURCE 200112L
#define _XOPEN_SOURCE 500
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
    if(unlink(nazwa) == -1){ //tutaj usuwam potok
        perror("unlink error\n");
        _exit(EXIT_FAILURE); //uzywam podkreslonej funkcji exit aby nie wywolala atexit
    } 
    printf("Koniec procesu - atexit: usunieto potok!\n");
}

void my_sighandler(){
    printf("*Wlasna obsluga sygnalu*\n");
    if(unlink(nazwa) == -1){ //tutaj usuwam potok
        perror("unlink (signal) error\n");
        _exit(EXIT_FAILURE); //uzywam podkreslonej funkcji exit aby nie wywolala atexit
    } 
    printf("Koniec procesu - atexit: usunieto potok!\n");
    _exit(EXIT_FAILURE); //zamykam proces
}

int main(int argc, char* argv[]){
    
    //argv[0] fifo.x
    //argv[1] in.txt
    //argv[2] out.txt
    //argv[3] nazwa_potoku
    
    nazwa = argv[3]; //przypisanie nazwy potoku do zmiennej globalnej, aby atexit dzialal poprawnie
    
    if(argc!=4){ // a.out in.txt out.txt nazwa_potoku
        perror("Podano bledna ilosc argumentow wywolania\n");
        _exit(EXIT_FAILURE);
    }
    
    if(atexit(koniec) != 0){ //na poczatku main'a aby nie zamknelo potoku ktorego jeszcze nie ma
        perror("atexit error\n");
        exit(EXIT_FAILURE);
    }
        
    
    //tworze potok nazwany 
    if(mkfifo(nazwa, 0644) == -1){
        perror("mkfifo error\n");
        _exit(EXIT_FAILURE);
    }
    
    
    switch(fork()){ //tworze proces potomny dla producenta
        case -1:
            perror("fork error\n");
            _exit(EXIT_FAILURE);
        case 0:
            //tu mam producenta
            execl("./producent.x","producent",argv[1],argv[2],argv[3], NULL);
            perror("Exec producent error\n"); 
            _exit(EXIT_FAILURE);	
        
        default:
            break;
            //idzie do kolejnego forka
    }
    
    switch(fork()){ //tworze proces potomny dla konsumenta
        case -1:
            perror("fork error\n");
            _exit(EXIT_FAILURE);
        case 0:
            //tu mam proces konsumenta
            execl("./konsument.x","konsument",argv[1],argv[2],argv[3], NULL);
            perror("Exec konsument error\n"); 
            _exit(EXIT_FAILURE);	
        
        default:
            break;
    }
    //tu juz mam tylko macierzysty
    
    //obsluga sygnalu SIGINT, usuwa potok w przypadku przerwania z klawiatury - ctrl C 
    if(signal(SIGINT, my_sighandler) == SIG_ERR){
			perror("Signal (my_sighandler) error\n");
			exit(EXIT_FAILURE);
    }
    
    for(int i=0;i<2;i++){ //czekam na zakonczenie potomnych
        if(wait(NULL) == -1){
            perror("Wait error\n");
            exit(EXIT_FAILURE);
        }}
 
    return 0;
}

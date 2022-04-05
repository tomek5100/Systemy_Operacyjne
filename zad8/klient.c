/* 
   Program zawierajacy "klienta"
   --------------------------------------------------------------------
   Autor: Tomasz Różycki                                    26.05.2021
   --------------------------------------------------------------------
*/

//makra
#define PATH "/"
//biblioteki dolaczane
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <mqueue.h>
#include "libqueue.h"

const char *name = NULL;

void koniec(void){
    printf("atexit: ");
    que_unlink(name); //usuwam kolejke komunikatow klienta
}

void my_sighandler(int sig){
    printf("*Wlasna obsluga sygnalu*\n");
    que_unlink(name); //usuwam kolejke komunikatow klienta
    _exit(EXIT_SUCCESS); //zamykam proces
}

/*struct mq_attr{ //struktura trzymajaca atrybuty kolejki komunikatow
        long mq_flags;; //Opcje: 0 lub O_NONBLOCK 
        long mq_maxmsg; //Maksymalna liczba komunikatow w kolejce 
        long mq_msgsize; //Maksymalny rozmiar komunikatu (w bajtach)
        long mq_curmsgs; //Liczba komunikatow aktualnie w kolejce
    }; */

int main(int argc, char* argv[]){
    
    if(atexit(koniec) != 0){
        perror("atexit error\n");
        exit(EXIT_FAILURE);
    }
    
    //obsluga sygnalu SIGINT, w przypadku przerwania z klawiatury - ctrl C 
    if(signal(SIGINT, my_sighandler) == SIG_ERR){
			perror("Signal (my_sighandler) error\n");
			exit(EXIT_FAILURE);
    }
    
    char nazwa[16]; //zmienna trzymajaca nazwe kolejki k. dla klienta
    int pid = getpid();
    sprintf(nazwa, "%s%d", PATH, pid);
    name = nazwa; 
    
    struct mq_attr klient;
    klient.mq_flags = 0;
    klient.mq_maxmsg = 8;
    klient.mq_msgsize = 128;
    klient.mq_curmsgs = 0;
    
    mqd_t mqdesclient = que_creat(nazwa, O_CREAT|O_EXCL|O_RDWR, &klient); //tworze kolejke klienta
    mqd_t mqdes = que_open(NAZWAKOLEJKI, O_WRONLY); //otwieram kolejke serwera
    
    printf("[Klient]Nazwa: %s, deksryptor: %d, atrybuty: (flags)%ld, (maxmsg)%ld, (msgsize)%ld, (curmsgs)%ld\n",nazwa, mqdesclient, klient.mq_flags, klient.mq_maxmsg, klient.mq_msgsize, klient.mq_curmsgs);
    
    char str[16]; //zmienna trzymajaca dzialania x+y
    srand(time(NULL));
    while(1){ 
        printf("Podaj dzialanie typu (x+y)\n");
       
        if(fgets((char *)str, 10, stdin) == NULL){//fgets zwraca NULL gdy EOF, czytam z 'stdin' czyli standard. strumienia wejscia
            printf("dzialanie %s\n",str);
            break;
        }
        
        char msg[64]; //komunikat 
        sprintf(msg, "%s %s", nazwa, str); //wpisuje pid na poczatek komunikatu oraz dzialanie

        printf("Komunikat wysylany przez klienta: %s \n", msg); // /PID 2+1
        
        que_send(mqdes, (const char *)msg, sizeof(msg)); //wysylam dzialanie do kolejki komunikatow serwera
        int czas = rand() % 4 + 1; //generuje liczbe z zakresu 1-4
        sleep(czas);
        
        //odbieram wykonane dzialanie
        char msg_sec[64];
        que_receive(mqdesclient, (char *)msg_sec, sizeof(msg_sec)+100);
        printf("[KLIENT]Wykonane dzialanie: %s\n",msg_sec);
        
        
    }//koniec petli while
    
    que_close(mqdesclient); //zamykam kolejke komunikatow klienta
    que_close(mqdes); //zamykam kolejke komunikatow serwera
    
    return 0;
}

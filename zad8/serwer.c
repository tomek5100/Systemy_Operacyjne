/* 
   Program zawierajacy "serwer"
   --------------------------------------------------------------------
   Autor: Tomasz Różycki                                    26.05.2021
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
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <mqueue.h>
#include "libqueue.h"

//mqd_t mqdes = 0;

void koniec(void){
    printf("atexit: ");
//    que_close(mqdes); //zamykam kolejke komunikatow serwera
    que_unlink(NAZWAKOLEJKI); //usuwam kolejke komunikatow serwera
    
}
void my_sighandler(int sig){
    printf("*Wlasna obsluga sygnalu*\n");
//    que_close(mqdes); //zamykam kolejke komunikatow serwera
    que_unlink(NAZWAKOLEJKI); //usuwam kolejke komunikatow serwera
    _exit(EXIT_SUCCESS); //zamykam proces
    
}


/*struct mq_attr{ //struktura trzymajaca atrybuty kolejki komunikatow
        long mq_flags; //Opcje: 0 lub O_NONBLOCK 
        long mq_maxmsg; //Maksymalna liczba komunikatow w kolejce 
        long mq_msgsize; //Maksymalny rozmiar komunikatu (w bajtach)
        long mq_curmsgs; //Liczba komunikatow aktualnie w kolejce
    }; */
     
int dzialanie(int x, char b, int y){
    if(b == '+')
        return x+y;
    if(b == '-')
        return x-y;
    if(b == '*')
        return x*y;
    else //(b == '/')
        return x/y;
}
    

int main(int argc, char* argv[]){
    
    
    
    struct mq_attr struktura; 
    struktura.mq_flags = 0;
    struktura.mq_maxmsg = 8;
    struktura.mq_msgsize = 128;
    struktura.mq_curmsgs = 0;
    
    mqd_t mqdes = que_creat(NAZWAKOLEJKI, O_CREAT|O_EXCL|O_RDWR, &struktura); //tworze kolejke serwera
    
    if(atexit(koniec) != 0){
        perror("atexit error\n");
        exit(EXIT_FAILURE);
    }
    //obsluga sygnalu SIGINT, w przypadku przerwania z klawiatury - ctrl C 
    if(signal(SIGINT, my_sighandler) == SIG_ERR){
			perror("Signal (my_sighandler) error\n");
			exit(EXIT_FAILURE);
    }
    
    printf("[Serwer]Nazwa: %s, deksryptor: %d, atrybuty: (flags)%ld, (maxmsg)%ld, (msgsize)%ld, (curmsgs)%ld\n",NAZWAKOLEJKI, mqdes, struktura.mq_flags, struktura.mq_maxmsg, struktura.mq_msgsize, struktura.mq_curmsgs);
//    que_close(mqdes); //zamykam kolejke komunikatow serwera
    srand(time(NULL));
    
    while(1){ //serwer dziala w nieskonczonej petli
//        mqdes = que_open(NAZWAKOLEJKI, O_RDONLY); //otwieram kolejke serwera w trybie do odbierania
        char msg[256]; //komunikat
        que_receive(mqdes, msg, sizeof(msg)+1000); //odbieram komunikat od klienta
        
        //rozbijam wiadomosc na /PID liczba znak liczba
        char nazwa[16]; // /PID
        int liczbaX, liczbaY; //dwie liczby xy
        char znak; //dodawanie itp.
        
        sscanf(msg, "%s%d%c%d", nazwa, &liczbaX, &znak, &liczbaY);
        
        printf("[Od klienta]Nazwa %s, liczbaX=%d, znak %c, liczbaY=%d\n",nazwa, liczbaX, znak, liczbaY);
        
        //otwieram kolejke komunikatow klienta
        mqd_t mqdesclient = que_open(nazwa, O_WRONLY);
        
        //wykonuje dzialanie
        int wynik = dzialanie(liczbaX, znak, liczbaY);
        char msg_sec[64];
        sprintf(msg_sec, "%d", wynik);
        int czas = rand() % 4 + 1; //generuje liczbe z zakresu 1-4
        sleep(czas);
        //odsylam 
        printf("[SERWER]Wykonane dzialanie: %s\n",msg_sec);
        que_send(mqdesclient, (const char *)msg_sec, sizeof(msg_sec));
        
        que_close(mqdesclient); //zamykam kolejke komunikatow klienta
        
    }
    
    
    
    return 0;
}

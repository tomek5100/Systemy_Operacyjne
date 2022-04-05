#include "funkcjek.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

mqd_t kolejkaklient;
mqd_t kolejkaserwer;
char nazwa [7];

void my_handler(int sig){
	printf("Sygnal przechwycony\n");
    exit(EXIT_FAILURE);
}

void koniec(void){
	mqClose(kolejkaserwer);
	mqClose(kolejkaklient);
	mqRemove(nazwa);
	printf("Koniec!\n");

}

int main(){
	sleep(1);
	if(signal(SIGINT, my_handler) == SIG_ERR){
        perror("SIGNAL ERROR");
        exit(EXIT_FAILURE);
    }
    
    if(atexit(koniec) != 0){
        perror("ATEXIT ERROR");
        exit(EXIT_FAILURE);
    }
    
    pid_t pid = getpid();
    sprintf(nazwa, "/%d", pid);
    
    mqCreate(nazwa); // utworzenie kolejki komunikatow klienta
    
    kolejkaklient = mqOpen(nazwa, O_RDONLY); // otwarcie kolejki komunikatow klienta
	printf("Utworzono kolejke komunikatow o nazwie: %s z deskryptorem: %d\n", nazwa, kolejkaklient);
	
	mqGetattr(kolejkaklient); // wypisanie informacji o kolejce
	sleep(1);
	
	kolejkaserwer = mqOpen(SERWER, O_WRONLY); // otwarcie kolejki komunikatow serwera
	
	printf("Podaj dzialanie\n");
	printf("Oddziel skladniki i znak spacjami\n");
	
	char linia[10], wiadomosc [20];
	srand(time(NULL));
	
	while (fgets(linia, sizeof(linia)/sizeof(char), stdin) != 0){
		sprintf(wiadomosc, "%d %s", pid, linia);
		
		mqSend(kolejkaserwer, wiadomosc, sizeof(wiadomosc) / sizeof(char)); // wiadomosc do kolejki komunikatow serwera
		printf("Wyslano wiadomosc: %s\n", wiadomosc);
		
		sleep(rand() % 3);
		
		mqReceive(kolejkaklient, wiadomosc, sizeof(wiadomosc) / sizeof(char));
		
		printf("Wynik to: %s\n", wiadomosc);
	}
	
	return 0;
    
}

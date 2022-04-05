#include "funkcjek.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

mqd_t fd; // zmienna globalna przechowujaca deskryptor

void my_handler(int sig){
	printf("Sygnal przechwycony\n");
    exit(EXIT_FAILURE);
}

void koniec(void){
	mqClose(fd);//zamkniecie kolejki
	mqRemove(SERWER);//usuniecie kolejki
	printf("Koniec!\n");
}

int main(){
	
	if(signal(SIGINT, my_handler) == SIG_ERR){
        perror("SIGNAL ERROR");
        exit(EXIT_FAILURE);
    }
    
    if(atexit(koniec) != 0){
        perror("ATEXIT ERROR");
        exit(EXIT_FAILURE);
    }
	//mqRemove(SERWER);
    
	mqCreate(SERWER); // tworzenie kolejki w trybie do otwierania
	
	fd = mqOpen(SERWER, O_RDONLY); // otwarcie kolejki komunikatow serwera
	printf("Utworzono kolejke komunikatow o nazwie: %s z deskryptorem: %d\n", SERWER, fd);
	
	mqGetattr(fd); // wypisanie informacji o kolejce
	
	int identyfikator;
	char wiadomosc[20], nazwa [7];
	char znak[1];
	char *a;
	int x=0,y=0;
	int wynik=0;
	
	
	while(1){
		int b = 0;
		b = (sizeof(wiadomosc)/ sizeof(char));
		mqReceive(fd, wiadomosc, b);
		printf("Otrzymano wiadomosc: %s\n", wiadomosc);
		
		a = strtok(wiadomosc, " ");
		identyfikator = atoi(a);
		
		int i;
		for(i=1; a != NULL; i++){
			a = strtok(NULL, " ");
			
			if(i == 1){		
              			x = atoi(a);
            		}
            		
            if(i == 2){
                sprintf(znak, "%s", a);
            }
            if(i == 3){
                y = atoi(a);
            }
		}
		
		switch (znak[0]){
			case '-':
				wynik = x - y;
				break;
			case '+':
				wynik = x+y;
				break;
			case '*':
				wynik = x*y;
				break;
			case '/':
				if(y == 0){
					printf("Dzielenie przez 0 niedozwolone\n");
					exit(EXIT_FAILURE);
				}
				else
				wynik = x/y;
				break;
				
		}
		sprintf(nazwa, "/%d", identyfikator);
		mqd_t kolejkaklient;
		kolejkaklient = mqOpen(nazwa, O_WRONLY); // otwarcie kolejki komunikatow klienta
		
		srand(time(NULL));
		sleep(rand() % 3);
		
		char nowa_wiadomosc[20];
		sprintf(nowa_wiadomosc, "%d", wynik);
		
		mqSend(kolejkaklient, nowa_wiadomosc, sizeof(wiadomosc) / sizeof(char)); // wyslanie wiadomosci do kolejki komunikatow klienta
		
		printf("Wyslano wiadomosc o tresci: %s\n", nowa_wiadomosc);
		
		mqClose(kolejkaklient); // zamkniecie kolejki komunikatow klienta
		
	}
	
	return 0;
	
}

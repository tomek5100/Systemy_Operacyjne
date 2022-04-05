#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>
#include "funkcje.h"

int main(){
	sem_t *s = semOpen("/semafor");
	printf("		Przed sekcja krytyczna. Wartosc semafora: %d, PID: %d\n", semInfo(s), getpid() );
	semP(s); // opuszczanie semafora
	
	printf("		W sekcji krytycznej. Wartosc semafora: %d, PID: %d\n", semInfo(s), getpid() );
	int a, number;
	if((a = open("numer.txt",O_RDONLY, 0666)) == -1){ // sprawdz flagi!!
		perror("OPEN ERROR");
		exit(EXIT_FAILURE);
	}
	
	if(read(a, &number, sizeof(number)) == -1){
        perror("READ ERROR");
        exit(EXIT_FAILURE);
    }
    
    printf("	Numer: %d\n", number);
    
    number++;
    
    close(a);
    
	srand(time(NULL));
    sleep(rand() % 5);
    
    int b;
	if((b = open("numer.txt",O_WRONLY, 0666)) == -1){ // sprawdz flagi!!
		perror("OPEN ERROR");
		exit(EXIT_FAILURE);
	}
	
	if(write(b, &number, sizeof(number)) == -1){
        perror("WRITE ERROR");
        exit(EXIT_FAILURE);
    }
    
    close(b);
    semV(s); //podniesienie semafora
    
    printf("		Po sekcji krytycznej. Wartosc semafora: %d, PID: %d\n", semInfo(s), getpid() );
    
    return 0;
}

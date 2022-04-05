#include "funkcje.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(){

	semCreate("/semafor",1); // tworze semafor
	sem_t *s = semOpen("/semafor");
	printf("Wartosc semafora: %d\n",semInfo(s));
	printf("Adres semafora: %p\n", (void *)s);
	semClose(s);
	
	int a;
	if((a = open("numer.txt",O_RDWR|O_CREAT|O_APPEND, 0666)) == -1){ // sprawdz flagi!!
		perror("OPEN ERROR");
		exit(EXIT_FAILURE);
	}
	
	int number = 0;
	if(write(a, &number, sizeof(number)) == -1){
        perror("WRITE ERROR");
        exit(EXIT_FAILURE);
    }
    
    close(a); //brak obslugi bledu dla zamykania deskryptora pliku
    
    int i,j;
    for(i=0; i<3; i++){
    	pid_t b = fork();
    	
    	switch(b){
    		case -1: 
    			perror("FORK ERROR");
				exit(EXIT_FAILURE);
			case 0:
				execl("./program.x", "program.x", NULL);
           	 	perror("EXECL ERROR");
           		exit(EXIT_FAILURE);
           default:
           		for(j=0; j<3; j++){
           			wait(NULL);
				   }
				break;
			}
	}
	
	int c;
	if((c = open("numer.txt",O_RDONLY, 0666)) == -1){ // sprawdz flagi!!
		perror("OPEN ERROR");
		exit(EXIT_FAILURE);
	}
	
	if(read(c, &number, sizeof(number)) == -1){
        perror("READ ERROR");
        exit(EXIT_FAILURE);
    }
    
    printf("Koncowy numer w pliku: %d\n", number);
    
    if(number == i){
    	printf("Koncowy numer poprawny\n");
	}
	
	close(c);
	
		
	
	if(atexit(semRemove) != 0){
        perror("ATEXIT ERROR");
        exit(EXIT_FAILURE);
    }
    
    if(signal(SIGINT, my_handler) == SIG_ERR){
        perror("SIGNAL ERROR");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}

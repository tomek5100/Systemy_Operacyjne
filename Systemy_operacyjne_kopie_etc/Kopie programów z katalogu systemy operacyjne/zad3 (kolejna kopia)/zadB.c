/* 
   Program uruchamia program z podpunktu a) poprzez funkcje exec w procesie
   potomnym, proces macierzysty wysyla do niego sygnaly poprzez funkcje kill.
   --------------------------------------------------------------------
   Autor: Tomasz Różycki                       		13.04.2021
   --------------------------------------------------------------------
*/

//makra
#define _POSIX_C_SOURCE 200112L
#define _XOPEN_SOURCE 500
#define PATH "./"

//biblioteki dolaczane
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


int main(int argc, char* argv[]){
    char PathName[128]; //nazwa sciezki/programu
    sprintf(PathName, "%s%s", PATH, argv[0]); 
    //int wybor = atoi(argv[1]);//wybor opcji obslugi sygnalu 1-3 dalem w komentarz bo zbedna w tym prog
    int sig = atoi(argv[2]); //przechwycony numer sygnalu
    
    if(argc!=3){
    	printf("Podano bledna ilosc argumentow wywolania\n");
    	exit(EXIT_FAILURE);
    }
    int id = fork();
    switch(id)
    {
	case -1: //blad
		perror("Fork error\n"); //obsluga bledu
		exit(EXIT_FAILURE);
	break;
	
	case 0: //proces potomny
		printf("Uruchamianie programu zadA...\n"); 
		execl("./zadA.x","zadA", argv[1], argv[2], NULL);	//uruchomienie programu zadA.x
		perror("Exec error\n"); //obsluga bledu dla exec
		exit(EXIT_FAILURE);		
	
	break;
	
	default: //proces macierzysty
		
		if(kill(id, 0)==ESRCH){ //przed wyslaniem sygnalu sprawdzam czy proces potomny istnieje
			perror("Procces does not exist!\n");
			exit(EXIT_FAILURE);
			}		
		printf("Proces potomny istnieje! Wysylanie sygnalu:\n");
		sleep(2); //oczekiwanie na dostarczenie sygnalu
		
		if(kill(id, sig)==-1){ //wyslanie sygnalu do potomnego
		perror("Kill error\n");
		exit(EXIT_FAILURE);
		}
	break;
	
 	return 0;
}
}



















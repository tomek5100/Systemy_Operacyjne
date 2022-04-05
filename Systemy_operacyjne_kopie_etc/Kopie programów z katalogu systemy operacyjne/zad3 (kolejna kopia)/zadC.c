/* 
   Program tworzy potomka, ktory zostaje liderem swojej grupy, wywoluje on
   funkcje execl. Proces macierzysty przekazuje sygnal do grupy procesow potomnych
   ktory zostal podany podczas wywolania programu. 
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
    int sig = atoi(argv[2]); //przechwycony numer sygnalu
    if(argc!=3){
    	printf("Podano bledna ilosc argumentow wywolania\n");
    	exit(EXIT_FAILURE);
    }
    int ID = fork(); //tworze proces potomny
    switch(ID)
    {
	case -1: //blad
		perror("Fork error\n"); //obsluga bledu
		exit(EXIT_FAILURE);
	break;
	
	case 0: //proces potomny
		if(setpgid(ID, 0)==-1){ //proces potomny staje sie liderem nowej grupy procesow
			perror("Setpgid error\n");
			exit(EXIT_FAILURE);
		}
		//uruchomienie programu zadCc.x
		if(execl("./zadCc.x","zadCc", argv[1], argv[2], NULL)==-1){		
		perror("Exec error\n"); //obsluga bledu dla execl
		exit(EXIT_FAILURE);
		}		
	break;
	
	default: //proces macierzysty
		sleep(2); //usypiam aby procesy potomne mialy czas powstac
		if(kill(-ID, 0)==ESRCH){ //przed wyslaniem sygnalu sprawdzam czy proces potomny istnieje
			perror("Procces does not exist!\n");
			exit(EXIT_FAILURE);
			}		
		printf("Proces potomny istnieje! Wysylanie sygnalu:\n");
		
		
		if(kill(-ID, sig)==-1){ //wyslanie sygnalu do grupy potomnych		
		perror("Kill error\n");
		exit(EXIT_FAILURE);
		}
		if(wait(NULL)==-1){
		perror("Wait error\n"); //obsluga bledu dla wait
		 exit(EXIT_FAILURE);
		}
	break;
	}
	
 	return 0;
}

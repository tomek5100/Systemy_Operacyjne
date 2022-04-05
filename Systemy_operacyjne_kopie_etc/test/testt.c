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
    
    
    
    
    int ID = fork();
    switch(ID)
    {
	case -1: //blad
		perror("Fork error\n"); //obsluga bledu
		exit(EXIT_FAILURE);
	break;
	
	case 0: //proces potomny
		
		printf("Uruchamianie programu zadA... PIDpot %d\n",getpid()); 
				
	
	break;
	
	default: //proces macierzysty
		
			
		printf("Proces potomny istnieje! Wysylanie sygnalu: PID %d\n",getpid());
		
	break;
	
 	return 0;
}
}

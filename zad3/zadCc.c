/* 
   Proces-lider grupy- ignoruje sygnal od procesu macierzystego, wywoluje 
   trzykrotnie funkcje fork, trzy procesy uruchamiaja funkcje execl, ktora
   wykonuje program z podpunktu a). Lider czeka na zakonczenie trwania procesow
   po czym zwraca status zakonczenia procesow potomnych. 
   --------------------------------------------------------------------
   Autor: Tomasz Różycki                       				13.04.2021
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
	int sig = atoi(argv[2]); //przechwycony numer sygnalu
    int status; //zmienna ktora bedzie trzymac wartosc zwracana przez funkcje waitpid
    int i;
    if(signal(sig, SIG_IGN) == SIG_ERR){ //lider grupy procesow ignoruje sygnal
    	perror("Signal error\n");
    	exit(EXIT_FAILURE);
    }
    for(i=0; i<3; i++){
	switch(fork()){
	case -1: 
		perror("Fork error\n"); //obsluga bledu
		exit(EXIT_FAILURE);	
	break;
				
	case 0: //trzy procesy potomne
		//uruchomienie programu zadA.x
		if(execl("./zadA.x","zadA.x", argv[1], argv[2], NULL)==-1){		
		perror("Exec error\n"); //obsluga bledu dla execl
		exit(EXIT_FAILURE);
		}
	break;
	
				
	default:
		if(i==2){
			for(i=0;i<3;i++){
				if(wait(&status)==-1){
		 			perror("Wait error\n"); //obsluga bledu dla wait
		 			exit(EXIT_FAILURE);
				}
			printf("%d.Status zakonczenia: %d\n",i+1,status);
			}}
	break;
	}}
				
	
return 0;
}










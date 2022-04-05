/* 
   Program do obslugi sygnalów z mozliwosciami: (1) wykonania operacji domyślnej,
   (2) ignorowania oraz (3) przechwycenia i wlasnej obslugi sygnalu.
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

char *strsignal(int sig);		//do wypisania nazwy sygnalu
extern const char * const sys_siglist[]; //deklaracja pozwalajaca uzyc tablicy nazw sygnalow

void my_sighandler(int sig){
        printf("*Wlasna obsluga sygnalu* Numer sygnalu: %d Nazwa sygnalu: %s\n",sig,strsignal(sig));
        }
int main(int argc, char* argv[]){
    
    char PathName[128]; //nazwa sciezki/programu
    sprintf(PathName, "%s%s", PATH, argv[0]); 
    int wybor = atoi(argv[1]); //wybor opcji obslugi sygnalu 1-3
    int sig = atoi(argv[2]); //przechwycony numer sygnalu
    
    if(argc!=3){ 
    	printf("Podano bledna ilosc argumentow wywolania\n");
    	exit(EXIT_FAILURE);
    }
    printf("PID procesu: %d\n",getpid());
    switch(wybor)
    {
	case 1: //wykonanie operacji domyslnej, sterowanej przez jadro systemu
		if(signal(sig, SIG_DFL) == SIG_ERR){	//jesli signal zwroci (-1)
			perror("Signal (SIG_DFL) error\n"); //to nastepuje obsluga bledu
			exit(EXIT_FAILURE);
		}
		break;
	case 2: //wykonanie operacji ignorowania
		if(signal(sig, SIG_IGN) == SIG_ERR){
			perror("Signal (SIG_IGN) error\n");
			exit(EXIT_FAILURE);
		}
		break;
	case 3: //wykonanie operacji wlasnej
		if(signal(sig, my_sighandler) == SIG_ERR){
			perror("Signal (my_sighandler) error\n");
			exit(EXIT_FAILURE);
		}
		break;
	default:
		printf("Podano nieistniejacy numer obslugi sygnalu!\n");
		exit(1); 
		break;
	}
	pause(); //proces czeka na sygnal
	
 	return 0;
}

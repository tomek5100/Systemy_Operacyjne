#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include <time.h>
#include <errno.h>

#define BUFZ_LEN 4
#define BUFR_LEN 2

int pipefd[2];

void producer(char * file){
	
	if(close(pipefd[0])){
		perror("Closing pipe's reading side error");
		exit(EXIT_FAILURE);
	}
	
	int fd = open(file, O_RDONLY, 0);
		if(fd == -1){
		  perror("Opening file error");
		  exit(EXIT_FAILURE);
		}
	
	char bufZ[BUFZ_LEN];
	while(1){
	
		int rb = read(fd, bufZ, BUFZ_LEN); 
			if(rb == -1){
				perror("Reading from file error\n");
				exit(EXIT_FAILURE);
			}
			
			if(rb == 0){
				printf("\n(PRODUCENT) Zakończono czytanie pliku.\n");
				break;
			}
			
		int onTerminal = 0;
		while(onTerminal < rb){
		
			char * przesuniecie = bufZ + onTerminal; 
			printf("\n");
			printf("\n(PRODUCENT) Do potoku zostaje wysłany towar:\n");
			printf("\n");
			int printed = write(STDOUT_FILENO, przesuniecie, rb - onTerminal); 
				if(printed == -1){
					perror("Writing on terminal error");
					exit(EXIT_FAILURE);
				}
			onTerminal += printed;
			printf("\n");
			
		}
		
		int inPipe = 0;
		while(inPipe < rb){
		
			char * przesuniecie = bufZ + inPipe;
			int send = write(pipefd[1], przesuniecie, rb - inPipe);
				if(send == -1){
					perror("Writing to pipe error");
					exit(EXIT_FAILURE);
				}
			inPipe += send;	
		}
		srand(time(0));
		int random = ((rand() % 6) +1);
		printf("\n(PRODCENT) Śpię: %d\n", random);
		sleep(random);
	}
 
	if(close(fd)){
		perror("Closing fd error");
		exit(EXIT_FAILURE);
	}
	
	if(close(pipefd[1])){
		perror("Closing pipefd error");
		exit(EXIT_FAILURE);
	}
}


void consumer(char * output){
	 
	 if(close(pipefd[1])){
	 	perror("Closing writing pipe's side error");
	 	exit(EXIT_FAILURE);
	 }
	 
	 int fd = open(output, O_WRONLY | O_CREAT | O_EXCL, 0664);
	 
		if(fd == -1){
			perror("Opening file error");
			exit(EXIT_FAILURE);
		}
		
	char bufR[BUFR_LEN];
	while(1){
		
		int rb = read(pipefd[0],bufR, BUFR_LEN);
			if(rb == -1){
				perror("Reading from pipe error");
				exit(EXIT_FAILURE);
			}

			if(rb == 0){				
				printf("\nZakończono pobieranie surowca z potoku i jego zapis.\n");
				break;
			}
			
		int onTerminal = 0;
		while(onTerminal < rb){
			char * przesuniecie = bufR + onTerminal;
			
			printf("\n");
			printf("\n(KONSUMENT) Pobiera dane/ surowiec z potoku:\n");
			printf("\n");
			
			int printed = write(STDOUT_FILENO, przesuniecie, rb - onTerminal);
				if(printed == -1){
					perror("Writing on terminal error");
					exit(EXIT_FAILURE);
				}
			printf("\n");
			onTerminal += printed;
		}
		
		
		int inFile=0;
		while(inFile < rb){
			char * przesuniecie = bufR + inFile;
			int send = write(fd, przesuniecie, rb - inFile);
				if(send == -1){
					perror("Writing to fd error");
					exit(EXIT_FAILURE);
				}
			inFile += send;
		}
		
		srand(time(NULL));
		int randomC = ((rand() % 4) + 1);
		printf("\n(KONSUMENT) Spię: %d\n", randomC);
		sleep(randomC);
	
	}
	
	if(close(fd) == -1){
		perror("Closing file error");
		exit(EXIT_FAILURE);
	}
	
	if(close(pipefd[0]) == -1){
		perror("Closing pipe's reading side error");
		exit(EXIT_FAILURE);
	}
}


int main(int argc, char* argv[]){// ./prog in out

	if(argc != 3){
		printf("Błąd w podaniu argumentów! Proszę podać nazwy plikow do czytania oraz pisania");
		exit(-3);
	}
	
	
	if(pipe(pipefd) == -1){
		perror("Creating pipe error");
		exit(EXIT_FAILURE);
	}
	
	
	int childPID = fork();
	int stat;
	
	switch(childPID){
	
		 case -1:
			perror("Fork error!");
			exit(-1);
			break;
		
		 case 0:
			consumer(argv[2]);
			break;
		
		 default:
			sleep(2);
			 int childExited = waitpid(childPID,&stat,WNOHANG);
				if( childExited == -1){
					perror("Waitpid error");
					exit(EXIT_FAILURE);
				}
				
				if( childExited == childPID){
					exit(EXIT_FAILURE);
				}
				
			producer(argv[1]);
			
			int stat_loc;
			if(wait(&stat_loc) == -1){
				perror("Wait error");
				exit(EXIT_FAILURE);
			}
			
			
			break;
	}
	
 return 0;
}

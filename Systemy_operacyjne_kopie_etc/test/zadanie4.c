#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define LIMIT 6

int main(int argc, char *argv[]){
	
	int filedes[2];
	pipe(filedes);
	char znak[LIMIT];
	int bajty1, bajty2;

	if(argc !=3){
		printf("Nieprawidlowa liczba argumentow\n");
		exit(EXIT_FAILURE);
	}
	
	int plik1 = open(argv[1],O_RDONLY , 0);
	int plik2 = open(argv[2], O_WRONLY | O_CREAT,0666);
	
	srand(time(0));
	
	switch(fork()){
		case -1:
			perror("fork error");
			exit(EXIT_FAILURE);
			exit(1);
		case 0:
			close(filedes[1]);
			printf("Jestem konsumentem, zapisuje dane do pliku\n");
			sleep(1);
			while((read(filedes[0], znak,sizeof(znak)))>0){
				sleep(rand()%10);
				write(plik2, znak, 6);
				printf("Konsument umieszcza w pliku: %s \n",znak);}
			close(filedes[0]);
			close(plik2);
		
			_exit(2);
		default:
			close(filedes[0]);
			printf("Jestem producentem, czytam dane z pliku\n");
		
			while((read(plik1, znak, sizeof(znak)))>0){
				sleep(rand()%10);
				write(filedes[1], znak, 6);
				printf("Producent pobral z pliku: %s \n",znak);
				sleep(1);
			}
			close(filedes[1]);
			close(plik1);
			wait(NULL);
			break;
	}

	return 0;
}


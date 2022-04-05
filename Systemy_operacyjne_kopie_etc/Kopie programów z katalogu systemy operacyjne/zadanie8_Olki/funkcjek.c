#include "funkcjek.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>


void mqCreate(const char *name){
	struct mq_attr atrr;
	
	atrr.mq_flags = 0;
	atrr.mq_maxmsg = 5; // maksymalna liczba komunikatow w kolejce
	atrr.mq_msgsize = 20; // maksymalny rozmiar komunikatu(w bajtach)
	atrr.mq_curmsgs = 0;
	
	mqd_t fd;
	
	fd = mq_open(name,O_CREAT|O_RDWR, 0777, &atrr); // otwieranie kolejki
	if(fd == -1){
		perror("MQ_OPEN ERROR");
		exit(EXIT_FAILURE);
	}
	if(mq_close(fd) == -1){
		perror("MQ_CLOSE ERROR");
		exit(EXIT_FAILURE);
	}
}

void mqClose(mqd_t fd){
	if(mq_close(fd) == -1){
		perror("MQ_CLOSE ERROR");
		exit(EXIT_FAILURE);
	}
}

void mqRemove(const char *name){
	if(mq_unlink(name) == -1){
		perror("MQ_UNLINK ERROR");
		exit(EXIT_FAILURE);
	}
}

mqd_t mqOpen(const char *name, int flag){
	int fd;
	fd = mq_open(name, flag);
	
	if(fd == -1){
		perror("MQ_OPEN ERROR");
		exit(EXIT_FAILURE);
	}
	
	return fd;
}

void mqGetattr(mqd_t fd){
	struct mq_attr atrr;
	
	if(mq_getattr(fd, &atrr) == -1){
        perror("MQ_GETATTR ERROR");
        exit(EXIT_FAILURE);
    }
    
    printf("Flaga kolejki: %ld\n", atrr.mq_flags);
    printf("Maksymalna liczba komunikatow w kolejce: %ld\n",atrr.mq_maxmsg);
    printf("Maksymalny rozmiar komunikatu: %ld\n", atrr.mq_msgsize);
    printf("Aktualna liczba komunikatow w kolejce: %ld\n", atrr.mq_curmsgs);
    printf("\n");
}

void mqReceive(mqd_t fd, char *bufor, size_t rozmiar){
	if(mq_receive(fd, bufor, rozmiar, 0) == -1){
        perror("MQ_RECIVE ERROR");
        exit(EXIT_FAILURE);
    }
}

void mqSend(mqd_t fd, char *bufor, size_t rozmiar){
	if(mq_send(fd, bufor, rozmiar, 0) == -1){
        perror("MQ_SEND ERROR");
        exit(EXIT_FAILURE);
    }
}



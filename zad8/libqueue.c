/* 
   Program zawierajacy funkcje do operowania na Kolejkach komunikatow.
   --------------------------------------------------------------------
   Autor: Tomasz Rozycki                                    26.05.2021
   --------------------------------------------------------------------
*/

//makra

//biblioteki dolaczane
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <mqueue.h>
#include "libqueue.h"


mqd_t que_creat(const char *name, int oflag, struct mq_attr *attr){ //tworzenie kolejki komunikatow
    mqd_t mqdes;
    if((mqdes = mq_open(name, oflag, 0666, attr)) == (mqd_t)-1){ 
        perror("que_creat error\n");
        _exit(EXIT_FAILURE);
    }
    return mqdes;
}

mqd_t que_open(const char *name, int oflag){ //otwieranie kolejki komunikatow
    mqd_t mqdes;
    if((mqdes = mq_open(name, oflag)) == (mqd_t)-1){ 
        perror("que_open error\n");
        exit(EXIT_FAILURE);
    }
    return mqdes;
}

void que_close(mqd_t mqdes){ //zamykanie kolejki komunikatow
    if(mq_close(mqdes) == -1){
        perror("que_ error\n");
        exit(EXIT_FAILURE);
    }
}

void que_unlink(const char *name){ //usuwanie kolejki komunikatow
    if(mq_unlink(name) == -1){
        perror("que_unlink error\n");
        exit(EXIT_FAILURE);
    }
    printf("Usunieto kolejke komunikatow\n");
}

/*void que_getvalue(mqd_t mqdes, struct mq_attr *attr){ //uzyskiwanie artrybutow kolejki komunikatow
     if(mq_getattr(mqdes, attr) == -1){
        perror("que_getvalue error\n");
        exit(EXIT_FAILURE);
    }
} 

void que_setvalue(mqd_t mqdes, struct mq_attr *newattr, struct mq_attr *oldattr){ //ustawianie artrybutow kolejki komunikatow
    if(mq_setattr(mqdes, newattr, oldattr) == -1){
        perror("que_setvalue error\n");
        exit(EXIT_FAILURE);
    }
} */

void que_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len){ 
    if(mq_send(mqdes, (char *)msg_ptr, msg_len, 1) == -1){ //wysylanie komunikatow do kolejki komunikatow, o priorytecie 1
        perror("que_send error\n");
        exit(EXIT_FAILURE);
    }
}

void que_receive(mqd_t mqdes, const char *msg_ptr, size_t msg_len){
    if(mq_receive(mqdes, (char *)msg_ptr, msg_len, NULL) == -1){ //odbieranie komunikatow z kolejki komunikatow
         perror("que_receive error\n");
        exit(EXIT_FAILURE);
    }
}

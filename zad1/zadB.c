#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    
    printf("UID: %d, GID: %d, PID: %d, PPID: %d, PGID: %d.\n", getuid(),getgid(),getpid(),getppid(),getpgrp());
    
    int id;
    
    for(int i=0;i<3;i++){
        id = fork();
        if(id==-1) //blad
        {
            perror("FORK ERROR");
            exit(EXIT_FAILURE);
        }
        if(id==0) //proces potomny
        {
            printf("UID: %d, GID: %d, PID: %d, PPID: %d, PGID: %d.\n", getuid(),getgid(),getpid(),getppid(),getpgrp());
        }
        else //czyli PID procesu macierzystego
        {
       
            wait(NULL); //macierzysty czeka na zakonczenie potomnych, zrobic obsluge bledow
        }
    
    }
            
    exit(EXIT_SUCCESS);    
}


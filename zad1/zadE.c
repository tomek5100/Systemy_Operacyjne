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
        if(id==0) //potomny
        {
            setpgid(0,0); //proces potomny staje sie liderem swojej wlasnej grupy czyli PGID, zrobic obsluge bledow
            printf("UID: %d, GID: %d, PID: %d, PPID: %d, PGID: %d.\n", getuid(),getgid(),getpid(),getppid(),getpgrp());
        }
        else //macierzysty
        {
       
            wait(NULL); //macierzysty czeka na zakonczenie potomnych
        }
    
    }
            
    exit(EXIT_SUCCESS);    
}


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
            printf("UID: %d, GID: %d, PID: %d, PPID: %d, PGID: %d.\n", getuid(),getgid(),getpid(),getppid(),getpgrp());
            sleep(1); //dzieki temu procesy wypisza sie chronologicznie
        }
        else //macierzysty
        {
       
             //macierzysty czeka na zakonczenie potomnych, tutaj kazal usunac
        }
    
    }
    sleep(4);      //to kazal dopisac
    exit(EXIT_SUCCESS);    
}

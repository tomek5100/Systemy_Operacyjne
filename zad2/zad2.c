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
        if(id==-1)
        {
            perror("FORK ERROR");
            exit(EXIT_FAILURE);
        }
        if(id==0)
        {	//kazal zrobic sciezke niejawna, czyli nie wpisaywac nazw plikow
            execl("./zadA.x","zadA.x",NULL); //nazwa sciezkowa, argument zerowy(plik wykonawczy), NULL
            perror("execl erorr"); //obsluga bledow
            _exit(2);
        }
        else
        {
       
            wait(NULL);
        }
    
    }
            
    exit(EXIT_SUCCESS);    
}


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    
    printf("UID: %d, GID: %d, PID: %d, PPID: %d, PGID: %d.\n", getuid(),getgid(),getpid(),getppid(),getpgrp());
    int pid = getpid();
   
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
            sleep(1); //tutaj mi kazal dopisac
            printf("UID: %d, GID: %d, PID: %d, PPID: %d, PGID: %d.\n", getuid(),getgid(),getpid(),getppid(),getpgrp());
           
        }
        else //macierzysty
        {
       
            //sleep(2); //macierzysty czeka na zakonczenie potomnych
        }
    
    }
     
     int pid2 = getpid();
     if(pid2==pid){    //sprawdzam czy to jest proces macierzysty
      	_exit(0);	//zabijam macierzysty, funkcja systemd adoptuje procesy potomne
      	}
     else if(pid2!=pid){
      	//sleep(10); //usypiam potomkow aby miec czas na wyswietlenie drzewa pstree -p
      } 
       	
            
    exit(EXIT_SUCCESS);    
}

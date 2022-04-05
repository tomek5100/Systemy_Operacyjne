#define _POSIX_C_SOURCE 200112L //unikam ostrzezenia o niezadeklarowaniu kill
#define _XOPEN_SOURCE 500 //unikam bledu o funkcji getpgid

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
int x=0;

void handle_sigusr1(int sig){
    if (x==0){
        printf("text");
    }}

int main(int argc, char* argv[])
{
    int pid = fork();

    if(pid==-1){
        return 1;
    }

    if(pid==0){
        //child process
        sleep(5);
        kill(getppid(), SIGUSR1);
    }else{
        struct sigaction sa;
        sa.sa_flags = SA_RESTART;
        sa.sa_handler = &handle_sigusr1;
        sigaction(SIGUSR1, &sa, NULL);

        printf("text");
        scanf("%d",&x);
        if(x==15){
            printf("Right");
        }
        else{
            printf("Wrong");
        }
        wait(NULL);
    }

    return 0;
}

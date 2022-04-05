#define _POSIX_C_SOURCE 200112L //unikam ostrzezenia o niezadeklarowaniu kill
#define _XOPEN_SOURCE 500 //unikam bledu o funkcji getpgid

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
 
int main(int argc, char* argv[]){ 
	int pid = fork();
		printf("pid:%d\n",pid); //rodzica i potomka
	if(pid == -1){
	return 1;
	}
	
	if (pid ==0){
	while(1){
		printf("text\n");
		usleep(50000);
	}
	}
	else{
	sleep(1);
	printf("pid:%d\n",pid); 
	//czyli wysylajac pid do funkcji kill przesylam pid rodzica, a SIGKILL zabija potomka
	kill(pid, SIGKILL);
	wait(NULL);
	
	}
	return 0;
}

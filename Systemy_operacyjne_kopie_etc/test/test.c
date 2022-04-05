#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){ 
int pida, pidb;
int status;
pida = fork();
if (pida == 0){ /* proces potomny */
sleep(10);
exit(7);
}
/* proces macierzysty */
printf("Mam przodka o identyfikatorze %d\n", pida);
kill(pida, 9);
pidb = wait(&status);
printf("Status zakonczenia procesu %d: %x\n", pidb, status);
}

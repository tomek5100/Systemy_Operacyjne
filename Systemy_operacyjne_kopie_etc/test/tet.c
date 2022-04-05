#include <stdio.h>
#include <limits.h>
#include <sys/param.h>

int main(){
printf("%d\n",PIPE_BUF); //maksymalny rozmiar przeslanych danych do buforu PIPE BUFOR
return 0;
}

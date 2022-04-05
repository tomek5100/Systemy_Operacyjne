//=================================================================================
// Author: Dawid Dobrowolski SO Group 7,
//=================================================================================
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define RIGHTS 0666 // Rights Access - 0666 means that everbody has read and write access
#define S_MAX 1024 // Size of static array

char buff[S_MAX];

int main(int argc, char* argv[])
{
  int file_r,file_w,read_p;

  //  In case of wrong number of called arguments, program will end with failure status
  if(argc != 3)
    {
      printf("%s",argv[0]);
      exit(1);
    }

  //  open function return value -1 when returns an error OR non-negative intiger
  //  when the operation is successful
  //  O_RDONLY flag specifies read permission
  if((file_r = open(argv[1], O_RDONLY)) ==  -1)
    {
      perror("Open function error");
      exit(1);
    }

  //  O_CREAT flag create a file if the pathname does not exist
  //  RIGHTS mode defines the rights access
  //  O_WRONLY flag specifies write permission
  if((file_w  = open(argv[2],O_WRONLY | O_CREAT,RIGHTS)) ==  -1)
    {
      perror("Open function error");
      exit(1);
    }

    // the loop will end when there is nothing left to read
    while((read_p =read(file_r, buff, S_MAX)) > 0 )
    if(write(file_w, buff, read_p) !=  read_p)
    {
      perror("Transfer data error");
      exit(1);
    }

    // Close a file descriptors so that is no longer refers to any file.
    // Its return -1 when function returns an error.
    if((close(file_r))  ==  -1)
    {
      perror("Close function error");
      exit(1);
    }

    if((close(file_w))  ==  -1)
    {
      perror("Close function error");
      exit(1);
    }

    exit(0);
}

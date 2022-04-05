#ifndef FUNKCJEK
#define FUNKCJEK

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <mqueue.h>

#define SERWER "/kolejkaserwer"

void mqCreate(const char *name);

void mqClose(mqd_t fd);

void mqRemove(const char *name);

mqd_t mqOpen(const char *name, int flag);

void mqGetattr(mqd_t fd);

void mqReceive(mqd_t fd, char *bufor, size_t rozmiar);

void mqSend(mqd_t fd, char *bufor, size_t rozmiar);



#endif

/* 
   Plik naglowkowy
   --------------------------------------------------------------------
   Autor: Tomasz Rozycki                                    26.05.2021
   --------------------------------------------------------------------
*/
#ifndef LIBQUEUE_H
#define LIBQUEUE_H
#include <mqueue.h>

//nazwa kolejki
#define NAZWAKOLEJKI "/kolejka"

mqd_t que_creat(const char *name, int oflag, struct mq_attr *attr); //tworzenie kolejki komunikatow
mqd_t que_open(const char *name, int oflag); //otwieranie kolejki komunikatow
void que_close(mqd_t mqdes); //zamykanie kolejki komunikatow
void que_unlink(const char *name); //usuwanie kolejki komunikatow
//void que_getvalue(mqd_t mqdes, struct mq_attr *attr); //uzyskiwanie artrybutow kolejki komunikatow
//void que_setvalue(mqd_t mqdes, struct mq_attr *newattr, struct mq_attr *oldattr); //ustawianie artrybutow kolejki komunikatow
void que_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len); //wysylanie komunikatow do kolejki komunikatow 
void que_receive(mqd_t mqdes, const char *msg_ptr, size_t msg_len); //odbieranie komunikatow z kolejki komunikatow
#endif

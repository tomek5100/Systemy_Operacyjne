/* 
   Plik naglowkowy
   --------------------------------------------------------------------
   Autor: Tomasz Rozycki                                    11.05.2021
   --------------------------------------------------------------------
*/
#ifndef LIB_H
#define LIB_H

sem_t tworzenie(const char *name, int value); 
sem_t *otwieranie(const char *name);
void wartosc(sem_t *sem);
void opusc(sem_t *sem);
void podnies(sem_t *sem);
void zamykanie(sem_t *sem);
void usuwanie(const char *name);
#endif

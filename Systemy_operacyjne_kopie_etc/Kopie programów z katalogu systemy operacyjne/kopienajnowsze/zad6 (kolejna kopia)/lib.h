/* 
   Plik naglowkowy
   --------------------------------------------------------------------
   Autor: Tomasz Rozycki                                    11.05.2021
   --------------------------------------------------------------------
*/
#ifndef LIB_H
#define LIB_H

void tworzenie(const char *name, int value); 
sem_t *otwieranie(const char *name);
void wartosc(sem_t *S);
void opusc(sem_t *S);
void podnies(sem_t *S);
void zamykanie(sem_t *S);
void usuwanie(const char *name);
#endif

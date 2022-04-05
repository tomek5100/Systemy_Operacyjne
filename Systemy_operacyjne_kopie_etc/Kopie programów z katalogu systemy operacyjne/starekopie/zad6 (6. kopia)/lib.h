/* 
   Plik naglowkowy
   --------------------------------------------------------------------
   Autor: Tomasz Rozycki                                    11.05.2021
   --------------------------------------------------------------------
*/
#ifndef LIB_H
#define LIB_H

typedef sem_t *semid;

semid sem;
semid tworzenie(const char *name, int value); 
semid otwieranie(const char *name);
void wartosc(semid sem);
void opusc(semid sem);
void podnies(semid sem);
void zamykanie(semid sem);
void usuwanie(const char *name);

#endif

/* 
   Plik naglowkowy
   --------------------------------------------------------------------
   Autor: Tomasz Rozycki                                    11.05.2021
   --------------------------------------------------------------------
*/
#ifndef LIB_H
#define LIB_H

typedef sem_t *semid;

semid adres; //zmienna trzymajaca adres semafora, semid to TYP zmiennej
semid tworzenie(const char *name, int value); //chyba usunac int value
semid otwieranie(const char *name);
void wartosc(semid adres);
void opusc(semid adres);
void podnies(semid adres);
void zamykanie(semid adres);
void usuwanie(const char *name);

#endif

/* 
   Plik naglowkowy
   --------------------------------------------------------------------
   Autor: Tomasz Rozycki                                    22.05.2021
   --------------------------------------------------------------------
*/
#ifndef LIBMEM_H
#define LIBMEM_H
int mem_creat(const char *name); //tworzenie obiektu pamieci dzielonej
int mem_open(const char *name); //otwieranie obiektu pamieci dzielonej
void mem_ftruncate(int fd, off_t lenght); //ustawianie rozmiaru dla obiektu pamieci dzielonej
void mem_close(int fd); //zamykanie pamieci dzielonej
void mem_del(const char *name); //usuwanie obiektu pamieci dzielonej
void* mem_map(int fd, int size); //odwzrowowywanie obiektu pamieci dzielonej w witrualna przestrzen adresowa procesu
void mem_munmap(void *adres, int size); //usuwanie odwzorowania obszaru pamieci dzielonej
//int mem_info(int fd, struct stat *statbuf); //funkcja do wypisywania rozmiaru obiektu p.d
#endif

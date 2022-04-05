#ifndef semaphores
#define semaphores

sem_t * semaphore_init(char *name, int val);
sem_t * semaphore_open(char *name);
void semaphore_wait(sem_t *semaphore);
void semaphore_post(sem_t *semaphore);
void semaphore_close(sem_t *semaphore);
void semaphore_unlink(char *name);
int semaphore_value(sem_t *semaphore);

#endif
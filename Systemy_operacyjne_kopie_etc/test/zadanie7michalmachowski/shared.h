#ifndef shared
#define shared

int sminit(char *name);
int smopen(char *name);
void smsetlength(int fd, int len);
void smclose(int fd);
void smunlink(char *name);
char * smmap(int len, int fd);
void smunmap(void *ptr, int len);

#endif
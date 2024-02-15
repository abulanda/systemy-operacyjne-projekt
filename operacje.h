#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/errno.h>
int alokujSem(key_t klucz, int number, int flagi);
void inicjalizujSem(int semID, int number, int val);
int zwolnijSem(int semID, int number);
int PN(int semID, int number, int flags, int n);
int VN(int semID, int number, int n);

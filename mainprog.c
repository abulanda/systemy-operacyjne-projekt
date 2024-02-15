#include <stdio.h>
#include "operacje.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <sys/sem.h>
#include <sys/wait.h>
#define P 20
#define MAX 10
#define MAX2 20
#define D 2

int main()
{
    key_t kls, klm;
    int semID;
    int N = 1;
    int i;
    int shmID;
    int* bufor;

    if ((kls = ftok(".", 'A')) == -1)
    {
        perror("Blad ftok (main)");
        exit(1);
    }

    semID = alokujSem(kls, N, IPC_CREAT | IPC_EXCL | 0666);
    inicjalizujSem(semID, 0, P);
    printf("Semafor gotowy!\n");

    klm = ftok(".", 'B');

    if ((shmID = shmget(klm, MAX2 * sizeof(int), IPC_CREAT | 0666)) == -1)
    {
        perror("Blad shmget");
        exit(1);
    }

    bufor = (int*)shmat(shmID, NULL, 0);
    if (bufor == (int*)-1)
    {
        perror("Blad shmat");
        exit(1);
    }

    bufor[0] = 0;

    fflush(stdout);

    for (i = 0; i < P; i++)
    {
        switch (fork())
        {
        case -1:
            perror("Blad fork (mainprog)");
            exit(2);
        case 0:
            execl("./czyt", "czyt", NULL);
        }
    }

    for (i = 0; i < P; i++)
    {
        switch (fork())
        {
        case -1:
            perror("Blad fork (mainprog)");
            exit(2);
        case 0:
            execl("./pisz", "pisz", NULL);
        }
    }

    //for(i=0; i<D; 

    for (i = 0; i < 2 * P; i++)
        wait(NULL);

    zwolnijSem(semID, 0);

    shmctl(shmID, IPC_RMID, NULL);

    printf("MAIN: KONIEC.\n");

    return 0;
}

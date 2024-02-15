#include <stdio.h>
#include "operacje.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <sys/sem.h>
#define MAX 22
#define MAX2 20

int main()
{
	key_t kluczs, kluczm;
	int semID;
	int shmID;
	int* bufor;
	int N = 1;
	int i;
	int semnumer;
	int M = 20;

	printf("piszacy----------------\n");

	if ((kluczs = ftok(".", 'A')) == -1)
	{
		printf("Blad ftok (A) \n");
		exit(2);
	}

	semID = alokujSem(kluczs, N, IPC_CREAT | 0666);

	if (semID == -1)
	{
		printf("blad semafora(pisz)\n");
		exit(1);
	}

	kluczm = ftok(".", 'B');

	shmID = shmget(kluczm, MAX2 * sizeof(int), IPC_CREAT | 0666);

	if (shmID == -1)
	{
		printf("blad pamieci dzielonej\n");
		exit(1);
	}

	bufor = (int*)shmat(shmID, NULL, 0);
	semnumer = 0;
	int bufindex = getpid() % M;
	//bufor[bufindex]=getpid()
	PN(semID, semnumer, bufindex, M);
	bufor[bufindex] = getpid();
	printf("proces piszacy o PID %d zapisuje do bufora %d: %d\n", getpid(), bufindex, bufor[bufindex]);
	sleep(1);
	VN(semID, semnumer, M);

	return 0;
}

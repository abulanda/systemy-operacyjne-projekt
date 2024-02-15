#include <stdio.h>
#include "operacje.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
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

	printf("czytajacy----------------\n");

	if ((kluczs = ftok(".", 'A')) == -1)
	{
		printf("Blad ftok (A) \n");
		exit(2);
	}

	semID = alokujSem(kluczs, N, IPC_CREAT | 0666);

	if (semID == -1)
	{
		printf("blad semafora - czyt\n");
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

	for (i = 0; i < M; i++)
	{
		PN(semID, semnumer, i, 1);
		sleep(1);
		printf("proces czytajacy o PID %d odczytal z bufora %d: %d\n", getpid(), i, bufor[i]);
		sleep(1);
		VN(semID, semnumer, 1);
		//sleep(1);
	}

	return 0;
}

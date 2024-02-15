#include "operacje.h"
int alokujSem(key_t klucz, int number, int flagi)
{
	int semID;
	if ((semID = semget(klucz, number, flagi)) == -1)
	{
		perror("Blad semget (alokujSemafor): ");
		exit(1);
	}
	return semID;
}
int zwolnijSem(int semID, int number)
{
	return semctl(semID, number, IPC_RMID, NULL);

}
void inicjalizujSem(int semID, int number, int val)
{
	if (semctl(semID, number, SETVAL, val) == -1)
	{
		perror("Blad semctl (inicjalizujSemafor): ");
		exit(1);
	}

}

int PN(int semID, int number, int flags, int n)
{
	int result;
	struct sembuf operacje[1];
	operacje[0].sem_num = number;
	operacje[0].sem_op = -n;
	operacje[0].sem_flg = 0 | flags;
	if (semop(semID, operacje, 1) == -1)
	{
		perror("Blad semop(PN)");
		return -1;
	}

	return 1;
}
int VN(int semID, int number, int n)
{
	struct sembuf operacje[1];
	operacje[0].sem_num = number;
	operacje[0].sem_op = n;
	//operacje.sem_flg = n;
	if (semop(semID, operacje, 1) == -1)
	{
		perror("Blad semop(VN)");
		return -1;

	}
	return;
}

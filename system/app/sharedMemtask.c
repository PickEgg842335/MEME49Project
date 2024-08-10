#include "./include/app.h"

key_t key;
int shmid;
int *shared_array;

void sfinitialsharedMem(void)
{
    key = ftok("shmfile", 65);
    shmid = shmget(key, 10 * sizeof(int), 0666 | IPC_CREAT);
    shared_array = shmat(shmid, (void*)0, 0);
}

void sfsharedMemTask(void)
{

}
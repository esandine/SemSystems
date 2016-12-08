#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <sys/shm.h>

union semun{
  int val;
  struct semi_ds *buf;
  unsigned short *arry;
  struct seminfo *_buf;
};

int createSemaphore(int* sc){
  int key = ftok("makefile", 22);
  int semid;
  //Creates Semaphore
  semid = semget(key, 1, IPC_CREAT | 0644);
  printf("semaphore created %d\n", semid);
  union semun su;
  su.val = 10;
  //setting semaphore value
  *sc = semctl(semid, 0, SETVAL, su);
  printf("value set: %d\n", *sc);
  return semid;
}

int createShmem(int *sh){
  int key = ftok("makefile", 22);
  int shmid;
  //Creates Shmem
  shmid = shmget(key, 4, IPC_CREAT | 0644);
  printf("shmem created %d\n", shmid);
  //setting shmemaphore value
  sh = (int*)shmat(shmid, 0, 0);
  printf("mem attatched: %d\n", *sh);
  return shmid;
}
int main(int argc, char *argv[]){
  int semid;
  int shmid;
  int sc;
  int *sh;
  int key = ftok("makefile", 22);
  if (strncmp(argv[1], "-c", strlen(argv[1])) == 0){
    semid = createSemaphore(&sc);
    shmid = createShmem(sh);
  }
  else if (strncmp(argv[1], "-v", strlen(argv[1])) == 0){
    semid = semget(key, 1, 0);
    //getting the value of a semaphore
    sc = semctl(semid, 0, GETVAL);

    printf("semaphore value: %d\n",sc);
  }
  else if(strncmp(argv[1], "-r", strlen(argv[1])) == 0){
    semid = semget(key, 1, 0);
    //removing a semaphore
    sc = semctl(semid, 0, IPC_RMID);
    printf("semaphore removed: %d\n", sc);
  }
  return 0;

}

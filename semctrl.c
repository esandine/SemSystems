#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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
  semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0644);
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
  return shmid;
}

int openFile(){
  int fd;
  if( access( "story.txt", F_OK ) != -1 ) {
    fd = open("story.txt", O_RDWR | O_TRUNC);
  }
  else{
    fd = open("story.txt", O_RDWR | O_CREAT, 0644);
  }
  printf("File opened, file descriptor: %d\n",fd);
  return fd;
}

int removeSemaphore(){
  int key = ftok("makefile", 22);
  int semid = semget(key, 1, 0);
  //removing a semaphore
  int sc;
  union semun su;
  sc = semctl(semid, 0, IPC_RMID);
  printf("semaphore removed: %d\n", sc);
  return sc;
}

int removeShmem(){
  int key = ftok("makefile", 22);
  int shmid = shmget(key, 0, 0);
  struct shmid_ds d;
  shmctl(shmid, IPC_RMID, &d);
  printf("shared memory removed: %d\n", shmid);
  return shmid;
}

int closeFile(int fd){
  close(fd);
  printf("File closed, file descriptor: %d\n",fd);
  return fd;
}

int main(int argc, char *argv[]){
  umask(0000);
  int semid;
  int shmid;
  int sc;
  int *sh;
  int fd = openFile();
  int key = ftok("makefile", 22);
  if (strncmp(argv[1], "-c", strlen(argv[1])) == 0){
    semid = createSemaphore(&sc);
    shmid = createShmem(sh);
  }
  else if (strncmp(argv[1], "-v", strlen(argv[1])) == 0){
    char buf[256];
    printf("Lseek %ld\n",lseek(fd, 0, SEEK_SET));
    read(fd, buf, 256);
    printf("%s\n",buf);
  }
  else if(strncmp(argv[1], "-r", strlen(argv[1])) == 0){
    removeSemaphore();
    removeShmem();
    closeFile(3);
  }
  return 0;

}

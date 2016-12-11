#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>

union semun {	
	int val;
	struct semi_ds *buf;
	unsigned short *array;
	struct seminfo *_buf;
};

int main (){
  umask(0000);
  int key = ftok("makefile", 22);//generates the key
  int semid = semget(key, 1, 0);//gets the semaphore id
  int shmid = shmget(key, 4, 0);//gets the shared mem id
  int *length = shmat(shmid, 0, 0);;//gets the length of the last line
  int sval;
  char fbuf[*length + 100];//buffer for fgets
  int fd;

  union semun su;
  sval = semctl(semid, 0, GETVAL, su);

  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;

  //start if semaphore not 0.
  semop(semid, &sb, 1);

  if( access( "story.txt", F_OK ) != -1 ) {
    fd = open("story.txt", O_RDWR);
    if(lseek(fd, 0, SEEK_END) == 0){
      
    }
    else{
      lseek(fd, *length * -1, SEEK_END);
      char buf[*length];//buffer for reading
      read(fd, buf, *length);
      *strchr(buf, '\n') = 0;
      printf("Last line:\n%s\n", buf);
    }
  }
  else{
    *length = 0;
    fd = open("story.txt", O_RDWR | O_CREAT, 0644);
    printf("File created\n");
  }
  printf("Next line:\n");
  fgets(fbuf, *length + 100, stdin);

  int i = 0;
  while(fbuf[i] != '\n'){
    i++;
  }
  fbuf[i + 1] = 0;
  i = 0;
  while(fbuf[i]){
    i++;
  }
  write(fd, fbuf, i);
  *length = i;//update shmem with length of line added, including the newline character
  
  close(fd);
  semop(semid, &sb, 1);
  //semaphore reset to 1
  return 0;
}

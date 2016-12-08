#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(){
  srand( time(NULL));
  int x = random()%10;
  int semid= semget(ftok("makefile",22),1,0);
  printf("[%d] before access\n", getpid());

  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;
  
  semop(semid, &sb, 1);
  printf("[%d] I'm ni!\n", getpid());

  sleep(x);

  sb.sem_op = 1;
  semop(semid, &sb, 1);
  printf("[%d] Im done\n", getpid());
  return 0;
}




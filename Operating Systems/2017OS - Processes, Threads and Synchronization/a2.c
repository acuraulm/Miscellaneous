#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/sem.h>

int sem_id,sem_id2,sem_id3,sem_id4; // semahpores identifiers
void create_semset(); // initializes 4 semaphores with value 0
void newProcess(int); // empty process, info(BEGIN / end)
void* thread_function_2(void*); // thread funtions used for
void* thread_function_3(void*);     // describing their behavior and
void* thread_function_6(void*);         //syncronization
void process2();
void process3();
void process4();
void process6();
void V(int, int);
void P(int, int);

int main(){
    init();
    create_semset();
//P1 start
    info(BEGIN, 1, 0);
    int P2id = fork();
    if(P2id == 0)
	{
    process2();
	}
//P1 end
    waitpid(P2id, NULL, 0);
    info(END, 1, 0);
    return 0;
}


void* thread_function_2(void *arg)
{
    int i = (intptr_t) arg;
    if(i == 4){
    info(BEGIN, 2, i);
    info(END, 2, i);
    V(sem_id3,0);
    }
    else if(i == 6){
    P(sem_id4,0);
    info(BEGIN, 2, i);
    info(END,2,i);
    }
    else{
    info(BEGIN, 2, i);
    info(END, 2, i);
    }
	return NULL;
}
void process2()
{
info(BEGIN, 2, 0);
    int i;
		int P3id = fork();
		if(P3id == 0){
			process3();
		}
		int P4id = fork();
		if(P4id == 0){
			process4();
		}
		int P7id = fork();
		if(P7id == 0){
			newProcess(7);
        }
    pthread_t th[6];
    for (i=1; i<7; i++)
    if (pthread_create(&th[i], NULL, (void* (*) (void*)) thread_function_2, (void*)(intptr_t) i) != 0) {
	    perror("Cannot create threads");
	    exit(i);
	  }
    for (int i=1; i<7; i++)
        if(pthread_join(th[i],NULL)!= 0){
        exit(i);
        }
    waitpid(P3id, NULL, 0);
    waitpid(P4id, NULL, 0);
    waitpid(P7id, NULL, 0);
info(END, 2, 0);
exit(2);
}
void* thread_function_3(void *arg)
{
    int i = (intptr_t) arg;
	info(BEGIN, 3, i);
	info(END, 3, i);
	return NULL;
}
void process3()
{
int i;
pthread_t th[51];
info(BEGIN, 3, 0);
for (i=1; i<51; i++)
	  if (pthread_create(&th[i], NULL, (void* (*) (void*)) thread_function_3, (void*)(intptr_t) i) != 0) {
	    perror("Cannot create threads");
	    exit(i);
	  }
    for (int i=1; i<51; i++)
        if(pthread_join(th[i],NULL)!= 0){
        exit(i);
        }
info(END, 3, 0);
exit(3);
}
void process4()
{
info(BEGIN, 4, 0);
	int P5id = fork();
	if(P5id == 0){
	info(BEGIN, 5, 0);
	int P6id = fork();
		if(P6id == 0){
                        process6();
			}
        waitpid(P6id, NULL, 0);
	info(END, 5, 0);
	exit(5);
	}
waitpid(P5id, NULL, 0);
info(END, 4, 0);
exit(4);
}
void* thread_function_6(void *arg)
{
    int i = (intptr_t) arg;

    if(i == 5){
    P(sem_id,0);
    info(BEGIN, 6, i);
    info(END, 6, i);
    V(sem_id2,0);
    }
    else if(i ==2){
    P(sem_id3,0);
    info(BEGIN, 6, 2);
    info(END,6, i);
    V(sem_id4,0);
    }
    else if(i == 1){
    info(BEGIN, 6, i);
    V(sem_id,0);
    P(sem_id2, 0);
    info(END,6,i);
    }
    else{
    info(BEGIN, 6, i);
    info(END, 6, i);
    }
	return NULL;
}
void process6()
{

info(BEGIN, 6, 0);
	int i;
	pthread_t th[6];
	for (i=1; i<6; i++)
	  if (pthread_create(&th[i], NULL, (void* (*) (void*)) thread_function_6, (void*)(intptr_t) i) != 0) {
	    perror("Cannot create threads");
	    exit(i);
	  }
    for (int i=1; i<6; i++)
        if(pthread_join(th[i],NULL)!= 0){
        exit(i);
        }
info(END, 6, 0);
exit(6);
}
void create_semset()
{
        sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
    if (sem_id < 0) {
        perror("Error creating the semaphore set");
        exit(2);
    }
    sem_id2 = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
    if (sem_id2 < 0) {
        perror("Error creating the semaphore set");
        exit(2);
    }
    sem_id3 = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
    if (sem_id2 < 0) {
        perror("Error creating the semaphore set");
        exit(2);
    }
    sem_id4 = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
    if (sem_id2 < 0) {
        perror("Error creating the semaphore set");
        exit(2);
    }
   semctl(sem_id, 0, SETVAL, 0);
   semctl(sem_id2, 0, SETVAL, 0);
   semctl(sem_id3, 0, SETVAL, 0);
   semctl(sem_id4, 0, SETVAL, 0);
}
void V(int semId, int semNr)
{
    struct sembuf op = {semNr, +1, 0};
    semop(semId, &op, 1);
}
void P(int semId, int semNr)
{
    struct sembuf op = {semNr, -1, 0};
    semop(semId, &op, 1);
}
void newProcess(int pNr)
{
info(BEGIN, pNr, 0);
info(END, pNr, 0);
exit(pNr);
}


/*
 ============================================================================
 Name        : Assignment1.c
 Author      : Daniel Webb
 Version     :
 Copyright   : Your copyright notice
 Description : Delegate shell commands via systemcall in child process
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main(void) {

	/*
	 * Initialize Arguments
	 */
	char * shared_memory;
	key_t shared_memory_key;
	int shared_memory_id;
	char * shared_memory_addr;
	char * shared_memory_file = "/tmp/shared_memory";
	char * sem_rec = "/receiver_semaphore";
	char * sem_proc = "/processor_semaphore";

	sem_t * semaphore_rec_id = sem_open(sem_rec, O_CREAT, S_IREAD|S_IWRITE, 0);
	sem_t * semaphore_proc_id = sem_open(sem_proc, O_CREAT, S_IREAD|S_IWRITE, 0);
	FILE * secrets_file;
	int num_count = 0;

	while(1){

		sem_wait(semaphore_rec_id);

		if(( shared_memory_key = ftok(shared_memory_file, 'x')) == -1){
			perror("ftok");
			exit(1);
		}

		if((shared_memory_id = shmget(shared_memory_key, 1024, 0644 | IPC_CREAT)) == -1){
			perror("shmget");
			exit(1);
		}
		if((shared_memory_addr = (char *) shmat(shared_memory_id, 0, 0)) == -1){
			perror("shmat");
			exit(1);
		}

		printf("Reading: %s\n", shared_memory_addr);

		num_count = 0;
		for( int i = 0; shared_memory_addr[i] != '\0'; i++){
			if (isdigit(shared_memory_addr[i])){
				num_count++;
			}
		}
		printf("Number of digits: %d\n", num_count);

		if((secrets_file = fopen("secrets.out", "a")) == NULL){
			printf("Could not open secrets.out file!\n");
			exit(1);
		}

		fprintf(secrets_file, "%d - %s\n", num_count, shared_memory_addr);
		fclose(secrets_file);

		shmdt(shared_memory_addr);

		sem_post(semaphore_proc_id);

	}
}
/*
 ============================================================================
 Name        : processor.c
 Author      : Daniel Webb
 Version     :
 Copyright   : Your copyright notice
 Description : Pull strings out of the shared memory and compute how many digits are in them
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

		// wait for the receiver to notify us
		sem_wait(semaphore_rec_id);

		// get shared memory key
		if(( shared_memory_key = ftok(shared_memory_file, 'x')) == -1){
			perror("ftok");
			exit(1);
		}

		// get id based on the key
		if((shared_memory_id = shmget(shared_memory_key, 1024, 0644 | IPC_CREAT)) == -1){
			perror("shmget");
			exit(1);
		}
		// attach to the memory
		if((shared_memory_addr = (char *) shmat(shared_memory_id, 0, 0)) == -1){
			perror("shmat");
			exit(1);
		}

		printf("Reading: %s\n", shared_memory_addr);

		num_count = 0;
		for( int i = 0; shared_memory_addr[i] != '\0'; i++){
			// if C00L starts at the current index, skip ahead of it so the 0's
			// in C00L are not counted
			if( strstr(&shared_memory_addr[i], "C00L") == &shared_memory_addr[i] ){
				i = i + 3;
				continue;
			}
			// if the character is a digit, add it to the count
			if (isdigit(shared_memory_addr[i])){
				num_count++;
			}
		}
		printf("Number of digits: %d\n", num_count);

		// write the number of digits and the full string to file (append only)
		if((secrets_file = fopen("secrets.out", "a")) == NULL){
			printf("Could not open secrets.out file!\n");
			exit(1);
		}

		fprintf(secrets_file, "%d - %s\n", num_count, shared_memory_addr);
		fclose(secrets_file);

		shmdt(shared_memory_addr);

		// tell the receiver that we are done
		sem_post(semaphore_proc_id);

	}
}

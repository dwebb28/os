/*
 ============================================================================
 Name        : receiver.c
 Author      : Daniel Webb
 Version     :
 Copyright   : Your copyright notice
 Description : Receive text from the user and send strings to shared memory only if
 	 	 	   they include the substring "C00L"
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
	size_t input_max = 100;
	char * input = NULL;
	char * shared_memory;
	key_t shared_memory_key;
	int shared_memory_id;
	char * shared_memory_addr;
	char * shared_memory_file = "/tmp/shared_memory";
	char * sem_rec = "/receiver_semaphore";
	char * sem_proc = "/processor_semaphore";

	sem_t * semaphore_rec_id = sem_open(sem_rec, O_CREAT, S_IREAD|S_IWRITE, 0);
	sem_t * semaphore_proc_id = sem_open(sem_proc, O_CREAT, S_IREAD|S_IWRITE, 0);

	while(1){

		input = (char *) malloc(input_max * sizeof(char));

		// get shared memory key
		if(( shared_memory_key = ftok(shared_memory_file, 'x')) == -1){
			perror("ftok");
			exit(1);
		}

		// get shared memory id associated with the above created key
		if((shared_memory_id = shmget(shared_memory_key, 1024, 0644 | IPC_CREAT)) == -1){
			perror("shmget");
			exit(1);
		}
		// get the address of the shared memory and attach to it
		if((shared_memory_addr = (char *) shmat(shared_memory_id, 0, 0)) == -1){
			perror("shmat");
			exit(1);
		}

		/*
		 * Initialize the input char array with null values
		 */
		for(int i = 0; i < input_max; i++)
		{
			input[i] = '\0';
		}

		/*
		 * Get the line of input from STDIN and remove the last char which
		 * will be a '\n' (newline) character
		 */
		printf("\nEnter some text (Ctrl+C to quit): ");
		getline(&input, &input_max, stdin);
		input[strlen(input) - 1] = '\0';

		//if C00L is a substirng of the user input, copy it to memory
		if( strstr(input, "C00L") != NULL ){
			memcpy(shared_memory_addr, input, strlen(input) + 1);

			printf("Success");
			// detach from the shared memory
			shmdt(shared_memory_addr);

			// tell the processor we are done and wait for the response from the processor
			sem_post(semaphore_rec_id);
			sem_wait(semaphore_proc_id);
		}else{
			printf("Not C00L");
		}


	}
}

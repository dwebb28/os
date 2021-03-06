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

int main(void) {

	while(1){

		/*
		 * Initialize Arguments
		 */
		size_t input_max = 100;
		size_t arg_index = 0;
		size_t num_args_allocated = 5;
		size_t max_arg_length = 100;
		char ** command_args = (char **) malloc( num_args_allocated * sizeof(char*));
		char * arg = NULL;
		int file_descriptors[2];
		char * input = (char *) malloc(input_max * sizeof(char));

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
		printf("\nEnter a command to run (Ctrl+C to quit): ");
		getline(&input, &input_max, stdin);
		input[strlen(input) - 1] = '\0';

		/*
		 * Get the first token of the argument input delimited by whitespace
		 */
		arg = strtok(input, " ");

		if(arg == NULL)
		{
			printf("No arguments provided.");
			continue;
		}

		/*
		 * Continue to get all arguments separated by whitespace from stdin
		 */
		while(arg != NULL)
		{
			if(arg_index == num_args_allocated)
			{
				//allocate more space by 5 if ran out of space in char * array
				num_args_allocated += 5;
				command_args = (char **) realloc(command_args, num_args_allocated * sizeof(char *));
			}
			// add argument to the char ** in the next index and increase the index counter
			command_args[arg_index] = (char *) malloc(max_arg_length * sizeof(char));
			strcpy(command_args[arg_index], arg);
			arg_index++;

			// get next arg
			arg = strtok(NULL, " ");
		}


		/*
		 * Create a pipe for the int array of file descriptors
		 * These two file descriptor ints will be used to communicate between
		 * The child process and the parent process
		 */
		pipe(file_descriptors);

		// close the write file_descriptor
		close(file_descriptors[1]);

		/*
		 * Fork a child process that will have a pid > 0
		 * Parent will close redirect stdout and stdin to the
		 * write (index 1) file descriptor using the OS dup2 call
		 * Child will close both of the file descirptors so the OS can write
		 * and use the execvp command to call the function and stdout and stderr
		 * will be piped properly
		 */
		pid_t pid = fork();
		if (pid == -1){
			printf("Fork error.");
			exit(1);
		} else if (pid != 0){
			dup2(file_descriptors[1], STDOUT_FILENO);
			dup2(file_descriptors[1], STDERR_FILENO);
		} else {
			close(file_descriptors[0]);
			close(file_descriptors[1]);
			char * command = command_args[0];
			// execvp allows for the program to not have the full path since excvp will
			// search the PATH environment for an executable
			execvp(command, command_args);
			exit(0);
		}
		// ensure that the parent waits for all of its children (only one in this case).
		wait(NULL);

		// print result of the command reading from the read (index 0) file descriptor with
		// 4096 bytes at a time
		char buffer[4096];
		while (1) {
		  ssize_t output_length = read(file_descriptors[0], buffer, sizeof(buffer));
		  if (output_length == -1) {
			  printf("Error");
		  }
		  else if (output_length == 0)
		  {
		    break;
		  }
		  else
		  {
			// print the contents of the buffer
		    printf("%s", buffer);
		  }
		}
		close(file_descriptors[0]);

		// free memory
		for(int i = 0; i < num_args_allocated; i++)
		{
			free(command_args[i]);
			command_args[i] = NULL;
		}
		free(command_args);
		command_args = NULL;

		free(input);
		input = NULL;

		free(arg);
		arg = NULL;


	}
}

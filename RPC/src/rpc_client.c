/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "rpc.h"


void
count_numbers_1(char *host)
{
	CLIENT *clnt;
	int  *result_1;
	rpc_data  count_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, COUNT_NUMBERS, COUNT_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	/*
		 * Initialize Arguments
		 */
	size_t input_max = 100;
	char * input = NULL;
	char buffer[1024] = {0};

	while(1){

		input = (char *) malloc(input_max * sizeof(char));

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

		//count_1_arg.content = (char *) malloc(input_max * sizeof(char));
		memcpy(count_1_arg.content, input, strlen(input)+ 1);

		printf("Content set as %s with size %lu\n", count_1_arg.content,
				strlen(count_1_arg.content));

		//if C00L is a substirng of the user input, copy it to memory
		if( strstr(input, "C00L") != NULL ){

			printf("Sent: [%s] to server\n", count_1_arg.content);

			result_1 = count_1(&count_1_arg, clnt);
			if (result_1 == (int *) NULL) {
				clnt_perror (clnt, "call failed");
			}
			printf("Received [%d] digits from server\n",*result_1);

			// detach from the shared memory

			// tell the processor we are done and wait for the response from the processor
		}else{
			printf("Not C00L");
		}

		free(input);
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	count_numbers_1 (host);
exit (0);
}
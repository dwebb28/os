/*
 ============================================================================
 Name        : client.c
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
#include <sys/socket.h>
#include <netinet/in.h>

int main(void) {

	/*
	 * Initialize Arguments
	 */
	size_t input_max = 100;
	char * input = NULL;
	struct sockaddr_in socket_address;
	int created_socket = 0, response;
	struct sockaddr_in server_address;
	char buffer[1024] = {0};
	int port = 10001;
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

		//if C00L is a substirng of the user input, copy it to memory
		if( strstr(input, "C00L") != NULL ){
			//memcpy(shared_memory_addr, input, strlen(input) + 1);

		    if ((created_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		    {
		        printf("\n Failed to create socket \n");
		        return -1;
		    }

		    memset(&server_address, '0', sizeof(server_address));

		    server_address.sin_family = AF_INET;
		    server_address.sin_port = htons(port);

		    if(inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr)<=0)
		    {
		        printf("\nInvalid address/ Address not supported \n");
		        return -1;
		    }

		    if (connect(created_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
		    {
		        printf("\nConnection Failed \n");
		        return -1;
		    }
		    send(created_socket , input , strlen(input) , 0 );
		    printf("[%s] sent\n", input);
		    response = read( created_socket , buffer, 4096);
		    printf("Received %s\n",buffer );

			// detach from the shared memory

			// tell the processor we are done and wait for the response from the processor
		}else{
			printf("Not C00L");
		}

		free(input);


	}
}

/*
 ============================================================================
 Name        : server.c
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
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>

int main(void) {

	/*
	 * Initialize Arguments
	 */
	FILE * secrets_file;
	int listen_port = 10001;
    struct sockaddr_in socket_address;
    int opt = 1;
    int addrlen = sizeof(socket_address);
	int num_count = 0;
    char buffer[1024] = {0};
	int socket_file_desc, created_socket, read_file_desc;
    char *thanks = "Thank you";

	while(1){

		// wait for the receiver to notify us
	    if ((socket_file_desc = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	    {
	        perror("socket creation has failed");
	        exit(EXIT_FAILURE);
	    }

	    if (setsockopt(socket_file_desc, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
	                                                  &opt, sizeof(opt)))
	    {
	        perror("setsockopt");
	        exit(EXIT_FAILURE);
	    }

	    socket_address.sin_family = AF_INET;
	    socket_address.sin_addr.s_addr = INADDR_ANY;
	    socket_address.sin_port = htons( listen_port );

	    // Forcefully attaching socket to the port 8080
	    if (bind(socket_file_desc, (struct sockaddr *)&socket_address,
	                                 sizeof(socket_address))<0)
	    {
	        perror("bind failed");
	        exit(EXIT_FAILURE);
	    }
	    if (listen(socket_file_desc, 3) < 0)
	    {
	        perror("listen");
	        exit(EXIT_FAILURE);
	    }
	    if ((created_socket = accept(socket_file_desc, (struct sockaddr *)&socket_address,
	                       	   (socklen_t*)&addrlen))<0)
	    {
	        perror("accept");
	        exit(EXIT_FAILURE);
	    }

	    read_file_desc = read( created_socket , buffer, 4096);
		printf("%s\n",buffer );
		send(created_socket , thanks , strlen(thanks) , 0 );
		printf("Responded to client\n");
		close(socket_file_desc);

		num_count = 0;
		for( int i = 0; buffer[i] != '\0'; i++){
			// if C00L starts at the current index, skip ahead of it so the 0's
			// in C00L are not counted
			if( strstr(&buffer[i], "C00L") == &buffer[i] ){
				i = i + 3;
				continue;
			}
			// if the character is a digit, add it to the count
			if (isdigit(buffer[i])){
				num_count++;
			}
		}



		printf("Number of digits: %d\n", num_count);

		// write the number of digits and the full string to file (append only)
//		if((secrets_file = fopen("secrets.out", "a")) == NULL){
//			printf("Could not open secrets.out file!\n");
//			exit(1);
//		}
//
//		//fprintf(secrets_file, "%d - %s\n", num_count, shared_memory_addr);
//		fclose(secrets_file);



	}
}

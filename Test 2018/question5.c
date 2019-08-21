#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h> 
#include <errno.h>
#include <unistd.h>

#define MAXDATASIZE 1024  


int main(int argc, char *argv[])  {

	if (argc != 2) {  
		fprintf(stderr,"usage: threeServer port-number\n"); 
		exit(1); 
	} 

	// place your code between the lines of //
	///////////////////////////////////////////////////////
	
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	///////////////////////////////////////////////////////

	struct sockaddr_in sa, caller;
	sa.sin_family = AF_INET;          
	sa.sin_addr.s_addr = INADDR_ANY; 
	sa.sin_port = htons(atoi(argv[1])); 

	// place your code between the lines of //
	///////////////////////////////////////////////////////
	
	int rc = bind(sock, (struct sockaddr *)&sa, sizeof(sa));
	if (rc == -1) {
		perror("bind");
		exit(1);
	} 

	//////////////////////////////////////////////////////

	// place your code between the lines of //
	///////////////////////////////////////////////////////
	rc = listen(sock, 5);
	if (rc == -1) {
		perror("listen");
		exit(1);
	}

	///////////////////////////////////////////////////////

	socklen_t length = sizeof(caller);
	// place your code between the lines of //
	/////////////////////////////////////////////////////////////
	int sockout = accept(sock, (struct sockaddr *)&caller, &length);

	/////////////////////////////////////////////////////////////

	char message[MAXDATASIZE] = "congrats you successfully connected to the server!";
	while (strlen(message) > 0)
	{
		int numbytes; // number of bytes of data read from socket

		// send data to the client and then get data back from the client:
		// place your code between the lines of //
		///////////////////////////////////////////////////////
		write(sockout, message, strlen(message));
		numbytes = read(sockout, message, MAXDATASIZE-1);
		///////////////////////////////////////////////////////

		message[numbytes - 1] = '\0';
		
	}

	// place your code between the lines of //
	///////////////////////////////////////////////////////
	close(sockout);
	close(sock);
	///////////////////////////////////////////////////////

	exit (0);
}


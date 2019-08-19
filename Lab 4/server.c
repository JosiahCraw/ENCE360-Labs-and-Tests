#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h> 
#include <errno.h>
#include <unistd.h>

#define MAXDATASIZE 1024 // max buffer size 
#define SERVER_PORT 2000

int listen_on(int port)
{

    int s = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in sa;
    sa.sin_family = AF_INET;          /* communicate using internet address */
    sa.sin_addr.s_addr = INADDR_ANY; /* accept all calls                   */
    sa.sin_port = htons(port); /* this is port number                */

    int rc = bind(s, (struct sockaddr *)&sa, sizeof(sa)); /* bind address to socket   */
    if (rc == -1) { // Check for errors
        perror("bind");
        exit(1);
    }

    rc = listen(s, 5); /* listen for connections on a socket */
    if (rc == -1) { // Check for errors
        perror("listen");
        exit(1);
    }

    return s;
}


int accept_connection(int s) {
    
    int sock = accept(s, NULL, NULL);
    
    return sock;
}


void handle_request(int msgsock) {

      // This initial code reads a single message (and ignores it!)
    char buffer[MAXDATASIZE] = "\0";
    int num_read = 0;
    
    while (1) {
    //read a message from the client

    if ((num_read = read(msgsock, buffer, MAXDATASIZE - 1)) < 0) {
        break;
    }
    printf("read a message %d bytes: %s\n", num_read, buffer);
    
    send(msgsock, buffer, strlen(buffer), 0);
    }
    printf("Connection Closed!\n");
    exit(0);

}


// handle request by forking a new process
void handle_fork(int msgsock) {

    //TODO: run this line inside a forked child process
    int child_pid;

    child_pid = fork();

    if (child_pid == 0) {
        handle_request(msgsock);
    }

    return;
}





int main(int argc, char *argv[]) {


    printf("\nThis is the server with pid %d listening on port %d\n", getpid(), SERVER_PORT);

    // setup the server to bind and listen on a port
    int s = listen_on(SERVER_PORT);

    while (1) { // forever

        int msgsock = accept_connection(s); // wait for a client to connect
        printf("Got connection from client!");

        // handle the request with a new process
        handle_fork(msgsock);
    }

    close(s);
    exit(0);
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>

#define MAXDATASIZE 128

// Set the Port number (to be used later in a TCP connection) by doing a UDP request
// First the client requests the port number by sending a "ping" message
// Second transmit the port number to the client
int setPortNumber(int port)
{
    int socketfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[MAXDATASIZE];
    int n, len;


    // Place your code between the lines of //
    ///////////////////////////////////////////////////////
    socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    ///////////////////////////////////////////////////////

    memset(&servaddr, 0, sizeof(struct sockaddr_in));
    memset(&cliaddr, 0, sizeof(struct sockaddr_in));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);
    len = sizeof(struct sockaddr_in);

    // Bind the socket with the server address
    if ( bind(socketfd, (const struct sockaddr *)&servaddr, sizeof(struct sockaddr_in)) < 0 )  {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    n = recvfrom(socketfd, (char *)buffer, MAXDATASIZE, MSG_WAITALL, (struct sockaddr *) &cliaddr, &len);
    sprintf(buffer, "%d", port+1);
    sendto(socketfd, buffer, strlen(buffer), MSG_CONFIRM,
               (const struct sockaddr *) &cliaddr, len);

    return port+1;
}


int main(int argc, char *argv[])  {

    int tcp_port;
    int n;
    int s, sockfd;
    socklen_t length;
    char message[MAXDATASIZE] = "congrats you successfully connected to the server!!";

    if (argc != 2) {
        fprintf(stderr,"usage: threeServer port-number\n");
        exit(1);
    }

    tcp_port = setPortNumber(atoi(argv[1]));

    // Place your code between the lines of //
    ///////////////////////////////////////////////////////
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ///////////////////////////////////////////////////////

    struct sockaddr_in sa, caller;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_port = htons(tcp_port);

    // Place your code between the lines of //
    ///////////////////////////////////////////////////////
    bind(sockfd, (struct sockaddr *)&sa, sizeof(sa));
    //////////////////////////////////////////////////////

    // Place your code between the lines of //
    ///////////////////////////////////////////////////////
    listen(sockfd, 5);
    ///////////////////////////////////////////////////////

    length = sizeof(caller);
    // Place your code between the lines of //
    /////////////////////////////////////////////////////////////
    s = accept(sockfd, (struct sockaddr *)&caller, &length);
    /////////////////////////////////////////////////////////////


    while (strlen(message) > 0)
    {
        int numbytes; // number of bytes of data read from socket

        // send data to the client and then get data back from the client:
        // place your code between the lines of //
        ///////////////////////////////////////////////////////
        write(s, message, strlen(message));
    		numbytes = read(s, message, MAXDATASIZE-1);
        ///////////////////////////////////////////////////////
        if (numbytes == 0) break;
        message[numbytes - 1] = '\0';
    }

    // Place your code between the lines of //
    ///////////////////////////////////////////////////////
    close(sockfd);
    close(s);
    ///////////////////////////////////////////////////////

    exit (0);
}

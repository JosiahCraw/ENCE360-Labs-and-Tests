#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAXDATASIZE 128


// Get the Port number (to be used later in a TCP connection) by doing a UDP request
// First the client requests the port number by sending a "ping" message
// Second Read the  port number sent by the server
int getPortNumber(char* hostname, char* udp_port)
{
    int socketfd;
    struct sockaddr_in sa;
    const char* message = "ping";
    char buffer[MAXDATASIZE];
    int len;
    int n;

    // Place your code between the lines of //
    ///////////////////////////////////////////////////////
    socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    ///////////////////////////////////////////////////////

    memset(&sa, 0, sizeof(struct sockaddr_in));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_port = htons(atoi(udp_port));
    len = sizeof(struct sockaddr_in);

    // Place your code between the lines of //
    ///////////////////////////////////////////////////////
    sendto(socketfd, (char *)message, strlen(message), MSG_CONFIRM, (const struct sockaddr *) &sa,
            sizeof(sa));

    n = recvfrom(socketfd, (char *)buffer, MAXDATASIZE,
                MSG_WAITALL, (struct sockaddr *)&sa,
                &len);
    ///////////////////////////////////////////////////////
    buffer[n] = '\0';

    return atoi(buffer);
}


int main(int argc, char *argv[])
{
    int sockfd;
    int tcp_port;
    struct addrinfo their_addrinfo; // server address info
    struct addrinfo *their_addr = NULL; // connector's address information
    int n, numbytes;
    char buffer[MAXDATASIZE]; //buffer contains data from/to server

    if (argc != 3) {
        fprintf(stderr,"usage: threeClient hostname\n");
        exit(1);
    }

    tcp_port = getPortNumber(argv[1], argv[2]);
    // place your code between the lines of //
    ///////////////////////////////////////////////////////
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ///////////////////////////////////////////////////////


    memset(&their_addrinfo, 0, sizeof(struct addrinfo));
    their_addrinfo.ai_family = AF_INET;
    their_addrinfo.ai_socktype = SOCK_STREAM;
    sprintf(buffer, "%d", tcp_port);
    n= getaddrinfo(argv[1], buffer, &their_addrinfo, &their_addr);

    // place your code between the lines of //
    ///////////////////////////////////////////////////////
    connect(sockfd, their_addr->ai_addr, their_addr->ai_addrlen);
    numbytes = read(sockfd, buffer, MAXDATASIZE-1);
    ///////////////////////////////////////////////////////


    do
    {

        // send data to the server and then get data back from the server:
        // place your code between the lines of //
        ///////////////////////////////////////////////////////
        write(sockfd, buffer, strlen(buffer));
        numbytes = read(sockfd, buffer, MAXDATASIZE-1);
        ///////////////////////////////////////////////////////
        printf("%s\n", buffer); //print out what you received
        if (numbytes == 0) break;
        buffer[numbytes-1] = '\0';
        // send data to the server and then get data back from the server:
        // place your code between the lines of //
        ///////////////////////////////////////////////////////
        write(sockfd, buffer, strlen(buffer));
        numbytes = read(sockfd, buffer, MAXDATASIZE-1);
        ///////////////////////////////////////////////////////
    }
    while (strlen(buffer) > 0);

    // place your code between the lines of //
    ///////////////////////////////////////////////////////
    close(sockfd);
    ///////////////////////////////////////////////////////

    return 0;
}

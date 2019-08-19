#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <netdb.h> 
#include <unistd.h>
#include <sys/socket.h>
#include <readline/readline.h>
#include <readline/history.h>


#define MAXDATASIZE 1024 // max buffer size 
#define SERVER_PORT 2000

int client_socket(char *hostname)
{
    char port[20];
    struct addrinfo their_addrinfo; // server address info
    struct addrinfo *their_addr = NULL; // connector's address information  
    int sockfd;

    int n = snprintf(port, 20, "%d", SERVER_PORT); // Make a string out of the port number
    if ((n < 0) || (n >= 20))
    {
        printf("ERROR: Malformed Port\n");
        exit(EXIT_FAILURE);
    }
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { perror("Socket Failure"); exit(1); }
    memset(&their_addrinfo, 0, sizeof(struct addrinfo));

    their_addrinfo.ai_family = AF_INET;
    their_addrinfo.ai_socktype = SOCK_STREAM;
    their_addrinfo.ai_protocol = PF_UNSPEC;

    getaddrinfo(hostname, port, &their_addrinfo, &their_addr);

    connect(sockfd, their_addr->ai_addr, their_addr->ai_addrlen);

    return sockfd;
}


int main(int argc, char *argv[])
{
    char buffer[MAXDATASIZE];

    if (argc != 2) {
        fprintf(stderr, "usage: client hostname\n");
        exit(1);
    }

    int sockfd = client_socket(argv[1]);

    int numbytes = 0;
    char *line;

    do {

        line = readline(">> ");
        write(sockfd, line, strlen(line));

        numbytes = read(sockfd, buffer, MAXDATASIZE - 1);
        buffer[numbytes] = '\0';

        printf("server: %s\n", buffer);

    } while (numbytes > 0);

    free(line);
    close(sockfd);

    return 0;
}

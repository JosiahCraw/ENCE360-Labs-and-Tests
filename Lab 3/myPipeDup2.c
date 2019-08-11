/* Title: pipedup2.c
 * Description: ENCE360 Example code - dup2.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define INP 1
#define OUTP 0

int main(void) {
    int fd[2];
    int fd1[2];

    pid_t childpid, childpid1;

    if (pipe(fd) == -1) { perror("Pipe 0 failed"); }
    if (pipe(fd1) == -1) { perror("Pipe 1 failed"); }

    if ((childpid = fork()) == 0) { /* Child code: Runs ls */
        dup2(fd[INP], STDOUT_FILENO);
        close(fd[OUTP]);
        close(fd[INP]);
        execl("/bin/ls", "ls", "-l", NULL);
        perror("The exec of ls failed");
    } else { /*  Child code: Runs sort */
        if ((childpid1 = fork()) == 0) {
            dup2(fd[OUTP], STDIN_FILENO);

            close(fd[OUTP]);
            close(fd[INP]);

            dup2(fd1[INP], STDOUT_FILENO);

            close(fd1[INP]);
            close(fd1[OUTP]);

            execl("/usr/bin/sort", "sort", "-k", "+8", NULL);

            perror("The exec of sort failed");
        } else { // Parent Code: runs head -5
            dup2(fd1[OUTP], STDIN_FILENO);
            close(fd1[OUTP]);
            close(fd1[INP]);
            execl("/usr/bin/head", "head", "-5", NULL);
            /* Note: The location of sort depends on your distribution.
            * Use 'which sort' to find the correct location */
            perror("The exec of head failed");
        }
    }

    exit(0);
}

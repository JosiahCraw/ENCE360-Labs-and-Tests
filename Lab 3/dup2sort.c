/* Title: dup2eg.c
 * Description: ENCE360 Example code - dup2.
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void) {

    int fd;

    /* Open my.file */
    if ((fd = open("my.file", O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR)) == -1) {
        perror("Couldn't open my.file");
        exit(1);
    }

    if (dup2(fd, 0) == -1) {
        perror("Could not redirect stdin");
        exit(3);
    }

    close(fd);

    /* Replace the current process with "ls -l" */
    execlp("sort", "sort", "-k7", NULL);

    /* If we reach this line, execl must have failed */
    perror("Exec failed");
    exit(3);
}

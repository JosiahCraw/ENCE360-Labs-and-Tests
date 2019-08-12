#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void install_handler(void);
void sigRecieved();

int callCount;

int main(int argc, char * argv[]) {
        install_handler();
        while (1) {;};
}

void install_handler(void) {
    signal(SIGQUIT, sigRecieved);
}

void sigRecieved (void) {
    signal(SIGQUIT, sigRecieved);
    write(1, "SIGQUIT\n", 8);
    if (callCount == 1) { exit(0); }
    callCount++;
}

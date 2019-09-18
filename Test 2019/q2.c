#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <wait.h>


// Place your code for interrupt handlers here
// between the lines of //
///////////////////////////////////////////////
void sig() {
  exit(7);
}
///////////////////////////////////////////////


void translator(int input_pipe[], int output_pipe[])
{
    int c;
    char ch;
    int rc;

    // First, close unnecessary file descriptors
    // Place your code between the lines of //
    ///////////////////////////////////////////////
    close(input_pipe[1]);
    close(output_pipe[0]);
    ///////////////////////////////////////////////

    // Register your interest in any interrupt handlers here
    // between the lines of //
    ///////////////////////////////////////////////
    signal(SIGPIPE, sig);
    ///////////////////////////////////////////////

    /* enter a loop of reading from the user_handler's pipe, translating */
    /* the character, and writing back to the user handler.              */
    while ((rc = read(input_pipe[0], &ch, 1)) > 0) {
        c = ch;
        if (__isascii(c) && isupper(c))
            c = tolower(c);
        ch = c;

        if (ch == '5') {
            rc= 5;
            break;
        }
        else {
            rc = write(output_pipe[1], &ch, 1);

            if (rc < 0) {
                perror("translator: write");
                break;
            }
        }
    }

    close(input_pipe[0]);
    close(output_pipe[1]);
    exit(rc);
}

void user_handler(int input_pipe[], int output_pipe[])
{
    int c;    /* user input - must be 'int', to recognize EOF (= -1). */
    char ch;  /* the same - as a char. */
    int rc;   /* return values of functions. */

    // First, close unnecessary file descriptors
    // Place your code between the lines of //
    ///////////////////////////////////////////////
    close(input_pipe[1]);
  	close(output_pipe[0]);
    ///////////////////////////////////////////////

    printf("Enter text to translate:\n");
    /* loop: read input from user, send via one pipe to the translator, */
    /* read via other pipe what the translator returned, and write to   */
    /* stdout. exit on EOF from user.                                   */
    while ((c = getchar()) > 0) {
        ch = (char)c;

        // Write to translator
        // Place your code between the lines of //
        ///////////////////////////////////////////////
        rc = write(output_pipe[1], &ch, 1);
        ///////////////////////////////////////////////

        if (ch == '7') {
            break;
        }

        // Read back from translator
        // Place your code between the lines of //
        ///////////////////////////////////////////////
        rc = read(input_pipe[0], &ch, 1);
        ///////////////////////////////////////////////

        if (rc < 0) {
            perror("user_handler: read\n");
            close(input_pipe[0]);
            close(output_pipe[1]);
            exit(1);
        }
        else if (rc == 0) {
            break;   //EOF
        }
        else {
            c = (int)ch;
            putchar(c);
        }
    }

    close(input_pipe[0]);
    close(output_pipe[1]);
}




int main(int argc, char* argv[])
{
    int parent_to_child[2];
    int child_to_parent[2];
    int pid;
    int rc;
    int status;


    // Create a pipe connect from parent to child.
    // You must use the error handler in the
    // accompanying if statement.
    // Place your code between the lines of //
    //////////////////////////////////////////////
    rc = pipe(parent_to_child);
    //////////////////////////////////////////////
    if (rc == -1) {
        perror("main: pipe user_to_translator");
        exit(1);
    }


    // Create a pipe connect from child to parent.
    // You must use the error handler in the
    // accompanying if statement.
    // Place your code between the lines of //
    //////////////////////////////////////////////
    rc = pipe(child_to_parent);
    //////////////////////////////////////////////
    if (rc == -1) {
        perror("main: pipe translator_to_user");
        exit(1);
    }

    // Fork a child process to run the translator.
    // Parent process to handle user IO and
    // communicate with child for translations.
    // Place your code between the lines of //
    //////////////////////////////////////////////
    pid = fork();
    //////////////////////////////////////////////

    switch (pid) {
    case -1:
        perror("main: fork");
        exit(1);
    case 0:
        translator(parent_to_child, child_to_parent); /* line 'A' */
        exit(0);
    default:
        user_handler(child_to_parent, parent_to_child); /* line 'B' */
    }

    // Wait on child and get its exit status - You will need waitpid()
    // Print exit status to stdout
    // Place your code between the lines of //
    ////////////////////////////
    wait(&status);
    ////////////////////////////

    printf("\nReturn status of Child: %d\n", WEXITSTATUS(status));
    return 0;
}

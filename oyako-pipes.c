// original code template provided by mtrigobo
// - Characters from command line arguments are sent to child process
//   from parent process one at a time through pipe.
// - Child process counts number of characters sent through pipe.
// - Child process returns number of characters counted to parent process.
// - Parent process prints number of characters counted by child process.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>         // for fork()
#include <sys/types.h>      // for pid_t
#include <sys/wait.h>       // for waitpid()

int main(int argc, char **argv)
{
    pid_t   pid;
    int     mypipe[2];
    int     status;

    // set up pipe
    if (pipe(mypipe)) {
        fprintf(stderr, "Pipe failed.\n");
        return EXIT_FAILURE;
    }

    // call fork()
    pid = fork();

    if (pid < 0) {
        printf("fork error %d\n", pid);
        return -1;
    } else if (pid == 0) {
        // -- running in child process --
        int     nChars = 0;

        // Receive characters from parent process via pipe
        // one at a time, and count them.
        close(mypipe[1]);

        FILE *stream;
        int c;
        stream = fdopen(mypipe[0], "r");
        while ((c = fgetc(stream)) != EOF) {
            nChars++;
        }
        fclose(stream);

        // Return number of characters counted to parent process.
        return nChars;
    } else {
        // -- running in parent process --
        int     nChars = 0;

        // Send characters from command line arguments starting with
        // argv[1] one at a time through pipe to child process.
        FILE *stream;
        stream = fdopen(mypipe[1], "w");
        if (argc > 1) {
            // note that we are starting from 1 for argv[1]
            for (int i = 1; i < argc; i++) {
                fprintf(stream, "%s", argv[i]);
            }
        }
        fclose(stream);

        // Wait for child process to return. Reap child process.
        // Receive number of characters counted via the value
        // returned when the child process is reaped.

        if (waitpid(pid, &status, 0) > 0) {
        
            // parent reaps child status
            if (WIFEXITED(status)) {
            
                // child exited normally
                nChars = WEXITSTATUS(status);
                
            }
        }

        if (nChars == 1)
            printf("child counted %d character\n", nChars);
        else
            printf("child counted %d characters\n", nChars);
        return 0;
    }
}

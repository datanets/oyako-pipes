oyako-pipes
===========

This application provides an example of how to use forking and pipes. When run, oyako-pipes forks into two processes (parent and child). The parent process takes the (command line) arguments and sends them to the child process (one non-space character at a time) through a pipe.

## How to Compile

    gcc -m32 -std=gnu99 -Wall -g oyako-pipes.c -o oyako-pipes

## Example Output

    $ ./oyako-pipes hello world
    child counted 10 characters

## Notes

- See LICENSE file for license information.
- Original code template provided by mtrigobo

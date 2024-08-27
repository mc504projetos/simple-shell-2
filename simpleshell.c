/*****************************************************************
Nome: Anderson Murillo                  RA: 248221
Nome: Isaac do Nascimento Oliveira      RA: 247175
******************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_LINE_LEN 1024
#define MAX_CMD_LEN 256
#define MAX_ARGS_LEN 256

void read_command(char *cmd, char *args[]) {
    char cmd_line[MAX_LINE_LEN];

    // get user command
    printf("simple_shell$: ");
    fgets(cmd_line, sizeof(cmd_line), stdin);

    // remove \n in the end of the user input
    size_t len = strlen(cmd_line);
    if (len > 0 && cmd_line[len - 1] == '\n')
        cmd_line[len - 1] = '\0';

    // split command line by " "
    char *token = strtok(cmd_line, " ");
    int argcount = 0;

    while (token != NULL && argcount < MAX_ARGS_LEN - 1) {
        args[argcount++] = token;
        token = strtok(NULL, " ");
    }
    args[argcount] = NULL; // terminate args list with NULL

    // copy command
    strcpy(cmd, args[0]);

}

int main(int argc, char *argv[]) {
    char cmd[MAX_CMD_LEN];
    char *args[MAX_ARGS_LEN];
    char full_cmd_path[MAX_CMD_LEN + MAX_LINE_LEN];

    while (1) {
        int status;
        pid_t pid;

        // read user command
        read_command(cmd, args);

        // build the complete command filepath
        strcpy(full_cmd_path, argv[1]);
        strcat(full_cmd_path, "/");
        strcat(full_cmd_path, cmd);

        // create child process to execute the command
        pid = fork();
        if (pid == 0)
            execv(full_cmd_path, args);
        else
            wait(&status); // wait for the child process to finish
    }

    return 0;
}

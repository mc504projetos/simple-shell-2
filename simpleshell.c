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

void read_command(char *cmd, char *args[], int *background) {
    char cmd_line[MAX_LINE_LEN];

    // get user command
    printf("simple_shell$: ");
    if (!fgets(cmd_line, sizeof(cmd_line), stdin))
        exit(1);  // Saia se ocorrer erro na leitura

    // remove \n in the end of the user input
    size_t len = strlen(cmd_line);
    if (len > 0 && cmd_line[len - 1] == '\n')
        cmd_line[--len] = '\0';

    // verify if it's background process
    *background = 0;
    if (len > 0 && cmd_line[len - 1] == '&') {
        *background = 1;
        cmd_line[--len] = '\0';  // removes '&' from cmd line
    }

    // if "ENTER", returns to main
    if (len == 0) {
        cmd[0] = '\0';
        return;
    }

    // split command line by " "
    int argcount = 0;
    char *token = strtok(cmd_line, " ");

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
    int background;

    while (1) {
	    // read user command
        read_command(cmd, args, &background);

        // if pressed "ENTER", awaits for the cmd
        if (cmd[0] == '\0')
            continue;

        // if "exit", closes shell
        if (strcmp(cmd, "exit") == 0)
            break;

	    // build the complete command filepath
        strcpy(full_cmd_path, argv[1]);
        strcat(full_cmd_path, "/");
        strcat(full_cmd_path, cmd);

	    // execute command
        pid_t pid = fork();
        if (pid == 0) {
            execv(full_cmd_path, args); // if the flows continues,
            // it means execv failed (returns -1)
            perror("Erro: comando não encontrado");
            exit(1);
        } else if (pid > 0) {
            if (!background)
                wait(NULL);  // waits for the non-background
                // child process to end
    }

    return 0;
}

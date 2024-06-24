#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define HISTORY_SIZE 10

static bool should_quit = false;        // Flag to check if the user wants to quit the shell
static bool display_history = false;    // Flag to check if the user wants to display the history

char* history[HISTORY_SIZE];
int history_count = 0;

// Function read_line reads a line from the standard input and returns it as a string.
char* read_line() {
    char *line = NULL;
    size_t buflen = 0;
    errno = 0;
    ssize_t strlen = getline(&line, &buflen, stdin);

    if (strlen < 0) {
        if (errno) {
            perror("bash");
        }
        exit(1);
    }
    return line;
}

// Function file_read_line reads a line from a file and returns it as a string.
char *file_read_line(const char *filename) {
    
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL) {
        printf("Error opening file: %s\n", filename);
        return NULL;
    }

    fseek(fptr, 0, SEEK_END);
    long file_size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    char *line = (char *)malloc(file_size + 1);
    if (line == NULL) {
        printf("Error allocating memory.\n");
        fclose(fptr);
        return NULL;
    }

    if (fgets(line, file_size + 1, fptr) == NULL) {
        printf("Error reading line from file: %s\n", strerror(errno));
        fclose(fptr);
        free(line);
        return NULL;
    }

    fclose(fptr);
    return line;
}

// Function split_line serves as a helper function to split a string into tokens based on a delimiter
// specified by the user. It returns an array of tokens.
char **split_line(char *line, char *delimiter) { 
    int bufferSize = 64; // Starting buffer size
    char **tokens = malloc(bufferSize * sizeof(char*));
    if (!tokens) {
        fprintf(stderr, "Error allocating memory.\n");
        exit(EXIT_FAILURE);
    }

    char *mutableString = strdup(line); // Duplicate string for tokenization
    if (!mutableString) {
        fprintf(stderr, "Error duplicating line.\n");
        exit(EXIT_FAILURE);
    }

    char *token = strtok(mutableString, delimiter); // Tokenize the string
    int i = 0;
    while (token != NULL) {
        // Manually allocate memory and copy the token
        tokens[i] = malloc(strlen(token) + 1); // +1 for the null terminator
        if (!tokens[i]) {
            fprintf(stderr, "Error allocating memory for token.\n");
            exit(EXIT_FAILURE);
        }
        strcpy(tokens[i], token);

        i++;
        if (i >= bufferSize) {
            bufferSize += 64; // Increase buffer size if necessary
            tokens = realloc(tokens, bufferSize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "Error reallocating memory.\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, delimiter);
    }
    tokens[i] = NULL; // Null-terminate the list of tokens

    free(mutableString); // Free the duplicated string
    return tokens;
}

// ** Helper functions for the built-in History command **
// Function add_to_history adds a command to the history array.
void add_to_history(const char* command) {
    free(history[history_count % HISTORY_SIZE]);             // Free the slot if it's about to be overwritten
    history[history_count % HISTORY_SIZE] = strdup(command); // Duplicate the command and store it in the history array
    history_count++;
}

// Function shell_history prints the history of commands entered by the user.
void shell_history() {
    int start = history_count > HISTORY_SIZE ? history_count - HISTORY_SIZE : 0; // Start from the beginning if the history is less than HISTORY_SIZE
    for (int i = start; i < history_count; i++) {
        printf("%d %s", i + 1, history[i % HISTORY_SIZE]);
    }
}

// Function shell_cd changes the current working directory.
void shell_cd(char *args) {
    char **tokens = split_line(args, " \n");
    int i = 0;

    if (tokens[1] == NULL) {
        fprintf(stderr, "cd: missing argument\n");
    } else if(i > 2) {
        fprintf(stderr, "cd: too many arguments\n");
    } else {
        if (chdir(tokens[1]) != 0) {
            perror("cd");
        }
    }
}

// Function shell_quit exits the shell.
void shell_quit() {
    exit(EXIT_SUCCESS);
}


// Function direct_execute executes commands without pipes. It takes a string of commands as input.
// It does not take input from a previous command.
void direct_execute(char *tokens) {
    char **cmd_tokens = split_line(tokens, ";");        // Split the command into tokens
    int i = 0;
    int n = 0;
    while (cmd_tokens[i] != NULL) {                     // Loop through the tokens, fork and execute each command
        if (strstr(cmd_tokens[i], "quit") != NULL) {    // Check for quit command
            should_quit = true;
            i++;                                        // Continue to the next command
            continue;
        }
        if(strstr(cmd_tokens[i], "history") != NULL){
            display_history = true;
            i++;
            continue;
        }

        pid_t child_pid = fork();   // Fork a child process
        char **args = split_line(cmd_tokens[i], " \t\r\n"); // Split the command into arguments
        if (child_pid == 0) {
            // Execute the command
            execvp(args[0], args);
            perror("bash");
            exit(0);  
        } else if (child_pid > 0) {
            n++;
        } else {
            perror("bash");
            break;
        }
        free(args); // Free args after use
        i++;
    }

    for(int i = 0; i < n; i++){     // Wait for all child processes to finish outside the loop 
        int status;                 // so that the processes run concurrently
        wait(&status);
    }

    if(display_history) shell_history();
    if(should_quit) shell_quit();

    free(cmd_tokens); // Free cmd_tokens after use
}


// Function pipe_execute executes commands with pipes. It takes a string of commands as input.
// It takes as input the output of a previous command (buffer).
void pipe_execute(char *command, char *buffer) {
    
    char **cmd_tokens = split_line(command, ";");
    int i = 0;
    int n = 0;                         // Counter for the number of child processes
    while (cmd_tokens[i] != NULL) {
        if (strstr(cmd_tokens[i], "quit") != NULL) { // Check for quit command
            should_quit = true;
            i++;                                   // Continue to the next command
            continue;
        }
        if(strstr(cmd_tokens[i], "history") != NULL){
            display_history = true;
            i++;
            continue;
        }
        
        // Create a pipe for each command to provide it the buffer as input
        int pipefd[2];
        if (pipe(pipefd) == -1) {
            perror("Pipe creation failed");
            exit(EXIT_FAILURE);
        }
    
        // Write the buffer to the write end of the pipe
        write(pipefd[1], buffer, strlen(buffer));
        // Close the write end of the pipe after writing
        close(pipefd[1]);

        char **args = split_line(cmd_tokens[i], " \t\r\n");
        pid_t child_pid = fork();
        if (child_pid == 0) {
            // Redirect the standard input to the read end of the pipe
            dup2(pipefd[0], STDIN_FILENO);
            // Close the read end of the pipe in the child process after dup2
            close(pipefd[0]);
            
            // Execute the command
            execvp(args[0], args);
            perror("pipe");
            exit(EXIT_FAILURE);

        } else if (child_pid > 0) {
            n++;
        } else {
            perror("pipe");
            break;
        }
        free(args); 
        i++;
    }
    
    for(int i = 0; i < n; i++){
        int status;
        wait(&status);
    }

    free(cmd_tokens); // Free cmd_tokens after use
}

// Function command_execute executes the commands entered by the user. It takes a string of commands as input.
void command_execute(char *command) {           
    if (strstr(command, "cd") != NULL) {
        shell_cd(command);
        return;
    }

    should_quit = false;
    display_history = false;
    

    if (!(strchr(command, '|') != NULL)) {              // If no pipes in the command then execute the commands directly
        direct_execute(command);
    } else {                                            // Execution with pipes
        char **commands = split_line(command, "|");
        
        char buffer[4096];                              // This buffer will be used to pass output between commands
        int i = 0;
        while (commands[i] != NULL) {
            if (strstr(commands[i], "quit") != NULL) { // Check for quit command
                should_quit = true;
                i++;                                   // Continue to the next command
                continue;
            }
            if(strstr(commands[i], "history") != NULL){
                display_history = true;
                i++;
                continue;
            }
            
            int pipefds[2];     // Create a pipe for each command
            pipe(pipefds);      
            
            int stdout_save = dup(STDOUT_FILENO);
            dup2(pipefds[1], STDOUT_FILENO);
            close(pipefds[1]);

            // Execute the current command segment
            // For the first command, direct_execute is fine as there's no previous output to use
            pid_t child_id = fork();    // Fork a child process
            if (child_id == 0) {
                if (i == 0) {
                    direct_execute(commands[i]);
                    
                } else {
                    // For subsequent commands, use the output of the previous commands (buffer) as input
                    pipe_execute(commands[i], buffer);
                }
                exit(0);
            } else if (child_id > 0) {
                int status;
                wait(&status);
            } else {
                perror("bash");
                break;
            }
            // Restore stdout and capture the output
            dup2(stdout_save, STDOUT_FILENO);
            close(stdout_save);

            ssize_t bytes_read = read(pipefds[0], buffer, sizeof(buffer) - 1);  // Read the output from the read end of the pipe
            if (bytes_read >= 0) buffer[bytes_read] = '\0';                     // Null-terminate the buffer
            close(pipefds[0]);

            i++;
        }
        printf("%s", buffer);
        
        if(display_history) shell_history();
        if(should_quit) shell_quit();

        free(commands); // Free commands after use
    }
}


int main(int argc, char **argv) {

    if (argc > 1) { // Check if a filename is provided as an argument
        
        FILE *file = fopen(argv[1], "r");
        
        if (file == NULL) {
            fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
            return 1;
        }

        char *input = NULL;
        size_t len = 0;
        ssize_t read;

        // Read each line from the file and execute commands
        while ((read = getline(&input, &len, file)) != -1) {
            if (input != NULL) {
                command_execute(input);
            }
        }

        free(input);
        fclose(file);
    } else { // If no arguments provided, run as normal shell
        
        while (true) {  // Loop to keep the shell running
            printf("$ ");
            char *input = read_line();
            char *line;
            add_to_history(input);

            if (strncmp(input, "bash", 4) == 0) {   // Check for bash command, if the user enters bash <filename>
                char **line_file = split_line(input, " \t\r\n");
                line = file_read_line(line_file[1]);
                command_execute(line);
            } else {    // If no bash command, execute the command as normal
                line = input;
                if (line != NULL) {
                    command_execute(line);
                }
            }
            
            free(line);
        }
    }
    return 0;
}
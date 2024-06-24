/*
 * Erblina Nivokazi
 * 150200917
 */


#include "blg312e.h"
#include <unistd.h>
#include <pthread.h>

// This structure is needed to hold client configuration
typedef struct {
    char* server_host;
    int server_port;
    char* request_file;
} client_config_t;

// This structure is needed to pass arguments to the thread function
typedef struct {
    char* host;
    int port;
    char* filename;
} thread_args_t;

/*
 * Send an HTTP request for the specified file 
 */
void clientSend(int fd, char *filename) {
    char buffer[MAXLINE];
    char host_name[MAXLINE];

    Gethostname(host_name, MAXLINE);

    /* Form and send the HTTP request */
    snprintf(buffer, sizeof(buffer), "GET %s HTTP/1.1\r\n", filename);
    snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), "Host: %s\r\n\r\n", host_name);
    Rio_writen(fd, buffer, strlen(buffer));
}

/*
 * Read the HTTP response and print it out
 */
void clientPrint(int fd) {
    rio_t rio;
    char buffer[MAXBUF];
    int length = 0;
    int n;

    Rio_readinitb(&rio, fd);

    /* Read and display the HTTP Header */
    n = Rio_readlineb(&rio, buffer, MAXBUF);
    while (strcmp(buffer, "\r\n") && (n > 0)) {
        printf("Header: %s", buffer);
        n = Rio_readlineb(&rio, buffer, MAXBUF);

        /* If you want to look for certain HTTP tags... */
        if (sscanf(buffer, "Content-Length: %d ", &length) == 1) {
            printf("Length = %d\n", length);
        }
    }

    /* Read and display the HTTP Body */
    n = Rio_readlineb(&rio, buffer, MAXBUF);
    while (n > 0) {
        printf("%s", buffer);
        n = Rio_readlineb(&rio, buffer, MAXBUF);
    }
}

/*Extra Function handleRequest*/
// Function to handle a single HTTP request
void* handleRequest(void* arg);

int main(int argc, char *argv[]) {
    // I changed the structure of the main function a little

    // Check for correct number of arguments, I added a threads option for the user to specify the number of threads
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <host> <port> <filename> <threads>\n", argv[0]);
        exit(1);
    }

    // Parse arguments
    client_config_t client_config;
    client_config.server_host = argv[1];
    client_config.server_port = atoi(argv[2]);
    client_config.request_file = argv[3];
    int num_threads = atoi(argv[4]);

    // Create threads
    pthread_t* threads = (pthread_t*) malloc(num_threads * sizeof(pthread_t));
    thread_args_t thread_args;
    thread_args.host = client_config.server_host;
    thread_args.port = client_config.server_port;
    thread_args.filename = client_config.request_file;

    // Create threads, each thread will send an HTTP request to the server
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, handleRequest, &thread_args);
    }

    // Wait for threads to finish
    for (int i = 0; i < num_threads; i++) {
        printf("Wait for thread number %d to finish\n", i);
        pthread_join(threads[i], NULL);
    }

    // Free memory allocation which was done by malloc 
    free(threads);
    return 0;
}

// Function to handle a single HTTP request
// This function is called by each thread to send an HTTP request to the server
// and print the response, then close the connection
// this function was needed because I needed to pass different arguments to each thread
void* handleRequest(void* arg) {
    thread_args_t* args = (thread_args_t*) arg;

    /* open connection to specified host and port */
    int client_fd = Open_clientfd(args->host, args->port);

    clientSend(client_fd, args->filename);
    clientPrint(client_fd);

    Close(client_fd);
    return NULL;
}
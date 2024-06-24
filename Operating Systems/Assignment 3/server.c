/*
 * Erblina Nivokazi
 * 150200917
 */

#include "blg312e.h"
#include "request.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <signal.h>

// Structure which is used for the configuration of the server
typedef struct {
    int port;
    int num_threads;
    int buffer_size;
} server_config_t;

// Structure of buffer to store connection file descriptors
typedef struct {
    int* fds;
    int count;
    int capacity;
    sem_t full;
    sem_t empty;
    sem_t mutex;
} conn_buffer_t;

// Global variables which help for server configuration and connection buffer
server_config_t config;
conn_buffer_t conn_buffer;

// Function for parsing command-line arguments
void getargs(int argc, char *argv[]);

// Function to initialize the connection buffer
void init_conn_buffer(conn_buffer_t* buffer, int capacity);

// Worker thread function to handle connections
void* worker_thread(void* arg);

// Signal handler for graceful shutdown
void handle_shutdown(int signum);

int main(int argc, char *argv[]) {
    // I changed the structure of the main function while doing the homework

    // Parse the command-line arguments
    getargs(argc, argv);
    // Initialize the connection buffer
    init_conn_buffer(&conn_buffer, config.buffer_size);

    // Create the worker threads
    pthread_t* thread_pool = (pthread_t*) malloc(config.num_threads * sizeof(pthread_t));
    for (int i = 0; i < config.num_threads; i++) {
        pthread_create(&thread_pool[i], NULL, worker_thread, NULL);
    }

    // This register the signal handler for a better shutdown
    // THis is considered to be good practice when implementing a server
    signal(SIGINT, handle_shutdown);

    // Create the listening socket which listens to incoming connections
    int listen_fd = Open_listenfd(config.port);
    // THis is the main loop to accept incoming connections
    while (1) {
        // Accept the incoming conection
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int conn_fd = Accept(listen_fd, (SA*)&client_addr, &client_len);

        // using sem_wait to wait for an empty slot in the buffer
        sem_wait(&conn_buffer.empty);
        sem_wait(&conn_buffer.mutex);

        // Also add the connection to the buffer
        conn_buffer.fds[conn_buffer.count] = conn_fd;
        conn_buffer.count++;

        // using sem_post to signal that the buffer is not empty anymore
        sem_post(&conn_buffer.mutex);
        sem_post(&conn_buffer.full);
    }

    // Free the memory allocated for the thread pool
    free(thread_pool);
    return 0;
}

void getargs(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <port> <threads> <buffers>\n", argv[0]);
        exit(1);
    }
    config.port = atoi(argv[1]);
    config.num_threads = atoi(argv[2]);
    config.buffer_size = atoi(argv[3]);
}

void init_conn_buffer(conn_buffer_t* buffer, int capacity) {
    // Allocate memory for the buffer
    buffer->fds = (int*) malloc(capacity * sizeof(int));
    buffer->count = 0;                          // Initialize the count to 0 bcs there is no connection yet
    buffer->capacity = capacity;                // Set the capacity of the buffer
    sem_init(&buffer->full, 0, 0);              // Initialize the full semaphore to 0
    sem_init(&buffer->empty, 0, capacity);      // Initialize the empty semaphore to the capacity of the buffer
    sem_init(&buffer->mutex, 0, 1);             // Initalize the mutex semaphore to 1
}

void* worker_thread(void* arg) {
    // This is the worker thread function which handles the connections
    // This function is executed by each worker thread
    while (1) {
        // Wait for a connection to be added to the buffer
        sem_wait(&conn_buffer.full);
        sem_wait(&conn_buffer.mutex);

        // Get the connection file descriptor from the buffer
        int conn_fd = conn_buffer.fds[0];

        // Shift the connections in the buffer to the left by 1
        for (int i = 0; i < conn_buffer.count - 1; i++) {
            conn_buffer.fds[i] = conn_buffer.fds[i + 1];
        }
        conn_buffer.count--;

        // Signal that the buffer is not full anymore
        sem_post(&conn_buffer.mutex);
        requestHandle(conn_fd);

        // Close the connection
        Close(conn_fd);
        sem_post(&conn_buffer.empty);
    }
    return NULL;
}

void handle_shutdown(int signum) {
    // This is the signal handler function which is called when the server is shut down
    printf("\nServer shutting down...\n");
    free(conn_buffer.fds);
    exit(EXIT_SUCCESS);
}
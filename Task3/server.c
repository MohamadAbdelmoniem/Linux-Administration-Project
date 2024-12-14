#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>      
#include <arpa/inet.h>   
#include <unistd.h>      
#include <signal.h>      
#include <stdlib.h>     
#include <sys/wait.h>   
#include <netinet/in.h>  
#define PORT 3600

// Signal handler for SIGINT
void sigint_handler(int sig) {
    printf("Received SIGINT signal. Server is terminating gracefully.\n");
    exit(0);
}

int main() {
    // Register the SIGINT handler
    signal(SIGINT, sigint_handler);

    // Create a socket with IPv4, TCP
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    int status = 0; // Status of the socket

    // Check if the socket was created
    if (socket_fd < 0) {
        printf("Error creating socket\n");
        return 1;
    }

    // Initialize the server address struct
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address)); // Zero out the structure
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the address
    status = bind(socket_fd, (struct sockaddr*)&server_address, sizeof(server_address));
    // Check if the socket was bound to the address
    if (status < 0) {
        printf("Error binding socket to address\n");
        return 1;
    }

    // Listen for incoming connections
    status = listen(socket_fd, 5);
    // Check if the socket is listening
    if (status < 0) {
        printf("Error listening on socket\n");
        return 1;
    }
    printf("Server is listening on port %d...\n", PORT);

    // Infinite loop to accept new connections
    while (1) {
        // Accept a connection
        struct sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        int client_socket_fd = accept(socket_fd, (struct sockaddr*)&client_address, &client_address_len);
        // Check if the connection was accepted
        if (client_socket_fd < 0) {
            printf("Error accepting connection\n");
            continue; // Continue accepting new connections
        }
        printf("Connection established with client: %s:%d\n",
               inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

        // Create a child process to handle the client
        pid_t pid = fork();
        if (pid < 0) {
            printf("Error in fork\n");
            close(client_socket_fd);
            continue;
        } else if (pid == 0) {
            // Child process
            close(socket_fd); // Close the listening socket in child
            // Infinite loop to handle client commands
            while (1) {
                // Read from the client
                char buffer[1024];
                memset(buffer, 0, sizeof(buffer)); // Reset buffer
                ssize_t bytes_read = read(client_socket_fd, buffer, sizeof(buffer) - 1);
                if (bytes_read <= 0) {
                    printf("Client disconnected or error occurred\n");
                    break;
                }
                buffer[bytes_read] = '\0'; // Null-terminate the buffer

                // Check if the command is "stop"
                if (strncmp(buffer, "stop", 4) == 0) {
                    printf("Client requested to stop. Closing connection.\n");
                    break;
                }

                // Execute the command using system()
                int exit_status = system(buffer);

                // Send the exit status back to the client
                int net_exit_status = htonl(exit_status);
                ssize_t bytes_written = write(client_socket_fd, &net_exit_status, sizeof(net_exit_status));
                if (bytes_written != sizeof(net_exit_status)) {
                    printf("Error sending exit status to client\n");
                    break;
                }
            }
            close(client_socket_fd); // Close client socket
            exit(0); // Terminate child process
        } else {
            // Parent process
            close(client_socket_fd); // Close client socket in parent
            // Wait for child processes to prevent zombies
            int status;
            pid_t wpid;
            while ((wpid = waitpid(-1, &status, WNOHANG)) > 0);
        }
    }

    // Close the listening socket
    close(socket_fd);
    return 0;
}
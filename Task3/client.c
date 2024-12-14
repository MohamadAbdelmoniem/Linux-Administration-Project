#include "stdio.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>   // For inet_addr
#include <stdlib.h>      // For atoi
#include <string.h>      // For strlen
#include <unistd.h>      // For close, read, write

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("You must pass the IP Address and PORT\n");
        exit(1);
    }

    char *server_ip = argv[1];
    int port = atoi(argv[2]);

    // Create a socket with IPv4, TCP
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    int status = 0; // Status of the socket

    // Check if the socket was created
    if (socket_fd < 0) {
        printf("Error creating socket\n");
        exit(1);
    }

    // Initialize the server address struct
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address)); // Zero out the structure
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);  // Convert port to network byte order
    server_address.sin_addr.s_addr = inet_addr(server_ip);  // Convert IP address to network byte order

    // Connect to the server
    status = connect(socket_fd, (struct sockaddr*)&server_address, sizeof(server_address));
    // Check if the connection was successful
    if (status < 0) {
        printf("Error connecting to server\n");
        exit(1);
    }

    // Infinite loop to send commands to the server
    while (1) {
        // Ask the user to input a command
        char command[1024];
        printf("Enter a command to execute on the server (type 'stop' to quit): ");
        scanf(" %[^\n]", command); // Read a command including spaces until a newline

        // Send the command to the server
        ssize_t bytes_written = write(socket_fd, command, strlen(command));
        if (bytes_written <= 0) {
            printf("Error sending command to server\n");
            break;
        }

        // Check if the command is "stop"
        if (strncmp(command, "stop", 4) == 0) {
            printf("Stopping client as per user request.\n");
            break;
        }

        // Receive the exit status from the server
        int net_exit_status;
        ssize_t bytes_read = read(socket_fd, &net_exit_status, sizeof(net_exit_status));
        if (bytes_read <= 0) {
            printf("Error receiving exit status from server\n");
            break;
        }
        int exit_status = ntohl(net_exit_status);

        // Print the exit status
        printf("Command exit status: %d\n", exit_status);
    }

    // Close the socket
    close(socket_fd);

    return 0;
}

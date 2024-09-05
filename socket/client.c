// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    struct sockaddr_in serv_addr;
    char message[BUFFER_SIZE];
    char buffer[BUFFER_SIZE] = {0};
    int sock = 0;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Get user input for the message to send
    printf("Enter message to send to server: ");
    if (fgets(message, BUFFER_SIZE, stdin) == NULL) {
        perror("Input error");
        close(sock);
        exit(EXIT_FAILURE);
    }
    // Remove newline character if present
    message[strcspn(message, "\n")] = '\0';

    // Send message to the server
    send(sock, message, strlen(message), 0);
    printf("Message sent to server\n");

    // Read response from the server
    ssize_t bytes_received = read(sock, buffer, BUFFER_SIZE - 1);
    if (bytes_received < 0) {
        perror("Read failed");
        close(sock);
        exit(EXIT_FAILURE);
    }
    buffer[bytes_received] = '\0'; // Null-terminate the received message
    printf("Received response from server: %s\n", buffer);

    // Close socket
    close(sock);

    return 0;
}
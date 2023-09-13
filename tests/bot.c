#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define SERVER_IP "192.168.50.11"
#define SERVER_PORT 23

#define TIMEOUT_SECONDS 180

void printMessage(const unsigned char* buf, size_t length) {
    printf("Received message: ");
    for (size_t i = 0; i < length; i++) {
        printf("%02X ", buf[i]);
    }
    printf("\n");
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(server_addr.sin_addr)) <= 0) {
        perror("Invalid server address");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Maintain the connection
    unsigned char buf[10];
    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_SECONDS;
    timeout.tv_usec = 0;

    // Send the bot message
    unsigned char botMsg[4] = { 0x00, 0x00, 0x00, 0x01 }; // Modify as needed
    if (send(sockfd, botMsg, sizeof(botMsg), 0) == -1) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Set the timeout for the socket
        if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) < 0) {
            perror("Timeout setting failed");
            exit(EXIT_FAILURE);
        }

        // Write data to the server
        const char* message = "0miraiBot";
        printf("Sending message\n");
        if (send(sockfd, message, strlen(message), 0) == -1) {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }

        printf("Message sent\n");

        // Read data from the server
        ssize_t bytesRead = recv(sockfd, buf, sizeof(buf), 0);
        if (bytesRead <= 0) {
            perror("Connection closed or error occurred");
            break;
        }
        printMessage(buf, bytesRead);

        // Sleep for a while before sending the next message (for demonstration purposes)
        sleep(1);
    }

    // Close the socket
    close(sockfd);

    return 0;
}
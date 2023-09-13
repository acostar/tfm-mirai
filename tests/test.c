#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

#define BUFFER_SIZE 256

int main() {
    char inputBuffer[BUFFER_SIZE];
    char outputBuffer[BUFFER_SIZE];
    fd_set readSet;
    int maxfd, activity;

    printf("Chat Program - Type 'exit' to quit\n");

    while (1) {
        // Clear the file descriptor set
        FD_ZERO(&readSet);

        // Add stdin (file descriptor 0) to the set
        FD_SET(0, &readSet);

        // Set the max file descriptor value
        maxfd = 0;

        // Wait for activity on the file descriptors
        activity = select(maxfd + 1, &readSet, NULL, NULL, NULL);

        if (activity == -1) {
            perror("select");
            exit(EXIT_FAILURE);
        }

        // Check if stdin (user input) is ready for reading
        printf("readSet value 1: %d \n", readSet);
        if (FD_ISSET(0, &readSet)) {
            printf("readSet value 2: %d \n", readSet);
            // Read user input from stdin
            fgets(inputBuffer, BUFFER_SIZE, stdin);

            // Remove newline character from the input
            inputBuffer[strcspn(inputBuffer, "\n")] = '\0';

            // Check if the user wants to exit
            if (strcmp(inputBuffer, "exit") == 0) {
                printf("Exiting the chat program...\n");
                break;
            }

            // Process the user input
            printf("You: %s\n", inputBuffer);
        }
        printf("readSet value 3: %d \n", readSet);
        // Check if stdout (console output) is ready for writing
        if (FD_ISSET(1, &readSet)) {
            printf("readSet value 4: %d \n", readSet);
            // Prepare the output message
            snprintf(outputBuffer, BUFFER_SIZE, "Received: %s", inputBuffer);

            // Write the output message to stdout
            printf("Other User: %s\n", outputBuffer);
        }
    }

    return 0;
}
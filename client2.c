#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 5000
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {

    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        printf("Invalid address\n");
        return -1;
    }

    printf("Client2 sending logs...\n");

    while(1) {

        printf("Enter log: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        buffer[strcspn(buffer,"\n")] = 0;

        if(strcmp(buffer,"exit")==0)
            break;

        char message[BUFFER_SIZE + 20];   // larger buffer

        snprintf(message, sizeof(message), "Client2: %s", buffer);

        sendto(sockfd, message, strlen(message), 0,
               (struct sockaddr*)&server_addr, sizeof(server_addr));
    }

    close(sockfd);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 5000
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {

    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if(sockfd < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP Client started\n");

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    while(1){

        printf("Enter log message: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        buffer[strcspn(buffer, "\n")] = '\0';

        if(strcmp(buffer,"exit")==0)
            break;

        sendto(sockfd, buffer, strlen(buffer), 0,
              (struct sockaddr *)&server_addr, addr_len);

        printf("Log sent to server\n");
    }

    close(sockfd);

    return 0;
}
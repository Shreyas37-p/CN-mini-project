#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define PORT 5000
#define BUFFER_SIZE 1024

int main() {

    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(client_addr);

    long total_bytes = 0;
    int log_count = 0;

    time_t start_time = time(NULL);

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Distributed Log Aggregation Server Started\n");

    while (1) {

        int bytes = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                             (struct sockaddr *)&client_addr, &addr_len);

        if (bytes < 0) {
            perror("Receive failed");
            continue;
        }

        buffer[bytes] = '\0';

        // Update statistics
        total_bytes += bytes;
        log_count++;

        // Timestamp
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);

        char time_str[30];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);

        printf("[%s] %s\n", time_str, buffer);

        // Calculate throughput
        double elapsed_time = difftime(now, start_time);

        if (elapsed_time > 0) {
            double throughput = total_bytes / elapsed_time;

            printf("Logs received: %d | Total Data: %ld bytes | Throughput: %.2f bytes/sec\n",
                   log_count, total_bytes, throughput);
        }
    }

    close(sockfd);
    return 0;
}
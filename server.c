#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <openssl/evp.h>

#define PORT 5000
#define BUFFER_SIZE 1024

#define KEY "0123456789abcdef"
#define IV  "abcdef9876543210"

// Decrypt function
int decrypt(unsigned char *ciphertext, int ciphertext_len,
            unsigned char *plaintext) {

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len, plaintext_len;

    EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL,
                       (unsigned char *)KEY, (unsigned char *)IV);

    EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len);
    plaintext_len = len;

    EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return plaintext_len;
}

int main() {

    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    unsigned char buffer[BUFFER_SIZE];
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

    // Bind
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Secure Log Server Started on UDP port %d\n", PORT);

    while (1) {

        int bytes = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                             (struct sockaddr *)&client_addr, &addr_len);

        if (bytes < 0) {
            perror("Receive failed");
            continue;
        }

        // Decrypt received message
        unsigned char decrypted[BUFFER_SIZE];
        int decrypted_len = decrypt(buffer, bytes, decrypted);
        decrypted[decrypted_len] = '\0';

        total_bytes += bytes;
        log_count++;

        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char time_str[30];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);

        printf("[%s] %s\n", time_str, decrypted);

        double elapsed_time = difftime(now, start_time);
        if (elapsed_time > 0) {
            double throughput = total_bytes / elapsed_time;
            printf("Logs: %d | Data: %ld bytes | Throughput: %.2f bytes/sec\n",
                   log_count, total_bytes, throughput);
        }
    }

    close(sockfd);
    return 0;
}
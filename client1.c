#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <openssl/evp.h>

#define PORT 5000
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

#define KEY "0123456789abcdef"
#define IV  "abcdef9876543210"

// Encrypt function
int encrypt(unsigned char *plaintext, int plaintext_len,
            unsigned char *ciphertext) {

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len, ciphertext_len;

    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL,
                       (unsigned char *)KEY, (unsigned char *)IV);

    EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len);
    ciphertext_len = len;

    EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return ciphertext_len;
}

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

    printf("Client1 sending encrypted logs...\n");

    while(1) {

        printf("Enter log: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        buffer[strcspn(buffer,"\n")] = 0;

        if(strcmp(buffer,"exit")==0)
            break;

        char message[BUFFER_SIZE + 20];
        snprintf(message, sizeof(message), "Client1: %s", buffer);

        unsigned char ciphertext[BUFFER_SIZE];

        int cipher_len = encrypt((unsigned char *)message,
                                 strlen(message),
                                 ciphertext);

        sendto(sockfd, ciphertext, cipher_len, 0,
               (struct sockaddr*)&server_addr, sizeof(server_addr));
    }

    close(sockfd);
}
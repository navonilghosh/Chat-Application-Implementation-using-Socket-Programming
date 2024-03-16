#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 12345
#define MAX_BUFFER_SIZE 1024

int main() {
    int serverSocket, client1, client2;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size = sizeof(clientAddr);
    char buffer[MAX_BUFFER_SIZE];

    // Create a socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("socket");
        exit(1);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = PORT;
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the given address
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 2) == 0) {
        printf("Server is listening...\n");
    } else {
        perror("listen");
        exit(1);
    }

    // Accept connections from two clients
    client1 = accept(serverSocket, (struct sockaddr *)&clientAddr, &addr_size);
    printf("Client 1 connected from %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

    client2 = accept(serverSocket, (struct sockaddr *)&clientAddr, &addr_size);
    printf("Client 2 connected from %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

    // Chat loop
    while (1) {
        bzero(buffer, sizeof(buffer));

        if (recv(client1, buffer, sizeof(buffer), 0) <= 0) {
            printf("Client 1 has disconnected.\n");
            break;
        }
        printf("Client 1: %s", buffer);
        send(client2, buffer, strlen(buffer), 0);

        bzero(buffer, sizeof(buffer));

        if (recv(client2, buffer, sizeof(buffer), 0) <= 0) {
            printf("Client 2 has disconnected.\n");
            break;
        }
        printf("Client 2: %s", buffer);
        send(client1, buffer, strlen(buffer), 0);
    }

    close(client1);
    close(client2);
    close(serverSocket);
    return 0;
}


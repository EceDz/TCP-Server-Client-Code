//20220702045 Ece Duzgec
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 2025
#define MAXPENDING 5

typedef struct {
    int grid[8][8];
    int min_value;
    int max_value;
    float average;
} sensor_grid;

void DieWithError(char *errorMessage) {
    perror(errorMessage);
    exit(1);
}

void printSensorGrid(sensor_grid *data) {
    printf("\n========== Sensor Grid Data ==========\n");
    printf("8x8 Grid:\n");
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%4d ", data->grid[i][j]);
        }
        printf("\n");
    }
    printf("\nStatistics:\n");
    printf("  Minimum Value: %d\n", data->min_value);
    printf("  Maximum Value: %d\n", data->max_value);
    printf("  Average Value: %.2f\n", data->average);
    printf("======================================\n\n");
}

void handleClient(int clntSocket) {
    sensor_grid receivedData;
    int bytesRcvd;
    
    bytesRcvd = recv(clntSocket, &receivedData, sizeof(sensor_grid), 0);
    
    if (bytesRcvd < 0) {
        DieWithError("recv() failed");
    } else if (bytesRcvd == 0) {
        printf("Client closed connection\n");
    } else if (bytesRcvd == sizeof(sensor_grid)) {
        printSensorGrid(&receivedData);
    } else {
        printf("Warning: Received incomplete data (%d bytes)\n", bytesRcvd);
    }
    
    close(clntSocket);
}

int main() {
    int servSock, clntSock;
    struct sockaddr_in servAddr, clntAddr;
    unsigned int clntLen;
    
    // Create socket
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        DieWithError("socket() failed");
    }
    
    // Set socket option to reuse address
    int opt = 1;
    if (setsockopt(servSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        DieWithError("setsockopt() failed");
    }
    
    // Construct server address structure
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(PORT);
    
    // Bind to local address
    if (bind(servSock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        DieWithError("bind() failed");
    }
    
    // Listen for incoming connections
    if (listen(servSock, MAXPENDING) < 0) {
        DieWithError("listen() failed");
    }
    
    printf("Server listening on port %d...\n", PORT);
    
    // Run forever
    for (;;) {
        clntLen = sizeof(clntAddr);
        
        if ((clntSock = accept(servSock, (struct sockaddr *)&clntAddr, &clntLen)) < 0) {
            DieWithError("accept() failed");
        }
        
        printf("Handling client %s\n", inet_ntoa(clntAddr.sin_addr));
        
        handleClient(clntSock);
    }
    
    return 0;
}

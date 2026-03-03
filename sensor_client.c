////20220702045 Ece Duzgec
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define PORT 2025
#define SERVER_IP "127.0.0.1"

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

void prepareSensorGrid(sensor_grid *data) {
    int sum = 0;
    data->min_value = 1000;
    data->max_value = 0;
    
    // Fill grid with random values and compute statistics
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            data->grid[i][j] = rand() % 1001; // Random value between 0 and 1000
            sum += data->grid[i][j];
            
            if (data->grid[i][j] < data->min_value) {
                data->min_value = data->grid[i][j];
            }
            if (data->grid[i][j] > data->max_value) {
                data->max_value = data->grid[i][j];
            }
        }
    }
    
    data->average = sum / 64.0;
}

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in servAddr;
    sensor_grid data;
    char *servIP;
    
    // Use command line argument for server IP if provided, otherwise use localhost
    if (argc >= 2) {
        servIP = argv[1];
    } else {
        servIP = SERVER_IP;
    }
    
    // Seed random number generator
    srand(time(NULL));
    
    // Create socket
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        DieWithError("socket() failed");
    }
    
    // Construct server address structure
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(servIP);
    servAddr.sin_port = htons(PORT);
    
    // Connect to server
    if (connect(sock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        DieWithError("connect() failed");
    }
    
    printf("Connected to server %s on port %d\n", servIP, PORT);
    
    // Prepare sensor grid data
    prepareSensorGrid(&data);
    
    printf("Sending sensor grid data...\n");
    printf("  Min: %d, Max: %d, Average: %.2f\n", 
           data.min_value, data.max_value, data.average);
    
    // Send sensor grid structure to server
    if (send(sock, &data, sizeof(sensor_grid), 0) != sizeof(sensor_grid)) {
        DieWithError("send() sent a different number of bytes than expected");
    }
    
    printf("Data sent successfully!\n");
    
    close(sock);
    return 0;
}

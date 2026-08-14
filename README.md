# TCP Server-Client Sensor Grid

A simple C client-server application demonstrating raw TCP socket programming on Linux. The client simulates an 8x8 grid of sensor readings, computes basic statistics, and sends the data to the server over a TCP connection. The server receives and prints the grid.

## How It Works

1. **Server** (`sensor_server.c`) binds to a TCP port and listens for incoming connections. For each client, it receives a `sensor_grid` struct and prints the grid values along with the min, max, and average.
2. **Client** (`sensor_client.c`) generates an 8x8 grid of random integers (0–1000), computes the minimum, maximum, and average, then connects to the server and sends the struct directly over the socket.

Data is exchanged as a raw, fixed-size C struct:

```c
typedef struct {
    int grid[8][8];
    int min_value;
    int max_value;
    float average;
} sensor_grid;
```

## Requirements

- Linux (uses POSIX sockets: `sys/socket.h`, `arpa/inet.h`, `unistd.h`)
- GCC or another C compiler

## Building

```bash
gcc -o sensor_server sensor_server.c
gcc -o sensor_client sensor_client.c
```

## Usage

1. Start the server (listens on port `2025` by default):

   ```bash
   ./sensor_server
   ```

2. In another terminal, run the client. By default it connects to `127.0.0.1`, or you can pass a server IP as an argument:

   ```bash
   ./sensor_client
   # or, to connect to a remote server:
   ./sensor_client 192.168.1.10
   ```

3. The client generates and sends a random 8x8 grid; the server prints the received grid and its statistics:

   ```
   ========== Sensor Grid Data ==========
   8x8 Grid:
    123  456  789 ...
   ...

   Statistics:
     Minimum Value: 12
     Maximum Value: 987
     Average Value: 512.34
   ======================================
   ```

## Notes

- The server handles one client connection at a time (no threading/forking).
- Port and buffer sizes are configured via `#define` constants (`PORT 2025`) at the top of each file.
- This project was written as an exercise in raw socket programming (`socket()`, `bind()`, `listen()`, `accept()`, `connect()`, `send()`, `recv()`).

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

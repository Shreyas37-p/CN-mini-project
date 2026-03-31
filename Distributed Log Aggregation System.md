# Distributed Log Aggregation System

A lightweight UDP-based distributed log aggregation system implemented in C, offering optional DTLS certificate support for secure communication.

---

## Project Structure

```
.
├── client1.c          # UDP log client (Client 1)
├── client2.c          # UDP log client (Client 2)
├── server.c           # UDP log aggregation server
├── ssl_setup.sh        # Script to generate self-signed TLS/DTLS certificate
├── server.crt         # Generated certificate (after running ssl_setup.sh)
└── server.key         # Generated private key (after running ssl_setup.sh)
```

---

## Overview

This project illustrates a fundamental distributed logging architecture:

- **Clients** (e.g., `client1.c`, `client2.c`) transmit labeled log messages over UDP to a central server.
- The **Server** receives, timestamps, and displays all incoming logs, while also tracking throughput statistics.
- A **Certificate script** facilitates the generation of a self-signed X.509 certificate for potential future integration with DTLS (Datagram Transport Layer Security) to enable encrypted communication.

---

## Prerequisites

To compile and run this system, the following are required:

- **GCC** or any compatible C compiler.
- **Linux/macOS** operating system, as the system utilizes POSIX sockets (`<arpa/inet.h>`, `<unistd.h>`).
- **OpenSSL** library, primarily for certificate generation purposes.

---

## Configuration

Before compilation, it is necessary to update the following constants within `client1.c` and `client2.c` if the default values are not suitable:

| Constant      | Default Value    | Description                                   |
|---------------|------------------|-----------------------------------------------|
| `PORT`        | `5000`           | The UDP port used for communication.          |
| `SERVER_IP`   | `10.221.134.118` | The IP address of the log aggregation server. |
| `BUFFER_SIZE` | `1024`           | The maximum size of a log message in bytes.   |

The server is configured to listen on `INADDR_ANY` (all available network interfaces) on port `5000` by default.

---

## Compilation

Compile the server and both clients separately using the following commands:

```bash
# Compile the server
gcc -o server server.c

# Compile Client 1
gcc -o client1 client1.c

# Compile Client 2
gcc -o client2 client2.c
```

---

## Running the System

### 1. Start the Server

Execute the server application on the machine designated for log aggregation:

```bash
./server
```

**Expected Output:**
```
Distributed Log Aggregation Server Started
```

The server will continuously monitor for incoming UDP packets and display:
- A timestamp for each received log entry.
- The log message, including its source label (e.g., `Client1:`).
- Real-time statistics: total log count, cumulative bytes received, and current throughput (bytes/second).

### 2. Start the Clients

Run each client application on separate machines, or on the same machine for testing purposes:

```bash
./client1
```

```bash
./client2
```

**Expected Output (for each client):**
```
ClientX sending logs...
Enter log: 
```

Upon prompting, type any log message and press Enter to transmit it. Each client automatically prefixes its messages with its respective label (e.g., `Client1:`, `Client2:`) before sending them to the server.

To terminate a client, type `exit` and press Enter:
```
exit
```

---

## Certificate Generation (for DTLS)

To generate a self-signed certificate for securing UDP communication with DTLS, execute the provided script:

```bash
chmod +x ssl_setup.sh
./ssl_setup.sh
```

This process will generate two essential files:
- `server.crt`: A self-signed X.509 certificate, valid for 365 days.
- `server.key`: An RSA 2048-bit private key.

**Certificate Details:**
| Field          | Value         |
|----------------|---------------|
| Country        | IN            |
| State          | Karnataka     |
| City           | Bangalore     |
| Organization   | PES University|
| Unit           | CSE           |
| Common Name    | localhost     |

> **Note:** The current client and server implementations utilize plain UDP. The generated certificate is intended for future integration with a DTLS library, such as OpenSSL's DTLS or wolfSSL, to enable secure communication.

---

## How It Works

### Clients (`client1.c`, `client2.c`)
1.  Each client creates a UDP socket (`SOCK_DGRAM`).
2.  It configures the server's address using `inet_pton`.
3.  Log messages are read from standard input in a continuous loop.
4.  A unique prefix (e.g., `"Client1: "`, `"Client2: "`) is prepended to each message before transmission via `sendto`.
5.  The client gracefully exits upon receiving the input `"exit"`.

### Server (`server.c`)
1.  The server creates and binds a UDP socket to `INADDR_ANY:5000`.
2.  It enters an infinite loop, awaiting incoming packets using `recvfrom`.
3.  For each received packet:
    - The message is null-terminated and printed to standard output, accompanied by a timestamp.
    - The total byte count and log count are updated.
    - Throughput (bytes/second) is calculated and displayed, reflecting the data rate since the server's inception.

---

## Example Session

**Server Terminal:**
```
Distributed Log Aggregation Server Started
[2025-03-30 10:15:42] Client1: Server started successfully
Logs received: 1 | Total Data: 34 bytes | Throughput: 34.00 bytes/sec
[2025-03-30 10:15:48] Client2: CPU usage at 12%
Logs received: 2 | Total Data: 55 bytes | Throughput: 27.50 bytes/sec
[2025-03-30 10:15:55] Client1: Disk space low
Logs received: 3 | Total Data: 70 bytes | Throughput: 23.33 bytes/sec
```

**Client 1 Terminal:**
```
Client1 sending logs...
Enter log: Server started successfully
Enter log: Disk space low
Enter log: exit
```

**Client 2 Terminal:**
```
Client2 sending logs...
Enter log: CPU usage at 12%
Enter log: exit
```

---

## Limitations & Future Work

- The current implementation relies on **plain UDP**, which does not guarantee reliability (packets may be lost or arrive out of order).
- **No encryption** is present in the current version; DTLS support can be integrated using the generated certificates to enhance security.
- **No persistent log storage** is implemented; logs are currently only printed to `stdout` on the server.
- **Error handling** could be further robustified for production environments.

---

## Authors

Developed at **PES University, Department of CSE**, Bangalore, Karnataka, India.

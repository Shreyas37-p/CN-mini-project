# Distributed Log Aggregation System

A lightweight UDP-based distributed log aggregation system written in C, with optional DTLS certificate support for secure communication.

---

## Project Structure

```
.
├── client1.c          # UDP log client (Client 1)
├── server.c           # UDP log aggregation server
├── ssl_setup.sh        # Script to generate self-signed TLS/DTLS certificate
├── server.crt         # Generated certificate (after running gen_cert.sh)
└── server.key         # Generated private key (after running gen_cert.sh)
```

---

## Overview

This project demonstrates a simple distributed logging architecture:

- **Client(s)** send labeled log messages over UDP to a central server.
- **Server** receives, timestamps, and displays all incoming logs while tracking throughput statistics.
- **Certificate script** generates a self-signed X.509 certificate for use with DTLS (Datagram TLS) if encryption is added in the future.

---

## Prerequisites

- GCC or any C compiler
- Linux/macOS (uses POSIX sockets: `<arpa/inet.h>`, `<unistd.h>`)
- OpenSSL (for certificate generation only)

---

## Configuration

Before compiling, update the following constants in `client1.c` if needed:

| Constant | Default Value | Description |
|---|---|---|
| `PORT` | `5000` | UDP port to communicate on |
| `SERVER_IP` | `10.221.134.118` | IP address of the server machine |
| `BUFFER_SIZE` | `1024` | Max size of a log message |

The server listens on `INADDR_ANY` (all interfaces) on port `5000` by default.

---

## Compilation

Compile the server and client separately:

```bash
# Compile the server
gcc -o server server.c

# Compile the client
gcc -o client1 client1.c
```

---

## Running the System

### 1. Start the Server

Run this on the machine that will aggregate logs:

```bash
./server
```

Expected output:
```
Distributed Log Aggregation Server Started
```

The server will continuously listen for incoming UDP packets and print:
- Timestamp of each received log
- The log message with its source label
- Running statistics: log count, total bytes received, and throughput (bytes/sec)

### 2. Start the Client

Run this on the client machine (or same machine for testing):

```bash
./client1
```

Expected output:
```
Client1 sending logs...
Enter log: 
```

Type any log message and press Enter to send it. All messages are automatically prefixed with `Client1:` before being sent to the server.

To exit the client, type:
```
exit
```

---

## Certificate Generation (for DTLS)

To generate a self-signed certificate for securing UDP communication with DTLS:

```bash
chmod +x gen_cert.sh
./gen_cert.sh
```

This creates two files:
- `server.crt` — Self-signed X.509 certificate (valid for 365 days)
- `server.key` — RSA 2048-bit private key

Certificate details:
| Field | Value |
|---|---|
| Country | IN |
| State | Karnataka |
| City | Bangalore |
| Organization | PES University |
| Unit | CSE |
| Common Name | localhost |

> **Note:** The current client and server use plain UDP. The certificate is prepared for future integration with a DTLS library such as OpenSSL's DTLS or wolfSSL.

---

## How It Works

### Client (`client1.c`)
1. Creates a UDP socket (`SOCK_DGRAM`).
2. Configures the server address using `inet_pton`.
3. Reads log messages from stdin in a loop.
4. Prepends `"Client1: "` to each message and sends it via `sendto`.
5. Exits cleanly on input `"exit"`.

### Server (`server.c`)
1. Creates and binds a UDP socket to `INADDR_ANY:5000`.
2. Waits for packets using `recvfrom` in an infinite loop.
3. For each packet:
   - Null-terminates and prints the message with a timestamp.
   - Updates total byte count and log count.
   - Calculates and prints throughput (bytes/sec) since server start.

---

## Example Session

**Server terminal:**
```
Distributed Log Aggregation Server Started
[2025-03-30 10:15:42] Client1: Server started successfully
Logs received: 1 | Total Data: 34 bytes | Throughput: 34.00 bytes/sec
[2025-03-30 10:15:48] Client1: CPU usage at 12%
Logs received: 2 | Total Data: 55 bytes | Throughput: 27.50 bytes/sec
```

**Client terminal:**
```
Client sending logs...
Enter log: Server started successfully
Enter log: CPU usage at 12%
Enter log: exit


Client1 sending logs...
Enter log: Server started successfully
Enter log: CPU usage at 12%
Enter log: exit
```

---

## Limitations & Future Work

- Currently uses **plain UDP** — no reliability guarantees (packets may be lost or arrive out of order).
- No encryption in the current version; DTLS support can be added using the generated certificates.
- Only one client (`client1.c`) is included — additional clients can be created by copying and updating the `Client1:` label prefix.
- No persistent log storage; logs are only printed to stdout on the server.

---

## Authors

Developed at **PES University, Department of CSE**, Bangalore, Karnataka, India.

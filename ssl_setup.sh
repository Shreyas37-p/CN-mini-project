#!/bin/bash

echo "Generating self-signed certificate for UDP security (DTLS)..."

openssl req -x509 -newkey rsa:2048 
-keyout server.key 
-out server.crt 
-days 365 
-nodes 
-subj "/C=IN/ST=Karnataka/L=Bangalore/O=PES University/OU=CSE/CN=localhost/emailAddress=[shreyas@example.com](mailto:shreyas@example.com)"

echo "Certificate generated successfully!"
echo "Files created:"
echo "server.crt"
echo "server.key"

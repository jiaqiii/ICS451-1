#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    unsigned int dest_port; // will specify port to open
    struct sockaddr_in remote_addr; // creates structure that can store IP addresses, one for client, one for server
    int addrlen = sizeof(remote_addr);
    int socket_fd; // creates file descriptor for tcp header to be received
    unsigned char tcp_header[20]; // buffer for tcp_header
    unsigned int src_port;
    
    // Checking if argument is given as port number
    if(argc <= 1)
    {
        printf("%d Please specify a port number as an argument\n",argc);
        exit(1);
    }
    else if(argc > 2)
    {
        printf("%d Please specify only a single port number as an argument.\n",argc);
        exit(1);
    }
    else
    {
        // sets dest port number to 1st argument
        dest_port = (unsigned int) atoi(argv[1]);
    }

    // puts dest port into the TCP header
    tcp_header[2] = dest_port >> 8;
    tcp_header[3] = dest_port;

    // sets up to  use IPv4 addresses and assigns IP address and port
    remote_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "localhost", &(remote_addr.sin_addr));
    remote_addr.sin_port = htons(dest_port);

    // sets up socket tcp_header descriptor to use IPv4 (AF_INET), TCP (SOCK_STREAM), and to be done via IP (0)
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    // connexts tcp_header descriptor to address, if it returns -1, error, else everything is fine
    if(connect(socket_fd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
    {
        fprintf(stderr, "error on connect");
    }
    // connected to server
    printf("Connected to server.\n");
    // get src port number
    getsockname(socket_fd, (struct sockaddr*) &remote_addr, &addrlen);
    src_port = ntohs(remote_addr.sin_port);
    tcp_header[0] = src_port >> 8;
    tcp_header[1] = src_port;
    
    // receive tcp_header from client to buffer of 15000 bytes
   //printf("Receiving tcp_header from server.\n\n");
   //recv(socket_fd, tcp_header, sizeof(tcp_header), 0);
    // print tcp_header to the screen
//    printf("%s", tcp_header);
} 

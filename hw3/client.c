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
    int port; // will specify port to open
    struct sockaddr_in remote_addr; // creates structure that can store IP addresses, one for client, one for server
    int socket_fd; // creates file descriptor for file to be received
    char file[15000]; // buffer for file

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
        // sets port number to 1st argument
        port = atoi(argv[1]);
    }

    // sets up to  use IPv4 addresses and assigns IP address and port
    remote_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "localhost", &(remote_addr.sin_addr));
    remote_addr.sin_port = htons(port);

    // sets up socket file descriptor to use IPv4 (AF_INET), TCP (SOCK_STREAM), and to be done via IP (0)
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    // connexts file descriptor to address, if it returns -1, error, else everything is fine
    if(connect(socket_fd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
    {
        fprintf(stderr, "error on connect");
    }
    // connected to server
    printf("Connected to server.\n");
    // receive file from client to buffer of 15000 bytes
    printf("Receiving file from server.\n\n");
    recv(socket_fd, file, sizeof(file), 0);
    // print file to the screen
    printf("%s", file);
}

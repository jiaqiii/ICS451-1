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

char webpage[] = "this is a webpage\n";
int main(int argc, char argv[])
{
    struct sockaddr_in server_addr, client_addr;    
    socklen_t sin_len = sizeof(client_addr);
    int server_socket, client_socket;
    int on = 1;
   
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0)
    {
        perror("socket");
        exit(1);
    }
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(25000);

    if(bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind");
        close(server_socket);
        exit(1);
    }

    if(listen(server_socket, 10) == -1)
    {
        perror("listen");
        close(server_socket);
        exit(1);
    }

    while(1)
    {
        client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &sin_len);
        if(!fork())
        {
            // child process
           close(server_socket);
           int fd = open("ArduinoIDE_guide_galileo.html",O_RDONLY);
           //int offset = 0;
           sendfile(client_socket, fd, NULL, 15000);
           close(fd);
           printf("Closing connection to client.\n");
           close(client_socket);
           exit(0);
        }
        close(client_socket);
    }
    return 0;
}

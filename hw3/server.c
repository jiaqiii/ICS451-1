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
    struct sockaddr_in server_addr, client_addr; // creates structure that can store IP addresses, one for client, one for server
    socklen_t sin_len = sizeof(client_addr); // specifies length of address
    int server_fd, client_fd; // creates file descriptors for client and server
    char disconnected_msg[32] = "\nClosing connection to server.\n";
    int on = 1;
    
    // Checking if argument is given as port number
    if(argc <= 1)
    {
        printf("Please specify a port number as an argument\n",argc);
        exit(1);
    }
    else if(argc > 2)
    {
        printf("Please specify only a single port number as an argument.\n",argc);
        exit(1);
    }
    else
    {
        // sets port number to 1st argument
        port = atoi(argv[1]);
    }

    // sets up server socket to use IPv4 (AF_INET), TCP (SOCK_STREAM), and to be done via IP (0)
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // If running socket returns a negative value, it is an error, exit program.
    if(server_fd < 0)
    {
        perror("socket");
        exit(1);
    }

    // sets socket options to reuse the address
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));
    // specifies address format as IPv4
    server_addr.sin_family = AF_INET;
    // specifies actual address of the host machine, can be any address in this case
    server_addr.sin_addr.s_addr = INADDR_ANY;
    // specifies which port to use on the host machine, set this as the argument given to the user
    server_addr.sin_port = htons(port);

    // binds the socket to the address and port number, if it returns a negative value, some error occurred
    if(bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind");
        close(server_fd);
        exit(1);
    }

    // the server is listening for a connection, returns negative 1 if error occurred
    if(listen(server_fd, 10) == -1)
    {
        perror("listen");
        close(server_fd);
        exit(1);
    }

    // server stays listening while this loop is open
    while(1)
    {
        // connects filedescriptor to client
        client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &sin_len);
        // creates child process to handle connection while server is listening
        if(!fork())
        {
           // child process
           printf("Client has connected.\n"); 
           close(server_fd); // closes server_fd file descriptor
           int fd = open("ArduinoIDE_guide_galileo.html",O_RDONLY); // opens file as a file descriptor to be sent
           sendfile(client_fd, fd, NULL, 15000); //sends file to connected client
           close(fd); // close file descriptor of file transferred
           printf("Closing connection to client.\n"); 
           write(client_fd, disconnected_msg, strlen(disconnected_msg));
           close(client_fd); // closes client connection
           exit(0);
        }
        close(client_fd);
    }
    return 0;
}

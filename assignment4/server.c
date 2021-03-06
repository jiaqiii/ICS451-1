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
    int on = 1;
    unsigned int src_port;
    unsigned char syn[20];
    unsigned char syn_ack[20];
    unsigned char ack[20];
    unsigned char fin[20];
    unsigned char ack2[20];
    unsigned char fin2[20];
    unsigned char ack3[20];

    ssize_t len;
    int sent_bytes = 0;
    int offset;
    char file_size[256];
    struct stat file_stat;
    int remain_data;
    
    // Checking if argument is given as port number
    if(argc <= 1)
    {
        printf("Please specify a port number as an argument\n");
        exit(1);
    }
    else if(argc > 2)
    {
        printf("Please specify only a single port number as an argument.\n");
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

    int fd = open("image.png",O_RDONLY); // opens file as a file descriptor to be sent
    // get file stats
    fstat(fd, &file_stat);

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
        sprintf(file_size, "%d", file_stat.st_size);
        // creates child process to handle connection while server is listening
        if(!fork())
        {
           // child process
           printf("Client has connected.\n"); 
           printf("Recieving TCP SYN handshake from client.\n\n");
           close(server_fd); // closes server_fd file descriptor
           recv(client_fd, syn, 20, 0);
           int i;
           for(i = 0;i<20;i++)
           {
               printf("TCP SYN Header Byte #%d: %d\n", i, syn[i]);
           }
           // get src port number
           getsockname(client_fd, (struct sockaddr*) &client_addr, &sin_len);
           src_port = ntohs(client_addr.sin_port);
           // putting source port number into SYN ACK TCP header
           syn_ack[0] = src_port >> 8;
           syn_ack[1] = src_port;
           // putting dest number into SYN ACK TCP header
           syn_ack[2] = syn[0];
           syn_ack[3] = syn[1];
           // putting sequence numbers in, basically the ack number
           syn_ack[4] = syn[8];
           syn_ack[5] = syn[9];
           syn_ack[6] = syn[10];
           syn_ack[7] = syn[11];
           // putting ack number in (sequence number + 1)
           syn_ack[8] = syn[4];
           syn_ack[9] = syn[5];
           syn_ack[10] = syn[6];
           syn_ack[11] = syn[7] + 1;
           // setting data offset and reserved bits to 0 in tcp header
           syn_ack[12] = 0x00;
           // setting SYN ACK flags in tcp header, hex value is 0x12
           syn_ack[13] = 0x12;
           // setting window size to 17520 bytes (0x4470 in hex)
           syn_ack[14] = 0x44;
           syn_ack[15] = 0x70;
           // setting checksum to 0xffff
           syn_ack[16] = 0xff;
           syn_ack[17] = 0xff;
           // setting URG pointer to all zeros
           syn_ack[18] = 0x00;
           syn_ack[19] = 0x00;
           printf("\nSending TCP SYN ACK packet in response to SYN.\n");
           send(client_fd, syn_ack, 20, 0);
           printf("\nReceiving TCP ACK packet in response to SYN ACK.\n");
           recv(client_fd, ack, 20, 0);
           int j;
           for(j = 0;j<20;j++)
           {
               printf("TCP SYN-ACK Header Byte #%d: %d\n", j, ack[j]);
           }
           len = send(client_fd, file_size, sizeof(file_size), 0); 
           offset = 0;
           remain_data = file_stat.st_size;
           while(((sent_bytes = sendfile(client_fd, fd, &offset, 1000)) > 0) && (remain_data > 0))
           {
               printf("Server sent %d bytes from file's data and remaining data is %d\n", sent_bytes, remain_data);
               remain_data -= sent_bytes;
               printf("Server sent %d bytes from file's data and remaining data is %d\n", sent_bytes, remain_data);
           }
           // receiving FIN from client
           recv(client_fd, fin, 20, 0);
           for(j = 0;j<20;j++)
           {
               printf("TCP FIN Header Byte #%d: %d\n", j, fin[j]);
           }
           // putting source port number into TCP header
           ack2[0] = src_port >> 8;
           ack2[1] = src_port;
           // putting dest number into TCP header
           ack2[2] = fin[0];
           ack2[3] = fin[1];
           // putting sequence numbers in, basically the ack number
           ack2[4] = fin[8];
           ack2[5] = fin[9];
           ack2[6] = fin[10];
           ack2[7] = fin[11];
           // putting ack number in (sequence number + 1)
           ack2[8] = fin[4];
           ack2[9] = fin[5];
           ack2[10] = fin[6];
           ack2[11] = fin[7] + 1;
           // setting data offset and reserved bits to 0 in tcp header
           ack2[12] = 0x00;
           // setting ACK flag in tcp header, hex value is 0x10
           ack2[13] = 0x10;
           // setting window size to 17520 bytes (0x4470 in hex)
           ack2[14] = 0x44;
           ack2[15] = 0x70;
           // setting checksum to 0xffff
           ack2[16] = 0xff;
           ack2[17] = 0xff;
           // setting URG pointer to all zeros
           ack2[18] = 0x00;
           ack2[19] = 0x00;
           // sending ACK to client for FIN
	   send(client_fd, ack2, 20, 0);
         
           // sending FIN to client 
           send(client_fd, fin2, 20, 0);
           // putting source port number into FIN TCP header
           fin2[0] = src_port >> 8;
           fin2[1] = src_port;
           // putting dest number into TCP header
           fin2[2] = ack2[0];
           fin2[3] = ack2[1];
           // putting sequence numbers in, basically the ack number
           fin2[4] = ack2[8];
           fin2[5] = ack2[9];
           fin2[6] = ack2[10];
           fin2[7] = ack2[11];
           // putting ack number in (sequence number + 1)
           fin2[8] = ack2[4];
           fin2[9] = ack2[5];
           fin2[10] = ack2[6];
           fin2[11] = ack2[7] + 2; // adding 1 more as simply adding one will be next seq number
           // setting data offset and reserved bits to 0 in tcp header
           fin2[12] = 0x00;
           // setting FIN flag in tcp header, hex value is 0x01
           fin2[13] = 0x01;
           // setting window size to 17520 bytes (0x4470 in hex)
           fin2[14] = 0x44;
           fin2[15] = 0x70;
           // setting checksum to 0xffff
           fin2[16] = 0xff;
           fin2[17] = 0xff;
           // setting URG pointer to all zeros
           fin2[18] = 0x00;
           fin2[19] = 0x00;
           send(client_fd, fin2, 20, 0);
           recv(client_fd, ack3, 20, 0);
           for(j = 0;j<20;j++)
           {
               printf("TCP final ACK Header Byte #%d: %d\n", j, ack3[j]);
           }
 
           printf("Closing connection to client.\n"); 
           close(client_fd); // closes client connection
           exit(0);
        }
        close(fd); // close file descriptor of file transferred
        close(client_fd);
    }
    return 0;
}

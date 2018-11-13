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
    struct sockaddr_in remote_addr; // creates structure that can store IP addresses, one for client, one for server
    int addrlen = sizeof(remote_addr);
    int socket_fd; // creates file descriptor for tcp header to be sent and received
    char file[15000]; // buffer for file
    unsigned char syn[20]; // buffer for initial SYN tcp header
    unsigned char syn_ack[20]; // buffer for SYN-ACK tcp header
    unsigned char ack[20]; // buffer for ACK tcp header
    unsigned int src_port; // will specify source port that was opened
    unsigned int dest_port; // will specify port to open
    unsigned int seq_num0; // will generate random seq number byte 1
    unsigned int seq_num1; // will generate random seq number byte 2
    unsigned int seq_num2; // will generate random seq number byte 3
    unsigned int seq_num3; // will generate random seq number byte 4
    
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
    syn[2] = dest_port >> 8;
    syn[3] = dest_port;

    // sets up to  use IPv4 addresses and assigns IP address and port
    remote_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "localhost", &(remote_addr.sin_addr));
    remote_addr.sin_port = htons(dest_port);

    // sets up socket syn descriptor to use IPv4 (AF_INET), TCP (SOCK_STREAM), and to be done via IP (0)
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    // connexts syn descriptor to address, if it returns -1, error, else everything is fine
    if(connect(socket_fd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
    {
        fprintf(stderr, "error on connect");
    }
    // connected to server
    printf("Connected to server.\n");
    // get src port number
    getsockname(socket_fd, (struct sockaddr*) &remote_addr, &addrlen);
    src_port = ntohs(remote_addr.sin_port);
    syn[0] = src_port >> 8;
    syn[1] = src_port;
    srand(time(NULL)); 
    seq_num0 = rand(); seq_num2 = rand(); seq_num1 = rand(); seq_num3 = rand(); // generates random seq numbers
    // put randomly generated seq numbers in tcp header
    syn[4] = seq_num0;
    syn[5] = seq_num1;
    syn[6] = seq_num2;
    syn[7] = seq_num3;
    // Creates the put ack number in tcp header, which is the sequence number + 1
    syn[8] = seq_num0;
    syn[9] = seq_num1;
    syn[10] = seq_num2;
    syn[11] = seq_num3 + 1;
    // setting data offset and reserved bits to 0 in tcp header
    syn[12] = 0x00;
    // Setting only SYN flag in header, which is second to last flag, so decimal value of 0x02
    syn[13] = 0x02;
    // setting window size to 17520 bytes (0x4470 in hex)
    syn[14] = 0x44;
    syn[15] = 0x70;
    // setting checksum to 0xffff
    syn[16] = 0xff;
    syn[17] = 0xff;
    // setting URG pointer to all zeros
    syn[18] = 0x00;
    syn[19] = 0x00;
    // receive tcp header from client to buffer of 20 bytes
    printf("Sending TCP SYN packet to server.\n\n");
    send(socket_fd, syn, 20, 0);
    printf("Receiving SYN ACK from server.\n\n");
    recv(socket_fd, syn_ack, 20, 0);
    int j;
    for(j = 0;j<20;j++)
    {
        printf("TCP SYN-ACK Header Byte #%d: %d\n", j, syn_ack[j]);
    }
    // setting source packet number for ack packet
    ack[0] = src_port >> 8;
    ack[1] = src_port;
    // setting dest packet number for ack packet
    ack[2] = dest_port >> 8;
    ack[3] = dest_port;
    // put sequence in, basically ack number from SYN ACK packet
    ack[4] = syn_ack[8];
    ack[5] = syn_ack[9];
    ack[6] = syn_ack[10];
    ack[7] = syn_ack[11];
    // putting ack number in, (sequence number from SYN ACK + 1)
    ack[8] = syn_ack[4];
    ack[9] = syn_ack[5];
    ack[10] = syn_ack[6];
    ack[11] = syn_ack[7] + 1;
    // setting data offset and reserved bits to 0 in tcp header
    ack[12] = 0x00;
    // setting SYN ACK flags in tcp header, hex value is 0x10
    ack[13] = 0x10;
    // setting window size to 17520 (0x4470)
    ack[14] = 0x44;
    ack[15] = 0x70;
    // setting checksum to 0xffff
    ack[16] = 0xff;
    ack[17] = 0xff;
    // setting URG pointer to all zeros
    ack[18] = 0x00;
    ack[19] = 0x00;
    printf("\nSending TCP ACK packet in response to SYN ACK.\n\n");
    send(socket_fd, ack, 20, 0);
    printf("Receiving file\n\n");
    int k;
    for(k = 0;k<20;k++)
    {
        printf("Sent TCP ACK Header Byte #%d: %d\n", j, ack[k]);
    }
    recv(socket_fd, file , sizeof(file), 0);
    // print file to the screen
    printf("%s", file);
} 

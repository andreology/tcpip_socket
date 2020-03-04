//Andre Barajas, Gabriel Espejo
//CS 327, Spring 2020
//Project 2
//A TCP/IP based server utilizing UDP protocol to
//transfer files and communicate with clients
#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <dirent.h>

#define sendrecvflag 0

//helper functions
//From FileTransfer Examplse
void clearBuf(char *b)
{
    int i;
    for (i = 0; i < 32; i++)
        b[i] = '\0';
}
int sendFile(FILE *fp, char *buf, int s)
{
    char ch;
    for (int i = 0; i < s; i++)
    {
        ch = fgetc(fp);
       // printf("%c", ch);
        buf[i] = ch;
        
        if (ch == EOF)
        {
         //   printf("\nEOF\n");
            return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    char *ip_address[20];
    int port;
    struct sockaddr_in socket_connect;
    char client_buffer[32];
    //reading input for ip address and port
    sscanf(argv[1], "%s", &ip_address);
    sscanf(argv[2], "%d", &port);
    int sc_length = sizeof(socket_connect);
    FILE *fp;
    //represents an address family, where most internet apps use AF_INET
    socket_connect.sin_family = AF_INET;
    //a 16-bit port number in Network Byte order
    socket_connect.sin_port = htons(port);
    //convert IPv4 and IPv6 to binary
    socket_connect.sin_addr.s_addr = inet_addr(ip_address);

    //creating socket for client
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    //connecting to server
    if (connect(client_socket, (struct sockaddr *)&socket_connect, sizeof(socket_connect)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    else
    {
        printf("\n>>> Connection established\n");
    }
    char dir[] = "dir";
    char con_mess[] = ">>> Connected!\n";
    printf("\nIP: %s Port: %d\n", ip_address, port);

    while (1)
    {
        sendto(client_socket, con_mess, 32, 0, (struct sockaddr *)&socket_connect, sc_length);

        clearBuf(client_buffer);
        printf("\nEnter a valid file name on the server or enter “dir” command:\n");
        scanf("%s", client_buffer); //user enters value
        fp = fopen(client_buffer, "r");
        
        if(fp == NULL && strcmp("exit",client_buffer) == 0)
        {
            printf("\nexiting...\n");
            exit(0);
        }
        else if(fp == NULL && strcmp("dir",client_buffer) == 0)
        {
            sendto(client_socket, client_buffer, 32, 0, (struct sockaddr *)&socket_connect, sc_length);
            clearBuf(client_buffer);
            char dir_mess[100];
            int z = recvfrom(client_socket, dir_mess, 100,
                                0, (struct sockaddr *)&socket_connect,
                                &sc_length);
            printf("\nDirectories:\n");
            printf("%s", dir_mess);
        }
        else if (fp == NULL) //File is not in directory
        {
            printf("\nFile not found\n");
        }
        else //file is in Client directory
        {
            sendto(client_socket, client_buffer, 32, 0, (struct sockaddr *)&socket_connect, sc_length);
          //  while (1)
          //  {
                clearBuf(client_buffer);
                int y = recvfrom(client_socket, client_buffer, 32,
                                0, (struct sockaddr *)&socket_connect,
                                &sc_length);
                if (strcmp("The file already exist!", client_buffer) == 0)        //File already in server directory
                {
                    printf("\n%s\n",client_buffer);
                    exit(0);
                }
                else if (strcmp(">>> file exists!", client_buffer) == 0)          //Tranfer File
                {
                    printf("\n%s\n",client_buffer);
                    if (sendFile(fp, client_buffer, 32))
                    {
                        send(client_socket, client_buffer, strlen(client_buffer), 0);
                    }
                    //fclose(fp);
                    exit(0);
                }
                
           // }
        }
    //    /*------------------Req 6------------------*/
    //   else if(fp == NULL && strcmp(client_buffer, dir) == 0){
    //
    //      write(client_socket, client_buffer,sizeof(client_buffer));
    //      bzero(client_buffer,sizeof(client_buffer));
    //
    //      //Doesn't Work v
    //      recvfrom(client_socket, client_buffer, 32,
    //                   0, (struct sockaddr*)&socket_connect, &sc_length);
    //
    //      printf("\ndirectories:\n");
    //      printf("%s", client_buffer);
    //      bzero(client_buffer,sizeof(client_buffer));
    //   }
    
    }
}

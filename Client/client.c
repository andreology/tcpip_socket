//Andre Barajas, Gabriel
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
//Taken from FileTransfer example
int sendFile(FILE* fp, char* buf, int s) 
{  
	// if (fp == NULL) { 
	// 	strcpy(buf, nofile); 
	// 	int len = strlen(nofile); 
	// 	buf[len] = EOF; 
	// 	//for (i = 0; i <= len; i++) 
	// 		//buf[i] = Cipher(buf[i]); 
  //   return 1; 
	// } 
	char ch; 
	for (int i = 0; i < s; i++) { 
		ch = fgetc(fp);  
		buf[i] = ch; 
		if (ch == EOF) 
			return 1; 
	} 
	return 0; 
}

int assign_sock_addy(struct sockaddr_in addy, int port, char ip) {
  int validate = 1;

  return validate;
}

void socket_descriptor(int socket) {
  //taken from professor Fahim code.
  if (socket < 0)
		printf("\nfile descriptor not received!!\n");
	else
		printf("\nfile descriptor %d received\n", socket);
}

int main(int argc, char *argv[]) {
  char *ip_address[20];
  int port;
  struct sockaddr_in socket_connect;
  char client_buffer[32];
  sscanf(argv[1], "%s", &ip_address);
  sscanf(argv[2], "%d", &port);
  int sc_length = sizeof(socket_connect);
  FILE* fp;
  int nbytes;
 
  //represents an address family, where most internet apps use AF_INET
  socket_connect.sin_family = AF_INET;
  //a 16-bit port number in Network Byte order
  socket_connect.sin_port = htons(port);
  //convert IPv4 and IPv6 to binary
  socket_connect.sin_addr.s_addr = inet_addr(ip_address);

  //creating socket for client
  int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
  socket_descriptor(client_socket);
  //connecting to server
  if (connect(client_socket, (struct sockaddr *)&socket_connect, sizeof(socket_connect)) < 0)
  {
    printf("\nConnection Failed \n");
    return -1;
  }else {
    printf("\nConnection established\n");
  }
  char dir[] = "dir";
  send(client_socket , "Connected!" , strlen("Connected!") , 0 );
  printf("IP: %s Port: %d\n", ip_address, port);
  while(1){
    printf("\nPlease enter file name:\n");
   
    scanf("%s",client_buffer); //user enters value
    
    fp = fopen(client_buffer, "r");
    //User sends a file that is in the directory
    if(fp == NULL && strcmp(client_buffer, dir) != 0)
    {
      printf("\nFile not found\n");
    }
    //User chooses dir
    else if(fp == NULL && strcmp(client_buffer, dir) == 0){
      //  sendto(client_socket, client_buffer, 32, 
      //             sendrecvflag, (struct sockaddr*)&socket_connect, sizeof(socket_connect));
       write(client_socket, client_buffer,sizeof(client_buffer));
       bzero(client_buffer,sizeof(client_buffer));
       recvfrom(client_socket, client_buffer, 32, 0, (struct sockaddr*)&socket_connect, &sc_length);
       printf("\ndirectories:\n");
       printf("%s", client_buffer);
       bzero(client_buffer,sizeof(client_buffer));
    }
    //File is not in directory
    else
    {
      sendto(client_socket, client_buffer, 32, 
                  sendrecvflag, (struct sockaddr*)&socket_connect, sizeof(socket_connect));
      printf("\nFile successfully sent\n");
    }
    
    printf("\n-------------\n");

    //while(1){}
  }
}

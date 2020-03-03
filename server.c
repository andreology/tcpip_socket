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

//helper functions
//From FileTransfer Examplse
void clearBuf(char* b) 
{ 
	int i; 
	for (i = 0; i < 32; i++) 
		b[i] = '\0'; 
}


void verify_sock_descriptor(int socket) {
  //verifying descripto
  if (socket < 0)
		printf("\nERROR: Socket not established\n");
	else
		printf("\nSocket established w/descriptor %d \n", socket);
}

int main(int argc, char *argv[]) {
  int port;
  int new_socket;
  int socket_descr;
  int incoming_socket;
  char server_buffer[32];
  char dir[] = "dir";
  FILE* fp;
  struct dirent *de;  // Pointer for directory entry 

  sscanf(argv[1], "%d", &port);
  //creating socket structuresockaddr_in
  struct sockaddr_in socket_connect;
  int sc_length = sizeof(socket_connect);
  socket_connect.sin_family = AF_INET;
  //a 16-bit port number in Network Byte order
  socket_connect.sin_port = htons(port);
  //a 32-bit IP address in Netowrk Byte Order, INADDR_ANY for localhost
  socket_connect.sin_addr.s_addr = INADDR_ANY;
  //printf("stuff in args %d\n", port);
  //creating socket
  int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
  verify_sock_descriptor(server_socket);

  //binding to socket at address and port number specified
  if (bind(server_socket, (struct sockaddr*)&socket_connect, sizeof(socket_connect)) == 0)
    printf("\nSuccessfully binded!\n");
  else
    printf("\nBinding Failed!\n");
  read(server_socket, server_buffer, sizeof(server_buffer));
  printf("\n%s\n", server_buffer);
  clearBuf(server_buffer);
  printf("\n%s\n", server_buffer);
  
 while(1) 
 {
    bzero(server_buffer,sizeof(server_buffer));
    recvfrom(server_socket, server_buffer, 32, 0, (struct sockaddr*)&socket_connect, &sc_length);
    printf("recvfrom: %s\n",server_buffer);
    
    /*------------------Req 6------------------*/
    if(strcmp(server_buffer, dir) == 0)
    {
      // opendir() returns a pointer of DIR type.  
      DIR *dr = opendir(".");
      char temp[32];
      int first = 0;
      while ((de = readdir(dr)) != NULL) 
      {
        if(first == 0){
          strcpy(temp,de->d_name);
          strcat(temp, "\n");
          first = 1;
        }
        else
        {
          //printf("%s", de->d_name); 
          strcat(temp,de->d_name);
          strcat(temp, "\n");
        }
      }
      bzero(server_buffer,sizeof(server_buffer));      
      strcpy(server_buffer,temp);
      //printf("2. %s\n",server_buffer);
      write(server_socket,server_buffer,sizeof(server_buffer));            
    }
    /*------------------Req 6------------------*/
    /*------------------Req 5------------------*/
    else if()
    {

    }
    /*------------------Req 5------------------*/
     //printf("\nafter:\n%s", server_buffer);
     //printf("\nsize: %ld\n", sizeof(server_buffer));

   
 }
  return 0;
}
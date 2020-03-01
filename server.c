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

while(1) {
    // sendto(server_socket, server_buffer, 32, 0, (struct sockaddr*)&socket_connect,
    //   sc_length);
    recvfrom(server_socket, server_buffer, 32, 0,
            (struct sockaddr*)&socket_connect, &sc_length);
    printf("\n %s", server_buffer);
    printf("\nWaiting for file...");
}
  return 0;
}

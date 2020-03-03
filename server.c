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

typedef enum {true, false} bool;
//helper functions
//From FileTransfer Examplse
void clearBuf(char* b)
{
	int i;
	for (i = 0; i < 32; i++)
		b[i] = '\0';
}
/*------------------Req 5------------------*/
//Taken from FileTransfer Example
int recvFile(char* buf, int s)
{
	char ch;
	for (int i = 0; i < s; i++) {
		ch = buf[i];
		if (ch == EOF)
			return 1;
		else
			printf("%c", ch);
	}
	return 0;
}

int main(int argc, char *argv[])
{
  int port;
  int new_socket;
  int socket_descr;
  char server_buffer[32];
  char dir[] = "dir";
  FILE* fp;
  struct dirent *de;  // Pointer for directory entry
	//reading input for port
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

	char touch_command[] = "touch";

	//strcat(touch_command, " tsafd.txt");
	//system(touch_command);
  //binding to socket at address and port number specified
  if (bind(server_socket, (struct sockaddr*)&socket_connect, sizeof(socket_connect)) != 0)
    printf("\nBinding Failed!\n");

  read(server_socket, server_buffer, sizeof(server_buffer));
  printf("\n%s\n", server_buffer);
  clearBuf(server_buffer);
  printf("\n%s\n", server_buffer);



	 while(1)
	 {
		 printf("\nserver whil \n");
    //bzero(server_buffer,sizeof(server_buffer));
		clearBuf(server_buffer);
		read(server_socket, server_buffer, sizeof(server_buffer));
    printf("chceking %s\n", server_buffer);
		bool flag = false;
		fp = fopen(server_buffer, "r");
			if(fp == NULL)
			{
				clearBuf(server_buffer);
				printf("\nohting\n");
				char con_mess[] = "no";
				send(server_socket , con_mess , strlen(con_mess) , 0 );

				clearBuf(server_buffer);
				while(1) {
					printf("server 2nd while");
				recvfrom(server_socket, server_buffer, 32,
								0, (struct sockaddr*)&socket_connect,
								&sc_length);

 							}
			}


		printf("cleating buffer");
		 clearBuf(server_buffer);
		 // if (fp != NULL)
 			// fclose(fp);
 		}
  return 0;
}
// /*------------------Req 6------------------*/
// if(strcmp(server_buffer, dir) == 0)
// {
//   DIR *dr = opendir(".");
//   char temp[32];
//   int first = 0;
//   while ((de = readdir(dr)) != NULL)
//   {
//     if(first == 0){
//       strcpy(temp,de->d_name);
//       strcat(temp, "\n");
//       first = 1;
//     }
//     else
//     {
//       strcat(temp,de->d_name);
//       strcat(temp, "\n");
//     }
//   }
//   bzero(server_buffer,sizeof(server_buffer));
//   strcpy(server_buffer,temp);
//   write(server_socket,server_buffer,sizeof(server_buffer));
// }

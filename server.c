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
	int port, byte_received;
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

	char touch_command[20] = "echo ";
	char file_name[10];

  //binding to socket at address and port number specified
  	if (bind(server_socket, (struct sockaddr*)&socket_connect, sizeof(socket_connect)) != 0)
    {
		printf("\nBinding Failed!\n");
	}
	while(1)
	{
		// printf("\n%s\n", server_buffer);
	   	clearBuf(server_buffer);
		byte_received = recvfrom(server_socket, server_buffer, 32, 0,
						(struct sockaddr*)&socket_connect, &sc_length);
	  	printf("\n%s\n", server_buffer);

		clearBuf(server_buffer);
		read(server_socket, file_name, sizeof(file_name));
		// printf("\n%s\n", file_name);
		fp = fopen(file_name, "r");
		if(strcmp("dir", file_name) == 0)
        {
			DIR *dr = opendir(".");
			char dir_mess[100];
			int first = 0;
			while ((de = readdir(dr)) != NULL)
			{
				if(first == 0){
					strcpy(dir_mess,de->d_name);
					strcat(dir_mess, " ");
					first = 1;
				}
				else
				{
					strcat(dir_mess,de->d_name);
					strcat(dir_mess, " ");
				}
			}
			
			sendto(server_socket, dir_mess, 100, 0,
					(struct sockaddr*)&socket_connect, sc_length);

        }
		else if(fp == NULL)                    //File needs to be transferred to server
		{
			clearBuf(server_buffer);
			char con_mess[] = ">>> file exists!";
			//	send(server_socket , con_mess , strlen(con_mess) , 0 );
			sendto(server_socket, con_mess, 32, 0,
					(struct sockaddr*)&socket_connect, sc_length);
			// while(1) 
			// {
				clearBuf(server_buffer);
				byte_received =	recvfrom(server_socket, server_buffer, 32,
								0, (struct sockaddr*)&socket_connect,
								&sc_length);
				strcat(touch_command, "\"");
				strcat(touch_command, server_buffer);
				strcat(touch_command, "\"");
				strcat(touch_command, " >> ");
				strcat(touch_command, file_name);
				system(touch_command);
 			// }
			exit(0);
		}
		else                                  //File is already in server
		{
			clearBuf(server_buffer);
			char con_mess[] = "The file already exist!";
			sendto(server_socket, con_mess, 32, 0,
					(struct sockaddr*)&socket_connect, sc_length);
			exit(0);		
		}
		printf("clearing buffer");
		clearBuf(server_buffer);
 	}
  return 0;
}
// /*------------------Req 6------------------*/
// if(strcmp(server_buffer, dir) == 0)
// {
//   DIR *dr = opendir(".");
//   char dir_mess[32];
//   int first = 0;
//   while ((de = readdir(dr)) != NULL)
//   {
//     if(first == 0){
//       strcpy(dir_mess,de->d_name);
//       strcat(dir_mess, "\n");
//       first = 1;
//     }
//     else
//     {
//       strcat(dir_mess,de->d_name);
//       strcat(dir_mess, "\n");
//     }
//   }
//   bzero(server_buffer,sizeof(server_buffer));
//   strcpy(server_buffer,dir_mess);
//   write(server_socket,server_buffer,sizeof(server_buffer));
// }

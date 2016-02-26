#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>

#define MY_PORT		8000
#define MAXBUF		16384

int main(int Count, char *Strings[])
{   int sockfd;
	struct sockaddr_in self;
	char buffer[MAXBUF];

	/*---Create streaming socket---*/
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("Socket");
		exit(errno);
	}

	/*---Initialize address/port structure---*/
	bzero(&self, sizeof(self));
	self.sin_family = AF_INET;
	self.sin_port = htons(MY_PORT);
	//self.sin_addr.s_addr = INADDR_ANY;
	self.sin_addr.s_addr = inet_addr("10.0.0.2");

	/*---Assign a port number to the socket---*/
    if ( bind(sockfd, (struct sockaddr*)&self, sizeof(self)) != 0 )
	{
		perror("socket--bind");
		exit(errno);
	}

	/*---Make it a "listening socket"---*/
	if ( listen(sockfd, 4096) != 0 )
	{
		perror("socket--listen");
		exit(errno);
	}

	/*---Forever... ---*/
	while (1)
	{	int clientfd;
		int bytes_recvd = 0;
		struct sockaddr_in client_addr;
		int addrlen=sizeof(client_addr);

		printf("wait accept\n");
		/*---accept a connection (creating a data pipe)---*/
		clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
		printf("%s:%d connected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		/*---Echo back anything sent---*/
		//send(clientfd, buffer, recv(clientfd, buffer, MAXBUF, 0), 0);
		while (recv(clientfd, buffer, MAXBUF, 0) > 1 )
		{
		}

		/*---Close data connection---*/
		//close(clientfd);
	}

	/*---Clean up (should never get here!)---*/
	close(sockfd);
	return 0;
}

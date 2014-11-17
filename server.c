#include "headFile.h"
#include "func.c"

int main()
{
	int sockfd,client_fd;
	struct sockaddr_in my_addr;
	struct sockaddr_in remote_addr;

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);
	init_server_add_at_server(&my_addr);
	Bind(sockfd, &my_addr);
	Listen(sockfd, BACKLOG);
	printf("Listening:\n");
	while(1)
	{
		client_fd = Accept(sockfd, &remote_addr);		
		printf("Received a connection!\n");
		RecvFile_Store_Then_Send(client_fd);
		
	}
	close(client_fd);
	close(sockfd);
	return 0;
}

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <unistd.h> //for read write and close
#include <arpa/inet.h> //for inet_addr()

#define MAX_ADDR 4
#define MAX_DOMAIN 20
typedef char string[30];

typedef struct table_row
{
	string domain;
	string address[MAX_ADDR];
}entry;

int main()
{
	
	entry request;
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;
	char buff[30] = {0};

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd<0)
		perror("Socket Creation failed...!");
	else
		printf("Socket Created Successfully...!\n");

	bzero(&servaddr, sizeof(servaddr));
	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(3335);
	socklen_t len = sizeof(entry);
	while(1)
	{

		bzero(&request, sizeof(entry));
		printf("Enter the domain name: ");
		scanf(" %[^\n]", request.domain);

		if (strcmp(request.domain, "*") == 0)
			break;

		sendto(sockfd, request.domain, sizeof(request.domain), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
		recvfrom(sockfd, &request, sizeof(entry), 0, (struct sockaddr *)&servaddr, &len);

		if (!request.address[0][0])
			printf("No such entry in the DNS..!!\n");
		else
		{
			printf("The IP Address is: \n");
			for (int i = 0; i < MAX_ADDR; i++)
			{
				if (request.address[i][0])
					printf("%s\n", request.address[i]);
			}
			printf("\n");
		}
	}

	close(sockfd);
}
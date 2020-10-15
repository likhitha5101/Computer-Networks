#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <unistd.h> //for read, write and close
#include <arpa/inet.h> //for inet_addr()
#include <stdlib.h>

#include "validity.h"

int main()
{
	struct sockaddr_in seraddr, client1;
	int sockfd;
	int len, i, count, k;
	packet new_pack, received_pack;

	do{
		printf("Enter IP address : ");
		scanf("%s", new_pack.sourceIP);
	}while(is_IpInvalid(new_pack.sourceIP));

	do{
		printf("Enter MAC address : ");
		scanf("%s", new_pack.sourceMAC);
	}while(is_MACInvalid(new_pack.sourceMAC));

	//Socket Creation
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
		perror("Socket Creation failed...!");
	else
		printf("Socket Created Successfully...!\n");

	bzero(&seraddr,sizeof(seraddr));

	seraddr.sin_family=AF_INET;
	seraddr.sin_port=htons(3335);
	seraddr.sin_addr.s_addr=inet_addr("127.0.0.1");

	//Connect to a socket
	connect(sockfd,(struct sockaddr *)&seraddr,sizeof(seraddr));

	bzero(&received_pack, sizeof(received_pack));
	recv(sockfd, &received_pack, sizeof(received_pack), 0);
	printf("\nARP Request Received : \n");
	display(received_pack);


	if(strcmp(new_pack.sourceIP, received_pack.destIP) == 0){
		printf("\nIP Address matches.\n");

		packet reply_pack ;
		reply_pack.arpOperation = 2;
		strcpy(reply_pack.sourceIP, new_pack.sourceIP);
		strcpy(reply_pack.sourceMAC, new_pack.sourceMAC);
		strcpy(reply_pack.destIP, received_pack.sourceIP);
		strcpy(reply_pack.destMAC, received_pack.sourceMAC);

		send(sockfd, &reply_pack, sizeof(reply_pack), 0);

		printf("ARP Reply sent : \n");
		display(reply_pack);

		bzero(&received_pack, sizeof(received_pack));
		recv(sockfd, &received_pack, sizeof(received_pack), 0);
		printf("\nReceived data packet from: %s\n", received_pack.sourceMAC);

	}

	else{
		printf("IP address does not match.\n");
	}


	close(sockfd);
	return 0;
}
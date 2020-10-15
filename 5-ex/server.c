#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <unistd.h> //for read, write and close
#include <stdlib.h>
#include"dnsFunctions.h"

void print_table(entry table[MAX_DOMAIN]){
	printf("\t\tDomain Name\t\tAddress\n");
	printf("\t\t****************************************\n");
	for (int i = 0; i < MAX_DOMAIN; i++)
	{
		if (table[i].domain[0])
		{
			printf("\t\t%-15s \t %-15s\n", table[i].domain, table[i].address[0]);
			for (int j = 1; j < MAX_ADDR && table[i].address[j][0]; j++)
				printf("\t\t%-15s \t %-15s\n", " ", table[i].address[j]);
		}
	}
	printf("\n");
}
int main()
{

	int sockfd,n;
	struct sockaddr_in seraddr, cli;
	char buff[100],op;
	socklen_t len;

	entry table[MAX_DOMAIN],*result;
	bzero(table,MAX_DOMAIN * sizeof(entry));

	//Socket Creation
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0)
		perror("Socket Creation failed...!");
	else
		printf("Socket Created Successfully...!\n");

	bzero(&seraddr,sizeof(seraddr));

	seraddr.sin_family=AF_INET;
	seraddr.sin_port=htons(3335);
	seraddr.sin_addr.s_addr=htonl(INADDR_ANY);

	//binding to the socket
	if(bind(sockfd,(struct sockaddr *) &seraddr, sizeof(seraddr))<0)
		perror("Bind failed...!");
	else
		printf("Bound Successfully...!\n");

	len=sizeof(cli);

	createEntry(table, "www.yahoo.com", "10.2.45.67");
	createEntry(table, "www.annauniv.edu", "197.34.53.122");
	createEntry(table, "www.google.com", "142.89.78.66");

	print_table(table);

	while (1)
	{
		int t=0;
		recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&cli, &len);
		result = getAddress(table, buff);
		sendto(sockfd, result, sizeof(entry), 0, (struct sockaddr *)&cli, len);


		printf("Do you want to modify the table (y/n) ?");
		scanf(" %c",&op);

		if(op=='y'){
			string domain, address;
			printf("Enter the domain name: ");
			scanf(" %s",domain);
			do{ 
				printf("Enter the IP address: ");
				scanf(" %s",address);


				createEntry(table,domain,address);
			}while(t==2);

			print_table(table);
		}
	}

	close(sockfd);
	return 0;
}

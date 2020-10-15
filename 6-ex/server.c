#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h> //for read, write and close
#include <stdlib.h>
#include "validity.h"

int main()
{

	int sockfd,newfd=0,i, n,fd, max, client_sockets[10]={0}, flag =0;
	struct sockaddr_in seraddr, client1;
	fd_set readfds;

	packet new_pack = createARP(), recvd_packet;

	printf("\n---Developing ARP Request packet---\n");
	display(new_pack);

	printf("\nThe ARP Request packet is broadcasted.\n");
	printf("Waiting for ARP Reply...\n");

	//Socket Creation
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
		perror("Socket Creation failed...!");

	bzero(&seraddr,sizeof(seraddr));

	seraddr.sin_family=AF_INET;
	seraddr.sin_port=htons(3335);
	seraddr.sin_addr.s_addr=htonl(INADDR_ANY);

	//binding to the socket
	if(bind(sockfd,(struct sockaddr *) &seraddr, sizeof(seraddr))<0)
		perror("Bind failed...!");

	//listen for connections
	listen(sockfd,10);

	socklen_t len=sizeof(client1);

	int activity;

	while(1){

		FD_ZERO(&readfds);			
		FD_SET(sockfd, &readfds);

		max = sockfd;

		for(i = 0; i < 10; i++){
			fd = client_sockets[i];
			
			if(fd > 0){
				FD_SET(fd, &readfds);
			}

			if(fd > max){		//Store the maximum FD value.
				max = fd;
			}
		}
		activity=select(max+1,&readfds,NULL,NULL,NULL);
		if(activity<0){
			perror("Error occurred in select()\n ");
		}

		if(FD_ISSET(sockfd, &readfds)){
			newfd = accept(sockfd, (struct sockaddr*)&client1, &len);
			if(newfd < 0){
				perror("Unable to accept the new connection.\n");
			}

			// printf("The ARP Request packet is broadcasted.\n");
			send(newfd, &new_pack, sizeof(new_pack), 0);
			// printf("Waiting for ARP Reply...\n");

			//Add the new client on an empty slot.
			for( i = 0; i < 10; i++){
				if(client_sockets[i] == 0){
					client_sockets[i] = newfd;
					break;
				}
			}
		}
		
		for(i = 0; i < 10; i++)
		{
			fd = client_sockets[i];
			bzero(&recvd_packet, sizeof(recvd_packet));

			if(FD_ISSET(fd, &readfds))
			{
				recv(fd, &recvd_packet, sizeof(recvd_packet), 0);

				//Check ARP response
				if(recvd_packet.arpOperation == 2)
				{
					printf("\nARP Reply received: \n");
					display(recvd_packet);

					printf("\nSending the packet to: %s\n", recvd_packet.sourceMAC);

					send(newfd, &new_pack, sizeof(new_pack), 0);
					flag = 1;
					break;
				}
			}
		}
		if(flag == 1)
			break;
	}
	close(sockfd);
	return 0;
}


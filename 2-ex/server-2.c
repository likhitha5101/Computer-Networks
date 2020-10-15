#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <unistd.h> //for read, write and close

int main(){

	int sockfd,newfd,n;
	struct sockaddr_in seraddr, client1;
	char buff[100];
	char str[20][100];

	//Socket Creation
	sockfd=socket(AF_INET,SOCK_STREAM,0);
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
		printf("Binded Successfully...!\n");

	//listen for connections
	listen(sockfd,3);

	socklen_t len=sizeof(client1);
	//Accept connection from client
	newfd=accept(sockfd,(struct sockaddr*)&client1,&len);

	//Read client message
	int i=0;
	printf("Message from client : ");
	while(1){
		n=read(newfd,buff,sizeof(buff));
		if(strcmp(buff,"*")==0)
			break;
		printf("%s\n",buff);
		strcpy(str[i],buff);
		i++;
	}
	printf("Message echoed to client :\n");
	for(int j=0;j<i;++j){
		n=write(newfd,str[j],sizeof(str[j]));
		printf("%s\n",str[j]);
	}
	

	close(sockfd);
	close(newfd);



	return 0;
}
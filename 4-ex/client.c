#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<fcntl.h>
#include <unistd.h> //for read write and close
#include <arpa/inet.h> //for inet_addr()

int main(){

	int sockfd,n,fd;
	struct sockaddr_in seraddr;
	char buff[1024],fname[100];

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

	//Send message
	printf("Enter the path of file : ");
	scanf("%s",fname);

	n=write(sockfd,fname,sizeof(fname));
	n=read(sockfd,buff,sizeof(buff));

	if(strcmp(buff,"File does not exist..!!")==0){
		printf("%s\n",buff);
	}
	else{

		printf("Enter path to save file: ");
		scanf("%s",fname);
	
		//writing the buffer msg into the file
		fd=open(fname,O_CREAT | O_WRONLY | S_IRWXU);
		n=write(fd,buff,sizeof(buff));
	
		printf("File transferred..!!!\n");
	}
	
	close(sockfd);

	return 0;
}
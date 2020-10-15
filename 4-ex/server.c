#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<fcntl.h>
#include <unistd.h> //for read, write and close

int main(){

    int sockfd,newfd,n,fd;
    struct sockaddr_in seraddr, client1;
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
    seraddr.sin_addr.s_addr=htonl(INADDR_ANY);

    //binding to the socket
    if(bind(sockfd,(struct sockaddr *) &seraddr, sizeof(seraddr))<0)
        perror("Bind failed...!");
    else
        printf("Bound Successfully...!\n");

    //listen for connections
    listen(sockfd,3);

    socklen_t len=sizeof(client1);
    //Accept connection from client
    newfd=accept(sockfd,(struct sockaddr*)&client1,&len);

    //Read client message
    n=read(newfd,fname,sizeof(fname));

    printf("File to be transferred is : %s\n",fname);
    fd=open(fname,O_RDONLY);

    if(fd<0){
        strcpy(buff,"File does not exist..!!");
        printf("File does not exist..!!\n");
    }
    else{
        bzero(buff,sizeof(buff));
        read(fd,buff,1024);
        close(fd);
        printf("File transferred..!!!\n");
    }
    
    n=write(newfd,buff,sizeof(buff));
    close(newfd);
    close(sockfd);

    return 0;
}
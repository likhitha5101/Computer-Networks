#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <unistd.h> //for read, write and close
#include <stdlib.h>

#define MAX 4

int fd_limit(int arr[MAX], int sockfd, fd_set *readfds)
{

    int max = -1;
    for (int i = 0; i < MAX; ++i)
    {
        if(arr[i] > max)
            max=arr[i];

        if (arr[i] > 0)
            FD_SET(arr[i], readfds);
    }

    if(sockfd > max)
        max=sockfd;

return max + 1;
}

int main()
{

    int sockfd,newfd[MAX]={0};
    struct sockaddr_in seraddr, cli;
    socklen_t n;
    char buff[100];

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    fd_set readfds;

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
    if(listen(sockfd,MAX)>=0)
        printf("Waiting for client ... \n");
    else
        perror("listen failed ... \n");

    n=sizeof(cli);
    bzero(newfd, sizeof(int ) * MAX );

    while(1)
    {

        FD_ZERO(&readfds);
        FD_SET(sockfd,&readfds); 

        int activity=select(sockfd+1,&readfds,NULL,NULL,&tv);

        if(activity<0){
            perror("Error occurred in select()\n ");
            exit(0);
        }

    	//for accepting new clients
        if (FD_ISSET(sockfd, &readfds))
        {
            int client = accept(sockfd, (struct sockaddr *)&cli, &n);
            if (client < 0)
                perror("Accept connection failed..!!\n ");

            for (int i = 0; i < MAX; i++)
                if (newfd[i] == 0)
                {
                    newfd[i] = client;
                    break;
                }
            FD_CLR(sockfd, &readfds);
        }

        int limit = fd_limit(newfd, sockfd, &readfds);

        //new message from existing clients
        activity = select(limit, &readfds, NULL, NULL, &tv);

        if (activity < 0){
            perror("Error occurred in select()\n ");
            exit(0);
        }


        for (int i = 0; i < MAX; i++)
        {
            if (newfd[i] < 0)
                continue;

            // Message from client 
            if (FD_ISSET(newfd[i], &readfds))
            {
                int count = read(newfd[i], buff, 100);
                
                // Client has terminated
                if (strcmp(buff, "*") == 0)
                {
                    close(newfd[i]);
                    newfd[i] = 0;
                    printf("Client %d disconnected!\n", i+1);
                }
                else
                {
                    printf("Client %d: %s \n", (i+1), buff);
                    bzero(buff, 100);
                    printf("Server: ");
                    scanf("%[^\n]", buff);
                    getchar();
                    
                    // Write response to client
                    write(newfd[i], buff, 100);
                }
            }
        }
    }

    close(sockfd);
    return 0;
    }
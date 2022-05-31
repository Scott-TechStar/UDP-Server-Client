#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
int main()
{
    int cont,create_socket,new_socket,addrlen,fd;
    int bufsize = 1024; 
    int nameLen=0;
    int client_address_size=0;
    char *buffer = malloc(10);
    char fname[256];
    struct sockaddr_in address,client;
      //creating socket
    if ((create_socket = socket(AF_INET,SOCK_DGRAM,0)) > 0)
    printf("The socket was created\n");

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;

    address.sin_port = htons(15000);
   // binding the socket
    if (bind(create_socket,(struct sockaddr *)&address,sizeof(address))== 0)
        printf("Binding Socket\n");

    nameLen=sizeof(address);

    if (getsockname(create_socket,(struct sockaddr *)&address,&nameLen)<0)
    {
        printf("\n\ngetsockname() error\n");
        exit(3);
    }
    
    printf("Port assigned is %d\n", ntohs(address.sin_port));

    client_address_size=sizeof(client);



    if(recvfrom(create_socket,fname, 255,0,(struct sockaddr *) &client,&client_address_size)<0)
    {
        printf("\n\nrecvfrom() failed\n");
        exit(4);
    }
    //request from client
    printf("A request for filename %s Received..\n", fname);

    if ((fd=open(fname, O_RDONLY))<0)
    {
        perror("File Open Failed");
        exit(0);
    }

    while((cont=read(fd, buffer, 10))>0) 
    {
        sleep(1);
        sendto(create_socket,buffer,cont,0,(struct sockaddr *) &client,client_address_size);
        printf("\n\nPacket sent\n");
    }

    sendto(create_socket,"*",1,0,(struct sockaddr *) &client,client_address_size);

    printf("Request Completed\n");
    return close(create_socket);
}
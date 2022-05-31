#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

int main()
{
    int create_socket,cont,res;
    char *arg="127.0.0.1"; // change this IP to your machines IP when running on a separate machine
    int bufsize = 1024;
    int server_address_size=0;
    char *buffer = malloc(10);
    char fname[256];
    struct sockaddr_in address;
    int serv_addr_size = sizeof(address);
    //creating socket
    if ((create_socket = socket(AF_INET,SOCK_DGRAM,0)) > 0)
        printf("The Socket was created\n");

    address.sin_family = AF_INET;
    address.sin_port = htons(15000);
    address.sin_addr.s_addr=inet_addr(arg);

    if (connect(create_socket,(struct sockaddr *) &address,sizeof(address)) == 0)
        printf("The connection was accepted with the server %s...\n",arg);

    printf("Enter The Filename to Request : ");
    scanf("%s",fname);
    res=sendto(create_socket, fname, sizeof(fname), 0,(struct sockaddr *) &address,sizeof(address));
    if(res<0)
    {
        printf("\n\nSendto falied...\n");
        exit(0);
    }
    printf("Request Accepted... Receiving File...\n\n");

    server_address_size=sizeof(address);

    printf("The contents of file are...\n\n");

    while((cont=recvfrom(create_socket, buffer, 10, 0,(struct sockaddr *) &address,&serv_addr_size))>0) 
    {
        if(buffer[cont-1]=='*')
           break;
        write(1, buffer, cont);

    }

    printf("\nEOF\n");
    return close(create_socket);
}
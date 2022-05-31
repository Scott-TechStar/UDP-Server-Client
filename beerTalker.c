/*
   File: beerTalker.c
   Course: CENG320
   Author: Leon King
   Date: Friday Sep 18, 2020   10:44 AM
*/


#include <stdio.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in addr;
    struct addrinfo hints, *servinfo,*p; 
    int returnValue; 

    if(argc!=3) {
          fprintf(stdout,"Usage: targetMachine targetPort\n"); 
          exit(1); 
           } 

    char *targetMachine=argv[1]; 
    char *targetPort=argv[2]; 

    int numbytes;
    //
    //Create a socket to listen to connections
    if ( (sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
	perror("socket error");
	exit(-1);
        } 

    //Create a UDP socket description
    bzero(&hints,sizeof(addr));
    hints.ai_family=AF_UNSPEC; 
    hints.ai_socktype=SOCK_DGRAM;
    hints.ai_flags=AI_PASSIVE;

    if((returnValue=getaddrinfo(targetMachine,targetPort,&hints,&servinfo))!=0) {
         fprintf(stderr,"getaddrinfo: %s\n",gai_strerror(returnValue)); 
         exit(1);
       } 
     
    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("beerTalker: socket failure");
            continue;
        }

        break;
    }



    char buffer[100];
    for(int i=10000;i>0;i--) 
    {   sprintf(buffer,"%d bottles of beer on the wall\n",i);  
	if ((numbytes = sendto(sockfd, buffer,strlen(buffer), 0, p->ai_addr, p->ai_addrlen))==-1)  {
	    perror("beerTalker: sendto");
	    exit(1);
	}
     fprintf(stdout, "beerTalker: sent %d bytes: %s\n", numbytes,buffer);
     usleep(500000); 
     } 

    close(sockfd);

    return 0;
}
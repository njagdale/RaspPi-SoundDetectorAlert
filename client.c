#include<stdio.h> 
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int sock;
struct sockaddr_in server;

int Init(){

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
        return -1; 
    }
    puts("Socket created");
    
    /* FIXME: Remove hardcoded address, get as args */ 
    server.sin_addr.s_addr = inet_addr("192.168.0.12");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return -1;
    }
     
    puts("Connected\n");
    return 0;
}

void DeInit(){
    close(sock);
}

int Send(char *message){

    //Send some data
    if( send(sock , message , strlen(message) , 0) < 0)
    {
        puts("Send failed");
        return -1;
    }
    return 0;
} 

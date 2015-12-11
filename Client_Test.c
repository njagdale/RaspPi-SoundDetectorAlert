/*
 * Author:      Nikhil Jagdale
 * Description: This program implements a test harness for client.h/c 
 * 
 * Copyright (c) 2015 Nikhil Jagdale
 */ 

#include "client.h"
#include "stdio.h"

int main(int argc , char *argv[])
{
    char message[1000];
    
    if(Init() == -1) 
        return -1; 
    
    //keep communicating with server
    while(1)
    {   
        printf("Enter message : ");
        scanf("%s" , message);

        if(Send(message) == -1) 
            return -1;  
        
        fflush(stdout);
    }   
    
    return 0;
}

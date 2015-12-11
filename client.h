/*
 * Author:      Nikhil Jagdale
 * Description: APIs for TCP communication to server. 
 * 
 * Copyright (c) 2015 Nikhil Jagdale
*/

#ifdef __CLIENT_H__
#define __CLIENT_H__

/* 
Init(): Initalizes Network Connection to a server  
Return Value: 0 on success, -1 on failure
*/
int Init(void);

/* 
DeInit: Closes sockets and disconnects from server 
Return Value: None
*/
void DeInit(void);

/* 
Send: Sends a message passed in a buffer 
Return: 0 on success, -1 on failure
*/
int Send(char*);

#endif

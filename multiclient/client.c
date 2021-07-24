#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<semaphore.h>


void *Comm(void *sockID){
    int clientSocket = *((int *) sockID);
    while(1){
        char data[1024];
        data[0] = '\0';
        int r = read(clientSocket,data,1024);
        
        if(strncmp("quit",data,4)==0)
            break;
        
        printf("Anonymous Client :: %s",data);
        data[0] = '\0';
    }
}

int main(){
    int clientsocket = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9800);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(connect(clientsocket, (struct sockaddr *) &serverAddr,sizeof(serverAddr)) < 0)    
        return -1;

    printf("connection established...\n");

    pthread_t thread;
    pthread_create(&thread,NULL,Comm,(void *) &clientsocket);
    while(1){
        char buffer[1024];
        buffer[0]='\0';
        fgets(buffer,1024,stdin);
        printf("You :: %s",buffer);
        
        //printf("debug ::  %s :: ",buffer);    to debug 
        int n = write(clientsocket,buffer,1024);
        if(n<0)
            return -1;

        if(strncmp("quit",buffer,4)==0)
            break;
        
        
        buffer[0]='\0';
    }
    pthread_join(thread,NULL);
    close(clientsocket);

    return 0;

}
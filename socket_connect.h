#ifdef SOCKET_CONNECT_H_
#define SOCKET_CONNECT_H_

#include<sys/socket.h>
#include<sys/types.h>
#include<fcntl.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<sys/time.h>
#include<string.h>
#include<unistd.h>
#include<iostream>
#include<stdlib.h>//主要是exit()函数

class socket_connect{
public:
     socket_connect();
     int connect_server(const char* host,const int server_port);
     ~socket_connect();
private:
     int sock_fd;
     unsigned long in_addr;
     struct sockaddr_in server_addr;   
     struct hostent *hp;
};
#endif



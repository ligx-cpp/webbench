#pragma once

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <netinet/in.h>
#include <ifaddrs.h>


class NetAddr
{
public:
     NetAddr();
     int conserver(const char* host,const int server_port);
     ~NetAddr();
private:
     int sock_fd;
     unsigned long i_addr;
     struct sockaddr_in server_addr;   
     struct hostent *hp;
        
};


#ifndef NETWORK_H_
#define NETWORK_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <memory.h>
using namespace std;

class NetAddr
{
public:
     NetAddr();
     int conserver(string host,int server_port);
     ~NetAddr();
public:
     int sock_fd;   
};
#endif

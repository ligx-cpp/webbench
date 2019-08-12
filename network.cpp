#include <string>
#include <vector>
#include <cstring>
#include <cassert>
#include <iostream>

#include <network.h>


NetAddr::NetAddr():sock_fd(0),i_addr(0)
{
    memset(&server_addr,0,sizeof(server_addr));
}
NetAddr::~NetAddr()
{

}

int NetAddr::conserver(const char* host,const int server_port)
{
    sock_fd=socket(AF_INET,SOCK_STREAM,0);
	if(sock_fd<0){
	    std::cout<<"套接字分配失败"<<std::endl;
	    return -1;
	}

	server_addr.sin_family=AF_INET;
        server_addr.sin_port=htons(server_port);
	
	i_addr=inet_addr(host);//点分十进制IP转化为二进制IP

	if(i_addr!=INADDR_NONE){//inet_addr()转化失败时返回INADDR_NONE
             memcpy(&server_addr.sin_addr, &i_addr, sizeof(i_addr));
	}else{//说明host是域名形式
             hp=gethostbyname(host);//通过域名得到主机信息
	     if(hp==NULL){
		  return -1;
	     }
             memcpy(&server_addr.sin_addr, hp->h_addr, hp->h_length);
	}
	if(connect(sock_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)                   
             return -1;
        return sock_fd;
}


#include "network.h"
#include <cstring>

NetAddr::NetAddr():sock_fd(0)
{
}
NetAddr::~NetAddr()
{

}
int NetAddr::conserver(string host,int server_port)
{
        const char* host_t=host.c_str();
	struct sockaddr_in servaddr;
        struct hostent *hp;
	/*(1) 创建套接字*/
        sock_fd = socket(AF_INET , SOCK_STREAM , 0);
	if(sock_fd == -1)
	{
		cout<<"创建套接字失败！"<<endl;
		return -1;
	}
	/*(2) 设置链接服务器地址结构*/
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;

        unsigned long i_addr=inet_addr(host_t);//点分十进制IP转化为二进制IP
	if(i_addr!=INADDR_NONE){//inet_addr()转化失败时返回INADDR_NONE
              memcpy(&servaddr.sin_addr, &i_addr, sizeof(i_addr));
	}else{//说明host是域名形式
             hp=gethostbyname(host_t);//通过域名得到主机信息
	     if(hp==NULL){
                  cout<<"通过域名得不到主机信息！"<<endl;
		  return -1;
	     }
             memcpy(&servaddr.sin_addr, hp->h_addr,hp->h_length);
	}
        servaddr.sin_port =htons(server_port);
	/*(3) 发送链接服务器请求*/
	if(connect(sock_fd,(struct sockaddr *)&servaddr , sizeof(servaddr)) < 0)
	{
		cout<<"连接失败！"<<endl;
                return -1;
	}
        return sock_fd;
}


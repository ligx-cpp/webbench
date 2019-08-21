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


        //设置套接为非阻塞模式
        fd_set fdr, fdw;//设置读集合和写集合
        struct timeval timeout;
        int err = 0;
        socklen_t errlen = sizeof(err);
        int flags = fcntl(sock_fd, F_GETFL, 0);
        if (flags < 0) {
            fprintf(stderr, "Get flags error:%s\n", strerror(errno));
            close(sock_fd);
            return -1;
        }
        flags |= O_NONBLOCK;
        if (fcntl(sock_fd, F_SETFL, flags) < 0) {
            fprintf(stderr, "Set flags error:%s\n", strerror(errno));
            close(sock_fd);
            return -1;
        }
        /*阻塞情况下linux系统默认超时时间为75s,现在修改为10s*/
        int rc = connect(sock_fd, (struct sockaddr*)&servaddr, sizeof(servaddr));
        if (rc != 0){//如果返回的结果如果是0,这说明正确连接
            if (errno == EINPROGRESS){//当我们以非阻塞的方式来进行连接的时候，返回的结果如果是-1,这并不代表这次连接发生了错误，如果它的返回结果是EINPROGRESS，那么就代表连接还在进行中,后面可以通过poll或者select来判断socket是否可写,如果可以写，说明连接完成了。
                /*正在处理连接*/
                FD_ZERO(&fdr);//清空读集合
                FD_ZERO(&fdw);//清空写集合
                FD_SET(sock_fd, &fdr);//将套接字添加到读集合中
                FD_SET(sock_fd, &fdw);//将套接字添加到写集合中
                timeout.tv_sec = 10;//设置等待时长10
                timeout.tv_usec = 0;
              rc = select(sock_fd+1, &fdr, &fdw, NULL, &timeout);//最大的文件描述符再加一
              
              if (rc < 0) {//select返回值小于0时是错误/*在connect的背景下理解为select调用失败*/
                  fprintf(stderr, "connect error:%s\n", strerror(errno));
                  close(sock_fd);
                  return -1;
              }
              if (rc == 0) {//select返回0时是没有满足条件的文件描述符/*在connect的背景下理解为连接超时*/
                  fprintf(stderr, "Connect timeout.\n");
                  close(sock_fd);
                  return -1;
              }
              
              if (rc == 1 && FD_ISSET(sock_fd, &fdw)) {/*[1] 当连接成功建立时，描述符变成可写,rc=1*/
                  return sock_fd;
              }
              /*[2] 当连接建立遇到错误时，描述符变为即可读，也可写，rc=2 遇到这种情况，可调用getsockopt函数*/
              if (rc == 2) {
                 if (getsockopt(sock_fd, SOL_SOCKET, SO_ERROR, &err, &errlen) == -1) {//getsockopt执行失败则返回-1(SO_ERROR　　　表示：获得套接字错误)
                      fprintf(stderr, "getsockopt(SO_ERROR): %s", strerror(errno));
                      close(sock_fd);
                      return -1;
                  }//if(getsockopt())
  
                  if (err) {
                      errno = err;
                      fprintf(stderr, "connect error:%s\n", strerror(errno));
                      close(sock_fd);
                      return -1;
                 }//if(err)
             }//if(rc==2)
 
         }//if(error==EINPROGRESS)
         fprintf(stderr, "connect failed, error:%s.\n", strerror(errno));//rc返回值不是0但也不是EINPROGRESS也不是我们需要的
         return -1;
     }//if(rc!=0)
     
}


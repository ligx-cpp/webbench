#include"bench.h"

int successed_sum=0;//成功连接的次数
int failed_sum=0;//失败请求的次数
int bytes_sum=0;//服务器返回的字节数
int send_failed_sum=0;
int read_failed_sum=0;
int close_failed_sum=0;
int connect_failed_sum=0;//定义好全局变量

bench::bench(int clients,int benchtime):clients(clients),benchtime(benchtime){
      memset(&sa,0,sizeof(sa));
      re_msg["bytes"]=0;//读取的总字节数
      re_msg["failed"]=0;//失败的总子进程数量
      re_msg["successed"]=0;//成功的总子进程数量
      re_msg["connect_failed"]=0;//连接失败的子进程数量
      re_msg["send_failed"]=0;//发送失败的子进程数量
      re_msg["read_failed"]=0;//读取失败的子进程数量
      re_msg["close_failed"]=0;//关闭失败的子进程数量

}
bench::~bench(){
	delete sc;
}
int bench::bench_ready(const char*host,const int port){
      pid_t p_id=0;
      FILE* f;
      //先进行一次试探性连接
      int sock=sc->connect_server(host,port);
      if(sock<0){
	  std::cout<<"试探性连接服务器失败！"<<std::endl;
	  return -1;
      }
      close(sock);//若试探性连接成功，则先关闭连接

      if(pipe(mypipe)){//创建管道
	  std::cout<<"创建管道失败！"<<endl;
	  return -1;
      }

      for(int i=0;i<clients;++i){
	  p_id=fork();//循环创建子进程
	  if(p_id==0||p_id==-1){
              sleep(1);
	      break;//在每次循环时，假设发现是子进程那么就直接从创建的子进程的循环中跳出来。不让你进入循环，这样做就保证了每次只有父进程来做创建子进程的工作
	  }
      }

      if(p_id==-1){//相当于是返回了错误的文件描述符
	  std::cout<<"子进程创建失败！"std::endl;
	  return -1;
      }

      if(p_id==0){//子进程向管道写数据:mypipe[1]表示写管道
	  //由子进程发送请求报文(这里我直接利用我的主机号访问并没有设置代理服务器)	  
	  re_msg=bench_core(host,port,request);

          f=fdopen(mypipe[1],"w");//fdopen函数是一个专门对管道的文件描述符进行操作的函数，此时以只写的方式打开，返回值是一个文件指针
          if(f=NULL){
		 std::cout<<"写管道打开失败"<<endl;
		 return -1;
	  }	  
	  fprintf(f,"d% d% d% d% d% d% d%",re_msg["bytes"],re_msg["failed"],re_msg["successed"],re_msg["connect_failed"],re_msg["send_failed"],re_msg["read_failed"],re_msg["close_failed"])//fprintf用于将格式化信息输出到文件流中
          close(f);  
          
          return 0;
      }else{//父进程从管道中读取数据:mypipe[0]表示读管道
	  int bytes=0;
	  int failed=0;
	  int successed=0;
	  int connect_failed;
	  int send_failed=0;
	  int read_failed=0;
	  int close_failed=0;

	  f=fdopen(mypipe[0],"r");
          if(f=NULL){
	       std::cout<<"读管道打开失败！"<<endl;
	  }
	  setvbuf(f,NULL,_IONBF,0);//因为我们不需要缓冲区，所以把缓冲区置为_IONBF


	  while(clients==0){//父进程循环读子进程的消息
	      int num=fscanf(f,"d% d% d% d% d% d% d%",&bytes,&failed,&successed,&connect_failed,&send_failed,&read_failed,&close_failed);
              if(num<7){
		  std::cout<<"子进程读取错误！"<<endl;
		  return -1;
	      }

	      successed_sum+=successed;
	      failed_sum+=failed;
	      bytes_sum+=bytes;
	      connect_failed_sum+=connect_failed;
	      send_failed_sum+=send_failed;
	      read_failed_sun+=read_failed;
	      close_failed_sum+=close_failed;

	      --clients;
	  }
          close(f);

	  //最后显示处理结果
	  std::cout<<"读取的字节总数为:"<<bytes_sum<<endl;
	  std::cout<<"成功数目为:"<<successed_sum<<endl;
	  std::cout<<"失败数目为:"<<failed_sum<<endl;
	  std::cout<<"失败连接数为:"<<connect_failed_sum<<endl;
	  std::cout<<"发送失败数目为:"<<send_failed_sum<<endl;
	  std::cout<<"读取失败数目为:"<<read_failed_sum<<endl;
	  std::cout<<"关闭连接失败数目为:"<<close_failed_sum<<endl;
      }
      return i;
}
map<string,int>& bench::benchcore(const char*host,const int port,const char *req){
          char buf[1500];
	  int rlen=strlen(req);//请求报文的长度
	  sa.sa_handler=larm_handler;//设置自定义的闹钟函数alarm_handler
	  sa.sa_flags=0;

	  if(sigaction(SIGALRM,&sa,NULL)){//如果超时就产生SIGALRM信号，然后用上面指定的函数处理
               return -1;
	  }
	  alarm(benchtime);//设置闹钟函数也就是一个定时器,参数是持续访问时间

	  while(true){//执行访问请求
              int flag =0;//仅用于设定一个标记，用于跳出双重while循环
	      if(timeout){//超时处理流程
		 if(failed>0)
	             --re_msg["failed"];
		 if(connect_failed>0)
	             --re_msg["connect_faliled"];
		 else if(send_failed>0)
	             --re_msg["send_failed"];
		 else if(read_faild>0)
		     --re_msg["read_failed"];
                 return ;
	      }
	      //与网站建立连接
             int sock=sc->connect_server(host,port);
             if(sock<0){//连接失败
		 ++re_msg["failed"];
		 ++re_msg["connect_failed"];
		 continue;
	     }

	     if(rlen!=write(sock,req,rlen)){//验证写事件是否会失败 
		 ++re_msg["failed"];
		 ++re_msg["send_failed"];
		 continue;
	     }

	     //因为主要目的是学习，所以默认使用等待服务器回复这种方式
	     while(1){
		 if(timeout)//如果超时则不再读取数据
		      break;
		 int r_num=read(sock,buf,1500);//如果套接字中数据小于要读取的字节数那么就会引起阻塞，返回-1
		 if(r_num<0){//说明读取失败，读取失败后要关闭套接字，否则会严重浪费资源
		      ++re_msg["failed"];
		      ++re_msg["read_failed"];
                      close(sock);
		      flag=1;
		      break;
		 }else{
		      if(r_num==0)
		          break;
		      else
		          bytes+=r_num;
		 }
	     }
	     if(flag==1){
		 continue;
	     }//这里用了标记的方式替换了 goto 这种用法，但实现了同样的功能

	     if(close(sock)){//套接字关闭失败 
		      ++re_msg["failed"];
		      ++re_msg["close_failed"];
		      continue;
	     }
	     
             ++re_msg["successed"];//至此。算是成功访问      
	  }
}

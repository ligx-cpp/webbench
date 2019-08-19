#include<bench.h>
#include<Thread.h>
volatile bool timout=false;

static void alarm_handler(int sig){ 
      timout=true;
}//报警函数设置为静态的

bench::bench(){

}
bench::bench(int clients,int benchtime,string request):clients(clients),benchtime(benchtime),request(request){
      //sc=new NetAddr();
      sa.sa_flags=0;
      sa.sa_handler=alarm_handler;
}
bench::~bench(){
      //delete sc;
}
int bench::bench_ready(string host,int port){
      
      //先进行一次试探性连接
      int sock=sc.conserver(host,port);
      if(sock<0){
	  std::cout<<"试探性连接服务器失败！"<<std::endl;
	  return -1;
      }
      close(sock);//若试探性连接成功，则先关闭连接
      req.cin_host(host);
      req.cin_port(port);
      req.cin_url_request(request);

      for(int i=0;i<clients;++i){
                     my_threads.push_back(thread(&wthread::write_in,&myweb,req));//第二个参数表示所有线程都是对同一对象操作,第二个参数必须是引用才能保证所有线程用的是同一对象
      }
      map<string,int>re_msg;

      for(auto iter=my_threads.begin();iter!=my_threads.end();++iter){
          iter->join();//等待线程执行完毕
      }
      cout<<"所有线程执行完毕"<<endl;
      //最后显示处理结果

      re_msg=myweb.get_map();
      std::cout<<"读取的字节总数为:"<<re_msg["bytes"]<<endl;
      std::cout<<"成功数目为:"<<re_msg["successed"]<<endl;
      std::cout<<"失败数目为:"<<re_msg["failed"]<<endl;
      return 0;
}
map<string,int> bench::bench_core(string host,const int port,string request,map<string,int>temp){
          char buf[1500];
	  const char* host_t=host.c_str();
	  const char* request_t=request.c_str();
	  int rlen=strlen(request_t);//请求报文的长度
	  sa.sa_handler=alarm_handler;//设置自定义的闹钟函数alarm_handler
	  sa.sa_flags=0;

	  if(sigaction(SIGALRM,&sa,NULL)){//如果超时就产生SIGALRM信号，然后用上面指定的函数处理
               cout<<"信号处理函数执行失败！"<<endl;
	  }
	  alarm(benchtime);//设置闹钟函数也就是一个定时器,参数是持续访问时间

	  while(true){//执行访问请求
              int flag =0;//仅用于设定一个标记，用于跳出双重while循环
	      if(timout){//超时处理流程
		 if(temp["failed"]>0)
	             --temp["failed"];
		 exit(0);
	      }
	      //与网站建立连接
             int sock=sc.conserver(host_t,port);
             if(sock<0){//连接失败
		 ++temp["failed"];
		 continue;
	     }

	     if(rlen!=write(sock,request_t,rlen)){//验证写事件是否会失败 
		 ++temp["failed"];
		 continue;
	     }

	     //因为主要目的是学习，所以默认使用等待服务器回复这种方式
	     while(1){
		 if(timout)//如果超时则不再读取数据
		      break;
		 int r_num=read(sock,buf,1500);//如果套接字中数据小于要读取的字节数那么就会引起阻塞，返回-1
		 if(r_num<0){//说明读取失败，读取失败后要关闭套接字，否则会严重浪费资源
		      ++temp["failed"];
                      close(sock);
		      flag=1;
		      break;
		 }else{
		      if(r_num==0)
		          break;
		      else
		          temp["bytes"]+=r_num;
		 }
	     }
	     if(flag==1){
		 continue;
	     }//这里用了标记的方式替换了 goto 这种用法，但实现了同样的功能

	     if(close(sock)){//套接字关闭失败 
		      ++temp["failed"];
		      continue;
	     }
	     
             ++temp["successed"];//至此。算是成功访问      
	  }
          return temp;
}

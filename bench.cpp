#include<bench.h>
#include<Thread.h>
bench::bench(){
       
}
bench::bench(int btime):benchtime(btime){

}
bench::bench(int clients,int btime,string request):clients(clients),benchtime(btime),request(request){
          
}
bench::~bench(){
     
}
int bench::bench_ready(string host,int port){
      //先进行一次试探性连接
      int sock=sc.conserver(host,port);
      if(sock<0){
           close(sock);
      }else{
           close(sock);//若试探性连接成功，则先关闭连接
      }
      req.cin_host(host);
      req.cin_port(port);
      req.cin_url_request(request);

      for(int i=0;i<clients;++i){
              my_threads.push_back(thread(&wthread::write_in,&myweb,req,benchtime));//第二个参数表示所有线程都是对同一对象操作,第二个参数必须是引用才能保证所有线程用的是同一对象
      }
      for(auto iter=my_threads.begin();iter!=my_threads.end();++iter){
          iter->join();//等待线程执行完毕
      }
      cout<<"所有线程执行完毕"<<endl;
      //最后显示处理结果
      std::cout<<"读取的字节总数为:"<<myweb.all["bytes"]<<endl;
      std::cout<<"成功数目为:"<<myweb.all["successed"]<<endl;
      std::cout<<"失败数目为:"<<myweb.all["failed"]<<endl;
      return 0;
}
map<string,long double> bench::bench_core(string host,const int port,string request,map<string,long double>temp){
          char buf[1500];
	  const char* host_t=host.c_str();
	  const char* request_t=request.c_str();
	  int rlen=strlen(request_t);//请求报文的长度
          steady_clock::time_point t1 = steady_clock::now();
	  while(true){//执行访问请求
              steady_clock::time_point t2 = steady_clock::now();
              int flag =0;//仅用于设定一个标记，用于跳出双重while循环
              duration<double> t3 = duration_cast<duration<double>>(t2-t1);
              //cout<<"当前执行时间为："<<t3.count()<<endl;
	      if(t3.count()> benchtime){//超时处理流程
		  break;//退出循环
	      }
	      //与网站建立连接
             int sock=sc.conserver(host_t,port);
             cout<<"sock为："<<sock<<endl;
             if(sock==-1){//连接失败
		 ++temp["failed"];
		 continue;
	     }

             int flags  = fcntl(sock,F_GETFL,0); //获取文件的flags值。
             fcntl(sock,F_SETFL,flags&~O_NONBLOCK); //前面非阻塞模式只是为了减少connect函数的阻塞时间;现在为了能正确读写(因为读写函数是处于阻塞模式的)所以将套接字的非阻塞模式改回来；
	     

             if(rlen!=send(sock,request_t,rlen,0)){//验证写事件是否会失败
                 cout<<"写事件失败！"<<endl;
		 ++temp["failed"];
		 continue;
	      }
	      //因为主要目的是学习，所以默认使用等待服务器回复这种方式
	      while(1){
		 if(t3.count()> benchtime)//如果超时则不再读取数据
		      break;
		 int r_num=recv(sock,buf,sizeof(buf),0);//如果套接字中数据小于要读取的字节数那么就会引起阻塞，返回-1
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
                    cout<<"套接字关闭失败！"<<endl;
		      ++temp["failed"];
		      continue;
	      }
              ++temp["successed"];//至此。算是成功访问 
	   }
           return temp;
}

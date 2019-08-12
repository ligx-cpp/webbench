#include<network.h>
#include<iostream>
#include<bench.h>
#include<request.h>
using namespace std;

int main(){
      char* url=new char[1000];
      int clients;
      int port=0;
      int benchtime=0;
      char* host=new char[1500];
      char* request=new char[2000];
      cout<<"欢迎使用网站压力测试工具!"<<endl;
      cout<<"请输入要访问的URL:";
      cin>>url;

      http_req req;
      req.build_request(url);
      request=req.get_request();

      port=req.get_port();
      host=req.get_host();

      cout<<"请输入要访问的客户端数:";
      cin>>clients;
      cout<<"请输入持续访问时间:";
      cin>>benchtime;

      bench bch(clients,benchtime,request);
      bch.bench_ready(host,port);

      std::cout<<"程序运行结束"<<endl;
      return 0; 
}


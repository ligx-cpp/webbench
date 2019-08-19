#include<network.h>
#include<iostream>
#include<bench.h>
#include<string>
#include<request.h>
using namespace std;

int main(){
      string url;
      int clients;
      int port=0;
      int benchtime=0;
      string host="";
      string request="";
      cout<<"欢迎使用网站压力测试工具!"<<endl;
      cout<<"请输入要访问的URL:";
      cin>>url;

      http_req req;
      req.build_request(url);
      request=req.get_url_request();

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


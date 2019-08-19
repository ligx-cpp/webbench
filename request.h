#ifndef REQUEST_H_
#define REQUEST_H_
#include<iostream>
#include<cstring>
#include<algorithm>
#include<string>
using namespace std;

class http_req{
public:
     http_req();
     int build_request(string url);
     int get_port(){return port;}
     string get_url_request(){return url_request;}
     string get_host(){return host;}
     void cin_host(string host_t){host=host_t;}
     void cin_port(int port_t){port=port_t;}
     void cin_url_request(string request_t){url_request=request_t;}
     ~http_req();
private:
     char tmp_port[10];
     int port;//这是端口号
     string url_request;
     string host;//这是主机号如www.baidu.com或者172.168.0.1
};
#endif

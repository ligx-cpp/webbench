#ifndef REQUEST_H_
#define REQUEST_H_
#include<iostream>
#include<cstring>
#include<algorithm>
using namespace std;
#define Hsize 1500
#define Msize 2048

class http_req{
public:
     http_req();
     int build_request(const char* temp_url);
     char* get_url_request(){return url_request;}
     int get_port(){return port;}
     char* get_host(){return host;}
     char* get_request(){return url_request;}
     ~http_req();
private:
     char* url_request;//存放http请求报文信息
     int port;
     char* host;
     char* tmp_port;
};
#endif

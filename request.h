#include<iostream>
#include<string>
#include<algorithm>
using namespace std;
using HOST_SIZE=1500;
using MAX_SIZE=2048;

class http_req{
public:
     http_req();
     void build_request(const char* temp_url);
     char* get_url_request(){return url_request;}
     int get_port(){return port;}
     char* get_host(){return host;}
     ~http_req();
private:
     char* url_request;//存放http请求报文信息
     int port;
     char* host;
     char* tmp_port;
}

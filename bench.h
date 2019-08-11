#include<iostream>
#include<unistd.h>
#include<signal.h>
#include<string>
#include<error.h>
#include<time.h>
#include<map>
#include<stdio.h>
#include"socket_connect.h"

using namespace std;
extern int bytes_sum;
extern int successed_sum;
extern int failed_sum;
extern int send_failed_sum;
extern int read_failed_sum;
extern int connect_failed_sun;
extern int close_failed_sum;

class socket_connect;
class bench{
public:
     bench(int clients,int benchtime);
     int bench_ready(const char*host,const int port);
     map<string,int>& bench_core(const char*host,const int port,const char*req);
     inline void alarm_handler(int sig){ timeout=true; return;}
     ~bench();
private:
     int clients;
     int mypipe[2];//用于父子进程间通信
     int benchtime;
     struct sigaction sa;
     map<string,int> re_msg;
     volatile bool timeout=false;
     socket_connect *sc;
};

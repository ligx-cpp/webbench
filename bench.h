#ifndef BENCH_H_
#define BENCH_H_
#include<iostream>
#include<unistd.h>
#include<signal.h>
#include<string>
#include<error.h>
#include<time.h>
#include<map>
#include<stdio.h>
#include<network.h>
#include<request.h>
#include<memory.h>

using namespace std;
extern int bytes_sum;
extern int successed_sum;
extern int failed_sum;
extern int send_failed_sum;
extern int read_failed_sum;
extern int connect_failed_sun;
extern int close_failed_sum;
extern volatile bool timout;

class bench{
public:
     bench();
     bench(int clients,int benchtime,char* request);
     int bench_ready(const char*host,const int port);
     int bench_core(const char*host,const int port,const char* request);
     
     ~bench();
private:
     
     int clients;
     int mypipe[2];//用于父子进程间通信
     int benchtime;
     char* request;
     struct sigaction sa;
     map<string,int> re_msg;
     NetAddr *sc;
     http_req *hr;
};
#endif

#ifndef BENCH_H_
#define BENCH_H_
#include<iostream>
#include<unistd.h>
#include<signal.h>
#include<string>
#include<error.h>
#include<time.h>
#include<map>
#include<vector>
#include<stdio.h>
#include<network.h>
#include<request.h>
#include<memory.h>

extern volatile bool timout;

class bench{
public:
     bench();
     bench(int clients,int benchtime,char* request);
     int bench_ready(const char*host,const int port);
     map<string,int> bench_core(const char*host,const int port,const char* request,map<string,int>temp);
     ~bench();
private:
     int clients;
     int benchtime;
     char* request;
     struct sigaction sa;
     vector<thread>my_threads;
     NetAddr *sc;
     http_req req;
     web_thread myweb;
};


#endif

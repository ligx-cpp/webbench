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
#include<thread>

extern volatile bool timout;
class bench{
public:
     bench();
     bench(int clients,int benchtime,string request);
     int bench_ready(string host,int port);
     map<string,int> bench_core(string host,const int port,string request,map<string,int>temp);
     ~bench();
private:
     int clients;
     int benchtime;
     string request;
     string host;
     struct sigaction sa;
     vector<thread>my_threads;
     NetAddr sc;
     http_req req;
     wthread myweb;
};


#endif

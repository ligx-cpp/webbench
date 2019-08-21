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
#include<stdlib.h>
#include<network.h>
#include<request.h>
#include<memory.h>
#include<Thread.h>
#include<thread>
#include <chrono>
#include <ratio>
#include <ctime>

using namespace std::chrono;
class bench{
public:
     bench();
     bench(int benchtime);
     bench(int clients,int btime,string request);
     int bench_ready(string host,int port);
     map<string,long double> bench_core(string host,const int port,string request,map<string,long double>temp);
     ~bench();
private:
     int clients;
     string request;
     int benchtime;
     vector<thread>my_threads;
     NetAddr sc;
     wthread myweb;
     http_req req; 
};


#endif

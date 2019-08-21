#ifndef THREAD_H_
#define THREAD_H_

#include<thread>
#include<mutex>
#include<request.h>
#include<map>
#include<iostream>
using std::mutex;
using namespace std;

class wthread{
public:
     void write_in(http_req req,int benchtime);//因为我只需要往队列里写数据，并不需要其他的线程在写的同时去读，所以只需要一个函数
public:
     map<string,long double>all;//用于收集信息的线程
private:
     
     mutex mu_by;//创建互斥量
     mutex mu_su;//创建互斥量
     mutex mu_fa;//创建互斥量
};
#endif

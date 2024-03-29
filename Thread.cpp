#include<Thread.h>
#include<bench.h>
void wthread::write_in(http_req req,int benchtime){
      map<string,long double>temp;
      temp["bytes"]=0;
      temp["failed"]=0;
      temp["successed"]=0;

      string host=req.get_host();
      int port=req.get_port();
      string request=req.get_url_request();
      bench bch(benchtime);
      temp=bch.bench_core(host,port,request,temp);
      lock_guard<mutex> m1(mu_by);//上锁
      all["bytes"]+=temp["bytes"];
      lock_guard<mutex> m2(mu_su);//上锁
      all["successed"]+=temp["successed"];
      lock_guard<mutex> m3(mu_fa);//上锁
      all["failed"]+=temp["failed"];
      return ;
}

#include<request.h>


http_req::http_req():port(80),host(""){
}
http_req::~http_req(){
}

int http_req::build_request(string url){
	 string temp_url(url);//直接赋值给一个string即可
         cout<<"temp_url为："<<temp_url<<endl;
	 //定义一个空字符串
	 string request="";
	 //因为我们默认使用http/1.1,get方法所以直接copy
	 request+="GET";
	 //然后按照格式在后面填充空格
	 request+=" ";

	 //随后是对url是否正确的判断
	 string::size_type pos,pos1,pos2;
	 pos=temp_url.find("://");
	 if(pos==string::npos){
		cout<<"不合法的url!"<<endl;
		return -1;
	 }

	 if(temp_url.size()>1500){//观察url是否过长
		 cout<<"url过长！"<<endl;
		 return -1;
	 }
         //url头必须是http://，先比较前7位
         if (0 != temp_url.compare(0,7,"http://")) {
                 cout << "url无法解析！"<<endl;
		 return -1;
         }
	 int cnt= temp_url.find("://",0)+3;
	 //接下来找到主机名或者Ip地址开始的地方找下一个‘/’如果找不到说明url非法，如果上面正确那么应该从下标是7的地方开始找
	 pos1=temp_url.find('/',cnt);
	 if(pos==string::npos){
		 cout<<"主机名没有结束符！"<<endl;
		 return -1;
	 }

	 //至此合法性判断结束，随后将url填写到请求行(url=http://www.baidu.com:80/)
	 pos2=temp_url.find(':',cnt);
	 if(pos2!=string::npos && pos2<pos1 ){//将要填写的url分为含有端口号类型和不含有端口号类型的
                 string tmp_host(temp_url,cnt,pos2-cnt);//先保存主机号
	         host=tmp_host;

		 string temp_port(temp_url,pos2+1,pos1-pos2-1);
		 strcpy(tmp_port,temp_port.c_str());//把端口号存入tmp_port然后转换成整形
		 port=atoi(tmp_port);
		 if(port==0)
		        port=80;
	 }else{//如果不存在端口号
		 string tmp_host(temp_url,cnt,pos1-cnt);//那就只把主机号存好就可以了
	         host=tmp_host;
	 }
	 string tmp_url(temp_url,cnt,temp_url.size()-cnt-1);
	 request+=tmp_url;//将其添加到request中

	 //由于默认使用http1.1版本
	 request+=" HTTP/1.1";//注意不要忘记前面加空格

	 //随后添加 回车换行符
	 request+="\r\n";

	 //继续添加请求报文的头部
	 request+="User-Agent:Webbench-lgx-1.0";//一行一个“名:值”对
	 //回车换行
	 request+="\r\n";
	 //http1.1需要填充Host字段
         request+="Host: ";
	 request+=host;
	 //回车换行
	 request+="\r\n";
	 //由于我们不需要传输内容给网站，所以我们不必使用长连接，所以手动设置关闭
	 request+="Connection: close\r\n";

	 //最后在末尾填入空行
	 request+="\r\n";
         
	 //将请求拷贝到字符数组 
         url_request=request;
	 return 0;
}

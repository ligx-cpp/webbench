#include"request.h"


http_req::http_req():port(80){
         url_request=new char[MAX_SIZE];
         host=new char[HOST_SIZE];
         tmp_port=new char[10];
}
http_req::~http_req(){
	delete [] url_request;
	delete [] tmp_port;
	delete [] host;
}

void http_req::build_request(const char* url){
	 string temp_url(url);//直接赋值给一个string即可

	 //定义一个空字符串
	 string request="";
	 //因为我们默认使用http/1.1,get方法所以直接copy
	 request+="GET";
	 //然后按照格式在后面填充空格
	 request+=" ";

	 //随后是对url是否正确的判断
	 string::size_type pos,pos1,pos2;
	 pos=temp_url.find("://");
	 if(pos==temp_url.npos){
		cout<<"不合法的url!"<<endl;
		return -1;
	 }

	 if(temp_url.size()>1500){//观察url是否过长
		 cout<<"url过长！"<<endl;
		 return -1;
	 }

	 //先比较前7位
	 string s(temp_url,0,7);
	 transform(s.begin(),s.end(),s.begin(),::tolower);
	 if(s=="http://"){//这么做的目的是忽略大小写进行比较
		 cout<<"url无法解析！"<<endl;
		 return -1;
	 }

	 //接下来找到主机名或者Ip地址开始的地方找下一个‘/’如果找不到说明url非法，如果上面正确那么应该从下标是7的地方开始找
	 pos1=temp_url.find('/',7);
	 if(pos==temp_url.npos){
		 cout<<"主机名没有结束符！"<<endl;
		 return -1;
	 }

	 //至此合法性判断结束，随后将url填写到请求行(url=http://www.baidu.com/)
	 pos2=temp_url.find(':',7);
	 if(pos2!=temp_url.npos && pos2<pos1 ){//将要填写的url分为含有端口号类型和不含有端口号类型的
                 string temp_host(temp_url,7,pos2-7);
		 strcpy(host,temp.c_str());//把主机号存入host

		 string temp_port(temp_url,pos2+1,pos1-pos2-1);
		 strcpy(tmp_port,temp_port.c_str());//把端口号存入tmp_port然后转换成整形
		 port=atoi(tmp_port);
		 if(port==0)
		        port=80;
	 }else{//如果不存在端口号
		 string temp_host(temp_url,7,pos1-7);
		 strcpy(host,temp.c_str());//那就只把主机号存好就可以了
	 }
	 string tmp_url(temp_url,7,temp_url.size()-7);
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
         strcpy(url_request,request.c_str());
	 return ;
}

#ifndef _HTTPSERVER_H_
#define _HTTPSERVER_H_
#include <iostream>
#include<cstring>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/wait.h>
using namespace std;


#define BUFFER_SIZE 101010
class  httpsever
{
	private:
		int sockfd;
	public:
		httpsever() {}
		httpsever(int id):sockfd(id) {}
		~httpsever() {}
		void solve()
		{
			string zone_s;
			bool status;
			char zone[BUFFER_SIZE];
			while(1)
			{
				status=true;
				char zone[BUFFER_SIZE];
				int rc=recv(sockfd,zone,BUFFER_SIZE-1,0);
				if(rc>0)
				{
					zone_s=string(zone);
					while(zone_s.find("HTTP/1.1")!=string::npos)
					{
						int request_end_pos=0;
						if((request_end_pos=zone_s.find("\r\n\r\n"))!=-1)
						{
							string request="";
							request_end_pos+=4;
							request=zone_s.substr(0,request_end_pos);

							int head_end_pos=request.find("Content-Length");
							int entity_pos=request.length();
							if(head_end_pos!=-1)
							{
								string num;
								head_end_pos+=15;
								while(request[head_end_pos]!='\r')
								{
									num+=request[head_end_pos++];
								}
								int entity_l=atoi(num.c_str());
								if((zone_s.length()-request.length())>=entity_l)
								{
									request+=zone_s.substr(request.length(),entity_l);
									request_end_pos+=entity_l;
								}
								else continue;
							}
							zone_s=zone_s.substr(request_end_pos);
							string method,url;
							request_end_pos=0;
							while(request[request_end_pos]!=' ')
							{
								method+=request[request_end_pos++];
							}
							if(method!="GET"&&method!="POST")
							{
								Bad_Request(method,url);
								continue;
							}
							++request_end_pos;
							while(request[request_end_pos]!=' ')
							{
								url+=request[request_end_pos++];
							}
							++request_end_pos;

							if(method=="GET")
							{
								get_method(method,url);
							}
							else if(method=="POST")
							{

								if(url!="/Post_show")
								{
									Bad_Request(method,url);
									continue;
								}
								string entity=request.substr(entity_pos,request.length()-entity_pos);

								//请求体按照Name=xxx&ID=xxx排列时才处理
								int namepos=entity.find("Name="),idpos=entity.find("&ID=");
								if(namepos==-1||idpos==-1||idpos<=namepos) //请求体中存在Name和ID并且按照Name、ID排列
								{
									Bad_Request(method,url);
									continue;
								}
								/* if(entity.find("=",idpos+4)!=string::npos){
								     Bad_Request(method,url);
								     continue;
								 }*/

								string name,id;
								name=entity.substr(namepos+5,idpos-namepos-5);
								id=entity.substr(idpos+4);
								post_method(name,id);
							}

						}
					}

				}
				else
				{
					if(rc<=0) //检测到客户端tcp连接关闭
					{
						break;
					}
				}
			}
			sleep(1);
			close(sockfd);
		}
		void Bad_Request(string method,string url)
		{
			string en;
			if(method!="GET"&&method!="POST")
			{
				en="<htmL><title>501 Not Implemented</title>c<body bgcolor=ffffff>\n Not Implemented\n<p>Does not implement this method: "+method+"\n,<r><em>HTTP Web Server </em>\n</body></html>\n";
				string tmp="Http/1.1 501 Not Implemented\r\nContent-Type: text/html\r\nContent-Length:"+to_string(en.length())+"\r\n\r\n";
				string finalentity=tmp+en;
				char outstring[1024];
				sprintf(outstring,"%s",finalentity.c_str());
				write(sockfd,outstring,strlen(outstring));
			}
			else
			{
				if(method=="GET")
				{
					en="<html><title>404 Not Found</title><body bgcolor=ffffff>\nNot Found\n<p>Could not find this file: "+url+"</p>\n<hr><em>HTTP Web Sever</em>\n</body></html>\n";
				}
				if(method=="POST")
				{
					en="<html><title>404 Not Found</title><body bgcolor=ffffff>\nNot Found\n<hr><em>HTTP Web Sever</em>\n</body></html>\n";
				}
				string tmp="Http/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length:"+to_string(en.length())+"\r\n\r\n";
				string finalentity=tmp+en;
				char outstring[1024];
				sprintf(outstring,"%s",finalentity.c_str());
				write(sockfd,outstring,strlen(outstring));
			}
		}
		void get_method(string method,string url)
		{
			int len=url.length();
			string tmp="./src";
			if(url.find(".")==string::npos)
			{
				if(url[len-1]=='/'||url.length()==0)
				{
					tmp+=url+"index.html";
				}
				else tmp+=url+"/index.html";
			}
			else tmp+=url;
			//cout<<tmp<<endl;
			int fd=open(tmp.c_str(),O_RDONLY);//若所有欲核查的权限都通过了检查则返回0值，表示成功，只要有一个权限被禁止则返回-1。
			if(fd>=0)
			{
				struct stat stat_buf;
				fstat(fd,&stat_buf);//通过文件名filename获取文件信息，并保存在buf所指的结构体stat中
				char outstring[1024];
				sprintf(outstring,"Http/1.1 200 OK\r\nContent-Length:%d\r\nContent-Type: text/html\r\n\r\n",stat_buf.st_size);//输出到命令行
				write(sockfd,outstring,strlen(outstring));//sockfd是描述符,类似于open函数
				sendfile(sockfd,fd,0,stat_buf.st_size);
			}
			else //Do not exist the path return 404
			{
				Bad_Request(method,url);
			}
		}
		void post_method(string name,string id)
		{
			string en="<html><title>POST Method</title><body bgcolor=ffffff>\n";
			string en1="Your name: "+name+"\nYour id: "+id+"\n"+"<hr><em>HTTP Web Server</em>\n</body></html>\n";
			en+=en1;
			string tmp="HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: "+to_string(en.length())+"\r\n\r\n";
			string finalentity=tmp+en;
			char outstring[1024];
			sprintf(outstring,"%s",finalentity.c_str());
			write(sockfd, outstring, strlen(outstring));
		}
};
#endif


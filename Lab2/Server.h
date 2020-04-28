#ifndef  SERVER_H
#define SERVER_H
#include <iostream>
#include<string.h>
#include<fstream>
#include<stdlib.h>
using namespace std;
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include<pthread.h>
#include<queue>
class Server{
	public:
		sockaddr_in server_addr;
		int server_port;
		int listenfd;
		Server(){}
		Server (int port){
			listenfd = socket(AF_INET,SOCK_STREAM,0); 
			 int reuse=1;
			 setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
			 server_addr.sin_family = AF_INET;  // 协议族，在socket编程中只能是AF_INET。
			 server_addr.sin_addr.s_addr = htonl(INADDR_ANY);          // 任意ip地址。
			 //servaddr.sin_addr.s_addr = inet_addr("118.89.50.198"); // 指定ip地址。
			 server_addr.sin_port = htons(8888);  // 指定通信端口。
			   if (bind(listenfd,(struct sockaddr *)&server_addr,sizeof(server_addr)) != 0 ){
			   	 perror("bind failed!!\n"); 
			   	 close(listenfd); 
			   }
			    if (listen(listenfd,10) != 0 ) {
			     perror("listen failed!\n"); 
			     close(listenfd);
			     }
		}
		int acc_conn(){
			sockaddr_in client_addr;
			int socklen=sizeof(client_addr);
			// multiple accept!!!!!!!!!!!
			int client_sockfd=accept(this->listenfd,(struct sockaddr *)&client_addr,(socklen_t*)&socklen);
			cout<<"client connected "<<inet_ntoa(client_addr.sin_addr)<<'\n';
			return client_sockfd;
		}
		int recv_request(string& str,int client_sockfd){
			 char msg[1024];
			 int mark=0;
			 if (recv(client_sockfd,msg,sizeof(msg),0)>=0) {   // 接收客户端的请求报文。
			 		mark=1;
			 		cout<<"request received\n";
    				//cout<<"request received\n"<<msg<<'\n';
    				str=msg;
    			}
    			//TODO: handle the state code
    			return mark;
		}
		int send_response(int client_sockfd, char* filename){
			int flag=0;//0 for 200  1 for 404
			string header="";
			char body[1024];
			char msg[2048];
			//body
			ifstream infile(filename);
			if(!infile) flag=1;//no such file   404
			infile.seekg(0,ios::end);
			int len=infile.tellg();
			infile.seekg(0,ios::beg);
			infile.read(body,sizeof(body));
			infile.close();
			//header
			if(flag==0){
				header+="HTTP/1.1 200 OK\n";
			}
			else if(flag==1){
				header+="HTTP/1.1 404 NOT FOUND\n";
			}
			header+="Server: SWG's Server\n";
			header+="Content-length: "+to_string(strlen(body))+"\n";
			header+="Content-type: text/html\n\n";
			sprintf(msg,"%s%s",header.data(),body);
			cout<<"发送!!\n";
			//delete it later  
			close(listenfd);
			return send(client_sockfd,msg,strlen(msg),0);
		}
};













#endif

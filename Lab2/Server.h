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
#include<assert.h>
class Server{
	public:
		sockaddr_in server_addr;
		int server_port;
		int listenfd;
		Server(){}
		Server (int port,string ip){
			 server_addr.sin_family = AF_INET;  // 协议族，在socket编程中只能是AF_INET。
			 server_addr.sin_addr.s_addr = htonl(INADDR_ANY);          // 任意ip地址。
			 //server_addr.sin_addr.s_addr = htonl(inet_addr("127.0.0.1")); // 指定ip地址。
			 server_addr.sin_port = htons(port);  // 指定通信端口。
			listenfd = socket(AF_INET,SOCK_STREAM,0); 
			assert(listenfd>=0);
			 int reuse=1;
			 setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
			 assert (bind(listenfd,(struct sockaddr *)&server_addr,sizeof(server_addr)) >= 0 );
			 assert (listen(listenfd,5) >= 0 );
		}
		int acc_conn(){
			struct sockaddr_in client_addr;
			//memset(&client_addr,0,sizeof(client_addr));
			socklen_t socklen=sizeof(sockaddr_in);
			int client_sockfd=accept(this->listenfd,(sockaddr *)&client_addr,&socklen);
			cout<<"client connected "<<inet_ntoa(client_addr.sin_addr)<<'\n';
			return client_sockfd;
		}
		int recv_request(string& str,int client_sockfd){
			 char msg[1024];
			 int mark=0;
			 if (recv(client_sockfd,msg,sizeof(msg),0)>=0) {   // 接收客户端的请求报文。
			 		mark=1;
			 		//cout<<"request received\n";
    				//cout<<"request received\n"<<msg<<'\n';
    				str=msg;
    			}
    			//TODO: handle the state code
    			return mark;
		}
		int send_response(PForm task,int client_sockfd, char* filename){//add headers
			string header="";
			char msg[2048];

			//header
			if(task.code==200){
				header+="HTTP/1.1 200 OK\n";
				}
			else if(flag==501){
				header+="HTTP/1.1 501 Not Implemented\n";
			}
			else {
				header+="HTTP/1.1 404 Not Found\n";
			}
				header+="Server: SWG's Server\n";
				header+="Content-length: "+to_string(strlen(body))+"\n";
				header+="Content-type: text/html\n\n";
				sprintf(msg,"%s%s",header.data(),task.content.data());
			return send(client_sockfd,msg,strlen(msg),0);
		}
};













#endif

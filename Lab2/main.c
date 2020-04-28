#include"Server.h"
struct PForm{
	int method;//0 for GET  1 for POST
	char* filename;
	string content; 
};
int main(){
	int port=8888;
	Server server(port);//set port, the server starts listening
	int client_sockfd=server.acc_conn();
	int cnt=0;
	while(1){//receive client's requests
		++cnt;
		string buffer=server.recv_request(client_sockfd);//message from client
		//THREAD PROCESS: (200OK  CONSIDERED ONLY)
		
		//PForm task=server.parser(buffer);
		PForm task;
//		task.filename="index.html";
//		task.method=0;
//		task.content="";
		task.filename="Post_show";
		task.method=1;
		task.content="postpostpost\n";
		if(task.method==0){//GET
			server.send_response(client_sockfd,task.filename);
		}
		else if(task.method==1){//POST
			ofstream outfile(task.filename,ios::app);
			outfile<<task.content;
			outfile.close();
			server.send_response(client_sockfd,task.filename);
			cout<<cnt<<'\n';
		}
	}
}

#include"Server.h"
struct PForm{
	int code;
	int sockfd;
	int method;//0 for GET  1 for POST
	char* filename;
	string content; 
};
queue<PForm>pipeline;
pthread_mutex_t mutex;
pthread_cond_t cond;
pthread_t* pthread_pool;
int pthread_num,port=8880;
string ip="127.0.0.1";
Server* server;
void* run(void *){
	while(1){
		pthread_mutex_lock(&mutex);
		while(pipeline.empty()){
			pthread_cond_wait(&cond,&mutex);
		}
		cout<<"发送!!\n";
		PForm task=pipeline.front();
		pipeline.pop();
		pthread_mutex_unlock(&mutex);
//		if(task.method==0){//GET
		server->send_response(tasl.method,task.sockfd,task.filename);
//		}
//		else if(task.method==1){//POST
//			ofstream outfile(task.filename,ios::app);
//			outfile<<task.content;
//			outfile.close();
//			server->send_response(task.method, task.sockfd,task.filename);
//		}
	}
}
PForm parser(string buffer){
	PForm task;
	//check method
	string filename="";
	int get_pos=buffer.find("GET");
	int post_pos=buffer.find("POST");
	if(get_pos!=string::npos){
		task.method=0;
		filename=buffer.substr(get_pos+5,buffer.find("HTTP")-get_pos-6);
		task.filename=filename.c_str();
	}
	else if(post_pos!=string::npos){
		task.method=1;
		filename=buffer.substr(post_pos+5,buffer.find("HTTP")-post_pos-6);
		task.filename=filename.c_str();
	}
	else{
		task.method=-1;
		// below to send bad
		task.filename="501.html";
	}
	return task;
}
int main(int argc, char *argv[]){
	pthread_mutex_init(&mutex,NULL);
	cond = PTHREAD_COND_INITIALIZER;
	string ip=argv[2];
	port=atoi(argv[4]);
	if(argc==7)
		pthread_num=atoi(argv[6]);
	else pthread_num=1;
	pthread_pool=new pthread_t[pthread_num];
	for(int i=0;i<pthread_num;i++)
	{
		pthread_create(pthread_pool+i, NULL, run, NULL);
		pthread_detach(pthread_pool[i]);//分离线程
	}
	
	server=new Server(port,ip);//set port, the server starts listening

	string buffer="";
	int client_sockfd=-1;
	while(1){//message from client){//receive client's requests
		client_sockfd=server->acc_conn();
		assert(client_sockfd>=0);
		if(server->recv_request(buffer,client_sockfd)){
			PForm task=parser(buffer);
			task.sockfd=client_sockfd;
//			task.filename="index.html";
//			task.method=0;
//			task.content="";
	//		task.filename="Post_show";
	//		task.method=1;
	//		task.content="postpostpost\n";
			pthread_mutex_lock(&mutex);
			pipeline.push(task);
			//wake up
			pthread_cond_signal(&cond);
			pthread_mutex_unlock(&mutex);
			}
			else break;
	}
	close(server->listenfd);
}

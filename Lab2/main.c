#include"Server.h"
struct PForm{
	int sockfd;
	int method;//0 for GET  1 for POST
	char* filename;
	string content; 
};
queue<PForm>pipeline;
pthread_mutex_t mutex;
pthread_cond_t cond;
pthread_t* pthread_pool;
int pthread_num,port;
Server* server;
void* run(void *){
	while(1){
		pthread_mutex_lock(&mutex);
		while(pipeline.empty()){
			pthread_cond_wait(&cond,&mutex);
		}
		PForm task=pipeline.front();
		pipeline.pop();
		pthread_mutex_unlock(&mutex);
		if(task.method==0){//GET
			server->send_response(task.sockfd,task.filename);
		}
		else if(task.method==1){//POST
			ofstream outfile(task.filename,ios::app);
			outfile<<task.content;
			outfile.close();
			server->send_response(task.sockfd,task.filename);
		}
	}
}
int main(int argc, char *argv[]){
	pthread_mutex_init(&mutex,NULL);
	cond = PTHREAD_COND_INITIALIZER;
	//string ip=argv[2];
	//port=atoi(argv[4]);
	port=8888;
	if(argc==7)
		pthread_num=atoi(argv[6]);
	else pthread_num=4;
	
	pthread_pool=new pthread_t[pthread_num];
	for(int i=0;i<pthread_num;i++)
	{
		pthread_create(pthread_pool+i, NULL, run, NULL);
		pthread_detach(pthread_pool[i]);//分离线程
	}
	
	server=new Server(port);//set port, the server starts listening

	string buffer="";
	int client_sockfd;
	while(1){//message from client){//receive client's requests
		//THREAD PROCESS: (200OK  CONSIDERED ONLY)
		client_sockfd=server->acc_conn();
		//PForm task=server.parser(buffer);
		PForm task;
		task.sockfd=client_sockfd;
		task.filename="index.html";
		task.method=0;
		task.content="";
//		task.filename="Post_show";
//		task.method=1;
//		task.content="postpostpost\n";
		pthread_mutex_lock(&mutex);
		pipeline.push(task);
		//wake up
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
	}
}

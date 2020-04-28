#include <exception>
#include <queue>
#include <assert.h>
#include "httpserver.h"
using namespace std;
#define PTHREAD_NUM 10

pthread_mutex_t mutex;
pthread_cond_t cond;
queue<httpsever> queue1;
int pthread_num;
pthread_t *pthread_pool;

void* run(void* arg)
{
	while(true)
	{
		pthread_mutex_lock(&mutex);
		while(queue1.empty())
		{
			pthread_cond_wait(&cond,&mutex);
		}
		httpsever http=queue1.front();
		queue1.pop();
		pthread_mutex_unlock(&mutex);
		http.solve();
	}
}

int main(int argc, char *argv[])
{
	pthread_mutex_init(&mutex,NULL);
	cond = PTHREAD_COND_INITIALIZER;
	if(argc < 5)
	{
		printf("need ip and port\n");
		return 1;
	}
	string ip = argv[2];
	int port = atoi(argv[4]);
	if(argc==7)
		pthread_num = atoi(argv[6]);
	else pthread_num = PTHREAD_NUM;
	pthread_pool = new pthread_t[pthread_num];
	if(!pthread_pool)
		throw std::exception();
	for(int i=0; i<pthread_num; i++)
	{
		if(pthread_create(pthread_pool+i, NULL, run, NULL) != 0);
		if(pthread_detach(pthread_pool[i]))
		{
			delete [] pthread_pool;
			throw std::exception();
		}
	}
	int sock;
	int connetionfd;
	struct sockaddr_in server_address;
	memset(&server_address,0,sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	sock = socket(AF_INET,SOCK_STREAM,0);
	int reuse=1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	assert(sock>=0);
	int ret = bind(sock, (struct sockaddr*)&server_address, sizeof(server_address));
	assert(ret!=-1);
	ret = listen(sock, 10);
	assert(ret!=-1);
	while(1)
	{
		struct sockaddr_in client;
		socklen_t client_addrlen = sizeof(client);
		connetionfd = accept(sock, (struct sockaddr*)&client, &client_addrlen);
		cout<<"client connected "<<inet_ntoa(client.sin_addr)<<'\n';
		assert(connetionfd>=0);
		httpsever http(connetionfd);
		pthread_mutex_lock(&mutex);
		queue1.push(http);
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
	}
	close(sock);
	return 0;
}


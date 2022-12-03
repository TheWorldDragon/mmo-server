#include <stdio.h>
#include <sys/syslog.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include "all.h"
static int epoll_fd=0;
static int passage_num=0;
static void init_stderr(const char* name,char*buffer)
{
	buffer[0]='\0';
	freopen(strcat(strcat(buffer,name),".error"),"a",stderr);
}
static void init_log(const char* name,char*buffer)
{
	buffer[0]=0;
	openlog(strcat(strcat(buffer,"dragon-server-"),name), 0,0);
}
//return server_fd
static int init_net()
{
	struct sockaddr_in in={};
	in.sin_port=htons(40000);
	in.sin_family=AF_INET;
	int server_fd =socket(AF_INET,SOCK_STREAM,0);
	if(server_fd<0)
		return -1;
	if(bind(server_fd,(void*)&in,sizeof(in))<0)
		return -1;
	if(listen(server_fd,10)<0)
		return -1;
	return server_fd;
}
void hinge_add_passage(passage_t*passage)
{
	struct epoll_event ev={};
	ev.events =EPOLLIN;
	ev.data.ptr = passage;
	passage_num++;
	if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, passage->fd, &ev)<0)
	{
		fprintf(stderr,"epoll add error\n");
	}
	 return;
}
void hinge_remove_passage(passage_t* passage)
{
	struct epoll_event ev={};
	ev.events =EPOLLIN;
	ev.data.ptr = passage;
	passage_num++;
	if(epoll_ctl(epoll_fd, EPOLL_CTL_DEL, passage->fd, &ev)<0)
	{
		fprintf(stderr,"epoll remove error\n");
	}
}
static void add_server_to_epoll(int server_fd)
{
	user_acceptor_t* acceptor = user_acceptor_new(server_fd);
	hinge_add_passage(acceptor->passage);
}
int main(int argc,char* argv[])
{
	int buffer_size = 1024*10;
	char* buffer = malloc(buffer_size); 
	init_stderr(argv[0],buffer);
	init_log(argv[0],buffer);
	int server_fd = init_net();
	if(server_fd<0)
	{
		fprintf(stderr,"init net error\n");
		exit(-1);
	}
	epoll_fd = epoll_create1(0);
	if(epoll_fd<0)
	{
		fprintf(stderr,"create epoll error\n");
		exit(-1);
	}
	add_server_to_epoll(server_fd);
	while(true)
	{
		struct epoll_event events[passage_num];bzero(events,sizeof(events));
		int num = epoll_wait(epoll_fd, events, passage_num, -1);
		if(num<0)
		{
			fprintf(stderr,"epoll_wait error\n");
			exit(-1);
		}
		for(int i=0;i<num;i++)
		{
			passage_t* passage = events[i].data.ptr;
			passage->pass(passage);
		}
	}
}







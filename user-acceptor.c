#include "all.h"

static void exec_client(int* fds)
{
	int fd1[2];
	if(pipe(fd1)<0)
		fprintf(stderr,"pipe error\n");
	int fd2[2];
	if(pipe(fd2)<0)
		fprintf(stderr,"pipe error\n");
	int read_fd=fd1[0];
	int write_fd= fd2[1];
	pid_t pid = fork();
	if(pid<0)
	{
		fprintf(stderr,"fork client error\n");
		exit(-1);
	}
	if(pid==0)
	{
		dup2(fd1[1],STDOUT_FILENO);
		dup2(fd2[0],STDIN_FILENO);
		close(fd1[0]);
		close(fd2[1]);
		if(execl("./client","",0)<0)
		{
			fprintf(stderr,"exec client error\n");
			exit(-1);
		}
	}
	if(close(fd1[1])<0)
		fprintf(stderr,"close piped fd error\n");
	if(close(fd2[0])<0)
		fprintf(stderr,"close piped fd error\n");
	fds[0]=read_fd;fds[1]=write_fd;
}
static void add_worker(int user_fd)
{
	int fds[2];
	exec_client(fds);
	copy_worker_t* read_user_worker = copy_worker_new(user_fd,fds[1]);
	copy_worker_t* write_user_worker = copy_worker_new(fds[0],user_fd);
	copy_worker_pair(read_user_worker,write_user_worker);
	hinge_add_passage(read_user_worker->passage);
	hinge_add_passage(write_user_worker->passage);
}
static void pass0(user_acceptor_t* self)
{
	int fd = self->passage->fd;
	int user_fd = accept(fd,NULL,NULL);
	if(user_fd<0)
	{
		fprintf(stderr,"accept a user but fd is error\n");
	}
	syslog(LOG_INFO,"A user connected");
	add_worker(user_fd);
}
static void pass(passage_t* self)
{
	pass0(self->child);
}
static user_acceptor_t* new(int server_fd)
{
	user_acceptor_t* self = calloc(1,sizeof(*self));
	self->passage = calloc(1,sizeof(*self->passage));
	self->passage->fd = server_fd;
	self->passage->child = self;
	self->passage->pass = pass;
	return self;
}

user_acceptor_t* user_acceptor_new(int server_fd)
{
	return new(server_fd);
}
void user_acceptor_delete(user_acceptor_t *self)
{

}

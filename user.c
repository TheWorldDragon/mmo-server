#include "all.h"
static char* public_key;
static void exec_openssl(int fd[2],int n,...)
{
	int fd1[2];
	int fd2[2];
	pipe(fd1);
	pipe(fd2);
	va_list list;
	va_start(list,n);
	char* params[n+1];
	bzero(params,sizeof(params));
	for(int i=0;i<n;i++)
	{
		params[i]= va_arg(list,char*);
	}
	int read_fd  = fd1[0];
	int write_fd = fd2[1];
	pid_t pid = fork();
	if(pid==0)
	{
		dup2(fd1[1],STDOUT_FILENO);
		dup2(fd2[0],STDIN_FILENO);
		if(execv("/bin/openssl",params)<0)
		{
			fprintf(stderr,"exec openssl error\n");
			exit(-1);
		}
	}
	else if(pid<0)
	{
		fprintf(stderr,"exec openssl error\n");
	}
	close(fd1[1]);
	close(fd2[0]);
	fd[0]=read_fd;
	fd[1] =write_fd;
}
static
int net()
{
	struct sockaddr_in in= {};
	in.sin_port = htons(40000);
	in.sin_family = AF_INET;
	int fd = socket(AF_INET,SOCK_STREAM,0);
	int re =connect(fd,(void*)&in,sizeof(in));
	return fd;
}
char* get_public_key(int fd)
{
	write(fd,0,1);
	char* buf = calloc(1,1024*3);
	int len = read(fd,buf,1024*3);
	while(true)
	{
		if(isprint(*buf))
		{
			break;

		}
		buf++;
	}
	return buf;

	
}
void* encrypt(char* p,int* len)
{
	int fifo = open("user.fifo",O_WRONLY);
	int fds[2];
	exec_openssl(fds,5,"pkeyutl","-encrypt","-inkey","user.fifo","-pubin");
	write(fifo,public_key,strlen(public_key));
	close(fifo);
	write(fds[1],p,strlen(p));

	void* re = malloc(1024);

	 *len =read(fds[0],re,1024);
	close(fds[1]);
	return re;

}
void login(int fd,char* a,char* p)
{
	Login* login = calloc(1,sizeof(*login));
	login__init(login);
	login->account = a;
	int len;
	void* en_p =encrypt(p,&len);
	login->password.data = en_p;
	login->password.len = len;
	u_int8_t* login_out = malloc(100);
	size_t login_size = login__pack(login,login_out);
	NetCommand* cmd = calloc(1,sizeof(*cmd));
	net_command__init(cmd);
	cmd->command = NET_COMMAND__COMMAND__LOGIN;
	cmd->hasparameter = true;
	cmd->parameterlength = login_size;
	void* cmd_out = malloc(1024);
	u_int8_t cmd_sizse = net_command__pack(cmd,cmd_out);
	write(fd,&cmd_sizse,1);
	write(fd,cmd_out,cmd_sizse);
	write(fd,login_out,login_size);
}
int main()
{
	char* buffer= malloc(1024*2);
	int fd = net();
	public_key = get_public_key(fd);
	while(true)
	{
		char* line = strdup(fgets(buffer,1024,stdin));
		if(strstr(line,"login")!=NULL)
		{
			char b1[55],b2[55],b3[55];
			sscanf(line,"%s %s %s",b1,b2,b3);
			login(fd,b2,b3);
			int len =read(fd,buffer,1024);
		}
	}

}


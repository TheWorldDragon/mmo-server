#include "all.h"
enum state
{
	INIT,GET_KEY,EXIT,LOGIN,LOGIN_SUCCESS
};
static char* private_key=NULL;
static char* public_key=NULL;
static void exec_openssl(int fd[2],int n,...);
static char* fifo_name=NULL;
size_t decrypt(void* password,int password_size,char* decrypted);
static void init_err(const char*name,char*buffer)
{
	buffer[0]=0;
	freopen(strcat(strcat(buffer,name),".error"),"a",stderr);
}
static char* gen_key(char* buf,int buf_size)
{
	int fds[2];
	exec_openssl(fds,1,"genrsa");
	int len = read(fds[0],buf,buf_size);
	if(len<=0)
	{
		fprintf(stderr,"exec openssl and read error\n");
		exit(-1);
	}
	buf[len]=0;
	char* re = malloc(len+1);
	strcpy(re,buf);
	close(fds[0]);
	close(fds[1]);
	return re;

}
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
static void init_log(char* name,char*buffer)
{
	openlog(strcat(strcat(buffer,"dragon-server-"),name),0,0);
}
static NetCommand* read_head()
{
	u_int8_t head_len=0;
	if(read(STDIN_FILENO,&head_len,1)<=0)
	{
		return NULL;
	}
	u_int8_t head[head_len];
	if(head_len>0)
	{
		int len = readn(STDIN_FILENO,head,head_len);
		if(len!=head_len)
		{
			return NULL;
		}
	}
	NetCommand* cmd = net_command__unpack(NULL,head_len,head);
	return cmd;
}
static int send_public_key()
{
	ReturnPublicKey re_key = {};
	return_public_key__init(&re_key);
	re_key.publickey = public_key;
	int re_key_size = return_public_key__get_packed_size(&re_key);
	u_int8_t re_key_out[re_key_size];
	if(return_public_key__pack(&re_key,re_key_out)<0)
		return -1;
	NetReturn re={};
	net_return__init(&re);
	re.commandreturn=NET_RETURN__COMMAND_RETURN__RETURN_PUBLIC_KEY;
	re.hasreturnmessage = true;
	re.returnmessagelength = re_key_size;
	u_int8_t re_size = net_return__get_packed_size(&re);
	u_int8_t re_out[re_size];
	if(net_return__pack(&re,re_out)<0)
		return -1;
	u_int8_t buf[1+re_size+re_key_size];
	u_int8_t* p = buf;
	buf[0] = re_size;
	memcpy(p+1,re_out,re_size);
	memcpy(p+1+re_size,re_key_out,re_key_size);
	if(write(STDOUT_FILENO,buf,1+re_size+re_key_size)<=0)
		return -1;
	return 0;

}
static void state_get_key(enum state* state)
{
	while(true)
	{
		NetCommand* cmd = read_head();
		if(cmd==NULL)
		{
			*state = EXIT;
			return;
		}
		if(cmd->command==NET_COMMAND__COMMAND__GET_PUBLIC_KEY)
		{
			if(send_public_key()<0)
			{
				*state = EXIT;
				return;
				
			}
			*state = LOGIN;
			return;
		}
		net_command__free_unpacked(cmd,NULL);
	}
}
static int parse_user_encrypted(size_t head_size,char** account,char**password)
{
	u_int8_t head[head_size];
	if(readn(STDIN_FILENO,head,head_size)<=0)
		return -1;
	Login* login = login__unpack(NULL,head_size,head);
	if(login==NULL)
		return -1;
	char* decrypted =  malloc(1024);
	int decrypted_len = decrypt( login->password.data,login->password.len,decrypted);
	if(decrypted<=0)
	{
		free(decrypted);
		login__free_unpacked(login,NULL);
	}
	*password = decrypted;
	*account = strdup(login->account);
	login__free_unpacked(login,NULL);
		return 0;
}
static void state_login(enum state* state)
{
	while(true)
	{
		NetCommand* cmd = read_head();
		if(cmd==NULL)
		{
			*state = EXIT;
			return;
		}
		if(cmd->command==NET_COMMAND__COMMAND__LOGIN)
		{
			char* password=NULL;char* account=NULL;
			if(parse_user_encrypted(cmd->parameterlength,&account,&password)<0)
			{
				net_command__free_unpacked(cmd,NULL);
				*state = EXIT;
					return;
			}
#if 0
			if(check_login(account,password)<=0)
			{
				free(password);free(account);
				net_command__free_unpacked(cmd,NULL);
				*state = EXIT;
					return;
			}
#endif
			free(password);free(account);
			*state=LOGIN_SUCCESS;
		}
		net_command__free_unpacked(cmd,NULL);
	}
}

static int interact(enum state* state,char* buffer,int buffer_size)
{
	switch(*state)
	{
		case INIT:*state=LOGIN;break;
		case GET_KEY:state_get_key(state);break;
		case LOGIN:state_login(state);break;
		case LOGIN_SUCCESS:break;
		case EXIT:return 0;
		default:break;
	}
	return 1;
}
char* gen_public(char* private_key,char* buffer,int buffer_size)
{
	int fds[2];
	exec_openssl(fds,2,"rsa","-pubout");
	if(write(fds[1],private_key,strlen(private_key))<=0)
	{
		fprintf(stderr,"write openssl error\n");
		exit(-1);
	}
	int len = read(fds[0],buffer,buffer_size);
	if(len<=0)
	{
		fprintf(stderr,"write openssl error\n");
		exit(-1);
	}
	buffer[len]=0;
	len = strlen(buffer);
	char* re = malloc(len+1);
	strcpy(re,buffer);
	close(fds[0]);
	close(fds[1]);
	return re;
}
size_t decrypt(void* password,int password_size,char* decrypted)
{
	int fds[2];
	exec_openssl(fds,4,"pkeyutl","-decrypt","-inkey",fifo_name);
	int fifo_fd = open(fifo_name,O_WRONLY);
	write(fifo_fd,public_key,strlen(public_key));
	close(fifo_fd);
	write(fds[1],password,password_size);
	int len = read(fds[0],decrypted,1024*2);
	if(len<=0)
		fprintf(stderr,"decrypt error\n");
	decrypted[len]=0;
	close(fds[1]);
	close(fds[0]);
	return len;
}
char* get_fifo_name(char* buffer)
{
	sprintf(buffer,"%d.fifo",getpid());
	int len = strlen(buffer);
	char* re = malloc(len+1);
	strcpy(re,buffer);
	return re;
}

void handle_exit()
{
	unlink(fifo_name);

}
int main(int argc,char* argv[])
{
	int buffer_size=1024*10;
	char* buffer = malloc(buffer_size);
	init_err(argv[0],buffer);
	init_log(argv[0],buffer);
	fifo_name = get_fifo_name(buffer);
	sprintf(buffer,"mkfifo %s",fifo_name);
	system(buffer);
	private_key= gen_key(buffer,buffer_size);
	public_key = gen_public(private_key,buffer,buffer_size);
	enum state state=INIT;
	atexit(handle_exit);
	while(true)
	{
		int re = interact(&state,buffer,buffer_size);
		if(re<0)
		{
			fprintf(stderr,"A client exit with error\n");
			break;
		}
		else if(re==0)
		{
			break;
		}
	}
	syslog(LOG_INFO,"A client exiting");
	free(fifo_name);
	free(buffer);
	free(public_key);
	free(private_key);
}

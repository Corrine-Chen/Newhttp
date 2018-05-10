#include "http.h"



int http_startup(const char* ip, int port)
{
	int listen_sock = socket(AF_INET, SOCK_STREAM, 0);

	if(listen_sock < 0)
	{
		perror("error: socket()");
		exit(ERROR_SOCKET);
	}

	int opt = 1;
	setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	struct sockaddr_in server;
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(ip);
	server.sin_port = htons(port);

	if(bind(listen_sock, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		perror("error:bind()");
		exit(ERROR_BIND);
	}

	if(listen(listen_sock, 10)<0)
	{
		perror("error:listen()");
		exit(ERROR_LISTEN);
	}
		
	return listen_sock;
}

// read line form socket
int http_getline(int sockfd, char *buf, int buf_length)
{
	char read_char = '\0';
	int  read_idx = 0;

	while( read_char != '\n' && read_idx < buf_length - 1 )
	{
		ssize_t s = recv(sockfd, &read_char, 1, 0); 

		if ( s > 0)
		{
			if ( read_char == '\r')
			{
				recv(sockfd, &read_char, 1, MSG_PEEK);
				if (read_char == '\n')
				{
					recv(sockfd, &read_char, 1, 0);
				}
				else
				{
					read_char = '\n';
				}
			}
			buf [read_idx] = read_char;
			read_idx++;
		}
	}

	buf[read_idx] = '\0';
	return read_idx;
}


// 处理 客户端发来的请求
void* http_handler_request(void* new_sock)
{

	int sockfd = (int)new_sock;
	char read_buf[BUFSIZE]; 
	int  read_length = 0;
	int cgi = 0;
	int state_code = 0;

#ifdef _DEBUG_
	printf("============================\n");
	char debug_buf[BUFSIZE];
	do
	{
		http_getline(sockfd, debug_buf, sizeof(debug_buf));
		printf("%s",debug_buf);
	}while(strcmp(debug_buf, "\n"));
	printf("============================\n");
#else

	read_length = http_getline(sockfd, read_buf, sizeof(read_buf));
	printf("release\n");

	/* save http method  */
	char method_buf[BUFSIZE];
	/* save http url */
	char url_buf[BUFSIZE];
	/* save path */
	char path_buf[BUFSIZE];

	// 获取方法字段
	int idx_buf = 0;
	int idx_method = 0;
	while(!isspace(read_buf[idx_buf]) && 
		  idx_buf < sizeof(read_buf)  && 
		  idx_method < sizeof(method_buf)) 
	{
		method_buf[ idx_method ] = read_buf[idx_buf];
		idx_method++;
		idx_buf++;
	}

	method_buf[idx_method] = '\0';

	/* only support GET and POST */
	if (strcasecmp(method_buf, "GET") && strcasecmp(method_buf, "POST"))
	{
		state_code = 501;
		goto end;
	}

	while(isspace(read_buf[idx_buf]) && idx_buf < sizeof(read_buf))
	{
		idx_buf++;
	}
	// 获取 URL 字段
	int idx_url = 0;
	while(!isspace(read_buf[idx_buf]) && idx_buf < sizeof(read_buf) && idx_url < sizeof(url_buf))
	{
		url_buf[idx_url] = read_buf[idx_buf];
		idx_url++;
		idx_buf++;
	}

	url_buf[idx_url] = '\0';

	printf("METHOD : %s\n", method_buf);
	printf("URL: %s\n", url_buf);


	// 如果是POST 将 CGI 置为 1
	if (strcasecmp(method_buf, "POST") == 0)
		cgi = 1;

	char* query_string = url_buf;
	// 如果是 GET 并且有参数需要将 cgi 置 1
	// 将路径和参数分割开
	if (strcasecmp(method_buf, "GET") == 0)
	{
		while(*query_string != '\0' &&  *query_string != '?')
			query_string++;
		if(*query_string == '?')
		{
			cgi = 1;
			*query_string = '\0';
			 query_string++;
		}
	}

	printf("----------------\n");
	printf("url_buf :%s\n", url_buf);
	printf("----------------\n");
	printf("query_string:%s\n", query_string);
	printf("----------------\n");

	// 判断是否是访问路径，是的话重定向首页
	sprintf(path_buf, "wwwroot%s", url_buf);
	if ( path_buf[strlen(path_buf) -1] == '/')
	{
		// 用strcat 有bug 应该用 sprintf
		// strcat(path_buf, "index.html");
		sprintf(path_buf, "wwwroot/index.html");
	}

        printf("path_buf:%s\n",path_buf);
	struct stat file_stat;

	if(stat(path_buf, &file_stat) < 0 )
	{ 
                perror("stat");          
		state_code = 404;
               printf("%d\n",state_code);
		goto end;
	}
	else
	{
		// 如果具有可执行权限，就将 CGI 置为1
		if(file_stat.st_mode & S_IXUSR ||
		   file_stat.st_mode & S_IXGRP ||
		   file_stat.st_mode & S_IXOTH)
			cgi = 1;

		// 可执行就去fork-exec 执行 CGI 脚本
		// 此时无需清空头部，因为需要获取长度，
		// 如果是发送静态页面则需要,清空头部信息.
		if (cgi)
		{
			state_code = http_execute_cgi(sockfd, method_buf,  path_buf, query_string);
			if( state_code == 200)
			{
				printf("execute cgi ok..\n");
			}
		}
		else
		{
			http_clear_head(sockfd);
			state_code = http_send_html(sockfd, path_buf, file_stat.st_size);
			if( state_code == 200)
			{
				printf("send html ok ..\n");
			}
		}
	}

end:
	/* echo_error no handler */
	if( state_code != 200)
		http_clear_head(sockfd);
	http_echo_error(sockfd, state_code);
	close(sockfd);
	return NULL;
#endif
}

// 读取并丢弃头部
void http_clear_head(int sockfd)
{
	char buf[BUFSIZE];
	do
	{
		http_getline(sockfd,buf, sizeof(buf));
	}while(strcmp("\n", buf));
}


void http_echo_404(int sockfd)
{
	const char* stat = "HTTP/1.0 404 NOT FOUND \r\n";
	send(sockfd, stat, strlen(stat), 0);
	const char* type = "ContentType: text/html\r\n\r\n";
	send(sockfd, type, strlen(type), 0);
	const char* html = "<HTML><title>404 not found</title><h1>404 ERRON</h1></HTML>";
	send(sockfd, html, strlen(html), 0);
}

void http_echo_403(int sockfd)
{
	const char* stat = "HTTP/1.0 403 Forbidden \r\n";
	send(sockfd, stat, strlen(stat), 0);
	const char* type = "ContentType: text/html\r\n\r\n";
	send(sockfd, type, strlen(type), 0);
	const char* html = "<HTML><title>401 forbidden</title><h1>403 forbidden</h1></HTML>";
	send(sockfd, html, strlen(html), 0);
}
void http_echo_501(int sockfd)
{
	const char* stat = "HTTP/1.0 501 Unavailable \r\n";
	send(sockfd, stat, strlen(stat), 0);
	const char* type = "ContentType: text/html\r\n\r\n";
	send(sockfd, type, strlen(type), 0);
	const char* html = "<HTML><title>501 Unavailable</title><h1>501 Unavailable</h1></HTML>";
	send(sockfd, html, strlen(html), 0);
}
void http_echo_error(int sockfd, int state)
{
	switch(state)
	{
		case 404:
			http_echo_404(sockfd);
			break;
		case 403:
			http_echo_403(sockfd);
			break;
		case 501:
			http_echo_501(sockfd);
			break;
	}
}


int http_execute_cgi(int sockfd, const char* method, const char* path, const char* query_string)
{

	int content_len = -1;
	if (strcasecmp(method, "GET") == 0)
	{
		http_clear_head(sockfd);
	}
	else
	{
		char buff[BUFSIZE];
		int ret = -1;
		do
		{
			http_getline(sockfd, buff, sizeof(buff));
			if(strncasecmp(buff, "Content-Length: ", 16) == 0)
			{
				content_len = atoi(&buff[16]);
			}
		}while(strcmp(buff, "\n"));

		if( content_len == -1)
		{
			return 403;
		}
		printf("Get content length successful\n");
	}

	printf("cgi: path: %s\n", path);
	int cgi_input[2]; // father -->> child 
	int cgi_output[2];// child  -->> father

	if ( pipe(cgi_input) < 0)
	{
		perror("error:pipe()");
		return 403;
	}
	if ( pipe(cgi_output) < 0)
	{
		perror("error:pipe()");
		return 403;
	}

	pid_t pid = fork();
	if (pid <  0)
	{
		perror("error:fork()");
		return 403;
	}
	else if (pid == 0)
	{//child
		close(cgi_output[0]);
		close(cgi_input[1]);

		char METHOD[BUFSIZE];
		char QUERY_STRING[BUFSIZE];
		char CONTENT_LENGTH[BUFSIZE];

		// 1. set method
		sprintf(METHOD, "METHOD=%s", method);
		putenv(METHOD);
		// 2. judge GET ok POST
		if ( strcasecmp(method, "GET") == 0)
		{
			sprintf(QUERY_STRING,"QUERY_STRING=%s", query_string);
			putenv(QUERY_STRING);
		}
		else
		{
			sprintf(CONTENT_LENGTH, "CONTENT_LENGTH=%d", content_len);
			putenv(CONTENT_LENGTH);
		}
		dup2(cgi_output[1],1);
		dup2(cgi_input[0],0);
		execl(path,  NULL);
		perror("error:execl()");
		exit(ERROR_EXECUTE);
	}

	// father
	close(cgi_output[1]);
	close(cgi_input[0]);

	int idx = 0;
	char c;
	for(; idx< content_len; idx++)
	{
		recv(sockfd,&c, 1, 0);
		write(cgi_input[1],&c,1);
	}

	while(1)
	{
		if(read(cgi_output[0], &c, 1) > 0)
		{
			send(sockfd, &c, 1 ,0);
		}
		else
		{
			break;
		}
	}

	waitpid(pid, NULL, 0);
	close(cgi_output[0]);
	close(cgi_input[1]);
	return 200;
}


int http_send_html(int sockfd, const char* path, int file_size)
{

	const char* state = "HTTP/1.0 200 OK\r\n";
	send(sockfd, state, strlen(state), 0);
	const char* type = "Content-Type: text/html\r\n\r\n";
	send(sockfd, type, strlen(type), 0);
	printf("html file path : %s\n",path);
	int file_fd = open(path, O_RDONLY);
	if( file_fd < 0)
	{
		perror("erron: open()");
		return 404;
	}

	if(sendfile(sockfd, file_fd, 0, file_size) <0)
	{
		perror("error:sendfile()");
		close(file_fd);
		return 404;
	}
	close(file_fd);
	return 200;
}

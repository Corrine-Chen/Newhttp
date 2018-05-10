#ifndef HTTP_H
#define HTTP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <arpa/inet.h>
#include <netinet/in.h>


/* 缓冲区大小 */
#define BUFSIZE 1024

/* 错误退出码
 * ERROR_USE 使用错误
 * ERROR_SOCKET socket()函数创建失败
 * ERROR_BIND   bind() 函数绑定失败
 * ERROR_LISTEN listen() 函数失败
 * ERROR_EXECUTE ececl() 程序替换失败
 */
enum STATUS
{
	ERROR_USE,
	ERROR_SOCKET,
	ERROR_BIND,
	ERROR_LISTEN,
	ERROR_EXECUTE
};



/*
 * 函数名称： http_startup()
 * 函数功能： 创建一个监听socket
 * 参数：  
 *   ip   : 指定服务的 IP 地址
 *   port : 要绑定套接字所在的端口
 */
int http_startup(const char* ip, int port);


/*
 * 函数名称： http_getline()
 * 函数功能： 从socket 中读取一行 HTTP 报文 
 * 参数：  
 *    sockfd : 要读的socket 描述符
 *    buf    ：读取到缓冲区的地址
 *    length ：缓冲区的大小
 */
int http_getline(int sockfd, char* buf, int length);

/* 
 * 函数名称：http_handler_requert()
 * 函数功能：线程执行函数，负责处理浏览器发来的请求，分析处发送静态网页还是执行 CGI 脚本
 * 参数：
 *		new_sock: 与客户端连接的socket
 */
void* http_handler_request(void* new_sock);


/*
 * 函数名称：http_execute_cgi()
 * 函数功能：创建子进程，并程序替换执行 CGI 脚本
 * 参数：
 *		sockfd : 与客户端连接的socket 描述符
 *		method ：客户端发来的方法
 *		path   : 可执行 CGI 脚本的路径
 *		query_string ：GET 方法的参数指针
 */
int http_execute_cgi(int sockfd,const char* method, const char* path, const char* query_string);

/*
 *  函数名称：http_send_html()
 *  函数功能：发送本地 html 给客户端
 *  参数：
 *		sockfd ：与客户端连接的socket描述符
 *		path   ：html 文档的路径
 *		file_size ： html 文档的大小
 */
int http_send_html(int sockfd, const char* path, int file_size);

/*
 * 函数名称：http_clear_head()
 * 函数功能：读取并丢弃 http请求报文剩余部分
 */
void http_clear_head(int sockfd);

/*
 * 三个错误状态反馈函数
 */
void http_echo_404(int sockfd);
void http_echo_403(int sockfd);
void http_echo_501(int sockfd);

/*
 * 根据 state 向客户端发送错误反馈
 */
void http_echo_error(int sockfd, int state);

#endif /* HTTP_H */

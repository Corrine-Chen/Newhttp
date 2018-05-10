#include "sql.h"
#include <assert.h>

void send_200()
{
	string stat = "HTTP/1.0 200 OK\r\n";
	cout << stat;
	string type = "ContenType: text/html\r\n\r\n";
	cout << type;
}

void send_404()
{
	string stat = "HTTP/1.0 404 NOT FOUND\r\n";
	cout << stat;
	string type = "ContenType: text/html\r\n\r\n";
	cout << type;
}

//bool get_arg(char* buf)
//{
//	char* method = NULL;
//	char* arg_string = NULL;
//	char* content_length = NULL;
//
//	method = getenv("METHOD");
//	if(method && strcasecmp(method, "GET") == 0){
//		arg_string = getenv("QUERY_STRING");
//		if( !arg_string)
//			return false;
//		strcpy(buf, arg_string);
//	}
//	else if (method && strcasecmp(method, "POST") == 0){
//		content_length = getenv("CONTENT_LENGTH");
//		if( ! content_length){
//			return false;
//		}
//		int i = 0;
//		char c = 0;
//		int nums = atoi(content_length);
//		for(; i < nums; ++i){
//			read(0, &c, 1);
//			buf[i] = c;
//		}
//		buf[i] = 0;
//		return true;
//	}
//	return false;
//}



int get_data(char* buf)
{
	assert(buf);
	char* method = NULL;
	char* arg_string = NULL;
	char* content_length = NULL;

	method = getenv("METHOD");
	if( method && strcasecmp(method, "GET") == 0)
	{
		arg_string = getenv("QUERY_STRING");
		if( !arg_string)
		{
			cerr << "get methpd GET arg error" << endl;
			return 1;
		}
		strcpy(buf, arg_string);
	}
	else if(method && strcasecmp(method, "POST") == 0)
	{
		content_length = getenv("CONTENT_LENGTH");
		if(!content_length)
		{
			cerr <<"get method POST content_length error!" << endl;
				return 2;
		}

		int i = 0;
		char c = 0;
		int nums = atoi(content_length);
		for(; i < nums; i++)
		{
			read(0, &c, 1);
			buf[i] = c;
		}
		buf[i] = '\0';
	}
	else
	{
		cerr << "get method error ! \n" << endl;
		return 3;
	}
	return 0;
}


void insert_data(char* buf)
{

	char* arg[4];
	int idx = 0;

	char* start = buf;
	while(*start && idx < 4){
		if(*start == '='){
			start++;
			arg[idx++] = start;
		}
		if( *start == '&'){
			*start = '\0';
		}
		start++;
	}
	// arg demo 　　　name=''&sex=''&hoby=''&school=''
	SqlConnector sql;
     if(sql.connect())
	if(sql.insert(arg[0],arg[1], arg[2], arg[3]))
	{
		send_200();
		cout << "<HTML>  insert OK  </HTML>";
		return ;
	}
	send_404();
	cout << "<HTML> insert failt </HTML>";

}

int main()
{
	char buf[1024];
	if( get_data(buf) )
	{
		cerr << "get_data_error" << endl;
	}
	insert_data(buf);

	return 0;
}

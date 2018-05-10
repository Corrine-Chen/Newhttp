#ifndef SQL_H
#define SQL_H

#include<iostream>
#include<string>
#include<string.h>
#include<stdlib.h>
#include<mysql.h>
#include<unistd.h>
using namespace std;
class SqlConnector
{
public:
	SqlConnector(const std::string _user="root",
	     		 const std::string _passwd="",
				 const std::string _ip="127.0.0.1",
				 const std::string _db="http",
				 const int& _port=3306);

	~SqlConnector();
	bool operator!=(const SqlConnector& sql);
	bool operator==(const SqlConnector& sql);
        bool connect();
	bool select();


        bool select_data(const std::string& name, 
			 const std::string& sex,
        		  const std::string& hobby,
  			  const std::string& school);

	bool insert(const std::string& name,
			    const std::string& sex,
				const std::string& hobby,
				const std::string& school);
private:
	MYSQL* connector;
	std::string user;
	std::string passwd;
	std::string ip;
	std::string db;
	int port;
};

#endif 

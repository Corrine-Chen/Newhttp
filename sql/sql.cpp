#include "sql.h"

SqlConnector::SqlConnector(const std::string _user,
					       const std::string _passwd,
						   const std::string _ip,
						   const std::string _db,
						   const int& _port)
	 : user(_user)
	 , passwd(_passwd)
	 , ip(_ip)
	 , db(_db)
	 , port(_port)
{
	connector = mysql_init(NULL);
}

bool SqlConnector::connect()
{
	if(mysql_real_connect(connector, ip.c_str(),\
				         user.c_str(), passwd.c_str(),\
						 db.c_str(), port, NULL, 0))
{
      // string c=mysql_error(connector);
	return true;
}
		return false;

}
bool SqlConnector::insert(const std::string& name, 
			 				  const std::string& sex,
			   				  const std::string& hobby,
			   				  const std::string& school)
{
	std::string insert="INSERT INTO  user_info1(name, sex, hobby, school) values('";
	insert += name;
	insert += "','";
	insert += sex;
	insert += "','";
	insert += hobby;
	insert += "','";
	insert += school;
	insert += "')";

	if( mysql_query(connector, insert.c_str()))
		return false;
	return true;
}


bool SqlConnector::select()
{
	std::string select("select * from user_info1");

	if( mysql_query( connector, select.c_str()) == 0)
	{
		MYSQL_RES *res = mysql_store_result(connector);

		if(res)
		{
			// get the row of the query result
			int rows = mysql_num_rows(res);
			//get the field of the query result 
			int fields = mysql_num_fields(res);
			MYSQL_FIELD *fd = NULL;

			// get the name of the field
			for(; fd = mysql_fetch_field(res); )
			{
				cout << fd->name << "--";
			}

			cout << "<br>";
			// print row and field 
			int i = 0;
			for (; i < rows; ++i)
			{
				MYSQL_ROW row = mysql_fetch_row(res);
				int j = 0;
				for(; j < fields; j++)
					cout << row[j] << "--";

				cout << "<br>";
			}
			return true;
		}
	}
	return false;
}


bool SqlConnector::select_data(const std::string& name, 
			 				  const std::string& sex,
			   				  const std::string& hobby,
			   				  const std::string& school)

{

    cout<<"<body bgcolor='#696969'>" ;
    std::string select_data;  
    if(name.empty()&&sex.empty()&&hobby.empty()&&school.empty()){
        select_data="select * from user_info1";
     }  else if(!name.empty()&&sex.empty()&&hobby.empty()&&school.empty()){  	
        select_data="select * from user_info1 where name='";
        select_data+=name;
        select_data+="'";
     }else if(name.empty()&&!sex.empty()&&hobby.empty()&&school.empty()){  	
        select_data="select * from user_info1 where sex='";
                    select_data+=sex;
                    select_data+="'";        
     } else if(name.empty()&&sex.empty()&&!hobby.empty()&&school.empty()){  	
       	select_data="select * from user_info1 where hobby='";
                    select_data+=hobby;
                    select_data+="'";
     }else if(name.empty()&&sex.empty()&&hobby.empty()&&!school.empty()){  	
        select_data="select * from user_info1 where school='";
                select_data+=school;
                  select_data+="'";
     }else if(!name.empty()&&!sex.empty()&&hobby.empty()&&school.empty()){
          	
        select_data="select * from user_info1 where name='";
                select_data+=name;
                select_data+="'";
                select_data+="and sex ='";
                select_data+=sex;
                select_data+="'";

     }else if(!name.empty()&&sex.empty()&&!hobby.empty()&&school.empty()){  	
          	
        select_data="select * from user_info1 where name='";
                select_data+=name;
                select_data+="'";
                select_data+="and hobby ='";
                select_data+=hobby;
                select_data+="'";
     }else if(!name.empty()&&sex.empty()&&hobby.empty()&&!school.empty()){  	
          	
        select_data="select * from user_info1 where name='";
                select_data+=name;
                select_data+="'";
                select_data+="and school ='";
                select_data+=school;
                select_data+="'";
     }else if(name.empty()&&!sex.empty()&&!hobby.empty()&&school.empty()){  	
          	
        select_data="select * from user_info1 where hobby='";
                select_data+=hobby;
                select_data+="'";
                select_data+="and sex ='";
                select_data+=sex;
                select_data+="'";
     }else if(name.empty()&&!sex.empty()&&hobby.empty()&&!school.empty()){  	
          	
        select_data="select * from user_info1 where school='";
                select_data+=school;
                select_data+="'";
                select_data+="and sex ='";
                select_data+=sex;
                select_data+="'";
     }else if(name.empty()&&sex.empty()&&!hobby.empty()&&!school.empty()){  	
          	
        select_data="select * from user_info1 where hobby='";
                select_data+=hobby;
                select_data+="'";
                select_data+="and school ='";
                select_data+=school;
                select_data+="'";
     }else if(!name.empty()&&!sex.empty()&&!hobby.empty()&&school.empty()){  	
          	
        select_data="select * from user_info1 where name='";
                select_data+=name;
                select_data+="'";
                select_data+="and sex ='";
                select_data+=sex;
                select_data+="'";
                select_data+="and hobby ='";
                select_data+=hobby;
                select_data+="'";
     }else if(!name.empty()&&!sex.empty()&&hobby.empty()&&!school.empty()){  	
          	
        select_data="select * from user_info1 where name='";
                select_data+=name;
                select_data+="'";
                select_data+="and sex ='";
                select_data+=sex;
                select_data+="'";
                select_data+="and school ='";
                select_data+=school;
                select_data+="'";
     }else if(!name.empty()&&sex.empty()&&!hobby.empty()&&!school.empty()){  	
          	
        select_data="select * from user_info1 where name='";
                select_data+=name;
                select_data+="'";
                select_data+="and school ='";
                select_data+=sex;
                select_data+="'";
                select_data+="and hobby ='";
                select_data+=hobby;
                select_data+="'";
     }else if(name.empty()&&!sex.empty()&&!hobby.empty()&&!school.empty()){
          	
        select_data="select * from user_info1 where school='";
                select_data+=school;
                select_data+="'";
                select_data+="and sex ='";
                select_data+=sex;
                select_data+="'";
                select_data+="and hobby ='";
                select_data+=hobby;
                select_data+="'";
      }else {	
        select_data="select * from user_info1 where school='";
                select_data+=school;
                select_data+="'";
                select_data+="and sex ='";
                select_data+=sex;
                select_data+="'";
                select_data+="and hobby ='";
                select_data+=hobby;
                select_data+="'";
                select_data+="and name ='";
                select_data+=name;
                select_data+="'";
      }
     	
   if( mysql_query( connector, select_data.c_str()) == 0)
   {
    	MYSQL_RES *res = mysql_store_result(connector);

    	if(res)
    	{
    		// get the row of the query result
    		int rows = mysql_num_rows(res);
                if(rows==0)
                {
                     cout<<"<title>NO SUCH DATA</title>"<<endl;
                     cout<<"<br>";                    
                     cout<<"<br>";                    
                     cout<<"<br>";                    
                     cout<<"<br>";                    
                     cout<<"<center><H1>"<<"NO SUCH DATA!"<<"</H1></center>";
                     return true;
                }
    		//get the field of the query result 
    		int fields = mysql_num_fields(res);
    		MYSQL_FIELD *fd = NULL;
    		// get the name of the field
                cout<<"<table style=background:#FAEBD7  border=1 align=center>";
                cout<<"<title>Users Table</title>";
                cout<<"<caption><H1>"<<"Users Table"<<"</H1></caption>";
                
                cout<<"<tr>";    
		for(; fd = mysql_fetch_field(res); )
    		{
    			cout <<"<th><h2>"<< fd->name <<"</h2></th>";
 
    		}
                cout<<"</tr>";
    		// print row and field 
    		int i = 0;
        	for (; i < rows; ++i)
    		{
    	        cout<<"<tr>";
    			MYSQL_ROW row = mysql_fetch_row(res);
                              int j = 0;
    			for(; j < fields; j++)
    				cout <<"<td><h2>"<< row[j] << "</h2></td>";

                cout<<"</tr>";
    		}
                cout<<"</table>";
    		return true;
    	}
   }
}
SqlConnector::~SqlConnector()
{
	mysql_close(connector);
}

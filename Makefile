BIN=httpd
CC=gcc
SRC=http.c main.c
OBJ=$(SRC:.c=.o)
#FLAGS=-D_DEBUG_
LDFLAGS=-lpthread
CGI_PATH=sql wwwroot/cgi-bin

.PHONY:all
all:$(CGI_BIN) $(BIN) cgi


cgi:
	@for i in `echo $(CGI_PATH)`;\
	do\
		cd  $$i;\
		make;\
		cd -;\
	done


# 链接
$(BIN):$(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)
	@echo "[linking] [$^] to [$@] ... done"

# 编译
%.o : %.c
	@$(CC) -c $< $(FLAGS) $(LDFLAGS)
	@echo "[compiling] [$^] to [$@] ... done"

.PHONY:clean
clean:
	@rm -rf $(OBJ) $(BIN)  output
	@for i in `echo $(CGI_PATH)`;\
	do\
		cd  $$i;\
		make clean;\
		cd -;\
	done
	@echo "clean project ... done"

.PHONY:output
output:
	@mkdir -p output/wwwroot/cgi-bin
	@cp -rf conf          output
	@cp wwwroot/*.html    output/wwwroot
	@cp sql/select_cgi    output/wwwroot/cgi-bin
	@cp sql/insert_cgi    output/wwwroot/cgi-bin
	@cp httpd             output 
	@cp wwwroot/cgi-bin/math_cgi output/wwwroot/cgi-bin
	@cp -r sql/mysql_lib output
	@cp plugin/ctl_server.sh output
	@echo "output project ... done"

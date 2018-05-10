.PHONY:all
all:select_cgi insert_cgi

WORK_PATH=$(shell pwd)
CC=g++
INCLUDE=$(WORK_PATH)/mysql_lib/include

# Method 1  use system lib
# Method 2  use self lib
LIB=/usr/lib/mysql

LIB=$(WORK_PATH)/mysql_lib/lib
LDFLAGS=-lmysqlclient

select_cgi:select_cgi.o sql.o
	@$(CC) -o $@ $^  -L$(LIB) $(LDFLAGS) 
	@echo "[linking] [$^] to [$@] ... done"
insert_cgi:insert_cgi.o sql.o
	@$(CC) -o $@ $^  -L$(LIB) $(LDFLAGS)
	@echo "[linking] [$^] to [$@] ... done"

%.o:%.cpp
	@$(CC) -c $<  -I$(INCLUDE)
	@echo "[linking] [$^] to [$@] ... done"

.PHONY:clean
clean:
	@rm -f *.o insert_cgi select_cgi

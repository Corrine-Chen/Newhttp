支持 GET 和 POST 线程池的HTTP 服务器。


前提：需要在 sql.cpp 中修改自己的数据库表 以及连接数据库的信息,如没有安装sql库 需要手动指定本地路径下的库


使用方法:

```
mkidr http

cd http

git clone https://github.com/HuoZhiGang/HTTP_SERVER.git

git cd HTTP_SERVER/

./build.sh

cd output/
# 以守护进程方式启动
./ctr_server.sh -d

# 前台启动服务
./ctl_server.sh -s

# 关闭
./ctl_server.sh -t

# 状态
./ctl_server.sh -ss
```

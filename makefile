build:
	@echo "开始编译"
	g++ -c Connection.cpp -I/usr/include/mysql
	g++ -c ConnectionPool.cpp -I/usr/include/mysql
	g++ -c main.cpp -I/usr/include/mysql
	g++ -o Pool Connection.o ConnectionPool.o main.o -L/usr/lib/x86_64-linux-gnu -lpthread -lmysqlclient
	@echo "编译完成"

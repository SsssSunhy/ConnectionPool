# 数据库连接池

- 关键技术：MySQL数据库编程、单例模式、queue队列容器、C++11多线程编程、线程互斥、线程同步通信和 unique_lock、基于CAS的原子整形、智能指针shared_ptr、lambda表达式、生产者-消费者线程模型
- 环境：Linux

## 运行

```bash
cd ConnectionPool
make
```

```bash
./Pool
```

## 文件结构

```bash
.
├── Connection.cpp	# 实现MySQL数据库的操作
├── Connection.h
├── ConnectionPool.cpp	# 实现连接池功能模块
├── ConnectionPool.h	
├── main.cpp	# main
├── makefile	
├── mysql.ini	# 配置文件
└── public.h	# 存放一些公共的处理的函数
```

## 项目背景

为了提高MySQL数据库（基于C/S设计）的访问瓶颈，除了在服务器端增加缓存服务器缓存常用的数据 之外（例如redis），还可以增加连接池，来提高MySQL Server的访问效率，在高并发情况下，大量的 **TCP三次握手、MySQL Server连接认证、MySQL Server关闭连接回收资源和TCP四次挥手**所耗费的性能时间也是很明显的，增加连接池就是为了减少这一部分的性能损耗。 

## 功能实现设计

ConnectionPool.cpp和ConnectionPool.h：连接池代码实现

Connection.cpp和Connection.h：数据库操作代码、增删改查代码实现

**连接池主要包含了以下功能点：**

- 连接池只需要一个实例，所以ConnectionPool以单例模式进行设计 

- 从ConnectionPool中可以获取和MySQL的连接Connection 

- 空闲连接Connection全部维护在一个线程安全的Connection队列中，使用线程互斥锁保证队列的线程安全 

- 如果Connection队列为空，还需要再获取连接，此时需要动态创建连接，上限数量是maxSize 

- 队列中空闲连接时间超过maxIdleTime的就要被释放掉，只保留初始的initSize个连接就可以了，这个功能点肯定需要放在独立的线程中去做 

- 如果Connection队列为空，而此时连接的数量已达上限maxSize，那么等待connectionTimeout时间如果还获取不到空闲的连接，那么获取连接失败，此处从Connection队列获取空闲连接，可以使用带超时时间的mutex互斥锁来实现连接超时时间 

- 用户获取的连接用shared_ptr智能指针来管理，用lambda表达式定制连接释放的功能（不真正释放连接，而是把连接归还到连接池中） 

- 连接的生产和连接的消费采用生产者-消费者线程模型来设计，使用了线程间的同步通信机制条件变量和互斥锁

## 数据库创建

```mysql
create database chat;

use chat;

CREATE TABLE user (
  id int(11) NOT NULL AUTO_INCREMENT,
  name varchar(50) DEFAULT NULL,
  age int(11) DEFAULT NULL,
  sex enum('male', 'female') DEFAULT NULL,
  PRIMARY KEY (id)
);
```

```mysql
mysql> desc user;
+-------+-----------------------+------+-----+---------+----------------+
| Field | Type                  | Null | Key | Default | Extra          |
+-------+-----------------------+------+-----+---------+----------------+
| id    | int                   | NO   | PRI | NULL    | auto_increment |
| name  | varchar(50)           | YES  |     | NULL    |                |
| age   | int                   | YES  |     | NULL    |                |
| sex   | enum('male','female') | YES  |     | NULL    |                |
+-------+-----------------------+------+-----+---------+----------------+
```

- 数据库测试

```c++
#include<iostream>
using namespace std;
#include "Connection.h"

int main()
{
    Connection conn;
    char sql[1024] = {0};
    sprintf(sql, "insert into user(name, age, sex) values('%s', '%d', '%s')", 
    "zhang san", 20, "male");
    conn.connect("127.0.0.1", 3306, "root", "123", "chat");
    conn.update(sql);
    return 0;
}
```

## 常见问题

- 找不到 mysql.h

```bash
# 缺库
sudo apt-get install libmysqlclient-dev
```

```bash
# 编译时添加mysql路径并设置链接器, 如：
g++ -o main main.cpp Connection.cpp -I/usr/include/mysql -lmysqlclient
```


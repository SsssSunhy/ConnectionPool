#pragma once
#include <string>
#include <queue>
#include <mutex>
#include <iostream>
#include <atomic>
#include <thread>
#include <condition_variable>
#include <memory>
#include <functional>
using namespace std;
#include "Connection.h"

/*
实现连接池功能模块
*/
class ConnectionPool
{
public:
    // 获取连接池对象实例
    static ConnectionPool* getConnectionPool();
    // 给外部提供接口，从连接池中获取一个可用的空闲连接
    shared_ptr<Connection> getConnection();
private:
    // 构造函数私有化
    ConnectionPool();

    // 从配置文件中加载配置项
    bool loadConfigFile(); 

    // 运行在独立的线程中，专门负责生产新连接
    void produceConnectionTask();

    // 扫描超过maxIdleTime时间的空闲连接，进行对于的连接回收
    void scannerConnectionTask();

    string _ip; // mysql的ip地址
    unsigned short _port; // mysql的端口号 3306
    string _username; // mysql登录用户名
    string _password; // mysql登录密码
    string _dbname; // 连接的数据库名称
    int _initSize; // 连接池的初始连接量
    int _maxSize; // 连接池的最大连接量
    int _maxIdleTime; // 连接池最大空闲时间
    int _connectionTimeout; // 连接池获取连接的超时时间

    queue<Connection*> _connectionQue; // 存储mysql连接的队列
    mutex _queueMutex; // 维护连接队列的线程安全互斥锁
    /*
    atomic_int 是 C++ 标准库提供的一种类型，可以对整数进行原子操作，
    保证操作的原子性和线程安全。在多线程并发的程序中，
    如果多个线程同时访问同一个变量，会出现竞争条件（Race Condition）的问题，
    这会导致程序的行为出现未定义的情况。
    使用 atomic_int 类型可以避免这种问题，确保对共享变量的操作不会被其它线程干扰，
    从而保证程序的正确性。
    通常情况下，atomic_int 被用于实现各种同步机制，如互斥锁、条件变量等。
    */
    atomic_int _connectionCnt; // 记录连接所创建的connection连接的总数量 
    condition_variable cv; // 设置条件变量，用于连接生产线程和连接消费线程的通信
};
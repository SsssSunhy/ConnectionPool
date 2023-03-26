#include<iostream>
using namespace std;
#include "Connection.h"
#include "ConnectionPool.h"

void SingleThread_no_ConnectionPool()
{
    // 单线程不使用连接池
    for(int i = 0; i < 1000; i++)
    {
        Connection conn;
        char sql[1024] = {0};
        sprintf(sql, "insert into user(name, age, sex) values('%s', '%d', '%s')", 
        "zhang san", 20, "male");
        conn.connect("127.0.0.1", 3306, "root", "123", "chat");
        conn.update(sql);
    }
}

void SingleThread_ConnectionPool()
{
    // 获取一个连接池，开始获取一次就行
    ConnectionPool *cp = ConnectionPool::getConnectionPool();
    for(int i = 0; i < 1000; i++)
    {
        shared_ptr<Connection> sp = cp->getConnection();
        char sql[1024] = {0};
        sprintf(sql, "insert into user(name, age, sex) values('%s', '%d', '%s')", 
        "zhang san", 20, "male");
        sp->update(sql);
    }
}

void MultiThreaded_ConnectionPool()
{
    thread t1([](){
        ConnectionPool *cp = ConnectionPool::getConnectionPool();
        for(int i = 0; i < 250; i++)
        {
            shared_ptr<Connection> sp = cp->getConnection();
            char sql[1024] = {0};
            sprintf(sql, "insert into user(name, age, sex) values('%s', '%d', '%s')", 
            "zhang san", 20, "male");
            sp->update(sql);
        }
    });
    thread t2([](){
        ConnectionPool *cp = ConnectionPool::getConnectionPool();
        for(int i = 0; i < 250; i++)
        {
            shared_ptr<Connection> sp = cp->getConnection();
            char sql[1024] = {0};
            sprintf(sql, "insert into user(name, age, sex) values('%s', '%d', '%s')", 
            "zhang san", 20, "male");
            sp->update(sql);
        }
    });
    thread t3([](){
        ConnectionPool *cp = ConnectionPool::getConnectionPool();
        for(int i = 0; i < 250; i++)
        {
            shared_ptr<Connection> sp = cp->getConnection();
            char sql[1024] = {0};
            sprintf(sql, "insert into user(name, age, sex) values('%s', '%d', '%s')", 
            "zhang san", 20, "male");
            sp->update(sql);
        }
    });
    thread t4([](){
        ConnectionPool *cp = ConnectionPool::getConnectionPool();
        for(int i = 0; i < 250; i++)
        {
            shared_ptr<Connection> sp = cp->getConnection();
            char sql[1024] = {0};
            sprintf(sql, "insert into user(name, age, sex) values('%s', '%d', '%s')", 
            "zhang san", 20, "male");
            sp->update(sql);
        }
    });
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

void MultiThreaded_no_ConnectionPool()
{
    thread t1([](){
        for(int i = 0; i < 250; i++)
        {
            Connection conn;
            char sql[1024] = {0};
            sprintf(sql, "insert into user(name, age, sex) values('%s', '%d', '%s')", 
            "zhang san", 20, "male");
            conn.connect("127.0.0.1", 3306, "root", "123", "chat");
            conn.update(sql);
        }
    });
    thread t2([](){
        for(int i = 0; i < 250; i++)
        {
            Connection conn;
            char sql[1024] = {0};
            sprintf(sql, "insert into user(name, age, sex) values('%s', '%d', '%s')", 
            "zhang san", 20, "male");
            conn.connect("127.0.0.1", 3306, "root", "123", "chat");
            conn.update(sql);
        }
    });
    thread t3([](){
        for(int i = 0; i < 250; i++)
        {
            Connection conn;
            char sql[1024] = {0};
            sprintf(sql, "insert into user(name, age, sex) values('%s', '%d', '%s')", 
            "zhang san", 20, "male");
            conn.connect("127.0.0.1", 3306, "root", "123", "chat");
            conn.update(sql);
        }
    });
    thread t4([](){
        for(int i = 0; i < 250; i++)
        {
            Connection conn;
            char sql[1024] = {0};
            sprintf(sql, "insert into user(name, age, sex) values('%s', '%d', '%s')", 
            "zhang san", 20, "male");
            conn.connect("127.0.0.1", 3306, "root", "123", "chat");
            conn.update(sql);
        }
    });
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

int main()
{
    clock_t begin = clock();
    // SingleThread_no_ConnectionPool();    // 1884435 ms
    // SingleThread_ConnectionPool();       // 103686 ms
    // MultiThreaded_ConnectionPool();      // 87371 ms
    
    // 测试多线程无连接池时需要先登陆一下mysql, 否则可能出现Segmentation fault
    Connection conn;
    conn.connect("127.0.0.1", 3306, "root", "123", "chat");
    MultiThreaded_no_ConnectionPool();      // 9513601 ms
    clock_t end = clock();
    cout << (end - begin) << " ms" << endl;
    return 0;
}
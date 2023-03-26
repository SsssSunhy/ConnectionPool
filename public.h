#pragma once

// 打印, 文件, 行号, 时间信息
#define LOG(str) \
    cout << __FILE__ << ":" << __LINE__ << " " << \
    __TIMESTAMP__ << " : " << str << endl;

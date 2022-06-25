#ifndef DATA_H
#define DATA_H

#include <QString>
#include <vector>

class DataPackage   //数据包
{
public:
    DataPackage(int, int, std::vector<QString>);
    int opt;    //操作编号
    int length; //参数的长度，用于判断系统是否成功接收到socket发送的信息
    std::vector<QString> parameters;    //用于存发送的参数
    bool isCompleted(); //判断当前数据包是否是完整的
};

#endif // DATA_H

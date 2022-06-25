#ifndef DATAANALYST_H
#define DATAANALYST_H
#include <QObject>
#include <map>
#include <vector>
#include "datapackage.h"
class DataAnalyst   //数据分析类，承担数据在服务器和客服端之间通信的数据格式封装任务
{
public:
    DataAnalyst();
    QString optToQString(QString, std::vector<QString>);    //将指定操作转成特殊格式的字符串
    QString packageToQString(DataPackage*);                 //将数据包转成字符串格式
    DataPackage* QStringToPackage(QString);                 //将字符串格式转成数据包
    DataPackage* optToPackage(QString, std::vector<QString>);   //将制定操作转成数据包
    DataPackage* errorPackage(QString);                            //特殊的失败数据包，用于给客服端传递失败信息
    DataPackage* acceptPackage(QString);                           //特殊的成功数据包，用于给客服端传递成功信息
    DataPackage* messageErrorPackage();
    int getOptId(QString);                                  //通过制定的功能获取ID
private:
    QString digitToQString(int);        //整数转QString
    int QStringToDigit(QString);        //QString转整数
    bool optExist(QString);

    std::map<QString, int> optMp;       //将功能映射成特殊的数字
    std::vector<QString> optSet;        //操作功能集，【未来在这里扩展】
    int cnt;
};

#endif // DATAANALYST_H

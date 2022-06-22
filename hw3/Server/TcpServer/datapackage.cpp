#include "datapackage.h"

DataPackage::DataPackage(int op, int len, std::vector<QString> cmd)
    : opt(op), length(len), parameters(cmd)
{

}

/**************************************************************************
 * @author: 曾群鸿
 * @Date: 2021-8-27
 * @Description: 判断一个数据包是否是完整的
 * @Parameters:
 **************************************************************************/
bool DataPackage::isCompleted()
{
    return length == parameters.size();
}

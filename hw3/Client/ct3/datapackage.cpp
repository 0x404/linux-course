#include "datapackage.h"

DataPackage::DataPackage(int op, int len, std::vector<QString> cmd)
    : opt(op), length(len), parameters(cmd)
{

}

bool DataPackage::isCompleted()
{
    // 检查数据包是否完整
    return length == parameters.size();
}

#include "datapackage.h"

DataPackage::DataPackage(int op, int len, std::vector<QString> cmd)
    : opt(op), length(len), parameters(cmd)
{

}

bool DataPackage::isCompleted()
{
    return length == parameters.size();
}

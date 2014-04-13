#include "myData.h"

myData::myData()
{

}

myData::myData(double value_, int index_)
{
	dataValue = value_;
	dataIndex = index_;
}
double myData::getValue() const
{
	return dataValue;
}
int myData::getIndex() const
{
	return dataIndex;
}



#ifndef MAXQUERY_H_H
#define MAXQUERY_H_H

#include "query.h"
#include "myData.h"
#include <deque>          // std::queue


const int WINDOWSIZE = 500;

template <class T>
class maxQuery:public Query<T>
{
private:
	T data;
	int size;
	std::deque<T> dataWindow;
	int windowSize;
public:
	maxQuery();
	maxQuery(int windowSize_);
	~maxQuery();
	double addNewData(T data_);
	T getCurMax();
	T update_with_new_value(T data);

};

template<class T>
maxQuery<T>::~maxQuery()
{

}

template<class T>
maxQuery<T>::maxQuery(int windowSize_)
{
	size = 0;
	windowSize = windowSize_;
}

template<class T>
maxQuery<T>::maxQuery()
{
	size = 0;
	windowSize = WINDOWSIZE;
}

template<class T>
double maxQuery<T>::addNewData(T data)
{
	if (dataWindow.empty())
	{
		dataWindow.push_back(data);
		return data.getValue();
	}
	if (dataWindow.front().getIndex() <= data.getIndex() - windowSize)
		dataWindow.pop_front();		// out of date
	if (dataWindow.front().getValue() >= data.getValue())
	{
		while (dataWindow.back().getValue() < data.getValue())
			dataWindow.pop_back();
		dataWindow.push_back(data);
	}
	else
	{
		while ((!dataWindow.empty()))
		{
			dataWindow.pop_front();
		}
		dataWindow.push_back(data);
	}
	return data.getValue();
}

template<class T>
T maxQuery<T>::getCurMax()
{
	return dataWindow.front();
} 

template<class T>
T maxQuery<T>::update_with_new_value(T data)
{
	addNewData(data);
	return getCurMax();
}
#endif
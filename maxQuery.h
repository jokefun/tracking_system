#ifndef MAXQUERY_H_H
#define MAXQUERY_H_H

#include "query.h"
#include <deque>          // std::queue


const int WINDOWSIZE = 500;

template <class T>
class MaxQuery:public Query<T>
{
	private:
		struct myData
		{
			T value;
			int index;
		};
		int size;
		int index;
		std::deque<myData> dataWindow;
		int windowSize;
		double addNewData(T data_);
		T getCurMax();
	public:
		MaxQuery();
		MaxQuery(int windowSize_);
		MaxQuery(const MaxQuery<T>& MQ);
		MaxQuery(MaxQuery<T>&& MQ);
		~MaxQuery();
		T update_with_new_value(T data);
		MaxQuery & operator=(const MaxQuery &);

};

template<class T>
MaxQuery<T> & MaxQuery<T>:: operator=(const MaxQuery &MQ)
{
	size = MQ.size;
	index = MQ.index;
	windowSize = MQ.windowSize;
	dataWindow = MQ.dataWindow;
	return *this;
}


template<class T>
MaxQuery<T>::MaxQuery(MaxQuery<T>&& MQ)
{
	size = MQ.size;
	index = MQ.index;
	windowSize = MQ.windowSize;
	dataWindow = MQ.dataWindow;
}


template<class T>
MaxQuery<T>::~MaxQuery()
{
	
}

template<class T>
MaxQuery<T>::MaxQuery(const MaxQuery<T>& MQ)
{
	size = MQ.size;
	index = MQ.index;
	windowSize = MQ.windowSize;
	dataWindow = MQ.dataWindow;
}

template<class T>
MaxQuery<T>::MaxQuery(int windowSize_)
{
	size = 0;
	index = 0;
	windowSize = windowSize_;
}

template<class T>
MaxQuery<T>::MaxQuery()
{
	size = 0;
	index = 0;
	windowSize = WINDOWSIZE;
}

template<class T>
double MaxQuery<T>::addNewData(T value)
{
	index++;
	struct myData data;
	data.value = value;
	data.index = index;
	if (dataWindow.empty())
	{
		dataWindow.push_back(data);
		return data.value;
	}
	if (dataWindow.front().index <= data.index - windowSize)
		dataWindow.pop_front();		// out of date
	if (dataWindow.front().value >= data.value)
	{
		while (dataWindow.back().value < data.value)
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
	return data.value;
}

template<class T>
T MaxQuery<T>::getCurMax()
{
	return dataWindow.front().value;
} 

template<class T>
T MaxQuery<T>::update_with_new_value(T data)
{
	addNewData(data);
	return getCurMax();
}


#endif
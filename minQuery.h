#ifndef MINQUERY_H_H
#define MINQUERY_H_H

#include "query.h"
#include <deque>          // std::queue


const int MINQUERY_WINDOWSIZE = 500;

template <class T>
class MinQuery:public Query<T>
{
	private:
		struct myData
		{
			T value;
			int index;
		};
		int size;
		int index;
		T min_value;
		T max_value;
		std::deque<myData> dataWindow;
		int windowSize;
		double addNewData(T data_);
		bool checkDataValid(T data_);
		T getCurMin();
	public:
		MinQuery();
		MinQuery(int windowSize_);
		MinQuery(int windowSize_, int min_value_, int max_value);
		MinQuery(const MinQuery<T>& MQ);
		MinQuery(MinQuery<T>&& MQ);
		~MinQuery();
		T update_with_new_value(T data);
		MinQuery & operator=(const MinQuery &);

};

template<class T>
MinQuery<T> & MinQuery<T>:: operator=(const MinQuery &MQ)
{
	size = MQ.size;
	index = MQ.index;
	windowSize = MQ.windowSize;
	dataWindow = MQ.dataWindow;
	return *this;
}


template<class T>
MinQuery<T>::MinQuery(MinQuery<T>&& MQ)
{
	size = MQ.size;
	index = MQ.index;
	windowSize = MQ.windowSize;
	dataWindow = MQ.dataWindow;
}

template<class T>
bool MinQuery<T>::checkDataValid(T data_)
{

	if (data_ <= max_value && data_ >= min_value)
		return true;	
	else 
		return false;
}




template<class T>
MinQuery<T>::~MinQuery()
{
	min_value = std::numeric_limits<T>::lowest();
	max_value = std::numeric_limits<T>::max();
}

template<class T>
MinQuery<T>::MinQuery(const MinQuery<T>& MQ)
{
	size = MQ.size;
	index = MQ.index;
	windowSize = MQ.windowSize;
	dataWindow = MQ.dataWindow;
}

template<class T>
MinQuery<T>::MinQuery(int windowSize_)
{
	size = 0;
	index = 0;
	min_value = std::numeric_limits<T>::lowest();
	max_value = std::numeric_limits<T>::max();
	windowSize = windowSize_;
}

template<class T>
MinQuery<T>::MinQuery(int windowSize_, int min_value_, int max_value_)
{
	size = 0;
	index = 0;
	min_value = min_value_;
	max_value = max_value_;
	windowSize = windowSize_;
}

template<class T>
MinQuery<T>::MinQuery()
{
	size = 0;
	index = 0;
	windowSize = MINQUERY_WINDOWSIZE;
}

template<class T>
double MinQuery<T>::addNewData(T value)
{
	index++;
	struct myData data;
	data.value = value;
	data.index = index;
	if (dataWindow.empty())
	{
		if (checkDataValid(value) == false)			//data invalid, no operation
			return std::numeric_limits<T>::max();
		dataWindow.push_back(data);					//if empty window, push data directly
		return data.value;
	}
	if (dataWindow.front().index <= data.index - windowSize)	
		dataWindow.pop_front();		// out of date

	if (checkDataValid(value) == false)			//data invalid, no operation
		return std::numeric_limits<T>::max();

	if (dataWindow.front().value <= data.value)		
	{
		while (dataWindow.back().value > data.value)
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
T MinQuery<T>::getCurMin()
{
	if (!dataWindow.empty())
		return dataWindow.front().value;
	else
	{
		printf("no data available\n");
		return numeric_limits<T>::max();
	}
} 

template<class T>
T MinQuery<T>::update_with_new_value(T data)
{
	addNewData(data);
	return getCurMin();
}


#endif
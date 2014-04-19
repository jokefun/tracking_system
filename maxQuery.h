#ifndef MAXQUERY_H_H
#define MAXQUERY_H_H

#include "query.h"
#include <deque>          // std::queue

// WHy do you need to make the window size a constant? (particularly for this query)
// ALso, since this constant is specific to MaxQuery, it should be a static const member or MaxQuery
// Note that such members are usually not made all caps in C++
const int WINDOWSIZE = 500;

// Note that if you paramaterize the class by an ordering relation, this class can provide both maximum and minimum queries.
template <class T>
class MaxQuery:public Query<T>
{
	//document your class members; I have no idea what they mean
	private:
		struct myData
		{
			T value;
			// why are your indices ints? e.g. why are they signed?
			int index;
		};
		int size;
		int index;
		// document your class invariant, i.e. the invariant describing how dataWindow summarizes the data in the window
		std::deque<myData> dataWindow;
		// why is this an int instead of a size_t?
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
	// why aren't you moving the dataWindow? It might have substantial state
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
	// This is C++ - struct is redundant below
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
	// there's no need to special case with this conditional; 
	// just use the following loop. The special case is the sort
	// of optimization you should only do if you have reason to 
	// think it will actually matter.
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
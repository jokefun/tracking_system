#ifndef MAXQUERY_H_H
#define MAXQUERY_H_H

#include "query.h"
#include <deque>          // std::queue

// WHy do you need to make the window size a constant? (particularly for this query)
// ALso, since this constant is specific to MaxQuery, it should be a static const member or MaxQuery
// Note that such members are usually not made all caps in C++
//default window size, if not specified
const int WINDOWSIZE = 500;

// Note that if you paramaterize the class by an ordering relation, this class can provide both maximum and minimum queries.

/*
this class has one public method for client, update_with_new_value()
client only need to provide data for this method and then a current 
maximum value in the sliding window will be returned. 


MaxQuery();		using default settings
MaxQuery(int windowSize_);		specify window size
MaxQuery(int windowSize_, T min_value_, T max_value);	specify window size/ min valid value and max valid value for data

time complexity: o(1) insert, o(1) query maximum in average.
*/
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
		T min_value;
		T max_value;
		// document your class invariant, i.e. the invariant describing how dataWindow summarizes the data in the window
		std::deque<myData> dataWindow;
		// why is this an int instead of a size_t?
		int windowSize;
		T addNewData(T data_);
		bool checkDataValid(T data_);
		T getCurMax();
	public:
		MaxQuery();
		MaxQuery(int windowSize_);		
		MaxQuery(int windowSize_, T min_value_, T max_value);
		MaxQuery(const MaxQuery<T>& MQ);
		MaxQuery(MaxQuery<T>&& MQ);
		~MaxQuery();
		T update_with_new_value(T data);		//add a new data to current data window, and update the maximum value of the window
		MaxQuery & operator=(const MaxQuery &);

};


template<class T>
MaxQuery<T>::MaxQuery()
{
	size = 0;
	index = 0;
	min_value = std::numeric_limits<T>::lowest();
	max_value = std::numeric_limits<T>::max();
	windowSize = WINDOWSIZE;
}


template<class T>
MaxQuery<T>::MaxQuery(int windowSize_)
{
	size = 0;
	index = 0;
	min_value = std::numeric_limits<T>::lowest();
	max_value = std::numeric_limits<T>::max();
	windowSize = windowSize_;
}

template<class T>
MaxQuery<T>::MaxQuery(int windowSize_, T min_value_, T max_value_)
{
	size = 0;
	index = 0;
	min_value = min_value_;
	max_value = max_value_;
	windowSize = windowSize_;
}


template<class T>
MaxQuery<T>::MaxQuery(const MaxQuery<T>& MQ)
{
	size = MQ.size;
	index = MQ.index;
	min_value = MQ.min_value;
	max_value = MQ.max_value;
	windowSize = MQ.windowSize;
	dataWindow = MQ.dataWindow;
}

template<class T>
MaxQuery<T>::MaxQuery(MaxQuery<T>&& MQ):
size(MQ.size),index(MQ.index),min_value(MQ.min_value),max_value(MQ.max_value),windowSize(MQ.windowSize),dataWindow(std::move(MQ.dataWindow))
{
//	cout<<"move constructor"<<endl;
	/*
	size = MQ.size;
	index = MQ.index;
	min_value = MQ.min_value;
	max_value = MQ.max_value;
	windowSize = MQ.windowSize;
	dataWindow = std::move(MQ.dataWindow);
	*/
}


template<class T>
MaxQuery<T> & MaxQuery<T>:: operator=(const MaxQuery &MQ)
{
	size = MQ.size;
	index = MQ.index;
	min_value = MQ.min_value;
	max_value = MQ.max_value;
	windowSize = MQ.windowSize;
	dataWindow = MQ.dataWindow;
	return *this;
}


template<class T>
bool MaxQuery<T>::checkDataValid(T data_)
{

	if (data_ <= max_value && data_ >= min_value)
		return true;	
	else 
		return false;
}



template<class T>
T MaxQuery<T>::addNewData(T value)
{
//	index++;
	// This is C++ - struct is redundant below
	struct myData data;
	data.value = value;
	data.index = index;
	int expiredIndex = index;
	index = (index+1)%windowSize;   //circular index, avoid overflow

	if (!(dataWindow.empty()))
	{
		if (dataWindow.front().index == expiredIndex)
			dataWindow.pop_front();		// out of date
	}

	if (dataWindow.empty())
	{
		if (checkDataValid(value) == false)			//data invalid, do not add add such data 
			return std::numeric_limits<T>::lowest();
		dataWindow.push_back(data);
		return data.value;
	}
/*	if (dataWindow.front().index <= data.index - windowSize)
		dataWindow.pop_front();		// out of date
*/
	// there's no need to special case with this conditional; 
	// just use the following loop. The special case is the sort
	// of optimization you should only do if you have reason to 
	// think it will actually matter.

	if (checkDataValid(value) == false)			//data invalid, no operation
		return std::numeric_limits<T>::lowest();

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
	if (!dataWindow.empty())
		return dataWindow.front().value;
	else
	{
		printf("no data available\n");
		return numeric_limits<T>::lowest();
	}
} 

template<class T>
T MaxQuery<T>::update_with_new_value(T data)
{
	addNewData(data);
	return getCurMax();
}



template<class T>
MaxQuery<T>::~MaxQuery()
{

}


#endif
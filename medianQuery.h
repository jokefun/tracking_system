#pragma once

/* #ifndef MEDIANQUERY_H_H */
/* #define MEDIANQUERY_H_H */

#include "query.h"
#include <map>

//default window size, if not specified
const int MEDIANQUERY_WINDOWSIZE = 500;
/*
this class has one public method for client, update_with_new_value()
client only need to provide data for this method and then a current 
median value in the sliding window will be returned. 

time complexity: o(logn) insert, o(logn) query median.
*/

template <class T>
class MedianQuery: public Query<T>
{
	private:
		struct myData
		{
			T value;
			int index;
		};
		int windowSize;
		int index;
		T minValue;
		T maxValue;
		T* cachedValue; 
		std::multimap<T, int> fhWindow;
		std::multimap<T, int> shWindow;
		T addNewData(T data_);
		bool checkDataValid(T data_);
		void deleteElement(int index);
		void insertElement(int index, T value);
		void balanceWindow();
		T getCurMedian();
		
	public:
		MedianQuery();
		MedianQuery(int windowSize_);		
		MedianQuery(int windowSize_, T min_value_, T max_value);
		MedianQuery(const MedianQuery<T>& MQ);
		MedianQuery(MedianQuery<T>&& MQ);
		~MedianQuery();
		T update_with_new_value(T data);		//add a new data to current data window, and update the median value of the window
		MedianQuery & operator=(const MedianQuery &);
		
};

template<class T>
MedianQuery<T>::MedianQuery()
{
	index = 0;
	min_value = std::numeric_limits<T>::lowest();
	max_value = std::numeric_limits<T>::max();
	windowSize = MEDIANQUERY_WINDOWSIZE;
	cachedValue = new T[windowSize];
}

template<class T>
MedianQuery<T>::MedianQuery(int windowSize_)
{
	index = 0;
	min_value = std::numeric_limits<T>::lowest();
	max_value = std::numeric_limits<T>::max();
	windowSize = windowSize_;
	cachedValue = new T[windowSize];
}

template<class T>
MedianQuery<T>::MedianQuery(int windowSize_, T min_value_, T max_value_)
{
	index = 0;
	min_value = min_value_;
	max_value = max_value_;
	windowSize = windowSize_;
	cachedValue = new T[windowSize];
}

template<class T>
MedianQuery<T>::MedianQuery(const MedianQuery<T>& MQ)
{
	index = MQ.index;
	min_value = MQ.min_value;
	max_value = MQ.max_value;
	windowSize = MQ.windowSize;
	fhWindow = MQ.fhWindow;
	shWindow = MQ.shWindow;
	cachedValue = new T[windowSize];
	for (int i = 0; i < windowSize; i++)
	{
		cachedValue[i] = MQ.cachedValue[i];
	}
}

template<class T>
MedianQuery<T>::MedianQuery(MedianQuery<T>&& MQ):
index(MQ.index),min_value(MQ.min_value),max_value(MQ.max_value),windowSize(MQ.windowSize),
fhWindow(std::move(MQ.fhWindow)),shWindow(std::move(MQ.shWindow)),cachedValue(std::move(MQ.cachedValue))
{
	MQ.cachedValue = NULL;
}

template<class T>
MedianQuery<T> & MedianQuery<T>::operator=(const MedianQuery<T> &MQ)
{
	index = MQ.index;
	min_value = MQ.min_value;
	max_value = MQ.max_value;
	windowSize = MQ.windowSize;
	fhWindow = MQ.fhWindow;
	shWindow = MQ.shWindow;
	cachedValue = new T[windowSize];
	for (int i = 0; i < windowSize; i++)
	{
		cachedValue[i] = MQ.cachedValue[i];
	}
}


template<class T>
bool MedianQuery<T>::checkDataValid(T data_)
{

	if (data_ <= max_value && data_ >= min_value)
		return true;	
	else 
		return false;
}

template<class T>
void MedianQuery<T>::insertElement(int index, T value)
{
	if (checkDataValid(value) == false)
		return;
	std::pair<T, int> p(value, index);

	cachedValue[index] = value;

//	std::multimap<T, int>::iterator it1 = std::prev(shWindow.end());
	std::multimap<T, int>::iterator it2 = shWindow.begin();
	if (it2 == shWindow.end() || (*it2).first >= value)
		fhWindow.insert(p);
	else
		shWindow.insert(p);
	balanceWindow();
}
// delete an expired data from window
template<class T>
void MedianQuery<T>::deleteElement(int index) 
{
	//size1 >= size2
	int size1 = fhWindow.size();
	int size2 = shWindow.size();
	if (size1 == 0)
		return;
	T value = cachedValue[index];

	std::multimap<T, int>::iterator it = fhWindow.find(value);
	if (it != fhWindow.end())
	{
		std::multimap<T, int>::iterator it1 = fhWindow.lower_bound(value);
		std::multimap<T, int>::iterator it2 = fhWindow.upper_bound(value);
		while (it1 != it2)
		{ 
			if (it1 != fhWindow.end() && (*it1).second != index)
				std::advance (it1,1);
			else
				break;
		}
		if (it1 != fhWindow.end())
			fhWindow.erase (it1);
	}
	it = shWindow.find(value);
	if (it != shWindow.end())
	{
		std::multimap<T, int>::iterator it1 = shWindow.lower_bound(value);
		std::multimap<T, int>::iterator it2 = shWindow.upper_bound(value);
		while (it1 != it2)
		{ 
			
			if (it1 != shWindow.end() &&(*it1).second != index)
				std::advance (it1,1);
			else
				break;
		}
		if (it1 != shWindow.end())
			shWindow.erase (it1);  //at most one will succeed
	}
	balanceWindow();
	

}

template<class T>
void MedianQuery<T>::balanceWindow()
{

	int size1 = fhWindow.size();
	int size2 = shWindow.size();
	//size1 >= size2
	if (size1 == size2 || size1 == size2+1)
		return;
	if (size1 == size2 - 1)
	{
		std::multimap<T, int>::iterator it = shWindow.begin();
		std::pair<T, int> p((*it).first, (*it).second);
		fhWindow.insert(p);
		shWindow.erase(it);
	}
	else if (size1 == size2 + 2)
	{
		std::multimap<T, int>::iterator it = std::prev(fhWindow.end());
		std::pair<T, int> p((*it).first, (*it).second);
		shWindow.insert(p);
		fhWindow.erase(it);
	}
	return;
}


template<class T>
T MedianQuery<T>::update_with_new_value(T data)
{
	addNewData(data);
	return getCurMedian();
}


template<class T>
T MedianQuery<T>::addNewData(T value)
{
	int expiredIndex = index;
	if (checkDataValid(value) == false)
	{
		
		deleteElement(expiredIndex);
		//data invalid, do not add add such data 
		return std::numeric_limits<T>::lowest();
	}
	deleteElement(expiredIndex);
	insertElement(index, value);
	index = (index + 1)%windowSize;
	return value;
}

template<class T>
T MedianQuery<T>::getCurMedian()
{
	// size1 >= size2
	int size1 = fhWindow.size();
	int size2 = shWindow.size();

	if (size1 == 0)
		return std::numeric_limits<T>::lowest();
	if (size1 == size2)
	{
		std::multimap<T,int>::iterator i1 = std::prev(fhWindow.end());
		std::multimap<T,int>::iterator i2 = shWindow.begin();
		return ((*i1).first+(*i2).first)/2;
	}
	else 
	{
		std::multimap<T,int>::iterator i1 = std::prev(fhWindow.end());
		return (*i1).first;
	}
	
}

template<class T>
MedianQuery<T>::~MedianQuery()
{
	if (cachedValue != NULL)
		delete []cachedValue;
}

/* #endif */

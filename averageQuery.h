#ifndef AVERAGEQUERY_H_H
#define AVERAGEQUERY_H_H

#include <deque>
#include <limits>
#include "query.h"

const int WINDOW_WIDTH = 500;
const double UPPERBOUND numeric_limits<double>::max();
const double LOWERBOUND numeric_limits<double>::lowest();

// You should try to document the requirements on class T
template <class T>
class AverageQuery:public Query<T>
{
private:
	// This sum might overflow, right? How can you do the computation to guarantee the absence of overlow?
    T windowSum;
    int dataCnt;
    int windowWidth;
    T upperBound;
    T lowerBound;
    std::deque<T> curDataWindow;
    int addDataToWindow(T newData);
    T getAverage();
public:
    AverageQuery();
    AverageQuery(int windowWidth_);
    AverageQuery(int windowWidth_, T upperBound_, T lowerBound_);
    ~AverageQuery();
    T update_with_new_value(T newData);
};


template <class T>
AverageQuery<T>::AverageQuery()
{
    windowSum = 0;
    dataCnt = 0;
    windowWidth = WINDOW_WIDTH;
    upperBound = UPPERBOUND;
    lowerBound = LOWERBOUND;
}

template <class T>
AverageQuery<T>::AverageQuery(int windowWidth_)
{
    windowSum = 0;
    dataCnt = 0;
    windowWidth = windowWidth_;
    upperBound = UPPERBOUND;
    lowerBound = LOWERBOUND;
}

template <class T>
AverageQuery<T>::AverageQuery(int windowWidth_, T upperBound_, T lowerBound_)
{
    windowSum = 0;
    dataCnt = 0;
    windowWidth = windowWidth_;
    upperBound = upperBound_;
    lowerBound = lowerBound_;
}


template <class T>
AverageQuery<T>::~AverageQuery()
{
}


template <class T>
int AverageQuery<T>::addDataToWindow(T newData)
{
    if(curDataWindow.size() >= windowWidth)
    {
        if (curDataWindow.front() >= lowerBound) && (curDataWindow.front() <= upperBound)
        {
            windowSum -= curDataWindow.front();
            dataCnt -= 1;
        }
        curDataWindow.pop_front();
        curDataWindow.push_back(newData);
        if (newData >= lowerBound) && (newData <= upperBound)
        {
            windowSum += newData;
            dataCnt += 1;
        }
    }
    else
    {
        curDataWindow.push_back(newData);
        if (newData >= lowerBound) && (newData <= upperBound)
        {
            windowSum += newData;
            dataCnt += 1;
        }
    }

    return 1;
}

template<class T>
T AverageQuery<T>::getAverage()
{
    if dataCnt >= 1
    {
        T ave = windowSum/dataCnt;
        return ave;
    }
    else return 0;
}

template<class T>
T AverageQuery<T>::update_with_new_value(T newData)
{
    addDataToWindow(newData);
    return getAverage();
}

#endif

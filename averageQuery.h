#ifndef MAXQUERY_H_H
#define MAXQUERY_H_H

#include "query.h"
#include <deque>

const int WINDOW_WIDTH = 50;

template <class T>
class AverageQuery:public Query<T>
{
private:
    T windowSum;
    int dataCnt;
    int windowWidth;
    std::deque<T> curDataWindow;
    int addDataToWindow(T newData);
    T getAverage();
public:
    AverageQuery();
    AverageQuery(int windowWidth_);
    ~AverageQuery();
    T update_with_new_value(T newData);
};


template <class T>
AverageQuery<T>::AverageQuery()
{
    windowSum = 0;
    dataCnt = 0;
    windowWidth = WINDOW_WIDTH;
}

template <class T>
AverageQuery<T>::AverageQuery(int windowWidth_)
{
    windowSum = 0;
    dataCnt = 0;
    windowWidth = windowWidth_;
}

template <class T>
AverageQuery<T>::~AverageQuery()
{
}


template <class T>
int AverageQuery<T>::addDataToWindow(T newData)
{
    //if(curDataWindow.empty())
    //{
    //    curDataWindow.push_back(newData);
    //    windowSum += newData;
    //    dataCnt += 1;
    //}

    if(curDataWindow.size() >= windowWidth)
    {
        windowSum -= curDataWindow.pop_front();
        curDataWindow.push_back(newData);
        windowSum += newData;
    }
    else
    {
        curDataWindow.push_back(newData);
        windowSum += newData;
        dataCnt += 1;
    }

    return 1;
}


template<class T>
T AverageQuery<T>::getAverage()
{
    T ave = windowSum/dataCnt;
    return ave;
}

template<class T>
T AverageQuery<T>::update_with_new_value(T newData)
{
    addDataToWindow(newData);
    return getAverage();
}

#endif

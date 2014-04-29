#ifndef STANDARDDEVIATIONQUERY_H_INCLUDED
#define STANDARDDEVIATIONQUERY_H_INCLUDED

#include <deque>
#include <limits>
#include "query.h"

using namespace std;

const int WINDOW_WIDTH = 500;
const double UPPERBOUND = numeric_limits<double>::max();
const double LOWERBOUND = numeric_limits<double>::lowest();

template <class T>
class StandardDeviationQuery:public Query<T>
{
private:
    T windowSum;
    T windowSquareSum;
    int dataCnt;
    int windowWidth;
    T upperBound;
    T lowerBound;
    std::deque<T> curDataWindow;
    int addDataToWindowSTD(T newData);
    T getStandardDeviation();
public:
    StandardDeviationQuery();
    StandardDeviationQuery(int windowWidth_);
    StandardDeviationQuery(int windowWidth_, T upperBound_, T lowerBound_);
    ~StandardDeviationQuery();
    T update_with_new_value(T newData);
};

template <class T>
StandardDeviationQuery<T>::StandardDeviationQuery()
{
    windowSum = 0;
    windowSquareSum = 0;
    dataCnt = 0;
    windowWidth = WINDOW_WIDTH;
    upperBound = UPPERBOUND;
    lowerBound = LOWERBOUND;
}

template <class T>
StandardDeviationQuery<T>::StandardDeviationQuery(int windowWidth_)
{
    windowSum = 0;
    windowSquareSum = 0;
    dataCnt = 0;
    windowWidth = windowWidth_;
    upperBound = UPPERBOUND;
    lowerBound = LOWERBOUND;

}

template <class T>
StandardDeviationQuery<T>::StandardDeviationQuery(int windowWidth_, T upperBound_, T lowerBound_)
{
    windowSum = 0;
    windowSquareSum = 0;
    dataCnt = 0;
    windowWidth = windowWidth_;
    upperBound = upperBound_;
    lowerBound = lowerBound_;

}

template <class T>
StandardDeviationQuery<T>::~StandardDeviationQuery()
{

}


template <class T>
int StandardDeviationQuery<T>::addDataToWindowSTD(T newData)
{
    if(curDataWindow.size() >= windowWidth)
    {
        if ((curDataWindow.front() >= lowerBound) && (curDataWindow.front() <= upperBound))
        {
            windowSum -= curDataWindow.front();
            windowSquareSum -= (curDataWindow.front() * curDataWindow.front());
            dataCnt -= 1;
        }
        curDataWindow.pop_front();
        curDataWindow.push_back(newData);
        if ((newData >= lowerBound) && (newData <= upperBound))
        {
            windowSum += newData;
            windowSquareSum += (newData * newData);
            dataCnt += 1;
        }
    }
    else
    {
        curDataWindow.push_back(newData);
        if ((newData >= lowerBound) && (newData <= upperBound))
        {
            windowSum += newData;
            windowSquareSum += (newData * newData);
            dataCnt += 1;
        }
    }

    return 1;

}


template<class T>
T StandardDeviationQuery<T>::getStandardDeviation()
{
    if (dataCnt >= 1)
    {
        T average_val = windowSum/dataCnt;
        T standardDeviation = sqrt((windowSquareSum/dataCnt) - average_val*average_val);
        return standardDeviation;
    }
    else return 0;

}

template<class T>
T StandardDeviationQuery<T>::update_with_new_value(T newData)
{
    addDataToWindowSTD(newData);
    return getStandardDeviation();

}

#endif

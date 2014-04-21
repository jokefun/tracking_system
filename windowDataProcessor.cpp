#include "windowDataProcessor.h"
#include "maxQuery.h"
#include "averageQuery.h"

using namespace std;

WindowDataProcessor::WindowDataProcessor(size_t w_size)
{
    window_size = w_size;
    initQueries();
}

WindowDataProcessor& WindowDataProcessor::operator =(const WindowDataProcessor& wdp)
{
    window_size = wdp.window_size;
    queries = wdp.queries;
    lastValues = wdp.lastValues;
    return *this;
}

WindowDataProcessor& WindowDataProcessor::operator =(WindowDataProcessor&& wdp)
{
    window_size = move(wdp.window_size);
    queries = move(wdp.queries);
    lastValues = move(wdp.lastValues);
    return *this;
}

void WindowDataProcessor::initQueries()
{
    // average
    queries.push_back(unique_ptr<Query<double> >(new AverageQuery<double>(window_size)));
    lastValues.push_back(0);
    // max
    queries.push_back(unique_ptr<Query<double> >(new MaxQuery<double>(window_size)));
    lastValues.push_back(0);
    // max
    // maybe more
}

void WindowDataProcessor::executeAllQueriesForData(double v)
{
    for (int i=0; i<NUM_OF_QUERYTYPES; i++)
    {
        lastValues[i] = queries[i]->update_with_new_value(v);
    }
}

void WindowDataProcessor::addNewData(double v)
{
    executeAllQueriesForData(v);
}

double WindowDataProcessor::getLastAverage()
{
    return lastValues[AVERAGE];
}

double WindowDataProcessor::getLastMax()
{
    return lastValues[MAX];
}

vector<double> WindowDataProcessor::getLastAll()
{
    return lastValues;
}

double WindowDataProcessor::addNewDataAndGetAverage(double v)
{
    executeAllQueriesForData(v);
    return lastValues[AVERAGE];
}

double WindowDataProcessor::addNewDataAndGetMax(double v)
{
    executeAllQueriesForData(v);
    return lastValues[MAX];
}

vector<double> WindowDataProcessor::addNewDataAndGetAll(double v)
{
    executeAllQueriesForData(v);
    return lastValues;
}

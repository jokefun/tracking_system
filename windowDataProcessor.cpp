#include "windowDataProcessor.h"
#include "maxQuery.h"
#include "averageQuery.h"

using namespace std;

void WindowDataProcessor::initQueries()
{
    // average
    queries.push_back(unique_ptr<Query<double> >(new AverageQuery<double>()));
    // max
    queries.push_back(unique_ptr<Query<double> >(new MaxQuery<double>()));
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

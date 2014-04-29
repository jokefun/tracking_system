#include "windowDataProcessor.h"
#include "maxQuery.h"
#include "minQuery.h"
#include "averageQuery.h"

using namespace std;

WindowDataProcessor::WindowDataProcessor(size_t w_size)
    : ds(w_size, NUM_OF_QUERYTYPES)
{
    window_size = w_size;
    initQueries();
    lastInput = 0;
}

WindowDataProcessor& WindowDataProcessor::operator =(const WindowDataProcessor& wdp)
{
    window_size = wdp.window_size;
    ds = wdp.ds;
    queries = wdp.queries;
    lastInput = wdp.lastInput;
    lastValues = wdp.lastValues;
    return *this;
}

WindowDataProcessor& WindowDataProcessor::operator =(WindowDataProcessor&& wdp)
{
    window_size = move(wdp.window_size);
    ds = move(wdp.ds);
    queries = move(wdp.queries);
    lastInput = move(wdp.lastInput);
    lastValues = move(wdp.lastValues);
    return *this;
}

void WindowDataProcessor::initQueries()
{
    /*
     * initialize all queries
     * queries stored in the query vector
     */

    // average
    queries.push_back(unique_ptr<Query<double> >(new AverageQuery<double>(window_size)));
    lastValues.push_back(0);
    // max
    queries.push_back(unique_ptr<Query<double> >(new MaxQuery<double>(window_size)));
    lastValues.push_back(0);
    // min
    queries.push_back(unique_ptr<Query<double> >(new MinQuery<double>(window_size)));
    lastValues.push_back(0);
    // maybe more
}

void WindowDataProcessor::executeAllQueriesForData(double v)
{
    /*
     * do all queries for the new data
     * invariant: lastValue[i] == queries[i]->update_with_new_value(lastInput)
     *            ds.last = {v, lastValues[...]}
     */
    lastInput = v;
    vector<double> vt;
    vt.push_back(v);
    for (int i=0; i<NUM_OF_QUERYTYPES; i++)
    {
        lastValues[i] = queries[i]->update_with_new_value(v);
        vt.push_back(lastValues[i]);
    }
    ds.addNewData(vt);
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

double WindowDataProcessor::getLastMin()
{
    return lastValues[MIN];
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

double WindowDataProcessor::addNewDataAndGetMin(double v)
{
    executeAllQueriesForData(v);
    return lastValues[MIN];
}

vector<double> WindowDataProcessor::addNewDataAndGetAll(double v)
{
    executeAllQueriesForData(v);
    return lastValues;
}

vector<vector<double> > WindowDataProcessor::retrieveData(string start, string end)
{
    return ds.retrieveData(start, end);
}

vector<vector<double> > WindowDataProcessor::retrieveData(time_point s, time_point e)
{
    return ds.retrieveData(s, e);
}

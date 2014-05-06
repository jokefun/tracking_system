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
    customQueries = wdp.customQueries;
    customQueries_count = wdp.customQueries_count;
    return *this;
}

WindowDataProcessor& WindowDataProcessor::operator =(WindowDataProcessor&& wdp)
{
    window_size = move(wdp.window_size);
    ds = move(wdp.ds);
    queries = move(wdp.queries);
    lastInput = move(wdp.lastInput);
    lastValues = move(wdp.lastValues);
    customQueries = move(wdp.customQueries);
    customQueries_count = move(wdp.customQueries_count);
    return *this;
}

void WindowDataProcessor::initQueries()
{
    /*
     * initialize all queries
     * queries stored in the query vector
     */

    // average
    queries.push_back(shared_ptr<Query<double> >(new AverageQuery<double>(window_size)));
    // max
    queries.push_back(shared_ptr<Query<double> >(new MaxQuery<double>(window_size)));
    // min
    queries.push_back(shared_ptr<Query<double> >(new MinQuery<double>(window_size)));
    // maybe more

    customQueries_count = 0;
}

void WindowDataProcessor::executeAllQueriesForData(double v)
{
    /*
     * do all queries for the new data
     * invariant: lastValue[i] == queries[i]->update_with_new_value(lastInput)
     *            ds.last = {v, lastValues[...]}
     */
    lastInput = v;
    lastValues.clear();

    // for default queries
    for (int i=0; i<NUM_OF_QUERYTYPES; i++)
    {
        lastValues.push_back(queries[i]->update_with_new_value(v));
    }

    // for custom queries
    for (int i=0; i<customQueries_count; i++)
    {
        lastValues.push_back(customQueries[i]->update_with_new_value(v));
    }

    // save to storage
    vector<double> vt(lastValues);
    vt.push_back(v);

    ds.addNewData(vt);
}

void WindowDataProcessor::addNewData(double v)
{
    executeAllQueriesForData(v);
}

double WindowDataProcessor::getLastAverage() const
{
    return lastValues[AVERAGE];
}

double WindowDataProcessor::getLastMax() const
{
    return lastValues[MAX];
}

double WindowDataProcessor::getLastMin() const
{
    return lastValues[MIN];
}

vector<double> WindowDataProcessor::getLastAll() const
{
    return lastValues;
}

double WindowDataProcessor::getLastCustom(size_t i) const
{
    if (i < customQueries_count)
    {
        return lastValues[NUM_OF_QUERYTYPES+i];
    }
    else
    {
        cout << "error: getLastCustom, index out of range\n";
        return 0;
    }
}

void WindowDataProcessor::addCustomQuery(shared_ptr<Query<double> > q)
{
    // add ptr to vector
    customQueries.push_back(q);
    // +1 to count to preserve invariant
    customQueries_count++;
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

vector<vector<double> > WindowDataProcessor::retrieveData(string start, string end) const
{
    return ds.retrieveData(start, end);
}

vector<vector<double> > WindowDataProcessor::retrieveData(time_point s, time_point e) const
{
    return ds.retrieveData(s, e);
}

#ifndef DATAPROCESSOR_D
#define DATAPROCESSOR_D

#include <iostream>
#include <memory>
#include <vector>

#include "query.h"
#include "dataStorage.h"

#define DEFAULT_WINDOW_SIZE_PROCESSOR 500

using namespace std;

/*
 * Processor is a wrapper for all kind of queries
 * Can be init with given window size
 * Will execute all queries at once when given a new data
 */
class WindowDataProcessor
{
    private:
        size_t window_size;

        // all queries
        vector<shared_ptr<Query<double> > > queries;

        // temp statistics
        /* \A QueryType qt ==> lastValues[qt] == queries[qt].updateWithValue(lastInput) */
        /* \A size_t t: 0<=t<customQueries_count ==>
            lastInput[t+NUM_OF_QUERYTYPES] == customQueries[t].updateWithValue(lastInput) */
        double lastInput;
        vector<double> lastValues;

        enum QueryType
        {
            AVERAGE = 0,
            MAX,
            MIN,
            NUM_OF_QUERYTYPES
        };

        // custom queries
        /* customQueries.size() == customQueries_count */
        vector<shared_ptr<Query<double> > > customQueries;
        size_t customQueries_count;

        // storage
        DataStorage ds;

    private:
        void initQueries();
        void executeAllQueriesForData(double);

    public:
        WindowDataProcessor(size_t w_size);
        WindowDataProcessor() { WindowDataProcessor(DEFAULT_WINDOW_SIZE_PROCESSOR); }
        // copy
        WindowDataProcessor(const WindowDataProcessor& wdp) :
            window_size(wdp.window_size),
            ds(window_size, NUM_OF_QUERYTYPES),
            queries(wdp.queries),
            lastInput(wdp.lastInput),
            lastValues(wdp.lastValues),
            customQueries(wdp.customQueries),
            customQueries_count(wdp.customQueries_count) {}
        // move
        WindowDataProcessor(WindowDataProcessor&& wdp) :
            window_size(move(wdp.window_size)),
            ds(window_size, NUM_OF_QUERYTYPES),
            queries(move(wdp.queries)),
            lastInput(move(wdp.lastInput)),
            lastValues(move(wdp.lastValues)),
            customQueries(move(wdp.customQueries)),
            customQueries_count(move(wdp.customQueries_count)) {}

        ~WindowDataProcessor() {} // release queries

        WindowDataProcessor& operator =(const WindowDataProcessor& wdp);
        WindowDataProcessor& operator =(WindowDataProcessor&& wdp);

        /*
         * call to feed processor with new data
         * all queries will be executed
         */
        void addNewData(double); // new data, as well as new statistics, are added to storage

        /*
         * call to get the results for last input data
         */
        double getLastAverage();
        double getLastMax();
        double getLastMin();
        vector<double> getLastAll();

        /*
         * get one of the result for custom queries
         * i starts from 0
         * will return 0 if out of range
         */
        double getLastCustom(size_t i);

        void addCustomQuery(shared_ptr<Query<double> >);

        /*
         * call to add new data and get corresponding result as well
         */
        double addNewDataAndGetAverage(double);
        double addNewDataAndGetMax(double);
        double addNewDataAndGetMin(double);
        vector<double> addNewDataAndGetAll(double);

        /*
         * query in data storage for datas in given period
         */
        vector<vector<double> > retrieveData(string start, string end);
        vector<vector<double> > retrieveData(time_point s, time_point e);
};
#endif

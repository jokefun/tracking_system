#ifndef DATAPROCESSOR_D
#define DATAPROCESSOR_D

#include <iostream>
#include <memory>
#include <vector>

#include "query.h"

#define DEFAULT_WINDOW_SIZE 500

using namespace std;

class WindowDataProcessor
{
    private:
        size_t window_size;
        // TODO storage

        vector<shared_ptr<Query<double> > > queries;

        /* temp statistics */
        vector<double> lastValues;

        // average handler
        // max handler

    public:
        enum QueryType
        {
            AVERAGE = 0,
            MAX,
            NUM_OF_QUERYTYPES
        };

    private:
        void initQueries();
        void executeAllQueriesForData(double);

    public:
        WindowDataProcessor(size_t w_size);
        WindowDataProcessor() { WindowDataProcessor(DEFAULT_WINDOW_SIZE); }
        // copy
        WindowDataProcessor(const WindowDataProcessor& wdp) :
            window_size(wdp.window_size),
            queries(wdp.queries),
            lastValues(wdp.lastValues) {}
        // move
        WindowDataProcessor(WindowDataProcessor&& wdp) :
            window_size(move(wdp.window_size)),
            queries(move(wdp.queries)),
            lastValues(move(wdp.lastValues)) {}

        ~WindowDataProcessor() {} // release queries

        WindowDataProcessor& operator =(const WindowDataProcessor& wdp);
        WindowDataProcessor& operator =(WindowDataProcessor&& wdp);

        void addNewData(double); // new data, as well as new statistics, are added to storage

        double getLastAverage();
        double getLastMax();
        vector<double> getLastAll();

        double addNewDataAndGetAverage(double);
        double addNewDataAndGetMax(double);
        vector<double> addNewDataAndGetAll(double);

        // set handler for statistics
        // void setHandlerForAverage();
        // void setHandlerForMax();
};
#endif

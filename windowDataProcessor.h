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

        vector<unique_ptr<Query<double> > > queries;

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
        WindowDataProcessor(size_t w_size) : window_size(w_size) {}
        WindowDataProcessor() { WindowDataProcessor(DEFAULT_WINDOW_SIZE); }

        ~WindowDataProcessor() {} // release queries

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

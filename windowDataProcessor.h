#ifndef DATAPROCESSOR_D
#define DATAPROCESSOR_D

#include <iostream>
#include <vector>

#include "query.h"
#include "maxQuery.h"
#include "averageQuery.h"

#define DEFAULT_WINDOW_SIZE 500

using namespace std;

class WindowDataProcessor
{
    private:
        size_t window_size;
        // TODO storage

        vector< Query<double>* > queries;


        /* temp statistics */
        double lastAverage = 0;
        double lastMax = 0;
        double lastMin = 0;

        // average handler
        // max handler
        // min handler


        void initQueries();
        void executeAllQueries();

    public:
        WindowDataProcessor(int w_size);
        WindowDataProcessor() { WindowDataProcessor(DEFAULT_WINDOW_SIZE); }

        void addNewData(double); // new data, as well as new statistics, are added to storage

        double addNewDataAndGetAverage(double);
        double addNewDataAndGetMax(double);
        double addNewDataAndGetMin(double);
        double addNewDataAndGetAll(double);

        double getLastAverage();
        double getLastMax();
        double getLastMin();

        // set handler for statistics
        // void setHandlerForAverage();
        // void setHandlerForMax();
        // void setHandlerForMin();
};
#endif

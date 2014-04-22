#ifndef DATASTORAGE_D
#define DATASTORAGE_D

#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <chrono>
#include <algorithm>

#define DEFAULT_WINDOW_SIZE 50
#define DEFAULT_NUMBER_STATISTICS 2
#define DEFAULT_WRITE_BUFFER_SIZE 10000


using namespace std;

typedef chrono::system_clock::time_point time_point;

class DataStorage
{
    private:
        struct DataWithTime
        {
            vector<double> data;
            time_point timestamp;
            DataWithTime(vector<double> d, time_point t)
            {
                data = d;
                timestamp = t;
            }
            DataWithTime(const DataWithTime& dwt)
                : data(dwt.data), timestamp(dwt.timestamp) {}
            ~DataWithTime() {}
        };

        size_t window_size;
        size_t number_statistics;
        vector<DataWithTime> current_window;
        vector<DataWithTime> write_buffer_database;

    public:
        // constructors
        DataStorage(size_t w_size, size_t n_statistics) :
            window_size(w_size),
            number_statistics(n_statistics) {}
        DataStorage(size_t w_size) :
            window_size(w_size),
            number_statistics(DEFAULT_NUMBER_STATISTICS) {}
        DataStorage() :
            DataStorage(DEFAULT_WINDOW_SIZE) {}

        DataStorage(const DataStorage& d) :
            window_size(d.window_size),
            number_statistics(d.number_statistics),
            current_window(d.current_window),
            write_buffer_database(d.write_buffer_database) {}
        DataStorage(DataStorage&& d) :
            window_size(move(d.window_size)),
            number_statistics(move(d.number_statistics)),
            current_window(move(d.current_window)),
            write_buffer_database(move(d.write_buffer_database)) {}
        // destructor
        ~DataStorage() {}

        // assign overwrite
        DataStorage& operator =(const DataStorage& d);
        DataStorage& operator =(DataStorage&& d);

        // interfaces
        void addNewData(vector<double> d);
        vector<vector<double> > retrieveData(string start, string end);
        vector<vector<double> > retrieveData(time_point s, time_point e);

    private:
        time_point stringToTimePoint(string ts);
        void addNewData(vector<double> d, time_point t);

        /* bool dataInMemory(time_point s, time_point e); // return true is all in memory */
};


#endif

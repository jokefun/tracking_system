
#include <cassert>

#include "dataStorage.h"

using namespace std;

void printV(vector<double> v)
{
     cout << endl;
     cout << endl;
     for (vector<double>::iterator i = v.begin(); i != v.end(); i++)
     {
         cout << *i << ", ";
     }
     cout << endl;
     cout << endl;
}

DataStorage& DataStorage::operator =(const DataStorage& d)
{
    window_size = d.window_size;
    number_statistics = d.number_statistics;
    current_window = d.current_window;
    write_buffer_database = d.write_buffer_database;
    return *this;
}

DataStorage& DataStorage::operator =(DataStorage&& d)
{
    window_size = move(d.window_size);
    number_statistics = move(d.number_statistics);
    current_window = move(d.current_window);
    write_buffer_database = move(d.write_buffer_database);
    return *this;
}

void DataStorage::addNewData(vector<double> d)
{
    chrono::system_clock::time_point tp = chrono::system_clock::now();
    addNewData(d, tp);
}

void DataStorage::addNewData(vector<double> d, time_point t)
{
    // may need to lock here for multi thread, so all data are sorted by time
    current_window.push_back(DataWithTime(d, t));

    if (current_window.size() > window_size)
    {
        write_buffer_database.push_back(current_window.front());
        // pop here, since current_window will never be empty here, directly erase here
        assert(!current_window.empty());
        current_window.erase(current_window.begin());
    }

    // if write buffer is full, write to database and clear
    /* if (write_buffer_database.size() >= DEFAULT_WRITE_BUFFER_SIZE) */
    /* { */
    /*     // TODO write to database */
    /*     // write_buffer_database.clear(); */
    /* } */
}

vector<vector<double> > DataStorage::retrieveData(string start, string end)
{
    time_point s = stringToTimePoint(start);
    time_point e = stringToTimePoint(end);
    return retrieveData(s, e);
}

vector<vector<double> > DataStorage::retrieveData(time_point s, time_point e)
{
    vector<vector<double> > retriveResult;

    if (s >= e) return retriveResult;

    // for data inside window
    if (!current_window.empty() &&
            current_window.back().timestamp >= s && current_window.front().timestamp <= e)
    {
        vector<DataWithTime>::iterator left = current_window.begin();
        while (left!=current_window.end() && (*left).timestamp<s) left++;

        vector<DataWithTime>::iterator right = current_window.end();
        right--; // start from end--
        while (right>=current_window.begin() && (*right).timestamp>e) right--;

        for (vector<DataWithTime>::iterator i=left; i<=right; i++)
            retriveResult.push_back((*i).data);
    }

    // for data inside window
    if (!write_buffer_database.empty() &&
            write_buffer_database.back().timestamp >= s && write_buffer_database.front().timestamp <= e)
    {
        vector<DataWithTime>::iterator left = write_buffer_database.begin();
        while (left!=write_buffer_database.end() && (*left).timestamp<s) left++;

        vector<DataWithTime>::iterator right = write_buffer_database.end();
        right--; // start from end--
        while (right>=write_buffer_database.begin() && (*right).timestamp>e) right--;

        for (vector<DataWithTime>::iterator i=left; i<=right; i++)
            retriveResult.push_back((*i).data);
    }

    // if start and end are inside the memory
    // copy data from window and return
    // if part of start and end is outsize of memory
    // query in database and return

    return retriveResult;
}

time_point DataStorage::stringToTimePoint(string ts)
{
    struct tm t;
    strptime(ts.c_str(), "%H:%M:%S %m/%d/%Y", &t);
    return chrono::system_clock::from_time_t(mktime(&t));
}



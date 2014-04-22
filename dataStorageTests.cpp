#include <iostream>
#include <chrono>
#include <thread>

#include "dataStorage.h"

using namespace std;

int main()
{
    using namespace std::chrono;

    DataStorage ds(1);

    int x = 1000;

    system_clock::time_point tp0 = system_clock::now();
    time_t t0 = system_clock::to_time_t(tp0);
    ds.addNewData({1,2});
    cout << ctime(&t0);
    std::this_thread::sleep_for(std::chrono::milliseconds(x));

    system_clock::time_point tp1 = system_clock::now();
    time_t t1 = system_clock::to_time_t(tp1);
    ds.addNewData({3,4});
    cout << ctime(&t1);
    std::this_thread::sleep_for(std::chrono::milliseconds(x));

    system_clock::time_point tp2 = system_clock::now();
    time_t t2 = system_clock::to_time_t(tp2);
    ds.addNewData({5,6});
    cout << ctime(&t2);
    std::this_thread::sleep_for(std::chrono::milliseconds(x));

    system_clock::time_point tp3 = system_clock::now();
    time_t t3 = system_clock::to_time_t(tp3);
    ds.addNewData({7,8});
    cout << ctime(&t3);

    vector<vector<double> > r = ds.retrieveData(tp1, tp3);
    cout << r.size() << endl;
    for (vector<vector<double> >:: iterator i = r.begin(); i!=r.end(); i++)
    {
        for (vector<double>::iterator ii=(*i).begin(); ii!=(*i).end(); ii++)
            cout << *ii << ", ";
        cout << endl;
    }

    return 0;
}

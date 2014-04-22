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
    ds.addNewData({1,2}, tp0);
    std::this_thread::sleep_for(std::chrono::milliseconds(x));

    system_clock::time_point tp1 = system_clock::now();
    time_t t1 = system_clock::to_time_t(tp1);
    ds.addNewData({3,4}, tp1);
    std::this_thread::sleep_for(std::chrono::milliseconds(x));

    system_clock::time_point tp2 = system_clock::now();
    time_t t2 = system_clock::to_time_t(tp2);
    ds.addNewData({5,6}, tp2);
    std::this_thread::sleep_for(std::chrono::milliseconds(x));

    system_clock::time_point tp3 = system_clock::now();
    time_t t3 = system_clock::to_time_t(tp3);
    ds.addNewData({7,8}, tp3);
    /* std::this_thread::sleep_for(std::chrono::milliseconds(x)); */

    char b1[20];
    b1[19] = '\0';
    strftime(b1, 19, "%H:%M:%S %m/%d/%Y", localtime(&t1));

    char b2[20];
    b2[19] = '\0';
    strftime(b2, 19, "%H:%M:%S %m/%d/%Y", localtime(&t2));

    struct tm tm;
    strptime(b1, "%H:%M:%S %m/%d/%Y", &tm);
    time_t t = mktime(&tm);  // t is now your desired time_t
    cout << ctime(&t) << endl;

    cout << b1 << " - " << b2 << endl;
    /* vector<vector<double> > r = ds.retrieveData(string(b1),string(b2)); */
    vector<vector<double> > r = ds.retrieveData(tp1 + std::chrono::milliseconds(25), tp2);
    cout << r.size() << endl;
    for (vector<vector<double> >:: iterator i = r.begin(); i!=r.end(); i++)
    {
        for (vector<double>::iterator ii=(*i).begin(); ii!=(*i).end(); ii++)
            cout << *ii << ", ";
        cout << endl;
    }

    //////////////////////////////////////////////////////////////////////
    /* system_clock::time_point tttp = system_clock::now(); */
    /* time_t ttt = system_clock::to_time_t(tttp); */
    /* char tttb[20]; */
    /* tttb[19] = '\0'; */
    /* strftime(tttb, 20, "%H:%M:%S %m/%d/%Y", localtime(&ttt)); */

    /* cout << tttp.time_since_epoch().count() << endl; */
    /* cout << ds.stringToTimePoint(string(tttb)).time_since_epoch().count() << endl; */

    return 0;
}

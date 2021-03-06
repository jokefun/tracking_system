#include <iostream>
#include <random>
#include <vector>
#include <numeric>
#include <algorithm>

#include "windowDataProcessor.h"
#include "averageQuery.h"

#define EPSILON_DOUBLE 0.00001

using namespace std;

// print
void print(const vector<double>& v)
{
    vector<double>::const_iterator i;
    for (i = v.begin(); i != v.end(); ++i)
    {
        cout << *i << " ";
    }
    cout << endl;
}

/*
 * populate random double into vector examples
 */
void populateRandomExample(vector<double>& examples, int count)
{
    random_device generator;
    normal_distribution<double> distribution(5.0,2.0);

    for (int i=0; i<count; ++i) {
        examples.push_back(distribution(generator));
    }
}

void printWhenTestError(string info,
        vector<double>::iterator b, vector<double>::iterator e,
        double fromQuery, double fromTestFunc)
{
    cout << info << endl;
    cout << "data: ";
    for (vector<double>::iterator i=b; i!=e+1; ++i)
        cout << *i << " ";
    cout << "\nquery result: " << fromQuery;
    cout << "\ntestF result: " << fromTestFunc << endl;
}

double testFunctionNewElementQuery(vector<double>::iterator b, vector<double>::iterator e)
{
    return *e;
}

double testFunctionAverage(vector<double>::iterator b, vector<double>::iterator e)
{
    return accumulate(b, e+1, 0.0) / (distance(b,e) + 1);
}

double testFunctionMax(vector<double>::iterator b, vector<double>::iterator e)
{
    return *max_element(b, e+1);
}

double testFunctionMin(vector<double>::iterator b, vector<double>::iterator e)
{
    return *min_element(b, e+1);
}


/*
 * run over examples using query and testFunction
 * to test whether query works as expected
 */
bool doTest(vector<double>& examples, int window_size,
        WindowDataProcessor& processor)
{

    // testing custom queries
    processor.addCustomQuery(shared_ptr<Query<double> >(new AverageQuery<double>(window_size)));

    for (vector<double>::iterator i=examples.begin(); i!=examples.end(); ++i)
    {
        processor.addNewData(*i);

        vector<double>::iterator b; // = i-window_size+1;
        if (distance(examples.begin(), i) < window_size)
            b = examples.begin();
        else
            b = i-window_size+1;
        double fromTestFuncAverage = testFunctionAverage(b, i);
        if (abs(processor.getLastAverage()-fromTestFuncAverage) > EPSILON_DOUBLE)
        {
            printWhenTestError("Error in average --- ",b,i,processor.getLastAverage(),fromTestFuncAverage);
            return false;
        }

        double fromTestFuncMax = testFunctionMax(b, i);
        if (abs(processor.getLastMax()-fromTestFuncMax) > EPSILON_DOUBLE)
        {
            printWhenTestError("Error in max --- ",b,i,processor.getLastMax(),fromTestFuncMax);
            return false;
        }

        double fromTestFuncMin = testFunctionMin(b, i);
        if (abs(processor.getLastMin()-fromTestFuncMin) > EPSILON_DOUBLE)
        {
            printWhenTestError("Error in min --- ",b,i,processor.getLastMin(),fromTestFuncMin);
            return false;
        }

        // testing custom queries
        double fromTestFuncCustom_Average = testFunctionAverage(b, i);
        if (abs(processor.getLastCustom(0)-fromTestFuncAverage) > EPSILON_DOUBLE)
        {
            printWhenTestError("Error in Custom_average --- ",b,i,processor.getLastCustom(0),fromTestFuncAverage);
            return false;
        }
    }
    return true;
}

int main()
{
    /* Query<double>* query[3]; */
    const int count=20;
    const int window_size=4;

    vector<double> examples;
    populateRandomExample(examples, count);
    print(examples);

    WindowDataProcessor p(window_size);

    chrono::system_clock::time_point tp1 = chrono::system_clock::now();

    if (!doTest(examples, window_size, p))
    {
        cout << "test failed\n";
    }
    else
    {
        cout << "test passed\n";
    }

    chrono::system_clock::time_point tp2 = chrono::system_clock::now();

    vector<vector<double> > r = p.retrieveData(tp1, tp2);
    cout << r.size() << endl;
    for (vector<vector<double> >:: iterator i = r.begin(); i!=r.end(); i++)
    {
        for (vector<double>::iterator ii=(*i).begin(); ii!=(*i).end(); ii++)
            cout << *ii << ", ";
        cout << endl;
    }

    return 0;
}



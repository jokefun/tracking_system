#include <iostream>
#include <random>
#include <vector>
#include <numeric>
#include <algorithm>

#include "windowDataProcessor.h"

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

    if (!doTest(examples, window_size, p))
    {
        cout << "test failed\n";
    }
    else
    {
        cout << "test passed\n";
    }

    return 0;
}



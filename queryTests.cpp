#include <iostream>
#include <random>
#include <vector>

#include "query.h"
#include "newElementQuery.h"

using namespace std;

// print
void print(const vector<double>& v)
{
    vector<double>::const_iterator i;
    for (i = v.begin(); i != v.end(); ++i)
    {
        cout << *i << endl;
    }
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

void printWhenTestError(vector<double>::iterator b, vector<double>::iterator e,
        double fromQuery, double fromTestFunc)
{
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

/*
 * run over examples using query and testFunction
 * to test whether query works as expected
 */
bool doTest(vector<double>& examples, int window_size,
        Query<double>* query,
        double (*testFunction)(vector<double>::iterator, vector<double>::iterator)) // query and function needed
{
    if (!query) return false;

    for (vector<double>::iterator i=examples.begin(); i!=examples.end(); ++i)
    {
        double fromQuery = query->update_with_new_value(*i);

        vector<double>::iterator b = i-window_size;
        if (distance(examples.begin(), b) < 0)
            b = examples.begin();
        double fromTestFunc = (*testFunction)(b, i);

        if (fromQuery != fromTestFunc)
        {
            printWhenTestError(b,i,fromQuery,fromTestFunc);
            return false;
        }
    }
    return true;
}

int main()
{
    /* Query<double>* query[3]; */
    const int count=10000;
    const int window_size=500;

    vector<double> examples;
    populateRandomExample(examples, count);
    /* print(examples); */

    NewElementQuery<double> neq;

    if (!doTest(examples, window_size, (Query<double>*)&neq, &testFunctionNewElementQuery))
    {
        cout << "test failed\n";
    }
    else
    {
        cout << "test passed\n";
    }

    return 0;
}


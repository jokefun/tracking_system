#include <iostream>
#include <random>
#include <vector>

#include "query.h"

using namespace std;

void print(const vector<double>& v)
{
    vector<double>::const_iterator i;
    for (i = v.begin(); i != v.end(); ++i)
    {
        cout << *i << endl;
    }
}

void populateRandomExample(vector<double>& examples, int count)
{
    random_device generator;
    normal_distribution<double> distribution(5.0,2.0);

    for (int i=0; i<count; ++i) {
        examples.push_back(distribution(generator));
    }
}

int main()
{
    /* Query<double>* query[3]; */
    const int count=10;

    vector<double> examples;
    populateRandomExample(examples, count);
    print(examples);

    return 0;
}


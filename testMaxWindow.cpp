#include <iostream>
#include <random>
#include <vector>

#include "maxQuery.h"
#include "myData.h"

using namespace std;

// print

void print(const vector<myData>& v)
{
    vector<myData>::const_iterator i;
    for (i = v.begin(); i != v.end(); ++i)
    {
        cout << (*i).getIndex()<<":"<<i->getValue() << endl;
    }
}

void populateRandomExample(vector<myData>& examples, int count)
{
    random_device generator;
    normal_distribution<double> distribution(5.0,5.0);

    for (int i=0; i<count; ++i) {
		myData d(distribution(generator), i);
        examples.push_back(d);
    }
}

void generateExample1(vector<myData>& examples, int count)
{

	for (int i = count/2; i >= 0; i--)
	{
		myData d(i, count/2 - i);
		examples.push_back(d);
	}
	for (int i = count/2+1; i <= count; i++)
	{
		myData d( i - count/2, i);
		examples.push_back(d);
	}

}


int main()
{
	const int count=20;
    const int window_size=3;
	maxQuery<myData> mq(window_size);
	
	vector<myData> examples;
    populateRandomExample(examples, count);
	//generateExample1(examples, count);
	for (vector<myData>::iterator i=examples.begin(); i != examples.end(); i++)
	{
		myData value = mq.update_with_new_value(*i);
		printf("value:%f\t%f\n", (*i).getValue(), value.getValue());
	}
	
}


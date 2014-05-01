#include <iostream>
#include <random>
#include <vector>
#include <numeric>
#include <algorithm>

#include "query.h"
#include "maxQuery.h"
#include "minQuery.h"
#include "averageQuery.h"
#include "medianQuery.h"

#define EPSILON_DOUBLE 0.00001

double max_value = std::numeric_limits< double >::max();
double min_value = std::numeric_limits< double >::lowest();

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

void printWhenTestError(vector<double>::iterator b, vector<double>::iterator e,
        double fromQuery, double fromTestFunc)
{
    cout << "data: ";
    for (vector<double>::iterator i=b; i!=e+1; ++i)
        cout << *i << " ";
    cout << "\nquery result: " << fromQuery;
    cout << "\ntestF result: " << fromTestFunc << endl;
}

double testFunctionNewElementQuery(vector<double>::iterator b, vector<double>::iterator e, double min_value, double max_value)
{
    return *e;
}

double testFunctionAverage(vector<double>::iterator b, vector<double>::iterator e, double min_value, double max_value)
{
    return accumulate(b, e+1, 0.0) / (distance(b,e) + 1);
}

double testFunctionMax(vector<double>::iterator b, vector<double>::iterator e, double min_value, double max_value)
{
	vector<double>::iterator it;
    double max = std::numeric_limits< double >::lowest();
	
	
	for (it = b; it <= e; it++)
	{
		if (*it > max_value || *it < min_value)
			continue;
		if (*it > max)
		{
			max = *it;
		}
	}
	return max;
}

double testFunctionMin(vector<double>::iterator b, vector<double>::iterator e, double min_value, double max_value)
{
	double min = std::numeric_limits< double >::max();
	vector<double>::iterator it;
	for (it = b; it <= e; it++)
	{
		if (*it > max_value || *it < min_value)
			continue;
		if (*it < min)
		{
			min = *it;
		}
	}
	return min;
}



double testFunctionMedian(vector<double>::iterator b, vector<double>::iterator e, double min_value, double max_value)
{
	double median = std::numeric_limits< double >::max();
	vector<double>::iterator it = b;
	int size = e - b;
	if (size == 0)
		median = *it;
	else if (size % 2 == 1)
	{
		median = (*(it+size/2) + (*(it+size/2+1)))/2;
	}
	else
	{
		median = *(it+size/2+1);
	}
	return median;
}


/*
 * run over examples using query and testFunction
 * to test whether query works as expected
 */
bool doTest(vector<double>& examples, int window_size,
        Query<double>* query,
        double (*testFunction)(vector<double>::iterator, vector<double>::iterator, double min_value, double max_value)) // query and function needed
{
    if (!query) return false;

    for (vector<double>::iterator i=examples.begin(); i!=examples.end(); ++i)
    {
        double fromQuery = query->update_with_new_value(*i);
		cout << fromQuery << " ";
        vector<double>::iterator b; // = i-window_size+1;
        if (distance(examples.begin(), i) < window_size)
            b = examples.begin();
        else
        {
            b = i-window_size+1;
        }
        double fromTestFunc = (*testFunction)(b, i, min_value, max_value);

        if (abs(fromQuery-fromTestFunc) > EPSILON_DOUBLE)
        {
            printWhenTestError(b,i,fromQuery,fromTestFunc);
            return false;
        }
    }
    return true;
}



void testData1(vector<double> &examples)
{
	examples.push_back(-3);
	examples.push_back(-1);
	examples.push_back(-2);
	examples.push_back(-1.5);
	examples.push_back(-1.51);
	examples.push_back(10);
	examples.push_back(1.1);
	examples.push_back(1.2);
	examples.push_back(0.9);
	examples.push_back(0.8);
	examples.push_back(0.7);
	examples.push_back(0.6);
}


int main()
{
    /* Query<double>* query[3]; */
    const int count=20;
    const int window_size=3;

    vector<double> examples;
//    populateRandomExample(examples, count);
	testData1(examples);
    print(examples);



 //    MaxQuery<double> query(window_size, min_value, max_value); 
//	 MinQuery<double> min_query(window_size, min_value, max_value); 

	 MaxQuery<double> query(window_size); 
	 MinQuery<double> min_query(window_size); 
	 MedianQuery<double> med_query(window_size);
 //   AverageQuery<double> query(window_size);
	
	


	



	cout<<"window size"<<window_size<<endl;
	cout<<"minvalue:"<<min_value<<" maxvalue"<<max_value<<endl;
	cout<<"test MaxQuery"<<endl;
	if (!doTest(examples, window_size, (Query<double>*)&query, &testFunctionMax))
	{
		cout << "test failed\n";
	}
	else
	{
		cout << "test passed\n"<<endl;
	}
	cout<<"test MinQuery"<<endl;
	if (!doTest(examples, window_size, (Query<double>*)&min_query, &testFunctionMin))
	{
		cout << "test failed\n";
	}
	else
	{
		cout << "test passed\n"<<endl;
	}
	cout<<"test Median"<<endl;
	if (!doTest(examples, window_size, (Query<double>*)&med_query, &testFunctionMedian))
	{
		cout << "test failed\n";
	}
	else
	{
		cout << "test passed\n";
	}

//	MaxQuery<double> querytest(std::move(query));

	/*
    if (!doTest(examples, window_size, (Query<double>*)&query, &testFunctionAverage))
    {
        cout << "test failed\n";
    }
    else
    {
        cout << "test passed\n";
    }
	*/
    return 0;
}



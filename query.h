// every reasonable C++ compiler (including all C++11 compilers) support #pragma once, which is 
// a cleaner alternative to an include guard

#ifndef QUERY_DEFINATION
#define QUERY_DEFINATION

template <typename T>
class Query
{
    public:
        virtual T update_with_new_value(T v) = 0;
};

#endif

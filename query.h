#pragma once

/* #ifndef QUERY_DEFINATION */
/* #define QUERY_DEFINATION */

template <typename T>
class Query
{
    public:
        virtual T update_with_new_value(T v) = 0;
};

/* #endif */

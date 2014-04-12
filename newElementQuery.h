#ifndef NEWELEMENTQUERY_DEFINATION
#define NEWELEMENTQUERY_DEFINATION

#include "query.h"

template <typename T>
class NewElementQuery : public Query<T>
{
    public:
        T update_with_new_value(T v);
};

template <typename T>
T NewElementQuery<T>::update_with_new_value(T v)
{
    return v;
}

#endif


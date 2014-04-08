#include <iostream>
#include <deque>
#include "window.h"

using namespace std;

Window& Window::operator =(const Window& w)
{
    window_data = w.window_data;
    window_size = w.window_size;
    return *this;
}

Window& Window::operator =(Window&& w)
{
    window_data = move(w.window_data);
    window_size = move(w.window_size);
    return *this;
}

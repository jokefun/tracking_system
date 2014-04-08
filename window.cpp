#include <iostream>
#include <deque>
#include "window.h"

using namespace std;

Window& Window::operator =(const Window& w)
{
    windowed_data = w.windowed_data;
    window_size = w.window_size;
    return *this;
}

Window& Window::operator =(Window&& w)
{
    windowed_data = move(w.windowed_data);
    window_size = move(w.window_size);
    return *this;
}

int main()
{
    cout << "hello" << endl;
    return 0;
}

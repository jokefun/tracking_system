#include <deque>
#include <iostream>

using namespace std;

#ifndef DEFAULT_WINDOW_SIZE
#define DEFAULT_WINDOW_SIZE 1000
#endif

class Window
{
private:
    deque<int> window_data;
    uint window_size;

public:
    Window(uint w_size): window_data(), window_size(w_size) {}
    // default
    Window() { Window(DEFAULT_WINDOW_SIZE); }
    // copy
    Window(const Window& w):
        window_data(w.window_data),
        window_size(w.window_size) {}
    // move
    Window(Window&& w):
        window_data(move(w.window_data)),
        window_size(move(w.window_size)) {}
    ~Window() {}

    Window& operator =(const Window& w);
    Window& operator =(Window&& w);
};

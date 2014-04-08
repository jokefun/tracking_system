#include <deque>
#include <iostream>

using namespace std;

#ifndef DEFAULT_WINDOW_SIZE
#define DEFAULT_WINDOW_SIZE 1000
#endif

class Window
{
private:
    deque<int> windowed_data;
    uint window_size;

public:
    Window(uint w_size): windowed_data(), window_size(w_size) {}
    // default
    Window() { Window(DEFAULT_WINDOW_SIZE); }
    // copy
    Window(const Window& w):
        windowed_data(w.windowed_data),
        window_size(w.window_size) {}
    // move
    Window(Window&& w):
        windowed_data(move(w.windowed_data)),
        window_size(move(w.window_size)) {}
    ~Window() {}

    Window& operator =(const Window& w);
    Window& operator =(Window&& w);
};

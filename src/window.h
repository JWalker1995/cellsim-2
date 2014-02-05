#ifndef WINDOW_H
#define WINDOW_H

#include "gl.h"

#include "simulation.h"

#include <iostream>
#include <unistd.h>

class Window
{
public:
    Window(Simulation *sim);

    ~Window();

    bool run();

protected:
    bool error = false;

    GLFWwindow *window = 0;

    Simulation *sim;
};

void window_error_callback(int code, const char *str);

#endif // WINDOW_H

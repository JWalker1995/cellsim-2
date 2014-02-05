#include "window.h"

Window::Window(Simulation *sim)
    : sim(sim)
{
    std::cout << "GLFW version: " << glfwGetVersionString() << std::endl;

    glfwSetErrorCallback(window_error_callback);

    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "GLFW error!" << std::endl;
        error = true;
        return;
    }

    // Create GLFW window
    window = glfwCreateWindow(640, 480, "CellSim", NULL, NULL);
    if (!window)
    {
        std::cerr << "Could not create GLFW window!" << std::endl;
        error = true;
        return;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    GLenum glew_init = glewInit();
    if (glew_init != GLEW_OK)
    {
        std::cerr << glewGetErrorString(glew_init) << std::endl;
        error = true;
        return;
    }

    std::cout << "GLEW version: " << glewGetString(GLEW_VERSION) << std::endl;

    // Set GLFW options
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);

    glClearColor(0.0, 0.0, 0.2, 0.0);

    //glfwSetCursorPosCallback(window, cursor_callback);
}

Window::~Window()
{
    if (window) { glfwDestroyWindow(window); }
    glfwTerminate();
}

bool Window::run()
{
    if (error) {return true;}

    double sleep_to = glfwGetTime();

    // Main loop
    while (!(glfwWindowShouldClose(window) || error))
    {
        int width;
        int height;

        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        // TODO: tick time should adapt based on load
        sim->tick(TARGET_SPF);
        sim->render(width, height);

        glFlush();
        glfwSwapBuffers(window);

        sleep_to += TARGET_SPF;
        double time = glfwGetTime();
        if (time < sleep_to)
        {
            // Extra time - sleep
            usleep((sleep_to - time) * 1e6);
        }
        else
        {
            // No extra time
            sleep_to = time;
        }

        glfwPollEvents();
    }

    return error;
}

void window_error_callback(int code, const char *str)
{
    switch (code)
    {
    case GLFW_NOT_INITIALIZED:
        std::cerr << "GLFW has not been initialized." << std::endl;
        break;
    case GLFW_NO_CURRENT_CONTEXT:
        std::cerr << "No context is current for this thread." << std::endl;
        break;
    case GLFW_INVALID_ENUM:
        std::cerr << "One of the enum parameters for the function was given an invalid enum." << std::endl;
        break;
    case GLFW_INVALID_VALUE:
        std::cerr << "One of the parameters for the function was given an invalid value." << std::endl;
        break;
    case GLFW_OUT_OF_MEMORY:
        std::cerr << "A memory allocation failed." << std::endl;
        break;
    case GLFW_API_UNAVAILABLE:
        std::cerr << "GLFW could not find support for the requested client API on the system." << std::endl;
        break;
    case GLFW_VERSION_UNAVAILABLE:
        std::cerr << "The requested client API version is not available." << std::endl;
        break;
    case GLFW_PLATFORM_ERROR:
        std::cerr << "A platform-specific error occurred that does not match any of the more specific categories." << std::endl;
        break;
    case GLFW_FORMAT_UNAVAILABLE:
        std::cerr << "The clipboard did not contain data in the requested format." << std::endl;
        break;
    }

    std::cerr << str << std::endl;
}

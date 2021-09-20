#ifndef _GLFW_HANDLER_COMMON_H_
#define _GLFW_HANDLER_COMMON_H_

struct GLFWwindow;

namespace glfw_handler
{
    namespace common
    {
        // Initialized glfw and returns a valid window instance
        bool start_glfw();

        // Should be called after the initialization of the glfw
        GLFWwindow* create_window(int width, int height, const char* title);

    }
}

#endif
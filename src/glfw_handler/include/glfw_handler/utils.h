#ifndef _GLFW_HANDLER_UTILS_H_
#define _GLFW_HANDLER_UTILS_H_

struct GLFWwindow;

namespace glfw_handler
{
    namespace utils
    {
        // glfw: whenever the window size changed (by OS or user resize) this callback function executes
        void framebuffer_size_callback(GLFWwindow* window, int width, int height);

        // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
        void processInput(GLFWwindow *window);
    }
}

#endif
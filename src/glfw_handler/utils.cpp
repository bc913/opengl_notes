#include "glfw_handler/utils.h"
#include <GLFW/glfw3.h>

namespace glfw_handler
{
    namespace utils
    {
        void framebuffer_size_callback(GLFWwindow* window, int width, int height)
        {
            // make sure the viewport matches the new window dimensions; note that width and 
            // height will be significantly larger than specified on retina display.
            glViewport(0, 0, width, height);
        }

        void processInput(GLFWwindow *window)
        {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);
        }
    }
}
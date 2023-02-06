#include "glfw_handler/common.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glfw_handler/utils.h"

namespace glfw_handler
{
    namespace common
    {
        bool start_glfw()
        {
            //TODO:glfwSetErrorCallback(GLFW::glfw_error_callback);
            if (!glfwInit())
                return false;

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        #ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

            return true;
        }

        GLFWwindow* create_window(int width, int height, const char* title)
        {
            /* Create a windowed mode window and its OpenGL context */
            GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
            if (!window)
            {
                glfwTerminate();
                return nullptr;
            }

            /* Make the window's context current */
            glfwMakeContextCurrent(window);
            glfwSetFramebufferSizeCallback(window, utils::framebuffer_size_callback);
            glfwWindowHint(GLFW_SAMPLES, 4);

            return window;
        }
    }
}
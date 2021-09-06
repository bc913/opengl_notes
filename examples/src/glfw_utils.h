#ifndef _GLFW_UTILS_H
#define _GLFW_UTILS_H

#include <string>
struct GLFWwindow;

namespace Utility::GLFW
{
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void update_fps_counter(GLFWwindow* window);
	bool parse_file_into_str(const char* file_name, char* shader_str, int max_len);
	bool gl_log_err(const char* message, ...);
	void glfw_error_callback(int error, const char* description);
	GLFWwindow* start_glfw();
}

#endif // !_GLFW_UTILS_H

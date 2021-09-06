#include "glfw_utils.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio> // sprintf_s
#include <cstdarg>
#include <filesystem>
#define GL_LOG_FILE "gl.log"

namespace Utility::GLFW
{
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		std::cout << "Window_size_changed_callback" << std::flush;
		glViewport(0, 0, width, height);
	}

	void update_fps_counter(GLFWwindow* window)
	{
		static double previous_seconds = glfwGetTime();
		static int frame_count;
		double current_seconds = glfwGetTime();
		double elapsed_seconds = current_seconds - previous_seconds;
		if (elapsed_seconds > 0.25) {
			previous_seconds = current_seconds;
			double fps = (double)frame_count / elapsed_seconds;
			char tmp[128];
			sprintf_s(tmp, 128, "opengl @ fps: %.2f", fps);
			glfwSetWindowTitle(window, tmp);
			frame_count = 0;
		}
		frame_count++;
	}

	bool parse_file_into_str(const char* file_name, char* shader_str, int max_len)
	{


		/*auto file = std::filesystem::path(file_name);
		if (!std::filesystem::exists(file))
			std::cout << "Can not find the file\n";

		std::filesystem::create_directory("Osman");

		std::ifstream t(file_name);
		if (t.fail())
			std::cout << "Fail to read\n";
		std::stringstream buffer;
		buffer << t.rdbuf();
		shader_str = buffer.str();*/

		FILE* file = fopen(file_name, "r");
		if (!file) {
			gl_log_err("ERROR: opening file for reading: %s\n", file_name);
			return false;
		}
		size_t cnt = fread(shader_str, 1, max_len - 1, file);
		if ((int)cnt >= max_len - 1) {
			gl_log_err("WARNING: file %s too big - truncated.\n", file_name);
		}
		if (ferror(file)) {
			gl_log_err("ERROR: reading shader file %s\n", file_name);
			fclose(file);
			return false;
		}
		// append \0 to end of file string
		shader_str[cnt] = 0;
		fclose(file);
		return true;
	}

	bool gl_log_err(const char* message, ...) {
		va_list argptr;
		FILE* file = fopen(GL_LOG_FILE, "a");
		if (!file) {
			fprintf(stderr, "ERROR: could not open GL_LOG_FILE %s file for appending\n",
				GL_LOG_FILE);
			return false;
		}
		va_start(argptr, message);
		vfprintf(file, message, argptr);
		va_end(argptr);
		va_start(argptr, message);
		vfprintf(stderr, message, argptr);
		va_end(argptr);
		fclose(file);
		return true;
	}

	void glfw_error_callback(int error, const char* description) {
		fputs(description, stderr);
		gl_log_err("%s\n", description);
	}

	GLFWwindow* start_glfw()
	{
		glfwSetErrorCallback(GLFW::glfw_error_callback);
		if (!glfwInit())
		{
			std::cerr << "ERROR: could not start GLFW3\n";
			return nullptr;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		/* Create a windowed mode window and its OpenGL context */
		GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
		if (!window)
		{
			glfwTerminate();
			return nullptr;
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, Utility::GLFW::framebuffer_size_callback);
		glfwWindowHint(GLFW_SAMPLES, 4);

		return window;
	}

}
#include "glew_utils.h"
#include <GL/glew.h> // Include this first
#include <stdio.h>
#include <iostream>

namespace Utility::GLEW
{
	bool start_glew()
	{
		// start GLEW extension handler
		glewExperimental = GL_TRUE;
		glewInit();

		// get version info
		const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
		const GLubyte* version = glGetString(GL_VERSION);		// version as a string
		printf("Renderer: %s\n", renderer);
		printf("OpenGL version supported %s\n", version);

		std::cout << "Renderer: " << renderer << "\n";
		std::cout << "OpenGL version supported: " << version << "\n";

		//gl_log("renderer: %s\nversion: %s\n", renderer, version);

		return true;

	}
}
#ifndef _LIGHTING_H_
#define _LIGHTING_H_

#include <GL/glew.h>
#include <string>

enum class ShaderType;
struct GLFWwindow;

namespace tutorials::lighting
{
	int Intro();
	int AmbientLighting(bool dynamic = false);
	int DiffuseLighting(bool dynamic = false);
	int SpecularLighting(bool dynamic = false);
	int IntroAll(bool dynamic = false);
}

namespace tutorials::lighting::material
{
	int Intro(bool dynamic = false);
	
}

namespace tutorials::lighting::lighting_maps
{
	int DiffuseMap();
	int SpecularMap();
}

#endif // !_LIGHTING_H_

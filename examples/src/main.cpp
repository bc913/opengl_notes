#include "tutorials/tutorials.h"
#include "tutorials/lighting.h"

//https://github.com/amhndu/fly
//https://www.youtube.com/watch?v=qQJ7irgxZFQ&feature=youtu.be

//http://www.opengl-tutorial.org/
//https://github.com/SonarSystems/OpenGL-Tutorials

int main()
{
	//return tutorials::cube::translation::Run();
	//tutorials::getting_started::transformations::Scale();
	return tutorials::lighting::lighting_maps::SpecularMap();
}

// Starts opengl context ad ties it to your window
// glew:
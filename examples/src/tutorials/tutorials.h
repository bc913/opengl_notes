#ifndef _TUTORIALS_H_
#define _TUTORIALS_H_

#include <GL/glew.h>
#include <string>

enum class ShaderType;
struct GLFWwindow;

namespace tutorials
{
	namespace getting_started
	{
		namespace triangle
		{
			int Run();
			int RunWithSingleColor();
			int RunMultipleVBO_SingleVAO();
			int LooselyPackedVBO_SingleVAO();
			int MultipleVBO_MultipleVAO();
			//INdexed VBO
			int RunIndexedVBOForSingleColor();
			int RunIndexedVBOWithColors();
		}

		namespace textures
		{
			int Run();
			int RunWithMultipleTexturesCombined();
		}

		namespace transformations
		{
			void Translate();
			void Rotate();
			void Scale();
			int StaticTransformation();
			int DynamicTransformation();
		}

		namespace coordinate_systems
		{
			int Rotate2Din3DOnce();
			int RotateCubeOnce();
			int RotateCubeContinously();
			int MultipleCubes(bool rotate = false);
		}

		namespace camera
		{
			void TestVectors();
			int MultipleCubeCameraRotatesAroundTheScene();
			int WalkAroundMultipleCubesWithUserInput();
			int WalkAroundWithKeyboardLookAroundWithMouse();
		}
	}

	namespace hello_triangle
	{
		int Run();
		GLuint GenerateVertexBuffer();
		GLuint GenerateVertexArrayObject(GLuint& vbo);
		GLuint GenerateTexture(std::string imageFile);
	}

	namespace cube
	{
		GLuint GenerateVertexBuffer(GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat edgeLength);
		GLuint GenerateElementBuffer();
		GLuint GenerateVertexArrayObject(GLuint& vbo, GLuint& ebo);

		namespace translation
		{
			int Run();
		}

		namespace rotation
		{
			int Run();
		}
	}
}
#endif // !_TUTORIALS_H_

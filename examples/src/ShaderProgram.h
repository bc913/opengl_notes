#ifndef _SHADER_PROGRAM_H
#define _SHADER_PROGRAM_H

#include <string>
#include <glm/glm.hpp>
#include "ShaderType.h"

typedef unsigned int GLuint;

class ShaderProgram
{
public:
	ShaderProgram(const char* vertexShaderFile, const char* fragmentShaderFile);
	ShaderProgram(const char* vertexShaderSource, ShaderSourceType vertShaderSourceType,
		const char* fragmentShaderSource, ShaderSourceType fragShaderSourceType);
	~ShaderProgram();

    void setBool(std::string name, bool value) const;
    void setInt(std::string name, int value) const;
	void setFloat(std::string name, float value) const;

    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec2(const std::string& name, float x, float y) const;
    // ------------------------------------------------------------------------
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    // ------------------------------------------------------------------------
    void setVec4(const std::string& name, const glm::vec4& value) const;
	void setVec4(const std::string& name, float x, float y, float z, float w) const;
	// ------------------------------------------------------------------------
	void setMat2(const std::string& name, const glm::mat2& mat) const;
	void setMat3(const std::string& name, const glm::mat3& mat) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;

	void use();
	GLuint Id() const;
private:

	GLuint GenerateShader(const char* shaderSource, ShaderType type, ShaderSourceType sourceType = ShaderSourceType::File);
	void GenerateProgram(GLuint vertexShader, GLuint fragmentShader);

private:
	GLuint program_ = -1;

};

#endif
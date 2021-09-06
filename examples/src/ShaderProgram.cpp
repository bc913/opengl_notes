#include "ShaderProgram.h"
#include <GL/glew.h> // Include this first
#include "utils.h"
#include <string>
#include <iostream>

ShaderProgram::ShaderProgram(const char* vertexShaderFile, const char* fragmentShaderFile)
{
    auto vertexShader = GenerateShader(vertexShaderFile, ShaderType::Vertex);
    auto fragmentShader = GenerateShader(fragmentShaderFile, ShaderType::Fragment);
    GenerateProgram(vertexShader, fragmentShader);

}

ShaderProgram::ShaderProgram(const char* vertexShaderSource, ShaderSourceType vertShaderSourceType,
    const char* fragmentShaderSource, ShaderSourceType fragShaderSourceType)
{
    auto vertexShader = GenerateShader(vertexShaderSource, ShaderType::Vertex, vertShaderSourceType);
    auto fragmentShader = GenerateShader(fragmentShaderSource, ShaderType::Fragment, fragShaderSourceType);
    GenerateProgram(vertexShader, fragmentShader);
}

GLuint ShaderProgram::GenerateShader(const char* shaderSource, ShaderType type, 
    ShaderSourceType sourceType)
{
    const GLchar* p = nullptr;
    GLuint sh = -1;

    if (sourceType == ShaderSourceType::File)
    {
        char shader[1024 * 256];
        Utility::parse_file_into_str(shaderSource, shader, 1024 * 256);
        p = (const GLchar*)shader;
    }
    else
        p = (const GLchar*)shaderSource;


    switch (type)
    {
    case ShaderType::Vertex:
        sh = glCreateShader(GL_VERTEX_SHADER);
        break;
    case ShaderType::Fragment:
        sh = glCreateShader(GL_FRAGMENT_SHADER);
        break;
    default:
        break;
    }

    
    glShaderSource(sh, 1, &p, NULL);
    glCompileShader(sh);

    // check for compile errors
    int success = -1;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &success);
    if (GL_TRUE != success)
    {
        char infoLog[512];
        glGetShaderInfoLog(sh, 512, NULL, infoLog);
        std::cerr << "Shader can not compile: " << infoLog << "\n";
        return -1; // or exit or something
    }

    return sh;
}

void ShaderProgram::GenerateProgram(GLuint vertexShader, GLuint fragmentShader)
{
    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, vertexShader);
    glAttachShader(shader_programme, fragmentShader); 

    glLinkProgram(shader_programme);

    int success = -1;
    glGetProgramiv(shader_programme, GL_LINK_STATUS, &success);
    if (GL_TRUE != success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shader_programme, 512, NULL, infoLog);
        std::cerr << "ERROR: Shader programme linking failed: " << infoLog << std::endl;
        return;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    program_ =  shader_programme;
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(program_);
}

void ShaderProgram::setBool(std::string name, bool value) const
{
    glUniform1i(glGetUniformLocation(program_, name.c_str()), (int)value);
}

void ShaderProgram::setInt(std::string name, int value) const
{
    glUniform1i(glGetUniformLocation(program_, name.c_str()), value);
}

void ShaderProgram::setFloat(std::string name, float value) const
{
    glUniform1f(glGetUniformLocation(program_, name.c_str()), value);
}

// ------------------------------------------------------------------------

void ShaderProgram::setVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(program_, name.c_str()), 1, &value[0]);
}
void ShaderProgram::setVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(program_, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void ShaderProgram::setVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(program_, name.c_str()), 1, &value[0]);
}
void ShaderProgram::setVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(program_, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void ShaderProgram::setVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(program_, name.c_str()), 1, &value[0]);
}
void ShaderProgram::setVec4(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(program_, name.c_str()), x, y, z, w);
}

// ------------------------------------------------------------------------
void ShaderProgram::setMat2(const std::string& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(program_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::setMat3(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(program_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(program_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::use()
{
    glUseProgram(program_);
}

GLuint ShaderProgram::Id() const
{
    return program_;
}
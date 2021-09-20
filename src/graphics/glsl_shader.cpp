#include "graphics/glsl_shader.h"
#include "graphics/shader_type.h"
#include <glad/glad.h>
// std
#include <stdexcept>
#include <vector>
#include <iostream>
namespace graphics
{
    glsl_shader::glsl_shader(shader_type type, std::string source) :
        m_type(type), m_source(source)
    {
        if(m_type == shader_type::Unknown)
            throw std::invalid_argument("Given shader type can not be Unknown.");

        if(m_source.empty())
            throw std::invalid_argument("Given source string can not be empty");

        create_shader();
    }

    glsl_shader::~glsl_shader()
    {
        if(m_id)
            glDeleteShader(m_id);
    }

    unsigned int glsl_shader::id() const
    {
        return m_id;
    }

    bool glsl_shader::compile()
    {
        GLint source_len = m_source.length();
        const GLchar* code = m_source.c_str();
        glShaderSource(m_id, 1, &code, NULL); //todo: errorcheck
        glCompileShader(m_id); //todo: errorcheck

        GLint success = -1;
        glGetShaderiv(m_id, GL_COMPILE_STATUS, &success); //todo: errorcheck

        if(success != GL_TRUE)
        {
            GLint log_len;
            glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &log_len);
            if(log_len)
            {
                std::vector<char> log(log_len);
                GLsizei size;
                glGetShaderInfoLog(m_id, log_len, NULL, log.data());
                std::cout << "\nGLSL Compiler errors";
                std::cout << "\n" << std::string(log.begin(), log.end()) << "\n";
            }
            else
            {
                std::cout << "Unknown compiler error" << "\n";
            }
        }

        return success;
    }

    // ===============
    // PRIVATE
    // ===============
    void glsl_shader::create_shader()
    {
        switch (m_type)
        {
        case shader_type::Vertex:
            m_id = glCreateShader(GL_VERTEX_SHADER);
            break;
        case shader_type::Fragment:
            m_id = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        default:
            throw std::logic_error("Can not create unsupported shader type.");
            break;
        }
    }
}
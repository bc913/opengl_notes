#ifndef _GRAPHICS_GLSL_SHADER_H_
#define _GRAPHICS_GLSL_SHADER_H_

#include <string>

namespace graphics
{
    enum class shader_type;

    class glsl_shader
    {
    public:
        unsigned int id() const;
        bool compile();
    public:
        glsl_shader(shader_type type, std::string source);
        ~glsl_shader();
    private:
        std::string m_source;
        shader_type m_type;
        unsigned int m_id;
    private:
        void create_shader();
    };
}

#endif
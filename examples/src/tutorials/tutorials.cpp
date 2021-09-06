#include "tutorials.h"
#include <GLFW/glfw3.h>
#include "../glfw_utils.h"
#include "../glew_utils.h"
#include <iostream>
#include "../ShaderType.h"
#include "../ShaderProgram.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

#include<math.h>
#include <cmath>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

namespace tutorials
{
    namespace getting_started::triangle
    {
        int Run()
        {
            // Initialize the glfw & glew
            GLFWwindow* window = Utility::GLFW::start_glfw();
            Utility::GLEW::start_glew();

            // Some openGL settings
            // tell GL to only draw onto a pixel if the shape is closer to the viewer
            glEnable(GL_DEPTH_TEST); // enable depth-testing
            glDepthFunc(GL_LESS);		 // depth-testing interprets a smaller value as "closer"

            // ====================
            // VBO - VAO GENERATION
            // ====================
            
            // Generate vertex data
            // Coordinates for 3 vertex with {X,Y,Z}
            GLfloat coordinates[] = {
                0.0f,   0.5f,   0.0f,
                0.5f,   -0.5f,  0.0f,
                -0.5f,  -0.5f,  0.0f
            };

            // Generate tightly-packed VBO
            // VBO will only be occupied by one vertex attribute
            // which holds the coordinate data
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), coordinates, GL_STATIC_DRAW);

            // 5. Generate and bind VAO
            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            // 6. Define and associate the vertex attribute to the vbo
            // // Before doing so, you have to make sure that the corresponding vbo is bound
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            // Generate the vertex attribute (tightly packed)
            // since it is tightly packed, stride = 0
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            // Enable it's index for the most recently bound vbo (coord_vbo)
            glEnableVertexAttribArray(0);

            // Bind to VBO-0 if you are done
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // ====================
            //      SHADERS
            // ====================
            // 1. Define the shader source
            const char* vertex_shader_source_str = 
                "#version 330 core\n"
                "layout (location = 0) in vec3 aPos;\n"
                "void main()\n"
                "{\n"
                "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                "}\0";

            // 2. Create the shader
            GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

            // 3. Attach the source string to the shader & compile
            const GLchar* vertex_shader_source = (const GLchar*)vertex_shader_source_str;
            glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
            glCompileShader(vertex_shader);

            // 4. Check for the errors
            int success = -1;
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 6. Create the program to attach defined shaders
            GLuint shader_programme = glCreateProgram();

            // 7. Attach & link the shaders to the program
            glAttachShader(shader_programme, vertex_shader);
            glLinkProgram(shader_programme);

            // 8. Check against the linking errors
            success = -1;
            glGetProgramiv(shader_programme, GL_LINK_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetProgramInfoLog(shader_programme, 512, NULL, infoLog);
                std::cerr << "ERROR: Shader programme linking failed: " << infoLog << std::endl;
                return -1;
            }

            // 9. Deleting shader at this point does not hurt
            glDeleteShader(vertex_shader);

            

            /* Loop until the user closes the window */
            while (!glfwWindowShouldClose(window))
            {
                // fps counter
                Utility::GLFW::update_fps_counter(window);

                /* Render here */
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// scene background color
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // 10. use the program
                glUseProgram(shader_programme);
                /* draw points 0-3 from the currently bound VAO with current in-use
                shader */
                glBindVertexArray(vao);
                glDrawArrays(GL_TRIANGLES, 0, 3);


                /* Swap front and back buffers */
                /* update other events like input handling */
                glfwSwapBuffers(window);

                /* Poll for and process events */
                /* put the stuff we've been drawing onto the display */
                glfwPollEvents();
            }

            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);

            glfwTerminate();
            return 0;
        }

        int RunWithSingleColor()
        {
            // Initialize the glfw & glew
            GLFWwindow* window = Utility::GLFW::start_glfw();
            Utility::GLEW::start_glew();

            // Some openGL settings
            // tell GL to only draw onto a pixel if the shape is closer to the viewer
            glEnable(GL_DEPTH_TEST); // enable depth-testing
            glDepthFunc(GL_LESS);		 // depth-testing interprets a smaller value as "closer"

            // ====================
            // VBO - VAO GENERATION
            // ====================

            // Generate vertex data
            // Coordinates for 3 vertex with {X,Y,Z}
            GLfloat coordinates[] = {
                0.0f,   0.5f,   0.0f,
                0.5f,   -0.5f,  0.0f,
                -0.5f,  -0.5f,  0.0f
            };

            // Generate tightly-packed VBO
            // Each VBO will only be occupied by one vertex attribute
            // which holds the coordinate data
            // 2,3 & 4
            GLuint coord_vbo;
            glGenBuffers(1, &coord_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, coord_vbo);
            glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), coordinates, GL_STATIC_DRAW);

            // 5. Generate and bind VAO
            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            // 6. Define and associate the vertex attribute to the vbo
            glBindBuffer(GL_ARRAY_BUFFER, coord_vbo);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

            // ====================
            //      SHADERS
            // ====================
            // 1. Define the shader source
            const char* vertex_shader_source_str =
                "#version 330 core\n"
                "layout (location = 0) in vec3 vertex_position;\n"
                "out vec3 colour;\n"
                "void main()\n"
                "{\n"
                "   gl_Position = vec4(vertex_position.x, vertex_position.y, vertex_position.z, 1.0);\n"
                "}\0";

            const char* fragment_shader_source_str =
                "#version 330 core\n"
                "out vec4 frag_colour;\n"
                "void main()\n"
                "{\n"
                "   frag_colour = vec4 (1.0f, 0.5f, 0.2f, 1.0);\n"
                "}\0";

            // 2. Create the vertex shader
            GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

            // 3. Attach the source string to the shader & compile
            const GLchar* vertex_shader_source = (const GLchar*)vertex_shader_source_str;
            glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
            glCompileShader(vertex_shader);

            // 4. Check for the errors
            int success = -1;
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // REpeat steps 2, 3 & 4 fragment shader
            GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            const GLchar* fragment_shader_source = (const GLchar*)fragment_shader_source_str;
            glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
            glCompileShader(fragment_shader);
            // error check
            success = -1;
            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 6. Create the program to attach defined shaders
            GLuint shader_programme = glCreateProgram();

            // 7. Attach & link the shaders to the program
            glAttachShader(shader_programme, vertex_shader);
            glAttachShader(shader_programme, fragment_shader);
            glLinkProgram(shader_programme);

            // 8. Check against the linking errors
            success = -1;
            glGetProgramiv(shader_programme, GL_LINK_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetProgramInfoLog(shader_programme, 512, NULL, infoLog);
                std::cerr << "ERROR: Shader programme linking failed: " << infoLog << std::endl;
                return -1;
            }

            // 9. Deleting shader at this point does not hurt
            glDeleteShader(vertex_shader);


            /* Loop until the user closes the window */
            while (!glfwWindowShouldClose(window))
            {
                // fps counter
                Utility::GLFW::update_fps_counter(window);

                /* Render here */
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// scene background color
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // 10. use the program
                glUseProgram(shader_programme);
                /* draw points 0-3 from the currently bound VAO with current in-use
                shader */
                glBindVertexArray(vao);
                glDrawArrays(GL_TRIANGLES, 0, 3);


                /* Swap front and back buffers */
                /* update other events like input handling */
                glfwSwapBuffers(window);

                /* Poll for and process events */
                /* put the stuff we've been drawing onto the display */
                glfwPollEvents();
            }

            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &coord_vbo);

            glfwTerminate();
            return 0;
        }

        int RunMultipleVBO_SingleVAO()
        {
            // Initialize the glfw & glew
            GLFWwindow* window = Utility::GLFW::start_glfw();
            Utility::GLEW::start_glew();

            // Some openGL settings
            // tell GL to only draw onto a pixel if the shape is closer to the viewer
            glEnable(GL_DEPTH_TEST); // enable depth-testing
            glDepthFunc(GL_LESS);		 // depth-testing interprets a smaller value as "closer"

            // ====================
            // VBO - VAO GENERATION
            // ====================

            // Generate vertex data
            // Coordinates for 3 vertex with {X,Y,Z}
            GLfloat coordinates[] = {
                0.0f,   0.5f,   0.0f,
                0.5f,   -0.5f,  0.0f,
                -0.5f,  -0.5f,  0.0f
            };

            GLfloat colours[] = {
                1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f
            };

            // Generate tightly-packed VBO
            // Each VBO will only be occupied by one vertex attribute
            // which holds the coordinate data
            // 2,3 & 4
            GLuint coord_vbo;
            glGenBuffers(1, &coord_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, coord_vbo);
            glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), coordinates, GL_STATIC_DRAW);

            GLuint colours_vbo;
            glGenBuffers(1, &colours_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
            glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colours, GL_STATIC_DRAW);

            // 5. Generate and bind VAO
            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            // 6. Define and associate the vertex attribute to the vbo
            glBindBuffer(GL_ARRAY_BUFFER, coord_vbo);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

            // Since we have multiple vbos to be linked to single vao, 
            // each vertex attrib array should have unique indexes.
            glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

            // ====================
            //      SHADERS
            // ====================
            // 1. Define the shader source
            const char* vertex_shader_source_str =
                "#version 330 core\n"
                "layout (location = 0) in vec3 vertex_position;\n"
                "layout (location = 1) in vec3 vertex_colour;\n"
                "out vec3 colour;\n"
                "void main()\n"
                "{\n"
                "   colour = vertex_colour;\n"
                "   gl_Position = vec4(vertex_position.x, vertex_position.y, vertex_position.z, 1.0);\n"
                "}\0";

            const char* fragment_shader_source_str = 
                "#version 330 core\n"
                "in vec3 colour;\n"
                "out vec4 frag_colour;\n"
                "void main()\n"
                "{\n"
                "   frag_colour = vec4 (colour, 1.0);\n"
                "}\0";

            // 2. Create the vertex shader
            GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);            

            // 3. Attach the source string to the shader & compile
            const GLchar* vertex_shader_source = (const GLchar*)vertex_shader_source_str;
            glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
            glCompileShader(vertex_shader);

            // 4. Check for the errors
            int success = -1;
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // REpeat steps 2, 3 & 4 fragment shader
            GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            const GLchar* fragment_shader_source = (const GLchar*)fragment_shader_source_str;
            glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
            glCompileShader(fragment_shader);
            // error check
            success = -1;
            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }            

            // 6. Create the program to attach defined shaders
            GLuint shader_programme = glCreateProgram();

            // 7. Attach & link the shaders to the program
            glAttachShader(shader_programme, vertex_shader);
            glAttachShader(shader_programme, fragment_shader);
            glLinkProgram(shader_programme);

            // 8. Check against the linking errors
            success = -1;
            glGetProgramiv(shader_programme, GL_LINK_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetProgramInfoLog(shader_programme, 512, NULL, infoLog);
                std::cerr << "ERROR: Shader programme linking failed: " << infoLog << std::endl;
                return -1;
            }

            // 9. Deleting shader at this point does not hurt
            glDeleteShader(vertex_shader);


            /* Loop until the user closes the window */
            while (!glfwWindowShouldClose(window))
            {
                // fps counter
                Utility::GLFW::update_fps_counter(window);

                /* Render here */
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// scene background color
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // 10. use the program
                glUseProgram(shader_programme);
                /* draw points 0-3 from the currently bound VAO with current in-use
                shader */
                glBindVertexArray(vao);
                glDrawArrays(GL_TRIANGLES, 0, 3);


                /* Swap front and back buffers */
                /* update other events like input handling */
                glfwSwapBuffers(window);

                /* Poll for and process events */
                /* put the stuff we've been drawing onto the display */
                glfwPollEvents();
            }

            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &coord_vbo);
            glDeleteBuffers(1, &colours_vbo);           

            glfwTerminate();
            return 0;
        }

        int LooselyPackedVBO_SingleVAO()
        {
            // Initialize the glfw & glew
            GLFWwindow* window = Utility::GLFW::start_glfw();
            Utility::GLEW::start_glew();

            // Some openGL settings
            // tell GL to only draw onto a pixel if the shape is closer to the viewer
            glEnable(GL_DEPTH_TEST); // enable depth-testing
            glDepthFunc(GL_LESS);		 // depth-testing interprets a smaller value as "closer"

            // ====================
            // VBO - VAO GENERATION
            // ====================

            // Generate vertex data
            GLfloat vertices[] = {
                // Coord                // Colors      
                0.0f,   0.5f,   0.0f,   1.0f, 0.0f, 0.0f,
                0.5f,   -0.5f,  0.0f,   0.0f, 1.0f, 0.0f,
                -0.5f,  -0.5f,  0.0f,   0.0f, 0.0f, 1.0f
            };


            // Generate tightly-packed VBO
            // VBO will only be occupied by one vertex attribute
            // which holds the coordinate data
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // 5. Generate and bind VAO
            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            // 6. Define and associate the vertex attribute to the vbo
            // // Before doing so, you have to make sure that the corresponding vbo is bound
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            // Vertex attribute for the coordinates
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);

            // vertex attributes for the colors
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);

            // Bind to VBO-0 if you are done
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // ====================
            //      SHADERS
            // ====================
            // 1. Define the shader source
            const char* vertex_shader_source_str =
                "#version 330 core\n"
                "layout (location = 0) in vec3 vertex_position;\n"
                "layout (location = 1) in vec3 vertex_color;\n"
                "out vec3 color;\n"
                "void main()\n"
                "{\n"
                "   color = vertex_color;\n"
                "   gl_Position = vec4(vertex_position.x, vertex_position.y, vertex_position.z, 1.0);\n"
                "}\0";

            const char* fragment_shader_source_str =
                "#version 330 core\n"
                "in vec3 color;\n"
                "out vec4 frag_color;\n"
                "void main()\n"
                "{\n"
                "   frag_color = vec4 (color, 1.0);\n"
                "}\0";

            // 2. Create the vertex shader
            GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

            // 3. Attach the source string to the shader & compile
            const GLchar* vertex_shader_source = (const GLchar*)vertex_shader_source_str;
            glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
            glCompileShader(vertex_shader);

            // 4. Check for the errors
            int success = -1;
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // REpeat steps 2, 3 & 4 fragment shader
            GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            const GLchar* fragment_shader_source = (const GLchar*)fragment_shader_source_str;
            glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
            glCompileShader(fragment_shader);
            // error check
            success = -1;
            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 6. Create the program to attach defined shaders
            GLuint shader_programme = glCreateProgram();

            // 7. Attach & link the shaders to the program
            glAttachShader(shader_programme, vertex_shader);
            glAttachShader(shader_programme, fragment_shader);
            glLinkProgram(shader_programme);

            // 8. Check against the linking errors
            success = -1;
            glGetProgramiv(shader_programme, GL_LINK_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetProgramInfoLog(shader_programme, 512, NULL, infoLog);
                std::cerr << "ERROR: Shader programme linking failed: " << infoLog << std::endl;
                return -1;
            }

            // 9. Deleting shader at this point does not hurt
            glDeleteShader(vertex_shader);


            /* Loop until the user closes the window */
            while (!glfwWindowShouldClose(window))
            {
                // fps counter
                Utility::GLFW::update_fps_counter(window);

                /* Render here */
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// scene background color
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // 10. use the program
                glUseProgram(shader_programme);
                /* draw points 0-3 from the currently bound VAO with current in-use
                shader */
                glBindVertexArray(vao);
                glDrawArrays(GL_TRIANGLES, 0, 3);


                /* Swap front and back buffers */
                /* update other events like input handling */
                glfwSwapBuffers(window);

                /* Poll for and process events */
                /* put the stuff we've been drawing onto the display */
                glfwPollEvents();
            }

            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);

            glfwTerminate();
            return 0;
        }

        int MultipleVBO_MultipleVAO()
        {
            // Initialize the glfw & glew
            GLFWwindow* window = Utility::GLFW::start_glfw();
            Utility::GLEW::start_glew();

            // Some openGL settings
            // tell GL to only draw onto a pixel if the shape is closer to the viewer
            glEnable(GL_DEPTH_TEST); // enable depth-testing
            glDepthFunc(GL_LESS);		 // depth-testing interprets a smaller value as "closer"

            // ====================
            // VBO - VAO GENERATION
            // ====================

            // Generate vertex data
            // Coordinates for 3 vertex with {X,Y,Z}
            GLfloat first_triangle_vertex_data[] = {
                // Coords            // Colors       
                -0.9f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // left 
                -0.0f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // right
                -0.45f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f // top 
            };

            GLfloat second_triangle_vertex_data[] = {
                // Coords           // Colors       
                0.0f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // left
                0.9f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // right
                0.45f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
            };

            // Generate tightly-packed VBO
            // Each VBO will only be occupied by one vertex attribute
            // which holds the coordinate data
            // 2,3 & 4
            GLuint first_triangle_vbo;
            glGenBuffers(1, &first_triangle_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, first_triangle_vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(first_triangle_vertex_data), first_triangle_vertex_data, GL_STATIC_DRAW);

            GLuint second_triangle_vbo;
            glGenBuffers(1, &second_triangle_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, second_triangle_vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(second_triangle_vertex_data), second_triangle_vertex_data, GL_STATIC_DRAW);

            // 5. Generate
            GLuint VAOs[2];
            glGenVertexArrays(2, VAOs);            

            // 6. Define and associate the vertex attribute to the vbo
            // first triangle
            glBindVertexArray(VAOs[0]);                 // Bind the corresponding VAO first
            glBindBuffer(GL_ARRAY_BUFFER, first_triangle_vbo);

            // Vertex attribute for the coordinates
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);

            // vertex attributes for the colors
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, 0); // not required
            glBindVertexArray(0); // not required

            // Second triangle
            glBindVertexArray(VAOs[1]);                 // Bind the corresponding VAO first
            glBindBuffer(GL_ARRAY_BUFFER, second_triangle_vbo);

            // Vertex attribute for the coordinates
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);

            // vertex attributes for the colors
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, 0); // not required
            glBindVertexArray(0); // not required

            // ====================
            //      SHADERS
            // ====================
            // 1. Define the shader source
            const char* vertex_shader_source_str =
                "#version 330 core\n"
                "layout (location = 0) in vec3 vertex_position;\n"
                "layout (location = 1) in vec3 vertex_color;\n"
                "out vec3 color;\n"
                "void main()\n"
                "{\n"
                "   color = vertex_color;\n"
                "   gl_Position = vec4(vertex_position.x, vertex_position.y, vertex_position.z, 1.0);\n"
                "}\0";

            const char* fragment_shader_source_str =
                "#version 330 core\n"
                "in vec3 color;\n"
                "out vec4 frag_color;\n"
                "void main()\n"
                "{\n"
                "   frag_color = vec4 (color, 1.0);\n"
                "}\0";

            // 2. Create the vertex shader
            GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

            // 3. Attach the source string to the shader & compile
            const GLchar* vertex_shader_source = (const GLchar*)vertex_shader_source_str;
            glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
            glCompileShader(vertex_shader);

            // 4. Check for the errors
            int success = -1;
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // REpeat steps 2, 3 & 4 fragment shader
            GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            const GLchar* fragment_shader_source = (const GLchar*)fragment_shader_source_str;
            glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
            glCompileShader(fragment_shader);
            // error check
            success = -1;
            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 6. Create the program to attach defined shaders
            GLuint shader_programme = glCreateProgram();

            // 7. Attach & link the shaders to the program
            glAttachShader(shader_programme, vertex_shader);
            glAttachShader(shader_programme, fragment_shader);
            glLinkProgram(shader_programme);

            // 8. Check against the linking errors
            success = -1;
            glGetProgramiv(shader_programme, GL_LINK_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetProgramInfoLog(shader_programme, 512, NULL, infoLog);
                std::cerr << "ERROR: Shader programme linking failed: " << infoLog << std::endl;
                return -1;
            }

            // 9. Deleting shader at this point does not hurt
            glDeleteShader(vertex_shader);


            /* Loop until the user closes the window */
            while (!glfwWindowShouldClose(window))
            {
                // fps counter
                Utility::GLFW::update_fps_counter(window);

                /* Render here */
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// scene background color
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // 10. use the program
                glUseProgram(shader_programme);
                /* draw points 0-3 from the currently bound VAO with current in-use
                shader */
                glBindVertexArray(VAOs[0]);
                glDrawArrays(GL_TRIANGLES, 0, 3);

                glBindVertexArray(VAOs[1]);
                glDrawArrays(GL_TRIANGLES, 0, 3);


                /* Swap front and back buffers */
                /* update other events like input handling */
                glfwSwapBuffers(window);

                /* Poll for and process events */
                /* put the stuff we've been drawing onto the display */
                glfwPollEvents();
            }

            glDeleteVertexArrays(2, VAOs);
            glDeleteBuffers(1, &first_triangle_vbo);
            glDeleteBuffers(1, &second_triangle_vbo);
            glDeleteProgram(shader_programme);

            glfwTerminate();
            return 0;
        }

        int RunIndexedVBOForSingleColor()
        {
            // Initialize the glfw & glew
            GLFWwindow* window = Utility::GLFW::start_glfw();
            Utility::GLEW::start_glew();

            // Some openGL settings
            // tell GL to only draw onto a pixel if the shape is closer to the viewer
            glEnable(GL_DEPTH_TEST); // enable depth-testing
            glDepthFunc(GL_LESS);		 // depth-testing interprets a smaller value as "closer"

            // ====================
            // VBO - VAO GENERATION
            // ====================

            // Generate vertex data
            // We have four vertices with 3 position data for each
            GLfloat vertices[] = {
                0.5f,  0.5f, 0.0f,  // top right
                0.5f, -0.5f, 0.0f,  // bottom right
                -0.5f, -0.5f, 0.0f,  // bottom left
                -0.5f,  0.5f, 0.0f   // top left 
            };

            // Generate data for indices
            GLuint indices[] = {  // note that we start from 0!
                0, 1, 3,  // first Triangle
                1, 2, 3   // second Triangle
            };

            // Generate tightly-packed VBO and then the Indexed VBO
            // 2,3 & 4
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // 5. Generate and bind VAO
            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao); //Bind Vao First

            // Indexed VBO
            // Since they are directly stored in VAO, the corresponding 
            // VAO should Be bound first.
            GLuint indexed_vbo;
            glGenBuffers(1, &indexed_vbo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexed_vbo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // 6. Define and associate the vertex attribute to the vbo
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

            // ====================
            //      SHADERS
            // ====================
            // 1. Define the shader source
            const char* vertex_shader_source_str =
                "#version 330 core\n"
                "layout (location = 0) in vec3 vertex_position;\n"
                "void main()\n"
                "{\n"
                "   gl_Position = vec4(vertex_position.x, vertex_position.y, vertex_position.z, 1.0);\n"
                "}\0";

            const char* fragment_shader_source_str =
                "#version 330 core\n"
                "out vec4 frag_colour;\n"
                "void main()\n"
                "{\n"
                "   frag_colour = vec4 (1.0f, 0.5f, 0.2f, 1.0);\n"
                "}\0";

            // 2. Create the vertex shader
            GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

            // 3. Attach the source string to the shader & compile
            const GLchar* vertex_shader_source = (const GLchar*)vertex_shader_source_str;
            glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
            glCompileShader(vertex_shader);

            // 4. Check for the errors
            int success = -1;
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // REpeat steps 2, 3 & 4 fragment shader
            GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            const GLchar* fragment_shader_source = (const GLchar*)fragment_shader_source_str;
            glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
            glCompileShader(fragment_shader);
            // error check
            success = -1;
            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 6. Create the program to attach defined shaders
            GLuint shader_programme = glCreateProgram();

            // 7. Attach & link the shaders to the program
            glAttachShader(shader_programme, vertex_shader);
            glAttachShader(shader_programme, fragment_shader);
            glLinkProgram(shader_programme);

            // 8. Check against the linking errors
            success = -1;
            glGetProgramiv(shader_programme, GL_LINK_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetProgramInfoLog(shader_programme, 512, NULL, infoLog);
                std::cerr << "ERROR: Shader programme linking failed: " << infoLog << std::endl;
                return -1;
            }

            // 9. Deleting shader at this point does not hurt
            glDeleteShader(vertex_shader);


            /* Loop until the user closes the window */
            while (!glfwWindowShouldClose(window))
            {
                // fps counter
                Utility::GLFW::update_fps_counter(window);

                /* Render here */
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// scene background color
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Show in wireframe
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glEnable(GL_LINE_SMOOTH);
                glLineWidth(10.0);

                // 10. use the program
                glUseProgram(shader_programme);
                /* draw points 0-3 from the currently bound VAO with current in-use
                shader */
                glBindVertexArray(vao);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


                /* Swap front and back buffers */
                /* update other events like input handling */
                glfwSwapBuffers(window);

                /* Poll for and process events */
                /* put the stuff we've been drawing onto the display */
                glfwPollEvents();
            }

            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
            glDeleteBuffers(1, &indexed_vbo);
            glDeleteProgram(shader_programme);

            glfwTerminate();
            return 0;
        }

        int RunIndexedVBOWithColors()
        {
            // Initialize the glfw & glew
            GLFWwindow* window = Utility::GLFW::start_glfw();
            Utility::GLEW::start_glew();

            // Some openGL settings
            // tell GL to only draw onto a pixel if the shape is closer to the viewer
            glEnable(GL_DEPTH_TEST); // enable depth-testing
            glDepthFunc(GL_LESS);		 // depth-testing interprets a smaller value as "closer"

            // ====================
            // VBO - VAO GENERATION
            // ====================

            // Generate vertex data
            // We have four vertices with 3 position data for each
            GLfloat vertices[] = {
                                        // Colors      
                0.5f,  0.5f,    0.0f,   1.0f, 0.0f, 0.0f,  // top right
                0.5f, -0.5f,    0.0f,   0.0f, 1.0f, 0.0f,  // bottom right
                -0.5f, -0.5f,   0.0f,   0.0f, 0.0f, 1.0f,  // bottom left
                -0.5f,  0.5f,   0.0f,   1.0f, 1.0f, 1.0f   // top left 
            };

            // Generate data for indices
            GLuint indices[] = {  // note that we start from 0!
                0, 1, 3,  // first Triangle
                1, 2, 3   // second Triangle
            };

            // Generate tightly-packed VBO and then the Indexed VBO
            // 2,3 & 4
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // 5. Generate and bind VAO
            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao); //Bind Vao First

            // Indexed VBO
            // IMPORTANT: Since they are directly stored in VAO, the corresponding 
            // VAO should Be bound first.
            GLuint indexed_vbo;
            glGenBuffers(1, &indexed_vbo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexed_vbo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // 6. Define and associate the vertex attribute to the vbo
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            //positions
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            //colors
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

            // ====================
            //      SHADERS
            // ====================
            // 1. Define the shader source
            const char* vertex_shader_source_str =
                "#version 330 core\n"
                "layout (location = 0) in vec3 vertex_position;\n"
                "layout (location = 1) in vec3 vertex_color;\n"
                "out vec3 color;\n"
                "void main()\n"
                "{\n"
                "   color = vertex_color;\n"
                "   gl_Position = vec4(vertex_position.x, vertex_position.y, vertex_position.z, 1.0);\n"
                "}\0";

            const char* fragment_shader_source_str =
                "#version 330 core\n"
                "in vec3 color;\n"
                "out vec4 frag_color;\n"
                "void main()\n"
                "{\n"
                "   frag_color = vec4 (color, 1.0);\n"
                "}\0";

            // 2. Create the vertex shader
            GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

            // 3. Attach the source string to the shader & compile
            const GLchar* vertex_shader_source = (const GLchar*)vertex_shader_source_str;
            glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
            glCompileShader(vertex_shader);

            // 4. Check for the errors
            int success = -1;
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // REpeat steps 2, 3 & 4 fragment shader
            GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            const GLchar* fragment_shader_source = (const GLchar*)fragment_shader_source_str;
            glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
            glCompileShader(fragment_shader);
            // error check
            success = -1;
            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 6. Create the program to attach defined shaders
            GLuint shader_programme = glCreateProgram();

            // 7. Attach & link the shaders to the program
            glAttachShader(shader_programme, vertex_shader);
            glAttachShader(shader_programme, fragment_shader);
            glLinkProgram(shader_programme);

            // 8. Check against the linking errors
            success = -1;
            glGetProgramiv(shader_programme, GL_LINK_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetProgramInfoLog(shader_programme, 512, NULL, infoLog);
                std::cerr << "ERROR: Shader programme linking failed: " << infoLog << std::endl;
                return -1;
            }

            // 9. Deleting shader at this point does not hurt
            glDeleteShader(vertex_shader);


            /* Loop until the user closes the window */
            while (!glfwWindowShouldClose(window))
            {
                // fps counter
                Utility::GLFW::update_fps_counter(window);

                /* Render here */
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// scene background color
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Show in wireframe
                /*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glEnable(GL_LINE_SMOOTH);
                glLineWidth(10.0);*/

                // 10. use the program
                glUseProgram(shader_programme);
                /* draw points 0-3 from the currently bound VAO with current in-use
                shader */
                glBindVertexArray(vao);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


                /* Swap front and back buffers */
                /* update other events like input handling */
                glfwSwapBuffers(window);

                /* Poll for and process events */
                /* put the stuff we've been drawing onto the display */
                glfwPollEvents();
            }

            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
            glDeleteBuffers(1, &indexed_vbo);
            glDeleteProgram(shader_programme);

            glfwTerminate();
            return 0;
        }
    }

    namespace getting_started::textures
    {
        int Run()
        {
            // Initialize the glfw & glew
            GLFWwindow* window = Utility::GLFW::start_glfw();
            Utility::GLEW::start_glew();

            // Some openGL settings
            // tell GL to only draw onto a pixel if the shape is closer to the viewer
            glEnable(GL_DEPTH_TEST); // enable depth-testing
            glDepthFunc(GL_LESS);		 // depth-testing interprets a smaller value as "closer"

            // ====================
            // VBO - VAO GENERATION
            // ====================
            // 1. Vertex data for positions, colors and texture coordinates
            GLfloat vertices[] = {
                // positions          // colors           // texture 2D coords
                 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
                 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
                -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
                -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
            };

            // Generate data for indices
            GLuint indices[] = {  // note that we start from 0!
                0, 1, 3,  // first Triangle
                1, 2, 3   // second Triangle
            };

            // Generate tightly-packed VBO and then the Indexed VBO
            // 2,3 & 4
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // 5. Generate and bind VAO
            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            // Indexed VBO
            // IMPORTANT: Since they are directly stored in VAO, the corresponding 
            // VAO should be bound first.
            GLuint indexed_vbo;
            glGenBuffers(1, &indexed_vbo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexed_vbo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // 6. Define and associate the vertex attributes to the vbo
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            //positions
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            //colors
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
            //textures
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
            glEnableVertexAttribArray(2);

            glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

            // ====================
            //    TEXTURE SETUP
            // ====================
            // 1. Load the corresponding image
            std::string image_file_path("resources\\container.jpg");
            bool is_png = image_file_path.find(".png") != std::string::npos;

            int width, height, nrChannels;
            unsigned char* data = stbi_load(image_file_path.c_str(), &width, &height, &nrChannels, 0);
            if (!data)
                std::cerr << "Failed to load texture image!!!" << std::endl;
            
            // 2. Generate the texture object like any other OpenGL object
            GLuint texture_obj;
            glGenTextures(1, &texture_obj);

            // 3. Bind it to a specific target. It is GL_TEXTURE_2D here.
            // RULE: texture_obj should and can NOT be bound to any other target after this point
            glBindTexture(GL_TEXTURE_2D, texture_obj);
            // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

            // 4. Set the sampling parameters
            // Wrapping (edge value sampling)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // Filtering
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // 5. Get the storage for the texture
            // Mutable storage in this case
            if (is_png)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            // 6. Storage contents can now be altered after this point.
            // It is often a good practice to generate a mipmap
            glGenerateMipmap(GL_TEXTURE_2D);
            
            // 7. You can now free the loaded image data.
            stbi_image_free(data);

            // ====================
            //      SHADERS
            // ====================
            // 8. Define the sources for the vertex and fragment shader
            const char* vertex_shader_source_str =
                "#version 330 core\n"
                "layout (location = 0) in vec3 vertex_position;\n"
                "layout (location = 1) in vec3 vertex_color;\n"
                "layout (location = 2) in vec2 vertex_texture_position;\n"
                "out vec3 color;\n"
                "out vec2 textureCoord;\n"
                "void main()\n"
                "{\n"
                "   gl_Position = vec4(vertex_position.x, vertex_position.y, vertex_position.z, 1.0);\n"
                "   color = vertex_color;\n"
                "   textureCoord = vec2(vertex_texture_position.x, vertex_texture_position.y);\n"
                "}\0";

            // 9. Fragment shader setup
            const char* fragment_shader_source_str =
                "#version 330 core\n"
                "in vec3 color;\n"
                "in vec2 textureCoord;\n"
                "out vec4 frag_color;\n"
                "uniform sampler2D texture1;\n" // texture sampler
                "void main()\n"
                "{\n"
                "   frag_color = texture(texture1, textureCoord) * vec4(color, 1.0);\n"
                "}\0";

            // 10-a. Create the vertex shader
            GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

            // 10-b. Attach the source string to the shader & compile
            const GLchar* vertex_shader_source = (const GLchar*)vertex_shader_source_str;
            glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
            glCompileShader(vertex_shader);

            // 10-c. Check for the errors
            int success = -1;
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 10-d REpeat steps 10-a, b and c for the fragment shader
            GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            const GLchar* fragment_shader_source = (const GLchar*)fragment_shader_source_str;
            glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
            glCompileShader(fragment_shader);
            // error check
            success = -1;
            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 10-e. Create the program to attach defined shaders
            GLuint shader_programme = glCreateProgram();

            // 10-f. Attach & link the shaders to the program
            glAttachShader(shader_programme, vertex_shader);
            glAttachShader(shader_programme, fragment_shader);
            glLinkProgram(shader_programme);

            // 10-g. Check against the linking errors
            success = -1;
            glGetProgramiv(shader_programme, GL_LINK_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetProgramInfoLog(shader_programme, 512, NULL, infoLog);
                std::cerr << "ERROR: Shader programme linking failed: " << infoLog << std::endl;
                return -1;
            }

            // 10-h. Deleting shader at this point does not hurt
            glDeleteShader(vertex_shader);

            // ====================
            //      MAIN UI LOOP
            // ====================
            /* Loop until the user closes the window */
            while (!glfwWindowShouldClose(window))
            {
                // fps counter
                Utility::GLFW::update_fps_counter(window);

                /* Render here */
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// scene background color
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // 11. Make the texture drawn
                glUseProgram(shader_programme);
                glBindTexture(GL_TEXTURE_2D, texture_obj);
                glBindVertexArray(vao);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                /* Swap front and back buffers */
                /* update other events like input handling */
                glfwSwapBuffers(window);

                /* Poll for and process events */
                /* put the stuff we've been drawing onto the display */
                glfwPollEvents();
            }

            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
            glDeleteBuffers(1, &indexed_vbo);
            glDeleteProgram(shader_programme);

            glfwTerminate();
            return 0;
        }

        int RunWithMultipleTexturesCombined()
        {
            // Initialize the glfw & glew
            GLFWwindow* window = Utility::GLFW::start_glfw();
            Utility::GLEW::start_glew();

            // Some openGL settings
            // tell GL to only draw onto a pixel if the shape is closer to the viewer
            glEnable(GL_DEPTH_TEST); // enable depth-testing
            glDepthFunc(GL_LESS);		 // depth-testing interprets a smaller value as "closer"

            // ====================
            // VBO - VAO GENERATION
            // ====================
            // 1. Vertex data for positions, colors and texture coordinates
            GLfloat vertices[] = {
                // positions          // colors           // texture 2D coords
                 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
                 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
                -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
                -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
            };

            // Generate data for indices
            GLuint indices[] = {  // note that we start from 0!
                0, 1, 3,  // first Triangle
                1, 2, 3   // second Triangle
            };

            // Generate tightly-packed VBO and then the Indexed VBO
            // 2,3 & 4
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // 5. Generate and bind VAO
            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            // Indexed VBO
            // IMPORTANT: Since they are directly stored in VAO, the corresponding 
            // VAO should be bound first.
            GLuint indexed_vbo;
            glGenBuffers(1, &indexed_vbo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexed_vbo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // 6. Define and associate the vertex attributes to the vbo
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            //positions
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            //colors
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
            //textures
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
            glEnableVertexAttribArray(2);

            glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

            // ====================
            //    TEXTURE SETUP
            // ====================
            
            // texture 1
            // ---------
            
            // 1. Load the corresponding image
            std::string image_file_path("resources\\container.jpg");
            bool is_png = image_file_path.find(".png") != std::string::npos;

            int width, height, nrChannels;
            unsigned char* data = stbi_load(image_file_path.c_str(), &width, &height, &nrChannels, 0);
            if (!data)
                std::cerr << "Failed to load texture image!!!" << std::endl;

            // 2. Generate the texture object like any other OpenGL object
            GLuint texture_obj;
            glGenTextures(1, &texture_obj);

            // 3. Bind it to a specific target. It is GL_TEXTURE_2D here.
            // RULE: texture_obj should and can NOT be bound to any other target after this point
            glBindTexture(GL_TEXTURE_2D, texture_obj);
            // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

            // 4. Set the sampling parameters
            // Wrapping (edge value sampling)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // Filtering
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // 5. Get the storage for the texture
            // Mutable storage in this case
            if (is_png)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            // 6. Storage contents can now be altered after this point.
            // It is often a good practice to generate a mipmap
            glGenerateMipmap(GL_TEXTURE_2D);

            // 7. You can now free the loaded image data.
            stbi_image_free(data);

            // texture 2
            // ---------
            // Repeat steps 1-7 for the second texture

            image_file_path = "resources\\awesomeface.png";
            is_png = image_file_path.find(".png") != std::string::npos;
            stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
            data = stbi_load(image_file_path.c_str(), &width, &height, &nrChannels, 0);
            if (!data)
                std::cerr << "Failed to load texture image!!!" << std::endl;
            
            // Texture object
            GLuint texture_obj2;
            glGenTextures(1, &texture_obj2);
            glBindTexture(GL_TEXTURE_2D, texture_obj2);

            // Sampling parameters
            // wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Storage
            if (is_png)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            // Mipmap setup
            glGenerateMipmap(GL_TEXTURE_2D);
            
            // free raw data
            stbi_image_free(data);
            

            // ====================
            //      SHADERS
            // ====================
            // 8. Define the sources for the vertex and fragment shader
            const char* vertex_shader_source_str =
                "#version 330 core\n"
                "layout (location = 0) in vec3 vertex_position;\n"
                "layout (location = 1) in vec3 vertex_color;\n"
                "layout (location = 2) in vec2 vertex_texture_position;\n"
                "out vec3 color;\n"
                "out vec2 textureCoord;\n"
                "void main()\n"
                "{\n"
                "   gl_Position = vec4(vertex_position.x, vertex_position.y, vertex_position.z, 1.0);\n"
                "   color = vertex_color;\n"
                "   textureCoord = vec2(vertex_texture_position.x, vertex_texture_position.y);\n"
                "}\0";

            // 9. Fragment shader setup
            const char* fragment_shader_source_str =
                "#version 330 core\n"
                "in vec3 color;\n"
                "in vec2 textureCoord;\n"
                "out vec4 frag_color;\n"
                "uniform sampler2D texture1;\n" // texture sampler
                "uniform sampler2D texture2;\n"
                "void main()\n"
                "{\n"
                "   frag_color = mix(texture(texture1, textureCoord), texture(texture2, textureCoord), 0.2);\n"
                "}\0";

            // 10-a. Create the vertex shader
            GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

            // 10-b. Attach the source string to the shader & compile
            const GLchar* vertex_shader_source = (const GLchar*)vertex_shader_source_str;
            glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
            glCompileShader(vertex_shader);

            // 10-c. Check for the errors
            int success = -1;
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 10-d REpeat steps 10-a, b and c for the fragment shader
            GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            const GLchar* fragment_shader_source = (const GLchar*)fragment_shader_source_str;
            glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
            glCompileShader(fragment_shader);
            // error check
            success = -1;
            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 10-e. Create the program to attach defined shaders
            GLuint shader_programme = glCreateProgram();

            // 10-f. Attach & link the shaders to the program
            glAttachShader(shader_programme, vertex_shader);
            glAttachShader(shader_programme, fragment_shader);
            glLinkProgram(shader_programme);

            // 10-g. Check against the linking errors
            success = -1;
            glGetProgramiv(shader_programme, GL_LINK_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetProgramInfoLog(shader_programme, 512, NULL, infoLog);
                std::cerr << "ERROR: Shader programme linking failed: " << infoLog << std::endl;
                return -1;
            }

            // ====================
            //  TEXTURE UNIT SETUP
            // ====================
            // Has to be done once
            
            // Since we make a modification on the shader, call glUseProgram for the shader programme
            glUseProgram(shader_programme);

            // Tell the shader what shader belongs to which texture object
            // get the location of the sampler variable named "texture1" and 
            // set the value of that sampler to the corresponding value of the texture image unit
            glUniform1i(glGetUniformLocation(shader_programme, "texture1"), 0);
            glUniform1i(glGetUniformLocation(shader_programme, "texture2"), 1);

            // NOTE: Make sure you activate the correct texture image unit for the related texture.

            // 10-h. Deleting shader at this point does not hurt
            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);

            // ====================
            //      MAIN UI LOOP
            // ====================
            /* Loop until the user closes the window */
            while (!glfwWindowShouldClose(window))
            {
                // fps counter
                Utility::GLFW::update_fps_counter(window);

                /* Render here */
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// scene background color
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Activate the texture image units and bind the associated textures
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture_obj);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, texture_obj2);

                // render
                glUseProgram(shader_programme);
                glBindVertexArray(vao);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                /* Swap front and back buffers */
                /* update other events like input handling */
                glfwSwapBuffers(window);

                /* Poll for and process events */
                /* put the stuff we've been drawing onto the display */
                glfwPollEvents();
            }

            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
            glDeleteBuffers(1, &indexed_vbo);
            glDeleteProgram(shader_programme);

            glfwTerminate();
            return 0;
        }
    }

    namespace getting_started::transformations
    {
        void Translate()
        {
            // identity matrix
            glm::mat4 id_matrix = glm::mat4(1.0f);
            
            // a translation matrix
            glm::vec3 translation_vector = glm::vec3(1.0f, 1.0f, 0.0f);
            // to translate 1 unit in x, 1 unit in y and 0 unit in z direction
            glm::mat4 translation_matrix = glm::translate(id_matrix, translation_vector);

            // Now translate
            glm::vec4 source(1.0f, 0.0f, 0.0f, 1.0f);
            source = translation_matrix * source;
            std::cout << "Result: {" 
                << source.x << ", " 
                << source.y << ", " 
                << source.z << ", " 
                << source.w 
             << "}\n";
        }

        void Rotate()
        {
            // identity matrix
            glm::mat4 id_matrix = glm::mat4(1.0f);

            // Rotation around X axis
            glm::vec3 x_axis_unit_vec = glm::vec3(1.0f, 0.0f, 0.0f);
            // rotation by 90 degrees around x axis
            glm::mat4 x_axis_rotation_matrix = glm::rotate(id_matrix, glm::radians(90.0f), x_axis_unit_vec);

            // Now rotate
            glm::vec4 source(1.0f, 1.0f, 0.0f, 1.0f);
            source = x_axis_rotation_matrix * source;
        }

        void Scale()
        {
            // identity matrix
            glm::mat4 id_matrix = glm::mat4(1.0f);

            // Uniform scaling
            glm::vec3 scaling_vector = glm::vec3(0.5f, 0.5f, 0.5f);
            glm::mat4 scaling_matrix = glm::scale(id_matrix, scaling_vector);

            // Scale the source
            glm::vec4 source(6.0f, 6.0f, 6.0f, 1.0f);
            source = scaling_matrix * source;
            std::cout << "Result: {"
                << source.x << ", "
                << source.y << ", "
                << source.z << ", "
                << source.w
                << "}\n";
        }

        int StaticTransformation()
        {
            // Initialize the glfw & glew
            GLFWwindow* window = Utility::GLFW::start_glfw();
            Utility::GLEW::start_glew();

            // Some openGL settings
            // tell GL to only draw onto a pixel if the shape is closer to the viewer
            glEnable(GL_DEPTH_TEST); // enable depth-testing
            glDepthFunc(GL_LESS);		 // depth-testing interprets a smaller value as "closer"

            // ====================
            // VBO - VAO GENERATION
            // ====================
            // 1. Vertex data for positions, colors and texture coordinates
            GLfloat vertices[] = {
                // positions          // colors           // texture 2D coords
                 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
                 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
                -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
                -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
            };

            // Generate data for indices
            GLuint indices[] = {  // note that we start from 0!
                0, 1, 3,  // first Triangle
                1, 2, 3   // second Triangle
            };

            // Generate tightly-packed VBO and then the Indexed VBO
            // 2,3 & 4
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // 5. Generate and bind VAO
            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            // Indexed VBO
            // IMPORTANT: Since they are directly stored in VAO, the corresponding 
            // VAO should be bound first.
            GLuint indexed_vbo;
            glGenBuffers(1, &indexed_vbo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexed_vbo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // 6. Define and associate the vertex attributes to the vbo
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            //positions
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            //colors
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
            //textures
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
            glEnableVertexAttribArray(2);

            glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

            // ====================
            //    TEXTURE SETUP
            // ====================

            // texture 1
            // ---------

            // 1. Load the corresponding image
            std::string image_file_path("resources\\container.jpg");
            bool is_png = image_file_path.find(".png") != std::string::npos;

            int width, height, nrChannels;
            unsigned char* data = stbi_load(image_file_path.c_str(), &width, &height, &nrChannels, 0);
            if (!data)
                std::cerr << "Failed to load texture image!!!" << std::endl;

            // 2. Generate the texture object like any other OpenGL object
            GLuint texture_obj;
            glGenTextures(1, &texture_obj);

            // 3. Bind it to a specific target. It is GL_TEXTURE_2D here.
            // RULE: texture_obj should and can NOT be bound to any other target after this point
            glBindTexture(GL_TEXTURE_2D, texture_obj);
            // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

            // 4. Set the sampling parameters
            // Wrapping (edge value sampling)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // Filtering
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // 5. Get the storage for the texture
            // Mutable storage in this case
            if (is_png)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            // 6. Storage contents can now be altered after this point.
            // It is often a good practice to generate a mipmap
            glGenerateMipmap(GL_TEXTURE_2D);

            // 7. You can now free the loaded image data.
            stbi_image_free(data);

            // texture 2
            // ---------
            // Repeat steps 1-7 for the second texture

            image_file_path = "resources\\awesomeface.png";
            is_png = image_file_path.find(".png") != std::string::npos;
            stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
            data = stbi_load(image_file_path.c_str(), &width, &height, &nrChannels, 0);
            if (!data)
                std::cerr << "Failed to load texture image!!!" << std::endl;

            // Texture object
            GLuint texture_obj2;
            glGenTextures(1, &texture_obj2);
            glBindTexture(GL_TEXTURE_2D, texture_obj2);

            // Sampling parameters
            // wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Storage
            if (is_png)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            // Mipmap setup
            glGenerateMipmap(GL_TEXTURE_2D);

            // free raw data
            stbi_image_free(data);


            // ====================
            //      SHADERS
            // ====================
            // 8. Define the sources for the vertex and fragment shader
            const char* vertex_shader_source_str =
                "#version 330 core\n"
                "layout (location = 0) in vec3 vertex_position;\n"
                "layout (location = 1) in vec3 vertex_color;\n"
                "layout (location = 2) in vec2 vertex_texture_position;\n"
                "out vec3 color;\n"
                "out vec2 textureCoord;\n"
                "uniform mat4 transformation;\n"
                "void main()\n"
                "{\n"
                "   gl_Position = transformation * vec4(vertex_position, 1.0);\n"
                "   color = vertex_color;\n"
                "   textureCoord = vec2(vertex_texture_position.x, vertex_texture_position.y);\n"
                "}\0";

            // 9. Fragment shader setup
            const char* fragment_shader_source_str =
                "#version 330 core\n"
                "in vec3 color;\n"
                "in vec2 textureCoord;\n"
                "out vec4 frag_color;\n"
                "uniform sampler2D texture1;\n" // texture sampler
                "uniform sampler2D texture2;\n"
                "void main()\n"
                "{\n"
                "   frag_color = mix(texture(texture1, textureCoord), texture(texture2, textureCoord), 0.2);\n"
                "}\0";

            // 10-a. Create the vertex shader
            GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

            // 10-b. Attach the source string to the shader & compile
            const GLchar* vertex_shader_source = (const GLchar*)vertex_shader_source_str;
            glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
            glCompileShader(vertex_shader);

            // 10-c. Check for the errors
            int success = -1;
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 10-d REpeat steps 10-a, b and c for the fragment shader
            GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            const GLchar* fragment_shader_source = (const GLchar*)fragment_shader_source_str;
            glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
            glCompileShader(fragment_shader);
            // error check
            success = -1;
            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 10-e. Create the program to attach defined shaders
            GLuint shader_programme = glCreateProgram();

            // 10-f. Attach & link the shaders to the program
            glAttachShader(shader_programme, vertex_shader);
            glAttachShader(shader_programme, fragment_shader);
            glLinkProgram(shader_programme);

            // 10-g. Check against the linking errors
            success = -1;
            glGetProgramiv(shader_programme, GL_LINK_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetProgramInfoLog(shader_programme, 512, NULL, infoLog);
                std::cerr << "ERROR: Shader programme linking failed: " << infoLog << std::endl;
                return -1;
            }

            // ====================
            //  TEXTURE UNIT SETUP
            // ====================
            // Has to be done once

            // Since we make a modification on the shader, call glUseProgram for the shader programme
            glUseProgram(shader_programme);

            // Tell the shader what shader belongs to which texture object
            // get the location of the sampler variable named "texture1" and 
            // set the value of that sampler to the corresponding value of the texture image unit
            glUniform1i(glGetUniformLocation(shader_programme, "texture1"), 0);
            glUniform1i(glGetUniformLocation(shader_programme, "texture2"), 1);

            // NOTE: Make sure you activate the correct texture image unit for the related texture.


            // ====================
            //  TRANSFORMATION SETUP
            // ====================
            glm::mat4 identity_matrix = glm::mat4(1.0f);
            // a translation matrix
            glm::vec3 translation_vector = glm::vec3(-0.25f, -0.25f, 0.0f);
            // to translate 1 unit in x, 1 unit in y and 0 unit in z direction
            glm::mat4 translation_matrix = glm::translate(identity_matrix, translation_vector);
            
            // Rotation around X axis
            glm::vec3 z_axis_unit_vec = glm::vec3(0.0f, 0.0f, 1.0f);
            // rotation by 90 degrees around z axis
            glm::mat4 z_axis_rotation_matrix = glm::rotate(identity_matrix, glm::radians(90.0f), z_axis_unit_vec);

            // Final transformation matrix
            glm::mat4 final_transformation = translation_matrix * z_axis_rotation_matrix;

            // We need to pass this info to the shader so use the shader programme first
            glUseProgram(shader_programme);
            // Get the location of the transformation matrix in the shader
            GLuint transformation_loc = glGetUniformLocation(shader_programme, "transformation");
            // Set it's data
            glUniformMatrix4fv(transformation_loc, 1, GL_FALSE, glm::value_ptr(final_transformation));

            // 10-h. Deleting shader at this point does not hurt
            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);

            // ====================
            //      MAIN UI LOOP
            // ====================
            /* Loop until the user closes the window */
            while (!glfwWindowShouldClose(window))
            {
                // fps counter
                Utility::GLFW::update_fps_counter(window);

                /* Render here */
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// scene background color
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Activate the texture image units and bind the associated textures
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture_obj);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, texture_obj2);

                // render
                glUseProgram(shader_programme);
                glBindVertexArray(vao);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                /* Swap front and back buffers */
                /* update other events like input handling */
                glfwSwapBuffers(window);

                /* Poll for and process events */
                /* put the stuff we've been drawing onto the display */
                glfwPollEvents();
            }

            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
            glDeleteBuffers(1, &indexed_vbo);
            glDeleteProgram(shader_programme);

            glfwTerminate();
            return 0;
        }

        int DynamicTransformation()
        {
            // Initialize the glfw & glew
            GLFWwindow* window = Utility::GLFW::start_glfw();
            Utility::GLEW::start_glew();

            // Some openGL settings
            // tell GL to only draw onto a pixel if the shape is closer to the viewer
            glEnable(GL_DEPTH_TEST); // enable depth-testing
            glDepthFunc(GL_LESS);		 // depth-testing interprets a smaller value as "closer"

            // ====================
            // VBO - VAO GENERATION
            // ====================
            // 1. Vertex data for positions, colors and texture coordinates
            GLfloat vertices[] = {
                // positions          // colors           // texture 2D coords
                 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
                 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
                -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
                -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
            };

            // Generate data for indices
            GLuint indices[] = {  // note that we start from 0!
                0, 1, 3,  // first Triangle
                1, 2, 3   // second Triangle
            };

            // Generate tightly-packed VBO and then the Indexed VBO
            // 2,3 & 4
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // 5. Generate and bind VAO
            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            // Indexed VBO
            // IMPORTANT: Since they are directly stored in VAO, the corresponding 
            // VAO should be bound first.
            GLuint indexed_vbo;
            glGenBuffers(1, &indexed_vbo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexed_vbo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // 6. Define and associate the vertex attributes to the vbo
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            //positions
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            //colors
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
            //textures
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
            glEnableVertexAttribArray(2);

            glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

            // ====================
            //    TEXTURE SETUP
            // ====================

            // texture 1
            // ---------

            // 1. Load the corresponding image
            std::string image_file_path("resources\\container.jpg");
            bool is_png = image_file_path.find(".png") != std::string::npos;

            int width, height, nrChannels;
            unsigned char* data = stbi_load(image_file_path.c_str(), &width, &height, &nrChannels, 0);
            if (!data)
                std::cerr << "Failed to load texture image!!!" << std::endl;

            // 2. Generate the texture object like any other OpenGL object
            GLuint texture_obj;
            glGenTextures(1, &texture_obj);

            // 3. Bind it to a specific target. It is GL_TEXTURE_2D here.
            // RULE: texture_obj should and can NOT be bound to any other target after this point
            glBindTexture(GL_TEXTURE_2D, texture_obj);
            // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

            // 4. Set the sampling parameters
            // Wrapping (edge value sampling)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // Filtering
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // 5. Get the storage for the texture
            // Mutable storage in this case
            if (is_png)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            // 6. Storage contents can now be altered after this point.
            // It is often a good practice to generate a mipmap
            glGenerateMipmap(GL_TEXTURE_2D);

            // 7. You can now free the loaded image data.
            stbi_image_free(data);

            // texture 2
            // ---------
            // Repeat steps 1-7 for the second texture

            image_file_path = "resources\\awesomeface.png";
            is_png = image_file_path.find(".png") != std::string::npos;
            stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
            data = stbi_load(image_file_path.c_str(), &width, &height, &nrChannels, 0);
            if (!data)
                std::cerr << "Failed to load texture image!!!" << std::endl;

            // Texture object
            GLuint texture_obj2;
            glGenTextures(1, &texture_obj2);
            glBindTexture(GL_TEXTURE_2D, texture_obj2);

            // Sampling parameters
            // wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Storage
            if (is_png)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            // Mipmap setup
            glGenerateMipmap(GL_TEXTURE_2D);

            // free raw data
            stbi_image_free(data);


            // ====================
            //      SHADERS
            // ====================
            // 8. Define the sources for the vertex and fragment shader
            const char* vertex_shader_source_str =
                "#version 330 core\n"
                "layout (location = 0) in vec3 vertex_position;\n"
                "layout (location = 1) in vec3 vertex_color;\n"
                "layout (location = 2) in vec2 vertex_texture_position;\n"
                "out vec3 color;\n"
                "out vec2 textureCoord;\n"
                "uniform mat4 transformation;\n"
                "void main()\n"
                "{\n"
                "   gl_Position = transformation * vec4(vertex_position, 1.0);\n"
                "   color = vertex_color;\n"
                "   textureCoord = vec2(vertex_texture_position.x, vertex_texture_position.y);\n"
                "}\0";

            // 9. Fragment shader setup
            const char* fragment_shader_source_str =
                "#version 330 core\n"
                "in vec3 color;\n"
                "in vec2 textureCoord;\n"
                "out vec4 frag_color;\n"
                "uniform sampler2D texture1;\n" // texture sampler
                "uniform sampler2D texture2;\n"
                "void main()\n"
                "{\n"
                "   frag_color = mix(texture(texture1, textureCoord), texture(texture2, textureCoord), 0.2);\n"
                "}\0";

            // 10-a. Create the vertex shader
            GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

            // 10-b. Attach the source string to the shader & compile
            const GLchar* vertex_shader_source = (const GLchar*)vertex_shader_source_str;
            glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
            glCompileShader(vertex_shader);

            // 10-c. Check for the errors
            int success = -1;
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 10-d REpeat steps 10-a, b and c for the fragment shader
            GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            const GLchar* fragment_shader_source = (const GLchar*)fragment_shader_source_str;
            glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
            glCompileShader(fragment_shader);
            // error check
            success = -1;
            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 10-e. Create the program to attach defined shaders
            GLuint shader_programme = glCreateProgram();

            // 10-f. Attach & link the shaders to the program
            glAttachShader(shader_programme, vertex_shader);
            glAttachShader(shader_programme, fragment_shader);
            glLinkProgram(shader_programme);

            // 10-g. Check against the linking errors
            success = -1;
            glGetProgramiv(shader_programme, GL_LINK_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetProgramInfoLog(shader_programme, 512, NULL, infoLog);
                std::cerr << "ERROR: Shader programme linking failed: " << infoLog << std::endl;
                return -1;
            }

            // ====================
            //  TEXTURE UNIT SETUP
            // ====================
            // Has to be done once

            // Since we make a modification on the shader, call glUseProgram for the shader programme
            glUseProgram(shader_programme);

            // Tell the shader what shader belongs to which texture object
            // get the location of the sampler variable named "texture1" and 
            // set the value of that sampler to the corresponding value of the texture image unit
            glUniform1i(glGetUniformLocation(shader_programme, "texture1"), 0);
            glUniform1i(glGetUniformLocation(shader_programme, "texture2"), 1);

            // NOTE: Make sure you activate the correct texture image unit for the related texture.


            // ====================
            //  TRANSFORMATION SETUP
            // ====================
            glm::mat4 identity_matrix = glm::mat4(1.0f);
            // a translation matrix
            glm::vec3 translation_vector = glm::vec3(-0.25f, -0.25f, 0.0f);
            // to translate 1 unit in x, 1 unit in y and 0 unit in z direction
            glm::mat4 translation_matrix = glm::translate(identity_matrix, translation_vector);

            // Let's carry over the dynamic part under the main ui loop
            
            // 10-h. Deleting shader at this point does not hurt
            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);

            // ====================
            //      MAIN UI LOOP
            // ====================
            /* Loop until the user closes the window */
            while (!glfwWindowShouldClose(window))
            {
                // fps counter
                Utility::GLFW::update_fps_counter(window);

                /* Render here */
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// scene background color
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Activate the texture image units and bind the associated textures
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture_obj);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, texture_obj2);

                // render
                glUseProgram(shader_programme);

                // Rotate the container dynamically
                // Rotation around X axis
                glm::vec3 z_axis_unit_vec = glm::vec3(0.0f, 0.0f, 1.0f);
                // rotation by 90 degrees around z axis - time dependent
                glm::mat4 z_axis_rotation_matrix = glm::rotate(identity_matrix, (float)glfwGetTime(), z_axis_unit_vec);

                // Final transformation matrix
                glm::mat4 final_transformation = translation_matrix * z_axis_rotation_matrix;

                // We need to pass this info to the shader so use the shader programme first
                glUseProgram(shader_programme);
                // Get the location of the transformation matrix in the shader
                GLuint transformation_loc = glGetUniformLocation(shader_programme, "transformation");
                // Set it's data
                glUniformMatrix4fv(transformation_loc, 1, GL_FALSE, glm::value_ptr(final_transformation));


                // Draw
                glBindVertexArray(vao);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                /* Swap front and back buffers */
                /* update other events like input handling */
                glfwSwapBuffers(window);

                /* Poll for and process events */
                /* put the stuff we've been drawing onto the display */
                glfwPollEvents();
            }

            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
            glDeleteBuffers(1, &indexed_vbo);
            glDeleteProgram(shader_programme);

            glfwTerminate();
            return 0;
        }

    }

    namespace getting_started::coordinate_systems
    {
        int Rotate2Din3DOnce()
        {
            // Initialize the glfw & glew
            GLFWwindow* window = Utility::GLFW::start_glfw();
            Utility::GLEW::start_glew();

            // Some openGL settings
            // tell GL to only draw onto a pixel if the shape is closer to the viewer
            glEnable(GL_DEPTH_TEST); // enable depth-testing
            glDepthFunc(GL_LESS);		 // depth-testing interprets a smaller value as "closer"

            // ====================
            // VBO - VAO GENERATION
            // ====================
            // 1. Vertex data for positions, colors and texture coordinates
            GLfloat vertices[] = {
                // positions        // texture 2D coords
                 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
                 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
                -0.5f,  0.5f, 0.0f, 0.0f, 1.0f  // top left 
            };

            // Generate data for indices
            GLuint indices[] = {  // note that we start from 0!
                0, 1, 3,  // first Triangle
                1, 2, 3   // second Triangle
            };

            // Generate tightly-packed VBO and then the Indexed VBO
            // 2,3 & 4
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // 5. Generate and bind VAO
            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            // Indexed VBO
            // IMPORTANT: Since they are directly stored in VAO, the corresponding 
            // VAO should be bound first.
            GLuint indexed_vbo;
            glGenBuffers(1, &indexed_vbo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexed_vbo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // 6. Define and associate the vertex attributes to the vbo
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            //positions
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            //textures
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

            // ====================
            //    TEXTURE SETUP
            // ====================

            // texture 1
            // ---------

            // 1. Load the corresponding image
            std::string image_file_path("resources\\container.jpg");
            bool is_png = image_file_path.find(".png") != std::string::npos;

            int width, height, nrChannels;
            unsigned char* data = stbi_load(image_file_path.c_str(), &width, &height, &nrChannels, 0);
            if (!data)
                std::cerr << "Failed to load texture image!!!" << std::endl;

            // 2. Generate the texture object like any other OpenGL object
            GLuint texture_obj;
            glGenTextures(1, &texture_obj);

            // 3. Bind it to a specific target. It is GL_TEXTURE_2D here.
            // RULE: texture_obj should and can NOT be bound to any other target after this point
            glBindTexture(GL_TEXTURE_2D, texture_obj);
            // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

            // 4. Set the sampling parameters
            // Wrapping (edge value sampling)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // Filtering
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // 5. Get the storage for the texture
            // Mutable storage in this case
            if (is_png)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            // 6. Storage contents can now be altered after this point.
            // It is often a good practice to generate a mipmap
            glGenerateMipmap(GL_TEXTURE_2D);

            // 7. You can now free the loaded image data.
            stbi_image_free(data);

            // texture 2
            // ---------
            // Repeat steps 1-7 for the second texture

            image_file_path = "resources\\awesomeface.png";
            is_png = image_file_path.find(".png") != std::string::npos;
            stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
            data = stbi_load(image_file_path.c_str(), &width, &height, &nrChannels, 0);
            if (!data)
                std::cerr << "Failed to load texture image!!!" << std::endl;

            // Texture object
            GLuint texture_obj2;
            glGenTextures(1, &texture_obj2);
            glBindTexture(GL_TEXTURE_2D, texture_obj2);

            // Sampling parameters
            // wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Storage
            if (is_png)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            // Mipmap setup
            glGenerateMipmap(GL_TEXTURE_2D);

            // free raw data
            stbi_image_free(data);


            // ====================
            //      SHADERS
            // ====================
            // 8. Define the sources for the vertex and fragment shader
            const char* vertex_shader_source_str =
                "#version 330 core\n"
                "layout (location = 0) in vec3 vertex_position;\n"
                "layout (location = 1) in vec2 vertex_texture_position;\n"
                "out vec2 textureCoord;\n"
                "uniform mat4 model_matrix;\n"
                "uniform mat4 view_matrix;\n"
                "uniform mat4 projection_matrix;\n"
                "void main()\n"
                "{\n"
                "   gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vertex_position, 1.0);\n"
                "   textureCoord = vec2(vertex_texture_position.x, vertex_texture_position.y);\n"
                "}\0";

            // 9. Fragment shader setup
            const char* fragment_shader_source_str =
                "#version 330 core\n"
                "in vec2 textureCoord;\n"
                "out vec4 frag_color;\n"
                "uniform sampler2D texture1;\n" // texture sampler
                "uniform sampler2D texture2;\n"
                "void main()\n"
                "{\n"
                "   frag_color = mix(texture(texture1, textureCoord), texture(texture2, textureCoord), 0.2);\n"
                "}\0";

            // 10-a. Create the vertex shader
            GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

            // 10-b. Attach the source string to the shader & compile
            const GLchar* vertex_shader_source = (const GLchar*)vertex_shader_source_str;
            glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
            glCompileShader(vertex_shader);

            // 10-c. Check for the errors
            int success = -1;
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 10-d REpeat steps 10-a, b and c for the fragment shader
            GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            const GLchar* fragment_shader_source = (const GLchar*)fragment_shader_source_str;
            glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
            glCompileShader(fragment_shader);
            // error check
            success = -1;
            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 10-e. Create the program to attach defined shaders
            GLuint shader_programme = glCreateProgram();

            // 10-f. Attach & link the shaders to the program
            glAttachShader(shader_programme, vertex_shader);
            glAttachShader(shader_programme, fragment_shader);
            glLinkProgram(shader_programme);

            // 10-g. Check against the linking errors
            success = -1;
            glGetProgramiv(shader_programme, GL_LINK_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetProgramInfoLog(shader_programme, 512, NULL, infoLog);
                std::cerr << "ERROR: Shader programme linking failed: " << infoLog << std::endl;
                return -1;
            }

            // ====================
            //  TEXTURE UNIT SETUP
            // ====================
            // Has to be done once

            // Since we make a modification on the shader, call glUseProgram for the shader programme
            glUseProgram(shader_programme);

            // Tell the shader what shader belongs to which texture object
            // get the location of the sampler variable named "texture1" and 
            // set the value of that sampler to the corresponding value of the texture image unit
            glUniform1i(glGetUniformLocation(shader_programme, "texture1"), 0);
            glUniform1i(glGetUniformLocation(shader_programme, "texture2"), 1);

            // NOTE: Make sure you activate the correct texture image unit for the related texture.


            // ====================
            //  TRANSFORMATION SETUP
            // ====================
            glm::mat4 identity_matrix = glm::mat4(1.0f);
            
            // Setup the transformation matrices
            // Rotation around X axis
            glm::vec3 x_axis_unit_vec = glm::vec3(1.0f, 0.0f, 0.0f);
            // rotation by 90 degrees around x axis - time dependent
            glm::mat4 model_matrix = glm::rotate(identity_matrix, glm::radians(-55.0f), x_axis_unit_vec);
            glm::mat4 view_matrix = glm::translate(identity_matrix, glm::vec3(0.0f, 0.0f, -3.0f));
            glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

            // we need to pass this info to the shader so call use
            glUseProgram(shader_programme);
            // Get the locations in the shaders
            GLuint model_mat_loc = glGetUniformLocation(shader_programme, "model_matrix");
            GLuint view_mat_loc = glGetUniformLocation(shader_programme, "view_matrix");
            GLuint projection_mat_loc = glGetUniformLocation(shader_programme, "projection_matrix");
            // Set the data for them (two different ways: pointer or [0][0] syntax)
            glUniformMatrix4fv(model_mat_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));
            glUniformMatrix4fv(view_mat_loc, 1, GL_FALSE, &view_matrix[0][0]);
            glUniformMatrix4fv(projection_mat_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

            // 10-h. Deleting shader at this point does not hurt
            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);

            // ====================
            //      MAIN UI LOOP
            // ====================
            /* Loop until the user closes the window */
            while (!glfwWindowShouldClose(window))
            {
                // fps counter
                Utility::GLFW::update_fps_counter(window);

                /* Render here */
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// scene background color
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Activate the texture image units and bind the associated textures
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture_obj);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, texture_obj2);

                // render
                glUseProgram(shader_programme);
                glBindVertexArray(vao);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                /* Swap front and back buffers */
                /* update other events like input handling */
                glfwSwapBuffers(window);

                /* Poll for and process events */
                /* put the stuff we've been drawing onto the display */
                glfwPollEvents();
            }

            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
            glDeleteBuffers(1, &indexed_vbo);
            glDeleteProgram(shader_programme);

            glfwTerminate();
            return 0;
        }

        int RotateCubeOnce()
        {
            // Initialize the glfw & glew
            GLFWwindow* window = Utility::GLFW::start_glfw();
            Utility::GLEW::start_glew();

            // Some openGL settings
            // tell GL to only draw onto a pixel if the shape is closer to the viewer
            glEnable(GL_DEPTH_TEST); // enable depth-testing
            //glDepthFunc(GL_LESS);		 // depth-testing interprets a smaller value as "closer"

            // ====================
            // VBO - VAO GENERATION
            // ====================
            // 1. Vertex data for positions, colors and texture coordinates
            GLfloat vertices[] = {
                // coords             // texture
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
            };


            // Generate tightly-packed VBO and then the Indexed VBO
            // 2,3 & 4
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // 5. Generate and bind VAO
            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            // 6. Define and associate the vertex attributes to the vbo
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            //positions
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            //textures
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

            // ====================
            //    TEXTURE SETUP
            // ====================

            // texture 1
            // ---------

            // 1. Load the corresponding image
            std::string image_file_path("resources\\container.jpg");
            bool is_png = image_file_path.find(".png") != std::string::npos;

            int width, height, nrChannels;
            unsigned char* data = stbi_load(image_file_path.c_str(), &width, &height, &nrChannels, 0);
            if (!data)
                std::cerr << "Failed to load texture image!!!" << std::endl;

            // 2. Generate the texture object like any other OpenGL object
            GLuint texture_obj;
            glGenTextures(1, &texture_obj);

            // 3. Bind it to a specific target. It is GL_TEXTURE_2D here.
            // RULE: texture_obj should and can NOT be bound to any other target after this point
            glBindTexture(GL_TEXTURE_2D, texture_obj);
            // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

            // 4. Set the sampling parameters
            // Wrapping (edge value sampling)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // Filtering
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // 5. Get the storage for the texture
            // Mutable storage in this case
            if (is_png)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            // 6. Storage contents can now be altered after this point.
            // It is often a good practice to generate a mipmap
            glGenerateMipmap(GL_TEXTURE_2D);

            // 7. You can now free the loaded image data.
            stbi_image_free(data);

            // texture 2
            // ---------
            // Repeat steps 1-7 for the second texture

            image_file_path = "resources\\awesomeface.png";
            is_png = image_file_path.find(".png") != std::string::npos;
            stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
            data = stbi_load(image_file_path.c_str(), &width, &height, &nrChannels, 0);
            if (!data)
                std::cerr << "Failed to load texture image!!!" << std::endl;

            // Texture object
            GLuint texture_obj2;
            glGenTextures(1, &texture_obj2);
            glBindTexture(GL_TEXTURE_2D, texture_obj2);

            // Sampling parameters
            // wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Storage
            if (is_png)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            // Mipmap setup
            glGenerateMipmap(GL_TEXTURE_2D);

            // free raw data
            stbi_image_free(data);

            // ====================
            //      SHADERS
            // ====================
            // 8. Define the sources for the vertex and fragment shader
            const char* vertex_shader_source_str =
                "#version 330 core\n"
                "layout (location = 0) in vec3 vertex_position;\n"
                "layout (location = 1) in vec2 vertex_texture_position;\n"
                "out vec2 textureCoord;\n"
                "uniform mat4 model_matrix;\n"
                "uniform mat4 view_matrix;\n"
                "uniform mat4 projection_matrix;\n"
                "void main()\n"
                "{\n"
                "   gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vertex_position, 1.0);\n"
                "   textureCoord = vec2(vertex_texture_position.x, vertex_texture_position.y);\n"
                "}\0";

            // 9. Fragment shader setup
            const char* fragment_shader_source_str =
                "#version 330 core\n"
                "in vec2 textureCoord;\n"
                "out vec4 frag_color;\n"
                "uniform sampler2D texture1;\n" // texture sampler
                "uniform sampler2D texture2;\n"
                "void main()\n"
                "{\n"
                "   frag_color = mix(texture(texture1, textureCoord), texture(texture2, textureCoord), 0.2);\n"
                "}\0";

            // 10-a. Create the vertex shader
            GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

            // 10-b. Attach the source string to the shader & compile
            const GLchar* vertex_shader_source = (const GLchar*)vertex_shader_source_str;
            glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
            glCompileShader(vertex_shader);

            // 10-c. Check for the errors
            int success = -1;
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 10-d REpeat steps 10-a, b and c for the fragment shader
            GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            const GLchar* fragment_shader_source = (const GLchar*)fragment_shader_source_str;
            glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
            glCompileShader(fragment_shader);
            // error check
            success = -1;
            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 10-e. Create the program to attach defined shaders
            GLuint shader_programme = glCreateProgram();

            // 10-f. Attach & link the shaders to the program
            glAttachShader(shader_programme, vertex_shader);
            glAttachShader(shader_programme, fragment_shader);
            glLinkProgram(shader_programme);

            // 10-g. Check against the linking errors
            success = -1;
            glGetProgramiv(shader_programme, GL_LINK_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetProgramInfoLog(shader_programme, 512, NULL, infoLog);
                std::cerr << "ERROR: Shader programme linking failed: " << infoLog << std::endl;
                return -1;
            }

            // ====================
            //  TEXTURE UNIT SETUP
            // ====================
            // Has to be done once

            // Since we make a modification on the shader, call glUseProgram for the shader programme
            glUseProgram(shader_programme);

            // Tell the shader what shader belongs to which texture object
            // get the location of the sampler variable named "texture1" and 
            // set the value of that sampler to the corresponding value of the texture image unit
            glUniform1i(glGetUniformLocation(shader_programme, "texture1"), 0);
            glUniform1i(glGetUniformLocation(shader_programme, "texture2"), 1);

            // NOTE: Make sure you activate the correct texture image unit for the related texture.


            // ====================
            //  TRANSFORMATION SETUP
            // ====================
            glm::mat4 identity_matrix = glm::mat4(1.0f);

            // Setup the transformation matrices
            // Rotation around X axis
            glm::vec3 x_axis_unit_vec = glm::vec3(1.0f, 0.0f, 0.0f);
            // rotation by 90 degrees around x axis - time dependent
            glm::mat4 model_matrix = glm::rotate(identity_matrix, glm::radians(-55.0f), x_axis_unit_vec);
            glm::mat4 view_matrix = glm::translate(identity_matrix, glm::vec3(0.0f, 0.0f, -3.0f));
            glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

            // we need to pass this info to the shader so call use
            glUseProgram(shader_programme);
            // Get the locations in the shaders
            GLuint model_mat_loc = glGetUniformLocation(shader_programme, "model_matrix");
            GLuint view_mat_loc = glGetUniformLocation(shader_programme, "view_matrix");
            GLuint projection_mat_loc = glGetUniformLocation(shader_programme, "projection_matrix");
            // Set the data for them (two different ways: pointer or [0][0] syntax)
            glUniformMatrix4fv(model_mat_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));
            glUniformMatrix4fv(view_mat_loc, 1, GL_FALSE, &view_matrix[0][0]);
            glUniformMatrix4fv(projection_mat_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

            // 10-h. Deleting shader at this point does not hurt
            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);

            // ====================
            //      MAIN UI LOOP
            // ====================
            /* Loop until the user closes the window */
            while (!glfwWindowShouldClose(window))
            {
                // fps counter
                Utility::GLFW::update_fps_counter(window);

                /* Render here */
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// scene background color
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Activate the texture image units and bind the associated textures
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture_obj);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, texture_obj2);

                // render
                glUseProgram(shader_programme);
                glBindVertexArray(vao);
                glDrawArrays(GL_TRIANGLES, 0, 36);

                /* Swap front and back buffers */
                /* update other events like input handling */
                glfwSwapBuffers(window);

                /* Poll for and process events */
                /* put the stuff we've been drawing onto the display */
                glfwPollEvents();
            }

            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
            glDeleteProgram(shader_programme);

            glfwTerminate();
            return 0;
        }

        int RotateCubeContinously()
        {
            // Initialize the glfw & glew
            GLFWwindow* window = Utility::GLFW::start_glfw();
            Utility::GLEW::start_glew();

            // Some openGL settings
            // tell GL to only draw onto a pixel if the shape is closer to the viewer
            glEnable(GL_DEPTH_TEST); // enable depth-testing
            //glDepthFunc(GL_LESS);		 // depth-testing interprets a smaller value as "closer"

            // ====================
            // VBO - VAO GENERATION
            // ====================
            // 1. Vertex data for positions, colors and texture coordinates
            GLfloat vertices[] = {
                // coords             // texture
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
            };


            // Generate tightly-packed VBO and then the Indexed VBO
            // 2,3 & 4
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // 5. Generate and bind VAO
            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            // 6. Define and associate the vertex attributes to the vbo
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            //positions
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            //textures
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

            // ====================
            //    TEXTURE SETUP
            // ====================

            // texture 1
            // ---------

            // 1. Load the corresponding image
            std::string image_file_path("resources\\container.jpg");
            bool is_png = image_file_path.find(".png") != std::string::npos;

            int width, height, nrChannels;
            unsigned char* data = stbi_load(image_file_path.c_str(), &width, &height, &nrChannels, 0);
            if (!data)
                std::cerr << "Failed to load texture image!!!" << std::endl;

            // 2. Generate the texture object like any other OpenGL object
            GLuint texture_obj;
            glGenTextures(1, &texture_obj);

            // 3. Bind it to a specific target. It is GL_TEXTURE_2D here.
            // RULE: texture_obj should and can NOT be bound to any other target after this point
            glBindTexture(GL_TEXTURE_2D, texture_obj);
            // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

            // 4. Set the sampling parameters
            // Wrapping (edge value sampling)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // Filtering
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // 5. Get the storage for the texture
            // Mutable storage in this case
            if (is_png)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            // 6. Storage contents can now be altered after this point.
            // It is often a good practice to generate a mipmap
            glGenerateMipmap(GL_TEXTURE_2D);

            // 7. You can now free the loaded image data.
            stbi_image_free(data);

            // texture 2
            // ---------
            // Repeat steps 1-7 for the second texture

            image_file_path = "resources\\awesomeface.png";
            is_png = image_file_path.find(".png") != std::string::npos;
            stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
            data = stbi_load(image_file_path.c_str(), &width, &height, &nrChannels, 0);
            if (!data)
                std::cerr << "Failed to load texture image!!!" << std::endl;

            // Texture object
            GLuint texture_obj2;
            glGenTextures(1, &texture_obj2);
            glBindTexture(GL_TEXTURE_2D, texture_obj2);

            // Sampling parameters
            // wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Storage
            if (is_png)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            // Mipmap setup
            glGenerateMipmap(GL_TEXTURE_2D);

            // free raw data
            stbi_image_free(data);

            // ====================
            //      SHADERS
            // ====================
            // 8. Define the sources for the vertex and fragment shader
            const char* vertex_shader_source_str =
                "#version 330 core\n"
                "layout (location = 0) in vec3 vertex_position;\n"
                "layout (location = 1) in vec2 vertex_texture_position;\n"
                "out vec2 textureCoord;\n"
                "uniform mat4 model_matrix;\n"
                "uniform mat4 view_matrix;\n"
                "uniform mat4 projection_matrix;\n"
                "void main()\n"
                "{\n"
                "   gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vertex_position, 1.0);\n"
                "   textureCoord = vec2(vertex_texture_position.x, vertex_texture_position.y);\n"
                "}\0";

            // 9. Fragment shader setup
            const char* fragment_shader_source_str =
                "#version 330 core\n"
                "in vec2 textureCoord;\n"
                "out vec4 frag_color;\n"
                "uniform sampler2D texture1;\n" // texture sampler
                "uniform sampler2D texture2;\n"
                "void main()\n"
                "{\n"
                "   frag_color = mix(texture(texture1, textureCoord), texture(texture2, textureCoord), 0.2);\n"
                "}\0";

            // 10-a. Create the vertex shader
            GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

            // 10-b. Attach the source string to the shader & compile
            const GLchar* vertex_shader_source = (const GLchar*)vertex_shader_source_str;
            glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
            glCompileShader(vertex_shader);

            // 10-c. Check for the errors
            int success = -1;
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 10-d REpeat steps 10-a, b and c for the fragment shader
            GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            const GLchar* fragment_shader_source = (const GLchar*)fragment_shader_source_str;
            glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
            glCompileShader(fragment_shader);
            // error check
            success = -1;
            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 10-e. Create the program to attach defined shaders
            GLuint shader_programme = glCreateProgram();

            // 10-f. Attach & link the shaders to the program
            glAttachShader(shader_programme, vertex_shader);
            glAttachShader(shader_programme, fragment_shader);
            glLinkProgram(shader_programme);

            // 10-g. Check against the linking errors
            success = -1;
            glGetProgramiv(shader_programme, GL_LINK_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetProgramInfoLog(shader_programme, 512, NULL, infoLog);
                std::cerr << "ERROR: Shader programme linking failed: " << infoLog << std::endl;
                return -1;
            }

            // ====================
            //  TEXTURE UNIT SETUP
            // ====================
            // Has to be done once

            // Since we make a modification on the shader, call glUseProgram for the shader programme
            glUseProgram(shader_programme);

            // Tell the shader what shader belongs to which texture object
            // get the location of the sampler variable named "texture1" and 
            // set the value of that sampler to the corresponding value of the texture image unit
            glUniform1i(glGetUniformLocation(shader_programme, "texture1"), 0);
            glUniform1i(glGetUniformLocation(shader_programme, "texture2"), 1);

            // NOTE: Make sure you activate the correct texture image unit for the related texture.


            // ====================
            //  TRANSFORMATION SETUP
            // ====================
            glm::mat4 identity_matrix = glm::mat4(1.0f);
            // Rotation axis
            glm::vec3 rotation_axis_vec = glm::vec3(1.0f, 0.0f, 0.0f);
            // since these are non changing vectors, it's better to keep them 
            // out of the main loop
            

            // 10-h. Deleting shader at this point does not hurt
            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);

            // ====================
            //      MAIN UI LOOP
            // ====================
            /* Loop until the user closes the window */
            while (!glfwWindowShouldClose(window))
            {
                // fps counter
                Utility::GLFW::update_fps_counter(window);

                /* Render here */
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// scene background color
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Activate the texture image units and bind the associated textures
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture_obj);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, texture_obj2);

                // Transformation setup
                // continuous rotation around the rotation axis vector
                glm::mat4 model_matrix = glm::rotate(identity_matrix, (float)glfwGetTime(), rotation_axis_vec);
                glm::mat4 view_matrix = glm::translate(identity_matrix, glm::vec3(0.0f, 0.0f, -3.0f));
                glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

                // we need to pass this info to the shader so call use
                glUseProgram(shader_programme);

                // Get the locations in the shaders
                GLuint model_mat_loc = glGetUniformLocation(shader_programme, "model_matrix");
                GLuint view_mat_loc = glGetUniformLocation(shader_programme, "view_matrix");
                GLuint projection_mat_loc = glGetUniformLocation(shader_programme, "projection_matrix");
                // Set the data for them (two different ways: pointer or [0][0] syntax)
                glUniformMatrix4fv(model_mat_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));
                glUniformMatrix4fv(view_mat_loc, 1, GL_FALSE, &view_matrix[0][0]);
                glUniformMatrix4fv(projection_mat_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

                glBindVertexArray(vao);
                glDrawArrays(GL_TRIANGLES, 0, 36);

                /* Swap front and back buffers */
                /* update other events like input handling */
                glfwSwapBuffers(window);

                /* Poll for and process events */
                /* put the stuff we've been drawing onto the display */
                glfwPollEvents();
            }

            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
            glDeleteProgram(shader_programme);

            glfwTerminate();
            return 0;
        }

        int MultipleCubes(bool rotate)
        {
            // Initialize the glfw & glew
            GLFWwindow* window = Utility::GLFW::start_glfw();
            Utility::GLEW::start_glew();

            // Some openGL settings
            // tell GL to only draw onto a pixel if the shape is closer to the viewer
            glEnable(GL_DEPTH_TEST); // enable depth-testing
            //glDepthFunc(GL_LESS);		 // depth-testing interprets a smaller value as "closer"

            // ====================
            // VBO - VAO GENERATION
            // ====================
            // 1. Vertex data for positions, colors and texture coordinates
            GLfloat vertices[] = {
                // coords             // texture
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
            };

            // world space positions of our cubes
            glm::vec3 cubePositions[] = {
                glm::vec3( 0.0f,  0.0f,  0.0f),
                glm::vec3( 2.0f,  5.0f, -15.0f),
                glm::vec3(-1.5f, -2.2f, -2.5f),
                glm::vec3(-3.8f, -2.0f, -12.3f),
                glm::vec3( 2.4f, -0.4f, -3.5f),
                glm::vec3(-1.7f,  3.0f, -7.5f),
                glm::vec3( 1.3f, -2.0f, -2.5f),
                glm::vec3( 1.5f,  2.0f, -2.5f),
                glm::vec3( 1.5f,  0.2f, -1.5f),
                glm::vec3(-1.3f,  1.0f, -1.5f)
            };


            // Generate tightly-packed VBO and then the Indexed VBO
            // 2,3 & 4
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // 5. Generate and bind VAO
            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            // 6. Define and associate the vertex attributes to the vbo
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            //positions
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            //textures
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

            // ====================
            //    TEXTURE SETUP
            // ====================

            // texture 1
            // ---------

            // 1. Load the corresponding image
            std::string image_file_path("resources\\container.jpg");
            bool is_png = image_file_path.find(".png") != std::string::npos;

            int width, height, nrChannels;
            unsigned char* data = stbi_load(image_file_path.c_str(), &width, &height, &nrChannels, 0);
            if (!data)
                std::cerr << "Failed to load texture image!!!" << std::endl;

            // 2. Generate the texture object like any other OpenGL object
            GLuint texture_obj;
            glGenTextures(1, &texture_obj);

            // 3. Bind it to a specific target. It is GL_TEXTURE_2D here.
            // RULE: texture_obj should and can NOT be bound to any other target after this point
            glBindTexture(GL_TEXTURE_2D, texture_obj);
            // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

            // 4. Set the sampling parameters
            // Wrapping (edge value sampling)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // Filtering
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // 5. Get the storage for the texture
            // Mutable storage in this case
            if (is_png)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            // 6. Storage contents can now be altered after this point.
            // It is often a good practice to generate a mipmap
            glGenerateMipmap(GL_TEXTURE_2D);

            // 7. You can now free the loaded image data.
            stbi_image_free(data);

            // texture 2
            // ---------
            // Repeat steps 1-7 for the second texture

            image_file_path = "resources\\awesomeface.png";
            is_png = image_file_path.find(".png") != std::string::npos;
            stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
            data = stbi_load(image_file_path.c_str(), &width, &height, &nrChannels, 0);
            if (!data)
                std::cerr << "Failed to load texture image!!!" << std::endl;

            // Texture object
            GLuint texture_obj2;
            glGenTextures(1, &texture_obj2);
            glBindTexture(GL_TEXTURE_2D, texture_obj2);

            // Sampling parameters
            // wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Storage
            if (is_png)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            // Mipmap setup
            glGenerateMipmap(GL_TEXTURE_2D);

            // free raw data
            stbi_image_free(data);

            // ====================
            //      SHADERS
            // ====================
            // 8. Define the sources for the vertex and fragment shader
            const char* vertex_shader_source_str =
                "#version 330 core\n"
                "layout (location = 0) in vec3 vertex_position;\n"
                "layout (location = 1) in vec2 vertex_texture_position;\n"
                "out vec2 textureCoord;\n"
                "uniform mat4 model_matrix;\n"
                "uniform mat4 view_matrix;\n"
                "uniform mat4 projection_matrix;\n"
                "void main()\n"
                "{\n"
                "   gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vertex_position, 1.0);\n"
                "   textureCoord = vec2(vertex_texture_position.x, vertex_texture_position.y);\n"
                "}\0";

            // 9. Fragment shader setup
            const char* fragment_shader_source_str =
                "#version 330 core\n"
                "in vec2 textureCoord;\n"
                "out vec4 frag_color;\n"
                "uniform sampler2D texture1;\n" // texture sampler
                "uniform sampler2D texture2;\n"
                "void main()\n"
                "{\n"
                "   frag_color = mix(texture(texture1, textureCoord), texture(texture2, textureCoord), 0.2);\n"
                "}\0";

            // 10-a. Create the vertex shader
            GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

            // 10-b. Attach the source string to the shader & compile
            const GLchar* vertex_shader_source = (const GLchar*)vertex_shader_source_str;
            glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
            glCompileShader(vertex_shader);

            // 10-c. Check for the errors
            int success = -1;
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 10-d REpeat steps 10-a, b and c for the fragment shader
            GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            const GLchar* fragment_shader_source = (const GLchar*)fragment_shader_source_str;
            glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
            glCompileShader(fragment_shader);
            // error check
            success = -1;
            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 10-e. Create the program to attach defined shaders
            GLuint shader_programme = glCreateProgram();

            // 10-f. Attach & link the shaders to the program
            glAttachShader(shader_programme, vertex_shader);
            glAttachShader(shader_programme, fragment_shader);
            glLinkProgram(shader_programme);

            // 10-g. Check against the linking errors
            success = -1;
            glGetProgramiv(shader_programme, GL_LINK_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetProgramInfoLog(shader_programme, 512, NULL, infoLog);
                std::cerr << "ERROR: Shader programme linking failed: " << infoLog << std::endl;
                return -1;
            }

            // ====================
            //  TEXTURE UNIT SETUP
            // ====================
            // Has to be done once

            // Since we make a modification on the shader, call glUseProgram for the shader programme
            glUseProgram(shader_programme);

            // Tell the shader what shader belongs to which texture object
            // get the location of the sampler variable named "texture1" and 
            // set the value of that sampler to the corresponding value of the texture image unit
            glUniform1i(glGetUniformLocation(shader_programme, "texture1"), 0);
            glUniform1i(glGetUniformLocation(shader_programme, "texture2"), 1);

            // NOTE: Make sure you activate the correct texture image unit for the related texture.


            // ====================
            //  TRANSFORMATION SETUP
            // ====================
            glm::mat4 identity_matrix = glm::mat4(1.0f);
            // Rotation axis
            glm::vec3 rotation_axis_vec = glm::vec3(1.0f, 0.0f, 0.0f);
            // since these are non changing vectors, it's better to keep them 
            // out of the main loop
            glm::mat4 view_matrix = glm::translate(identity_matrix, glm::vec3(0.0f, 0.0f, -3.0f));
            glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

            // we need to pass this info to the shader so call use
            glUseProgram(shader_programme);

            // Get the locations in the shaders                
            GLuint view_mat_loc = glGetUniformLocation(shader_programme, "view_matrix");
            GLuint projection_mat_loc = glGetUniformLocation(shader_programme, "projection_matrix");
            // Set the data for them (two different ways: pointer or [0][0] syntax)
            glUniformMatrix4fv(view_mat_loc, 1, GL_FALSE, &view_matrix[0][0]);
            glUniformMatrix4fv(projection_mat_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

            // 10-h. Deleting shader at this point does not hurt
            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);

            // ====================
            //      MAIN UI LOOP
            // ====================
            /* Loop until the user closes the window */
            while (!glfwWindowShouldClose(window))
            {
                // fps counter
                Utility::GLFW::update_fps_counter(window);

                /* Render here */
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// scene background color
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Activate the texture image units and bind the associated textures
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture_obj);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, texture_obj2);

                // render each box
                glUseProgram(shader_programme);
                glBindVertexArray(vao);
                for (unsigned int i = 0; i < 10; i++)
                {
                    // calculate the model matrix for each object and pass it to shader before drawing
                    glm::mat4 model_matrix = glm::translate(identity_matrix, cubePositions[i]);
                    float angle = 20.0f * i;
                    if (rotate && i % 3 == 0)  // every 3rd iteration (including the first) we set the angle using GLFW's time function.
                        angle = glfwGetTime() * 25.0f;

                    model_matrix = glm::rotate(model_matrix, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

                    GLuint model_mat_loc = glGetUniformLocation(shader_programme, "model_matrix");
                    glUniformMatrix4fv(model_mat_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }

                /* Swap front and back buffers */
                /* update other events like input handling */
                glfwSwapBuffers(window);

                /* Poll for and process events */
                /* put the stuff we've been drawing onto the display */
                glfwPollEvents();
            }

            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
            glDeleteProgram(shader_programme);

            glfwTerminate();
            return 0;
        }

    }

    namespace getting_started::camera
    {
        void TestVectors()
        {
            glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
            glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 beforeNormalze = cameraPos - cameraTarget;
            // Normalize to its unit vector
            glm::vec3 cameraDirection = glm::normalize(beforeNormalze);

            std::cout << "beforeNormalze: " << beforeNormalze.x << ", "
                << beforeNormalze.y << ", "
                << beforeNormalze.z << "\n";

            std::cout << "CameraDir: " << cameraDirection.x << ", "
                << cameraDirection.y << ", "
                << cameraDirection.z << "\n";
        }

        int MultipleCubeCameraRotatesAroundTheScene()
        {
            // Initialize the glfw & glew
            GLFWwindow* window = Utility::GLFW::start_glfw();
            Utility::GLEW::start_glew();

            // Some openGL settings
            // tell GL to only draw onto a pixel if the shape is closer to the viewer
            glEnable(GL_DEPTH_TEST); // enable depth-testing
            //glDepthFunc(GL_LESS);		 // depth-testing interprets a smaller value as "closer"

            // ====================
            // VBO - VAO GENERATION
            // ====================
            // 1. Vertex data for positions, colors and texture coordinates
            GLfloat vertices[] = {
                // coords             // texture
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
            };

            // world space positions of our cubes
            glm::vec3 cubePositions[] = {
                glm::vec3(0.0f,  0.0f,  0.0f),
                glm::vec3(2.0f,  5.0f, -15.0f),
                glm::vec3(-1.5f, -2.2f, -2.5f),
                glm::vec3(-3.8f, -2.0f, -12.3f),
                glm::vec3(2.4f, -0.4f, -3.5f),
                glm::vec3(-1.7f,  3.0f, -7.5f),
                glm::vec3(1.3f, -2.0f, -2.5f),
                glm::vec3(1.5f,  2.0f, -2.5f),
                glm::vec3(1.5f,  0.2f, -1.5f),
                glm::vec3(-1.3f,  1.0f, -1.5f)
            };


            // Generate tightly-packed VBO and then the Indexed VBO
            // 2,3 & 4
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // 5. Generate and bind VAO
            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            // 6. Define and associate the vertex attributes to the vbo
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            //positions
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            //textures
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

            // ====================
            //    TEXTURE SETUP
            // ====================

            // texture 1
            // ---------

            // 1. Load the corresponding image
            std::string image_file_path("resources\\container.jpg");
            bool is_png = image_file_path.find(".png") != std::string::npos;

            int width, height, nrChannels;
            unsigned char* data = stbi_load(image_file_path.c_str(), &width, &height, &nrChannels, 0);
            if (!data)
                std::cerr << "Failed to load texture image!!!" << std::endl;

            // 2. Generate the texture object like any other OpenGL object
            GLuint texture_obj;
            glGenTextures(1, &texture_obj);

            // 3. Bind it to a specific target. It is GL_TEXTURE_2D here.
            // RULE: texture_obj should and can NOT be bound to any other target after this point
            glBindTexture(GL_TEXTURE_2D, texture_obj);
            // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

            // 4. Set the sampling parameters
            // Wrapping (edge value sampling)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // Filtering
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // 5. Get the storage for the texture
            // Mutable storage in this case
            if (is_png)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            // 6. Storage contents can now be altered after this point.
            // It is often a good practice to generate a mipmap
            glGenerateMipmap(GL_TEXTURE_2D);

            // 7. You can now free the loaded image data.
            stbi_image_free(data);

            // texture 2
            // ---------
            // Repeat steps 1-7 for the second texture

            image_file_path = "resources\\awesomeface.png";
            is_png = image_file_path.find(".png") != std::string::npos;
            stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
            data = stbi_load(image_file_path.c_str(), &width, &height, &nrChannels, 0);
            if (!data)
                std::cerr << "Failed to load texture image!!!" << std::endl;

            // Texture object
            GLuint texture_obj2;
            glGenTextures(1, &texture_obj2);
            glBindTexture(GL_TEXTURE_2D, texture_obj2);

            // Sampling parameters
            // wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Storage
            if (is_png)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            // Mipmap setup
            glGenerateMipmap(GL_TEXTURE_2D);

            // free raw data
            stbi_image_free(data);

            // ====================
            //      SHADERS
            // ====================
            // 8. Define the sources for the vertex and fragment shader
            const char* vertex_shader_source_str =
                "#version 330 core\n"
                "layout (location = 0) in vec3 vertex_position;\n"
                "layout (location = 1) in vec2 vertex_texture_position;\n"
                "out vec2 textureCoord;\n"
                "uniform mat4 model_matrix;\n"
                "uniform mat4 view_matrix;\n"
                "uniform mat4 projection_matrix;\n"
                "void main()\n"
                "{\n"
                "   gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vertex_position, 1.0);\n"
                "   textureCoord = vec2(vertex_texture_position.x, vertex_texture_position.y);\n"
                "}\0";

            // 9. Fragment shader setup
            const char* fragment_shader_source_str =
                "#version 330 core\n"
                "in vec2 textureCoord;\n"
                "out vec4 frag_color;\n"
                "uniform sampler2D texture1;\n" // texture sampler
                "uniform sampler2D texture2;\n"
                "void main()\n"
                "{\n"
                "   frag_color = mix(texture(texture1, textureCoord), texture(texture2, textureCoord), 0.2);\n"
                "}\0";

            // 10-a. Create the vertex shader
            GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

            // 10-b. Attach the source string to the shader & compile
            const GLchar* vertex_shader_source = (const GLchar*)vertex_shader_source_str;
            glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
            glCompileShader(vertex_shader);

            // 10-c. Check for the errors
            int success = -1;
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 10-d REpeat steps 10-a, b and c for the fragment shader
            GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            const GLchar* fragment_shader_source = (const GLchar*)fragment_shader_source_str;
            glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
            glCompileShader(fragment_shader);
            // error check
            success = -1;
            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 10-e. Create the program to attach defined shaders
            GLuint shader_programme = glCreateProgram();

            // 10-f. Attach & link the shaders to the program
            glAttachShader(shader_programme, vertex_shader);
            glAttachShader(shader_programme, fragment_shader);
            glLinkProgram(shader_programme);

            // 10-g. Check against the linking errors
            success = -1;
            glGetProgramiv(shader_programme, GL_LINK_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetProgramInfoLog(shader_programme, 512, NULL, infoLog);
                std::cerr << "ERROR: Shader programme linking failed: " << infoLog << std::endl;
                return -1;
            }

            // ====================
            //  TEXTURE UNIT SETUP
            // ====================
            // Has to be done once

            // Since we make a modification on the shader, call glUseProgram for the shader programme
            glUseProgram(shader_programme);

            // Tell the shader what shader belongs to which texture object
            // get the location of the sampler variable named "texture1" and 
            // set the value of that sampler to the corresponding value of the texture image unit
            glUniform1i(glGetUniformLocation(shader_programme, "texture1"), 0);
            glUniform1i(glGetUniformLocation(shader_programme, "texture2"), 1);

            // NOTE: Make sure you activate the correct texture image unit for the related texture.


            // ====================
            //  TRANSFORMATION SETUP
            // ====================
            glm::mat4 identity_matrix = glm::mat4(1.0f);
            glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

            // we need to pass this info to the shader so call use
            glUseProgram(shader_programme);

            // Get the locations in the shaders                
            GLuint projection_mat_loc = glGetUniformLocation(shader_programme, "projection_matrix");
            // Set the data for them (two different ways: pointer or [0][0] syntax)
            glUniformMatrix4fv(projection_mat_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

            
            // 10-h. Deleting shader at this point does not hurt
            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);

            // ====================
            //      MAIN UI LOOP
            // ====================
            /* Loop until the user closes the window */
            while (!glfwWindowShouldClose(window))
            {
                // fps counter
                Utility::GLFW::update_fps_counter(window);

                /* Render here */
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// scene background color
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Activate the texture image units and bind the associated textures
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture_obj);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, texture_obj2);
                
                // Activate the shader programme
                glUseProgram(shader_programme);

                // camera/view transformation
                glm::mat4 view_matrix = identity_matrix;
                float radius = 10.0f;
                float camX = sin(glfwGetTime()) * radius;
                float camZ = cos(glfwGetTime()) * radius;
                view_matrix = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                // Get the locations in the shaders                
                GLuint view_mat_loc = glGetUniformLocation(shader_programme, "view_matrix");
                // Set the data for them (two different ways: pointer or [0][0] syntax)
                glUniformMatrix4fv(view_mat_loc, 1, GL_FALSE, &view_matrix[0][0]);

                // render each box
                glBindVertexArray(vao);
                for (unsigned int i = 0; i < 10; i++)
                {
                    // calculate the model matrix for each object and pass it to shader before drawing
                    glm::mat4 model_matrix = glm::translate(identity_matrix, cubePositions[i]);
                    float angle = 20.0f * i;
                    model_matrix = glm::rotate(model_matrix, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

                    GLuint model_mat_loc = glGetUniformLocation(shader_programme, "model_matrix");
                    glUniformMatrix4fv(model_mat_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }

                /* Swap front and back buffers */
                /* update other events like input handling */
                glfwSwapBuffers(window);

                /* Poll for and process events */
                /* put the stuff we've been drawing onto the display */
                glfwPollEvents();
            }

            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
            glDeleteProgram(shader_programme);

            glfwTerminate();
            return 0;
        }

        int WalkAroundMultipleCubesWithUserInput()
        {
            // Initialize the glfw & glew
            GLFWwindow* window = Utility::GLFW::start_glfw();
            Utility::GLEW::start_glew();

            // Some openGL settings
            // tell GL to only draw onto a pixel if the shape is closer to the viewer
            glEnable(GL_DEPTH_TEST); // enable depth-testing
            //glDepthFunc(GL_LESS);		 // depth-testing interprets a smaller value as "closer"

            // ====================
            // VBO - VAO GENERATION
            // ====================
            // 1. Vertex data for positions, colors and texture coordinates
            GLfloat vertices[] = {
                // coords             // texture
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
            };

            // world space positions of our cubes
            glm::vec3 cubePositions[] = {
                glm::vec3(0.0f,  0.0f,  0.0f),
                glm::vec3(2.0f,  5.0f, -15.0f),
                glm::vec3(-1.5f, -2.2f, -2.5f),
                glm::vec3(-3.8f, -2.0f, -12.3f),
                glm::vec3(2.4f, -0.4f, -3.5f),
                glm::vec3(-1.7f,  3.0f, -7.5f),
                glm::vec3(1.3f, -2.0f, -2.5f),
                glm::vec3(1.5f,  2.0f, -2.5f),
                glm::vec3(1.5f,  0.2f, -1.5f),
                glm::vec3(-1.3f,  1.0f, -1.5f)
            };


            // Generate tightly-packed VBO and then the Indexed VBO
            // 2,3 & 4
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // 5. Generate and bind VAO
            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            // 6. Define and associate the vertex attributes to the vbo
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            //positions
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            //textures
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

            // ====================
            //    TEXTURE SETUP
            // ====================

            // texture 1
            // ---------

            // 1. Load the corresponding image
            std::string image_file_path("resources\\container.jpg");
            bool is_png = image_file_path.find(".png") != std::string::npos;

            int width, height, nrChannels;
            unsigned char* data = stbi_load(image_file_path.c_str(), &width, &height, &nrChannels, 0);
            if (!data)
                std::cerr << "Failed to load texture image!!!" << std::endl;

            // 2. Generate the texture object like any other OpenGL object
            GLuint texture_obj;
            glGenTextures(1, &texture_obj);

            // 3. Bind it to a specific target. It is GL_TEXTURE_2D here.
            // RULE: texture_obj should and can NOT be bound to any other target after this point
            glBindTexture(GL_TEXTURE_2D, texture_obj);
            // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

            // 4. Set the sampling parameters
            // Wrapping (edge value sampling)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // Filtering
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // 5. Get the storage for the texture
            // Mutable storage in this case
            if (is_png)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            // 6. Storage contents can now be altered after this point.
            // It is often a good practice to generate a mipmap
            glGenerateMipmap(GL_TEXTURE_2D);

            // 7. You can now free the loaded image data.
            stbi_image_free(data);

            // texture 2
            // ---------
            // Repeat steps 1-7 for the second texture

            image_file_path = "resources\\awesomeface.png";
            is_png = image_file_path.find(".png") != std::string::npos;
            stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
            data = stbi_load(image_file_path.c_str(), &width, &height, &nrChannels, 0);
            if (!data)
                std::cerr << "Failed to load texture image!!!" << std::endl;

            // Texture object
            GLuint texture_obj2;
            glGenTextures(1, &texture_obj2);
            glBindTexture(GL_TEXTURE_2D, texture_obj2);

            // Sampling parameters
            // wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Storage
            if (is_png)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            // Mipmap setup
            glGenerateMipmap(GL_TEXTURE_2D);

            // free raw data
            stbi_image_free(data);

            // ====================
            //      SHADERS
            // ====================
            // 8. Define the sources for the vertex and fragment shader
            const char* vertex_shader_source_str =
                "#version 330 core\n"
                "layout (location = 0) in vec3 vertex_position;\n"
                "layout (location = 1) in vec2 vertex_texture_position;\n"
                "out vec2 textureCoord;\n"
                "uniform mat4 model_matrix;\n"
                "uniform mat4 view_matrix;\n"
                "uniform mat4 projection_matrix;\n"
                "void main()\n"
                "{\n"
                "   gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vertex_position, 1.0);\n"
                "   textureCoord = vec2(vertex_texture_position.x, vertex_texture_position.y);\n"
                "}\0";

            // 9. Fragment shader setup
            const char* fragment_shader_source_str =
                "#version 330 core\n"
                "in vec2 textureCoord;\n"
                "out vec4 frag_color;\n"
                "uniform sampler2D texture1;\n" // texture sampler
                "uniform sampler2D texture2;\n"
                "void main()\n"
                "{\n"
                "   frag_color = mix(texture(texture1, textureCoord), texture(texture2, textureCoord), 0.2);\n"
                "}\0";

            // 10-a. Create the vertex shader
            GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

            // 10-b. Attach the source string to the shader & compile
            const GLchar* vertex_shader_source = (const GLchar*)vertex_shader_source_str;
            glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
            glCompileShader(vertex_shader);

            // 10-c. Check for the errors
            int success = -1;
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 10-d REpeat steps 10-a, b and c for the fragment shader
            GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            const GLchar* fragment_shader_source = (const GLchar*)fragment_shader_source_str;
            glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
            glCompileShader(fragment_shader);
            // error check
            success = -1;
            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 10-e. Create the program to attach defined shaders
            GLuint shader_programme = glCreateProgram();

            // 10-f. Attach & link the shaders to the program
            glAttachShader(shader_programme, vertex_shader);
            glAttachShader(shader_programme, fragment_shader);
            glLinkProgram(shader_programme);

            // 10-g. Check against the linking errors
            success = -1;
            glGetProgramiv(shader_programme, GL_LINK_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetProgramInfoLog(shader_programme, 512, NULL, infoLog);
                std::cerr << "ERROR: Shader programme linking failed: " << infoLog << std::endl;
                return -1;
            }

            // ====================
            //  TEXTURE UNIT SETUP
            // ====================
            // Has to be done once

            // Since we make a modification on the shader, call glUseProgram for the shader programme
            glUseProgram(shader_programme);

            // Tell the shader what shader belongs to which texture object
            // get the location of the sampler variable named "texture1" and 
            // set the value of that sampler to the corresponding value of the texture image unit
            glUniform1i(glGetUniformLocation(shader_programme, "texture1"), 0);
            glUniform1i(glGetUniformLocation(shader_programme, "texture2"), 1);

            // NOTE: Make sure you activate the correct texture image unit for the related texture.


            // ====================
            //  TRANSFORMATION SETUP
            // ====================
            glm::mat4 identity_matrix = glm::mat4(1.0f);
            glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

            // we need to pass this info to the shader so call use
            glUseProgram(shader_programme);

            // Get the locations in the shaders                
            GLuint projection_mat_loc = glGetUniformLocation(shader_programme, "projection_matrix");
            // Set the data for them (two different ways: pointer or [0][0] syntax)
            glUniformMatrix4fv(projection_mat_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

            // Camera setup
            glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
            glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
            glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

            float deltaTime = 0.0f;	// time between current frame and last frame
            float lastFrame = 0.0f; // time of last frame

            // 10-h. Deleting shader at this point does not hurt
            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);

            // Lambda for processing input
            auto processInput = [&](GLFWwindow* window)
            {
                if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                    glfwSetWindowShouldClose(window, true);

                float cameraSpeed = 2.5 * deltaTime;
                if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                    cameraPos += cameraSpeed * cameraFront;
                if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                    cameraPos -= cameraSpeed * cameraFront;
                if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
                if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            };

            // ====================
            //      MAIN UI LOOP
            // ====================
            /* Loop until the user closes the window */
            while (!glfwWindowShouldClose(window))
            {
                // fps counter
                Utility::GLFW::update_fps_counter(window);

                // per-frame time logic
                // --------------------
                float currentFrame = glfwGetTime();
                deltaTime = currentFrame - lastFrame;
                lastFrame = currentFrame;

                // input
                // -----
                processInput(window);

                /* Render here */
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// scene background color
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Activate the texture image units and bind the associated textures
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture_obj);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, texture_obj2);

                // Activate the shader programme
                glUseProgram(shader_programme);

                // camera/view transformation
                glm::mat4 view_matrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
                // Get the locations in the shaders                
                GLuint view_mat_loc = glGetUniformLocation(shader_programme, "view_matrix");
                // Set the data for them (two different ways: pointer or [0][0] syntax)
                glUniformMatrix4fv(view_mat_loc, 1, GL_FALSE, &view_matrix[0][0]);

                // render each box
                glBindVertexArray(vao);
                for (unsigned int i = 0; i < 10; i++)
                {
                    // calculate the model matrix for each object and pass it to shader before drawing
                    glm::mat4 model_matrix = glm::translate(identity_matrix, cubePositions[i]);
                    float angle = 20.0f * i;
                    model_matrix = glm::rotate(model_matrix, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

                    GLuint model_mat_loc = glGetUniformLocation(shader_programme, "model_matrix");
                    glUniformMatrix4fv(model_mat_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }

                /* Swap front and back buffers */
                /* update other events like input handling */
                glfwSwapBuffers(window);

                /* Poll for and process events */
                /* put the stuff we've been drawing onto the display */
                glfwPollEvents();
            }

            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
            glDeleteProgram(shader_programme);

            glfwTerminate();
            return 0;
        }

        //  Look around
        // ====================
        bool firstMouse = true;
        float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
        float pitch = 0.0f;
        float lastX = 800.0f / 2.0;
        float lastY = 600.0 / 2.0;
        float fov = 45.0f;

        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

        void mouse_callback(GLFWwindow* window, double xpos, double ypos)
        {
            if (firstMouse)
            {
                lastX = xpos;
                lastY = ypos;
                firstMouse = false;
            }

            float xoffset = xpos - lastX;
            float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
            lastX = xpos;
            lastY = ypos;

            float sensitivity = 0.1f; // change this value to your liking
            xoffset *= sensitivity;
            yoffset *= sensitivity;

            yaw += xoffset;
            pitch += yoffset;

            // make sure that when pitch is out of bounds, screen doesn't get flipped
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;

            glm::vec3 front;
            front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            front.y = sin(glm::radians(pitch));
            front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            cameraFront = glm::normalize(front);
        }

        void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
        {
            fov -= (float)yoffset;
            if (fov < 1.0f)
                fov = 1.0f;
            if (fov > 45.0f)
                fov = 45.0f;
        }

        int WalkAroundWithKeyboardLookAroundWithMouse()
        {
            // Initialize the glfw & glew
            GLFWwindow* window = Utility::GLFW::start_glfw();
            glfwSetCursorPosCallback(window, mouse_callback); // required for mouse
            // tell GLFW to capture our mouse
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetScrollCallback(window, scroll_callback);

            Utility::GLEW::start_glew();

            // Some openGL settings
            // tell GL to only draw onto a pixel if the shape is closer to the viewer
            glEnable(GL_DEPTH_TEST); // enable depth-testing
            //glDepthFunc(GL_LESS);		 // depth-testing interprets a smaller value as "closer"

            // ====================
            // VBO - VAO GENERATION
            // ====================
            // 1. Vertex data for positions, colors and texture coordinates
            GLfloat vertices[] = {
                // coords             // texture
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
            };

            // world space positions of our cubes
            glm::vec3 cubePositions[] = {
                glm::vec3(0.0f,  0.0f,  0.0f),
                glm::vec3(2.0f,  5.0f, -15.0f),
                glm::vec3(-1.5f, -2.2f, -2.5f),
                glm::vec3(-3.8f, -2.0f, -12.3f),
                glm::vec3(2.4f, -0.4f, -3.5f),
                glm::vec3(-1.7f,  3.0f, -7.5f),
                glm::vec3(1.3f, -2.0f, -2.5f),
                glm::vec3(1.5f,  2.0f, -2.5f),
                glm::vec3(1.5f,  0.2f, -1.5f),
                glm::vec3(-1.3f,  1.0f, -1.5f)
            };


            // Generate tightly-packed VBO and then the Indexed VBO
            // 2,3 & 4
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // 5. Generate and bind VAO
            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            // 6. Define and associate the vertex attributes to the vbo
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            //positions
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            //textures
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

            // ====================
            //    TEXTURE SETUP
            // ====================

            // texture 1
            // ---------

            // 1. Load the corresponding image
            std::string image_file_path("resources\\container.jpg");
            bool is_png = image_file_path.find(".png") != std::string::npos;

            int width, height, nrChannels;
            unsigned char* data = stbi_load(image_file_path.c_str(), &width, &height, &nrChannels, 0);
            if (!data)
                std::cerr << "Failed to load texture image!!!" << std::endl;

            // 2. Generate the texture object like any other OpenGL object
            GLuint texture_obj;
            glGenTextures(1, &texture_obj);

            // 3. Bind it to a specific target. It is GL_TEXTURE_2D here.
            // RULE: texture_obj should and can NOT be bound to any other target after this point
            glBindTexture(GL_TEXTURE_2D, texture_obj);
            // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

            // 4. Set the sampling parameters
            // Wrapping (edge value sampling)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // Filtering
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // 5. Get the storage for the texture
            // Mutable storage in this case
            if (is_png)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            // 6. Storage contents can now be altered after this point.
            // It is often a good practice to generate a mipmap
            glGenerateMipmap(GL_TEXTURE_2D);

            // 7. You can now free the loaded image data.
            stbi_image_free(data);

            // texture 2
            // ---------
            // Repeat steps 1-7 for the second texture

            image_file_path = "resources\\awesomeface.png";
            is_png = image_file_path.find(".png") != std::string::npos;
            stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
            data = stbi_load(image_file_path.c_str(), &width, &height, &nrChannels, 0);
            if (!data)
                std::cerr << "Failed to load texture image!!!" << std::endl;

            // Texture object
            GLuint texture_obj2;
            glGenTextures(1, &texture_obj2);
            glBindTexture(GL_TEXTURE_2D, texture_obj2);

            // Sampling parameters
            // wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Storage
            if (is_png)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            // Mipmap setup
            glGenerateMipmap(GL_TEXTURE_2D);

            // free raw data
            stbi_image_free(data);

            // ====================
            //      SHADERS
            // ====================
            // 8. Define the sources for the vertex and fragment shader
            const char* vertex_shader_source_str =
                "#version 330 core\n"
                "layout (location = 0) in vec3 vertex_position;\n"
                "layout (location = 1) in vec2 vertex_texture_position;\n"
                "out vec2 textureCoord;\n"
                "uniform mat4 model_matrix;\n"
                "uniform mat4 view_matrix;\n"
                "uniform mat4 projection_matrix;\n"
                "void main()\n"
                "{\n"
                "   gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vertex_position, 1.0);\n"
                "   textureCoord = vec2(vertex_texture_position.x, vertex_texture_position.y);\n"
                "}\0";

            // 9. Fragment shader setup
            const char* fragment_shader_source_str =
                "#version 330 core\n"
                "in vec2 textureCoord;\n"
                "out vec4 frag_color;\n"
                "uniform sampler2D texture1;\n" // texture sampler
                "uniform sampler2D texture2;\n"
                "void main()\n"
                "{\n"
                "   frag_color = mix(texture(texture1, textureCoord), texture(texture2, textureCoord), 0.2);\n"
                "}\0";

            // 10-a. Create the vertex shader
            GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

            // 10-b. Attach the source string to the shader & compile
            const GLchar* vertex_shader_source = (const GLchar*)vertex_shader_source_str;
            glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
            glCompileShader(vertex_shader);

            // 10-c. Check for the errors
            int success = -1;
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 10-d REpeat steps 10-a, b and c for the fragment shader
            GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            const GLchar* fragment_shader_source = (const GLchar*)fragment_shader_source_str;
            glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
            glCompileShader(fragment_shader);
            // error check
            success = -1;
            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
                std::cerr << "Shader can not compile: " << infoLog << "\n";
                return -1; // or exit or something
            }

            // 10-e. Create the program to attach defined shaders
            GLuint shader_programme = glCreateProgram();

            // 10-f. Attach & link the shaders to the program
            glAttachShader(shader_programme, vertex_shader);
            glAttachShader(shader_programme, fragment_shader);
            glLinkProgram(shader_programme);

            // 10-g. Check against the linking errors
            success = -1;
            glGetProgramiv(shader_programme, GL_LINK_STATUS, &success);
            if (GL_TRUE != success)
            {
                char infoLog[512];
                glGetProgramInfoLog(shader_programme, 512, NULL, infoLog);
                std::cerr << "ERROR: Shader programme linking failed: " << infoLog << std::endl;
                return -1;
            }

            // ====================
            //  TEXTURE UNIT SETUP
            // ====================
            // Has to be done once

            // Since we make a modification on the shader, call glUseProgram for the shader programme
            glUseProgram(shader_programme);

            // Tell the shader what shader belongs to which texture object
            // get the location of the sampler variable named "texture1" and 
            // set the value of that sampler to the corresponding value of the texture image unit
            glUniform1i(glGetUniformLocation(shader_programme, "texture1"), 0);
            glUniform1i(glGetUniformLocation(shader_programme, "texture2"), 1);

            // NOTE: Make sure you activate the correct texture image unit for the related texture.


            // ====================
            //  TRANSFORMATION SETUP
            // ====================
            glm::mat4 identity_matrix = glm::mat4(1.0f);            

            //  Walk around
            // ====================
            glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
            // cameraFront is moved up to be global variable to be used in mouse_callback
            //glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
            glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

            float deltaTime = 0.0f;	// time between current frame and last frame
            float lastFrame = 0.0f; // time of last frame

            // Lambda for processing input
            auto processInput = [&](GLFWwindow* window)
            {
                if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                    glfwSetWindowShouldClose(window, true);

                float cameraSpeed = 2.5 * deltaTime;
                if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                    cameraPos += cameraSpeed * cameraFront;
                if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                    cameraPos -= cameraSpeed * cameraFront;
                if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
                if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            };

            // 10-h. Deleting shader at this point does not hurt
            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);           

            // ====================
            //      MAIN UI LOOP
            // ====================
            /* Loop until the user closes the window */
            while (!glfwWindowShouldClose(window))
            {
                // fps counter
                Utility::GLFW::update_fps_counter(window);

                // per-frame time logic
                // --------------------
                float currentFrame = glfwGetTime();
                deltaTime = currentFrame - lastFrame;
                lastFrame = currentFrame;

                // input
                // -----
                processInput(window);

                /* Render here */
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// scene background color
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Activate the texture image units and bind the associated textures
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture_obj);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, texture_obj2);

                // Activate the shader programme
                glUseProgram(shader_programme);
                // pass projection matrix to shader (note that in this case it could change every frame)
                glm::mat4 projection_matrix = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
                // we need to pass this info to the shader so call use               
                GLuint projection_mat_loc = glGetUniformLocation(shader_programme, "projection_matrix");
                glUniformMatrix4fv(projection_mat_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

                // camera/view transformation
                glm::mat4 view_matrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
                // Get the locations in the shaders                
                GLuint view_mat_loc = glGetUniformLocation(shader_programme, "view_matrix");
                // Set the data for them (two different ways: pointer or [0][0] syntax)
                glUniformMatrix4fv(view_mat_loc, 1, GL_FALSE, &view_matrix[0][0]);

                // render each box
                glBindVertexArray(vao);
                for (unsigned int i = 0; i < 10; i++)
                {
                    // calculate the model matrix for each object and pass it to shader before drawing
                    glm::mat4 model_matrix = glm::translate(identity_matrix, cubePositions[i]);
                    float angle = 20.0f * i;
                    model_matrix = glm::rotate(model_matrix, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

                    GLuint model_mat_loc = glGetUniformLocation(shader_programme, "model_matrix");
                    glUniformMatrix4fv(model_mat_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }

                /* Swap front and back buffers */
                /* update other events like input handling */
                glfwSwapBuffers(window);

                /* Poll for and process events */
                /* put the stuff we've been drawing onto the display */
                glfwPollEvents();
            }

            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
            glDeleteProgram(shader_programme);

            glfwTerminate();
            return 0;
        }       

    }

    namespace hello_triangle
    {
        int Run()
        {
            /* Initialize the library */
            GLFWwindow* window = Utility::GLFW::start_glfw();
            Utility::GLEW::start_glew();
            // tell GL to only draw onto a pixel if the shape is closer to the viewer
            glEnable(GL_DEPTH_TEST); // enable depth-testing
            glDepthFunc(GL_LESS);		 // depth-testing interprets a smaller value as "closer"

            GLuint vbo = hello_triangle::GenerateVertexBuffer();
            GLuint vao = hello_triangle::GenerateVertexArrayObject(vbo);
            auto shader_prog = ShaderProgram("tutorials\\shaders\\test_vs.glsl", "tutorials\\shaders\\test_fs.glsl");
            GLuint texture1 = GenerateTexture("resources\\wall.jpg");
            GLuint texture2 = GenerateTexture("resources\\awesomeface.png");

            shader_prog.use();
            shader_prog.setInt("texture1", 1); // or with shader class
            shader_prog.setInt("texture2", 1); // or with shader class

            int simulateColorChangeOldSate = GLFW_RELEASE;
            bool simulateColorChange = false;

            int showPointsOldState = GLFW_RELEASE;
            bool showPoints = false;

            int showLinesOldState = GLFW_RELEASE;
            bool showLines = false;

            int showTextureOldSate = GLFW_RELEASE;
            bool showTexture = false;

            /* Loop until the user closes the window */
            while (!glfwWindowShouldClose(window))
            {
                // fps counter
                Utility::GLFW::update_fps_counter(window);

                /* Render here */
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// scene background color
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // bind Texture
                if (showTexture)
                {
                   
                    
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, texture1);
                    glActiveTexture(GL_TEXTURE1);
                    glBindTexture(GL_TEXTURE_2D, texture2);
                    shader_prog.setBool("useTexture", true);
                    
                }                    
                else
                {
                    shader_prog.setBool("useTexture", false);
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, 0);
                    glActiveTexture(GL_TEXTURE1);
                    glBindTexture(GL_TEXTURE_2D, 0);
                }
                    
                
                // Do whatever you have to do
                shader_prog.use();

                if (simulateColorChange)
                {
                    float timeValue = glfwGetTime();
                    float greenValue = sin(timeValue) / 2.0f + 0.5f;

                    shader_prog.setBool("useCustomColor", true);
                    shader_prog.setVec4("customColor", 0.0f, greenValue, 0.0f, 1.0f);
                }
                else
                {
                    shader_prog.setBool("useCustomColor", false);
                }

                glBindVertexArray(vao);
                glDrawArrays(GL_TRIANGLES, 0, 3);

                if (showPoints)
                {
                    glDrawArrays(GL_POINTS, 0, 3);
                    glEnable(GL_PROGRAM_POINT_SIZE);
                    glPointSize(10.0);
                }
                else
                {
                    glDisable(GL_PROGRAM_POINT_SIZE);
                    glPointSize(0.0);

                }

                if (showLines)
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    glEnable(GL_LINE_SMOOTH);
                    glLineWidth(10.0);
                    //glPointSize(15.0); use when GL_POINT is used
                }
                else
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    glDisable(GL_LINE_SMOOTH);
                    glLineWidth(0.0);
                }


                // input
                if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                    glfwSetWindowShouldClose(window, true);

                // Color change
                int simulateColorChangeNewState = glfwGetKey(window, GLFW_KEY_ENTER);
                if (simulateColorChangeNewState == GLFW_RELEASE && simulateColorChangeOldSate == GLFW_PRESS)
                {
                    // whatever+
                    simulateColorChange = !simulateColorChange;
                    showTexture = false;
                    std::cout << "Enter pressed: " << std::boolalpha << simulateColorChange << "\n";
                }
                simulateColorChangeOldSate = simulateColorChangeNewState;

                // Points
                int showPointsNewState = glfwGetKey(window, GLFW_KEY_P);
                if (showPointsNewState == GLFW_RELEASE && showPointsOldState == GLFW_PRESS) 
                {
                    // whatever+
                    showPoints = !showPoints;
                    std::cout << "P pressed: " << std::boolalpha << showPoints << "\n";
                }
                showPointsOldState = showPointsNewState;

                // Lines
                int showLinesNewState = glfwGetKey(window, GLFW_KEY_L);
                if (showLinesNewState == GLFW_RELEASE && showLinesOldState == GLFW_PRESS) 
                {
                    // whatever+
                    showLines = !showLines;
                    std::cout << "L pressed: " << std::boolalpha << showLines << "\n";
                }
                showLinesOldState = showLinesNewState;

                // texture
                int showTextureNewState = glfwGetKey(window, GLFW_KEY_T);
                if (showTextureNewState == GLFW_RELEASE && showTextureOldSate == GLFW_PRESS)
                {
                    // whatever+
                    showTexture = !showTexture;
                    simulateColorChange = false;
                    std::cout << "T pressed: " << std::boolalpha << showTexture << "\n";
                }
                showTextureOldSate = showTextureNewState;

                /* Swap front and back buffers */
                glfwSwapBuffers(window);

                /* Poll for and process events */
                glfwPollEvents();
            }

            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
            glDeleteTextures(1, &texture1);
            glDeleteTextures(1, &texture2);

            glfwTerminate();
            return 0;
        }

        GLuint GenerateVertexBuffer()
        {
            // Define points
            GLfloat points[] = {
                // Coord                // Colors           // texture coordinates
                0.0f,   0.5f,   0.0f,   1.0f, 0.0f, 0.0f,   0.5f, 1.0f,
                0.5f,   -0.5f,  0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
                -0.5f,  -0.5f,  0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f };

            // VBO
            /* a vertex buffer object (VBO) is created here. this stores an array of
            data on the graphics adapter's memory. in our case - the vertex points */
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), points, GL_STATIC_DRAW);

            return vbo;
        }

        GLuint GenerateVertexArrayObject(GLuint& vbo)
        {
            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
            glEnableVertexAttribArray(2);

            // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
            //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
            // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
            glBindVertexArray(0);


            return vao;
        }

        GLuint GenerateTexture(std::string imageFile)
        {
            bool isPng = false;
            std::size_t pos = imageFile.find(".png");
            if (pos != std::string::npos)
                isPng = true;

            GLuint texture;

            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
            
            // set the texture wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // set texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // load image, create texture and generate mipmaps
            int width, height, nrChannels;
            // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
            unsigned char* data = stbi_load(imageFile.c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                if(isPng)
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                else
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else
            {
                std::cerr << "Failed to load texture" << std::endl;
            }
            stbi_image_free(data);

            return texture;

        }
    }

    namespace cube
    {
        GLuint GenerateVertexBuffer(GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat edgeLength)
        {
            // Define points
            GLfloat halfSideLength = edgeLength * 0.5f;

            GLfloat points[] =
            {
                // front face
                centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, 1.0f, 0.0f, 0.0f,// top left
                centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, 1.0f, 0.0f, 0.0f,// top right
                centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, 1.0f, 0.0f, 0.0f,// bottom right
                centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, 1.0f, 0.0f, 0.0f,// bottom left

                // back face
                centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, 0.0f, 1.0f, 0.0f,// top left
                centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, 0.0f, 1.0f, 0.0f,// top right
                centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, 0.0f, 1.0f, 0.0f,// bottom right
                centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, 0.0f, 1.0f, 0.0f// bottom left                
            };

            //float points[] = {
            //    // positions          // colors         
            //     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, // top right
            //     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, // bottom right
            //    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, // bottom left
            //    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f // top left 
            //};


            // VBO
            /* a vertex buffer object (VBO) is created here. this stores an array of
            data on the graphics adapter's memory. in our case - the vertex points */
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

            return vbo;
        }

        GLuint GenerateElementBuffer()
        {
            unsigned int indices[] = {
                0, 3, 2, // Front face bottom
                0, 2, 1,  // Front face top
                0 + 4, 3 + 4, 2 + 4, // back face bottom
                0 + 4, 2 + 4, 1 + 4,  // back face top
                0, 3, 7, // left face bottom
                4, 0, 7,  // left face top
                1, 2, 6, // right face bottom
                5, 1, 6,  // right face top
                7, 3, 2, // bottom face bottom
                7, 2, 6,  // bottom face top
                4, 0, 1, // top face bottom
                4, 1, 5  // top face top
            };

            //    unsigned int indices[] = {
            //0, 1, 3, // first triangle
            //1, 2, 3  // second triangle
            //    };

            GLuint ebo;
            glGenBuffers(1, &ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            return ebo;
        }

        GLuint GenerateVertexArrayObject(GLuint& vbo, GLuint& ebo)
        {
            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);

            // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
            //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
            // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
            glBindVertexArray(0);


            return vao;
        }

        namespace translation
        {
            int Run()
            {
                /* Initialize the library */
                GLFWwindow* window = Utility::GLFW::start_glfw();
                Utility::GLEW::start_glew();

                 // tell GL to only draw onto a pixel if the shape is closer to the viewer
                glEnable(GL_DEPTH_TEST); // enable depth-testing
                glDepthFunc(GL_LESS);		 // depth-testing interprets a smaller value as "closer"

                auto cubeEdge = 48.0;
                int windowWidth, windowHeight;
                glfwGetWindowSize(window, &windowWidth, &windowHeight);

                GLuint vbo = GenerateVertexBuffer(0.0, 0.0, 0.0, 96.0/windowHeight);
                GLuint ebo = GenerateElementBuffer();
                GLuint vao = GenerateVertexArrayObject(vbo, ebo);
                auto shader_prog = ShaderProgram("tutorials\\shaders\\cube_translation_vs.glsl", "tutorials\\shaders\\cube_translation_fs.glsl");


                auto totalDistance = 0.0f;
                int counter = 0;

                double step = 0.0;
                auto distance = 0.0;
                bool goingPos = true;
                while (!glfwWindowShouldClose(window))
                {
                    // fps counter
                    Utility::GLFW::update_fps_counter(window);

                    /* Render here */
                    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// scene background color
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    
                    //// create transformations
                    glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
                    /*auto t = (float)glfwGetTime();
                    auto s = glm::pi<float>() / 180.0;
                    auto sinn = glm::sin(s * t * 50.0f);
                    transform = glm::translate(transform, glm::vec3(sinn, 0.0f, 0.0f));
                    transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 1.0f));*/




                    

                    auto velocity = 7.5f;
                    auto t = (float)glfwGetTime();
                    float actDistance = velocity * t;
                    double integer;
                    double remaining = modf(actDistance, &integer);

                    std::cout << "mod: " << integer << ", " << remaining <<  "\n";
                    
                    if (integer < 1.0)
                        distance = 0.0f + remaining;
                    else if (integer == 1.0)
                    {
                        distance = 1.0f - remaining;
                        goingPos = false;
                    }                        
                    else if (std::fmod(integer, 2.0) == 0.0)
                    {                        
                        if (goingPos)
                            distance = 0.0f + remaining;                            
                        else
                            distance = 0.0f - remaining;                        
                    }
                    else if (std::fmod(integer, 2.0) == 1.0 )
                    {
                        if (distance < 0.0)
                        {
                            distance = -1.0f + remaining;
                            goingPos = true;
                        }
                        else
                        {
                            distance = 1.0f - remaining;
                            goingPos = false;
                        }                            
                    }                   

                    transform = glm::translate(transform, glm::vec3(distance, 0.0f, 0.0f));

                    // Do whatever you have to do
                    shader_prog.use();
                    shader_prog.setMat4("transform", transform);
                    glBindVertexArray(vao);
                    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                    // input
                    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                        glfwSetWindowShouldClose(window, true);

                    /* Swap front and back buffers */
                    glfwSwapBuffers(window);

                    /* Poll for and process events */
                    glfwPollEvents();
                }

                glDeleteVertexArrays(1, &vao);
                glDeleteBuffers(1, &vbo);
                glDeleteBuffers(1, &ebo);

                glfwTerminate();
                return 0;
            }
        }

        namespace rotation
        {

            int Run()
            {
                /* Initialize the library */
                GLFWwindow* window = Utility::GLFW::start_glfw();
                Utility::GLEW::start_glew();

                /* glfwSetKeyCallback(window, keyCallback);
                 glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);*/

                 // tell GL to only draw onto a pixel if the shape is closer to the viewer
                glEnable(GL_DEPTH_TEST); // enable depth-testing
                glDepthFunc(GL_LESS);		 // depth-testing interprets a smaller value as "closer"

                //int screenWidth, screenHeight;
                //glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
                //glViewport(0.0f, 0.0f, screenWidth, screenHeight); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels

                GLuint vbo = GenerateVertexBuffer(0.0, 0.0, 0.0, 1.0f);
                GLuint ebo = GenerateElementBuffer();
                GLuint vao = GenerateVertexArrayObject(vbo, ebo);
                auto shader_prog = ShaderProgram("tutorials\\shaders\\cube_rotation_vs.glsl", "tutorials\\shaders\\cube_rotation_fs.glsl");

                while (!glfwWindowShouldClose(window))
                {
                    // fps counter
                    Utility::GLFW::update_fps_counter(window);

                    /* Render here */
                    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// scene background color
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    // Do whatever you have to do
                    shader_prog.use();
                    // create transformations
                    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
                    glm::mat4 view = glm::mat4(1.0f);
                    glm::mat4 projection = glm::mat4(1.0f);
                    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
                    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
                    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
                    // retrieve the matrix uniform locations
                    shader_prog.setMat4("model", model);
                    shader_prog.setMat4("view", view);
                    // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
                    shader_prog.setMat4("projection", projection);

                    glBindVertexArray(vao);
                    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                    // input
                    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                        glfwSetWindowShouldClose(window, true);

                    /* Swap front and back buffers */
                    glfwSwapBuffers(window);

                    /* Poll for and process events */
                    glfwPollEvents();
                }

                glDeleteVertexArrays(1, &vao);
                glDeleteBuffers(1, &vbo);
                glDeleteBuffers(1, &ebo);

                glfwTerminate();
                return 0;
            }
        }

    }

}
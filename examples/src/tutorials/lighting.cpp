#include "lighting.h"
#include <GLFW/glfw3.h>
#include "../glfw_utils.h"
#include "../glew_utils.h"
#include <iostream>
#include "../ShaderType.h"
#include "../ShaderProgram.h"
#include "../camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#include "../stb_image.h"

#include<math.h>
#include <cmath>

#include "../texture_utils.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
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

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}


namespace tutorials::lighting
{
    int Intro()
    {
        // Initialize the glfw & glew
        GLFWwindow* window = Utility::GLFW::start_glfw();
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        // tell GLFW to capture our mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
            // coords          
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f
        };

        // VBO for the cube
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Object cube VAO
        GLuint object_cube_vao;
        glGenVertexArrays(1, &object_cube_vao);
        glBindVertexArray(object_cube_vao);

        // Bind the cube vbo to object cube VAO
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

        // Light source cube vao
        GLuint light_source_cube_vao;
        glGenVertexArrays(1, &light_source_cube_vao);
        glBindVertexArray(light_source_cube_vao);

        // Bind the cube vbo to light source cube VAO
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0); // not required


        // ====================
        //      SHADERS
        // ====================
        // object cube shader
        const char* object_cube_vertex_shader_src =
            "#version 330 core\n"
            "layout (location = 0) in vec3 vertex_position;\n"
            "uniform mat4 model_matrix;\n"
            "uniform mat4 view_matrix;\n"
            "uniform mat4 projection_matrix;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vertex_position, 1.0);\n"
            "}\0";

        // 9. Fragment shader setup
        const char* object_cube_fragment_shader_src =
            "#version 330 core\n"
            "uniform vec3 objectColor;\n"
            "uniform vec3 lightColor;\n"
            "out vec4 frag_color;\n"
            "void main()\n"
            "{\n"
            "   frag_color = vec4(lightColor * objectColor, 1.0);\n"
            "}\0";

        auto object_cube_shader = ShaderProgram(object_cube_vertex_shader_src, ShaderSourceType::Text,
            object_cube_fragment_shader_src, ShaderSourceType::Text);

        // ligth source cube shader
        const char* light_source_cube_vertex_shader_src =
            "#version 330 core\n"
            "layout (location = 0) in vec3 vertex_position;\n"
            "uniform mat4 model_matrix;\n"
            "uniform mat4 view_matrix;\n"
            "uniform mat4 projection_matrix;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vertex_position, 1.0);\n"
            "}\0";

        const char* light_source_cube_fragment_shader_src =
            "#version 330 core\n"
            "out vec4 frag_color;\n"
            "void main()\n"
            "{\n"
            "   frag_color = vec4(1.0);\n"
            "}\0";

        auto light_source_cube_shader = ShaderProgram(light_source_cube_vertex_shader_src, ShaderSourceType::Text,
            light_source_cube_fragment_shader_src, ShaderSourceType::Text);


        // ====================
        //  TRANSFORMATION SETUP
        // ====================
        glm::mat4 identity_matrix = glm::mat4(1.0f);
        

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
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);// scene background color
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 view_matrix = camera.GetViewMatrix();
            glm::mat4 projection_matrix = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 model_matrix = identity_matrix;

            // render object cube
            object_cube_shader.use();
            object_cube_shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
            object_cube_shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

            object_cube_shader.setMat4("model_matrix", model_matrix);
            object_cube_shader.setMat4("view_matrix", view_matrix);
            object_cube_shader.setMat4("projection_matrix", projection_matrix);

            glBindVertexArray(object_cube_vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // render light source cube
            light_source_cube_shader.use();
            light_source_cube_shader.setMat4("view_matrix", view_matrix);
            light_source_cube_shader.setMat4("projection_matrix", projection_matrix);
            model_matrix = identity_matrix;
            glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
            model_matrix = glm::translate(model_matrix, lightPos);
            model_matrix = glm::scale(model_matrix, glm::vec3(0.2f)); // a smaller cube
            light_source_cube_shader.setMat4("model_matrix", model_matrix);

            glBindVertexArray(light_source_cube_vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            /* Swap front and back buffers */
            /* update other events like input handling */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            /* put the stuff we've been drawing onto the display */
            glfwPollEvents();
        }

        glDeleteVertexArrays(1, &object_cube_vao);
        glDeleteVertexArrays(1, &light_source_cube_vao);
        glDeleteBuffers(1, &vbo);

        glfwTerminate();
        return 0;
    }

    int AmbientLighting(bool dynamic)
    {
        // Initialize the glfw & glew
        GLFWwindow* window = Utility::GLFW::start_glfw();
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        // tell GLFW to capture our mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
            // coords          
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f
        };

        // VBO for the cube
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Object cube VAO
        GLuint object_cube_vao;
        glGenVertexArrays(1, &object_cube_vao);
        glBindVertexArray(object_cube_vao);

        // Bind the cube vbo to object cube VAO
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

        // Light source cube vao
        GLuint light_source_cube_vao;
        glGenVertexArrays(1, &light_source_cube_vao);
        glBindVertexArray(light_source_cube_vao);

        // Bind the cube vbo to light source cube VAO
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0); // not required


        // ====================
        //      SHADERS
        // ====================
        // object cube shader
        const char* object_cube_vertex_shader_src =
            "#version 330 core\n"
            "layout (location = 0) in vec3 vertex_position;\n"
            "uniform mat4 model_matrix;\n"
            "uniform mat4 view_matrix;\n"
            "uniform mat4 projection_matrix;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vertex_position, 1.0);\n"
            "}\0";

        // 9. Fragment shader setup
        const char* object_cube_fragment_shader_src =
            "#version 330 core\n"
            "uniform vec3 objectColor;\n"
            "uniform vec3 lightColor;\n"
            "uniform float ambientStrength;\n"
            "out vec4 frag_color;\n"
            "void main()\n"
            "{\n"
            "   vec3 ambient = ambientStrength * lightColor;\n"
            "   vec3 result = ambient * objectColor;\n"
            "   frag_color = vec4(result, 1.0);\n"
            "}\0";

        auto object_cube_shader = ShaderProgram(object_cube_vertex_shader_src, ShaderSourceType::Text,
            object_cube_fragment_shader_src, ShaderSourceType::Text);

        // ligth source cube shader
        const char* light_source_cube_vertex_shader_src =
            "#version 330 core\n"
            "layout (location = 0) in vec3 vertex_position;\n"
            "uniform mat4 model_matrix;\n"
            "uniform mat4 view_matrix;\n"
            "uniform mat4 projection_matrix;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vertex_position, 1.0);\n"
            "}\0";

        const char* light_source_cube_fragment_shader_src =
            "#version 330 core\n"
            "out vec4 frag_color;\n"
            "void main()\n"
            "{\n"
            "   frag_color = vec4(1.0);\n"
            "}\0";

        auto light_source_cube_shader = ShaderProgram(light_source_cube_vertex_shader_src, ShaderSourceType::Text,
            light_source_cube_fragment_shader_src, ShaderSourceType::Text);


        // ====================
        //  TRANSFORMATION SETUP
        // ====================
        glm::mat4 identity_matrix = glm::mat4(1.0f);


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
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);// scene background color
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 view_matrix = camera.GetViewMatrix();
            glm::mat4 projection_matrix = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 model_matrix = identity_matrix;

            // render object cube
            object_cube_shader.use();
            object_cube_shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
            object_cube_shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
            float ambientStrength = dynamic ? currentFrame : 0.3;
            object_cube_shader.setFloat("ambientStrength", ambientStrength);

            object_cube_shader.setMat4("model_matrix", model_matrix);
            object_cube_shader.setMat4("view_matrix", view_matrix);
            object_cube_shader.setMat4("projection_matrix", projection_matrix);

            glBindVertexArray(object_cube_vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // render light source cube
            light_source_cube_shader.use();
            light_source_cube_shader.setMat4("view_matrix", view_matrix);
            light_source_cube_shader.setMat4("projection_matrix", projection_matrix);
            model_matrix = identity_matrix;
            glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
            model_matrix = glm::translate(model_matrix, lightPos);
            model_matrix = glm::scale(model_matrix, glm::vec3(0.2f)); // a smaller cube
            light_source_cube_shader.setMat4("model_matrix", model_matrix);

            glBindVertexArray(light_source_cube_vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            /* Swap front and back buffers */
            /* update other events like input handling */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            /* put the stuff we've been drawing onto the display */
            glfwPollEvents();
        }

        glDeleteVertexArrays(1, &object_cube_vao);
        glDeleteVertexArrays(1, &light_source_cube_vao);
        glDeleteBuffers(1, &vbo);

        glfwTerminate();
        return 0;
    }

    int DiffuseLighting(bool dynamic)
    {
        // Initialize the glfw & glew
        GLFWwindow* window = Utility::GLFW::start_glfw();
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        // tell GLFW to capture our mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
            // coords             // normals
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
        };

        // VBO for the cube
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Object cube VAO
        GLuint object_cube_vao;
        glGenVertexArrays(1, &object_cube_vao);
        glBindVertexArray(object_cube_vao);

        // Bind the cube vbo to object cube VAO
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // coords
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

        // Light source cube vao
        GLuint light_source_cube_vao;
        glGenVertexArrays(1, &light_source_cube_vao);
        glBindVertexArray(light_source_cube_vao);

        // Bind the cube vbo to light source cube VAO
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // no need for normals in the light source
        glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

        // ====================
        //      SHADERS
        // ====================
        // object cube shader
        const char* object_cube_vertex_shader_src =
            "#version 330 core\n"
            "layout (location = 0) in vec3 vertex_position;\n"
            "layout (location = 1) in vec3 vertex_normal;\n"
            "uniform mat4 model_matrix;\n"
            "uniform mat4 view_matrix;\n"
            "uniform mat4 projection_matrix;\n"
            "out vec3 frag_position;\n"
            "out vec3 frag_normal;\n"
            "void main()\n"
            "{\n"
            "   frag_position = vec3(model_matrix * vec4(vertex_position, 1.0));\n"
            "   frag_normal = vertex_normal;\n"
            "   gl_Position = projection_matrix * view_matrix * vec4(frag_position, 1.0);\n"
            "}\0";

        // 9. Fragment shader setup
        const char* object_cube_fragment_shader_src =
            "#version 330 core\n"
            "in vec3 frag_position;\n"
            "in vec3 frag_normal;\n"
            "uniform vec3 objectColor;\n"
            "uniform vec3 lightPos;\n"
            "uniform vec3 lightColor;\n"
            "uniform float ambientStrength;\n"
            "out vec4 frag_color;\n"
            "void main()\n"
            "{\n"
            "   vec3 normalized_frag_normal = normalize(frag_normal);\n"
            "   vec3 dir_vector_from_light_source_to_fragment = normalize(lightPos - frag_position);\n"
            "   float cosine_angle = dot(normalized_frag_normal, dir_vector_from_light_source_to_fragment);\n"
            "   cosine_angle = max(cosine_angle, 0.0);\n"
            "   vec3 diffuse = cosine_angle * lightColor;\n"
            "   vec3 ambient = ambientStrength * lightColor;\n"
            "   vec3 resulting_color = (ambient + diffuse) * objectColor;\n"
            "   frag_color = vec4(resulting_color, 1.0);\n"
            "}\0";

        auto object_cube_shader = ShaderProgram(object_cube_vertex_shader_src, ShaderSourceType::Text,
            object_cube_fragment_shader_src, ShaderSourceType::Text);

        // ligth source cube shader
        const char* light_source_cube_vertex_shader_src =
            "#version 330 core\n"
            "layout (location = 0) in vec3 vertex_position;\n"
            "uniform mat4 model_matrix;\n"
            "uniform mat4 view_matrix;\n"
            "uniform mat4 projection_matrix;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vertex_position, 1.0);\n"
            "}\0";

        const char* light_source_cube_fragment_shader_src =
            "#version 330 core\n"
            "out vec4 frag_color;\n"
            "void main()\n"
            "{\n"
            "   frag_color = vec4(1.0);\n"
            "}\0";

        auto light_source_cube_shader = ShaderProgram(light_source_cube_vertex_shader_src, ShaderSourceType::Text,
            light_source_cube_fragment_shader_src, ShaderSourceType::Text);


        // ====================
        //  TRANSFORMATION SETUP
        // ====================
        glm::mat4 identity_matrix = glm::mat4(1.0f);
        // ====================
        //  LIGHTING SETUP
        // ====================
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


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
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);// scene background color
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 view_matrix = camera.GetViewMatrix();
            glm::mat4 projection_matrix = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 model_matrix = identity_matrix;

            // render object cube
            object_cube_shader.use();
            object_cube_shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
            object_cube_shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
            object_cube_shader.setVec3("lightPos", lightPos);

            float ambientStrength = dynamic ? currentFrame : 0.1;
            object_cube_shader.setFloat("ambientStrength", ambientStrength);

            object_cube_shader.setMat4("model_matrix", model_matrix);
            object_cube_shader.setMat4("view_matrix", view_matrix);
            object_cube_shader.setMat4("projection_matrix", projection_matrix);

            glBindVertexArray(object_cube_vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // render light source cube
            light_source_cube_shader.use();
            light_source_cube_shader.setMat4("view_matrix", view_matrix);
            light_source_cube_shader.setMat4("projection_matrix", projection_matrix);

            model_matrix = identity_matrix;            
            model_matrix = glm::translate(model_matrix, lightPos);
            model_matrix = glm::scale(model_matrix, glm::vec3(0.2f)); // a smaller cube
            light_source_cube_shader.setMat4("model_matrix", model_matrix);

            glBindVertexArray(light_source_cube_vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            /* Swap front and back buffers */
            /* update other events like input handling */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            /* put the stuff we've been drawing onto the display */
            glfwPollEvents();
        }

        glDeleteVertexArrays(1, &object_cube_vao);
        glDeleteVertexArrays(1, &light_source_cube_vao);
        glDeleteBuffers(1, &vbo);

        glfwTerminate();
        return 0;
    }

    int SpecularLighting(bool dynamic)
    {
        // Initialize the glfw & glew
        GLFWwindow* window = Utility::GLFW::start_glfw();
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        // tell GLFW to capture our mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
            // coords             // normals
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
        };

        // VBO for the cube
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Object cube VAO
        GLuint object_cube_vao;
        glGenVertexArrays(1, &object_cube_vao);
        glBindVertexArray(object_cube_vao);

        // Bind the cube vbo to object cube VAO
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // coords
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

        // Light source cube vao
        GLuint light_source_cube_vao;
        glGenVertexArrays(1, &light_source_cube_vao);
        glBindVertexArray(light_source_cube_vao);

        // Bind the cube vbo to light source cube VAO
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // no need for normals in the light source
        glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

        // ====================
        //      SHADERS
        // ====================
        // object cube shader
        const char* object_cube_vertex_shader_src =
            "#version 330 core\n"
            "layout (location = 0) in vec3 vertex_position;\n"
            "layout (location = 1) in vec3 vertex_normal;\n"
            "uniform mat4 model_matrix;\n"
            "uniform mat4 view_matrix;\n"
            "uniform mat4 projection_matrix;\n"
            "out vec3 frag_position;\n"
            "out vec3 frag_normal;\n"
            "void main()\n"
            "{\n"
            "   frag_position = vec3(model_matrix * vec4(vertex_position, 1.0));\n"
            "   frag_normal = mat3(transpose(inverse(model_matrix))) * vertex_normal; \n"
            "   gl_Position = projection_matrix * view_matrix * vec4(frag_position, 1.0);\n"
            "}\0";

        // 9. Fragment shader setup
        const char* object_cube_fragment_shader_src =
            "#version 330 core\n"
            "in vec3 frag_position;\n"
            "in vec3 frag_normal;\n"
            "uniform vec3 objectColor;\n"
            "uniform vec3 camera_position;\n"
            "uniform vec3 lightPos;\n"
            "uniform vec3 lightColor;\n"
            "uniform float ambientStrength;\n"
            "out vec4 frag_color;\n"
            "void main()\n"
            "{\n"
            "   vec3 normalized_frag_normal = normalize(frag_normal);\n"//diffuse
            "   vec3 dir_vector_from_light_source_to_fragment = normalize(lightPos - frag_position);\n"
            "   float cosine_angle = dot(normalized_frag_normal, dir_vector_from_light_source_to_fragment);\n"
            "   cosine_angle = max(cosine_angle, 0.0);\n"
            "   vec3 diffuse = cosine_angle * lightColor;\n"
            "   float specularStrength = 0.5;\n" //specular
            "   vec3 dir_vec_from_camera_pos_to_fragment = normalize(camera_position - frag_position);\n"
            "   vec3 reflection_vec = reflect(-dir_vector_from_light_source_to_fragment, normalized_frag_normal);\n"
            "   float cosine_angle2 = dot(dir_vec_from_camera_pos_to_fragment, reflection_vec);\n"
            "   cosine_angle2 = max(cosine_angle2, 0.0);\n"
            "   int shininess = 32;\n"
            "   float specular_scalar = pow(cosine_angle2, shininess);\n"
            "   vec3 specular = specularStrength * specular_scalar * lightColor;\n"
            "   vec3 ambient = ambientStrength * lightColor;\n" //ambient
            "   vec3 resulting_color = (ambient + diffuse + specular) * objectColor;\n"
            "   frag_color = vec4(resulting_color, 1.0);\n"
            "}\0";

        auto object_cube_shader = ShaderProgram(object_cube_vertex_shader_src, ShaderSourceType::Text,
            object_cube_fragment_shader_src, ShaderSourceType::Text);

        // ligth source cube shader
        const char* light_source_cube_vertex_shader_src =
            "#version 330 core\n"
            "layout (location = 0) in vec3 vertex_position;\n"
            "uniform mat4 model_matrix;\n"
            "uniform mat4 view_matrix;\n"
            "uniform mat4 projection_matrix;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vertex_position, 1.0);\n"
            "}\0";

        const char* light_source_cube_fragment_shader_src =
            "#version 330 core\n"
            "out vec4 frag_color;\n"
            "void main()\n"
            "{\n"
            "   frag_color = vec4(1.0);\n"
            "}\0";

        auto light_source_cube_shader = ShaderProgram(light_source_cube_vertex_shader_src, ShaderSourceType::Text,
            light_source_cube_fragment_shader_src, ShaderSourceType::Text);


        // ====================
        //  TRANSFORMATION SETUP
        // ====================
        glm::mat4 identity_matrix = glm::mat4(1.0f);
        // ====================
        //  LIGHTING SETUP
        // ====================
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


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
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);// scene background color
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 view_matrix = camera.GetViewMatrix();
            glm::mat4 projection_matrix = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 model_matrix = identity_matrix;

            // render object cube
            object_cube_shader.use();
            object_cube_shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
            object_cube_shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
            object_cube_shader.setVec3("lightPos", lightPos);
            object_cube_shader.setVec3("camera_position", camera.Position);

            float ambientStrength = dynamic ? currentFrame : 0.1;
            object_cube_shader.setFloat("ambientStrength", ambientStrength);

            object_cube_shader.setMat4("model_matrix", model_matrix);
            object_cube_shader.setMat4("view_matrix", view_matrix);
            object_cube_shader.setMat4("projection_matrix", projection_matrix);

            glBindVertexArray(object_cube_vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // render light source cube
            light_source_cube_shader.use();
            light_source_cube_shader.setMat4("view_matrix", view_matrix);
            light_source_cube_shader.setMat4("projection_matrix", projection_matrix);

            model_matrix = identity_matrix;
            model_matrix = glm::translate(model_matrix, lightPos);
            model_matrix = glm::scale(model_matrix, glm::vec3(0.2f)); // a smaller cube
            light_source_cube_shader.setMat4("model_matrix", model_matrix);

            glBindVertexArray(light_source_cube_vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            /* Swap front and back buffers */
            /* update other events like input handling */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            /* put the stuff we've been drawing onto the display */
            glfwPollEvents();
        }

        glDeleteVertexArrays(1, &object_cube_vao);
        glDeleteVertexArrays(1, &light_source_cube_vao);
        glDeleteBuffers(1, &vbo);

        glfwTerminate();
        return 0;
    }

    int IntroAll(bool dynamic)
    {
        // Initialize the glfw & glew
        GLFWwindow* window = Utility::GLFW::start_glfw();
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        // tell GLFW to capture our mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
            // coords             // normals
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
        };

        // VBO for the cube
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Object cube VAO
        GLuint object_cube_vao;
        glGenVertexArrays(1, &object_cube_vao);
        glBindVertexArray(object_cube_vao);

        // Bind the cube vbo to object cube VAO
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // coords
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

        // Light source cube vao
        GLuint light_source_cube_vao;
        glGenVertexArrays(1, &light_source_cube_vao);
        glBindVertexArray(light_source_cube_vao);

        // Bind the cube vbo to light source cube VAO
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // no need for normals in the light source
        glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

        // ====================
        //      SHADERS
        // ====================
        auto object_cube_shader = ShaderProgram(
            "tutorials\\shaders\\lighting_intro_object_vs.glsl",
            "tutorials\\shaders\\lighting_intro_object_fs.glsl");

        auto light_source_cube_shader = ShaderProgram(
            "tutorials\\shaders\\lighting_intro_light_source_vs.glsl",
            "tutorials\\shaders\\lighting_intro_light_source_fs.glsl");


        // ====================
        //  TRANSFORMATION SETUP
        // ====================
        glm::mat4 identity_matrix = glm::mat4(1.0f);
        // ====================
        //  LIGHTING SETUP
        // ====================
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


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
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);// scene background color
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 view_matrix = camera.GetViewMatrix();
            glm::mat4 projection_matrix = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 model_matrix = identity_matrix;

            // render object cube
            object_cube_shader.use();
            object_cube_shader.setVec3("the_object.color", 1.0f, 0.5f, 0.31f);
            object_cube_shader.setFloat("the_object.specular_strength", 0.5f);
            object_cube_shader.setFloat("the_object.shininess", 32.0f);
            object_cube_shader.setVec3("light_source.color", 1.0f, 1.0f, 1.0f);
            object_cube_shader.setVec3("light_source.position", lightPos);
            object_cube_shader.setVec3("camera_position", camera.Position);

            float ambientStrength = dynamic ? currentFrame : 0.1;
            object_cube_shader.setFloat("the_object.ambient_strength", ambientStrength);

            object_cube_shader.setMat4("model_matrix", model_matrix);
            object_cube_shader.setMat4("view_matrix", view_matrix);
            object_cube_shader.setMat4("projection_matrix", projection_matrix);

            glBindVertexArray(object_cube_vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // render light source cube
            light_source_cube_shader.use();
            light_source_cube_shader.setMat4("view_matrix", view_matrix);
            light_source_cube_shader.setMat4("projection_matrix", projection_matrix);

            model_matrix = identity_matrix;
            model_matrix = glm::translate(model_matrix, lightPos);
            model_matrix = glm::scale(model_matrix, glm::vec3(0.2f)); // a smaller cube
            light_source_cube_shader.setMat4("model_matrix", model_matrix);

            glBindVertexArray(light_source_cube_vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            /* Swap front and back buffers */
            /* update other events like input handling */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            /* put the stuff we've been drawing onto the display */
            glfwPollEvents();
        }

        glDeleteVertexArrays(1, &object_cube_vao);
        glDeleteVertexArrays(1, &light_source_cube_vao);
        glDeleteBuffers(1, &vbo);

        glfwTerminate();
        return 0;
    }


}

namespace tutorials::lighting::material
{
    int Intro(bool dynamic)
    {
        // Initialize the glfw & glew
        GLFWwindow* window = Utility::GLFW::start_glfw();
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        // tell GLFW to capture our mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
            // coords             // normals
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
        };

        // VBO for the cube
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Object cube VAO
        GLuint object_cube_vao;
        glGenVertexArrays(1, &object_cube_vao);
        glBindVertexArray(object_cube_vao);

        // Bind the cube vbo to object cube VAO
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // coords
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

        // Light source cube vao
        GLuint light_source_cube_vao;
        glGenVertexArrays(1, &light_source_cube_vao);
        glBindVertexArray(light_source_cube_vao);

        // Bind the cube vbo to light source cube VAO
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // no need for normals in the light source
        glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

        // ====================
        //      SHADERS
        // ====================
        auto object_cube_shader = ShaderProgram(
            "tutorials\\shaders\\material_intro_object_vs.glsl",
            "tutorials\\shaders\\material_intro_object_fs.glsl");

        auto light_source_cube_shader = ShaderProgram(
            "tutorials\\shaders\\material_intro_light_source_vs.glsl",
            "tutorials\\shaders\\material_intro_light_source_fs.glsl");


        // ====================
        //  TRANSFORMATION SETUP
        // ====================
        glm::mat4 identity_matrix = glm::mat4(1.0f);
        // ====================
        //  LIGHTING SETUP
        // ====================
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


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
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);// scene background color
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 view_matrix = camera.GetViewMatrix();
            glm::mat4 projection_matrix = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 model_matrix = identity_matrix;

            // render object cube
            object_cube_shader.use();
            // vs
            object_cube_shader.setMat4("model_matrix", model_matrix);
            object_cube_shader.setMat4("view_matrix", view_matrix);
            object_cube_shader.setMat4("projection_matrix", projection_matrix);
            // fs
                // object
            float ambientStrength = dynamic ? currentFrame : 0.1;
            object_cube_shader.setVec3("the_object.ambient", 1.0f, 0.5f, 0.31f);
            object_cube_shader.setVec3("the_object.diffuse", 1.0f, 0.5f, 0.31f);
            object_cube_shader.setVec3("the_object.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
            object_cube_shader.setFloat("the_object.shininess", 32.0f);

                // light
            glm::vec3 lightColor;
            lightColor.x = (float)sin(glfwGetTime() * 2.0f);
            lightColor.y = (float)sin(glfwGetTime() * 0.7f);
            lightColor.z = (float)sin(glfwGetTime() * 1.3f);
            glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
            glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence

            object_cube_shader.setVec3("light_source.position", lightPos);
            object_cube_shader.setVec3("light_source.ambient", ambientColor);
            object_cube_shader.setVec3("light_source.diffuse", diffuseColor);
            object_cube_shader.setVec3("light_source.specular", 1.0f, 1.0f, 1.0f);

                // camera
            object_cube_shader.setVec3("camera_position", camera.Position);            

            glBindVertexArray(object_cube_vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // render light source cube
            light_source_cube_shader.use();
            light_source_cube_shader.setMat4("view_matrix", view_matrix);
            light_source_cube_shader.setMat4("projection_matrix", projection_matrix);

            model_matrix = identity_matrix;
            model_matrix = glm::translate(model_matrix, lightPos);
            model_matrix = glm::scale(model_matrix, glm::vec3(0.2f)); // a smaller cube
            light_source_cube_shader.setMat4("model_matrix", model_matrix);

            glBindVertexArray(light_source_cube_vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            /* Swap front and back buffers */
            /* update other events like input handling */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            /* put the stuff we've been drawing onto the display */
            glfwPollEvents();
        }

        glDeleteVertexArrays(1, &object_cube_vao);
        glDeleteVertexArrays(1, &light_source_cube_vao);
        glDeleteBuffers(1, &vbo);

        glfwTerminate();
        return 0;
    }

}

namespace tutorials::lighting::lighting_maps
{
    int DiffuseMap()
    {
        // Initialize the glfw & glew
        GLFWwindow* window = Utility::GLFW::start_glfw();
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        // tell GLFW to capture our mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
        };

        // VBO for the cube
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Object cube VAO
        GLuint object_cube_vao;
        glGenVertexArrays(1, &object_cube_vao);
        glBindVertexArray(object_cube_vao);

        // Bind the cube vbo to object cube VAO
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // coords
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        // texture
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

        // Light source cube vao
        GLuint light_source_cube_vao;
        glGenVertexArrays(1, &light_source_cube_vao);
        glBindVertexArray(light_source_cube_vao);

        // Bind the cube vbo to light source cube VAO
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // no need for normals in the light source
        glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

        // ====================
        //      TEXTURE
        // ====================
        GLuint container_diffused_texture = Utility::texture::load("resources\\container2.png");

        // ====================
        //      SHADERS
        // ====================
        auto object_cube_shader = ShaderProgram(
            "tutorials\\shaders\\lm_diffuse_map_object_vs.glsl",
            "tutorials\\shaders\\lm_diffuse_map_object_fs.glsl");

        auto light_source_cube_shader = ShaderProgram(
            "tutorials\\shaders\\lm_diffuse_map_light_source_vs.glsl",
            "tutorials\\shaders\\lm_diffuse_map_light_source_fs.glsl");

        // ====================
        //  TRANSFORMATION SETUP
        // ====================
        glm::mat4 identity_matrix = glm::mat4(1.0f);
        // ====================
        //  LIGHTING SETUP
        // ====================
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
        object_cube_shader.use(); 
        object_cube_shader.setInt("the_object.diffuse", 0);

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
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);// scene background color
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 view_matrix = camera.GetViewMatrix();
            glm::mat4 projection_matrix = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 model_matrix = identity_matrix;

            // render object cube
            object_cube_shader.use();
            // vs
            object_cube_shader.setMat4("model_matrix", model_matrix);
            object_cube_shader.setMat4("view_matrix", view_matrix);
            object_cube_shader.setMat4("projection_matrix", projection_matrix);
            // fs
                // object
            object_cube_shader.setVec3("the_object.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
            object_cube_shader.setFloat("the_object.shininess", 64.0f);

            // light
            //glm::vec3 lightColor;
            //lightColor.x = (float)sin(glfwGetTime() * 2.0f);
            //lightColor.y = (float)sin(glfwGetTime() * 0.7f);
            //lightColor.z = (float)sin(glfwGetTime() * 1.3f);
            //glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
            //glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence

            object_cube_shader.setVec3("light_source.position", lightPos);
            object_cube_shader.setVec3("light_source.ambient", 0.2f, 0.2f, 0.2f);
            object_cube_shader.setVec3("light_source.diffuse", 0.5f, 0.5f, 0.5f);
            object_cube_shader.setVec3("light_source.specular", 1.0f, 1.0f, 1.0f);

            // camera
            object_cube_shader.setVec3("camera_position", camera.Position);

            // bind diffuse map
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, container_diffused_texture);
            // render the cube
            glBindVertexArray(object_cube_vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // render light source cube
            light_source_cube_shader.use();
            light_source_cube_shader.setMat4("view_matrix", view_matrix);
            light_source_cube_shader.setMat4("projection_matrix", projection_matrix);

            model_matrix = identity_matrix;
            model_matrix = glm::translate(model_matrix, lightPos);
            model_matrix = glm::scale(model_matrix, glm::vec3(0.2f)); // a smaller cube
            light_source_cube_shader.setMat4("model_matrix", model_matrix);

            glBindVertexArray(light_source_cube_vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            /* Swap front and back buffers */
            /* update other events like input handling */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            /* put the stuff we've been drawing onto the display */
            glfwPollEvents();
        }

        glDeleteVertexArrays(1, &object_cube_vao);
        glDeleteVertexArrays(1, &light_source_cube_vao);
        glDeleteBuffers(1, &vbo);

        glfwTerminate();
        return 0;
    }

    int SpecularMap()
    {
        // Initialize the glfw & glew
        GLFWwindow* window = Utility::GLFW::start_glfw();
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        // tell GLFW to capture our mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
        };

        // VBO for the cube
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Object cube VAO
        GLuint object_cube_vao;
        glGenVertexArrays(1, &object_cube_vao);
        glBindVertexArray(object_cube_vao);

        // Bind the cube vbo to object cube VAO
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // coords
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        // texture
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

        // Light source cube vao
        GLuint light_source_cube_vao;
        glGenVertexArrays(1, &light_source_cube_vao);
        glBindVertexArray(light_source_cube_vao);

        // Bind the cube vbo to light source cube VAO
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // no need for normals in the light source
        glBindBuffer(GL_ARRAY_BUFFER, 0); // not required

        // ====================
        //      TEXTURE
        // ====================
        GLuint container_diffuse_texture = Utility::texture::load("resources\\container2.png");
        GLuint container_specular_texture = Utility::texture::load("resources\\container2_specular.png");

        // ====================
        //      SHADERS
        // ====================
        auto object_cube_shader = ShaderProgram(
            "tutorials\\shaders\\lm_specular_map_object_vs.glsl",
            "tutorials\\shaders\\lm_specular_map_object_fs.glsl");

        auto light_source_cube_shader = ShaderProgram(
            "tutorials\\shaders\\lm_specular_map_light_source_vs.glsl",
            "tutorials\\shaders\\lm_specular_map_light_source_fs.glsl");

        // ====================
        //  TRANSFORMATION SETUP
        // ====================
        glm::mat4 identity_matrix = glm::mat4(1.0f);
        // ====================
        //  LIGHTING SETUP
        // ====================
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
        object_cube_shader.use();
        object_cube_shader.setInt("the_object.diffuse", 0);
        object_cube_shader.setInt("the_object.specular", 1);

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
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);// scene background color
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 view_matrix = camera.GetViewMatrix();
            glm::mat4 projection_matrix = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 model_matrix = identity_matrix;

            // render object cube
            object_cube_shader.use();
            // vs
            object_cube_shader.setMat4("model_matrix", model_matrix);
            object_cube_shader.setMat4("view_matrix", view_matrix);
            object_cube_shader.setMat4("projection_matrix", projection_matrix);
            // fs
                // object
            object_cube_shader.setFloat("the_object.shininess", 64.0f);

            // light
            //glm::vec3 lightColor;
            //lightColor.x = (float)sin(glfwGetTime() * 2.0f);
            //lightColor.y = (float)sin(glfwGetTime() * 0.7f);
            //lightColor.z = (float)sin(glfwGetTime() * 1.3f);
            //glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
            //glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence

            object_cube_shader.setVec3("light_source.position", lightPos);
            object_cube_shader.setVec3("light_source.ambient", 0.2f, 0.2f, 0.2f);
            object_cube_shader.setVec3("light_source.diffuse", 0.5f, 0.5f, 0.5f);
            object_cube_shader.setVec3("light_source.specular", 1.0f, 1.0f, 1.0f);

            // camera
            object_cube_shader.setVec3("camera_position", camera.Position);

            // bind diffuse map
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, container_diffuse_texture);
            // bind specular map
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, container_specular_texture);
            // render the cube
            glBindVertexArray(object_cube_vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // render light source cube
            light_source_cube_shader.use();
            light_source_cube_shader.setMat4("view_matrix", view_matrix);
            light_source_cube_shader.setMat4("projection_matrix", projection_matrix);

            model_matrix = identity_matrix;
            model_matrix = glm::translate(model_matrix, lightPos);
            model_matrix = glm::scale(model_matrix, glm::vec3(0.2f)); // a smaller cube
            light_source_cube_shader.setMat4("model_matrix", model_matrix);

            glBindVertexArray(light_source_cube_vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            /* Swap front and back buffers */
            /* update other events like input handling */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            /* put the stuff we've been drawing onto the display */
            glfwPollEvents();
        }

        glDeleteVertexArrays(1, &object_cube_vao);
        glDeleteVertexArrays(1, &light_source_cube_vao);
        glDeleteBuffers(1, &vbo);

        glfwTerminate();
        return 0;
    }


}
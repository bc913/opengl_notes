#version 330 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

out vec3 frag_position;
out vec3 frag_normal;

void main()
{
   frag_position = vec3(model_matrix * vec4(vertex_position, 1.0));
   frag_normal = mat3(transpose(inverse(model_matrix))) * vertex_normal; 
   gl_Position = projection_matrix * view_matrix * vec4(frag_position, 1.0);
}
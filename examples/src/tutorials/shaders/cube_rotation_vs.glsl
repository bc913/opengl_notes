#version 410 core
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_colour;

out vec3 colour;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() 
{
	colour = vertex_colour;
	gl_Position = projection * view * model * vec4(vertex_position.x, vertex_position.y, vertex_position.z, 1.0);
}
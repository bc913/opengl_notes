#version 410 core
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_colour;
layout(location = 2) in vec2 vertex_textureCoord;

out vec3 colour;
out vec2 texCoord;

void main() {
	colour = vertex_colour;
	gl_Position = vec4(vertex_position.x, vertex_position.y, vertex_position.z, 1.0);
	texCoord = vec2(vertex_textureCoord.x, vertex_textureCoord.y);
}
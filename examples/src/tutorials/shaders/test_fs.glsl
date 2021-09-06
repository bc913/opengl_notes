#version 410 core

in vec3 colour;
in vec2 texCoord;

uniform vec4 customColor;
uniform bool useCustomColor;

// texture sampler
uniform bool useTexture;
uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 frag_colour;

void main() 
{
	if(useCustomColor)
		frag_colour = customColor;
	else if(useTexture)
	{
	//frag_colour = texture(texture1, texCoord);  
	frag_colour = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.5);
	}
	else
		frag_colour = vec4 (colour, 1.0);
		//
} 
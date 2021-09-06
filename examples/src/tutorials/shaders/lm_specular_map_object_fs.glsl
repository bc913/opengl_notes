#version 330 core

in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_texture_coords;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material the_object;
uniform Light light_source;
uniform vec3 camera_position;

out vec4 frag_color;

void main()
{
	// ambient
	// object's ambient color is equal to diffuse color for this example.
	vec3 ambient = light_source.ambient * texture(the_object.diffuse, frag_texture_coords).rgb;

	// diffuse
	vec3 normalized_frag_normal = normalize(frag_normal);
	vec3 dir_vector_from_light_source_to_fragment = normalize(light_source.position - frag_position);
	float cosine_angle = dot(normalized_frag_normal, dir_vector_from_light_source_to_fragment);
	cosine_angle = max(cosine_angle, 0.0);
	vec3 diffuse = light_source.diffuse * cosine_angle * texture(the_object.diffuse, frag_texture_coords).rgb;
	
	// specular
	vec3 dir_vec_from_camera_pos_to_fragment = normalize(camera_position - frag_position);
	vec3 reflection_vec = reflect(-dir_vector_from_light_source_to_fragment, normalized_frag_normal);
	float cosine_angle2 = dot(dir_vec_from_camera_pos_to_fragment, reflection_vec);
	cosine_angle2 = max(cosine_angle2, 0.0);
	float specular_scalar = pow(cosine_angle2, the_object.shininess);
	vec3 specular = light_source.specular * specular_scalar * texture(the_object.specular, frag_texture_coords).rgb;	

	// result
	vec3 resulting_color = (ambient + diffuse + specular);
	frag_color = vec4(resulting_color, 1.0);
}
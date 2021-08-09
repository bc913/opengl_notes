# Transformations

Transformations are the essential part of graphics programming and requires a bit of Linear Algebra and Math knowledge. Fortunately there are several APIs provide services for this purpose but it is always better to have knowledge.

One of the OpenGL tailored APIs is `GLM`. It is an header-only library and easy to use.

## Vectors
Vectors are represented with four components. This is called `homogeneous vector`. {x, y, z, w}
- The first three represent the x, y, and z directions 
- The last, w, is known as `homogeneous coordinate`. It can be considered as helper.
- To get the 3D vector from homogeneous vector, divide x, y and z components by w.

The main purpose of this coordinate is to make the translations/rotations of the vector possible using matrices (translation/rotatin matrix)

### Translations
Translation is the process of adding another vector on top of the original vector to return a new vector with a different position, thus moving the vector based on a translation vector.

Translation vectors can also be represented as translation matrices.
```cpp
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
        << "};
}
```
### Scaling
It is increasing/decreasing the magnitude(length) of the vector by multiplying/dividing a value or set of values.
- Uniform scaling: The direction of the vector remains same after the scaling operation which means that each component of the vector is scaled with the same value.
- Non-uniform scaling: The direction of the resulting vector is different than the source vector after scaling. This means at least one of the components are scaled with a different value than the others do.

Scaling is achieved with `scaling matrices`.

```cpp
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
        << "};
}
```

> Translations and Scaling can be combined but not with Rotation.


### Rotations
Rotations are happening around an `axis of rotation` with an angle of rotation. These infos can be populated in `rotation matrices`. Pure rotations (rotation around only one of the orthogonal axes: X, Y or Z) are easy to handle.

However it is getting complex for rotations requires the contribution of more than one orthogonal axes. There are several approaches and few of them are:
- Superposition of pure rotations
- Rotating around an arbitrary 3D unit axis.

These two are prone to a problem called `Gimbal Lock`. Calculating rotations using `quaternions` comes to the rescue with ease of computation.

```cpp
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
```





## How it works in OpenGL?
Since the rendering is handled in the server side, the required translation/rotation/scaling matrix information should be available to the shaders. This can be done by declaring the matrix variable with GLSL built-in (not GLM) type `mat4` (`mat3` and `mat2` is also available).

> If you need to perform a vertex related mat or translation/rotation/scaling operation, the required matrices should be available in the vertex shader. If it is color related operation, the required matrices should be available in fragment shader.

```cpp
#version 330 core
// Vertex shader - We'll do a translation/rotation/scaling operation
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_texture_position;
out vec3 color;
out vec2 textureCoord;
uniform mat4 transformation;
void main()
{
   gl_Position = transformation * vec4(vertex_position, 1.0f);
   color = vertex_color;
   textureCoord = vec2(vertex_texture_position.x, vertex_texture_position.y);
}
```
This is not enough. As you might have noticed, it is declared as `uniform` so it's an interface between the client and server side. The matrix data should somehow be passed from the client side.

- Translate only and once
```cpp
// ====================
//  TRANSFORMATION SETUP
// ====================
glm::mat4 identity_matrix = glm::mat4(1.0f);
// a translation matrix
glm::vec3 translation_vector = glm::vec3(0.5f, -0.5f, 0.0f);
// to translate 1 unit in x, 1 unit in y and 0 unit in z direction
glm::mat4 translation_matrix = glm::translate(identity_matrix, translation_vector);

// We need to pass this info to the shader so use the shader programme first
glUseProgram(shader_programme);
// Get the location of the transformation matrix in the shader
GLuint transformation_loc = glGetUniformLocation(shader_programme, "transformation");
// Set it's data
glUniformMatrix4fv(transformation_loc, 1, GL_FALSE, glm::value_ptr(translation_matrix));

```
- Translate and then rotate
```cpp
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

```

- Rotate dynamically (continuous) - using a timer
```cpp
// Before the main ui loop
// ====================
//  TRANSFORMATION SETUP
// ====================
glm::mat4 identity_matrix = glm::mat4(1.0f);
// a translation matrix
glm::vec3 translation_vector = glm::vec3(-0.25f, -0.25f, 0.0f);
// to translate 1 unit in x, 1 unit in y and 0 unit in z direction
glm::mat4 translation_matrix = glm::translate(identity_matrix, translation_vector);

// Let's carry over the dynamic part under the main ui loop

/* do other stuff*/

// ====================
//      MAIN UI LOOP
// ====================
glUseProgram(shader_programme);

// Rotate the container dynamically
// Rotation around X axis
glm::vec3 z_axis_unit_vec = glm::vec3(0.0f, 0.0f, 1.0f); // this part can also move out of the main ui loop but OK for now
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
```

> Looks like this step can be handled before or during rendering because it is one time static operation so I've preffered to place it before the main ui loop. If you have time dependent or dynamic (continuous) transformation, keeping it in the main ui loop is required.

## References
- [https://learnopengl.com/Getting-started/Transformations](https://learnopengl.com/Getting-started/Transformations)
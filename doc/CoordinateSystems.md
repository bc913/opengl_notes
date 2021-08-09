# Coordinate Systems

## Local Coordinate Systems
It is CS specific to the object. Each object has its own local coordinate system and it does NOT have any relation to the other objects or their coordinate systems.

## Global(world) coordinate system
This is the global coordinate system and it is used to define each object's position/direction relative to the global coordinate system's reference point. Each object is positioned in the global (world) space based on their locations in the global coordinate system. Assume you have multiple objects and all they are positioned at the location in the global coordinate system. We don't want them.

The transformation(mapping) from the object's local coordinate system to the global coordinate system is done `model matrix`. The model matrix is a transformation matrix that translates, scales and/or rotates your object to place it in the world at a location/orientation they belong to.

## View coordinate system
The view is usually referred as `camera` or `eye space` in OpenGL community. It is the user's view coordinate system which means what is seen from the camera's point of view. Therefore, it is the resulting space(or coordinate system) transforming the global coordinate system (space) to the camera's coordinate system (space).

The view space is thus the space as seen from the camera's point of view. This is usually accomplished with a combination of translations and rotations to translate/rotate the scene so that certain items are transformed to the front of the camera. These combined transformations are generally stored inside a view matrix that transforms world coordinates to view space. In the next chapter we'll extensively discuss how to create such a view matrix to simulate a camera.

## Clip coordinate system
### What is clip space?
At the end of each vertex shader run, OpenGL expects the coordinates to be within a specific range and any coordinate falls outside this range is `clipped` and those coordinates are discarded.

The remaining ones will end up as fragments visible on the screen. This is called `clip space`.
### Transformation from the view space to the clip space
The transformation from the view space to the clip space is achieved by `projection matrix`.
### Clip space coordinate system
The clip space coordinate system ranges from -1.0 to 1.0. However, during mapping (transformation) from view space to the clip space more fine calculations (or details) are required. Therefore, the projection matrix specifies a range of coordinates between -1000 and 1000 in each dimension.

The projection matrix transforms coordinates within (-1000, 1000) and any coordinate outside this range will be clipped. Those will not be mapped to (-1.0, 1.0) range.
> Even if one of the components (x,y,z) falls out-of-range, the coordinate will be clipped too.

### What does projection matrix do?
The projection box creates a `viewing box` which is called as `frustum` and each coordinate that ends up inside this frustum will be shown to the user. The total conversion(mapping) of these 3D coordinates to the 2D coordinates is called `projection`. It has two types:
- Orthographic projection
- Perspective projection

## What happens next?
Once all the vertices are transformed into the clip space, `perspective division` is performed where we divide the x, y and z components of the position vectors by the vector's homogeneous w component; perspective division is what transforms the 4D clip space coordinates to 3D normalized device coordinates. This step is performed automatically at the end of the vertex shader step. 

After this stage, the resulting coordinates are mapped to screen coordinates (using the settings of glViewport) and turned into fragments. 

## Where all of these happens at?
1. Vertex data comes into the vertex shader.
2. Transformations from local to global, from global to view and from view to clip space are performed within each vertex shader.
3. The resulting vertex should then assigned to vertex shader's built-in `gl_position` variable. This is the last step in the vertex shader.
>`gl_position` values are in clip coordinate system.
4. Then, `Perspective division` takes place implicitly to convert to transform the clip-space coordinates (output of the vertex shader) to `normalized device coordinates`.
5. Primitive assembly takes place.
6. (Assuming tesselation and geometry shader stages are skipped) the `clipping` stage takes the control and out-of-range coordinates are clipped here.
7. Now, it is time for rasterization stage to form the pixel-sized fragments.
8. Then fragment shader takes the control to color or texture the fragments.

## How does it work?
Since the transformations from the local to clip space are carried out in the vertex shader side, the corresponding transformation matrices should be defined in the `vertex shader` and they have to be declared as `uniform` to get the access(set data) from the client side.
- Vertex shader
```cpp
#version 330 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vertex_texture_position;
out vec2 textureCoord;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
void main()
{
   gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vertex_position, 1.0);
   textureCoord = vec2(vertex_texture_position.x, vertex_texture_position.y);
}
```
- The client has to access view, model and projection matrices within the vertex shader defined as `uniform`. This can be achieved, as previously mentioned, with `glGetUniformLocation` and `glUniformMatrix4fv`.
```cpp
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
```
> If it is a static (one time) operation, accessing/modifying `uniform` variables from the client side can be made out of the main UI loop. If it is a dynamic(continuous) operation, keep this within the main UI loop.

## References
- [https://learnopengl.com/Getting-started/Coordinate-Systems](https://learnopengl.com/Getting-started/Coordinate-Systems)
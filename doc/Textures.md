# Textures
A `Texture` can be considered as a 2D (1D or 3D is also possible) image to be mapped onto an visual object.

Textures are different type of quantities and in order to see them they should be somehow mapped onto the OpenGL objects. The mapping is achieved by relating the texture's coordinates with the vertex coordinates.

**3 defining characteristics of a texture:**
1. Texture type
2. Texture size
3. Texture format

> They can also be used as data storage like buffer objects.

## Sampling
Each texture coordinate actually corresponds to a texture color (or some other feature) but it has to be retrieved somehow. The proccess of fetching a texture feature(s) value at a given position using the texture coordinates is called `Sampling`. GLSL controls much of the process of sampling, but there are many values associated with the texture object that can affect this as well.

## Texture coordinate (system)
Each vertex of the object to be textured, should have an associated corresponding `texture coordinate`. This texture coordinate specifies what part of the texture (image) to sample from.

Texture coordinates range from zero (0) to one(1) for each axis (x-y). The most left-bottom point is (0, 0) and the most right-top point is (1,1).

## Mapping from texture coordinate (system) to the vertex coordinate (system)

TODO: The graphics from the notebook


## Texture Filtering
Some definitions has to be made before proceeding.
- **Texture Pixel(texel)**: This is the pixel for the texture. It is not the same as OpenGL pixel. They are different.

Textures has its own resolution and so do own texture pixels. Texture pixels do not have to match with the texture coordinates and so do vertex coordinates. We have to map these texture pixels onto the texture coordinates and then onto the vertex coordinate coordinate system to get an OpenGL pixel.

It works as follows:
image --> texture pixel --> filter it --> map onto texture coordinates --> map onto the vertex coordinates

Filtering is basically calculates the to be mapped pixel based on the position of the closest texture coordinate.
### How it works?
Let's assume you have 4 neighboring (as square) pixels and there is only one texture coordinate fall into this square. 

As the final visual, this region corresponds to the square will be represented as single pixel at the end since there is only one texture coordinates there.

The representing pixel will be selected(or calculated) based on the developer's choice. There are several options there:

- Nearest Neighbor (GL_NEAREST): The distance between the texture coordinate and the neighboring texels centers will be calculated and the closest texel will be picked.

- (Bi)linear (GL_LINEAR): Based on the distances of the neightboring texels, an interpolated value will be calculated and the result will be reflected as pixel.

## Texture Wrapping
If a vertex does not fit anywhere within the range of (0,0) - (1,1) `texture coordinate system`, then the texture would not totally fit the objects that consists of multiple vertices.

Texture Wrapping provides options to fill out-of-range vertex coordinates. It will specify how to sample the texture for out-of-range values.

- `GL_REPEAT`: The integer part of the coordinate will be ignored and a repeating pattern is formed.
- `GL_MIRRORED_REPEAT`: The texture will also be repeated, but it will be mirrored when the integer part of the coordinate is odd.
- `GL_CLAMP_TO_EDGE`: The coordinate will simply be clamped between 0 and 1.
- `GL_CLAMP_TO_BORDER`: The coordinates that fall outside the range will be given a specified border color.

## The aliasing effect
During animations and/or zoom in/out operations, you start to see some aliasing effect occurring. These are caused by sampling fewer than all of the texels; the choice of which texels are sampled changes between different frames of the animation. Even with linear filtering (see below), artifacts will appear as the camera zooms out.

## MipMaps
When a texture is directly applied to a surface, how many pixels of that texture (commonly called "texels") are used depends on the angle at which that surface is rendered and the distance from the viewer. The objects closer to the viewer requires more detail for texturing in comparison to the objects far from the viewer. Images with higher resolution for the closer objects and lower resolution for the far objects are more appriopriate.

However, if we try to represent the textures for the far objects with the higher resolution, this will an huge amount of computation to calculate the correct pixel (filtering) and results with a performance penalty on the server side.

Mipmaps comes to the rescue here. They are pre-filtered (pre-shrunk) versions of a specific texture so the most appropriate version of the texture from mipmaps will be selected and be mapped onto the object based on their distance from the camera(viewer). 

It has the following benefits:
- Improved usage quality: Eliminates the aliasing effect caused by oversampling the texture.
- Increased performance: Increases the cache efficiency. The lower bitmap resolution mipmapsfit more easily to the cache.
- Since the full size textures is not needed as often, reducing the bandwith usage and increasing app perfomance will benefit.

Cons:
- Mipmapping requires some extra computation and texture storage area; however, when it's not used, textures that are mapped onto smaller objects might shimmer and flash as the objects move. 

Each mipmap is half the size (rounded down) of the previous one in the chain, continuing until all dimensions are 1. So a 64x16 2D texture can have 6 mip-maps: 32x8, 16x4, 8x2, 4x1, 2x1, and 1x1.

### Base level
The base level of a mipmap chain is the largest one in size. It is also the one that defines the full size of the texture. OpenGL numbers this mipmap level as 0; the next largest mipmap level is 1, and so on.

## Texture Objects
Textures in OpenGL are OpenGL Objects, and they follow the standard conventions of such. So they have the standard glGenTextures, glBindTexture, as you would expect.

<p align="center">
  <img width="386" height="313" src="images/texture/texture_object_anatomy.jpg">
</p>

A texture object consists of three main components:
1. Texture storage
2. Parameters (Sampling and Texture)

### Texture Storage
It is the part of the Texture Objects that contains the actual pixel data.

- The storage can contain one or more images of a certain dimensionality. Each texture type has a specific arrangement of images in its storage.

- Since the texture storage can store multiple images, there should be a way of identifying a specific texture in the storage. Each image in a texture storage can uniquely be identified by the following numbers:

1. The mipmap `Level` (it contains the image) value for textures which have mipmap.
2. For `Array Type Textures`, the array `Layer` that contains the image.
3. For `CubeMap Type Textures`, the face within that array layer and mipmap level. Note that for cubemap array textures, the layer and face are combined into layer-faces.

Therefore, a texture data can be thought of as a three-dimensional array of images. First index is the Mipmap level, the second one is the array layer and the third one is the cube map face.

Details for texture storage can be found [here](https://www.khronos.org/opengl/wiki/Texture_Storage). Some noticable features will be presented here.

- Image Size: It is importannt to know the size of the individual images within a texture storage. The images that have the same mipmap level in a texture storage, will have the same size and that size depends on the `base mipmap level`: level 0. The size of `Level 0` images defines the texture's effective size.

> The number of array layers and cube map faces do not change with the mipmap level. If a texture has 3 array layers, every mipmap will have 3 array layers. This is important to remember when allocating texture storage and uploading pixel data.

- Kinds of Storage: There three (3) kinds of storage:
1. Mutable: Once it is allocated for a specific texture, it can then be altered to store another type of texture with a different size.

- It can NOT be shared. It is only bound to a single texture object.
- Mutable storage calls are made with `gl*TexImage*()` functions and they are capable of both allocating memory and placing (transferring) the pixel data into that memory.
2. Immutable: Allocates all of the images for the texture `at once` (single call) and can NOT be chamged once it is allocated. This is like `const pointer`. The pointer can NOT point any other location but the contents it points to can be modified.

Allocating an immutable storage requires the texture binding to its target first and then calling `glTexStorage*()` function. Each function only works with specific set of targets. This is like a `malloc` call, you have memory location allocated but no content.

- It can be shared between texture objects so they are referring to the same memory location. (It is like `std::shared_ptr`)

- To share the previously created immutable storate, use `glTextureView` function.


3. Buffer: Only `Buffer Textures` can use buffer storage where the texture gets its storage from a `Buffer Object`. `Buffer Textures` can NOT use mutable or immutable storage.

> Recommendation: If your implementation supports creating textures with immutable storage, you should use it wherever possible. It will save you from innumerable mistakes and headaches.

#### Access to the mutable/immutable storage contents
Once the storage has been defined with one of the above functions, the contents of the storage (the actual pixel data) can be modified and access via various functions.

- Automatic mipmap generation
- Pixel upload
- Compressed pixel upload
- Texture clear
- Texture copy
- Framebuffer copy
- Framebuffer rendertarget
- Invalidation
- Pixel Download

### Parameters (Sampling and Texture)
Sampling and texture parameters control many aspects of how the texture functions. Both parameters are set with the following functions.

```cpp
void glTexParameter[if]( GLenum target​, GLenum pname​, T param​);
void glTexParameter[if]v( GLenum target​, GLenum pname​, T *params​ );
void glTexParameterI[i ui]v( GLenum target​, GLenum pname​, T *params​ );
```
> Sampling parameters of the texture objects are also shared with `Sampler Objects` so both `Texture objects` and `Sampler objects` have them. If a texture is used with a sampler object, all of the parameters from the sampler object override those set by the texture object.

## Sampler Objects
Shaders can access texture coordinate data through vertex attribute pointers using `location` syntax but how about `sampling parameters`? There should be a mechanism or a data structure to access that data from shaders and `Sampler Objects` come to the rescue.

A Sampler Object is an OpenGL Object that stores the sampling parameters for a Texture access inside of a shader. This is an OpenGL object but GLSL has a special syntax to access this object.
```cpp
uniform sampler<type> <sampler_name>;
```
It is declared as `uniform` because as you might remember, `uniform`s are the interface between client-server and so client side data (sampling parameters) can only be available in the server side thanks to the `uniform` syntax.

### GLSL Sampler Variables
The Sampler objects are represented with `sampler variables` and various types. The sampler types are used in GLSL to represent a texture of a particular kind. Therefore, sampler types represent textures.
> RULE: Sampler variables SHOULD be declared as `uniform`.

## How does it work?
Since the setup and usage of textures are complex and comprehensive enough, it'd be better to explain it with the code.

0. Provide the texture coordinates for the corresponding vertices and populate the corresponding vertex attribute (pointers) to be used later in the shader stage.
1. Load the image from disk or memory location specified. OpenGL does not provide any functionality for this step but this is required to map the texture. Some third-party APIs can be used for this purpose.
2. Generate the texture object using common OpenGL object generation syntax. `glGenTextures()`
3. Bind the generated texture against the desired target (a type of textures). Once it is bound to a target(type), it can NOT be bound to another target.
4. Set the [sampling parameters](https://www.khronos.org/opengl/wiki/Sampler_Object#Sampling_parameters) to the bound texture object.
5. Allocate the storage (Mutable, Immutable or Buffer storage).
6. (Optional) Once the storage is allocated, the contents of the storage can be altered via some available functions.
7. Once you're done manipulating the storage contents,  you can now free the loaded image raw data because it is already loaded to the OpenGL context so no need to carry that load.
At this point, some third-party APIs can be used.
8. Now, we have all the texture data in the client side and we need to let server side know about the data. The texture coordinates are part of the vertex data and vertex data can only be passed to the server side through `Vertex Shader` so appripriate definitions should be made in the vertex shader side.
```cpp
#version 330 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_texture_position;

out vec3 color;
out vec2 textureCoord;

void main()
{
	gl_Position = vec4(vertex_position.x, vertex_position.y, vertex_position.z, 1.0);
	color = vertex_color;
	textureCoord = vec2(vertex_texture_position.x, vertex_texture_position.y);
}
```
After accessing the texture coordinates in vertex shader, this data should also be transferred to the fragment shader so fragment shader should accept the texture coordinate data as the input variable.

9. Now, it is time for the fragment shader. The texture coordinate data is already fetched from the vertex shader and we also need sampling parameters data. This can be achieved with the `[Sampler Objects](#Sampler-Objects)`.

```cpp
#version 330 core

in vec3 color;
in vec2 texCoord;

out vec4 frag_color;
uniform sampler2D texture1; // texture sampler

void main()
{
   frag_color = texture(texture1, texCoord);
}
```

The fragment color can now be set using texture coordinate and texture sampling data with the GLSL's built-in method called `texture`.

10. Generate and use the shaders as previoulsy described.
11. All we need to do is drawing the texture on the view. Make sure the texture is bound before attempting to draw.

The main code looks like this:
```cpp
// ====================
// VBO - VAO GENERATION
// ====================
// 0. Vertex data for positions, colors and texture coordinates
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

// Steps 10a-h
/*
Shader setup
...
*/


// ====================
//      MAIN UI LOOP
// ====================
while (!glfwWindowShouldClose(window))
{
    /*
    Some UI setup here
    */

    // 11. Make the texture drawn
    glUseProgram(shader_programme);
    glBindTexture(GL_TEXTURE_2D, texture_obj);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    /*
    Some UI stuff here too
    */
}

```

## Texture (Image) Units
There are two reasons to bind a texture object:
1. To modify object's storage or it's parameters
2. To render something with it.

Changing the texture stored state (storage or parameters) can be done with simple `glBindTexture` call. However, rendering with texture is a bit more complicated.

The way a program is associated with textures is somewhat unintuitive. The mapping is done with the rendering context. A texture can be bound to one or more locations. These locations within the rendering context are called `texture image units`. OpenGL contexts have a maximum number of texture image units, queriable from the constant `GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS`.

In other words, each texture object (and so the corresponding sampler object) is associated with a `texture image unit`. Actually, a `glBindTexture` call binds the texture object (and so the related sampler object) to a texture image unit. 

Each texture image unit supports bindings to all texture types and multiple texture objecst(and so the corresponding sampler objects) can be bound to the same texture image unit.

> IMPORTANT RULE: **DO NOT** bind two different texture (or sampler) type to the same texture unit. The rendering will fail. Bind each type of texture (sampler) to a different texture image unit.

### How does it work?
1. Define each texture with same syntax described above.
2. Since we have multiple textures, the fragment shader will have multiple `sampler` variables to access sampler objects of each texture.
```cpp
#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}
```
3. Also, we somehow have to tell the fragment shader which sampler belongs to which texture. This can be done from the client side.
```cpp
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

// NOTE: Make sure you activate the correct texture image unit for the related texture during rendering.
```
4. To make each texture drawn, we first have to activate the `texture image unit` for the texture it is associated.
5. Any subsequent call i.e. `glBindTexture`, `glTexImage2D()`, `glTexParameterf()` etc will now manipulate the texture bound to the current (active) texture image unit.
6. Repeat steps 4-5 for each texture.

```cpp
// Activate the texture image units and bind the associated textures
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, texture_obj);
glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_2D, texture_obj2);
```
7. Render as usual.

## References
- https://learnopengl.com/Getting-started/Textures
- https://www.khronos.org/opengl/wiki/Texture
- https://open.gl/textures
- https://www.haroldserrano.com/blog/a-brief-talk-about-opengl-textures
- http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/
- http://www.c-jump.com/bcc/common/Talk3/OpenGL/Wk07_texture/Wk07_texture.html
- https://www.glprogramming.com/red/chapter09.html
- https://users.cs.jmu.edu/bernstdh/web/common/lectures/summary_opengl-texture-mapping.php
- https://www.gamasutra.com/view/feature/3361/understanding_and_using_opengl_.php?print=1





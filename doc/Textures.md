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

1. Load the image from disk or memory location specified. OpenGL does not provide any functionality for this step but this is required to map the texture. Some third-party APIs can be used for this purpose.

```cpp
// 1. Load the texture image from file

// 2. Generate the texture object
GLuint texture_obj;
glGenTextures(1, &texture_obj);

// 3. Bind it to a specific target
// For this

```

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





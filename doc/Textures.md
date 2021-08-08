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

## Texture Units

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




